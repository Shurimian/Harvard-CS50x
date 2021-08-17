#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string textIn = get_string("Text: ");
    float avgLetter = 0;
    float avgSentence = 0;
    float index = 0;
    float wordCount = 0;
    float spaceCount = 0;
    float letterCount = 0;
    float sentenceCount = 0;
    float ascii = 65;
    int grade = 0;

    // Loop checking each letter in word
    for (int i = 0, len = (int) strlen(textIn); i < len; ++i)
    {
        // Loop comparing each letter to ASCII
        for (int ii = 0; ii < 26; ++ii)
        {
            if ((int) textIn[i] == (ascii + ii) || (int) textIn[i] == (ascii + 32 + ii))
            {
                ++letterCount;
            }
            else if ((int) textIn[i] == 32)
            {
                ++spaceCount;
                ii = 26;
            }
            else if ((int) textIn[i] == 63 || (int) textIn[i] == 33 || (int) textIn[i] == 46)
            {
                ++sentenceCount;
                ii = 26;
            }
        }
    }

    wordCount = spaceCount + 1;

    avgLetter = (letterCount / wordCount) * 100;
    avgSentence = (sentenceCount / wordCount) * 100;
    index = (0.0588 * avgLetter - 0.296 * avgSentence - 15.8) + 0.5;
    grade = (int) index;

    if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}