#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

bool same(int char1, int char2);
bool alphabet(int charCheck);

int main(int argc, string argv[])
{
    // Checks for correct # of arguments

    if (argc == 2)
    {
        // Checks for correct key size

        if ((int) strlen(argv[1]) != 26)
        {
            printf("ERROR: Key must contain 26 letters\n");
            return 1;
        }
        else
        {
            int errorCheck = 0;
            int charCheck = 0;
            int matchingChar = 0;

            // Loop goes through each character of the key

            for (int i = 0, len = strlen(argv[1]); i < len; ++i)
            {
                charCheck = (int) argv[1][i];

                // Checks if key contains non-alphabetical character

                if (charCheck < 65 || charCheck > 122 || alphabet(charCheck))
                {
                    errorCheck = 1;
                    i = strlen(argv[1]);
                }
                else
                {
                    // Checks for repeating alphabetical characters

                    for (int ii = 0; ii < 26; ++ii)
                    {
                        if (charCheck == (int) argv[1][ii] || same(charCheck, (int) argv[1][ii]))
                        {
                            ++matchingChar;
                        }
                    }

                    if (matchingChar > 26)
                    {
                        errorCheck = 2;
                        i = strlen(argv[1]);
                    }
                }
            }

            // Displays different error message depending on which failure mode

            if (errorCheck == 1)
            {
                printf("ERROR: Key must only use alphabetical characters. \n");
                return 1;
            }
            else if (errorCheck == 2)
            {
                printf("ERROR: Key must not have repeating characters. \n");
                return 1;
            }
            else
            {
                // Actual code for the cipher

                string textIn = get_string("plaintext: ");
                string textOut = textIn;
                int charMap = 0;
                int currentLetter = 0;
                int key[26];

                // Map the alphabet to the provided key

                for (int i = 0; i < 26; ++i)
                {
                    charMap = (int) argv[1][i];

                    if (charMap < 91)
                    {
                        key[i] = charMap - (65 + i);
                    }
                    else
                    {
                        key[i] = (charMap - 32) - (65 + i);
                    }
                }

                // Encode the original text

                // Loop through each letter in word

                for (int i = 0, len = strlen(textIn); i < len; ++i)
                {
                    currentLetter = (int) textIn[i];

                    // Check which letter it is

                    for (int ii = 0; ii < 26; ++ii)
                    {
                        if (currentLetter == 65 + ii)
                        {
                            textOut[i] = (int) textIn[i] + key[ii];
                        }
                        else if (currentLetter == 97 + ii)
                        {
                            textOut[i] = (int) textIn[i] + key[ii];
                        }
                    }
                }

                printf("ciphertext: ");

                for (int i = 0, len = strlen(textIn); i < len; ++i)
                {
                    printf("%c", textOut[i]);
                }

                printf("\n");
            }
        }
    }
    else
    {
        printf("ERROR: Incorrect number of arguments.\n");
        return 1;
    }
}

// Function returns true if characters are same regardless of case. Accepts integer ASCII

bool same(int char1, int char2)
{
    int condition = abs(char1 - char2);

    if (condition == 32)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Checks if character is an ascii letter

bool alphabet(int charCheck)
{
    switch (charCheck)
    {
        case 91: case 92: case 93: case 94: case 95: case 96:
            return true;
    }

    return false;
}