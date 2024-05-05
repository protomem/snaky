#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define COLS 60
#define ROWS 30

void cursor_hide();
void cursor_show();
void cursor_move_back();
struct termios init_term();
void deinit_term(struct termios* term);
void render_table();

int32_t main()
{
    cursor_hide();
    struct termios term = init_term();

    int32_t x[1000], y[1000];

    bool quit = false;
    while (!quit) {
        render_table();

        cursor_move_back();

        int32_t head = 0, tail = 0;
        x[head] = COLS / 2;
        y[head] = ROWS / 2;

        bool game_over = false;
        int32_t xdir = 1, ydir = 0;
        int32_t applex = -1, appley = 0;

        while (!quit && !game_over) {
            if (applex < 0) {
                applex = rand() % COLS;
                appley = rand() % ROWS;

                for (int32_t i = tail; i != head; i = (i + 1) % 1000)
                    if (x[i] == applex && y[i] == appley)
                        applex = -1;

                if (applex >= 0) {
                    // Draw apple
                    printf("\e[%iB\e[%iC❤", appley + 1, applex + 1);
                    printf("\e[%iF", appley + 1);
                }
            }

            // Clear snake tail
            printf("\e[%iB\e[%iC·", y[tail] + 1, x[tail] + 1);
            printf("\e[%iF", y[tail] + 1);

            if (x[head] == applex && y[tail] == appley) {
                applex = -1;
                printf("\a");
            } else {
                tail = (tail + 1) % 1000;
            }

            int32_t new_head = (head + 1) % 1000;
            x[new_head] = (x[head] + xdir + COLS) % COLS;
            y[new_head] = (y[head] + ydir + ROWS) % ROWS;
            head = new_head;

            for (int32_t i = tail; i != head; i = (i + 1) % 1000)
                if (x[i] == x[head] && y[i] == y[head])
                    game_over = true;

            // Draw head
            printf("\e[%iB\e[%iC▓", y[head] + 1, x[head] + 1);
            printf("\e[%iF", y[head] + 1);
            fflush(stdout);

            usleep(6 * 1000000 / 60);

            // Read keyboard input
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

            if (FD_ISSET(STDIN_FILENO, &fds)) {
                int32_t input = getchar();
                if (input == 'q') {
                    quit = true;
                } else if (input == 'h' && xdir != -1) { // left
                    xdir = -1;
                    ydir = 0;
                } else if (input == 'l' && xdir != 1) { // right
                    xdir = 1;
                    ydir = 0;
                } else if (input == 'j' && ydir != 1) { // down
                    xdir = 0;
                    ydir = 1;
                } else if (input == 'k' && ydir != -1) { // up
                    xdir = 0;
                    ydir = -1;
                }
            }
        }

        if (!quit) {
            // Show game over
            printf("\e[%iB\e[%iC Game Over!", ROWS / 2, COLS / 2 - 5);
            printf("\e[%iF", ROWS / 2);
            fflush(stdout);
            getchar();
        }
    }

    cursor_show();
    deinit_term(&term);

    return EXIT_SUCCESS;
}

void cursor_hide()
{
    printf("\e[?25l");
}

void cursor_show()
{
    printf("\e[?25h");
}

void cursor_move_back()
{
    printf("\e[%iA", ROWS + 2);
}

struct termios init_term()
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    return oldt;
}

void deinit_term(struct termios* term)
{
    tcsetattr(STDIN_FILENO, TCSANOW, term);
}

void render_table()
{
    // Top border
    printf("┌");
    for (size_t i = 0; i < COLS; i++)
        printf("─");
    printf("┐\n");

    // Left/Right borders and fields
    for (size_t i = 0; i < ROWS; i++) {
        printf("│");
        for (size_t j = 0; j < COLS; j++)
            printf("·");
        printf("│\n");
    }

    // Bottom border
    printf("└");
    for (size_t i = 0; i < COLS; i++)
        printf("─");
    printf("┘\n");
}
