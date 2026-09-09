/* DAA Lab-06, Q3: Convolution of vectors A (len m) and B (len n), n >= m,
   via FFT-based divide & conquer — O(n log n)

   Idea: convolution of A and B = coefficients of the product polynomial
   A(x) * B(x). Evaluate both at roots of unity with FFT (divide & conquer,
   O(n log n)), multiply pointwise (O(n)), then interpolate back with
   inverse FFT (O(n log n)).                                              */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef double complex cplx;

/* Cooley-Tukey FFT, divide & conquer, O(n log n). n must be a power of 2. */
void fft(cplx *a, int n, int invert) {
    if (n == 1) return;

    cplx *even = malloc((n / 2) * sizeof(cplx));
    cplx *odd  = malloc((n / 2) * sizeof(cplx));
    for (int i = 0; i < n / 2; i++) { even[i] = a[2 * i]; odd[i] = a[2 * i + 1]; }

    fft(even, n / 2, invert);   /* T(n) = 2T(n/2) + O(n) => O(n log n) */
    fft(odd, n / 2, invert);

    double ang = 2 * M_PI / n * (invert ? -1 : 1);
    cplx w = 1, wn = cexp(I * ang);
    for (int i = 0; i < n / 2; i++) {
        cplx t = w * odd[i];
        a[i]         = even[i] + t;
        a[i + n / 2] = even[i] - t;
        if (invert) { a[i] /= 2; a[i + n / 2] /= 2; }
        w *= wn;
    }
    free(even); free(odd);
}

/* Convolve A[0..m-1] with B[0..n-1] -> C[0..m+n-2] */
void convolve(int A[], int m, int B[], int n, long long C[]) {
    int size = 1;
    while (size < m + n) size <<= 1;

    cplx *fa = calloc(size, sizeof(cplx));
    cplx *fb = calloc(size, sizeof(cplx));
    for (int i = 0; i < m; i++) fa[i] = A[i];
    for (int i = 0; i < n; i++) fb[i] = B[i];

    fft(fa, size, 0);
    fft(fb, size, 0);
    for (int i = 0; i < size; i++) fa[i] *= fb[i];   /* pointwise, O(n) */
    fft(fa, size, 1);

    for (int i = 0; i < m + n - 1; i++) C[i] = (long long)llround(creal(fa[i]));

    free(fa); free(fb);
}

int main(void) {
    int A[] = {1, 2, 3};          /* m = 3 */
    int B[] = {4, 5, 6, 7};       /* n = 4, n >= m */
    int m = 3, n = 4;

    long long C[6];
    convolve(A, m, B, n, C);

    printf("A = "); for (int i = 0; i < m; i++) printf("%d ", A[i]); printf("\n");
    printf("B = "); for (int i = 0; i < n; i++) printf("%d ", B[i]); printf("\n");
    printf("C = A*B (convolution) = ");
    for (int i = 0; i < m + n - 1; i++) printf("%lld ", C[i]);
    printf("\n");

    printf("\nComplexity: FFT O(n log n) x2 + pointwise mult O(n) "
           "+ inverse FFT O(n log n) = O(n log n) overall.\n");
    return 0;
}

