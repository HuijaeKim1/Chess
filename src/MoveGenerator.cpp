#include <vector>
#include "MoveGenerator.hpp"
#include <utility>
#include <array>

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
    // Retrieve the pawn
    Piece pawn = board.getPiece(row, col);

    // Condition: if true, it's -1, if false it's 1
    int direction = (pawn.color == Color::White) ? -1 : 1;

    int startRow = (pawn.color == Color::White) ? 6 : 1;

    // Calculate the destination row
    int nextRow = row + direction;

    // Check if the move is valid
    if (isInsideBoard(nextRow, col) && board.getPiece(nextRow, col).isEmpty()){
        moves.emplace_back(row, col, nextRow, col);

        // Check if the pawn is at the starting row (for moving 2 squares)
        if (row == startRow){
            int doubleRow = row + 2 * direction;

            if (isInsideBoard(doubleRow, col) && board.getPiece(doubleRow, col).isEmpty()){
                moves.emplace_back(row, col, doubleRow, col);
            }
        }

        // Diagonal Moves (For capturing)
        for (int offset : {-1, 1}){
            int nextCol = col + offset;

            if (isInsideBoard(nextRow, nextCol)){
                Piece target = board.getPiece(nextRow, nextCol);

                if (!target.isEmpty() && target.color != pawn.color && target.type != PieceType::King){
                    moves.emplace_back(row, col, nextRow, nextCol);
                }
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