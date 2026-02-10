#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

void Sheet::CheckValidPosition(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid Position Exception!");
    }
}

void Sheet::SetCell(Position pos, std::string text) {
    CheckValidPosition(pos);

    auto it = sheet_.find(pos);

    if (it == sheet_.end()) {
        sheet_[pos] = std::make_unique<Cell>(*this, pos);
    }

    auto& cell = sheet_[pos];

    try {
        cell->Set(std::move(text));
    }
    catch (...) {
        cell->Set(""s);
        throw;
    }
}

const Cell* Sheet::GetCell(Position pos) const {
    CheckValidPosition(pos);
    auto it = sheet_.find(pos);
    return it != sheet_.end() ? it->second.get() : nullptr;
}

Cell* Sheet::GetCell(Position pos) {
    CheckValidPosition(pos);
    auto it = sheet_.find(pos);
    return it != sheet_.end() ? it->second.get() : nullptr;
}

void Sheet::ClearCell(Position pos) {
    CheckValidPosition(pos);
    sheet_.erase(pos);
}

Size Sheet::GetPrintableSize() const {
    Size size;
    for (const auto& [pos, cell] : sheet_) {
        if (cell != nullptr && !cell->GetText().empty()) {
            size.rows = std::max(size.rows, pos.row + 1);
            size.cols = std::max(size.cols, pos.col + 1);
        }
    }
    return size;
}

void Sheet::PrintValues(std::ostream& output) const {
    Size size = GetPrintableSize();

    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) {
                output << '\t';
            }

            Position pos{ row, col };

            if (auto it = sheet_.find(pos); it != sheet_.end() && it->second != nullptr) {
                auto value = it->second->GetValue();

                if (std::holds_alternative<std::string>(value)) {
                    output << std::get<std::string>(value);
                }
                else if (std::holds_alternative<double>(value)) {
                    output << std::get<double>(value);
                }
                else {
                    output << std::get<FormulaError>(value);
                }
            }
        }
        output << '\n';
    }
}

void Sheet::PrintTexts(std::ostream& output) const {
    Size size = GetPrintableSize();

    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col > 0) {
                output << '\t';
            }

            Position pos{ row, col };

            if (auto it = sheet_.find(pos); it != sheet_.end() && it->second != nullptr) {
                output << it->second->GetText();
            }
        }
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}