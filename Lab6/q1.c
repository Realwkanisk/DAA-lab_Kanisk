/* DAA Lab-06, Q1: 1D array operations and worst-case complexities
   n = number of elements in array a[]                              */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>

int cmp(const void *x, const void *y) {
    int left = *(const int *)x, right = *(const int *)y;
    return (left > right) - (left < right);
}

/* (i) max — O(n) */
int getMax(int a[], int n) {
    int m = a[0];
    for (int i = 1; i < n; i++) if (a[i] > m) m = a[i];
    return m;
}

/* (ii) first & second largest — O(n) */
void firstSecond(int a[], int n, int *f, int *s) {
    *f = *s = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (a[i] > *f) { *s = *f; *f = a[i]; }
        else if (a[i] > *s && a[i] != *f) *s = a[i];
    }
}

/* (iii) mean — O(n) */
double getMean(int a[], int n) {
    long sum = 0;
    for (int i = 0; i < n; i++) sum += a[i];
    return (double)sum / n;
}

/* (iv) median — needs sorting, O(n log n) */
double getMedian(int a[], int n) {
    int *b = malloc(n * sizeof(int));
    memcpy(b, a, n * sizeof(int));
    qsort(b, n, sizeof(int), cmp);
    double res = (n % 2) ? b[n / 2] : (b[n / 2 - 1] + b[n / 2]) / 2.0;
    free(b);
    return res;
}

/* (v) standard deviation — O(n) (mean already computed once) */
double getStdDev(int a[], int n) {
    double m = getMean(a, n), s = 0;
    for (int i = 0; i < n; i++) s += (a[i] - m) * (a[i] - m);
    return sqrt(s / n);
}

/* (vi) mode — sort then scan, O(n log n) */
int getMode(int a[], int n) {
    int *b = malloc(n * sizeof(int));
    memcpy(b, a, n * sizeof(int));
    qsort(b, n, sizeof(int), cmp);
    int best = b[0], bestCnt = 1, curCnt = 1;
    for (int i = 1; i < n; i++) {
        curCnt = (b[i] == b[i - 1]) ? curCnt + 1 : 1;
        if (curCnt > bestCnt) { bestCnt = curCnt; best = b[i]; }
    }
    free(b);
    return best;
}

/* (vii) remove duplicates — sort then compact, O(n log n) */
int removeDup(int a[], int n) {
    qsort(a, n, sizeof(int), cmp);
    int j = 0;
    for (int i = 0; i < n; i++)
        if (i == 0 || a[i] != a[j - 1]) a[j++] = a[i];
    return j; /* new length */
}

/* (viii) reverse — O(n) */
void reverseArr(int a[], int n) {
    int i = 0, j = n - 1;
    while (i < j) { int t = a[i]; a[i] = a[j]; a[j] = t; i++; j--; }
}

/* (ix) partition so that elements < pivot appear AFTER elements >= pivot
   (mirror image of Lomuto partition) — O(n) */
int partitionGE(int a[], int n, int pivot) {
    int i = -1;
    for (int j = 0; j < n; j++) {
        if (a[j] >= pivot) {
            i++;
            int t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }
    return i + 1; /* boundary: [0,i] holds >=pivot, [i+1,n-1] holds <pivot */
}

void printArr(int a[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

int main(void) {
    int a[] = {5, 2, 9, 2, 5, 7, 1, 9, 3};
    int n = sizeof(a) / sizeof(a[0]);
    int b[9];

    printf("Original: "); printArr(a, n);

    printf("Max: %d\n", getMax(a, n));

    int f, s;
    firstSecond(a, n, &f, &s);
    printf("1st: %d, 2nd: %d\n", f, s);

    printf("Mean: %.2f\n", getMean(a, n));
    printf("Median: %.2f\n", getMedian(a, n));
    printf("StdDev: %.2f\n", getStdDev(a, n));
    printf("Mode: %d\n", getMode(a, n));

    memcpy(b, a, sizeof(a));
    int newLen = removeDup(b, n);
    printf("Unique (%d): ", newLen); printArr(b, newLen);

    memcpy(b, a, sizeof(a));
    reverseArr(b, n);
    printf("Reversed: "); printArr(b, n);

    memcpy(b, a, sizeof(a));
    int bnd = partitionGE(b, n, 5);
    printf("Partitioned (pivot=5, >=5 first): "); printArr(b, n);
    printf("Boundary index: %d\n", bnd);

    printf("\nComplexity summary (n = array size):\n"
           "max O(n) | 1st&2nd O(n) | mean O(n) | median O(n log n)\n"
           "stddev O(n) | mode O(n log n) | dedup O(n log n)\n"
           "reverse O(n) | partition O(n)\n");
    return 0;
}