#include "Board.hpp"
#include <iostream>
#include <cctype>
#include <cstdlib>
using namespace std;


Board::Board() {
    reset();
}

void Board::clear() {
    squares.fill(Piece());  // Replace all 64 objects with empty Piece objects

    currentTurn = Color::White;

    whiteCanCastleKingside = false;
    whiteCanCastleQueenside = false;
    blackCanCastleKingside = false;
    blackCanCastleQueenside = false;

    enPassantRow = -1;
    enPassantCol = -1;

    halfMoveCounter = 0;
}

void Board::reset() {
    clear();

    currentTurn = Color::White;

    // Initialize black and white pawns
    for (int col = 0; col < 8; col++) {
        squares[1 * 8 + col] = Piece(PieceType::Pawn, Color::Black);
        squares[6 * 8 + col] = Piece(PieceType::Pawn, Color::White);
    }

    // Define the order of pieces on the back ranks
    const std::array<PieceType, 8> backRank = {
        PieceType::Rook,
        PieceType::Knight,
        PieceType::Bishop,
        PieceType::Queen,
        PieceType::King,
        PieceType::Bishop,
        PieceType::Knight,
        PieceType::Rook
    };

    // Initialize black and white back ranks
    for (int col = 0; col < 8; col++) {
        squares[0 * 8 + col] = Piece(backRank[col], Color::Black);
        squares[7 * 8 + col] = Piece(backRank[col], Color::White);
    }

    whiteCanCastleKingside = true;
    whiteCanCastleQueenside = true;
    blackCanCastleKingside = true;
    blackCanCastleQueenside = true;
}

Piece Board::getPiece(int row, int col) const{
    return squares[row * 8 + col];
}

void Board::setPiece(int row, int col, Piece piece){
    squares[row * 8 + col] = piece;
}

Color Board::getCurrentTurn() const{
    return currentTurn;
}

void Board::print() const {
    for (int row = 0; row < 8; row++) {

        std::cout << 8 - row << "  ";

        for (int col = 0; col < 8; col++) {
            Piece piece = getPiece(row, col);

            char symbol = '.';

            // Determine the character based on the piece type
            switch (piece.type) {
                case PieceType::None:
                    symbol = '.';
                    break;

                case PieceType::Pawn:
                    symbol = 'P';
                    break;

                case PieceType::Knight:
                    symbol = 'N';
                    break;

                case PieceType::Bishop:
                    symbol = 'B';
                    break;

                case PieceType::Rook:
                    symbol = 'R';
                    break;

                case PieceType::Queen:
                    symbol = 'Q';
                    break;

                case PieceType::King:
                    symbol = 'K';
                    break;
            }

            // Convert black pieces to lowercase
            if (piece.color == Color::Black) {
                symbol = static_cast<char>(std::tolower(symbol));
            }

            // Print the piece 
            std::cout << symbol << ' ';
        }

        // Move to the next row
        std::cout << '\n';
    }

    // Print the column labels
    std::cout << "\n   a b c d e f g h\n";

    // Display whose turn it is
    std::cout << "\n" << (currentTurn == Color::White ? "White" : "Black") << " to move.\n";
}

void Board::switchTurn(){
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
}

