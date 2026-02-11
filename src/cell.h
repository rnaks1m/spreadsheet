#pragma once

#include "common.h"
#include "formula.h"

#include <functional>
#include <unordered_set>
#include <optional>

class Sheet;

class Cell : public CellInterface {
public:
    explicit Cell(Sheet& sheet, Position pos);
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

    void AddDependent(Position pos);
    void RemoveDependent(Position pos);
    void InvalidateCache();
    bool IsReferenced() const;

private:
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    Sheet& sheet_;
    Position position_;
    std::unique_ptr<Impl> impl_;
    std::unordered_set<Position, PositionHasher> dependents_;
    mutable std::optional<Value> cache_;

    bool CheckCyclicDependency(const std::vector<Position>& new_refs) const;
    void UpdateDependencies(const std::vector<Position>& old_deps, const std::vector<Position>& new_deps);
};