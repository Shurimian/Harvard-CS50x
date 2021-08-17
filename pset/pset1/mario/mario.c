#include <stdio.h>
#include <cs50.h>

void spaces(int n);
void hash(int n);

int main(void)
{
    int size = get_int("Input a number between 1-8 inclusive: ");
    int space = 0;

    while (size < 1 || size > 8)
    {
        size = get_int("Try again. Input a number between 1-8 inclusive: ");
    }

    // For loop for # of layers

    for (int i = 0; i < size; ++i)
    {
        space = size - i;

        spaces(space);

        hash(i);

        printf("  ");

        hash(i);

        printf("\n");
    }
}

void spaces(int space)
{
    for (int ii = 0; ii < (space - 1); ++ii)
    {
        printf(" ");
    }
}

void hash(int i)
{
    for (int ii = 0; ii < (i + 1); ++ii)
    {
        printf("#");
    }
}