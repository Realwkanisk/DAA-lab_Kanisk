#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sw(int *a, int *b) { int t = *a; *a = *b; *b = t; }
void qs(int a[], int l, int h) {
    if (l >= h) return;
    int p = a[h], i = l;
    for (int j = l; j < h; j++) if (a[j] <= p) sw(&a[i++], &a[j]);
    sw(&a[i], &a[h]); qs(a, l, i - 1); qs(a, i + 1, h);
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
    fclose(f); qs(a, 0, c - 1); f = fopen("sorted_output.txt", "w");
    if (!f) { free(a); return 1; }
    for (int i = 0; i < c; i++) fprintf(f, "%d\n", a[i]);
    fclose(f); printf("Sorted %d values into sorted_output.txt\n", c);
    free(a); return 0;
}
