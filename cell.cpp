#include "cell.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include "stack"
#include <string>

#include "sheet.h"

class Cell::Impl {
public:
    virtual ~Impl() = default;
    virtual Value GetValue(const Sheet& sheet) const = 0;
    virtual std::string GetText() const = 0;
    virtual std::vector<Position> GetReferencedCells() const { return {}; }
};

class Cell::EmptyImpl : public Impl {
public:
    Value GetValue(const Sheet& sheet) const override {
        return "";
    }
    std::string GetText() const override {
        return "";
    }
};

class Cell::TextImpl : public Impl {
public:
    explicit TextImpl(std::string text) : text_(std::move(text)) { }

    Value GetValue(const Sheet& sheet) const override {
        if (!text_.empty() && text_[0] == ESCAPE_SIGN) {
            return text_.substr(1);
        }
        return text_;
    }

    std::string GetText() const override {
        return text_;
    }
private:
    std::string text_;
};

class Cell::FormulaImpl : public Impl {
public:
    explicit FormulaImpl(std::string expression) : formula_(ParseFormula(expression.substr(1))) {
        text_ = FORMULA_SIGN + formula_->GetExpression();
    }

    Value GetValue(const Sheet& sheet) const override {
        auto result = formula_->Evaluate(sheet);

        if (std::holds_alternative<double>(result)) {
            return std::get<double>(result);
        }
        return std::get<FormulaError>(result);
    }

    std::string GetText() const override {
        return text_;
    }

    std::vector<Position> GetReferencedCells() const override {
        return formula_->GetReferencedCells();
    }

private:
    std::string text_;
    std::unique_ptr<FormulaInterface> formula_;
};

Cell::Cell(Sheet& sheet, Position pos) : sheet_(sheet), position_(pos), impl_(std::make_unique<EmptyImpl>()) { }

Cell::~Cell() = default;

bool Cell::CheckCyclicDependency(const std::vector<Position>& new_refs) const {
    std::unordered_set<Position, PositionHasher> visited;
    std::stack<Position> stack;

    for (const auto& pos : new_refs) {
        stack.push(pos);
    }

    while (!stack.empty()) {
        Position current = stack.top();
        stack.pop();

        if (current == position_) {
            return true;
        }

        if (visited.insert(current).second) {
            if (const auto* cell = sheet_.GetCell(current)) {
                for (const auto& ref : cell->GetReferencedCells()) {
                    stack.push(ref);
                }
            }
        }
    }
    return false;
}

void Cell::UpdateDependencies(const std::vector<Position>& old_refs, const std::vector<Position>& new_refs) {
    for (const auto& pos : old_refs) {
        if (auto* cell = sheet_.GetCell(pos)) {
            cell->RemoveDependent(position_);
        }
    }
    for (const auto& pos : new_refs) {
        if (auto* cell = sheet_.GetCell(pos)) {
            cell->AddDependent(position_);
        }
    }
}

void Cell::Set(std::string text) {
    if (text.empty()) {
        impl_ = std::make_unique<EmptyImpl>();
    }
    else if (text.size() > 1 && text[0] == FORMULA_SIGN) {
        auto old_refs = GetReferencedCells();
        auto new_impl = std::make_unique<FormulaImpl>(std::move(text));
        auto new_refs = new_impl->GetReferencedCells();

        if (CheckCyclicDependency(new_refs)) {
            throw CircularDependencyException("Circular dependency detected");
        }

        impl_ = std::move(new_impl);
        UpdateDependencies(old_refs, new_refs);
    }
    else {
        impl_ = std::make_unique<TextImpl>(std::move(text));
    }
    InvalidateCache();
}

void Cell::Clear() {
    Set("");
}

Cell::Value Cell::GetValue() const {
    if (!cache_) {
        cache_ = impl_->GetValue(sheet_);
    }
    return *cache_;
}

std::string Cell::GetText() const {
    return impl_->GetText();
}

std::vector<Position> Cell::GetReferencedCells() const {
    return impl_->GetReferencedCells();
}

void Cell::AddDependent(Position pos) {
    dependents_.insert(pos);
}

void Cell::RemoveDependent(Position pos) {
    dependents_.erase(pos);
}

void Cell::InvalidateCache() {
    cache_.reset();
    for (Position dependent : dependents_) {
        if (auto* cell = sheet_.GetCell(dependent)) {
            static_cast<Cell*>(cell)->InvalidateCache();
        }
    }
}

bool Cell::IsReferenced() const {
    return !dependents_.empty();
}