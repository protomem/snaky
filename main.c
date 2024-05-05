#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define COLS 60
#define ROWS 30

void cursor_hide();
void cursor_show();
void cursor_move_back();
void render_table();

int main()
{
    cursor_hide();

    bool quit = false;
    while (!quit) {
        render_table();

        cursor_move_back();
    }

    cursor_show();

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
