/* ============================================================
   Q5: HITTING A MOVING TARGET
   ------------------------------------------------------------
   n spots on a line, numbered 1..n. The target sits at some
   unknown spot, and after every shot (that misses) it MUST move
   to an adjacent spot (left or right, never stays put). We fire
   one shot per turn and must guarantee a hit eventually.

   Key observation (parity argument):
   The target's spot-index parity (odd/even) FLIPS every single
   turn, because it always moves to an adjacent spot. So at time
   t (t=1,2,3,...) its parity is fixed by its parity at t=1 --
   we just don't know which of the 2 possibilities is true.

   ALGORITHM (guaranteed to hit for any n>=2):
     Sweep 1:  shoot spots 2,3,4,...,n-1   (n-2 shots)
     Sweep 2:  shoot spots n-1,n-2,...,2   (n-2 shots)
   Total shots = 2*(n-2)  [use just spot 1,2 back and forth for n=2]

   WHY it works: sweep 1 assumes the target started on an "even"
   spot -- if it really did, sweep 1's positions and the target's
   positions share the same parity at every matching time step,
   and (by a discrete intermediate-value argument, since both the
   shooter's target spot and the actual target move by exactly 1
   place per turn and start on the same side) they are forced to
   collide at some point during the sweep.
   If sweep 1 fails, the target must have started "odd"; by then
   its parity (after n-2 extra turns) lines up so that sweep 2,
   going the other direction, is guaranteed to catch it.

   Complexity:
     Algorithm itself : O(n) shots.
     This program also brute-force VALIDATES the algorithm for
     small n by trying every initial position and every possible
     sequence of target moves (exponential, only for demonstration):
        O(n * 2^(2(n-2)))
   ============================================================ */
#include <stdio.h>

#define MAXN 10   /* keep small: the validator below is exponential */

int seq[4*MAXN];     /* the fixed shot sequence */
int seqLen;

/* build the shot sequence: 2..n-1 then n-1..2 (n>=3);
   for n==2 we just alternate 1,2 (forced by adjacency on 2 spots) */
void buildSequence(int n) {
    seqLen = 0;
    /* n=2 is a degenerate edge case: with only 2 spots the target is
       FORCED to alternate 1,2,1,2,... deterministically (only one
       neighbour exists). Shooting spot 1 twice in a row is enough:
       it either catches a target that started at 1 immediately, or
       (if it started at 2) the forced bounce lands it on 1 for shot 2. */
    if (n == 2) { seq[seqLen++] = 1; seq[seqLen++] = 1; return; }
    for (int i = 2; i <= n-1; i++) seq[seqLen++] = i;
    for (int i = n-1; i >= 2; i--) seq[seqLen++] = i;
}

/* recursively try EVERY movement pattern for a target that starts
   at 'pos' and see whether our fixed shot sequence ever catches it.
   dir=+1/-1 tried at every step (adversarial target).                */
int survives(int pos, int n, int step) {
    if (step == seqLen) return 1;              /* target survived all shots */
    if (seq[step] == pos) return 0;             /* caught this turn         */
    int caught_all_branches = 1;
    /* target must move to an adjacent spot (reflect at the walls) */
    int options[2], no = 0;
    if (pos-1 >= 1) options[no++] = pos-1;
    if (pos+1 <= n) options[no++] = pos+1;
    for (int i = 0; i < no; i++)
        if (survives(options[i], n, step+1)) caught_all_branches = 0;
    return caught_all_branches ? 0 : 1;   /* 1 = some path survives (bad) */
}

int main(void) {
    int n;
    printf("Enter number of hiding spots n (2..%d): ", MAXN);
    scanf("%d", &n);
    if (n < 2 || n > MAXN) { printf("out of range\n"); return 1; }

    buildSequence(n);
    printf("\nShot sequence (%d shots): ", seqLen);
    for (int i = 0; i < seqLen; i++) printf("%d ", seq[i]);
    printf("\n");

    /* validate: try EVERY starting spot; if for even one starting spot
       the target has SOME adversarial escape route, the algorithm fails. */
    int allCaught = 1;
    for (int start = 1; start <= n; start++) {
        if (survives(start, n, 0)) {
            printf("FAILS for starting position %d (an escape route exists)\n", start);
            allCaught = 0;
        }
    }
    if (allCaught)
        printf("VALIDATED: every starting position is caught, "
               "regardless of how the target moves.\n");

    return 0;
}
