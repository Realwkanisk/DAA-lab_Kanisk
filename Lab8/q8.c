/* Q8: Optimal Binary Search Tree (CLRS formulation)
 * Input : n, p[1..n], q[0..n]   (sum of all = 1)
 * e[i][j] = expected cost of optimal BST on keys k_i..k_j
 * w[i][j] = sum p[i..j] + q[i-1..j]
 * e[i][i-1] = q[i-1];  e[i][j] = min_{r=i..j} e[i][r-1] + e[r+1][j] + w[i][j]
 * Time : O(n^3)   Space: O(n^2)   (Knuth's root-monotonicity would give O(n^2))
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
static int N, W;
static int *root;
#define R(i, j) root[(i) * W + (j)]
static void print_tree(int i, int j, int parent, const char *side) {
    if (i > j) {
        printf("  d%d is the %s child of k%d\n", j, side, parent);
        return;
    }
    int r = R(i, j);
    if (parent == 0) printf("  k%d is the root\n", r);
    else printf("  k%d is the %s child of k%d\n", r, side, parent);
    print_tree(i, r - 1, r, "left");
    print_tree(r + 1, j, r, "right");
}
int main(void) {
    int n;
    printf("Number of keys n: "); if (scanf("%d", &n) != 1 || n <= 0) return 1;
    N = n; W = n + 2;
    double *p = calloc(n + 2, sizeof *p), *q = calloc(n + 2, sizeof *q);
    double *e = malloc((size_t)W * W * sizeof *e), *w = malloc((size_t)W * W * sizeof *w);
    root = calloc((size_t)W * W, sizeof *root);
    printf("p[1..%d]: ", n); for (int i = 1; i <= n; i++) if (scanf("%lf", &p[i]) != 1) return 1;
    printf("q[0..%d]: ", n); for (int i = 0; i <= n; i++) if (scanf("%lf", &q[i]) != 1) return 1;
#define E(i, j) e[(i) * W + (j)]
#define WT(i, j) w[(i) * W + (j)]
    for (int i = 1; i <= n + 1; i++) { E(i, i-1) = q[i-1]; WT(i, i-1) = q[i-1]; }
    for (int l = 1; l <= n; l++)
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            E(i, j) = DBL_MAX;
            WT(i, j) = WT(i, j-1) + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double t = E(i, r-1) + E(r+1, j) + WT(i, j);
                if (t < E(i, j)) { E(i, j) = t; R(i, j) = r; }
            }
        }
    printf("Minimum expected search cost: %.4f\nTree structure:\n", E(1, n));
    print_tree(1, n, 0, "");
    free(p); free(q); free(e); free(w); free(root);
    return 0;
}
