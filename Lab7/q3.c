/* ============================================================
   Q3: REVE'S PUZZLE  (n disks, 4 pegs)
   ------------------------------------------------------------
   With 3 pegs the classic Hanoi answer is T3(n) = 2^n - 1.

   With 4 pegs (Frame-Stewart algorithm):
     - move the top k disks to a spare peg using ALL 4 pegs      -> T4(k)
     - move the remaining (n-k) big disks with only 3 pegs free  -> T3(n-k)
     - move the k disks back on top using all 4 pegs again       -> T4(k)
     T4(n) = min over 1<=k<n of  2*T4(k) + T3(n-k)
     T4(0) = 0

   For n = 8 disks the optimum is k = 3 (or 4, ties happen) and
   T4(8) = 33, which matches the puzzle statement.

   Complexity:
     Computing the best k for every size : O(n^2) (DP over k)
     Generating & printing the moves     : O(T4(n)) = O(2^(n/2))-ish
                                            (that's simply the size
                                             of the answer itself)
   ============================================================ */
#include <stdio.h>
#include <limits.h>

#define MAXN 20
long dp4[MAXN+1];     /* dp4[i]  = optimal move count with 4 pegs, i disks */
int  bestK[MAXN+1];   /* bestK[i]= the k that achieves dp4[i]              */
long moveCount = 0;

/* classic 3-peg count, closed form */
long T3(int n) { return (1L << n) - 1; }

/* ---- 3-peg Hanoi mover (used for the "big disks" phase) ----
   'top' = actual label of the smallest disk being moved in this call,
   so real disk number = top + (local index - 1).                    */
void hanoi3(int n, char from, char to, char via, int top) {
    if (n == 0) return;
    hanoi3(n-1, from, via, to, top);
    printf("Move disk %d : %c -> %c\n", top+n-1, from, to);
    moveCount++;
    hanoi3(n-1, via, to, from, top);
}

/* ---- 4-peg Frame-Stewart mover ---- */
/* disk numbering: 'top' is the label of the smallest disk in THIS subcall,
   disks top..top+n-1 are being moved (top+n-1 is the largest of the group) */
void hanoi4(int n, char from, char to, char aux1, char aux2, int top) {
    if (n == 0) return;
    if (n == 1) {
        printf("Move disk %d : %c -> %c\n", top, from, to);
        moveCount++;
        return;
    }
    int k = bestK[n];                                   /* how many small disks to park on aux2 */
    hanoi4(k, from, aux2, to, aux1, top);                /* step 1: k disks -> spare, using 4 pegs   */
    hanoi3(n-k, from, to, aux1, top+k);                  /* step 2: n-k big disks, only 3 pegs usable */
    hanoi4(k, aux2, to, from, aux1, top);                /* step 3: bring the k disks back, using 4 pegs */
}

int main(void) {
    int n;
    printf("Enter number of disks n: ");
    scanf("%d", &n);
    if (n < 1 || n > MAXN) { printf("out of range\n"); return 1; }

    /* --- DP to find optimal k and optimal move count for every size 1..n --- */
    dp4[0] = 0;
    if (n >= 1) { dp4[1] = 1; bestK[1] = 1; }   /* base case: 1 disk = 1 move, any peg count */
    for (int i = 2; i <= n; i++) {
        long best = LONG_MAX;
        int  bk = 1;
        for (int k = 1; k < i; k++) {
            long cost = 2*dp4[k] + T3(i-k);
            if (cost < best) { best = cost; bk = k; }
        }
        /* k = i (skip the 3-peg phase entirely) is also legal when i is small */
        dp4[i] = best;
        bestK[i] = bk;
    }

    printf("\nOptimal move count for %d disks on 4 pegs = %ld\n", n, dp4[n]);

    /* Actually perform & print the moves (relabelling disks 1..n so the
       biggest disk is n and the smallest is 1, largest-on-bottom rule).   */
    printf("\n-- Move sequence (peg A -> D, spares B,C) --\n");
    moveCount = 0;
    hanoi4(n, 'A', 'D', 'B', 'C', 1);
    printf("\nTotal moves actually made = %ld\n", moveCount);

    if (n == 8)
        printf("(Check) classic Reve's puzzle answer for 8 disks is 33 -> %s\n",
               moveCount == 33 ? "MATCH" : "MISMATCH");
    return 0;
}
