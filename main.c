#include "saper.h"
#include <ncurses.h>


int MAX_ROWS, MAX_COLS, MINES;

int main(int argc, char* argv[])
{
    MAX_ROWS = strtol(argv[1], NULL, 10);
    MAX_COLS = strtol(argv[2], NULL, 10);
    MINES = strtol(argv[3], NULL, 10);
    int slim = 0;
    if (argc == 5)
        if (strcmp(argv[4], "--slim") == 0)
            slim = 1;

    start(MAX_ROWS, MAX_COLS, MINES);

    // Seed the random number generator
    srand(time(FALSE));

    // Initialize ncursed
    initscr();
    curs_set(0);
    if (MINES >= MAX_COLS * MAX_ROWS) {
        printw("TOO MUCH BOMBS\n\n");
        // Wait for the user to press a key before exiting
        printw("Press any key to exit...\n");
        getch();

        // Clean up ncursed
        endwin();
        return 0;
    }

    // Initialize colors
    initialize_colors();

    // Enable the use of arrow keys
    keypad(stdscr, TRUE);

    // Set up the board
    char board[MAX_ROWS][MAX_COLS];
    int rows = MAX_ROWS;
    int cols = MAX_COLS;
    int mines = MINES;

    int b_flagi;
    int sap[MAX_ROWS][MAX_COLS];
    int to[MAX_ROWS][MAX_COLS];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            to[i][j] = 0;
        }
    }

    setup_board(board, sap, rows, cols, mines);

    // Main game loop
    int row, col;

    getyx(stdscr, row, col);
    while (1 != 0) {

        // Draw the board
        draw_board(board, rows, cols, row, col, slim, sap);

        int action = wgetch(stdscr);

        // Handle arrow key input
        if (action == KEY_LEFT && col > 0) {
            col--;
        } else if (action == KEY_RIGHT && col < cols - 1) {
            col++;
        } else if (action == KEY_UP && row > 0) {
            row--;
        } else if (action == KEY_DOWN && row < rows - 1) {
            row++;
        }

        if (action == ' ' && sap[row][col] == 9 && uncovered(board) == 0) {
            zero_move(row, col, sap, board, rows, cols);
        }

        // Update the board
        if (action == ' ') {
            if (sprawdz(row, col, sap, board)) {
                draw_board(board, rows, cols, row, col, slim, sap);
                gameover();
                // Wait for the user to press a key before exiting
                printw("Press any key to exit...\n");
                getch();

                // Clean up ncursed
                endwin();
                return 0;
            }
        }
        update_board(board, sap, to, rows, cols, &row, &col, action);
        if (czy_koniec(b_flagi, board) == MAX_COLS * MAX_ROWS - MINES) {
            draw_board(board, rows, cols, row, col, slim, sap);
            win();
            // Wait for the user to press a key before exiting
            printw("Press any key to exit...\n");
            getch();

            // Clean up ncursed
            endwin();
            return 0;
        }
    }
}