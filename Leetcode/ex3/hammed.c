int totalHammingDistance(int* nums, int numsSize) {
    int total_distance = 0;
    
    // Iterate through each of the 32 bit positions
    for (int i = 0; i < 32; i++) {
        int bit_count = 0;
        
        // Count how many numbers have the i-th bit set to 1
        for (int j = 0; j < numsSize; j++) {
            bit_count += (nums[j] >> i) & 1;
        }
        
        // Add the number of pairs that differ at the i-th bit
        // (Count of 1s) * (Count of 0s)
        total_distance += bit_count * (numsSize - bit_count);
    }
    
    return total_distance;
}
