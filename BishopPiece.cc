#include "BishopPiece.hh"
#include "ChessBoard.hh"

namespace Student {

    BishopPiece::BishopPiece(ChessBoard &board, Color color, int row, int column, Type type)
        : ChessPiece(board, color, row, column, type) {}

    bool BishopPiece::canMoveToLocation(int toRow, int toColumn) 
    {
        int rowDiff = std::abs(getRow() - toRow);
        int colDiff = std::abs(getColumn() - toColumn);
    
        if (rowDiff == colDiff) 
        {
            int rowStep = (toRow > getRow()) ? 1 : (toRow < getRow()) ? -1 : 0;
            int colStep = (toColumn > getColumn()) ? 1 : (toColumn < getColumn()) ? -1 : 0;
    
            int r = getRow() + rowStep;
            int c = getColumn() + colStep;
    
            while (r != toRow || c != toColumn) 
            {
                if (board.getPiece(r, c) != nullptr) 
                {
                    return false;
                }
                r += rowStep;
                c += colStep;
            }
    
            ChessPiece *targetPiece = board.getPiece(toRow, toColumn);
            if (targetPiece == nullptr || targetPiece->getColor() != getColor()) 
            {
                return true;
            }
        }
    
        return false;
    }

    const char *BishopPiece::toString() {
        if (color == Color::White) {
            return "♗";
        } 
        else 
        {
            return "♝";
        }
    }
} 
