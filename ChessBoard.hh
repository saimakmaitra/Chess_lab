#ifndef _CHESSBOARD_H__
#define _CHESSBOARD_H__

#include "ChessPiece.hh"
#include "KingPiece.hh"

#include <list>
#include <vector>
#include <sstream>

namespace Student
{
    class ChessBoard
    {
    private:
        int numRows = 0;
        int numCols = 0;
        Color turn = White;
        bool whiteKingMoved = false;
        bool blackKingMoved = false;
        bool whiteRookLeftMoved = false;
        bool whiteRookRightMoved = false;
        bool blackRookLeftMoved = false;
        bool blackRookRightMoved = false;
        /**
         * @brief
         * A 2D vector of pointers to ChessPiece objects.
         * board.at(row) returns the entire row.
         * board.at(row).at(col) returns a pointer to a ChessPiece object.
         * *(board.at(row).at(col)) returns the ChessPiece object itself.
         */
        std::vector<std::vector<ChessPiece *>> board;
        std::vector<ChessPiece *> pieces;
        KingPiece *whiteKing;
        KingPiece *blackKing;
        
    public:
        /**
         * @brief
         * Allocates memory on the heap for the board.
         * Remember to initialise all pointers to nullptr.
         * @param numRow
         * Number of rows of the chess board.
         * @param numCol
         * Number of columns of the chessboard
         */
        ChessBoard(int numRow, int numCol);

        /**
         * @return
         * Number of rows in chess board.
         */
        int getNumRows() { return numRows; }

        /**
         * @return
         * Number of columns in chess board.
         */
        int getNumCols() { return numCols; }

        /**
         * @return
         * Pointer to a piece.
         */
        ChessPiece *getPiece(int r, int c) { return board.at(r).at(c); }

        /**
         * @brief
         * Allocates memory for a new chess piece and assigns its
         * address to the corresponding pointer in the 'board' variable.
         * Remove any existing piece first before adding the new piece.
         * @param col
         * Color of the piece to be created.
         * @param ty
         * Type of the piece to be created.
         * @param startRow
         * Starting row of the piece to be created.
         * @param startColumn
         * Starting column of the piece to be created.
         */
        void createChessPiece(Color col, Type ty, int startRow, int startColumn);

        /**
         * @brief
         * Performs the move if the move is valid.
         * Account for the turn, staying within bounds and validity of the move.
         * This function is only needed for Part 2 and Part 3.
         * You can define a dummy implementation for Part 1 to get the code to compile.
         * @param fromRow
         * The row of the piece to be moved.
         * @param fromColumn
         * The column of the piece to be moved.
         * @param toRow
         * The row of the destination position.
         * @param toColumn
         * The column of the destination position.
         * @return
         * A boolean indicating whether move was executed successfully.
         */
        bool movePiece(int fromRow, int fromColumn, int toRow, int toColumn);

        /**
         * @brief
         * Checks if a move is valid without accounting for turns.
         * @param fromRow
         * The row of the piece to be moved.
         * @param fromColumn
         * The column of the piece to be moved.
         * @param toRow
         * The row of the destination position.
         * @param toColumn
         * The column of the destination position.
         * @return
         * Returns true if move may be executed without accounting for turn.
         */
        bool isValidMove(int fromRow, int fromColumn, int toRow, int toColumn);

        /**
         * @brief
         * Checks if the piece at a position is under threat.
         * This function is only needed for Part 2 and Part 3.
         * You can define a dummy implementation for Part 1 to get the code to compile.
         * @param row
         * Row of piece being checked.
         * @param column
         * Column of piece being checked.
         * @return
         * Returns true if a piece exists at the stated position, and an opponent
         * piece may move to the position.
         */
        bool isPieceUnderThreat(int row, int column);

        /**
         * @brief
         * Returns an output string stream displaying the layout of the board.
         * An ostringstream is used to automatically handle formatting of integers
         * and special characters into their string representations.
         * @return
         * An output stream containing the full board layout.
         */
        std::ostringstream displayBoard();


        //HELPER FUNCTION: PIECE CAPTURING
        /** 
         * @brief
         * Captures a piece at the given row and column. And deals with the removal/deletion of the piece.
         * @param row
         * The row of the piece to be captured.
         * @param column
         * The column of the piece to be captured.
         */
        void capturePiece(int row, int column); 
        
        void setKing(KingPiece* king, Color color);
        KingPiece *getKing(Color color);
        bool isKingInCheck(Color color);
        bool isKingSafe(int fromRow, int fromColumn, int toRow, int toColumn);
        bool isSquareUnderAttack(int row, int column, Color color);
        void updateCastlingFlags(ChessPiece *piece, int fromColumn);
        //DESTRUCTOR
        ~ChessBoard();
    };
}

#endif // _CHESSBOARD_H__
