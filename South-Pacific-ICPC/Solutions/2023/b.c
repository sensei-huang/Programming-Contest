/* Here’s a fast O(nm) dynamic-programming solution using affine-gap alignment (block deletion = splice with cost S+kC; mismatch transform = cost 
T). We keep three states per cell: end with a match/mismatch (M), a gap in B (delete from A, DA), or a gap in A (delete from B, DB), and compute with rolling rows to use 
O(m) memory. */


#include <stdio.h>
#include <string.h>
#include <limits.h>

static inline long long add_inf(long long a, long long b, long long INF) {
    return (a >= INF - b) ? INF : (a + b);
}

int main(void) {
    long long S, C, T;
    if (scanf("%lld %lld %lld", &S, &C, &T) != 3) return 0;

    char A[3005], B[3005];
    if (scanf("%3000s", A) != 1) return 0;
    if (scanf("%3000s", B) != 1) return 0;

    int n = (int)strlen(A);
    int m = (int)strlen(B);

    const long long INF = (1LL<<62);

    // Rolling arrays of length m+1
    static long long prevM[3005], prevDA[3005], prevDB[3005];
    static long long currM[3005], currDA[3005], currDB[3005];

    // Initialize row i=0
    prevM[0] = 0;
    prevDA[0] = INF;
    prevDB[0] = INF;
    
    for (int j = 1; j <= m; ++j) {
        // DB[0][j] = min( M[0][j-1]+S+C, DA[0][j-1]+S+C, DB[0][j-1]+C )
        long long v1 = add_inf(prevM[j-1], S + C, INF);
        long long v2 = add_inf(prevDA[j-1], S + C, INF); // prevDA for i=0 is INF
        long long v3 = add_inf(prevDB[j-1], C, INF);
        prevDB[j] = v3 < v1 ? v3 : v1;
        if (v2 < prevDB[j]) prevDB[j] = v2;

        prevM[j] = INF;    // M[0][j] impossible for j>0
        prevDA[j] = INF;   // DA[0][j] impossible
    }

    for (int i = 1; i <= n; ++i) {
        currM[0]  = INF; // M[i][0] impossible for i>0

        // DA[i][0] = min( M[i-1][0]+S+C, DB[i-1][0]+S+C, DA[i-1][0]+C )
        long long u1 = add_inf(prevM[0], S + C, INF);
        long long u2 = add_inf(prevDB[0], S + C, INF);
        long long u3 = add_inf(prevDA[0], C, INF);
        currDA[0] = u3 < u1 ? u3 : u1;
        if (u2 < currDA[0]) currDA[0] = u2;

        currDB[0] = INF; // DB[i][0] impossible

        for (int j = 1; j <= m; ++j) {
            // DA[i][j] = min( M[i-1][j]+S+C, DB[i-1][j]+S+C, DA[i-1][j]+C )
            long long da1 = add_inf(prevM[j], S + C, INF);
            long long da2 = add_inf(prevDB[j], S + C, INF);
            long long da3 = add_inf(prevDA[j], C, INF);
            long long DA = da3 < da1 ? da3 : da1;
            if (da2 < DA) DA = da2;
            currDA[j] = DA;

            // DB[i][j] = min( M[i][j-1]+S+C, DA[i][j-1]+S+C, DB[i][j-1]+C )
            long long db1 = add_inf(currM[j-1], S + C, INF);
            long long db2 = add_inf(currDA[j-1], S + C, INF);
            long long db3 = add_inf(currDB[j-1], C, INF);
            long long DB = db3 < db1 ? db3 : db1;
            if (db2 < DB) DB = db2;
            currDB[j] = DB;

            // M[i][j] = min( M[i-1][j-1], DA[i-1][j-1], DB[i-1][j-1] ) + cost
            long long base = prevM[j-1];
            if (prevDA[j-1] < base) base = prevDA[j-1];
            if (prevDB[j-1] < base) base = prevDB[j-1];
            long long cost = (A[i-1] == B[j-1]) ? 0 : T;
            currM[j] = add_inf(base, cost, INF);
        }

        // swap current row into previous
        for (int j = 0; j <= m; ++j) {
            prevM[j]  = currM[j];
            prevDA[j] = currDA[j];
            prevDB[j] = currDB[j];
        }
    }

    long long ans = prevM[m];
    if (prevDA[m] < ans) ans = prevDA[m];
    if (prevDB[m] < ans) ans = prevDB[m];

    printf("%lld\n", ans);
    return 0;
}
