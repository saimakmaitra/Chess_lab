#include "KingPiece.hh"
#include "ChessBoard.hh"

namespace Student
{
    KingPiece::KingPiece(ChessBoard &board, Color color, int row, int column, Type type) 
        : ChessPiece(board, color, row, column, type) {}

    bool KingPiece::canMoveToLocation(int toRow, int toColumn) 
    {
        int rowDiff = abs(toRow - getRow());
        int colDiff = abs(toColumn - getColumn());

        if (rowDiff <= 1 && colDiff <= 1) 
        {
            ChessPiece *destinationPiece = board.getPiece(toRow, toColumn);
            if (destinationPiece != nullptr && destinationPiece->getColor() == getColor()) 
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        // Castling logic
        if (rowDiff == 0 && (colDiff == 2)) 
        {
            if (canCastle(toColumn)) 
            {
                return true;
            }
        }
        
        return false;
    }

    bool KingPiece::canCastle(int toColumn) 
    {
        // Castling is only allowed if the King has not moved
        if (getHasMoved()) 
        {
            return false;
        }

        // Determine the Rook's column based on the castling direction
        int rookColumn = (toColumn > getColumn()) ? 7 : 0;
        ChessPiece *rook = board.getPiece(getRow(), rookColumn);

        // Validate the Rook's position and state
        if (rook == nullptr || rook->getType() != Type::Rook || rook->getColor() != getColor() || rook->getHasMoved()) 
        {
            return false;
        }

        // Ensure the path between the King and Rook is clear
        int step = (toColumn > getColumn()) ? 1 : -1;
        for (int col = getColumn() + step; col != rookColumn; col += step) 
        {
            if (board.getPiece(getRow(), col) != nullptr) 
            {
                return false;
            }
        }

        // Ensure the King does not move through or into a square under attack
        for (int col = getColumn(); col != toColumn + step; col += step) 
        {
            if (board.isSquareUnderAttack(getRow(), col, getColor())) 
            {
                return false;
            }
        }

        return true; // Castling is valid
    }

    const char *KingPiece::toString() 
    {
        if (color == Color::White) 
        {
            return "♔";
        } 
        else 
        {
            return "♚";
        }
    }
}
