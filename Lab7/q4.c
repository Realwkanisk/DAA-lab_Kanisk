/* ============================================================
   Q4: SECURITY SWITCHES
   ------------------------------------------------------------
   Switches s[1..n], s[n] = rightmost. All start ON, we want all OFF.
     - s[n] (rightmost) can be toggled freely.
     - s[i] (i<n) can be toggled only if s[i+1]=ON and every
       switch to the right of s[i+1] is OFF.

   KEY FACT: this exact toggle rule is precisely the rule used to
   step through the standard reflected binary GRAY CODE sequence
   G(0), G(1), G(2), ... , G(2^n - 1), where consecutive values
   differ in exactly one bit, and the whole sequence is a
   Hamiltonian path over all 2^n switch-states. Since it is a
   PATH (every state has at most 2 neighbours), the shortest
   route between any two states is unique and simply the
   difference of their positions (indices) on that path.

   G(0) = all switches OFF.
   We need the index m such that G(m) = all switches ON
   (that index m is the minimum number of moves, because moving
   from index 0 to index m along the path takes exactly m steps).

   m is obtained with the standard "Gray code -> binary" inverse:
        b = g;  while (g >>= 1) b ^= g;
   applied to g = (2^n - 1) (all-ones bit pattern).

   This gives the closed form (equivalent, verified by hand for
   n=1,2,3):           moves(n) = floor( 2^(n+1) / 3 )

   Once we know m, we regenerate the actual switch-flip sequence
   by walking the Gray code from index m down to 0; consecutive
   Gray codes differ in exactly one bit -> that bit is the switch
   to flip on that move.

   Complexity:
     Finding m               : O(n)          (bit shifts)
     Producing all moves     : O(n * moves(n)) = O(n * 2^n) worst case
                                (unavoidable: the answer itself has
                                 exponentially many moves)
   ============================================================ */
#include <stdio.h>

#define MAXN 20

/* standard reflected binary Gray code encode/decode */
unsigned toGray(unsigned b)  { return b ^ (b >> 1); }
unsigned fromGray(unsigned g) {
    unsigned b = g;
    while (g >>= 1) b ^= g;
    return b;
}

int main(void) {
    int n;
    printf("Enter number of switches n: ");
    scanf("%d", &n);
    if (n < 1 || n > MAXN) { printf("out of range\n"); return 1; }

    unsigned allOn = (1u << n) - 1;   /* bit pattern with all n switches ON */
    unsigned m = fromGray(allOn);     /* index of the all-ON state on the Gray path */

    printf("\nMinimum number of moves = %u\n", m);

    /* also print via the closed-form floor(2^(n+1)/3) as a cross-check */
    unsigned long check = (1UL << (n+1)) / 3;
    printf("Cross-check floor(2^(n+1)/3) = %lu  -> %s\n",
           check, (check == m) ? "MATCH" : "MISMATCH");

    /* --- regenerate & print the actual move sequence --- */
    printf("\n-- Move sequence (switch numbered 1=leftmost .. n=rightmost) --\n");
    unsigned cur = m;
    for (unsigned k = m; k >= 1; k--) {
        unsigned g1 = toGray(k);
        unsigned g2 = toGray(k-1);
        unsigned diff = g1 ^ g2;          /* exactly one bit set */
        int bit = 0;
        while ((diff >> bit) != 1) bit++; /* find that bit position (0=rightmost) */
        int switchNo = n - bit;           /* convert to 1..n, rightmost = n */
        printf("Move %2u: toggle switch %d\n", m - k + 1, switchNo);
        if (k == 0) break;                /* avoid unsigned underflow */
    }
    (void)cur;
    return 0;
}
