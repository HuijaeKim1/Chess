#pragma once

enum class PieceType {
    None,
    Pawn,
    Knight, 
    Bishop,
    Rook,
    King,
    Queen
};

enum class Color {
    None,
    Black, 
    White
};

struct Piece {
    PieceType type;
    Color color;

    Piece() : type(PieceType::None), color(Color::None) {}

    Piece(PieceType p, Color c) : type(p), color(c){
    }

    bool isEmpty() const{
        return type == PieceType::None;
    }
};