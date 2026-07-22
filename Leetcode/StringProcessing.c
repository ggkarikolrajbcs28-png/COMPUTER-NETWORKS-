#include <stdio.h>
char s[1111];
int main() {
    int cases, cas, i, total;
    scanf("%d", &cases);
    for (cas = 0; cas < cases; cas++) {
        scanf("%s", s);
        total = 0;
        for (i = 0; s[i]; i++) {
            if ('0' <= s[i] && s[i] <= '9') total += s[i] - '0';
        }
        printf("%d\n", total);
    }
    return 0;
	// your code goes here

}

