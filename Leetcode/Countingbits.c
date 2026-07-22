
 int countOnes(int num)
 {
    int count=0;

    while(num)
    {
        num &=(num-1);
        count++;

    }
    return count;
 }

int* countBits(int n, int* returnSize) {

    *returnSize =n+1;

    int* ans =(int*)malloc(sizeof(int)*(n+1) );

    for (int i =0;i<=n;i++)
    {
        ans[i]=countOnes(i);
    }

    return ans;
    
}
