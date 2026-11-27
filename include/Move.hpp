#pragma once
#include "Piece.hpp"

struct Move{
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;

    PieceType promotion;

    Move(int fr, int fc, int tr, int tc, PieceType p = PieceType::None) : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc), promotion(p){
    }

    // Check if player's move is legal
    bool operator==(const Move& other) const{
        return fromRow == other.fromRow &&
               fromCol == other.fromCol &&
               toRow == other.toRow &&
               toCol == other.toCol &&
               promotion == other.promotion;
    }
};