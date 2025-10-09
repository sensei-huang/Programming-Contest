/**
* Let the input set of primes be (S={p_1,\dots,p_N}) (with (0\le N\le 6)).
* A partition (S=S_1\cup S_2) (disjoint) yields one number (\prod S_1 + \prod S_2).
* To avoid counting each partition twice (since ((S_1,S_2)) and ((S_2,S_1)) give the same sum), 
  we fix the first prime (p_1) to be in (S_1) and enumerate all subsets that include (p_1).
  (Edge case (N=0): the only partition is (\varnothing,\varnothing\Rightarrow 1+1=2).)
* For each resulting value, factor it by trial division (numbers are (\le) about (3.5\times 10^{12}), so trial division up to (\sqrt{n}) is fast).
* Collect all prime factors found, sort, and print unique.

**/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int cmp_ull(const void *a, const void *b) {
    unsigned long long x = *(const unsigned long long*)a;
    unsigned long long y = *(const unsigned long long*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

static void add_factor(unsigned long long f,
                       unsigned long long **arr, int *sz, int *cap) {
    if (*cap == 0) {
        *cap = 16;
        *arr = (unsigned long long*)malloc((size_t)(*cap) * sizeof(unsigned long long));
    } else if (*sz == *cap) {
        *cap <<= 1;
        *arr = (unsigned long long*)realloc(*arr, (size_t)(*cap) * sizeof(unsigned long long));
    }
    (*arr)[(*sz)++] = f;
}

/* Factor n by trial division; append prime factors (once per prime) into ans[] */
static void factor_once(unsigned long long n,
                        unsigned long long **ans, int *asz, int *acap) {
    if (n < 2) return;
    if ((n & 1ULL) == 0ULL) {
        add_factor(2ULL, ans, asz, acap);
        while ((n & 1ULL) == 0ULL) n >>= 1;
    }
    for (unsigned long long d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) {
            add_factor(d, ans, asz, acap);
            while (n % d == 0ULL) n /= d;
        }
    }
    if (n > 1ULL) add_factor(n, ans, asz, acap);
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    unsigned long long p[6];
    for (int i = 0; i < N; ++i) scanf("%llu", &p[i]);

    /* Edge case: N == 0 => only partition {} + {} => 1 + 1 = 2 */
    if (N == 0) {
        printf("1 2\n");
        return 0;
    }

    /* Precompute total product of all primes (fits in 64-bit for N<=6, primes<=127) */
    unsigned long long totalProd = 1ULL;
    for (int i = 0; i < N; ++i) totalProd *= p[i];

    /* Generate E(S): for uniqueness, only consider subsets where bit 0 (p[0]) is in S1 */
    unsigned long long vals[1 << 5]; /* at most 2^(N-1) <= 32 */
    int vcnt = 0;
    int maxMask = 1 << N;
    for (int mask = 0; mask < maxMask; ++mask) {
        if ((mask & 1) == 0) continue; /* enforce p[0] in S1 to avoid duplicates */
        unsigned long long prod1 = 1ULL;
        for (int i = 0; i < N; ++i) {
            if (mask & (1 << i)) prod1 *= p[i];
        }
        unsigned long long prod2 = totalProd / prod1;  /* complement product */
        unsigned long long val = prod1 + prod2;
        vals[vcnt++] = val;
    }

    /* Optional: sort+unique E(S) (though above rule already avoids duplicates) */
    qsort(vals, (size_t)vcnt, sizeof(unsigned long long), cmp_ull);
    int uniq_vcnt = 0;
    for (int i = 0; i < vcnt; ++i) {
        if (i == 0 || vals[i] != vals[i-1]) vals[uniq_vcnt++] = vals[i];
    }

    /* Factor each value and collect all prime factors */
    unsigned long long *ans = NULL; int asz = 0, acap = 0;
    for (int i = 0; i < uniq_vcnt; ++i) {
        factor_once(vals[i], &ans, &asz, &acap);
    }

    /* Sort and unique the prime factors */
    qsort(ans, (size_t)asz, sizeof(unsigned long long), cmp_ull);
    int u = 0;
    for (int i = 0; i < asz; ++i) {
        if (i == 0 || ans[i] != ans[i-1]) ans[u++] = ans[i];
    }

    /* Output: count + ascending primes */
    printf("%d", u);
    for (int i = 0; i < u; ++i) printf(" %llu", ans[i]);
    printf("\n");

    free(ans);
    return 0;
}
