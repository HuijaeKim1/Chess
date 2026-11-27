#include <iostream>
#include <vector>
#include "Board.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"

int passed = 0;
int failed = 0;
int testNumber = 0;

// Print the result of each test
void check(bool condition, const char* description){
    testNumber++;

    if (condition){
        std::cout << testNumber << ". PASS: " << description << "\n";
        passed++;
    }
    else{
        std::cout << testNumber << ". FAIL: " << description << "\n";
        failed++;
    }
}

// Create a board containing only two kings
Board createKingsOnlyBoard(){
    Board board;
    board.clear();

    board.setPiece(7, 4, Piece(PieceType::King, Color::White));
    board.setPiece(0, 4, Piece(PieceType::King, Color::Black));

    return board;
}

// Execute a specified number of quiet half-moves
// Knights repeatedly move between their original and destination squares
void executeQuietMoves(Board& board, int count){
    const std::vector<Move> sequence = {
        Move(7, 6, 5, 5),
        Move(0, 6, 2, 5),
        Move(5, 5, 7, 6),
        Move(2, 5, 0, 6)
    };

    // Continue the sequence based on the current counter
    for (int i = 0; i < count; i++){
        int index = board.getHalfMoveCounter() % 4;
        board.makeMove(sequence[index]);
    }
}

// Test initial counter and draw conditions
void testInitialState(){
    Board board;

    check(board.getHalfMoveCounter() == 0, "Initial half-move counter is zero");

    check(!MoveGenerator::isFiftyMoveRule(board), "50-move rule is false initially");

    check(!MoveGenerator::isSeventyFiveMoveRule(board), "75-move rule is false initially");
}

// Test quiet moves
void testQuietMoves(){
    Board board;

    // White knight moves g1 to f3
    board.makeMove(Move(7, 6, 5, 5));

    check(board.getHalfMoveCounter() == 1, "White knight movement increments counter");

    // Black knight moves g8 to f6
    board.makeMove(Move(0, 6, 2, 5));

    check(board.getHalfMoveCounter() == 2, "Black knight movement increments counter");

    // Return both knights to their original squares
    board.makeMove(Move(5, 5, 7, 6));
    board.makeMove(Move(2, 5, 0, 6));

    check(board.getHalfMoveCounter() == 4, "Multiple quiet moves accumulate correctly");
}

// Test single-square pawn movement
void testSinglePawnMovement(){
    Board board;

    executeQuietMoves(board, 4);

    // White pawn moves e2 to e3
    board.makeMove(Move(6, 4, 5, 4));

    check(board.getHalfMoveCounter() == 0, "Single-square pawn movement resets counter");
}

// Test double-square pawn movement
void testDoublePawnMovement(){
    Board board;

    executeQuietMoves(board, 4);

    // White pawn moves e2 to e4
    board.makeMove(Move(6, 4, 4, 4));

    check(board.getHalfMoveCounter() == 0, "Double-square pawn movement resets counter");
}

