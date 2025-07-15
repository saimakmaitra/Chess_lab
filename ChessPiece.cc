#include "ChessPiece.hh"
#include "ChessBoard.hh"

namespace Student
{
    ChessPiece::ChessPiece(ChessBoard &board, Color color, int row, int column, Type type):board(board), color(color), row(row), column(column), type(type){}

//GETTERS
    Color ChessPiece::getColor()
    {
        return color;
    }
    
    Type ChessPiece::getType()
    {
        return type;
    }
    
    int ChessPiece::getRow()
    {
        return row;
    }

    int ChessPiece::getColumn()
    {
        return column;
    }


//SETTERS
    void ChessPiece::setPosition(int row, int column)
    {
        this->row = row;
        this->column = column;
    }

}