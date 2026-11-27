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

    int promotionRow = ((pawn.color == Color::White) ? 0 : 7);

    const std::array<PieceType, 4> promotionTypes = {
        PieceType::Queen,
        PieceType::Rook,
        PieceType::Bishop,
        PieceType::Knight
    };

    // Single-square forward movement
    if (isInsideBoard(nextRow, col) && board.getPiece(nextRow, col).isEmpty()) {

        // Check if the pawn reaches the promotion row
        if (nextRow == promotionRow){
            for (const auto& promotion : promotionTypes){

                // Generate promotion moves
                moves.emplace_back(row, col, promotionRow, col, promotion);
            }
        }
        else {
            moves.emplace_back(row, col, nextRow, col);
        }

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
            if (!target.isEmpty() && target.color != pawn.color && target.type != PieceType::King) {
                if (nextRow == promotionRow){
                    for (const auto& promotion : promotionTypes){
                        moves.emplace_back(row, col, promotionRow, nextCol, promotion);
                    }
                }
                else {
                    moves.emplace_back(row, col, nextRow, nextCol);
                }
                
            }

            // En Passant capture
            else if (target.isEmpty() && nextRow == board.getEnPassantRow() && nextCol == board.getEnPassantCol()){

                // Retrieve the adjacent piece and see if it's a pawn for en passant
                Piece adjacent = board.getPiece(row, nextCol);
                if (adjacent.type == PieceType::Pawn && adjacent.color != pawn.color){
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

// For rook, bishop, queen
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

    // King castling moves
    int kingStartRow = ((king.color == Color::White) ? 7 : 0);
    Color oppositeColor = ((king.color == Color::White) ? Color::Black : Color::White);

    // If the king is not in its original square castling is not permitted
    if (kingStartRow != row || col != 4){
        return;
    }

    // If the king is in check, castling is not permitted
    if (isInCheck(board, king.color)){
        return;
    }

    // Kingside castling
    if (board.canCastleKingside(king.color)){
        Piece rook = board.getPiece(kingStartRow, 7);

        // Castling requirements: rook of the same color, column 5,6 must be empty and not being attacked
        if (rook.type == PieceType::Rook && rook.color == king.color && 
            board.getPiece(kingStartRow, 5).isEmpty() && board.getPiece(kingStartRow, 6).isEmpty() &&
            !isSquareAttacked(board, kingStartRow, 5, oppositeColor) &&
            !isSquareAttacked(board, kingStartRow, 6, oppositeColor)){
                
            // Generate King side Castling
            moves.emplace_back(row, col, row, 6);
        }
    }

    // Queenside castling
    if (board.canCastleQueenside(king.color)){
        Piece rook = board.getPiece(kingStartRow, 0);

        // Castling requirements: rook of the same color, column 1,2,3 must be empty and 2,3 not being attacked
        if (rook.type == PieceType::Rook && rook.color == king.color && 
            board.getPiece(kingStartRow, 1).isEmpty() && board.getPiece(kingStartRow, 2).isEmpty() &&
            board.getPiece(kingStartRow, 3).isEmpty() && 
            !isSquareAttacked(board, kingStartRow, 2, oppositeColor) &&
            !isSquareAttacked(board, kingStartRow, 3, oppositeColor)){
                
            // Generate King side Castling
            moves.emplace_back(row, col, row, 2);
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

// See if the king of color (the parameter) is in check
bool MoveGenerator::isInCheck(const Board& board, Color color){
    Color oppositeColor = ((color == Color::White) ? Color::Black : Color::White);

    for (int row = 0; row < 8; row++){
        for (int col = 0; col < 8; col++){
            Piece retrieve = board.getPiece(row, col);

            if (retrieve.color == color && retrieve.type == PieceType::King){

                // Check if the target king is attacked by the oppositeColor piece
                return isSquareAttacked(board, row, col, oppositeColor);
            }
        }
    }

    // No matching king found
    return false;
}

std::vector<Move> MoveGenerator::generateLegalMoves(const Board& board){
    std::vector<Move> legalMoves;

    Color playerColor = board.getCurrentTurn();

    std::vector<Move> gen = generatePseudoLegalMoves(board);
    for (const auto& move : gen){
        Board tempBoard = board;

        tempBoard.makeMove(move);

        // If the king is not checked, it's a legal move
        if (!isInCheck(tempBoard, playerColor)){
            legalMoves.push_back(move);
        }

    }

    return legalMoves;
}

bool MoveGenerator::isCheckmate(const Board& board){
    Color currentTurn = board.getCurrentTurn();

    if (isInCheck(board, currentTurn)){
        std::vector<Move> legalMoves = generateLegalMoves(board);

        // If no legal moves are left, it's checkmate
        if (legalMoves.empty()){
            return true;
        }
    }

    // Not checkmate
    return false;
}

// Stalemate: king is not in check but there are no legal moves left
bool MoveGenerator::isStalemate(const Board& board){
    Color currentTurn = board.getCurrentTurn();

    if (!isInCheck(board, currentTurn)){
        std::vector<Move> legalMoves = generateLegalMoves(board);

        if (legalMoves.empty()){
            return true;
        }
    }

    // Not in stalemate
    return false;
}