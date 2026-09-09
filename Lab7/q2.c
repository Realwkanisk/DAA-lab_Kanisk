/* ============================================================
   Q2: SUPER EGG TESTING (E eggs, F floors)
   ------------------------------------------------------------
   dp[e][f] = minimum number of droppings needed, in the WORST
              case, to find the critical floor when we have
              e eggs and f floors still to test.

   Recurrence: drop an egg from some floor k (1<=k<=f)
     - egg breaks   -> problem reduces to (e-1) eggs, (k-1) floors
     - egg survives -> problem reduces to  e    eggs, (f-k) floors
   We must plan for the WORSE of the two, and we pick k that
   MINIMISES that worst case:

     dp[e][f] = 1 + min_{1<=k<=f} max( dp[e-1][k-1], dp[e][f-k] )
     dp[1][f] = f            (only one egg -> must go floor by floor)
     dp[e][0] = 0             (no floors -> 0 droppings needed)

   Complexity:
     Naive   : O(E * F^2) time, O(E*F) space   <-- used below
     (Can be improved to O(E*F) using the fact that dp[e][f]-dp[e][f-1]
      is monotone in f, but O(E*F^2) is fine for F<=100, E small.)
   ============================================================ */
#include <stdio.h>

#define MAXE 20
#define MAXF 100

int dp[MAXE+1][MAXF+1];

int main(void) {
    int e, f;
    printf("Enter number of eggs (E): ");
    scanf("%d", &e);
    printf("Enter number of floors (F): ");
    scanf("%d", &f);

    if (e < 1 || e > MAXE || f < 0 || f > MAXF) {
        printf("Out of supported range.\n");
        return 1;
    }

    /* base cases */
    for (int i = 0; i <= e; i++) dp[i][0] = 0;      /* 0 floors -> 0 tries */
    for (int j = 0; j <= f; j++) dp[1][j] = j;       /* 1 egg -> linear scan */

    /* fill table for e=2..E eggs */
    for (int i = 2; i <= e; i++) {
        for (int j = 1; j <= f; j++) {
            int best = 1 << 30;
            for (int k = 1; k <= j; k++) {
                int broke  = dp[i-1][k-1];   /* egg breaks at floor k   */
                int lived  = dp[i][j-k];     /* egg survives floor k    */
                int worst  = (broke > lived) ? broke : lived;
                if (worst + 1 < best) best = worst + 1;
            }
            dp[i][j] = best;
        }
    }

    printf("\nWith %d egg(s) and %d floor(s):\n", e, f);
    printf("Minimum droppings needed in the worst case = %d\n", dp[e][f]);

    /* sanity check for the classic case in the problem: 2 eggs, 100 floors */
    if (e >= 2 && f >= 100)
        printf("(Check) dp[2][100] = %d  (classic answer is 14)\n", dp[2][100]);

    return 0;
}
