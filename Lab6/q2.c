/* DAA Lab-06, Q2: n x n square matrix operations and worst-case complexities */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int **alloc2D(int n) {
    int **m = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) m[i] = calloc(n, sizeof(int));
    return m;
}
void free2D(int **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

/* (i) addition — O(n^2) */
void addMat(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

/* (ii) multiplication — O(n^3) with the naive triple loop */
void mulMat(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) C[i][j] += A[i][k] * B[k][j];
        }
}

/* (iii) zero-matrix check — O(n^2) */
int isZero(int **A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (A[i][j] != 0) return 0;
    return 1;
}

/* (iv) symmetric check — O(n^2) */
int isSymmetric(int **A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (A[i][j] != A[j][i]) return 0;
    return 1;
}

/* (v) determinant via cofactor expansion — O(n!) worst case
   (an O(n^3) Gaussian-elimination determinant is the practical alternative) */
double det(double **A, int n) {
    if (n == 1) return A[0][0];
    if (n == 2) return A[0][0] * A[1][1] - A[0][1] * A[1][0];

    double result = 0;
    double **sub = malloc((n - 1) * sizeof(double *));
    for (int i = 0; i < n - 1; i++) sub[i] = malloc((n - 1) * sizeof(double));

    for (int x = 0; x < n; x++) {
        int sr = 0;
        for (int i = 1; i < n; i++) {
            int sc = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                sub[sr][sc++] = A[i][j];
            }
            sr++;
        }
        double sign = (x % 2 == 0) ? 1 : -1;
        result += sign * A[0][x] * det(sub, n - 1);
    }
    for (int i = 0; i < n - 1; i++) free(sub[i]);
    free(sub);
    return result;
}

/* (vi) transpose in place — O(n^2) */
void transposeInPlace(int **A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            int t = A[i][j]; A[i][j] = A[j][i]; A[j][i] = t;
        }
}

/* (vii) dominant eigenvalue + eigenvector via power iteration
   — O(k * n^2) for k iterations (finds only the dominant pair;
   full spectrum needs O(n^3) methods like QR algorithm) */
void powerIteration(double **A, int n, int iters, double *vec, double *val) {
    for (int i = 0; i < n; i++) vec[i] = 1.0;

    double *y = malloc(n * sizeof(double));
    for (int it = 0; it < iters; it++) {
        for (int i = 0; i < n; i++) {
            y[i] = 0;
            for (int j = 0; j < n; j++) y[i] += A[i][j] * vec[j];
        }
        double norm = 0;
        for (int i = 0; i < n; i++) norm += y[i] * y[i];
        norm = sqrt(norm);
        if (norm == 0.0) {
            *val = 0.0;
            free(y);
            return;
        }
        for (int i = 0; i < n; i++) vec[i] = y[i] / norm;
    }
    /* Rayleigh quotient for eigenvalue */
    double num = 0, den = 0;
    for (int i = 0; i < n; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++) y[i] += A[i][j] * vec[j];
        num += vec[i] * y[i];
        den += vec[i] * vec[i];
    }
    *val = num / den;
    free(y);
}

void printMat(int **A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%4d ", A[i][j]);
        printf("\n");
    }
}

int main(void) {
    int n = 3;
    int **A = alloc2D(n), **B = alloc2D(n), **C = alloc2D(n);
    int vals[3][3] = {{2, 1, 0}, {1, 3, 1}, {0, 1, 2}};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { A[i][j] = vals[i][j]; B[i][j] = (i == j); }

    printf("A =\n"); printMat(A, n);

    addMat(A, B, C, n);
    printf("A+I =\n"); printMat(C, n);

    mulMat(A, B, C, n);
    printf("A*I =\n"); printMat(C, n);

    printf("isZero(A): %d\n", isZero(A, n));
    printf("isSymmetric(A): %d\n", isSymmetric(A, n));

    double **Ad = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        Ad[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) Ad[i][j] = A[i][j];
    }
    printf("det(A) = %.2f\n", det(Ad, n));

    transposeInPlace(A, n);
    printf("A^T =\n"); printMat(A, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) A[i][j] = vals[i][j]; /* restore */

    double vec[3], val;
    powerIteration(Ad, n, 100, vec, &val);
    printf("Dominant eigenvalue ~ %.4f\n", val);
    printf("Dominant eigenvector ~ [%.4f, %.4f, %.4f]\n", vec[0], vec[1], vec[2]);

    printf("\nComplexity summary (n = matrix dimension):\n"
           "add O(n^2) | multiply O(n^3) | isZero O(n^2) | isSymmetric O(n^2)\n"
           "determinant O(n!) | transpose O(n^2) | eigen (power iter) O(k n^2)\n");

    free2D(A, n); free2D(B, n); free2D(C, n);
    for (int i = 0; i < n; i++) free(Ad[i]);
    free(Ad);
    return 0;
}