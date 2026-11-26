#include "Board.hpp"
#include <iostream>
using namespace std;


Board::Board() {
    reset();
}

void Board::clear() {
    squares.fill(Piece());  // Replace all 64 objects with empty Piece objects
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

    // Move the piece to the destination
    setPiece(move.toRow, move.toCol, retrieve);

    // Clear the original square
    setPiece(move.fromRow, move.fromCol, Piece());
    switchTurn();
}