int findComplement(int num) {
    int result = 0;
    unsigned int pvalue = 1;
    
    while (num > 0) {
        if ((num % 2) == 0) {
            result += pvalue;
        }
        pvalue *= 2;
        num /= 2;         
    }
    return result;
}
