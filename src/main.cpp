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
bool containsMove(const std::vector<Move>& moves,
                  int fr, int fc, int tr, int tc) {

    Move target(fr, fc, tr, tc);

    return std::find(moves.begin(), moves.end(), target) != moves.end();
}

// Check the stored en passant coordinates
bool hasEnPassantTarget(const Board& board, int row, int col) {
    return board.getEnPassantRow() == row && board.getEnPassantCol() == col;
}

// Create a position where White can capture en passant
Board createWhiteEnPassantPosition() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 4, Piece(PieceType::King, Color::Black));

    // White pawn on e5
    board.setPiece(3, 4, Piece(PieceType::Pawn, Color::White));

    // Black pawn on d7
    board.setPiece(1, 3, Piece(PieceType::Pawn, Color::Black));

    // Move White's king to switch turns
    board.makeMove(Move(7, 4, 7, 5));

    // Black moves d7 to d5
    board.makeMove(Move(1, 3, 3, 3));

    return board;
}

// Create a position where Black can capture en passant
Board createBlackEnPassantPosition() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 4, Piece(PieceType::King, Color::Black));

    // Black pawn on d4
    board.setPiece(4, 3, Piece(PieceType::Pawn, Color::Black));

    // White pawn on e2
    board.setPiece(6, 4, Piece(PieceType::Pawn, Color::White));

    // White moves e2 to e4
    board.makeMove(Move(6, 4, 4, 4));

    return board;
}

// Test initial en passant state
void testInitialState() {
    Board board;

    check(hasEnPassantTarget(board, -1, -1),
          "Starting position has no en passant target");
}

// Test double-square pawn movement
void testDoubleSquareMovement() {
    Board board;

    // White moves e2 to e4
    board.makeMove(Move(6, 4, 4, 4));

    check(hasEnPassantTarget(board, 5, 4),
          "White double-square movement creates correct target");

    // Black moves d7 to d5
    board.makeMove(Move(1, 3, 3, 3));

    check(hasEnPassantTarget(board, 2, 3),
          "Black double-square movement creates correct target");
}

// Test white en passant generation
void testWhiteEnPassantGeneration() {
    Board board = createWhiteEnPassantPosition();

    auto pseudoLegalMoves = MoveGenerator::generatePseudoLegalMoves(board);
    auto legalMoves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(pseudoLegalMoves, 3, 4, 2, 3),
          "White en passant appears in pseudo-legal moves");

    check(containsMove(legalMoves, 3, 4, 2, 3),
          "White en passant appears in legal moves");
}

// Test black en passant generation
void testBlackEnPassantGeneration() {
    Board board = createBlackEnPassantPosition();

    auto pseudoLegalMoves = MoveGenerator::generatePseudoLegalMoves(board);
    auto legalMoves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(pseudoLegalMoves, 4, 3, 5, 4),
          "Black en passant appears in pseudo-legal moves");

    check(containsMove(legalMoves, 4, 3, 5, 4),
          "Black en passant appears in legal moves");
}

// Test white en passant execution
void testWhiteEnPassantExecution() {
    Board board = createWhiteEnPassantPosition();

    // White captures e5 to d6
    board.makeMove(Move(3, 4, 2, 3));

    Piece pawn = board.getPiece(2, 3);

    check(pawn.type == PieceType::Pawn && pawn.color == Color::White,
          "White pawn reaches correct en passant destination");

    check(board.getPiece(3, 3).isEmpty(),
          "Captured black pawn is removed");

    check(board.getPiece(3, 4).isEmpty(),
          "White pawn's original square is cleared");

    check(board.getCurrentTurn() == Color::Black,
          "Turn switches to Black after en passant");

    check(hasEnPassantTarget(board, -1, -1),
          "En passant target expires after capture");
}

// Test black en passant execution
void testBlackEnPassantExecution() {
    Board board = createBlackEnPassantPosition();

    // Black captures d4 to e3
    board.makeMove(Move(4, 3, 5, 4));

    Piece pawn = board.getPiece(5, 4);

    check(pawn.type == PieceType::Pawn && pawn.color == Color::Black,
          "Black pawn reaches correct en passant destination");

    check(board.getPiece(4, 4).isEmpty(),
          "Captured white pawn is removed");

    check(board.getPiece(4, 3).isEmpty(),
          "Black pawn's original square is cleared");

    check(board.getCurrentTurn() == Color::White,
          "Turn switches to White after en passant");

    check(hasEnPassantTarget(board, -1, -1),
          "Black en passant capture clears target");
}

// Test en passant capture in the opposite direction
void testOppositeDirection() {
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 4, Piece(PieceType::King, Color::Black));

    // White pawn on c5
    board.setPiece(3, 2, Piece(PieceType::Pawn, Color::White));

    // Black pawn on d7
    board.setPiece(1, 3, Piece(PieceType::Pawn, Color::Black));

    board.makeMove(Move(7, 4, 7, 5));
    board.makeMove(Move(1, 3, 3, 3));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(moves, 3, 2, 2, 3),
          "White can capture en passant to the right");

    board.makeMove(Move(3, 2, 2, 3));

    Piece pawn = board.getPiece(2, 3);

    check(pawn.type == PieceType::Pawn && pawn.color == Color::White &&
          board.getPiece(3, 3).isEmpty() && board.getPiece(3, 2).isEmpty(),
          "Right-side en passant executes correctly");
}