void Board::makeMove(const Move& move){
    // Retrieve the piece that we want to move
    Piece retrieve = getPiece(move.fromRow, move.fromCol);

    Piece captured = getPiece(move.toRow, move.toCol);

    // Reset the 50 and 75 move counter if a pawn moves or any piece is captured
    if (retrieve.type == PieceType::Pawn || !captured.isEmpty()){
        halfMoveCounter = 0;
    }
    else{
        halfMoveCounter++;
    }

    // Handle en passant capture
    if (retrieve.type == PieceType::Pawn && move.fromCol != move.toCol && move.toRow == enPassantRow
        && move.toCol == enPassantCol && captured.isEmpty()){
        
        // Remove the captured pawn
        setPiece(move.fromRow, move.toCol, Piece());
    }

    // Reset
    enPassantRow = -1;
    enPassantCol = -1;

    // Detect double-sqaure pawn movement
    if (retrieve.type == PieceType::Pawn && move.fromCol == move.toCol && abs(move.fromRow - move.toRow) == 2){
        enPassantRow = (move.fromRow + move.toRow) / 2;
        enPassantCol = move.fromCol; 
    }


    // Update castling rights if the king moves
    if (retrieve.type == PieceType::King){
        if (retrieve.color == Color::White){
            whiteCanCastleKingside = false;
            whiteCanCastleQueenside = false;
        }
        else{
            blackCanCastleKingside = false;
            blackCanCastleQueenside = false;
        }
    }

    // Update castling rights if the rook moves
    if (retrieve.type == PieceType::Rook){
        if (retrieve.color == Color::White){
            if (move.fromRow == 7 && move.fromCol == 7){
                whiteCanCastleKingside = false;
            }
            else if (move.fromRow == 7 && move.fromCol == 0){
                whiteCanCastleQueenside = false;
            }
        }
        else{
            if (move.fromRow == 0 && move.fromCol == 7){
                blackCanCastleKingside = false;
            }
            else if (move.fromRow == 0 && move.fromCol == 0){
                blackCanCastleQueenside = false;
            }
        }
    }

    // Update castling if rook gets captured
    if (captured.type == PieceType::Rook) {
        if (captured.color == Color::White) {

            // White rook captured on h1
            if (move.toRow == 7 && move.toCol == 7) {
                whiteCanCastleKingside = false;
            }
            // White rook captured on a1
            else if (move.toRow == 7 && move.toCol == 0) {
                whiteCanCastleQueenside = false;
            }
        }
        else {

            // Black rook captured on h8
            if (move.toRow == 0 && move.toCol == 7) {
                blackCanCastleKingside = false;
            }
            // Black rook captured on a8
            else if (move.toRow == 0 && move.toCol == 0) {
                blackCanCastleQueenside = false;
            }
        }
    }

    if (retrieve.type == PieceType::King){
        // Handle kingside castling
        if (retrieve.type == PieceType::King && move.fromRow == move.toRow && move.toCol - move.fromCol == 2) {

            // Retrieve the kingside rook
            Piece rook = getPiece(move.fromRow, 7);

            // Move the rook from column 7 to column 5
            setPiece(move.fromRow, 5, rook);

            // Clear the rook's original square
            setPiece(move.fromRow, 7, Piece());
        }

        // Handle Queenside castling
        else if (retrieve.type == PieceType::King && move.fromRow == move.toRow && move.toCol - move.fromCol == -2){
            // Retrieve the kingside rook
            Piece rook = getPiece(move.fromRow, 0);

            // Move the rook from column 7 to column 5
            setPiece(move.fromRow, 3, rook);

            // Clear the rook's original square
            setPiece(move.fromRow, 0, Piece());
        }

    }

    // Handle pawn promotion
    if (retrieve.type == PieceType::Pawn && move.promotion != PieceType::None){
        retrieve.type = move.promotion;
    }

    // Move the piece to the destination
    setPiece(move.toRow, move.toCol, retrieve);

    // Clear the original square
    setPiece(move.fromRow, move.fromCol, Piece());
    switchTurn();
}

bool Board::canCastleKingside(Color color) const{
    return ((color == Color::White) ? whiteCanCastleKingside : blackCanCastleKingside);
}

bool Board::canCastleQueenside(Color color) const{
        return ((color == Color::White) ? whiteCanCastleQueenside : blackCanCastleQueenside);
}

int Board::getEnPassantRow() const{
    return enPassantRow;
}

int Board::getEnPassantCol() const{
    return enPassantCol;
}

int Board::getHalfMoveCounter() const{
    return halfMoveCounter;
}