#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sw(int *a, int *b) { int t = *a; *a = *b; *b = t; }
void hp(int a[], int n, int i) {
    int m = i, l = 2 * i + 1, r = l + 1;
    if (l < n && a[l] > a[m]) m = l;
    if (r < n && a[r] > a[m]) m = r;
    if (m != i) { sw(&a[i], &a[m]); hp(a, n, m); }
}
void hs(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) hp(a, n, i);
    for (int i = n - 1; i > 0; i--) { sw(&a[0], &a[i]); hp(a, i, 0); }
}
int main(void) {
    int n, x, c = 0;
    printf("Enter N: ");
    if (scanf("%d", &n) != 1 || n < 1) return 1;
    FILE *f = fopen("input.txt", "w");
    if (!f) return 1;
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", rand() % 100000);
    fclose(f); f = fopen("input.txt", "r");
    if (!f) return 1;
    int cap = 16, *a = malloc((size_t)cap * sizeof *a);
    if (!a) { fclose(f); return 1; }
    while (fscanf(f, "%d", &x) == 1) {
        if (c == cap) { cap *= 2; a = realloc(a, (size_t)cap * sizeof *a); }
        a[c++] = x;
    }
    fclose(f); hs(a, c); f = fopen("sorted_output.txt", "w");
    if (!f) { free(a); return 1; }
    for (int i = 0; i < c; i++) fprintf(f, "%d\n", a[i]);
    fclose(f); printf("Sorted %d values into sorted_output.txt\n", c);
    free(a); return 0;
}
