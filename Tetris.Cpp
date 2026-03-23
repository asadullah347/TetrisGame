#include <iostream>
#include <windows.h> // for Sleep() and system("cls")
using namespace std;


const int rows = 20; //Height of the board
const int columns = 10; //Width of the board
int board[rows][columns]; //Represented as a 2D array. empty (0) or occupied (1)

// where the current piece is located
int pieceRow; 
int pieceCol; 

// Tetromino shapes represented as 4x4 matrices
wchar_t tetrominoShapes[7][4]; // 7 shapes, each represented as 4 rows of wide characters
int tetromino_I[4][4] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
};




void PrintBoard()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(board[i][j] == 0)
                cout << "."; // Empty cell
            else
            cout << board[i][j] << " "; //filled cell
        }
        cout << endl;
    }
}

// Function to place a piece on the board at a given position
void PlacePiece(int piece[4][4], int pieceRow, int pieceCol)
{
    for(int i = 0; i < 4; i++)
    {       // iterate rows of piece
        for(int j = 0; j < 4; j++)
        {   // iterate cols of piece
            if(piece[i][j] != 0)
            {     // only draw filled cells
                board[pieceRow + i][pieceCol + j] = piece[i][j];
            }
        }
    }
}


bool CheckCollision(int piece[4][4], int pieceRow, int pieceCol)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(piece[i][j] != 0)
            {
                int boardRow = pieceRow + i;
                int boardCol = pieceCol + j;
                // Check boundaries
                if(boardRow >= rows || boardCol < 0 || boardCol >= columns)
                    return true; // Collision with walls or floor
                if(board[boardRow][boardCol] != 0)
                    return true; // Collision with existing pieces
            }
        }
    }
    return false; // No collision
}


int main()
{
    // Initialize the board to empty
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            board[i][j] = 0;
        }
    }

    pieceRow = 0;
    pieceCol = 3;

    while (true)
    {
        // 1. Remove piece from old position
        for (int i = 0; i < 4; i++) 
        {
            for (int j = 0; j < 4; j++)
            {
                if(tetromino_I[i][j] != 0)
                {
                    board[pieceRow + i][pieceCol + j] = 0;
                }
            }
        }
        // 2. Try to move down
        if(!CheckCollision(tetromino_I, pieceRow + 1, pieceCol))
        {
            pieceRow++; // Move down
        }
        else
        {
            // collision detected, freeze piece
            PlacePiece(tetromino_I, pieceRow, pieceCol);
            break;
        }

        // 3. Place piece at new position
        PlacePiece(tetromino_I, pieceRow, pieceCol);

        // 4. Print board
        system("cls");
        PrintBoard();

        // 5. Wait a bit
        Sleep(500); // 0.5 second delay
    }
    
    cout << "Piece landed!" << endl;
    return 0;
}