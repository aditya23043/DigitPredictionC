/* NOTES
 *
 *   adi@archlinux ~/repo/DigitPredictionC (main)
 *   ❯ wc src/MNIST/train-images.idx3-ubyte
 *   35282   202155 47040016 src/MNIST/train-images.idx3-ubyte
 *
 *   first 16 bytes are for the magic + header bytes
 *   next we have 784 pixels for each image
 *   and train dataset has 60k images
 *   note that each pixel is one byte long (0x00 to 0xFF)
 *   that makes it -> 784 * 60,000 + 16 = 47040016
 *
 *   The IDX Binary File Format is stored in Big-Endian
 *   But x86 systems our little endian
 *   Need to swap the bytes when reading
 */

#include "bmp.c"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define MNIST_train_image "./src/MNIST/train-images.idx3-ubyte"
#define PRINT_BYTE(c) printf("0x%02x\n", c);
#define CHECK(n, num)                                                          \
    if (n < num)                                                               \
    perror("Corrupted data")

// big endian to little endian
u32 be_to_le(u32 num)
{
    return (((num >> 24) & 0x000000ff) | ((num >> 8) & 0x0000ff00) |
            ((num << 8) & 0x00ff0000) | ((num << 24) & 0xff000000));
}

void load_img()
{
    int train_img_fp = open(MNIST_train_image, O_RDONLY);
    if (train_img_fp == -1)
    {
        perror("File open");
        return;
    }

    u32 c;
    int n;
    int num_samples = 0;

    // check header bytes
    n = read(train_img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x00000803) // unsigned bits, 3 dimensional
    {
        printf("Wrong magic number!\n");
        return;
    }
    n = read(train_img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) == 0x0000ea60) // 60000 (train data)
    {
        num_samples = 60000;
    }
    else if (be_to_le(c) == 0x00002710) // 10000 (test data)
    {
        num_samples = 10000;
    }
    else
    {
        printf("Number of samples < 60000!\n");
        return;
    }
    n = read(train_img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x0000001c) // 28
    {
        printf("Image dimension < 28! (1)\n");
        return;
    }
    n = read(train_img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x0000001c) // 28
    {
        printf("Image dimension < 28! (2)\n");
        return;
    }

    u8(*image)[28][28] = malloc(num_samples * 28 * 28);
    for (int i = 0; i < num_samples; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            for (int k = 0; k < 28; k++)
            {
                n = read(train_img_fp, image, 60000 * 28 * 28);
                /* printf("0x%02x, ", image[i][j][k]); */
            }
        }
    }

    write_bmp("img.bmp", image[59999]);

    free(image);
    close(train_img_fp);
}
