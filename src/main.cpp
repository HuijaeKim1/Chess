#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

#include "Board.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"

// Check whether a specific move exists in the vector
bool containsMove(const std::vector<Move>& moves, int fr, int fc, int tr, int tc) {
    Move target(fr, fc, tr, tc);

    return std::find(moves.begin(), moves.end(), target) != moves.end();
}

// Print all generated moves in chess coordinates
void printMoves(const std::vector<Move>& moves) {
    for (const Move& move : moves) {

        // Convert internal coordinates to chess notation
        char fromFile = 'a' + move.fromCol;
        int fromRank = 8 - move.fromRow;

        char toFile = 'a' + move.toCol;
        int toRank = 8 - move.toRow;

        std::cout << fromFile << fromRank << " -> " << toFile << toRank << '\n';
    }
}

// Test 1: Standard starting position
void testStartingPosition() {
    Board board;

    board.print();

    std::vector<Move> moves =
        MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 20);

    // Verify all 16 pawn moves
    for (int col = 0; col < 8; col++) {
        assert(containsMove(moves, 6, col, 5, col));
        assert(containsMove(moves, 6, col, 4, col));
    }

    // Verify all four knight moves
    assert(containsMove(moves, 7, 1, 5, 0));
    assert(containsMove(moves, 7, 1, 5, 2));
    assert(containsMove(moves, 7, 6, 5, 5));
    assert(containsMove(moves, 7, 6, 5, 7));

    std::cout << "\nTotal possible moves: " << moves.size() << "\n\n";

    printMoves(moves);

    std::cout << "\n[PASS] Starting position\n";
}

// Test 2: Pawn movement
void testPawn() {
    Board board;
    board.clear();

    // Place a white pawn on e2
    board.setPiece(6, 4, Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Single and double forward movement
    assert(moves.size() == 2);
    assert(containsMove(moves, 6, 4, 5, 4));
    assert(containsMove(moves, 6, 4, 4, 4));

    // Block the second square
    board.setPiece(4, 4, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 1);
    assert(!containsMove(moves, 6, 4, 4, 4));

    // Block the first square
    board.setPiece(5, 4, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    // Place an enemy piece diagonally
    board.setPiece(5, 3, Piece(PieceType::Knight, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Pawn can capture even when forward movement is blocked
    assert(moves.size() == 1);
    assert(containsMove(moves, 6, 4, 5, 3));

    // Replace enemy knight with enemy king
    board.setPiece(5, 3, Piece(PieceType::King, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    // Test a pawn at the edge of the board
    board.clear();

    board.setPiece(4, 0, Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 1);
    assert(containsMove(moves, 4, 0, 3, 0));

    std::cout << "[PASS] Pawn movement\n";
}

// Test 3: Knight movement
void testKnight() {
    Board board;
    board.clear();

    // Knight in the center
    board.setPiece(4, 4, Piece(PieceType::Knight, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 8);

    // Knight in the corner
    board.clear();

    board.setPiece(0, 0, Piece(PieceType::Knight, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 2);
    assert(containsMove(moves, 0, 0, 1, 2));
    assert(containsMove(moves, 0, 0, 2, 1));

    // Enemy capture
    board.setPiece(1, 2, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 0, 0, 1, 2));

    // Friendly piece cannot be captured
    board.setPiece(1, 2, Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 0, 0, 1, 2));

    std::cout << "[PASS] Knight movement\n";
}

// Test 4: Bishop movement
void testBishop() {
    Board board;
    board.clear();

    // Bishop in the center
    board.setPiece(4, 4, Piece(PieceType::Bishop, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 13);

    // Place an enemy pawn on the bishop's diagonal
    board.setPiece(2, 2, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Can move to the empty square
    assert(containsMove(moves, 4, 4, 3, 3));

    // Can capture the enemy pawn
    assert(containsMove(moves, 4, 4, 2, 2));

    // Cannot move beyond the pawn
    assert(!containsMove(moves, 4, 4, 1, 1));
    assert(!containsMove(moves, 4, 4, 0, 0));

    // Replace enemy pawn with friendly pawn
    board.setPiece(2, 2, Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 2, 2));
    assert(!containsMove(moves, 4, 4, 1, 1));

    std::cout << "[PASS] Bishop movement\n";
}

// Test 5: Rook movement
void testRook() {
    Board board;
    board.clear();

    // Rook in the center
    board.setPiece(4, 4, Piece(PieceType::Rook, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 14);

    // Place an enemy piece two squares above the rook
    board.setPiece(2, 4, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 4, 4, 3, 4));
    assert(containsMove(moves, 4, 4, 2, 4));

    // Cannot move beyond the captured piece
    assert(!containsMove(moves, 4, 4, 1, 4));
    assert(!containsMove(moves, 4, 4, 0, 4));

    // Rook cannot move diagonally
    assert(!containsMove(moves, 4, 4, 3, 3));

    // Replace the enemy piece with a friendly piece
    board.setPiece(2, 4, Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 2, 4));

    std::cout << "[PASS] Rook movement\n";
}

// Test 6: Queen movement
void testQueen() {
    Board board;
    board.clear();

    // Queen in the center
    board.setPiece(4, 4, Piece(PieceType::Queen, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    // 14 rook moves + 13 bishop moves
    assert(moves.size() == 27);

    // Place an enemy piece horizontally
    board.setPiece(4, 6, Piece(PieceType::Pawn, Color::Black));

    // Place another enemy piece diagonally
    board.setPiece(2, 2, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Horizontal capture
    assert(containsMove(moves, 4, 4, 4, 6));
    assert(!containsMove(moves, 4, 4, 4, 7));

    // Diagonal capture
    assert(containsMove(moves, 4, 4, 2, 2));
    assert(!containsMove(moves, 4, 4, 1, 1));

    std::cout << "[PASS] Queen movement\n";
}

// Test 7: King movement
void testKing() {
    Board board;
    board.clear();

    // King in the center
    board.setPiece(4, 4, Piece(PieceType::King, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 8);

    // King cannot move two squares
    assert(!containsMove(moves, 4, 4, 2, 4));

    // Enemy capture
    board.setPiece(3, 4, Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 4, 4, 3, 4));

    // Cannot capture a friendly piece
    board.setPiece(3, 4, Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 3, 4));

    // Cannot capture an enemy king
    board.setPiece(3, 4, Piece(PieceType::King, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 3, 4));

    // King in the corner
    board.clear();

    board.setPiece(0, 0, Piece(PieceType::King, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 3);

    std::cout << "[PASS] King movement\n";
}

// Test 8: Empty board
void testEmptyBoard() {
    Board board;
    board.clear();

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    std::cout << "[PASS] Empty board\n";
}

// Test 9: Verify opponent pieces are ignored
void testOpponentPieces() {
    Board board;
    board.clear();

    // White is currently to move
    // Place only black pieces on the board
    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::Black));

    board.setPiece(0, 1, Piece(PieceType::Knight, Color::Black));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    std::cout << "[PASS] Opponent piece filtering\n";
}

int main() {
    std::cout << "================================\n";
    std::cout << "   CHESS ENGINE TEST SUITE\n";
    std::cout << "================================\n\n";

    testStartingPosition();
    testPawn();
    testKnight();
    testBishop();
    testRook();
    testQueen();
    testKing();
    testEmptyBoard();
    testOpponentPieces();

    std::cout << "\n================================\n";
    std::cout << " ALL 9 TEST GROUPS PASSED!\n";
    std::cout << "================================\n";

    return 0;
}