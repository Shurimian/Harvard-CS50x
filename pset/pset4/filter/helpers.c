#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average = 0;

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width; ++ii)
        {
            average = (((float) image[i][ii].rgbtBlue + image[i][ii].rgbtGreen + image[i][ii].rgbtRed) / 3) + 0.5;

            image[i][ii].rgbtBlue = average;
            image[i][ii].rgbtGreen = average;
            image[i][ii].rgbtRed = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempVal;

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width / 2; ++ii)
        {
            tempVal = image[i][ii];
            image[i][ii] = image[i][width - ii - 1];
            image[i][width - ii - 1] = tempVal;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int tempRed = 0;
    int tempGreen = 0;
    int tempBlue = 0;
    float divisor = 0;
    RGBTRIPLE newImage[height][width];

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width; ++ii)
        {
            divisor = tempBlue = tempGreen = tempRed = 0;

            for (int j = -1; j <= 1; ++j)
            {
                for (int k = -1; k <= 1; ++k)
                {
                    if (i + j >= 0 && i + j < height && ii + k >= 0 && ii + k < width)
                    {
                        tempBlue += image[i + j][ii + k].rgbtBlue;
                        tempGreen += image[i + j][ii + k].rgbtGreen;
                        tempRed += image[i + j][ii + k].rgbtRed;
                        ++divisor;
                    }
                }
            }

            newImage[i][ii].rgbtBlue = (tempBlue / divisor) + 0.5;
            newImage[i][ii].rgbtGreen = (tempGreen / divisor) + 0.5;
            newImage[i][ii].rgbtRed = (tempRed / divisor) + 0.5;
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width; ++ii)
        {
            image[i][ii] = newImage[i][ii];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float blueX, blueY, greenX, greenY, redX, redY = 0;
    float r, g, b = 0;
    int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE newImage[height][width];

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width; ++ii)
        {
            blueX = blueY = greenX = greenY = redX = redY = 0;

            for (int j = -1; j <= 1; ++j)
            {
                for (int k = -1; k <= 1; ++k)
                {
                    if (i + j >= 0 && i + j < height && ii + k >= 0 && ii + k < width)
                    {
                        blueX += image[i + j][ii + k].rgbtBlue * kernelX[1 + j][1 + k];
                        greenX += image[i + j][ii + k].rgbtGreen * kernelX[1 + j][1 + k];
                        redX += image[i + j][ii + k].rgbtRed * kernelX[1 + j][1 + k];

                        blueY += image[i + j][ii + k].rgbtBlue * kernelY[1 + j][1 + k];
                        greenY += image[i + j][ii + k].rgbtGreen * kernelY[1 + j][1 + k];
                        redY += image[i + j][ii + k].rgbtRed * kernelY[1 + j][1 + k];
                    }
                }
            }

            b = round(hypot(blueX, blueY));
            g = round(hypot(greenX, greenY));
            r = round(hypot(redX, redY));

            if (r > 255)
            {
                r = 255;
            }
            if (g > 255)
            {
                g = 255;
            }
            if (b > 255)
            {
                b = 255;
            }

            newImage[i][ii].rgbtBlue = b;
            newImage[i][ii].rgbtGreen = g;
            newImage[i][ii].rgbtRed = r;
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int ii = 0; ii < width; ++ii)
        {
            image[i][ii] = newImage[i][ii];
        }
    }

    return;
}
