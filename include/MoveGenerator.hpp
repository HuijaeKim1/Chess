#pragma once

#include <vector>
#include "Board.hpp"
#include "Move.hpp"

class MoveGenerator {
public:
    // Generate all pseudo-legal moves for the current player
    static std::vector<Move> generatePseudoLegalMoves(
        const Board& board
    );

private:
    // Check whether coordinates are inside the board
    static bool isInsideBoard(int row, int col);

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