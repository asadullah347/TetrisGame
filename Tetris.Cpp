#include <iostream>
#include <windows.h>
using namespace std;

const int rows = 20;      // Height of the game board
const int columns = 10;   // Width of the game board

int board[rows][columns]; // 2D board: 0 = empty, 1 = filled

int pieceRow; // Current piece row position
int pieceCol; // Current piece column position

// All tetromino shapes (fixed 4x4 grid)
int tetrominoes[7][4][4] = {
    { // I piece
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // O piece
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // T piece
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // S piece
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // Z piece
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // J piece
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    { // L piece
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

int currentPiece[4][4]; // Currently active piece
int currentIndex = 0;   // Index of current piece (0~6)

// Print the board to console
void PrintBoard()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(board[i][j] == 0)
                cout << "."; // Empty cell
            else
                cout << "#"; // Filled cell
        }
        cout << endl;
    }
}

// Place a piece onto the board
void PlacePiece(int piece[4][4], int row, int col)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(piece[i][j] != 0)
                board[row + i][col + j] = piece[i][j];
        }
    }
}

// Check collision with walls, floor, or other blocks
bool CheckCollision(int piece[4][4], int row, int col)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(piece[i][j] != 0)
            {
                int newRow = row + i;
                int newCol = col + j;

                // Check boundary collision
                if(newRow >= rows || newCol < 0 || newCol >= columns)
                    return true;

                // Check collision with existing blocks
                if(board[newRow][newCol] != 0)
                    return true;
            }
        }
    }
    return false;
}

// Rotate piece 90 degrees clockwise
void RotatePiece(int piece[4][4])
{
    int temp[4][4];

    // Apply rotation
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            temp[j][3 - i] = piece[i][j];

    // Copy back to original piece
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            piece[i][j] = temp[i][j];
}

// Clear completed lines and shift everything down
void ClearLines()
{
    for(int i = rows - 1; i >= 0; i--)
    {
        bool full = true;

        for(int j = 0; j < columns; j++)
        {
            if(board[i][j] == 0)
            {
                full = false;
                break;
            }
        }

        if(full)
        {
            // Shift down all rows above
            for(int k = i; k > 0; k--)
                for(int j = 0; j < columns; j++)
                    board[k][j] = board[k - 1][j];

            // Reset top row to empty
            for(int j = 0; j < columns; j++)
                board[0][j] = 0;

            i++; 
        }
    }
}

// Spawn a new random piece
void SpawnPiece()
{
    currentIndex = rand() % 7;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            currentPiece[i][j] = tetrominoes[currentIndex][i][j];

    pieceRow = 0;
    pieceCol = 3;
}

int main()
{
    // Initialize board to empty
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < columns; j++)
            board[i][j] = 0;

    SpawnPiece();

    int counter = 0; // Used to control gravity timing

    while(true)
    {
        counter++;

        // Remove piece from previous position
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if(currentPiece[i][j] != 0)
                    board[pieceRow + i][pieceCol + j] = 0;

        // Handle player input
        if(GetAsyncKeyState('A') & 0x8000)
            if(!CheckCollision(currentPiece, pieceRow, pieceCol - 1))
                pieceCol--;

        if(GetAsyncKeyState('D') & 0x8000)
            if(!CheckCollision(currentPiece, pieceRow, pieceCol + 1))
                pieceCol++;

        if(GetAsyncKeyState('S') & 0x8000)
            if(!CheckCollision(currentPiece, pieceRow + 1, pieceCol))
                pieceRow++;

        if(GetAsyncKeyState('W') & 0x8000) // Rotate
        {
            RotatePiece(currentPiece);

            // Rollback if rotation causes collision
            if(CheckCollision(currentPiece, pieceRow, pieceCol))
            {
                RotatePiece(currentPiece);
                RotatePiece(currentPiece);
                RotatePiece(currentPiece);
            }
        }

        //gravity every 10 frames
        if(counter % 10 == 0)
        {
            if(!CheckCollision(currentPiece, pieceRow + 1, pieceCol))
            {
                pieceRow++;
            }
            else
            {
                // Lock piece and spawn a new one
                PlacePiece(currentPiece, pieceRow, pieceCol);
                ClearLines();
                SpawnPiece();
            }
        }

        // Place piece at new position
        PlacePiece(currentPiece, pieceRow, pieceCol);

        // Render
        system("cls");
        PrintBoard();

        Sleep(50); // 0.05 second delay for smoother gameplay
    }

    return 0;
}