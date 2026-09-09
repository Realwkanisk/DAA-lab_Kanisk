/* ============================================================
   Q1: INVERT THE COIN-TRIANGLE
   ------------------------------------------------------------
   Idea:
     A triangle with n rows of tightly packed coins has
         N = n*(n+1)/2   coins.
     To flip it (point up -> point down) we only need to slide
     the coins that DON'T already lie inside the overlap of the
     start-triangle and the end-triangle (imagine sliding the
     end-triangle on top of the start-triangle so the two shapes
     overlap as much as possible - the overlapping coins never
     have to move).

     It is a known result that the largest possible overlap
     leaves exactly floor(N/3) coins outside it, so:

         min_moves(n) = floor( (n*(n+1)/2) / 3 )

   This program:
     1) computes that formula, and
     2) VALIDATES it by brute force: it lays the n-row triangle
        on an integer triangular lattice, tries every 180-degree
        rotated + translated copy of that triangle, and finds the
        translation that maximises the overlap. moves = N - maxOverlap
        should always match the formula above.

   Complexity:
     Formula            : O(1)
     Brute-force check  : O(n^4)  (n^2 candidate shifts, each
                           checked against n^2 lattice points)
     (the check is only meant to validate small n, the real
      answer used in practice is the O(1) formula)
   ============================================================ */
#include <stdio.h>
#include <string.h>

#define MAXN   14                 /* max rows we brute-force check   */
#define RNG    (3*MAXN)           /* half-width of the lattice grid  */
#define SZ     (2*RNG+1)          /* full grid width                 */

static int grid[SZ][SZ];          /* grid[x+RNG][y+RNG]=1 if a coin of
                                      the ORIGINAL triangle sits there */

/* mark the n-row upward triangle on the lattice.
   row r (0..n-1, top->bottom) has r+1 coins; a coin at column c
   (0..r) is placed at doubled x-coordinate (2c-r, r) so that all
   coordinates stay integers on the triangular lattice.            */
static void buildTriangle(int n, int px[], int py[], int *cnt) {
    int k = 0;
    for (int r = 0; r < n; r++)
        for (int c = 0; c <= r; c++) {
            px[k] = 2*c - r;
            py[k] = r;
            k++;
        }
    *cnt = k;
}

int main(void) {
    int n;
    printf("Enter number of rows n of the coin-triangle: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) {
        printf("n must be between 1 and %d\n", MAXN);
        return 1;
    }

    int px[MAXN*(MAXN+1)/2], py[MAXN*(MAXN+1)/2], cnt;
    buildTriangle(n, px, py, &cnt);

    /* mark original triangle coins on the grid for O(1) lookup */
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < cnt; i++)
        grid[px[i]+RNG][py[i]+RNG] = 1;

    /* try every translation (tx,ty) of the 180-rotated triangle
       target point = (tx - px[i], ty - py[i])                     */
    int best = 0;
    for (int tx = -RNG; tx <= RNG; tx++) {
        for (int ty = 0; ty <= 2*(n-1); ty++) {   /* ty range covers valid rows */
            int ov = 0;
            for (int i = 0; i < cnt; i++) {
                int qx = tx - px[i], qy = ty - py[i];
                if (qx+RNG >= 0 && qx+RNG < SZ && qy+RNG >= 0 && qy+RNG < SZ
                    && grid[qx+RNG][qy+RNG])
                    ov++;
            }
            if (ov > best) best = ov;
        }
    }

    int N = n*(n+1)/2;
    int bruteMoves  = N - best;
    int formulaMoves = N / 3;         /* integer division = floor */

    printf("\nRows (n)            : %d\n", n);
    printf("Total coins (N)      : %d\n", N);
    printf("Max overlap found    : %d\n", best);
    printf("Moves (brute force)  : %d\n", bruteMoves);
    printf("Moves (formula N/3)  : %d\n", formulaMoves);
    printf("%s\n", bruteMoves == formulaMoves ? "MATCH - formula validated"
                                               : "MISMATCH - check ranges");
    return 0;
}
