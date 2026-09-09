#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sw(int *a, int *b) { int t = *a; *a = *b; *b = t; }
int pt(int a[], int l, int h) {
    int p = a[h], i = l;
    for (int j = l; j < h; j++) if (a[j] <= p) sw(&a[i++], &a[j]);
    sw(&a[i], &a[h]); return i;
}
int kth(int a[], int n, int k) {
    int l = 0, h = n - 1;
    while (l <= h) {
        int p = pt(a, l, h);
        if (p == k) return a[p];
        if (p < k) l = p + 1; else h = p - 1;
    }
    return 0;
}
int main(void) {
    int n, k;
    printf("Enter N and K: ");
    if (scanf("%d%d", &n, &k) != 2 || n < 1 || k < 1 || k > n) return 1;
    int *a = malloc((size_t)n * sizeof *a);
    if (!a) return 1;
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) a[i] = rand() % 1000;
    printf("%d-th smallest = %d\n", k, kth(a, n, k - 1));
    free(a); return 0;
}
