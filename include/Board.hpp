#pragma once

#include <array>
#include "Piece.hpp"
#include "Move.hpp"

class Board {
private:
    // Store all 64 squares of the chessboard
    std::array<Piece, 64> squares;

    // Keep track of whose turn it is
    Color currentTurn = Color::White;

    void switchTurn();

    // Track castling rights for both players
    bool whiteCanCastleKingside = true;
    bool whiteCanCastleQueenside = true;

    bool blackCanCastleKingside = true;
    bool blackCanCastleQueenside = true;

    int enPassantRow = -1;
    int enPassantCol = -1;

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

    void makeMove(const Move& move);

    bool canCastleKingside(Color color) const;
    bool canCastleQueenside(Color color) const;

    int getEnPassantRow() const;
    int getEnPassantCol() const;
};
