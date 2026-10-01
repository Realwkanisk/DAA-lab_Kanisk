/* Q1: Minimum Coin Change
 * Input : n, then n coin values, then target V
 * State : dp[v] = min coins to make v;  dp[0]=0
 * Recurrence: dp[v] = min over coins c<=v of dp[v-c]+1  (INF if unreachable)
 * Time  : O(n*V)      Space: O(V)  (+O(V) for coin reconstruction)
 */
#include <stdio.h>
#include <stdlib.h>
#define INF 1000000000
int main(void) {
    int n, V;
    printf("Number of denominations: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    int *c = malloc(n * sizeof *c);
    printf("Denominations: ");
    for (int i = 0; i < n; i++) if (scanf("%d", &c[i]) != 1 || c[i] <= 0) return 1;
    printf("Target amount V: "); if (scanf("%d", &V) != 1 || V < 0) return 1;
    int *dp = malloc((V + 1) * sizeof *dp), *last = malloc((V + 1) * sizeof *last);
    dp[0] = 0;
    for (int v = 1; v <= V; v++) {
        dp[v] = INF; last[v] = -1;
        for (int i = 0; i < n; i++)
            if (c[i] <= v && dp[v - c[i]] != INF && dp[v - c[i]] + 1 < dp[v]) {
                dp[v] = dp[v - c[i]] + 1; last[v] = c[i];
            }
    }
    if (dp[V] == INF) { printf("Result: -1\n"); }
    else {
        printf("Result: %d\nCoins used:", dp[V]);
        for (int v = V; v > 0; v -= last[v]) printf(" %d", last[v]);
        printf("\n");
    }
    free(c); free(dp); free(last);
    return 0;
}
