/* TODO
 * Load MNIST dataset ✓  
 *      Parse IDX file format ✓  
 * Filter Data ×
 * MLE to find data parameters ×
 * Discriminant Analysis ×
 */

#include "mnist_load.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MNIST_train_images "./src/MNIST/train-images.idx3-ubyte"
#define MNIST_test_images "./src/MNIST/t10k-images.idx3-ubyte"
#define MNIST_train_labels "./src/MNIST/train-labels.idx1-ubyte"
#define MNIST_test_labels "./src/MNIST/t10k-labels.idx1-ubyte"

#define PRINT_ARR(arr) printf("[ "); for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) { printf("%d ", arr[i]); } printf("]\n");

typedef u8 Image1D[784];

Image1D* flatten_img(Image* imgs2d, int N)
{
    int i = 0, j = 0;

    Image1D *img1d = malloc(N * sizeof(Image1D));

    for(int i = 0; i < N; i++)
    {
        
    }

    return img1d;
}

int main(void)
{
    srand(time(NULL));

    Image* x_train = load_img(MNIST_train_images);    // 60k x 28 x 28
    Image* x_test  = load_img(MNIST_test_images);     // 10k x 28 x 28
    Label* y_train = load_label(MNIST_train_labels);  // 60k x 1
    Label* y_test = load_label(MNIST_test_labels);    // 10k x 1

    // choose 100 random indices for each digits (train set) -> indices0[100], indices1[100], indices2[100] <<<
    int indices0[100] = {0}, indices1[100] = {0}, indices2[100] = {0};
    int count0 = 0, count1 = 0, count2 = 0;
    int index = 0;
    int found = 0;
    while(count0 < 100 && count1 < 100 && count2 < 100)
    {
        index = (rand() % 60000);
        found = 0;
        
        if(count0 < 100 && y_train[index] == 0)
        {
            for(int i = 0; i < sizeof(indices0)/sizeof(indices0[0]); i++)
            {
                if(index == indices0[i])
                {
                    found = 1;
                }
            }
            if(!found)
            {
                indices0[count0] = index;
                count0++;
            }
        } 
        else if(count1 < 100 && y_train[index] == 1)
        {
            for(int i = 0; i < sizeof(indices1)/sizeof(indices1[0]); i++)
            {
                if(index == indices1[i])
                {
                    found = 1;
                }
            }
            if(!found)
            {
                indices1[count1] = index;
                count1++;
            }
        }
        else if(count2 < 100 && y_train[index] == 2)
        {
            for(int i = 0; i < sizeof(indices2)/sizeof(indices2[0]); i++)
            {
                if(index == indices2[i])
                {
                    found = 1;
                }
            }
            if(!found)
            {
                indices2[count2] = index;
                count2++;
            }
        }
    } // >>>

    // make the data arrays out of the indices chosen -> x_train_0[100,28,28], x_train_1[100,28,28], x_train_2[100,28,28] <<<
    Image* x_train_0_2d = malloc(100 * sizeof(Image));
    Image* x_train_1_2d = malloc(100 * sizeof(Image));
    Image* x_train_2_2d = malloc(100 * sizeof(Image));
    for(int i = 0; i < 100; i++)
    {
        memcpy(x_train_0_2d[i], x_train[indices0[i]], sizeof(Image));
        memcpy(x_train_1_2d[i], x_train[indices1[i]], sizeof(Image));
        memcpy(x_train_2_2d[i], x_train[indices2[i]], sizeof(Image));
    } // >>>
    

    /* Image1D* x_train_1d = flatten_img(x_train, 100); */

    free(x_train_0_2d);
    free(x_train_1_2d);
    free(x_train_2_2d);

    free(x_train);
    free(y_train);
    free(x_test);
    free(y_test);

    return 0;
}
