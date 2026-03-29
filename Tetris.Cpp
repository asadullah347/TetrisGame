#include <iostream>
#include <windows.h>
using namespace std;

const int rows = 20;      // Height of the game board
const int columns = 10;   // Width of the game board

int board[rows][columns]; // 2D board: 0 = empty, 1 = filled

int pieceRow; // Current piece row position
int pieceCol; // Current piece column position


int currentPiece[4][4]; // Currently active piece
int currentIndex = 0;   // Index of current piece (0~6)

float score = 0; //player score, increases by 100 for each cleared line and 1 per second.

// All tetromino shapes (fixed 4x4 grid)
int tetrominoes[7][4][4] =
{
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

//this is using a double buffer. double buffering creates an off-screen buffer where the entire frame is drawn before being displayed on the screen.
///This technique helps to eliminate flickering and provides smoother animations, as the user only sees the final rendered frame instead of the intermediate drawing steps.
//just like drawing on a piece of paper before showing it to everyone. 
void PrintBoardBuffered()
{
    char buffer[rows][columns * 3]; // each cell takes 3 chars: " . " or " # "

    // Fill buffer
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            buffer[i][j*3]   = ' ';
            buffer[i][j*3+1] = board[i][j] == 0 ? '.' : '#';
            buffer[i][j*3+2] = ' ';
        }
    }

    // Print buffer
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns * 3; j++)
        {
            cout << buffer[i][j];
        }
        cout << endl;
    }

    cout << "Score: " << score << endl;
}

// Place a piece onto the board
// This is called when a piece locks in place
// It updates the board array with the piece's blocks
// The piece is represented as a 4x4 grid, and we add it to the board at the specified row and column position
// We loop through the 4x4 piece grid, and for each non-zero block, we set the corresponding position on the board to the piece's value (which is 1 in this case)
// 조각이 떨어진후 보드에 조각을 그리기.
void PlacePiece(int piece[4][4], int rowPos, int colPos)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(piece[i][j] != 0)
                board[rowPos + i][colPos + j] = piece[i][j];
        }
    }
}

// Check collision with walls, floor, or other blocks
// row represent the top-left corner of the piece on the board, and col represent the leftmost column of the piece on the board
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
            // Move everything down
            for(int k = i; k > 0; k--)
            {
                for(int j = 0; j < columns; j++)
                {
                    board[k][j] = board[k - 1][j];
                }
            }

            // Clear top row
            for(int j = 0; j < columns; j++)
                board[0][j] = 0;

            i++; // recheck same row
            score += 10; 
        }
    }
}

// Spawn a new random piece
void SpawnPiece()
{
    //currentIndex is assigned a random value between 0 and 6, which corresponds to one of the seven tetromino shapes defined in the tetrominoes array. This ensures that each time a new piece is spawned, it will be randomly selected from the available shapes.
    currentIndex = rand() % 7;

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            currentPiece[i][j] = tetrominoes[currentIndex][i][j];

    pieceRow = 0; // represent the top row of the board, where new pieces will appear
    pieceCol = 3; // represent the middle column of the board, where new pieces will be centered when they spawn
}



void UpdateScore()
{
    score += 1; // Increase score by 1 for every second survived
}

int main()
{
    // Initialize board to empty
    srand((unsigned int)time(NULL)); // random seed
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < columns; j++)
            board[i][j] = 0;

    SpawnPiece();

    int counter = 0; // Used to control gravity timing

    while(true)
    {
        counter++;

        // Remove piece from previous position
        // because we will redraw it at the new position after handling input and gravity
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

        // Gravity every 10 frames (~0.5s)
        if(counter%10==0){
            if(!CheckCollision(currentPiece,pieceRow+1,pieceCol))
                pieceRow++;
            else{
                PlacePiece(currentPiece,pieceRow,pieceCol);
                ClearLines();
                SpawnPiece();
            }
        }

        // Place piece at new position
        PlacePiece(currentPiece, pieceRow, pieceCol);

        // Render
        COORD coord = {0, 0}; // top-left corner
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        PrintBoardBuffered();

        // Update score every second
        if(counter % 20 == 0) // 20 frames × 50ms = 1 second
            UpdateScore();

        Sleep(50); // 0.05 second delay for smoother gameplay
    }

    return 0;
}