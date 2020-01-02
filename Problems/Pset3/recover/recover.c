#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Byte type for buffer
typedef uint8_t byte;
const int jpegBlockSize = 512;

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover rawfile");
        return 1;
    }

    // Open File
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open file.");
        fclose(inptr);
        return 2;
    }

    int foundJpeg = 0;
    char *filename = malloc(sizeof(char) * strlen("%03i.jpg"));
    int numberCounter = 0;
    FILE *imgPtr;
    byte *currentBuffer = malloc(sizeof(char) * 512);

    while (fread(currentBuffer, jpegBlockSize, 1, inptr) == 1)
    {
        // Check buffer
        int isJPEGstart = *currentBuffer == 0xff && *(currentBuffer + 1) == 0xd8 && *(currentBuffer + 2) == 0xff
                            && (*(currentBuffer + 3) & 0xf0) == 0xe0;

        if (isJPEGstart)
        {
            // Close previous pointer
            if (foundJpeg)
            {
                fclose(imgPtr);
                foundJpeg = 0;
            }

            // Create new file
            sprintf(filename, "%03i.jpg", numberCounter++);
            imgPtr = fopen(filename, "w");
            foundJpeg = 1;
        }

        if (foundJpeg && imgPtr != NULL)
        {
            // Write data into jpeg file
            fwrite(currentBuffer, jpegBlockSize, 1, imgPtr);
        }
    }

    // Close for final jpeg
    if (foundJpeg && imgPtr != NULL)
    {
        fclose(imgPtr);
    }

    // End of program
    fclose(inptr);
    free(currentBuffer);
    free(filename);
    return 0;
}