#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <cs50.h>

int main(void)
{
    char *string1 = get_string("Enter String 1: ");
    char *string2 = get_string("Enter String 2: ");

    printf("%i \n", strcasecmp(string1, string2));

    return 0;
}