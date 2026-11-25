#include "Board.hpp"
#include <cassert>
#include <iostream>

int main() {
    Board board;

    // Test starting position
    assert(board.getPiece(0, 0).type == PieceType::Rook);
    assert(board.getPiece(0, 0).color == Color::Black);
    assert(board.getPiece(7, 3).type == PieceType::Queen);
    assert(board.getPiece(7, 4).type == PieceType::King);
    assert(board.getPiece(6, 4).type == PieceType::Pawn);
    assert(board.getPiece(4, 4).isEmpty());

    // Verify White moves first
    assert(board.getCurrentTurn() == Color::White);

    // Test placing a piece on an empty square
    board.setPiece(4, 4, Piece(PieceType::Queen, Color::White));
    assert(board.getPiece(4, 4).type == PieceType::Queen);

    // Test clearing the entire board
    board.clear();

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            assert(board.getPiece(row, col).isEmpty());
        }
    }

    // Restore the original starting position
    board.reset();

    assert(board.getPiece(4, 4).isEmpty());
    assert(board.getPiece(7, 4).type == PieceType::King);
    assert(board.getCurrentTurn() == Color::White);

    board.print();

    std::cout << "\nAll board tests passed!\n";

    return 0;
}