/* Q3: Longest Common Subsequence with reconstruction
 * Input : two strings (no spaces), one per line
 * L[i][j] = LCS length of X[0..i-1], Y[0..j-1]
 *   = L[i-1][j-1]+1 if X[i-1]==Y[j-1], else max(L[i-1][j], L[i][j-1])
 * Time : O(m*n)   Space: O(m*n) (needed for reconstruction)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL 10001
int main(void) {
    static char X[MAXL], Y[MAXL];
    printf("String X: "); if (scanf("%10000s", X) != 1) return 1;
    printf("String Y: "); if (scanf("%10000s", Y) != 1) return 1;
    int m = strlen(X), n = strlen(Y);
    int *L = calloc((size_t)(m + 1) * (n + 1), sizeof *L);
#define T(i, j) L[(size_t)(i) * (n + 1) + (j)]
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            T(i, j) = (X[i-1] == Y[j-1]) ? T(i-1, j-1) + 1
                    : (T(i-1, j) >= T(i, j-1) ? T(i-1, j) : T(i, j-1));
    int len = T(m, n);
    char *s = malloc(len + 1); s[len] = '\0';
    int i = m, j = n, k = len;
    while (i > 0 && j > 0) {
        if (X[i-1] == Y[j-1]) { s[--k] = X[i-1]; i--; j--; }
        else if (T(i-1, j) >= T(i, j-1)) i--;
        else j--;
    }
    printf("LCS length: %d\nLCS: %s\n", len, s);
    free(L); free(s);
    return 0;
}
