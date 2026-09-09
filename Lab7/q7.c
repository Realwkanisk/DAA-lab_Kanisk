/* ============================================================
   Q7: MATRIX CHAIN MULTIPLICATION (MCM)
   ------------------------------------------------------------
   Given dimensions p[0..n] for n matrices (matrix i has size
   p[i-1] x p[i]), find the parenthesisation that minimises the
   total number of scalar multiplications needed to compute the
   full product.

   dp[i][j] = min scalar multiplications to multiply matrices i..j
     dp[i][i] = 0
     dp[i][j] = min_{i<=k<j} dp[i][k] + dp[k+1][j] + p[i-1]*p[k]*p[j]
   split[i][j] stores the best k, used to reconstruct the
   optimal parenthesisation.

   Complexity: O(n^3) time, O(n^2) space.
   ============================================================ */
#include <stdio.h>
#include <limits.h>

#define MAXM 30

long long dp[MAXM][MAXM];
int sp[MAXM][MAXM];     /* best split point for [i][j] */

void printOrder(int i, int j) {
    if (i == j) { printf("M%d", i); return; }
    printf("(");
    printOrder(i, sp[i][j]);
    printOrder(sp[i][j]+1, j);
    printf(")");
}

int main(void) {
    int n;                       /* number of matrices */
    printf("Enter number of matrices: ");
    if (scanf("%d", &n) != 1 || n < 1 || n >= MAXM) {
        fprintf(stderr, "The number of matrices must be between 1 and %d.\n", MAXM - 1);
        return 1;
    }

    int p[MAXM+1];                /* p[0..n] dimension chain */
    printf("Enter %d dimensions (p0 p1 ... p%d) so matrix i is p(i-1) x p(i):\n", n+1, n);
    for (int i = 0; i <= n; i++) {
        if (scanf("%d", &p[i]) != 1 || p[i] <= 0) {
            fprintf(stderr, "Matrix dimensions must be positive integers.\n");
            return 1;
        }
    }

    for (int i = 1; i <= n; i++) dp[i][i] = 0;

    /* len = chain length being solved, from 2 matrices up to n */
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long cost = dp[i][k] + dp[k+1][j]
                               + (long long)p[i-1] * p[k] * p[j];
                if (cost < dp[i][j]) { dp[i][j] = cost; sp[i][j] = k; }
            }
        }
    }

    printf("\nMinimum scalar multiplications = %lld\n", dp[1][n]);
    printf("Optimal parenthesisation        = ");
    printOrder(1, n);
    printf("\n");
    return 0;
}