// Test pawn capturing an enemy piece
void testPawnCapture(){
    Board board = createKingsOnlyBoard();

    // White pawn on e4 and black knight on d5
    board.setPiece(4, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(3, 3, Piece(PieceType::Knight, Color::Black));

    // Execute two quiet king moves
    board.makeMove(Move(7, 4, 7, 5));
    board.makeMove(Move(0, 4, 0, 5));

    // White pawn captures black knight
    board.makeMove(Move(4, 4, 3, 3));

    check(board.getHalfMoveCounter() == 0, "Pawn capture resets counter");
}

// Test non-pawn capturing a pawn
void testCapturePawn(){
    Board board = createKingsOnlyBoard();

    board.setPiece(4, 4, Piece(PieceType::Rook, Color::White));
    board.setPiece(4, 0, Piece(PieceType::Pawn, Color::Black));

    board.makeMove(Move(7, 4, 7, 5));
    board.makeMove(Move(0, 4, 0, 5));

    // White rook captures black pawn
    board.makeMove(Move(4, 4, 4, 0));

    check(board.getHalfMoveCounter() == 0, "Non-pawn capturing pawn resets counter");
}

// Test non-pawn capturing another non-pawn
void testNonPawnCapture(){
    Board board = createKingsOnlyBoard();

    board.setPiece(4, 4, Piece(PieceType::Rook, Color::White));
    board.setPiece(4, 0, Piece(PieceType::Knight, Color::Black));

    board.makeMove(Move(7, 4, 7, 5));
    board.makeMove(Move(0, 4, 0, 5));

    // White rook captures black knight
    board.makeMove(Move(4, 4, 4, 0));

    check(board.getHalfMoveCounter() == 0, "Non-pawn capturing non-pawn resets counter");
}

// Test pawn promotion
void testPromotion(){
    Board board = createKingsOnlyBoard();

    // White pawn on a7
    board.setPiece(1, 0, Piece(PieceType::Pawn, Color::White));

    board.makeMove(Move(7, 4, 7, 5));
    board.makeMove(Move(0, 4, 0, 5));

    // Promote pawn to queen
    board.makeMove(Move(1, 0, 0, 0, PieceType::Queen));

    check(board.getHalfMoveCounter() == 0, "Pawn promotion resets counter");

    check(board.getPiece(0, 0).type == PieceType::Queen, "Pawn promotion still executes correctly");
}

// Test en passant
void testEnPassant(){
    Board board = createKingsOnlyBoard();

    // White pawn on e5 and black pawn on d7
    board.setPiece(3, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(1, 3, Piece(PieceType::Pawn, Color::Black));

    // White makes a quiet king move
    board.makeMove(Move(7, 4, 7, 5));

    // Black moves d7 to d5
    board.makeMove(Move(1, 3, 3, 3));

    // White captures en passant
    board.makeMove(Move(3, 4, 2, 3));

    check(board.getHalfMoveCounter() == 0, "En passant leaves counter at zero");

    check(board.getPiece(3, 3).isEmpty(), "En passant still removes captured pawn");
}

// Test kingside castling
void testKingsideCastling(){
    Board board;

    // Clear White's kingside castling path
    board.setPiece(7, 5, Piece());
    board.setPiece(7, 6, Piece());

    // White castles kingside
    board.makeMove(Move(7, 4, 7, 6));

    check(board.getHalfMoveCounter() == 1, "Kingside castling increments counter");
}

// Test queenside castling
void testQueensideCastling(){
    Board board;

    // Clear White's queenside castling path
    board.setPiece(7, 1, Piece());
    board.setPiece(7, 2, Piece());
    board.setPiece(7, 3, Piece());

    // White castles queenside
    board.makeMove(Move(7, 4, 7, 2));

    check(board.getHalfMoveCounter() == 1, "Queenside castling increments counter");
}

// Test the 50-move and 75-move thresholds
void testDrawThresholds(){
    Board board;

    // Execute 99 quiet half-moves
    executeQuietMoves(board, 99);

    check(board.getHalfMoveCounter() == 99, "Counter reaches exactly 99");

    check(!MoveGenerator::isFiftyMoveRule(board) && !MoveGenerator::isSeventyFiveMoveRule(board),
          "Neither draw threshold is reached at 99 half-moves");

    // Execute one additional half-move
    executeQuietMoves(board, 1);

    check(board.getHalfMoveCounter() == 100, "Counter reaches exactly 100");

    check(MoveGenerator::isFiftyMoveRule(board) && !MoveGenerator::isSeventyFiveMoveRule(board),
          "Only 50-move threshold is reached at 100 half-moves");

    // Continue to 101 half-moves
    executeQuietMoves(board, 1);

    check(MoveGenerator::isFiftyMoveRule(board), "50-move rule remains true at 101 half-moves");

    // Continue to 149 half-moves
    executeQuietMoves(board, 48);

    check(board.getHalfMoveCounter() == 149 && !MoveGenerator::isSeventyFiveMoveRule(board),
          "75-move rule remains false at 149 half-moves");

    // Continue to 150 half-moves
    executeQuietMoves(board, 1);

    check(board.getHalfMoveCounter() == 150, "Counter reaches exactly 150");

    check(MoveGenerator::isFiftyMoveRule(board) && MoveGenerator::isSeventyFiveMoveRule(board),
          "Both draw thresholds are reached at 150 half-moves");

    // Continue beyond both thresholds
    executeQuietMoves(board, 10);

    check(board.getHalfMoveCounter() == 160 &&
          MoveGenerator::isFiftyMoveRule(board) && MoveGenerator::isSeventyFiveMoveRule(board),
          "Both draw thresholds remain reached beyond 150 half-moves");
}

// Test resetting the counter after reaching a draw threshold
void testResetAfterThreshold(){
    Board board;

    executeQuietMoves(board, 100);

    // White pawn moves e2 to e4
    board.makeMove(Move(6, 4, 4, 4));

    check(board.getHalfMoveCounter() == 0 &&
          !MoveGenerator::isFiftyMoveRule(board) && !MoveGenerator::isSeventyFiveMoveRule(board),
          "Pawn movement resets counter and draw thresholds");
}

// Test clear() and reset()
void testClearAndReset(){
    Board board;

    executeQuietMoves(board, 20);

    board.clear();

    check(board.getHalfMoveCounter() == 0, "clear() resets half-move counter");

    board.reset();

    executeQuietMoves(board, 20);

    board.reset();

    check(board.getHalfMoveCounter() == 0, "reset() resets half-move counter");
}

// Test board copying and move generation
void testBoardPreservation(){
    Board board;

    executeQuietMoves(board, 20);

    Board copy = board;

    check(copy.getHalfMoveCounter() == 20, "Board copy preserves half-move counter");

    // Simulate legal moves without modifying the original board
    MoveGenerator::generateLegalMoves(board);

    check(board.getHalfMoveCounter() == 20, "Legal move generation preserves original counter");
}

int main(){
    testInitialState();
    testQuietMoves();
    testSinglePawnMovement();
    testDoublePawnMovement();
    testPawnCapture();
    testCapturePawn();
    testNonPawnCapture();
    testPromotion();
    testEnPassant();
    testKingsideCastling();
    testQueensideCastling();
    testDrawThresholds();
    testResetAfterThreshold();
    testClearAndReset();
    testBoardPreservation();

    std::cout << "\nTest Results\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total: " << testNumber << "\n";

    return failed == 0 ? 0 : 1;
}