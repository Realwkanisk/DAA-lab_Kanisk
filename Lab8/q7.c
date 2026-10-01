/* Q7: Rod Cutting with reconstruction
 * Input : n, then prices p[1..n]
 * r[j] = max revenue for rod of length j = max over i=1..j of p[i] + r[j-i]
 * first[j] stores the best first-piece length -> reconstruction
 * Time : O(n^2)   Space: O(n)
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n;
    printf("Rod length n: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    long long *p = malloc((n + 1) * sizeof *p), *r = malloc((n + 1) * sizeof *r);
    int *first = malloc((n + 1) * sizeof *first);
    printf("Prices p[1..%d]: ", n);
    for (int i = 1; i <= n; i++) if (scanf("%lld", &p[i]) != 1) return 1;
    r[0] = 0;
    for (int j = 1; j <= n; j++) {
        r[j] = -1;
        for (int i = 1; i <= j; i++)
            if (p[i] + r[j - i] > r[j]) { r[j] = p[i] + r[j - i]; first[j] = i; }
    }
    printf("Maximum revenue: %lld\nPieces:", r[n]);
    for (int j = n; j > 0; j -= first[j]) printf(" %d", first[j]);
    printf("\n");
    free(p); free(r); free(first);
    return 0;
}
