/* Q6: Edit Distance (Levenshtein) with traceback
 * Input : two strings (no spaces)
 * d[i][j] = edit distance between A[0..i-1] and B[0..j-1]
 *   d[i][0]=i, d[0][j]=j
 *   d[i][j] = d[i-1][j-1]                     if A[i-1]==B[j-1]
 *           = 1+min(d[i-1][j-1] (sub), d[i-1][j] (del), d[i][j-1] (ins))  otherwise
 * Time : O(m*n)   Space: O(m*n) (kept for traceback)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL 5001
static int min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }
int main(void) {
    static char A[MAXL], B[MAXL];
    printf("String A: "); if (scanf("%5000s", A) != 1) return 1;
    printf("String B: "); if (scanf("%5000s", B) != 1) return 1;
    int m = strlen(A), n = strlen(B);
    int *d = malloc((size_t)(m + 1) * (n + 1) * sizeof *d);
#define D(i, j) d[(size_t)(i) * (n + 1) + (j)]
    for (int i = 0; i <= m; i++) D(i, 0) = i;
    for (int j = 0; j <= n; j++) D(0, j) = j;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            D(i, j) = (A[i-1] == B[j-1]) ? D(i-1, j-1)
                    : 1 + min3(D(i-1, j-1), D(i-1, j), D(i, j-1));
    printf("Minimum edit distance: %d\nTraceback (A -> B):\n", D(m, n));
    char (*ops)[64] = malloc((size_t)(m + n + 1) * sizeof *ops);
    int k = 0, i = m, j = n;
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && A[i-1] == B[j-1] && D(i, j) == D(i-1, j-1)) {
            sprintf(ops[k++], "Match   '%c'", A[i-1]); i--; j--;
        } else if (i > 0 && j > 0 && D(i, j) == D(i-1, j-1) + 1) {
            sprintf(ops[k++], "Replace '%c' with '%c' (position %d in A)", A[i-1], B[j-1], i); i--; j--;
        } else if (i > 0 && D(i, j) == D(i-1, j) + 1) {
            sprintf(ops[k++], "Delete  '%c' (position %d in A)", A[i-1], i); i--;
        } else {
            sprintf(ops[k++], "Insert  '%c' (after position %d of A)", B[j-1], i); j--;
        }
    }
    for (int t = k - 1, step = 1; t >= 0; t--, step++) printf("  %2d. %s\n", step, ops[t]);
    free(d); free(ops);
    return 0;
}
