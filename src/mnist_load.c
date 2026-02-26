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
typedef u8 Image[28][28];
typedef u8 Label;

#define PRINT_BYTE(c) printf("0x%02x\n", c);

// Check if the number of bytes read is less than a certain amount
#define CHECK(n, num)                                                          \
    if (n < num)                                                               \
    perror("Corrupted data")

// big endian to little endian
u32 be_to_le(u32 num)
{
    return (((num >> 24) & 0x000000ff) | ((num >> 8) & 0x0000ff00) |
            ((num << 8) & 0x00ff0000) | ((num << 24) & 0xff000000));
}

/* u8 (*load_img())[28][28] */

// Loads all image bytes into a malloc'd Image array.
// Needs to be free'd in the main program
Image* load_img(char* dataset_filename)
{
    int img_fp = open(dataset_filename, O_RDONLY);
    if (img_fp == -1)
    {
        perror("Image File open");
        return NULL;
    }

    u32 c;
    int n;
    int num_samples = 0;

    // check header bytes
    n = read(img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x00000803) // unsigned bits, 3 dimensional
    {
		printf("Wrong magic number OR number of dimensions!\n");
        return NULL;
    }
    n = read(img_fp, &c, 4);
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
        printf("Number of samples < 60000 / 10000!\n");
        return NULL;
    }
    n = read(img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x0000001c) // 28
    {
        printf("Image dimension < 28! (1)\n");
        return NULL;
    }
    n = read(img_fp, &c, 4);
    CHECK(n, 4);
    if (be_to_le(c) != 0x0000001c) // 28
    {
        printf("Image dimension < 28! (2)\n");
        return NULL;
    }

    /* printf("# samples = %d\n", num_samples); */

    /* u8(*image)[28][28] = malloc(num_samples * 28 * 28); */
    Image* images = malloc(num_samples * 28 * 28);
    n = read(img_fp, images, num_samples * 28 * 28);

    /* write_bmp("img.bmp", image[59999]); */

    /* free(image); */

    close(img_fp);

    return images;
}

// Loads all label bytes into a malloc'd Label array.
// Needs to be free'd in the main program
Label* load_label(char* dataset_filename)
{
	int label_fp = open(dataset_filename, O_RDONLY);	
	if(label_fp == -1)
	{
		perror("Label File Open");
		return NULL;
	}

	u32 c;
	int n;
	int num_samples = 0;

	n = read(label_fp, &c, 4);
	CHECK(n, 4);
	if(be_to_le(c) != 0x00000801) // unsigned bits, 1 dimension
	{
		printf("Wrong magic number OR number of dimensions!\n");
		return NULL;
	}
	n = read(label_fp, &c, 4);
	CHECK(n, 4);
	if(be_to_le(c) == 0x0000ea60) // 60000
	{
		num_samples = 60000;
	} 
	else if(be_to_le(c) == 0x00002710) // 10000
	{
		num_samples = 10000;
	}
	else
	{
        printf("Number of samples < 60000 / 10000!\n");
        return NULL;
	}

	Label* labels = malloc(num_samples);
	n = read(label_fp, labels, num_samples);

	close(label_fp);

	return labels;
}
