#include "ChessBoard.hh"
#include "PawnPiece.hh"
#include "RookPiece.hh"
#include "BishopPiece.hh"
#include "KingPiece.hh"

using Student::ChessBoard;

std::ostringstream ChessBoard::displayBoard()
{
    std::ostringstream outputString;
    // top scale
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << i;
    }
    outputString << std::endl
                 << "  ";
    // top border
    for (int i = 0; i < numCols; i++)
    {
        outputString << "-";
    }
    outputString << std::endl;

    for (int row = 0; row < numRows; row++)
    {
        outputString << row << "|";
        for (int column = 0; column < numCols; column++)
        {
            ChessPiece *piece = board.at(row).at(column);
            outputString << (piece == nullptr ? " " : piece->toString());
        }
        outputString << "|" << std::endl;
    }

    // bottom border
    outputString << "  ";
    for (int i = 0; i < numCols; i++)
    {
        outputString << "-";
    }
    outputString << std::endl
                 << std::endl;

    return outputString;

}

//Initializer 
ChessBoard::ChessBoard(int numRow, int numCol)
{
    numRows = numRow;
    numCols = numCol;
    board = std::vector<std::vector<ChessPiece *>>(numRows, std::vector<ChessPiece *>(numCols, nullptr));
    turn = White;
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteRookLeftMoved = false;
    whiteRookRightMoved = false;
    blackRookLeftMoved = false;
    blackRookRightMoved = false;
}

void ChessBoard::createChessPiece(Color color, Type type, int startRow, int startColumn)
{
    // Check if the position is empty
    if (board.at(startRow).at(startColumn) != nullptr) {
        ChessPiece *oldPiece = board.at(startRow).at(startColumn);
        // Remove the old piece from the pieces vector
        for (auto iter = pieces.begin(); iter != pieces.end(); iter++) {
            if (*iter == oldPiece) {
                pieces.erase(iter);
                break;
            }
        }
        delete oldPiece;
    }

    ChessPiece *piece = nullptr;
    switch (type)
    {
    case Type::Bishop:
        piece = new BishopPiece(*this, color, startRow, startColumn, type);
        break;
    case Type::Pawn:
        piece = new PawnPiece(*this, color, startRow, startColumn, type);
        break;
    case Type::Rook:
        piece = new RookPiece(*this, color, startRow, startColumn, type);
        break;
    case Type::King:
        piece = new KingPiece(*this, color, startRow, startColumn, type);
        setKing(static_cast<KingPiece*>(piece), color);
        break;
    default:
        break;
    }
    board.at(startRow).at(startColumn) = piece;
    pieces.push_back(piece);
}

bool ChessBoard::isValidMove(int fromRow, int fromColumn, int toRow, int toColumn)
{   
//Check coordinate boundaries are within the board
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols || toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols)
    {
        return false;
    } 
//Check if piece exists
    ChessPiece *piece = board.at(fromRow).at(fromColumn);
    if (piece == nullptr)
    {
        return false;
    }
//Check if piece is moving to the same location
    if (fromRow == toRow && fromColumn == toColumn)
    {
        return false;
    }   

    ChessPiece *targetPiece = board.at(toRow).at(toColumn);
    if (getPiece(toRow, toColumn) != nullptr && targetPiece->getColor() == piece->getColor())
    {
        return false;
    }
//Check if piece can move to location
    if (!piece->canMoveToLocation(toRow, toColumn))
        {
            return false;
        }
// return piece->canMoveToLocation(toRow, toColumn);
    ChessPiece* originalTargetPiece = board.at(toRow).at(toColumn);
    board.at(toRow).at(toColumn) = piece;
    board.at(fromRow).at(fromColumn) = nullptr;
    piece->setPosition(toRow, toColumn);

    bool kingInCheck = isKingInCheck(piece->getColor());

    board.at(fromRow).at(fromColumn) = piece;
    board.at(toRow).at(toColumn) = originalTargetPiece;
    piece->setPosition(fromRow, fromColumn);

// If the move puts the King in check, it is invalid
    if (kingInCheck)
    {
        return false;
    }

// Special handling for castling
    if (piece->getType() == King && abs(toColumn - fromColumn) == 2)
    {
        int row = fromRow;
        int rookCol = (toColumn > fromColumn) ? numCols - 1 : 0;
        ChessPiece *rook = getPiece(row, rookCol);

        // Ensure the Rook exists and has not moved
        if (rook == nullptr || rook->getType() != Type::Rook || rook->getColor() != piece->getColor() || rook->getHasMoved())
        {
            return false;
        }

        // Ensure the path between the King and Rook is clear
        int step = (toColumn > fromColumn) ? 1 : -1;
        for (int col = fromColumn + step; col != rookCol; col += step)
        {
            if (getPiece(row, col) != nullptr)
            {
                return false;
            }
        }

        // Ensure the King does not move through or into a square under attack
        for (int col = fromColumn; col != toColumn + step; col += step)
        {
            if (isSquareUnderAttack(row, col, piece->getColor()))
            {
                return false;
            }
        }
    }

    return true;
}

