#include <iostream>
using namespace std;


const int rows = 20; //Height of the board
const int columns = 10; //Width of the board
int board[rows][columns]; //Represented as a 2D array. empty (0) or occupied (1)


// Tetromino shapes represented as 4x4 matrices
wchar_t tetrominoShapes[7][4]; // 7 shapes, each represented as 4 rows of wide characters
int tetromino_I[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

// For simplicity, we will only define O and I shapes for now.
int pieceRow = 0; // top of the board
int pieceCol = 3; // somewhere near center





void PrintBoard() {
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
void PlacePiece(int piece[4][4], int pieceRow, int pieceCol) {
    for(int i = 0; i < 4; i++) {       // iterate rows of piece
        for(int j = 0; j < 4; j++) {   // iterate cols of piece
            if(piece[i][j] != 0) {     // only draw filled cells
                board[pieceRow + i][pieceCol + j] = piece[i][j];
            }
        }
    }
}


//Cheack collision of piece with board boundaries
bool CheckCollision()
{
    
}
    

int main()
{
    // Initialize the board to empty
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = 0;
        }
    }

    PlacePiece(tetromino_I, pieceRow, pieceCol);
    PrintBoard();
    return 0;
}