#pragma once

#include <array>
#include "Piece.hpp"

class Board {
private:
    // Store all 64 squares of the chessboard
    std::array<Piece, 64> squares;

    // Keep track of whose turn it is
    Color currentTurn = Color::White;

public:
    Board();

    // Reset the board to the standard starting position
    void reset();

    // Remove all pieces from the board
    void clear();

    Piece getPiece(int row, int col) const;

    void setPiece(int row, int col, Piece piece);

    Color getCurrentTurn() const;

    void print() const;
};
