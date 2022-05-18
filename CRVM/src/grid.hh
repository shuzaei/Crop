#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template <typename Int> class Grid {

#define Count(list, color) (std::count(list.begin(), list.end(), color))
#define RotateL(list, num) (std::rotate(list.begin(), list.begin() + num, list.end()))
#define RotateR(list, num) (std::rotate(list.begin(), list.end() - num, list.end()))

    using Grid_ = std::vector<std::vector<Int>>;

    private:
    Grid_ grid_;

    Int rows_;
    Int cols_;

    public:
    Grid() {}
    Grid(Int rows, Int cols) : rows_(rows), cols_(cols), grid_(rows, std::vector<Int>(cols)) {}
    Grid(Int rows, Int cols, Grid_ grid) : rows_(rows), cols_(cols), grid_(grid) {}
    ~Grid() {}

    void Input(const Int &rows, const Int &cols, std::istream &in = std::cin) {
        rows_ = rows;
        cols_ = cols;
        grid_.resize(rows_);
        for (auto &row : grid_) {
            row.resize(cols_);
            for (auto &col : row) in >> col;
        }
    }

    void Reset(Int rows, Int cols) {
        rows_ = rows, cols_ = cols, grid_ = Grid_(rows_, std::vector<Int>(cols_));
    }
    void Reset(Int rows, Int cols, Grid_ grid) { rows_ = rows, cols_ = cols, grid_ = grid; }

    void Set(const Int &row, const Int &col, const Int &color) { grid_[row][col] = color; }
    Int Get(const Int &row, const Int &col) const { return grid_[row][col]; }

    void Print() {
        for (auto &row : grid_) {
            for (Int i = 0; i < cols_; ++i) {
                std::cout << row[i];
                if (i != cols_ - 1) std::cout << ' ';
            }
            std::cout << '\n';
        }
        std::flush(std::cout);
    }

    void RotateDown(const Int &color) {
        Grid_ grid_copy(rows_, std::vector<Int>(cols_, color));
        for (Int i = 0; i < rows_; ++i) {
            for (Int j = 0; j < cols_; ++j) grid_copy[i][j] = grid_[j][i];
            RotateR(grid_copy[i], Count(grid_copy[i], color));
        }

        for (Int i = 0; i < rows_; ++i) {
            for (Int j = 0; j < cols_; ++j) grid_[i][j] = grid_copy[j][i];
        }
    }

    void RotateRight(const Int &color) {
        for (auto &row : grid_) { RotateR(row, Count(row, color)); }
    }

    void RotateUp(const Int &color) {
        Grid_ grid_copy(rows_, std::vector<Int>(cols_, color));
        for (Int i = 0; i < rows_; ++i) {
            for (Int j = 0; j < cols_; ++j) grid_copy[i][j] = grid_[j][i];
            RotateL(grid_copy[i], Count(grid_copy[i], color));
        }

        for (Int i = 0; i < rows_; ++i) {
            for (Int j = 0; j < cols_; ++j) grid_[i][j] = grid_copy[j][i];
        }
    }

    void RotateLeft(const Int &color) {
        for (auto &row : grid_) { RotateL(row, Count(row, color)); }
    }

    bool Test(const Int &x, const Int &y, const Int &color) { return grid_[x][y] == color; }

#undef Count
#undef RotateL
#undef RotateR
};
