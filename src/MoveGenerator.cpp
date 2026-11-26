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


bool MoveGenerator::isSquareAttacked(const Board& board, int row, int col, Color attackingColor) {
    // 1. Check pawn attacks
    // Ex: If a white pawn is attacking e4, it must come from either d3 or f3. chess rank 3 = array row 5.
    int pawnRow = row + ((attackingColor == Color::White) ? 1 : -1);

    // Check which diagonal square where an attacking pawn could exist. Ex: e4, it comes from either d3 or f3. see if it is d or f.
    for (int offset : {-1, 1}){
        int pawnCol = col + offset;
        if (isInsideBoard(pawnRow, pawnCol)){
            Piece retrieve = board.getPiece(pawnRow, pawnCol);

            // White attacks e4, so we must verify that a white pawn is attacking e4
            if (retrieve.color == attackingColor && retrieve.type == PieceType::Pawn){
                return true;
            }
        }
    }

    // 2. Check knight attacks
    const std::array<std::pair<int, int>, 8> knightOffsets = {{
        {-2, -1}, {-2, 1},
        {-1, -2}, {-1, 2},
        {1, -2},  {1, 2},
        {2, -1},  {2, 1}
    }};

    for (const auto& offset : knightOffsets){
        int knightRow = row + offset.first;
        int knightCol = col + offset.second;

        if (isInsideBoard(knightRow, knightCol)){
            Piece retrieve = board.getPiece(knightRow, knightCol);

            if (retrieve.color == attackingColor && retrieve.type == PieceType::Knight){
                return true;
            }
        }
    }

    // 3. Check rook and queen attacks
    const std::array<std::pair<int, int>, 4> straightOffsets = {{
        {-1, 0},  // Up
        {1, 0},   // Down
        {0, -1},  // Left
        {0, 1}    // Right
    }};

    // Search in all four straight directions
    for (const auto& offset : straightOffsets) {
        int straightRow = row + offset.first;
        int straightCol = col + offset.second;

        // Continue searching until we reach the board boundary
        while (isInsideBoard(straightRow, straightCol)) {
            Piece retrieve = board.getPiece(straightRow, straightCol);

            // Check if the square contains a piece
            if (!retrieve.isEmpty()) {

                // Check if the piece is an attacking rook or queen
                if (retrieve.color == attackingColor && (retrieve.type == PieceType::Rook || retrieve.type == PieceType::Queen)) {
                    return true;
                }

                // Any other occupied square blocks the attack
                break;
            }

            straightRow += offset.first;
            straightCol += offset.second;
        }
    }

    // 4. Check bishop and queen attacks
    const std::array<std::pair<int, int>, 4> diagonalOffsets = {{
        {-1, -1},  
        {-1, 1},   
        {1, -1},  
        {1, 1}    
    }};

    // Search in all four straight directions
    for (const auto& offset : diagonalOffsets) {
        int diagonalRow = row + offset.first;
        int diagonalCol = col + offset.second;

        // Continue searching until we reach the board boundary
        while (isInsideBoard(diagonalRow, diagonalCol)) {
            Piece retrieve = board.getPiece(diagonalRow, diagonalCol);

            // Check if the square contains a piece
            if (!retrieve.isEmpty()) {

                // Check if the piece is an attacking rook or queen
                if (retrieve.color == attackingColor && (retrieve.type == PieceType::Bishop || retrieve.type == PieceType::Queen)) {
                    return true;
                }

                // Any other occupied square blocks the attack
                break;
            }

            diagonalRow += offset.first;
            diagonalCol += offset.second;
        }
    }
    // 5. Check king attacks
    const std::array<std::pair<int, int>, 8> kingOffsets = {{
        {-1, -1},
        {0, -1},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, 1},
        {-1, 1},
        {-1, 0}
    }};

    for (const auto& offset : kingOffsets){
        int kingRow = row + offset.first;
        int kingCol = col + offset.second;

        if (isInsideBoard(kingRow, kingCol)){
            Piece retrieve = board.getPiece(kingRow, kingCol);

            if (retrieve.color == attackingColor && retrieve.type == PieceType::King){
                return true;
            }
        }
    }



    // No attacking pieces found
    return false;
}