// Test en passant expiration
void testEnPassantExpiration() {
    Board board = createWhiteEnPassantPosition();

    // White moves king instead of capturing
    board.makeMove(Move(7, 5, 7, 4));

    check(hasEnPassantTarget(board, -1, -1),
          "En passant expires after an unrelated move");

    // Black moves king to return the turn to White
    board.makeMove(Move(0, 4, 0, 5));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(!containsMove(moves, 3, 4, 2, 3),
          "Expired en passant capture is no longer generated");
}

// Test replacing an old en passant target
void testTargetReplacement() {
    Board board;

    // White moves e2 to e4
    board.makeMove(Move(6, 4, 4, 4));

    // Black moves c7 to c5
    board.makeMove(Move(1, 2, 3, 2));

    check(hasEnPassantTarget(board, 2, 2),
          "New double-square move replaces previous target");
}

// Test missing adjacent pawn
void testMissingAdjacentPawn() {
    Board board = createWhiteEnPassantPosition();

    // Remove the black pawn
    board.setPiece(3, 3, Piece());

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    check(!containsMove(moves, 3, 4, 2, 3),
          "En passant rejected when adjacent pawn is missing");
}

// Test adjacent friendly pawn
void testFriendlyAdjacentPawn() {
    Board board = createWhiteEnPassantPosition();

    // Replace black pawn with a white pawn
    board.setPiece(3, 3, Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    check(!containsMove(moves, 3, 4, 2, 3),
          "En passant cannot capture a friendly pawn");
}

// Test adjacent enemy piece that is not a pawn
void testAdjacentNonPawn() {
    Board board = createWhiteEnPassantPosition();

    // Replace black pawn with a black knight
    board.setPiece(3, 3, Piece(PieceType::Knight, Color::Black));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    check(!containsMove(moves, 3, 4, 2, 3),
          "En passant cannot capture a non-pawn piece");
}

// Test en passant exposing the king to check
void testEnPassantKingSafety() {
    Board board;
    board.clear();

    // White king on h5
    board.setPiece(3, 7, Piece(PieceType::King, Color::White));

    // Black king on h8
    board.setPiece(0, 7, Piece(PieceType::King, Color::Black));

    // Black rook on a5
    board.setPiece(3, 0, Piece(PieceType::Rook, Color::Black));

    // White pawn on e5
    board.setPiece(3, 4, Piece(PieceType::Pawn, Color::White));

    // Black pawn on d7
    board.setPiece(1, 3, Piece(PieceType::Pawn, Color::Black));

    // Switch to Black by moving a temporary white knight
    board.setPiece(7, 1, Piece(PieceType::Knight, Color::White));
    board.makeMove(Move(7, 1, 5, 2));

    // Black moves d7 to d5
    board.makeMove(Move(1, 3, 3, 3));

    auto pseudoLegalMoves = MoveGenerator::generatePseudoLegalMoves(board);
    auto legalMoves = MoveGenerator::generateLegalMoves(board);

    check(containsMove(pseudoLegalMoves, 3, 4, 2, 3),
          "Pinned en passant capture appears in pseudo-legal moves");

    check(!containsMove(legalMoves, 3, 4, 2, 3),
          "En passant exposing own king is rejected");
}

// Test clear and reset
void testClearAndReset() {
    Board board;

    board.makeMove(Move(6, 4, 4, 4));
    board.clear();

    check(hasEnPassantTarget(board, -1, -1),
          "clear() removes en passant target");

    board.reset();
    board.makeMove(Move(6, 4, 4, 4));
    board.reset();

    check(hasEnPassantTarget(board, -1, -1),
          "reset() removes en passant target");
}

// Test board copying
void testBoardCopy() {
    Board board = createWhiteEnPassantPosition();

    Board copy = board;

    check(hasEnPassantTarget(copy, 2, 3),
          "Board copy preserves en passant state");

    // Execute en passant only on the copied board
    copy.makeMove(Move(3, 4, 2, 3));

    check(hasEnPassantTarget(board, 2, 3) &&
          board.getPiece(3, 4).type == PieceType::Pawn &&
          board.getPiece(3, 3).type == PieceType::Pawn,
          "Executing en passant on copy does not modify original board");
}

// Test starting position
void testStartingPosition() {
    Board board;

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 20,
          "Starting position still generates 20 legal moves");
}

int main() {
    testInitialState();
    testDoubleSquareMovement();
    testWhiteEnPassantGeneration();
    testBlackEnPassantGeneration();
    testWhiteEnPassantExecution();
    testBlackEnPassantExecution();
    testOppositeDirection();
    testEnPassantExpiration();
    testTargetReplacement();
    testMissingAdjacentPawn();
    testFriendlyAdjacentPawn();
    testAdjacentNonPawn();
    testEnPassantKingSafety();
    testClearAndReset();
    testBoardCopy();
    testStartingPosition();

    std::cout << "\nTest Results\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total: " << testNumber << "\n";

    return failed == 0 ? 0 : 1;
}