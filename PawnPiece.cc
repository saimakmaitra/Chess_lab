#include "PawnPiece.hh"
#include "ChessBoard.hh"

namespace Student
{
    PawnPiece::PawnPiece(ChessBoard &board, Color color, int row, int column, Type type):ChessPiece(board, color, row, column, type){}

    bool PawnPiece::canMoveToLocation(int toRow, int toColumn)
    {
        int direction = (getColor() == Black) ? 1 : -1;

        // Two-square movement
        if (toRow == getRow() + 2 * direction && toColumn == getColumn())
        {
            if ((getColor() == Black && getRow() == 1) || (getColor() == White && getRow() == 6))
            {
                if (board.getPiece(toRow, toColumn) == nullptr && board.getPiece(getRow() + direction, toColumn) == nullptr)
                {
                    return true;
                }
            }
        }

        // One-square movement
        if (toRow == getRow() + direction && toColumn == getColumn())
        {
            if (board.getPiece(toRow, toColumn) == nullptr)
            {
                return true;
            }
        }

        // Diagonal movement
        if (toRow == getRow() + direction && abs(getColumn() - toColumn) == 1)
        {
            // Check if there is a piece in the capture square
            ChessPiece *targetPiece = board.getPiece(toRow, toColumn);
            // Check if the piece is an enemy piece
            if (targetPiece != nullptr && targetPiece->getColor() != getColor())
            {
                return true;
            }
        }

        // Move is not standard pawn movement
        return false;
    }

//For displayBoard
    const char *PawnPiece::toString(){
        if (color == Color::White){
            return "♙";
        }
        else{
            return "♟";
        }   
    }

}