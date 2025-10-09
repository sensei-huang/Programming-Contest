/**
Here’s an O(N) C solution.
Key idea: subtract (K) from each score so the problem becomes “longest subarray with sum (>= 0)”. 
Using prefix sums (P[i]) and a monotone decreasing stack of indices, we scan from the right to find the longest (j-i) with (P[j] >= P[i]).
Why it works?
Let (A_i = V_i - K). A subarray ([l..r]) has average (>= K) iff (\sum_{i=l} to r A_i > 0), i.e., (P[r]-P[l-1] >= 0). 
We precompute a strictly decreasing stack of prefix indices (candidate left ends). Scanning (j) from (N to 0), 
whenever (P[j] >= P[i]) for the current stack top (i), we can pop and update the best length (j-i). This finds the maximum in linear time and memory.
**/

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

int main(void) {
    long long Nll, K;
    if (!readll(&Nll) || !readll(&K)) return 0;
    int N = (int)Nll;

    // Prefix sums P[0..N], P[0]=0, P[i]=sum_{1..i}(Vi-K)
    int64_t *P = (int64_t*)malloc((size_t)(N + 1) * sizeof(int64_t));
    if (!P) return 1;
    P[0] = 0;

    for (int i = 1; i <= N; ++i) {
        long long v; readll(&v);
        P[i] = P[i-1] + (int64_t)(v - K);
    }

    // Build stack of indices with strictly decreasing P[idx]
    int *stk = (int*)malloc((size_t)(N + 1) * sizeof(int));
    if (!stk) return 1;
    int top = 0;
    stk[top++] = 0;
    for (int i = 1; i <= N; ++i) {
        if (P[i] < P[stk[top-1]]) stk[top++] = i;
    }

    // Scan from right: maximize j - i with P[j] >= P[i]
    int best = 0;
    for (int j = N; j >= 0 && top > 0; --j) {
        while (top > 0 && P[j] >= P[stk[top-1]]) {
            int i = stk[--top];
            if (j - i > best) best = j - i;
        }
    }

    printf("%d\n", best);

    free(stk);
    free(P);
    return 0;
}
