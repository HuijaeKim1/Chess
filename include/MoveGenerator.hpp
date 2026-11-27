#pragma once

#include <utility>
#include <vector>
#include "Board.hpp"
#include "Move.hpp"

class MoveGenerator {
public:
    // Generate all pseudo-legal moves for the current player
    static std::vector<Move> generatePseudoLegalMoves(const Board& board);

    // Check whether a square is attacked by a specified color
    static bool isSquareAttacked(const Board& board, int row, int col, Color attackingColor);

    // Check whether the king is in check
    static bool isInCheck(const Board& board, Color color);

    // Generate moves that do not leave the player's king in check
    static std::vector<Move> generateLegalMoves(const Board& board);

    // Check if it's checkmate/stalemate
    static bool isCheckmate(const Board& board);
    static bool isStalemate(const Board& board);

    // 1st Draw condition
    static bool isInsufficientMaterial(const Board& board);

    static bool isFiftyMoveRule(const Board& board);
    static bool isSeventyFiveMoveRule(const Board& board);


private:
    // Check whether coordinates are inside the board
    static bool isInsideBoard(int row, int col);

    // Helper function form sliding pieces (rook, bishop, queen)
    static void generateSlidingMoves(const Board& board, int row, int col, std::vector<Move>& moves, const std::vector<std::pair<int, int>>& offsets);

    // Generate possible pawn moves
    static void generatePawnMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    // Generate knight moves
    static void generateKnightMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    // Generate bishop moves
    static void generateBishopMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    // Generate rook moves
    static void generateRookMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    // Generate queen moves
    static void generateQueenMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    // Generate king moves
    static void generateKingMoves(const Board& board, int row, int col, std::vector<Move>& moves);

    
};