#include <iostream>
#include <vector>
#include <algorithm>
#include "Board.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"

int passed = 0;
int failed = 0;
int testNumber = 0;

// Print the result of each test
void check(bool condition, const char* description) {
    testNumber++;

    if (condition) {
        std::cout << testNumber << ". PASS: " << description << "\n";
        passed++;
    }
    else {
        std::cout << testNumber << ". FAIL: " << description << "\n";
        failed++;
    }
}

// Check whether a specific move exists
bool containsMove(const std::vector<Move>& moves, int fr, int fc, int tr, int tc) {
    Move target(fr, fc, tr, tc);
    return std::find(moves.begin(), moves.end(), target) != moves.end();
}

// Test the standard starting position
void testStartingPosition() {
    Board board;

    check(!MoveGenerator::isCheckmate(board), "Starting position is not checkmate");

    check(!MoveGenerator::isStalemate(board), "Starting position is not stalemate");

    check(MoveGenerator::generateLegalMoves(board).size() == 20, "Starting position has 20 legal moves");
}

// Test check with available escape squares
void testCheckWithEscape() {
    Board board;
    board.clear();

    // White king on e1
    board.setPiece(7, 4, Piece(PieceType::King, Color::White));

    // Black king on a8
    board.setPiece(0, 0, Piece(PieceType::King, Color::Black));

    // Black rook on e8
    board.setPiece(0, 4, Piece(PieceType::Rook, Color::Black));

    check(MoveGenerator::isInCheck(board, Color::White), "White king is in check");

    check(!MoveGenerator::isCheckmate(board), "Check with an escape square is not checkmate");

    check(!MoveGenerator::isStalemate(board), "Player in check is not stalemated");
}

// Test king capturing an attacking piece
void testKingCapture() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 0, Piece(PieceType::King, Color::Black));

    // Unprotected black rook on e2
    board.setPiece(6, 4, Piece(PieceType::Rook, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(moves, 7, 4, 6, 4), "King can escape check by capturing attacker");

    check(!MoveGenerator::isCheckmate(board), "Capturable attacker does not cause checkmate");
}

// Test blocking a check with another piece
void testBlockingCheck() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 0, Piece(PieceType::King, Color::Black));
    board.setPiece(0, 4, Piece(PieceType::Rook, Color::Black));

    // White bishop on d2
    board.setPiece(6, 3, Piece(PieceType::Bishop, Color::White));

    auto moves = MoveGenerator::generateLegalMoves(board);

    // Bishop moves from d2 to e3 to block the rook
    check(containsMove(moves, 6, 3, 5, 4), "Bishop can block a rook check");

    check(!MoveGenerator::isCheckmate(board), "Blockable check is not checkmate");
}

// Test another piece capturing the attacker
void testCaptureAttacker() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 0, Piece(PieceType::King, Color::Black));

    // Black rook on e8
    board.setPiece(0, 4, Piece(PieceType::Rook, Color::Black));

    // White rook on h8
    board.setPiece(0, 7, Piece(PieceType::Rook, Color::White));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(moves, 0, 7, 0, 4), "White rook can capture the checking piece");

    check(!MoveGenerator::isCheckmate(board), "Capturing attacker prevents checkmate");
}

// Test checkmate against Black
void testBlackCheckmate() {
    Board board;
    board.clear();

    // Black king on h8
    board.setPiece(0, 7, Piece(PieceType::King, Color::Black));

    // White king on g6
    board.setPiece(2, 6, Piece(PieceType::King, Color::White));

    // White queen moves from f7 to g7
    board.setPiece(1, 5, Piece(PieceType::Queen, Color::White));
    board.makeMove(Move(1, 5, 1, 6));

    check(MoveGenerator::isInCheck(board, Color::Black), "Black king is in check");

    check(MoveGenerator::generateLegalMoves(board).empty(), "Checkmated Black has no legal moves");

    check(MoveGenerator::isCheckmate(board), "Black checkmate correctly detected");

    check(!MoveGenerator::isStalemate(board), "Black checkmate is not stalemate");
}

// Test checkmate against White
void testWhiteCheckmate() {
    Board board;
    board.clear();

    // White king on h1
    board.setPiece(7, 7, Piece(PieceType::King, Color::White));

    // Black king on g3
    board.setPiece(5, 6, Piece(PieceType::King, Color::Black));

    // Black queen on g2
    board.setPiece(6, 6, Piece(PieceType::Queen, Color::Black));

    check(MoveGenerator::isInCheck(board, Color::White), "White king is in check");

    check(MoveGenerator::generateLegalMoves(board).empty(), "Checkmated White has no legal moves");

    check(MoveGenerator::isCheckmate(board), "White checkmate correctly detected");

    check(!MoveGenerator::isStalemate(board), "White checkmate is not stalemate");
}

