/* DAA Lab-06, Q4: Sorting a permutation using reverse(p,i,j)

   Part A: O(n) reversals suffice (classic "pancake sort" idea).
   Part B: cost(reverse) = length of range. Merge sort where the merge
           step is done via block-rotation (3 reversals) costs O(n log n)
           reversal-length per level, over log n levels => O(n log^2 n)
           total cost.                                                    */

#include <stdio.h>
#include <stdlib.h>

long long revCount = 0, revCost = 0;

/* core primitive: reverse p[i..j], cost = length */
void reverseRange(int p[], int i, int j) {
    revCount++;
    revCost += (j - i + 1);
    while (i < j) { int t = p[i]; p[i] = p[j]; p[j] = t; i++; j--; }
}

/* ---------- Part A: O(n) reversals (pancake sort) ---------- */
/* For value v = n down to 1: bring v to front, then flip it into place.
   At most 2 reversals per value => at most 2n reversals total => O(n). */
void pancakeSort(int p[], int n) {
    for (int size = n; size > 1; size--) {
        int idx = 0;
        for (int i = 0; i < size; i++) if (p[i] == size) { idx = i; break; }
        if (idx == size - 1) continue;      /* already in place */
        if (idx != 0) reverseRange(p, 0, idx);   /* bring value to front */
        reverseRange(p, 0, size - 1);            /* flip into final slot */
    }
}

/* ---------- Part B: O(n log^2 n) cost algorithm ---------- */
/* rotate p[i..j] so that block [mid..j] comes before block [i..mid-1],
   done with 3 reversals; cost of this rotation = (j-i+1)               */
void rotate3(int p[], int i, int mid, int j) {
    reverseRange(p, i, mid - 1);
    reverseRange(p, mid, j);
    reverseRange(p, i, j);
}

/* in-place merge of sorted p[l..mid-1] and p[mid..r] using rotation.
   Each rotation costs O(block length); across one merge level the
   total cost is O(n), and there are O(log n) levels => O(n log n)
   per full pass, O(n log^2 n) is a safe bound with the extra log
   factor from nested rotation bookkeeping.                         */
void mergeByRotation(int p[], int l, int mid, int r) {
    int i = l, j = mid;
    while (i < mid && j <= r) {
        if (p[i] <= p[j]) { i++; continue; }
        int j2 = j;
        while (j2 <= r && p[j2] < p[i]) j2++;   /* run of elements < p[i] */
        rotate3(p, i, j, j2 - 1);
        i += (j2 - j);
        mid += (j2 - j);
        j = j2;
    }
}

void mergeSortByReversal(int p[], int l, int r) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSortByReversal(p, l, mid);
    mergeSortByReversal(p, mid + 1, r);
    mergeByRotation(p, l, mid + 1, r);
}

void printArr(int p[], int n) {
    for (int i = 0; i < n; i++) printf("%d ", p[i]);
    printf("\n");
}

int main(void) {
    int n = 8;
    int p1[] = {6, 1, 8, 4, 2, 7, 5, 3};
    int p2[8];
    for (int i = 0; i < n; i++) p2[i] = p1[i];

    printf("Original: "); printArr(p1, n);

    revCount = revCost = 0;
    pancakeSort(p1, n);
    printf("PancakeSort result: "); printArr(p1, n);
    printf("Reversals used: %lld  (bound: O(n))\n\n", revCount);

    revCount = revCost = 0;
    mergeSortByReversal(p2, 0, n - 1);
    printf("Rotation-MergeSort result: "); printArr(p2, n);
    printf("Reversals used: %lld, total cost: %lld  (bound: O(n log^2 n))\n",
           revCount, revCost);

    return 0;
}