#include <iostream>
#include <vector>
#include <array>
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
                  int fr, int fc, int tr, int tc,
                  PieceType promotion = PieceType::None) {

    Move target(fr, fc, tr, tc, promotion);

    return std::find(moves.begin(), moves.end(), target) != moves.end();
}

// All possible promotion types
const std::array<PieceType, 4> promotionTypes = {
    PieceType::Queen,
    PieceType::Rook,
    PieceType::Bishop,
    PieceType::Knight
};

// Test white forward promotion
void testWhitePromotion() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 4, "White generates exactly 4 forward promotion moves");

    bool allPresent = true;

    for (auto type : promotionTypes) {
        if (!containsMove(moves, 1, 4, 0, 4, type)) {
            allPresent = false;
        }
    }

    check(allPresent, "White generates all four promotion types");

    check(!containsMove(moves, 1, 4, 0, 4),
          "White cannot reach final rank without promotion");
}

// Test black forward promotion
void testBlackPromotion() {
    Board board;
    board.clear();

    // Temporary white move to switch the turn to Black
    board.setPiece(6, 0, Piece(PieceType::Pawn, Color::White));
    board.makeMove(Move(6, 0, 5, 0));
    board.setPiece(5, 0, Piece());

    board.setPiece(6, 4, Piece(PieceType::Pawn, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 4, "Black generates exactly 4 forward promotion moves");

    bool allPresent = true;

    for (auto type : promotionTypes) {
        if (!containsMove(moves, 6, 4, 7, 4, type)) {
            allPresent = false;
        }
    }

    check(allPresent, "Black generates all four promotion types");

    check(!containsMove(moves, 6, 4, 7, 4),
          "Black cannot reach final rank without promotion");
}

// Test blocked forward promotion
void testBlockedPromotion() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 4, Piece(PieceType::Rook, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.empty(), "Pawn cannot promote forward into an occupied square");
}

// Test capture promotion
void testCapturePromotion() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 3, Piece(PieceType::Rook, Color::Black));
    board.setPiece(0, 5, Piece(PieceType::Knight, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 12, "Pawn generates 12 moves with two captures and forward promotion");

    bool leftPresent = true;
    bool rightPresent = true;
    bool forwardPresent = true;

    for (auto type : promotionTypes) {
        if (!containsMove(moves, 1, 4, 0, 3, type)) {
            leftPresent = false;
        }

        if (!containsMove(moves, 1, 4, 0, 5, type)) {
            rightPresent = false;
        }

        if (!containsMove(moves, 1, 4, 0, 4, type)) {
            forwardPresent = false;
        }
    }

    check(leftPresent, "All four left capture promotions generated");
    check(rightPresent, "All four right capture promotions generated");
    check(forwardPresent, "All four forward promotions generated");
}

// Test capture promotion when forward movement is blocked
void testBlockedForwardCapture() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 4, Piece(PieceType::Rook, Color::Black));
    board.setPiece(0, 3, Piece(PieceType::Knight, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 4, "Capture promotion still works when forward movement is blocked");

    bool allPresent = true;

    for (auto type : promotionTypes) {
        if (!containsMove(moves, 1, 4, 0, 3, type)) {
            allPresent = false;
        }
    }

    check(allPresent, "All capture promotion types available despite blocked forward square");
}

// Test friendly pieces and enemy kings
void testInvalidPromotionCaptures() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 3, Piece(PieceType::Rook, Color::White));
    board.setPiece(0, 5, Piece(PieceType::King, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    bool friendlyCapture = false;
    bool kingCapture = false;

    for (auto type : promotionTypes) {
        if (containsMove(moves, 1, 4, 0, 3, type)) {
            friendlyCapture = true;
        }

        if (containsMove(moves, 1, 4, 0, 5, type)) {
            kingCapture = true;
        }
    }

    check(!friendlyCapture, "Promotion cannot capture friendly pieces");
    check(!kingCapture, "Promotion cannot capture enemy king");
}

// Test promotions on the edge of the board
void testEdgePromotion() {
    Board board;
    board.clear();

    board.setPiece(1, 0, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 1, Piece(PieceType::Rook, Color::Black));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 8, "Pawn on a-file generates exactly 8 promotion moves");

    board.clear();

    board.setPiece(1, 7, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 6, Piece(PieceType::Rook, Color::Black));

    moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 8, "Pawn on h-file generates exactly 8 promotion moves");
}

