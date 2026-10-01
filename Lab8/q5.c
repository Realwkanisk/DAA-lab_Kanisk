/* Q5: Maximum Sum Increasing Subsequence
 * Input : n, then n positive integers
 * msis[i] = a[i] + max(0, max msis[j] for j<i, a[j]<a[i]);  answer = max msis[i]
 * Time : O(n^2)   Space: O(n)
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n;
    printf("n: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    long long *a = malloc(n * sizeof *a), *s = malloc(n * sizeof *s);
    int *par = malloc(n * sizeof *par);
    printf("Array: ");
    for (int i = 0; i < n; i++) if (scanf("%lld", &a[i]) != 1) return 1;
    long long best = 0; int bi = 0;
    for (int i = 0; i < n; i++) {
        s[i] = a[i]; par[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && s[j] + a[i] > s[i]) { s[i] = s[j] + a[i]; par[i] = j; }
        if (s[i] > best) { best = s[i]; bi = i; }
    }
    printf("Maximum sum: %lld\n", best);
    int cnt = 0; for (int i = bi; i != -1; i = par[i]) cnt++;
    long long *seq = malloc(cnt * sizeof *seq), k = cnt;
    for (int i = bi; i != -1; i = par[i]) seq[--k] = a[i];
    printf("Subsequence:");
    for (int i = 0; i < cnt; i++) printf(" %lld", seq[i]);
    printf("\n");
    free(a); free(s); free(par); free(seq);
    return 0;
}
