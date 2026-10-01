/* Q2: Coin Change - number of distinct combinations
 * Input : n, n coin values, target V
 * dp[v] = number of combinations summing to v; dp[0]=1.
 * Coins are the OUTER loop so each multiset is counted once (order ignored).
 *   for each coin c: for v=c..V: dp[v] += dp[v-c]
 * Time : O(n*V)   Space: O(V)
 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int n, V;
    printf("Number of denominations: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    int *c = malloc(n * sizeof *c);
    printf("Denominations: ");
    for (int i = 0; i < n; i++) if (scanf("%d", &c[i]) != 1 || c[i] <= 0) return 1;
    printf("Target amount V: "); if (scanf("%d", &V) != 1 || V < 0) return 1;
    unsigned long long *dp = calloc(V + 1, sizeof *dp);
    dp[0] = 1;
    for (int i = 0; i < n; i++)
        for (int v = c[i]; v <= V; v++) dp[v] += dp[v - c[i]];
    printf("Number of ways: %llu\n", dp[V]);
    free(c); free(dp);
    return 0;
}
