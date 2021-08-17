#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long sizeI = get_long("Enter a initial population size: ");
    long sizeF = 0;
    long sizeC = 0;
    long yearCount = 0;

    while (sizeI < 9)
    {
        sizeI = get_long("Initial population must be 9 or greater. Reenter size: ");
    }

    sizeC = sizeI;

    sizeF = get_long("Enter a final population size: ");

    while (sizeF < sizeI)
    {
        sizeF = get_long("Final population must be equal or greater than initial population. Reenter size: ");
    }

    while (sizeC < sizeF)
    {
        sizeC += sizeI/3 - sizeI/4;
        sizeI = sizeC;
        ++yearCount;
    }

    printf("Years: %li \n", yearCount);
}