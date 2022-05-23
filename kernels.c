/********************************************************
 * Kernels to be optimized
 ********************************************************/

#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Diana and Nika",              /* Team name */

    "Diana Morgan",     /* First member full name */
    "diana.morganr@campus.technion.ac.il",  /* First member email address */

    "Veronika Sorochenkova",                   /* Second member full name (leave blank if none) */
    "veronika@campus.technion.ac.il"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    int temp_dim = dim - 1;
    int count_ind;

    for (j = 0; j < dim; j++) {
        count_ind = temp_dim - j;
        for (i = 0; i < dim; i++)
            dst[RIDX(count_ind, i, dim)] = src[RIDX(i, j, dim)];
    }
}

char three_rotate_descr[] = "three_rotate: Current working version";
void three_rotate(int dim, pixel *src, pixel *dst) {
    int i, j;
    int temp_dim = dim - 1;
    int calc = 0;
    int loop_lim = dim -2;
    for (j = 0; j < dim; j++) {
        calc = temp_dim - j;
        for (i = 0; i < loop_lim; i+=3) {
            dst[RIDX(calc, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(calc, i + 1, dim)] = src[RIDX(i + 1 , j, dim)];
            dst[RIDX(calc, i + 2 , dim)] = src[RIDX(i + 2, j, dim)];
        }
        for( ; i < dim; i++)
            dst[RIDX(temp_dim - j, i, dim)] = src[RIDX(i, j, dim)];
    }


}

char four_rotate_descr[] = "four_rotate: Current working version";
void four_rotate(int dim, pixel *src, pixel *dst){
    int i, j;
    int temp_dim = dim - 1;
    int calc = 0;
    int loop_lim = dim - 3;

    for (j = 0; j < dim; j++) {
        calc = temp_dim - j;
        for (i = 0; i < loop_lim; i+=4) {
            dst[RIDX(calc, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(calc, i + 1, dim)] = src[RIDX(i + 1 , j, dim)];
            dst[RIDX(calc, i + 2 , dim)] = src[RIDX(i + 2, j, dim)];
            dst[RIDX(calc, i + 3 , dim)] = src[RIDX(i + 3, j, dim)];

        }
        for( ; i < dim; i++)
            dst[RIDX(temp_dim - j, i, dim)] = src[RIDX(i, j, dim)];
    }
}
char oneloop_rotate_descr[] = "oneloop_rotate: Current working version";
void oneloop_rotate(int dim, pixel *src, pixel *dst){
    int k;
    double dev = 1/(double)dim;
    int limit = dim * dim;
    int calc = dim - 1;
    for(k=0; k < limit; k++){
        int div = k * dev;
        int mod = k%dim;
        dst[RIDX(calc-div, mod, dim)] = src[RIDX(mod, div, dim)];

    }

}

char eight_rotate_descr[] = "eight_rotate: Current working version";
void eight_rotate(int dim, pixel *src, pixel *dst){
    int i, j;
    int temp_dim = dim - 1;
    int calc = 0;
    int loop_lim = dim - 7;

    for (i = 0; i < dim; i++) {
        for (j = 0; j < loop_lim; j+=8) {
            calc = temp_dim - j;
            dst[RIDX(calc, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(calc - 1, i, dim)] = src[RIDX(i, j + 1, dim)];
            dst[RIDX(calc - 2, i , dim)] = src[RIDX(i, j + 2, dim)];
            dst[RIDX(calc - 3, i, dim)] = src[RIDX(i, j + 3, dim)];
            dst[RIDX(calc - 4, i, dim)] = src[RIDX(i, j + 4, dim)];
            dst[RIDX(calc - 5, i, dim)] = src[RIDX(i, j + 5, dim)];
            dst[RIDX(calc - 6, i, dim)] = src[RIDX(i, j + 6, dim)];
            dst[RIDX(calc - 7, i, dim)] = src[RIDX(i, j + 7, dim)];

        }
        for( ; j < dim; j++)
            dst[RIDX(temp_dim - j, i, dim)] = src[RIDX(i, j, dim)];
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&three_rotate, three_rotate_descr);
    add_rotate_function(&four_rotate, four_rotate_descr);
    add_rotate_function(&eight_rotate, eight_rotate_descr);
    add_rotate_function(&oneloop_rotate, oneloop_rotate_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}