// Test Fool's Mate from the standard starting position
void testFoolsMate() {
    Board board;

    // 1. f3
    board.makeMove(Move(6, 5, 5, 5));

    // 1... e5
    board.makeMove(Move(1, 4, 3, 4));

    // 2. g4
    board.makeMove(Move(6, 6, 4, 6));

    // 2... Qh4#
    auto moves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(moves, 0, 3, 4, 7), "Fool's Mate finishing move is legal");

    board.makeMove(Move(0, 3, 4, 7));

    check(MoveGenerator::isCheckmate(board), "Fool's Mate correctly detected");

    check(!MoveGenerator::isStalemate(board), "Fool's Mate is not stalemate");
}

// Test stalemate against Black
void testBlackStalemate() {
    Board board;
    board.clear();

    // Black king on h8
    board.setPiece(0, 7, Piece(PieceType::King, Color::Black));

    // White king on g6
    board.setPiece(2, 6, Piece(PieceType::King, Color::White));

    // White queen moves from e7 to f7
    board.setPiece(1, 4, Piece(PieceType::Queen, Color::White));
    board.makeMove(Move(1, 4, 1, 5));

    check(!MoveGenerator::isInCheck(board, Color::Black), "Stalemated Black is not in check");

    check(MoveGenerator::generateLegalMoves(board).empty(), "Stalemated Black has no legal moves");

    check(MoveGenerator::isStalemate(board) && !MoveGenerator::isCheckmate(board),
          "Black stalemate correctly detected");
}

// Test stalemate against White
void testWhiteStalemate() {
    Board board;
    board.clear();

    // White king on h1
    board.setPiece(7, 7, Piece(PieceType::King, Color::White));

    // Black king on g3
    board.setPiece(5, 6, Piece(PieceType::King, Color::Black));

    // Black queen on f2
    board.setPiece(6, 5, Piece(PieceType::Queen, Color::Black));

    check(!MoveGenerator::isInCheck(board, Color::White), "Stalemated White is not in check");

    check(MoveGenerator::generateLegalMoves(board).empty(), "Stalemated White has no legal moves");

    check(MoveGenerator::isStalemate(board) && !MoveGenerator::isCheckmate(board),
          "White stalemate correctly detected");
}

// Test position where the king cannot move but another piece can
void testOtherPieceCanMove() {
    Board board;
    board.clear();

    // White king has no legal moves
    board.setPiece(7, 7, Piece(PieceType::King, Color::White));
    board.setPiece(5, 6, Piece(PieceType::King, Color::Black));
    board.setPiece(6, 5, Piece(PieceType::Queen, Color::Black));

    // Give White a pawn that can move
    board.setPiece(6, 0, Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generateLegalMoves(board);

    bool kingCanMove = false;

    for (const auto& move : moves) {
        if (move.fromRow == 7 && move.fromCol == 7) {
            kingCanMove = true;
        }
    }

    check(!kingCanMove && !moves.empty() && !MoveGenerator::isStalemate(board),
          "No king moves does not mean stalemate if another piece can move");
}

// Test that game-state detection does not modify the board
void testBoardPreservation() {
    Board board;

    // Create an en passant target
    board.makeMove(Move(6, 4, 4, 4));

    Board original = board;

    MoveGenerator::isCheckmate(board);
    MoveGenerator::isStalemate(board);

    bool unchanged = true;

    // Compare all 64 squares
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece before = original.getPiece(row, col);
            Piece after = board.getPiece(row, col);

            if (before.type != after.type || before.color != after.color) {
                unchanged = false;
            }
        }
    }

    // Compare turn and en passant information
    if (board.getCurrentTurn() != original.getCurrentTurn() ||
        board.getEnPassantRow() != original.getEnPassantRow() ||
        board.getEnPassantCol() != original.getEnPassantCol()) {
        unchanged = false;
    }

    // Compare castling rights for both colors
    for (Color color : {Color::White, Color::Black}) {
        if (board.canCastleKingside(color) != original.canCastleKingside(color) ||
            board.canCastleQueenside(color) != original.canCastleQueenside(color)) {
            unchanged = false;
        }
    }

    check(unchanged, "Game-state detection preserves the original board");
}

int main() {
    testStartingPosition();
    testCheckWithEscape();
    testKingCapture();
    testBlockingCheck();
    testCaptureAttacker();
    testBlackCheckmate();
    testWhiteCheckmate();
    testFoolsMate();
    testBlackStalemate();
    testWhiteStalemate();
    testOtherPieceCanMove();
    testBoardPreservation();

    std::cout << "\nTest Results\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total: " << testNumber << "\n";

    return failed == 0 ? 0 : 1;
}