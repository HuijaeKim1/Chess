#include <vector>
#include <iostream>
#include "MoveGenerator.hpp"
#include <utility>
#include <array>
using namespace std;

bool MoveGenerator::isInsideBoard(int row, int col){
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(const Board& board) {
    // Create a vector to store generated moves
    std::vector<Move> moves;

    // Retrieve the current player's color
    Color currentTurn = board.getCurrentTurn();

    // Iterate through all 64 squares
    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){

            // Retrieve the piece at the current square
            Piece piece = board.getPiece(row, col);

            // Skip empty squares and opponent pieces
            if (piece.isEmpty() || piece.color != currentTurn) {
                continue;
            }

            // Determine the piece type
            switch (piece.type) {
                case PieceType::None:
                    break;      

                case PieceType::Pawn:
                    generatePawnMoves(board, row, col, moves);
                    break;

                case PieceType::Knight:
                    generateKnightMoves(board, row, col, moves);
                    break;

                case PieceType::Bishop:
                    generateBishopMoves(board, row, col, moves);
                    break;

                case PieceType::Rook:
                    generateRookMoves(board, row, col, moves);
                    break;

                case PieceType::Queen:
                    generateQueenMoves(board, row, col, moves);
                    break;

                case PieceType::King:
                    generateKingMoves(board, row, col, moves);
                    break;
            }
        }
    }

    return moves;
}


void MoveGenerator::generatePawnMoves(const Board& board, int row, int col, std::vector<Move>& moves) {
    Piece pawn = board.getPiece(row, col);

    // Condition. If true it selects -1, if false, it selects 1
    int direction = (pawn.color == Color::White) ? -1 : 1;

    int startRow = (pawn.color == Color::White) ? 6 : 1;

    int nextRow = row + direction;

    // Single-square forward movement
    if (isInsideBoard(nextRow, col) &&
        board.getPiece(nextRow, col).isEmpty()) {

        moves.emplace_back(row, col, nextRow, col);

        // Double-square movement from the starting row
        if (row == startRow) {
            int doubleRow = row + 2 * direction;

            if (isInsideBoard(doubleRow, col) &&
                board.getPiece(doubleRow, col).isEmpty()) {

                moves.emplace_back(row, col, doubleRow, col);
            }
        }
    }

    // Diagonal captures
    for (int offset : {-1, 1}) {
        int nextCol = col + offset;

        // Check whether the destination is inside the board
        if (isInsideBoard(nextRow, nextCol)) {
            Piece target = board.getPiece(nextRow, nextCol);

            // Capture only occupied enemy squares, excluding kings
            if (!target.isEmpty() &&
                target.color != pawn.color &&
                target.type != PieceType::King) {

                moves.emplace_back(row, col, nextRow, nextCol);
            }
        }
    }
}


void MoveGenerator::generateKnightMoves(const Board& board, int row, int col, std::vector<Move>& moves){
    Piece knight = board.getPiece(row, col);

    /*
    std::pair<int, int> offset = {-2, 1}; stores two values together
    offset.first accesses -2, offset.second accesses 1
    */

    const std::array<std::pair<int, int>, 8> offsets = {{
        {-2, -1},
        {-1, -2},
        {1, -2},
        {2, -1},
        {2, 1},
        {1, 2},
        {-1, 2},
        {-2, 1}
    }};

    // Iterate through the offsets (possible destinations)
    for (const auto& offset : offsets){
        int nextRow = row + offset.first;
        int nextCol = col + offset.second;

        if (isInsideBoard(nextRow, nextCol)){
            // Retrieve the piece from the destination 
            Piece target = board.getPiece(nextRow, nextCol);

            if (target.isEmpty() || (target.color != knight.color && target.type != PieceType::King)){
                moves.emplace_back(row, col, nextRow, nextCol);
            }
        }
    }
}


void MoveGenerator::generateSlidingMoves(const Board& board, int row, int col, std::vector<Move>& moves, const std::vector<std::pair<int, int>>& offsets){
    Piece movingPiece = board.getPiece(row, col);

    for (const auto& offset : offsets){
        int nextRow = row + offset.first;
        int nextCol = col + offset.second;

        while (isInsideBoard(nextRow, nextCol)){
            Piece target = board.getPiece(nextRow, nextCol);

            if (target.isEmpty()){
                moves.emplace_back(row, col, nextRow, nextCol);
            } 
            else {
                if (target.color != movingPiece.color && target.type != PieceType::King){
                    moves.emplace_back(row, col, nextRow, nextCol);
                }
                break;
            }

            nextRow += offset.first;
            nextCol += offset.second;
        }
    }
}


void MoveGenerator::generateBishopMoves(const Board& board, int row, int col, std::vector<Move>& moves){
    // Bishop's possible moves
    const std::vector<std::pair<int, int>>& offsets = {{
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}
    }};

    generateSlidingMoves(board, row, col, moves, offsets);
}


void MoveGenerator::generateRookMoves(const Board& board, int row, int col, std::vector<Move>& moves) {  
    const std::vector<std::pair<int, int>> offsets = {
        {0, 1},     // Right
        {1, 0},     // Down
        {0, -1},    // Left
        {-1, 0}     // Up
    };

    generateSlidingMoves(board, row, col, moves, offsets);
}

void MoveGenerator::generateQueenMoves(const Board& board, int row, int col, std::vector<Move>& moves) {
    const std::vector<std::pair<int, int>> offsets = {
        // Diagonal directions
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},

        // Horizontal and vertical directions
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}
    };

    generateSlidingMoves(board, row, col, moves, offsets);
}

void MoveGenerator::generateKingMoves(const Board& board, int row, int col, std::vector<Move>& moves){
    Piece king = board.getPiece(row, col);

    const std::array<std::pair<int, int>, 8> offsets = {{
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0}
    }};

    for (const auto& offset : offsets){
        int nextRow = row + offset.first;
        int nextCol = col + offset.second;

        if (isInsideBoard(nextRow, nextCol)){
            // Retrieve the piece from the destination 
            Piece target = board.getPiece(nextRow, nextCol);

            if (target.isEmpty() || (target.color != king.color && target.type != PieceType::King)){
                moves.emplace_back(row, col, nextRow, nextCol);
            }
        }
    }
}