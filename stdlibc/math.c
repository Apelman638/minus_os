int pow(int x, int y) { // x^y, add to its own file
    int ret = x;
    for(int i = 1; i < y; i++) {
        ret *= x;
    }
    return ret;
}