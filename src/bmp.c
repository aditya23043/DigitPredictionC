#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

void write_bmp(char *filename, u8 image[28][28])
{
    int bmp_fp = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (bmp_fp == -1)
    {
        perror("BMP file");
        return;
    }

    // image info
    int width = 28;
    int height = 28;
    int header_size = 54;
    int color_table_size = 256 * 4; // rgb and reserved
    int file_size = header_size + color_table_size + (width * height);

    // file header
    write(bmp_fp, &(u16){0x4d42}, 2);    // 424d but little endian
    write(bmp_fp, &(u32){file_size}, 4); // file size
    write(bmp_fp, &(u32){0}, 4);         // reserved = 0
    write(bmp_fp, &(u32){54 + color_table_size},
          4); // Data offset: 14 + 40 + 1024

    // info header
    write(bmp_fp, &(u32){40}, 4);     // size of info header
    write(bmp_fp, &(u32){width}, 4);  // width
    write(bmp_fp, &(u32){height}, 4); // height
    write(bmp_fp, &(u16){1}, 2);      // num of planes
    write(bmp_fp, &(u16){8}, 2);      // bits per pixel
    write(bmp_fp, &(u32){0}, 4);      // compression
    write(bmp_fp, &(u32){width * height},
          4);                      // image size (=0 if compression=0)
    write(bmp_fp, &(u32){0}, 4);   // XPixelsPerM
    write(bmp_fp, &(u32){0}, 4);   // YPixelsPerM
    write(bmp_fp, &(u32){256}, 4); // colors used
    write(bmp_fp, &(u32){0}, 4);   // imp colors

    // color table
    for (int i = 0; i < 256; i++)
    {
        write(bmp_fp, &(u8){i}, 1);
        write(bmp_fp, &(u8){i}, 1);
        write(bmp_fp, &(u8){i}, 1);
        write(bmp_fp, &(u8){0}, 1);
    }

    // image data
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            write(bmp_fp, &image[i][j], 1);
        }
    }

    close(bmp_fp);
}
