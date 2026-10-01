/* Q9: Collatz Conjecture explorer (modular)
 * Modes:
 *   1) Single start n : print trajectory, steps, peak value
 *   2) Interval [a,b] : longest trajectory and average steps (memoised)
 * Concepts: iterative control flow, functional decomposition,
 *           dynamic memory (memo cache), integer-overflow detection.
 * Complexity: single n -> O(steps) time. Interval -> with memoisation each
 *   value below b is computed once, so ~O(sum of steps until hitting cache),
 *   space O(b) for the cache. Nobody has proved the number of steps is finite
 *   for every n (open problem).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t u64;
#define CACHE_LIMIT 100000000ULL

/* One Collatz step; returns 0 on overflow (3n+1 would exceed u64). */
static int collatz_next(u64 n, u64 *out) {
    if (n % 2 == 0) { *out = n / 2; return 1; }
    if (n > (UINT64_MAX - 1) / 3) return 0;
    *out = 3 * n + 1; return 1;
}

typedef struct { u64 steps, peak; int overflow; } Stats;

static Stats analyse(u64 n, int print) {
    Stats s = {0, n, 0};
    if (print) printf("%llu", (unsigned long long)n);
    while (n != 1) {
        u64 nx;
        if (!collatz_next(n, &nx)) { s.overflow = 1; break; }
        n = nx; s.steps++;
        if (n > s.peak) s.peak = n;
        if (print) printf(" -> %llu", (unsigned long long)n);
    }
    if (print) printf("\n");
    return s;
}

static void mode_single(void) {
    unsigned long long n;
    printf("Starting value n >= 1: ");
    if (scanf("%llu", &n) != 1 || n < 1) { printf("Invalid input.\n"); return; }
    Stats s = analyse(n, 1);
    if (s.overflow) printf("Stopped: 64-bit overflow detected.\n");
    printf("Steps: %llu\nPeak value: %llu\n", (unsigned long long)s.steps, (unsigned long long)s.peak);
}

static void mode_interval(void) {
    unsigned long long a, b;
    printf("Interval a b (1 <= a <= b): ");
    if (scanf("%llu %llu", &a, &b) != 2 || a < 1 || a > b) { printf("Invalid input.\n"); return; }
    u64 cache_n = b < CACHE_LIMIT ? b : CACHE_LIMIT;
    uint32_t *cache = calloc(cache_n + 1, sizeof *cache);   /* steps to reach 1; 0 = unknown (except n=1) */
    if (!cache) { printf("Out of memory.\n"); return; }
    u64 best_n = a, best_steps = 0, total = 0, overflows = 0;
    for (u64 start = a; start <= b; start++) {
        u64 n = start, steps = 0; int ovf = 0;
        while (n != 1) {
            if (n <= cache_n && cache[n]) { steps += cache[n]; break; }
            u64 nx;
            if (!collatz_next(n, &nx)) { ovf = 1; break; }
            n = nx; steps++;
        }
        if (ovf) { overflows++; continue; }
        if (start <= cache_n) cache[start] = (uint32_t)steps;
        total += steps;
        if (steps > best_steps) { best_steps = steps; best_n = start; }
    }
    printf("Interval [%llu, %llu]\n", a, b);
    printf("Longest trajectory : n = %llu, %llu steps\n", (unsigned long long)best_n, (unsigned long long)best_steps);
    printf("Average steps      : %.3f\n", (double)total / (double)(b - a + 1 - overflows ? b - a + 1 - overflows : 1));
    if (overflows) printf("Overflowed starts  : %llu\n", (unsigned long long)overflows);
    free(cache);
}

int main(void) {
    int choice;
    printf("1) Single trajectory\n2) Interval analysis\nChoice: ");
    if (scanf("%d", &choice) != 1) return 1;
    if (choice == 1) mode_single();
    else if (choice == 2) mode_interval();
    else printf("Unknown choice.\n");
    return 0;
}
