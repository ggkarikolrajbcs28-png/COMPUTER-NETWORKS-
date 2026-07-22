#define MAX_NODES 5000000
int trie[MAX_NODES][2];
int node_count;
void initTrie() {
    node_count = 0;
    trie[0][0] = -1;
    trie[0][1] = -1;
}
void insert(int num) {
    int curr = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (trie[curr][bit] == -1) {
            node_count++;
            trie[node_count][0] = -1;
            trie[node_count][1] = -1;
            trie[curr][bit] = node_count;
        }
        curr = trie[curr][bit];
    }
}

int getMaxXOR(int num) {
    int curr = 0;
    int max_xor_for_num = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (num >> i) & 1;
        int toggled_bit = 1 - bit;
        
        if (trie[curr][toggled_bit] != -1) {
            max_xor_for_num |= (1 << i);
            curr = trie[curr][toggled_bit];
        } else {
            curr = trie[curr][bit];
        }
    }
    return max_xor_for_num;
}
int findMaximumXOR(int* nums, int numsSize) {
    if (numsSize == 0) return 0;
    initTrie();
    int global_max_xor = 0;
    insert(nums[0]);
    for (int i = 1; i < numsSize; i++) {
        int current_max = getMaxXOR(nums[i]);
        if (current_max > global_max_xor) {
            global_max_xor = current_max;
        }
        insert(nums[i]);
    }
    return global_max_xor;
}
