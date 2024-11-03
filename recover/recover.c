#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover file");
        return 1;
    }

    FILE *img = fopen(argv[1], "r");
    FILE *JPG;
    BYTE B[512];
    int fileBuffer = 0;
    char fileName[9];
    bool open = false;

    while (fread(&B, sizeof(BYTE), 512, img) == 512)
    {
        if (B[0] == 0xff && B[1] == 0xd8 && B[2] == 0xff && B[3] >= 0xe0 && B[3] <= 0xef)
        {
            if (open == true)
            {
                fclose(JPG);
                open = false;
            }
            sprintf(fileName, "%03i.jpg", fileBuffer);
            fileBuffer++;

            JPG = fopen(fileName, "w");
            if (JPG == NULL)
            {
                return 1;
            }
            open = true;

            fwrite(&B, sizeof(BYTE), 512, JPG);
        }
        else if (open == true)
        {
            fwrite(&B, sizeof(BYTE), 512, JPG);
        }
    }
    if (open == true)
    {
        fclose(JPG);
    }

    fclose(img);
}
