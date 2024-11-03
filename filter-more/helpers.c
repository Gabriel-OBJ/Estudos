#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int media;
    for (int i = 0; i < height; i++)
    {
        media = 0;
        for (int j = 0; j < width; j++)
        {
            media = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) /
                          (float) 3);
            image[i][j].rgbtRed = media;
            image[i][j].rgbtGreen = media;
            image[i][j].rgbtBlue = media;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE swap;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = image[i][j];
            image[i][j] = swap;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_cp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_cp[i][j] = image[i][j];
        }
    }

    int avgRed;
    int avgGreen;
    int avgBlue;
    int pixelCount;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avgRed = 0;
            avgGreen = 0;
            avgBlue = 0;
            pixelCount = 0;
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {

                    }
                    else
                    {
                        avgRed += image_cp[i + k][j + l].rgbtRed;
                        avgGreen += image_cp[i + k][j + l].rgbtGreen;
                        avgBlue += image_cp[i + k][j + l].rgbtBlue;
                        pixelCount++;
                    }
                }
            }
            image[i][j].rgbtRed = round(avgRed / (float) pixelCount);
            image[i][j].rgbtGreen = round(avgGreen / (float) pixelCount);
            image[i][j].rgbtBlue = round(avgBlue / (float) pixelCount);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_cp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_cp[i][j] = image[i][j];
        }
    }

    int gxKernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gyKernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int kernelNumber;
    int gxSumR;
    int gxSumG;
    int gxSumB;
    int gySumR;
    int gySumG;
    int gySumB;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            kernelNumber = 0;
            gxSumR = 0;
            gxSumG = 0;
            gxSumB = 0;
            gySumR = 0;
            gySumG = 0;
            gySumB = 0;
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        kernelNumber++;
                    }
                    else
                    {
                        gxSumR += image_cp[i + k][j + l].rgbtRed * gxKernel[kernelNumber];
                        gxSumG += image_cp[i + k][j + l].rgbtGreen * gxKernel[kernelNumber];
                        gxSumB += image_cp[i + k][j + l].rgbtBlue * gxKernel[kernelNumber];

                        gySumR += image_cp[i + k][j + l].rgbtRed * gyKernel[kernelNumber];
                        gySumG += image_cp[i + k][j + l].rgbtGreen * gyKernel[kernelNumber];
                        gySumB += image_cp[i + k][j + l].rgbtBlue * gyKernel[kernelNumber];

                        kernelNumber++;
                    }
                }
            }
            if (round(sqrt((gxSumR * gxSumR)  + (gySumR * gySumR)))  > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(sqrt(pow(gxSumR, 2) + pow(gySumR, 2)));
            }

            if (round(sqrt((gxSumG * gxSumG) + (gySumG * gySumG))) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(sqrt(pow(gxSumG, 2) + pow(gySumG, 2)));
            }

            if (round(sqrt((gxSumB * gxSumB) + (gySumB * gySumB))) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(sqrt(pow(gxSumB, 2) + pow(gySumB, 2)));
            }
        }
    }
    return;
}