// Test executing all four promotion types
void testPromotionExecution() {
    const std::array<const char*, 4> names = {
        "Pawn successfully promotes to Queen",
        "Pawn successfully promotes to Rook",
        "Pawn successfully promotes to Bishop",
        "Pawn successfully promotes to Knight"
    };

    for (int i = 0; i < 4; i++) {
        Board board;
        board.clear();

        board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));

        board.makeMove(Move(1, 4, 0, 4, promotionTypes[i]));

        Piece promoted = board.getPiece(0, 4);

        check(promoted.type == promotionTypes[i], names[i]);
        check(promoted.color == Color::White, "Promoted piece preserves white color");
        check(board.getPiece(1, 4).isEmpty(), "Original pawn square is empty");
        check(board.getCurrentTurn() == Color::Black, "Turn switches after promotion");
    }
}

// Test black promotion execution
void testBlackPromotionExecution() {
    Board board;
    board.clear();

    board.setPiece(6, 0, Piece(PieceType::Pawn, Color::White));
    board.makeMove(Move(6, 0, 5, 0));
    board.setPiece(5, 0, Piece());

    board.setPiece(6, 4, Piece(PieceType::Pawn, Color::Black));

    board.makeMove(Move(6, 4, 7, 4, PieceType::Queen));

    Piece promoted = board.getPiece(7, 4);

    check(promoted.type == PieceType::Queen, "Black pawn successfully promotes");
    check(promoted.color == Color::Black, "Promoted piece preserves black color");
    check(board.getPiece(6, 4).isEmpty(), "Original black pawn square is empty");
    check(board.getCurrentTurn() == Color::White, "Turn switches to White after black promotion");
}

// Test executing a capture promotion
void testCapturePromotionExecution() {
    Board board;
    board.clear();

    board.setPiece(1, 4, Piece(PieceType::Pawn, Color::White));
    board.setPiece(0, 3, Piece(PieceType::Rook, Color::Black));

    board.makeMove(Move(1, 4, 0, 3, PieceType::Knight));

    Piece promoted = board.getPiece(0, 3);

    check(promoted.type == PieceType::Knight, "Capture promotion replaces captured piece");
    check(promoted.color == Color::White, "Capture promotion preserves pawn color");
    check(board.getPiece(1, 4).isEmpty(), "Capture promotion clears original square");
}

// Test updated equality operator
void testMoveEquality() {
    Move move1(1, 4, 0, 4, PieceType::Queen);
    Move move2(1, 4, 0, 4, PieceType::Knight);
    Move move3(1, 4, 0, 4, PieceType::Queen);
    Move move4(1, 4, 0, 4);

    check(!(move1 == move2), "Different promotion types are unequal");
    check(move1 == move3, "Identical promotion moves are equal");
    check(!(move1 == move4), "Promotion move differs from ordinary move");
    check(move4.promotion == PieceType::None, "Default promotion type is None");
}

// Test that ordinary pawn movement is unaffected
void testNormalPawnMovement() {
    Board board;
    board.clear();

    board.setPiece(6, 4, Piece(PieceType::Pawn, Color::White));

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 2, "Ordinary pawn still generates two starting moves");

    board.makeMove(Move(6, 4, 4, 4));

    Piece pawn = board.getPiece(4, 4);

    check(pawn.type == PieceType::Pawn, "Ordinary pawn movement does not change piece type");
    check(pawn.color == Color::White, "Ordinary pawn movement preserves color");
}

// Test starting position
void testStartingPosition() {
    Board board;

    auto moves = MoveGenerator::generateLegalMoves(board);

    check(moves.size() == 20, "Starting position still generates 20 legal moves");

    bool noPromotions = true;

    for (const auto& move : moves) {
        if (move.promotion != PieceType::None) {
            noPromotions = false;
        }
    }

    check(noPromotions, "Starting position contains no promotion moves");
}

int main() {
    testWhitePromotion();
    testBlackPromotion();
    testBlockedPromotion();
    testCapturePromotion();
    testBlockedForwardCapture();
    testInvalidPromotionCaptures();
    testEdgePromotion();
    testPromotionExecution();
    testBlackPromotionExecution();
    testCapturePromotionExecution();
    testMoveEquality();
    testNormalPawnMovement();
    testStartingPosition();

    std::cout << "\nTest Results\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total: " << testNumber << "\n";

    return failed == 0 ? 0 : 1;
}