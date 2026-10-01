/* Q4: Longest (strictly) Increasing Subsequence
 * Input : n, then n integers
 * Method 1 (DP, with reconstruction): dp[i] = 1 + max dp[j] (j<i, a[j]<a[i])
 *          Time O(n^2)  Space O(n)
 * Method 2 (patience sorting / binary search) used as a cross-check:
 *          Time O(n log n)  Space O(n)
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n;
    printf("n: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    int *a = malloc(n * sizeof *a), *dp = malloc(n * sizeof *dp), *par = malloc(n * sizeof *par);
    printf("Array: ");
    for (int i = 0; i < n; i++) if (scanf("%d", &a[i]) != 1) return 1;
    int best = 0, bi = 0;
    for (int i = 0; i < n; i++) {
        dp[i] = 1; par[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) { dp[i] = dp[j] + 1; par[i] = j; }
        if (dp[i] > best) { best = dp[i]; bi = i; }
    }
    /* O(n log n) check */
    int *tail = malloc(n * sizeof *tail), len = 0;
    for (int i = 0; i < n; i++) {
        int lo = 0, hi = len;
        while (lo < hi) { int mid = (lo + hi) / 2; if (tail[mid] < a[i]) lo = mid + 1; else hi = mid; }
        tail[lo] = a[i]; if (lo == len) len++;
    }
    printf("LIS length (O(n^2) DP)    : %d\n", best);
    printf("LIS length (O(n log n))   : %d\n", len);
    int *seq = malloc(best * sizeof *seq), k = best;
    for (int i = bi; i != -1; i = par[i]) seq[--k] = a[i];
    printf("One LIS:");
    for (int i = 0; i < best; i++) printf(" %d", seq[i]);
    printf("\n");
    free(a); free(dp); free(par); free(tail); free(seq);
    return 0;
}
