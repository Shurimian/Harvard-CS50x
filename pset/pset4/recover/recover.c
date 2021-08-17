#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // Check for correct number of arguments

    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover.c cardInfo.raw %03.0f \n", 003.0);
        return 1;
    }

    // Open file to check for correct name

    FILE *fileIn = fopen(argv[1], "r");

    if (fileIn == NULL)
    {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 1;
    }

    int blockSize = 512;
    int loopCond = 0;
    float fileCount = 000.0;
    char nameBuffer[8];
    BYTE bytes[blockSize];
    FILE *fileOut;

    // Run loop while data is being read in

    while (fread(&bytes, blockSize, 1, fileIn))
    {
        // Check if valid jpeg

        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            loopCond = 1;
        }

        if (loopCond == 1)
        {
            // Create and open file with proper name
            if (fileCount != 0)
            {
                fclose(fileOut);
            }

            sprintf(nameBuffer, "%03.0f.jpg", fileCount);
            fileOut = fopen(nameBuffer, "a");
            ++fileCount;

            // Write data to file

            fwrite(&bytes, blockSize, 1, fileOut);

            loopCond = 2;
        }
        else if (loopCond == 2)
        {
            fwrite(&bytes, blockSize, 1, fileOut);
        }
    }

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}