#include <stdlib.h>
#include <math.h>

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes) {
    // Total number of subsets is 2^numsSize
    int totalSubsets = 1 << numsSize; 
    *returnSize = totalSubsets;
    
    // Allocate memory for the list of subsets
    int** result = (int**)malloc(totalSubsets * sizeof(int*));
    *returnColumnSizes = (int*)malloc(totalSubsets * sizeof(int));
    
    // Iterate from 000...0 to 111...1 (in binary)
    for (int i = 0; i < totalSubsets; i++) {
        int subsetSize = 0;
        
        // Count how many bits are set to 1 to find the subset size
        for (int j = 0; j < numsSize; j++) {
            if ((i >> j) & 1) {
                subsetSize++;
            }
        }
        
        // Allocate space for this specific subset
        result[i] = (int*)malloc(subsetSize * sizeof(int));
        (*returnColumnSizes)[i] = subsetSize;
        
        // Fill the subset based on the active bits of counter 'i'
        int index = 0;
        for (int j = 0; j < numsSize; j++) {
            if ((i >> j) & 1) {
                result[i][index++] = nums[j];
            }
        }
    }
    
    return result;
}
