int hammingDistance(int x, int y) {
    for (x ^= y, y = 0 ; x ; x &= x - 1, y++);
    return y;
}
