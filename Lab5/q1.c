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
    int n;
    printf("Enter N: ");
    if (scanf("%d", &n) != 1 || n < 1) return 1;
    int *a = malloc((size_t)n * sizeof *a);
    if (!a) return 1;
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) a[i] = rand() % 1000;
    int x = kth(a, n, (n - 1) / 2);
    if (n % 2) printf("Median = %d\n", x);
    else printf("Median = %.2f\n", (x + kth(a, n, n / 2)) / 2.0);
    free(a); return 0;
}
