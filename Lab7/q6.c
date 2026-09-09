/* ============================================================
   Q6: THE BEST TIME TO BE ALIVE
   ------------------------------------------------------------
   Input: for each of n scientists, a birth year b[i] and a
   death year d[i]  (b[i] <= d[i]).
   Find the year (or years) during which the largest number of
   these scientists were alive simultaneously.

   Rule for ties: "if A died the same year B was born, that death
   happens BEFORE that birth" -> at a shared year, we must process
   the DEATH event before the BIRTH event, so A and B are never
   counted as alive together in that year.

   ALGORITHM (classic sweep-line, same idea as "max overlap of
   intervals" / "meeting rooms"):
     1) Make 2n events: (b[i], +1)  and  (d[i]+1, -1)
        -- using d[i]+1 for the death makes a person "alive" for
           years b[i]..d[i] inclusive, and naturally removes them
           strictly before their death year's overlap is broken,
           matching the tie rule above.
     2) Sort events by year.
     3) Sweep left to right, keeping a running count; the running
        count is exactly the number of people alive that year.
        Track the year where the count peaks.

   Complexity: O(n log n) for the sort, O(n) for the sweep.
   ============================================================ */
#include <stdio.h>
#include <stdlib.h>

typedef struct { int year, delta; } Event;

int cmp(const void *a, const void *b) {
    const Event *x = a, *y = b;
    if (x->year != y->year)
        return (x->year > y->year) - (x->year < y->year);
    return (x->delta > y->delta) - (x->delta < y->delta); /* death before birth */
}

int main(void) {
    int n;
    printf("Enter number of scientists: ");
    scanf("%d", &n);

    Event ev[2*n];
    for (int i = 0; i < n; i++) {
        int b, d;
        printf("Scientist %d - birth year, death year: ", i+1);
        scanf("%d %d", &b, &d);
        ev[2*i]     = (Event){b,   +1};
        ev[2*i + 1] = (Event){d+1, -1};   /* alive through year d, gone from d+1 */
    }

    qsort(ev, 2*n, sizeof(Event), cmp);

    int alive = 0, best = 0, bestYear = 0;
    for (int i = 0; i < 2*n; i++) {
        alive += ev[i].delta;
        if (alive > best) { best = alive; bestYear = ev[i].year; }
    }

    printf("\nMax scientists alive at once = %d\n", best);
    printf("That peak starts in year       = %d\n", bestYear);
    return 0;
}
