#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static inline int readll(long long *out) {
    int c = getchar_unlocked();
    int neg = 0;
    while (c != '-' && (c < '0' || c > '9')) { if (c == EOF) return 0; c = getchar_unlocked(); }
    if (c == '-') { neg = 1; c = getchar_unlocked(); }
    long long x = 0;
    while (c >= '0' && c <= '9') { x = x*10 + (c - '0'); c = getchar_unlocked(); }
    *out = neg ? -x : x;
    return 1;
}

static int cmp_ll(const void *a, const void *b) {
    long long x = *(const long long*)a, y = *(const long long*)b;
    return (x < y) ? -1 : (x > y);
}

/* Count unordered pairs in sorted array a[0..n-1] whose sum == S */
static long long count_pairs_sum_sorted(const long long *a, int n, long long S) {
    int i = 0, j = n - 1;
    long long cnt = 0;
    while (i < j) {
        long long s = a[i] + a[j];
        if (s == S) {
            if (a[i] == a[j]) {
                long long m = (long long)(j - i + 1);
                cnt += m * (m - 1) / 2;
                break;
            } else {
                long long vi = a[i], vj = a[j];
                long long ci = 0, cj = 0;
                while (i < j && a[i] == vi) { ++ci; ++i; }
                while (j >= i && a[j] == vj) { ++cj; --j; }
                cnt += ci * cj;
            }
        } else if (s < S) {
            ++i;
        } else {
            --j;
        }
    }
    return cnt;
}

int main(void) {
    long long Nll;
    if (!readll(&Nll)) return 0;
    int N = (int)Nll;

    int *parent = (int*)malloc((size_t)(N + 1) * sizeof(int));
    long long *prod = (long long*)malloc((size_t)(N + 1) * sizeof(long long));
    int *deg = (int*)calloc((size_t)(N + 1), sizeof(int));
    if (!parent || !prod || !deg) return 1;

    for (int i = 1; i <= N; ++i) {
        long long Mll, Pll;
        readll(&Mll); readll(&Pll);
        int M = (int)Mll;
        parent[i] = M;  /* CEO has -1 */
        prod[i] = Pll;
        if (M != -1) deg[M]++;
    }

    /* Build CSR-like children arrays */
    int *start = (int*)malloc((size_t)(N + 2) * sizeof(int));
    int *nextpos = (int*)malloc((size_t)(N + 1) * sizeof(int));
    int total_edges = 0, Wmax = 0;
    for (int i = 1; i <= N; ++i) {
        start[i] = total_edges;
        total_edges += deg[i];
        if (deg[i] > Wmax) Wmax = deg[i];
    }
    start[N + 1] = total_edges;
    for (int i = 1; i <= N; ++i) nextpos[i] = start[i];

    int *child = (int*)malloc((size_t)total_edges * sizeof(int));
    if (!start || !nextpos || !child) return 1;

    for (int i = 2; i <= N; ++i) {  /* employee 1 has no manager */
        int m = parent[i];
        if (m != -1) child[nextpos[m]++] = i;
    }

    /* Temporary buffer for child productivities when recomputing a manager’s pairs */
    long long *buf = (long long*)malloc((size_t)(Wmax > 1 ? Wmax : 1) * sizeof(long long));
    if (!buf) return 1;

    /* Initial synergy */
    long long synergy = 0;
    for (int m = 1; m <= N; ++m) {
        int len = deg[m];
        if (len < 2) continue;
        int st = start[m];
        for (int k = 0; k < len; ++k) buf[k] = prod[ child[st + k] ];
        qsort(buf, (size_t)len, sizeof(long long), cmp_ll);
        synergy += count_pairs_sum_sorted(buf, len, prod[m]);
    }

    long long Ill;
    readll(&Ill);
    int I = (int)Ill;

    for (int q = 0; q < I; ++q) {
        long long Ell, Xll;
        readll(&Ell); readll(&Xll);
        int E = (int)Ell;
        long long oldP = prod[E];
        long long newP = oldP + Xll;

        /* 1) Effect under E's manager: pairs (E, y) in that child set */
        int m = parent[E];
        if (m != -1) {
            long long comp_old = prod[m] - oldP;
            long long comp_new = prod[m] - newP;
            long long cnt_old = 0, cnt_new = 0;
            int st = start[m], len = deg[m];
            for (int k = 0; k < len; ++k) {
                int y = child[st + k];
                if (y == E) continue;
                if (prod[y] == comp_old) ++cnt_old;
                if (prod[y] == comp_new) ++cnt_new;
            }
            synergy += (cnt_new - cnt_old);
        }

        /* Apply the productivity update */
        prod[E] = newP;

        /* 2) Effect as a manager: target sum among E's own children changes */
        int lenE = deg[E];
        if (lenE >= 2) {
            int stE = start[E];
            for (int k = 0; k < lenE; ++k) buf[k] = prod[ child[stE + k] ];
            qsort(buf, (size_t)lenE, sizeof(long long), cmp_ll);
            long long old_pairs = count_pairs_sum_sorted(buf, lenE, oldP);
            long long new_pairs = count_pairs_sum_sorted(buf, lenE, newP);
            synergy += (new_pairs - old_pairs);
        }

        printf("%lld\n", synergy);
    }

    free(buf);
    free(child);
    free(nextpos);
    free(start);
    free(deg);
    free(prod);
    free(parent);
    return 0;
}
