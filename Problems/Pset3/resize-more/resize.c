// Resizes a BMP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

// Global functions
int isAFloat(char *ptr, int size);
float adjustFloat(float resizeScale);
int getResizeValueFromFloat(float resizeScale);
BITMAPFILEHEADER copyBitmapFileHeader(BITMAPFILEHEADER bf);
BITMAPINFOHEADER copyBitmapInfoHeader(BITMAPINFOHEADER bi);
void modifyHeadersAndPaddingFromResizeValues(BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi, int *padding, int resizeScale, int isResizeUp);

int isDebug = 0;

int main(int argc, char *argv[])
{
    // Ensure proper usage: Check args, check float
    int isValid = 1;

    if (argc != 4)
    {
        isValid = 0;
    }

    if (!isAFloat(argv[1], strlen(argv[1])))
    {
        printf("That was not a float value.\n");
        isValid = 0;
    }

    if (!isValid)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }
    else if (isDebug >= 10)   // Only displays while it is debug
    {
        printf("Valid\n");
    }

    // Save arguments
    float resizeFloat = adjustFloat(atof(argv[1]));
    char *infile = argv[2];
    char *outfile = argv[3];

    // Proper resize Value
    int isResizeUp = 1;
    int resizeValue = 0;

    // Convert the resizeFloat into usable scaler
    if (resizeFloat < 1)
    {
        isResizeUp = 0;
    }
    resizeValue = getResizeValueFromFloat(resizeFloat);


    if (isDebug >= 10) // Debugging purposes
    {
        printf("The resizeValue is: %d, and resize up is: %d\n", resizeValue, isResizeUp);
    }

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inbf;
    fread(&inbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inbi;
    fread(&inbi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inbf.bfType != 0x4d42 || inbf.bfOffBits != 54 || inbi.biSize != 40 ||
        inbi.biBitCount != 24 || inbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Determine input file's padding for scanlines
    int inpadding = (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Create the output file's headers
    BITMAPFILEHEADER outbf = copyBitmapFileHeader(inbf);
    BITMAPINFOHEADER outbi = copyBitmapInfoHeader(inbi);
    int outpadding = 0;

    modifyHeadersAndPaddingFromResizeValues(&outbf, &outbi, &outpadding, resizeValue, isResizeUp);

    if (isDebug >= 9) // For debug
    {
        printf("==Input file==\n");
        printf("Size of bmp: %u\n", inbf.bfSize);
        printf("Size of bf: %lu\n", sizeof(BITMAPFILEHEADER));
        printf("Size of bi: %lu\n", sizeof(BITMAPINFOHEADER));
        printf("Height: %d\n", inbi.biHeight);
        printf("Width: %d\n", inbi.biWidth);
        printf("biSizeImage: %u\n", inbi.biSizeImage);
        printf("Size of Triple Array: %lu\n", abs(inbi.biHeight) * ((inbi.biWidth) * sizeof(RGBTRIPLE) + inpadding));
        printf("==Output file==\n");
        printf("Resize value: %d", resizeValue);
        printf(" isResizeUp: %d\n", isResizeUp);
        printf("Size of bmp: %u\n", outbf.bfSize);
        printf("Size of bf: %lu\n", sizeof(BITMAPFILEHEADER));
        printf("Size of bi: %lu\n", sizeof(BITMAPINFOHEADER));
        printf("Height: %d\n", outbi.biHeight);
        printf("Width: %d\n", outbi.biWidth);
        printf("Padding: %d\n", outpadding);
        printf("biSizeImage: %u\n", outbi.biSizeImage);
        printf("Size of Triple Array: %lu\n", abs(outbi.biHeight) * ((outbi.biWidth) * sizeof(RGBTRIPLE) + outpadding));
    }

    // Write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Writing RGBTRIPLES
    // Algorithm to resize up
    if (isResizeUp)
    {
        // Iterate over infile's scanlines
        for (int i = 0, biHeight = abs(inbi.biHeight); i < biHeight; i++)
        {
            int heightRepeat = 0;

            while (heightRepeat < resizeValue)    // repeat resizeValue amount of times
            {
                // Put cursor back at the beginning of scanline
                if (heightRepeat != 0)
                {
                    fseek(inptr, -1 * inbi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
                }

                // Iterate over pixels in scanline
                for (int j = 0; j < inbi.biWidth; j++)
                {
                    RGBTRIPLE triple;

                    // Read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                    // Write RGB triple to outfile resizeValue number of times
                    int widthRepeat = 0;
                    while (widthRepeat < resizeValue)
                    {
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                        widthRepeat++;
                    }
                }

                // Add outfile padding
                for (int k = 0; k < outpadding; k++)
                {
                    fputc(0x00, outptr);
                }
                heightRepeat++;
            }

            // Skip over infile padding, if any
            fseek(inptr, inpadding, SEEK_CUR);
        }
    }
    else // Algorithm for resize down
    {
        // Number of bytes to skip (Multiplied by resizeValue - 1 since the 1 is the line that was just processed)
        long heightSkip = (inbi.biWidth * sizeof(RGBTRIPLE) + inpadding) * (resizeValue - 1);

        // Number of bytes to skip (Multiplied by resizeValue - 1 since 1 is the pixel that was just processed)
        long widthSkip = (resizeValue - 1) * sizeof(RGBTRIPLE);

        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(inbi.biHeight); i < biHeight; i += resizeValue)
        {
            // Skip over scanline.
            // Put at the beginning so that it only skips when i < bi.Height, and it skips only after previous iteration
            if (i != 0)
            {
                fseek(inptr, heightSkip, SEEK_CUR);
            }

            // Iterate over pixels in scanline
            for (int j = 0; j < inbi.biWidth; j += resizeValue)
            {
                // Skip over pixels.
                // Put at the beginning so that it only skips when i < bi.Height, and it skips only after previous iteration
                if (j != 0)
                {
                    fseek(inptr, widthSkip, SEEK_CUR);
                }

                RGBTRIPLE triple;

                // Read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // Write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // Skip over infile padding, if any
            fseek(inptr, inpadding, SEEK_CUR);

            // Skip over remaining triples
            int remainingTriples = inbi.biWidth % resizeValue;
            if (remainingTriples == 0)
            {
                fseek(inptr, widthSkip, SEEK_CUR);
            }
            else
            {
                fseek(inptr, (remainingTriples - 1) * sizeof(RGBTRIPLE), SEEK_CUR);
            }

            // Add outfile padding
            for (int k = 0; k < outpadding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);

    return 0;
}

// Return whether a char is a number/null/period
int charIsNumeric(char character)
{
    if (character == '.' || (character >= '0' && character <= '9') || character == '\0')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// Return whether a string is a float
int isAFloat(char *ptr, int size)
{
    int isValid = 1;

    // Go through whole array to see if it's a float
    for (int index = 0; index <= size; index++)
    {
        // Get character at pointer
        char character = *(ptr + index);

        if (!charIsNumeric(character))
        {
            isValid = 0;
            break;
        }
    }

    return isValid;
}

// Keep float if under 1, else, get rid of numbers after periods
float adjustFloat(float resizeScale)
{
    if (resizeScale >= 1)
    {
        // Gets rid of floating values
        int integer = resizeScale;
        return integer;
    }
    else
    {
        // If under 1, then simply return the resizeScale
        return resizeScale;
    }
}

// Return a usable scale for main
int getResizeValueFromFloat(float resizeScale)
{
    if (resizeScale < 1)
    {
        return 1 / resizeScale;
    }
    else
    {
        return resizeScale;
    }

}

void modifyHeadersAndPaddingFromResizeValues(BITMAPFILEHEADER *bf, BITMAPINFOHEADER *bi, int *padding, int resizeScale, int isResizeUp)
{
    int width = 0;
    int height = 0;

    if (isResizeUp)
    {
        width = (*bi).biWidth * resizeScale;
        height = (*bi).biHeight * resizeScale;
    }
    else
    {
        // Keep height sign
        int heightIsPositive = 1;
        height = (*bi).biHeight;
        if (height < 0)
        {
            heightIsPositive = 0;
        }

        // Modify width and height
        width = (*bi).biWidth / resizeScale;
        height = height / resizeScale;

        // Get a non-zero value
        if (width == 0)
        {
            width = 1;
        }
        if (height == 0)
        {
            if (heightIsPositive)
            {
                height = 1;
            }
            else
            {
                height = -1;
            }
        }

    }

    // Change width and height
    (*bi).biWidth = width;
    (*bi).biHeight = height;

    // Compute padding
    *padding = (4 - ((*bi).biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Compute RBG triple array size
    (*bi).biSizeImage = abs((*bi).biHeight) * ((*bi).biWidth * sizeof(RGBTRIPLE) + (*padding));

    // Compute image size
    (*bf).bfSize = (*bf).bfOffBits + (*bi).biSizeImage;

}

// Return a copy of BITMAPFILEHEADER
BITMAPFILEHEADER copyBitmapFileHeader(BITMAPFILEHEADER bf)
{
    BITMAPFILEHEADER copy;
    copy.bfType = bf.bfType;
    copy.bfSize = bf.bfSize;
    copy.bfReserved1 = bf.bfReserved1;
    copy.bfReserved2 = bf.bfReserved2;
    copy.bfOffBits = bf.bfOffBits;

    return copy;
}

// Return a copy of BITMAPINFOHEADER
BITMAPINFOHEADER copyBitmapInfoHeader(BITMAPINFOHEADER bi)
{
    BITMAPINFOHEADER copy;
    copy.biSize = bi.biSize;
    copy.biWidth = bi.biWidth;
    copy.biHeight = bi.biHeight;
    copy.biPlanes = bi.biPlanes;
    copy.biBitCount = bi.biBitCount;
    copy.biCompression = bi.biCompression;
    copy.biSizeImage = bi.biSizeImage;
    copy.biXPelsPerMeter = bi.biXPelsPerMeter;
    copy.biYPelsPerMeter = bi.biYPelsPerMeter;
    copy.biClrUsed = bi.biClrUsed;
    copy.biClrImportant = bi.biClrImportant;

    return copy;
}