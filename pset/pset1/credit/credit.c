#include <stdio.h>
#include <cs50.h>


bool checkSum(long parser, long cardNum, long length)
{
    int sum1 = 0;
    int sum2 = 0;
    int tempVal = 0;
    long divisor = 100;

    for (int i = 0; i < parser; ++i)
    {
        tempVal = (cardNum % divisor) / (divisor / 10) * 2;

        if (tempVal < 10)
        {
            sum1 += tempVal;
        }
        else
        {
            sum1 += (tempVal % 10) + 1;
        }

        divisor *= 100;
    }

    divisor = 100;

    for (int i = 0; i < (length - parser); ++i)
    {
        sum2 += (cardNum % (divisor / 10)) / (divisor / 100);
        divisor *= 100;
    }

    if (((sum1 + sum2) % 10) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(void)
{
    long cardNum = get_long("Input a card number: ");
    long thirteen = 1000000000000;
    long fourteen = 10000000000000;
    long fifteen = 100000000000000;
    long sixteen = 1000000000000000;
    long seventeen = 10000000000000000;
    long digits = 0;
    long size = cardNum / thirteen;
    long val1 = 0;
    long val2 = 0;

    if (size < 1 || size >= 10000)
    {
        printf("INVALID\n");
    }
    else if (size >= 1 && size < 10)
    {
        val1 = (cardNum % fourteen) / (thirteen);

        if (checkSum(6, cardNum, 13) && val1 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (size >= 10 && size < 100)
    {
        val1 = (cardNum % fifteen) / (fourteen);

        if (checkSum(7, cardNum, 14) && val1 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (size >= 100 && size < 1000)
    {
        val1 = (cardNum % sixteen) / (fifteen);
        val2 = (cardNum % fifteen) / (fourteen);

        if (checkSum(7, cardNum, 15))
        {
            if (val1 == 3)
            {
                if (val2 == 4 || val2 == 7)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (val1 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (size >= 1000 && size < 10000)
    {
        val1 = (cardNum % seventeen) / (sixteen);
        val2 = (cardNum % sixteen) / (fifteen);

        if (checkSum(8, cardNum, 16))
        {
            if (val1 == 5)
            {
                if (val2 == 1 || val2 == 2 || val2 == 3 || val2 == 4 || val2 == 5)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else if (val1 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }

}