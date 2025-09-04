/* You just need a “wiggle” order: make every internal element a strict local min or max. Then it can’t be the median of its three-stick window.

A simple way: sort, then swap each adjacent pair 
(0↔1, 2↔3,… )
(0↔1, 2↔3,…). This guarantees the zigzag property for distinct lengths.

Here’s a clean C solution. */

#include <stdio.h>
#include <stdlib.h>

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int *a = (int *)malloc(n * sizeof(int));
    if (!a) return 0;

    for (int i = 0; i < n; ++i) scanf("%d", &a[i]);

    qsort(a, n, sizeof(int), cmp_int);     // sort ascending

    // swap adjacent pairs: (0,1), (2,3), ...
    for (int i = 0; i + 1 < n; i += 2) {
        int t = a[i];
        a[i] = a[i + 1];
        a[i + 1] = t;
    }

    for (int i = 0; i < n; ++i) {
        if (i) putchar(' ');
        printf("%d", a[i]);
    }
    putchar('\n');

    free(a);
    return 0;
}
