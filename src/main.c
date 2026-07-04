/* ================================================================
 *   SNAKE GAME  -  Simple Console Version (C Language)
 *   Made for: Internship Project
 *   Platform: Windows (VS Code)
 * ================================================================
 *   Concepts used:
 *     - Arrays
 *     - Structures
 *     - Functions
 *     - Loops & Conditionals
 *     - File Handling (High Score save/load)
 *     - conio.h (keyboard input without pressing Enter)
 *
 *   How to compile (VS Code terminal):
 *       gcc snake.c -o snake.exe
 *   How to run:
 *       .\snake.exe
 * ================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>      // for kbhit(), getch()  -> Windows only
#include <windows.h>    // for Sleep()

#define WIDTH   30
#define HEIGHT  20
#define MAX_LENGTH 100

// ---------- Global Game Variables ----------
int gameOver;
int score, highScore;

int snakeX[MAX_LENGTH], snakeY[MAX_LENGTH];   // arrays to store snake body positions
int snakeLength;

int foodX, foodY;

char direction;   // 'U', 'D', 'L', 'R'

// ---------- Function Declarations ----------
void Setup();
void Draw();
void Input();
void Logic();
void GenerateFood();
void LoadHighScore();
void SaveHighScore();

// ================================================================
//  MAIN FUNCTION
// ================================================================
int main() {
    LoadHighScore();
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(120);   // controls game speed
    }

    if (score > highScore) {
        highScore = score;
        SaveHighScore();
    }

    printf("\n\n   GAME OVER!\n");
    printf("   Your Score : %d\n", score);
    printf("   High Score : %d\n\n", highScore);

    return 0;
}

// ================================================================
//  SETUP: initialize snake, score, direction
// ================================================================
void Setup() {
    gameOver = 0;
    direction = 'R';   // snake starts moving right

    score = 0;
    snakeLength = 3;

    // starting position roughly at center
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = WIDTH / 2 - i;
        snakeY[i] = HEIGHT / 2;
    }

    GenerateFood();
}

// ================================================================
//  DRAW: prints the game board, snake, food and score
// ================================================================
void Draw() {
    system("cls");   // clear screen (Windows)

    // top wall
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            if (x == 0) printf("#");   // left wall

            if (x == foodX && y == foodY) {
                printf("*");           // food
            }
            else {
                int printed = 0;
                for (int i = 0; i < snakeLength; i++) {
                    if (snakeX[i] == x && snakeY[i] == y) {
                        if (i == 0)
                            printf("O");   // head
                        else
                            printf("o");   // body
                        printed = 1;
                        break;
                    }
                }
                if (!printed) printf(" ");
            }

            if (x == WIDTH - 1) printf("#");  // right wall
        }
        printf("\n");
    }

    // bottom wall
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    printf("Score : %d      High Score : %d\n", score, highScore);
    printf("Controls: W A S D to move, X to quit\n");
}

// ================================================================
//  INPUT: reads keyboard key (non-blocking)
// ================================================================
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
            case 'W':
                if (direction != 'D') direction = 'U';
                break;
            case 's':
            case 'S':
                if (direction != 'U') direction = 'D';
                break;
            case 'a':
            case 'A':
                if (direction != 'R') direction = 'L';
                break;
            case 'd':
            case 'D':
                if (direction != 'L') direction = 'R';
                break;
            case 'x':
            case 'X':
                gameOver = 1;
                break;
        }
    }
}

// ================================================================
//  LOGIC: moves the snake, checks collisions, checks food
// ================================================================
void Logic() {
    int prevX = snakeX[0];
    int prevY = snakeY[0];
    int prev2X, prev2Y;

    // move head according to direction
    switch (direction) {
        case 'U': snakeY[0]--; break;
        case 'D': snakeY[0]++; break;
        case 'L': snakeX[0]--; break;
        case 'R': snakeX[0]++; break;
    }

    // move rest of the body (each segment follows the one ahead of it)
    for (int i = 1; i < snakeLength; i++) {
        prev2X = snakeX[i];
        prev2Y = snakeY[i];
        snakeX[i] = prevX;
        snakeY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // wall collision
    if (snakeX[0] < 0 || snakeX[0] >= WIDTH || snakeY[0] < 0 || snakeY[0] >= HEIGHT) {
        gameOver = 1;
    }

    // self collision
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = 1;
        }
    }

    // food collision -> grow snake
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score += 10;
        snakeLength++;
        if (snakeLength >= MAX_LENGTH) snakeLength = MAX_LENGTH - 1;
        GenerateFood();
    }
}

// ================================================================
//  GENERATE FOOD at a random empty position
// ================================================================
void GenerateFood() {
    int valid;
    do {
        valid = 1;
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;

        for (int i = 0; i < snakeLength; i++) {
            if (snakeX[i] == foodX && snakeY[i] == foodY) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

// ================================================================
//  FILE HANDLING: load and save high score
// ================================================================
void LoadHighScore() {
    FILE *fp = fopen("highscore.txt", "r");
    if (fp == NULL) {
        highScore = 0;
        return;
    }
    fscanf(fp, "%d", &highScore);
    fclose(fp);
}

void SaveHighScore() {
    FILE *fp = fopen("highscore.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", highScore);
        fclose(fp);
    }
}
