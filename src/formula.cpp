#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#ARITHM!";
}

namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:
    explicit Formula(std::string expression) try : ast_(ParseFormulaAST(expression)) { }
    catch (...) {
        throw FormulaException("Formula Exception!");
    }

    Value Evaluate(const SheetInterface& sheet) const override {
        try {
            auto cell_lookup = [&sheet](Position pos) {
                const auto* cell = sheet.GetCell(pos);
                if (!cell) return 0.0;
                auto val = cell->GetValue();
                if (std::holds_alternative<double>(val)) return std::get<double>(val);
                throw FormulaError(FormulaError::Category::Arithmetic);
            };
            return ast_.Execute(cell_lookup);
        }
        catch (...) {
            return FormulaError(FormulaError::Category::Value);
        }
    }

    std::string GetExpression() const override {
        std::ostringstream out;
        ast_.PrintFormula(out);
        return out.str();
    }

    std::vector<Position> GetReferencedCells() const {
        auto cell_from_ast = ast_.GetCells();
        std::vector<Position> cells(cell_from_ast.begin(), cell_from_ast.end());
        return cells;
    }

private:
    FormulaAST ast_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}