#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

#include "Board.hpp"
#include "Piece.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"


// Check whether a specific move exists in the vector
bool containsMove(const std::vector<Move>& moves,
                  int fr, int fc, int tr, int tc) {

    Move target(fr, fc, tr, tc);

    return std::find(moves.begin(), moves.end(), target)
           != moves.end();
}

// Print all generated moves in chess notation
void printMoves(const std::vector<Move>& moves) {
    for (const Move& move : moves) {

        // Convert internal coordinates to chess notation
        char fromFile = 'a' + move.fromCol;
        int fromRank = 8 - move.fromRow;

        char toFile = 'a' + move.toCol;
        int toRank = 8 - move.toRow;

        std::cout << fromFile << fromRank
                  << " -> "
                  << toFile << toRank << '\n';
    }
}


// TEST 1: STARTING POSITION
void testStartingPosition() {
    Board board;

    board.print();

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

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

    std::cout << "\nTotal possible moves: "
              << moves.size() << "\n\n";

    printMoves(moves);

    std::cout << "\n[PASS] Starting position\n";
}



// TEST 2: PAWN MOVEMENT
void testPawn() {
    Board board;
    board.clear();

    // Place a white pawn on e2
    board.setPiece(6, 4,
        Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Single and double forward movement
    assert(moves.size() == 2);
    assert(containsMove(moves, 6, 4, 5, 4));
    assert(containsMove(moves, 6, 4, 4, 4));

    // Block the second square
    board.setPiece(4, 4,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 1);
    assert(!containsMove(moves, 6, 4, 4, 4));

    // Block the first square
    board.setPiece(5, 4,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    // Place an enemy piece diagonally
    board.setPiece(5, 3,
        Piece(PieceType::Knight, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Pawn can capture even when forward movement is blocked
    assert(moves.size() == 1);
    assert(containsMove(moves, 6, 4, 5, 3));

    // Replace enemy knight with enemy king
    board.setPiece(5, 3,
        Piece(PieceType::King, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    // Test a pawn at the edge of the board
    board.clear();

    board.setPiece(4, 0,
        Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 1);
    assert(containsMove(moves, 4, 0, 3, 0));

    std::cout << "[PASS] Pawn movement\n";
}


// TEST 3: KNIGHT MOVEMENT
void testKnight() {
    Board board;
    board.clear();

    // Knight in the center
    board.setPiece(4, 4,
        Piece(PieceType::Knight, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 8);

    // Knight in the corner
    board.clear();

    board.setPiece(0, 0,
        Piece(PieceType::Knight, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 2);
    assert(containsMove(moves, 0, 0, 1, 2));
    assert(containsMove(moves, 0, 0, 2, 1));

    // Enemy capture
    board.setPiece(1, 2,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 0, 0, 1, 2));

    // Friendly piece cannot be captured
    board.setPiece(1, 2,
        Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 0, 0, 1, 2));

    std::cout << "[PASS] Knight movement\n";
}


// TEST 4: BISHOP MOVEMENT
void testBishop() {
    Board board;
    board.clear();

    // Bishop in the center
    board.setPiece(4, 4,
        Piece(PieceType::Bishop, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 13);

    // Place an enemy pawn on the bishop's diagonal
    board.setPiece(2, 2,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Can move to the empty square
    assert(containsMove(moves, 4, 4, 3, 3));

    // Can capture the enemy pawn
    assert(containsMove(moves, 4, 4, 2, 2));

    // Cannot move beyond the pawn
    assert(!containsMove(moves, 4, 4, 1, 1));
    assert(!containsMove(moves, 4, 4, 0, 0));

    // Replace enemy pawn with friendly pawn
    board.setPiece(2, 2,
        Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 2, 2));
    assert(!containsMove(moves, 4, 4, 1, 1));

    std::cout << "[PASS] Bishop movement\n";
}


// TEST 5: ROOK MOVEMENT
void testRook() {
    Board board;
    board.clear();

    // Rook in the center
    board.setPiece(4, 4,
        Piece(PieceType::Rook, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 14);

    // Place an enemy piece above the rook
    board.setPiece(2, 4,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 4, 4, 3, 4));
    assert(containsMove(moves, 4, 4, 2, 4));

    // Cannot move beyond the captured piece
    assert(!containsMove(moves, 4, 4, 1, 4));
    assert(!containsMove(moves, 4, 4, 0, 4));

    // Rook cannot move diagonally
    assert(!containsMove(moves, 4, 4, 3, 3));

    // Replace the enemy piece with a friendly piece
    board.setPiece(2, 4,
        Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 2, 4));

    std::cout << "[PASS] Rook movement\n";
}


// TEST 6: QUEEN MOVEMENT
void testQueen() {
    Board board;
    board.clear();

    // Queen in the center
    board.setPiece(4, 4,
        Piece(PieceType::Queen, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    // 14 rook moves + 13 bishop moves
    assert(moves.size() == 27);

    // Place an enemy piece horizontally
    board.setPiece(4, 6,
        Piece(PieceType::Pawn, Color::Black));

    // Place another enemy piece diagonally
    board.setPiece(2, 2,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    // Horizontal capture
    assert(containsMove(moves, 4, 4, 4, 6));
    assert(!containsMove(moves, 4, 4, 4, 7));

    // Diagonal capture
    assert(containsMove(moves, 4, 4, 2, 2));
    assert(!containsMove(moves, 4, 4, 1, 1));

    std::cout << "[PASS] Queen movement\n";
}


// TEST 7: KING MOVEMENT
void testKing() {
    Board board;
    board.clear();

    // King in the center
    board.setPiece(4, 4,
        Piece(PieceType::King, Color::White));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 8);

    // King cannot move two squares
    assert(!containsMove(moves, 4, 4, 2, 4));

    // Enemy capture
    board.setPiece(3, 4,
        Piece(PieceType::Pawn, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 4, 4, 3, 4));

    // Cannot capture a friendly piece
    board.setPiece(3, 4,
        Piece(PieceType::Pawn, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 3, 4));

    // Cannot capture an enemy king
    board.setPiece(3, 4,
        Piece(PieceType::King, Color::Black));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(!containsMove(moves, 4, 4, 3, 4));

    // King in the corner
    board.clear();

    board.setPiece(0, 0,
        Piece(PieceType::King, Color::White));

    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 3);

    std::cout << "[PASS] King movement\n";
}


// TEST 8: EMPTY BOARD
void testEmptyBoard() {
    Board board;
    board.clear();

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    std::cout << "[PASS] Empty board\n";
}


// TEST 9: OPPONENT PIECE FILTERING
void testOpponentPieces() {
    Board board;
    board.clear();

    // White is currently to move
    // Place only black pieces on the board
    board.setPiece(1, 4,
        Piece(PieceType::Pawn, Color::Black));

    board.setPiece(0, 1,
        Piece(PieceType::Knight, Color::Black));

    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.empty());

    std::cout << "[PASS] Opponent piece filtering\n";
}


// TEST 10: MOVE EXECUTION AND TURN SWITCHING
void testMakeMove() {
    Board board;

    // Verify that White starts
    assert(board.getCurrentTurn() == Color::White);


    // Move 1: White plays e2 -> e4
    board.makeMove(Move(6, 4, 4, 4));

    // Verify the destination square
    assert(board.getPiece(4, 4).type == PieceType::Pawn);
    assert(board.getPiece(4, 4).color == Color::White);

    // Verify the original square is empty
    assert(board.getPiece(6, 4).isEmpty());

    // Verify turn switched to Black
    assert(board.getCurrentTurn() == Color::Black);

    // Generate Black's possible moves
    auto moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(moves.size() == 20);

    // Verify Black's single and double pawn moves
    for (int col = 0; col < 8; col++) {
        assert(containsMove(moves, 1, col, 2, col));
        assert(containsMove(moves, 1, col, 3, col));
    }

    // Verify all generated moves belong to Black
    for (const Move& move : moves) {
        assert(board.getPiece(move.fromRow, move.fromCol).color
               == Color::Black);
    }


    // Move 2: Black plays d7 -> d5
    board.makeMove(Move(1, 3, 3, 3));

    assert(board.getPiece(3, 3).type == PieceType::Pawn);
    assert(board.getPiece(3, 3).color == Color::Black);
    assert(board.getPiece(1, 3).isEmpty());
    assert(board.getCurrentTurn() == Color::White);

    // Verify White's capture is generated
    moves = MoveGenerator::generatePseudoLegalMoves(board);

    assert(containsMove(moves, 4, 4, 3, 3));


    // Move 3: White captures e4 -> d5
    board.makeMove(Move(4, 4, 3, 3));

    // White pawn should replace the black pawn
    assert(board.getPiece(3, 3).type == PieceType::Pawn);
    assert(board.getPiece(3, 3).color == Color::White);

    // White pawn's original square should be empty
    assert(board.getPiece(4, 4).isEmpty());

    // Black should now have the turn
    assert(board.getCurrentTurn() == Color::Black);

    // Count remaining pieces
    int whitePieces = 0;
    int blackPieces = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = board.getPiece(row, col);

            if (piece.color == Color::White) {
                whitePieces++;
            }
            else if (piece.color == Color::Black) {
                blackPieces++;
            }
        }
    }

    assert(whitePieces == 16);
    assert(blackPieces == 15);

    // Display the board after all three moves
    std::cout << "\nBoard after move execution test:\n\n";

    board.print();

    std::cout << "\n[PASS] Move execution and captures\n";
}




int main() {
    std::cout << "   CHESS ENGINE TEST SUITE\n";

    // Test basic piece movement
    testStartingPosition();
    testPawn();
    testKnight();
    testBishop();
    testRook();
    testQueen();
    testKing();

    // Test board conditions
    testEmptyBoard();
    testOpponentPieces();

    // Test move execution and turn switching
    testMakeMove();

    std::cout << " ALL 10 TEST GROUPS PASSED!\n";

    return 0;
}