bool ChessBoard::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols ||
        toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols)
    {
        return false;
    }
    //Move piece
    ChessPiece *piece = board.at(fromRow).at(fromColumn);
    if (piece == nullptr)
    {
        return false;
    }
    //Check if correct turn    
    if ((piece->getColor() == White && turn == Black) ||
        (piece->getColor() == Black && turn == White))
    {
        return false;
    }

    //Check if move is valid
    if (!isValidMove(fromRow, fromColumn, toRow, toColumn))
    {
        return false;
    }

    //Capture opponent piece if exists
    if (board.at(toRow).at(toColumn) != nullptr)
    {
       capturePiece(toRow, toColumn);
    }

    //Move piece
    board.at(toRow).at(toColumn) = piece;
    board.at(fromRow).at(fromColumn) = nullptr;
    piece->setPosition(toRow, toColumn);

    // Update hasMoved flag
    piece->setHasMoved(true);

    // Handle castling
    if (piece->getType() == King && abs(toColumn - fromColumn) == 2) 
    {
        int rookCol = (toColumn > fromColumn) ? numCols - 1 : 0;
        int rookNewCol = (toColumn > fromColumn) ? toColumn - 1 : toColumn + 1;

        ChessPiece *rook = getPiece(fromRow, rookCol);
        board[fromRow][rookNewCol] = rook;
        board[fromRow][rookCol] = nullptr;
        rook->setPosition(fromRow, rookNewCol);
        rook->setHasMoved(true);
    }

    // Switch turn
    turn = (turn == White) ? Black : White;
    return true;
}

bool ChessBoard::isPieceUnderThreat(int row, int column)
{
//Get target piece
    ChessPiece *piece = board.at(row).at(column);
//Check if piece exists   
    if (piece == nullptr)
    {
        return false;
    }
//Check if piece is under threat
Color pieceColor = piece->getColor();

// Iterate through all cells on the chessboard
for (int i = 0; i < numRows; i++)
{
    for (int j = 0; j < numCols; j++)
    {
        // Get the piece at the current cell
        ChessPiece *opponentPiece = board.at(i).at(j);

        // Check if the piece exists and is an opponent's piece
        if (opponentPiece != nullptr && opponentPiece->getColor() != pieceColor)
        {
            // Check if the opponent can move to the target location
            if (isKingSafe(i, j, row, column))
            {
                return true; // The target piece is under threat
            }
        }
    }
}

    return false;
}

void ChessBoard::setKing(KingPiece* king, Color color)
{
    if (color == White) 
    {
        whiteKing = king;
    } 
    else 
    {
        blackKing = king;
    }
}

bool ChessBoard::isKingInCheck(Color color)
{
    KingPiece* king = (color == White) ? whiteKing : blackKing;

    if (king == nullptr) return false;

    for (int i = 0; i < numRows; i++) 
    {
        for (int j = 0; j < numCols; j++) 
        {
            ChessPiece *opponentPiece = getPiece(i, j);
            if (opponentPiece != nullptr && opponentPiece->getColor() != color) 
            {
                if (opponentPiece->canMoveToLocation(king->getRow(), king->getColumn())) 
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool ChessBoard::isKingSafe(int fromRow, int fromColumn, int toRow, int toColumn)
{
    if (fromRow < 0 || fromRow >= numRows || fromColumn < 0 || fromColumn >= numCols ||
        toRow < 0 || toRow >= numRows || toColumn < 0 || toColumn >= numCols)
    {
        return false;
    }

    ChessPiece *piece = board.at(fromRow).at(fromColumn);
    if (piece == nullptr)
    {
        return false;
    }

    if (fromRow == toRow && fromColumn == toColumn)
        {
            return false;
        }

    ChessPiece *targetPiece = board.at(toRow).at(toColumn);
    if (targetPiece != nullptr && targetPiece->getColor() == piece->getColor())
    {
        return false;
    }
    
    if (!piece->canMoveToLocation(toRow, toColumn))
    {
        return false;
    }

    return true;
}

//HELPER FUNCTION: PIECE CAPTURING
void ChessBoard::capturePiece(int row, int column)
{
    ChessPiece *piece = board.at(row).at(column);
    for (auto iter = pieces.begin(); iter != pieces.end(); iter++)
    {
        if (*iter == piece)
        {
            pieces.erase(iter);
            break;
        }
    }
    delete piece;
    board.at(row).at(column) = nullptr;
}

//DESTRUCTOR
ChessBoard::~ChessBoard() {
    for (auto& row : board) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

bool ChessBoard::isSquareUnderAttack(int row, int column, Color color)
{
    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            ChessPiece *opponentPiece = getPiece(i, j);
            if (opponentPiece != nullptr && opponentPiece->getColor() != color)
            {
                if (opponentPiece->canMoveToLocation(row, column))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void ChessBoard::updateCastlingFlags(ChessPiece *piece, int fromColumn)
{
    if (piece->getType() == Type::King)
    {
        if (piece->getColor() == White)
        {
            whiteKingMoved = true;
        }
        else
        {
            blackKingMoved = true;
        }
    }
    else if (piece->getType() == Type::Rook)
    {
        if (piece->getColor() == White)
        {
            if (fromColumn == 0)
                whiteRookLeftMoved = true;
            else if (fromColumn == numCols - 1)
                whiteRookRightMoved = true;
        }
        else
        {
            if (fromColumn == 0)
                blackRookLeftMoved = true;
            else if (fromColumn == numCols - 1)
                blackRookRightMoved = true;
        }
    }
}