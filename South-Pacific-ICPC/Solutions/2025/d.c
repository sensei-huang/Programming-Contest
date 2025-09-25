/********************
Here’s a clean O(N log N) solution based on one key observation:
For each word Si, let
mi = min(Si, reverse(Si)) (the lexicographically smaller form)
Mi = max(Si, reverse(Si)) (the larger form)
Across all choices of reversals:
The earliest index Si can achieve is the number of other words j whose best (i.e., lexicographically larger) form is still strictly less than mi.
→ Earliest = count{ Mj < mi }
(Pick mi as your orientation to minimize predecessors.)
The latest index Si can achieve is the number of other words j whose worst (i.e., lexicographically smaller) form is strictly less than Mi.
→ Latest = count{ mj < Mi }, and subtract 1 if Si is not a palindrome (because your own mi is < your Mi and would otherwise be counted).
So we precompute arrays m[ ] and M[ ], sort copies of them, and answer with two binary searches per word.

********************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Portable strdup */
static char* dupstr(const char* s) {
    size_t n = strlen(s) + 1;
    char* p = (char*)malloc(n);
    if (!p) { perror("malloc"); exit(1); }
    memcpy(p, s, n);
    return p;
}

/* Reverse-copy a string */
static char* revdup(const char* s) {
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    if (!p) { perror("malloc"); exit(1); }
    for (size_t i = 0; i < n; ++i) p[i] = s[n - 1 - i];
    p[n] = '\0';
    return p;
}

/* qsort comparator for array of char* (lexicographic) */
static int cmp_strptr(const void* a, const void* b) {
    const char* const sa = *(const char* const*)a;
    const char* const sb = *(const char* const*)b;
    return strcmp(sa, sb);
}

/* lower_bound on sorted array of char* (count of elements < key) */
static int lower_bound_str(char** arr, int n, const char* key) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (strcmp(arr[mid], key) < 0) lo = mid + 1;
        else hi = mid;
    }
    return lo; /* number of elements < key */
}

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) return 0;

    /* Constraints say total input length <= 1.5e6, safe single buffer */
    char *buf = (char*)malloc(1500000 + 10);
    if (!buf) { perror("malloc"); return 1; }

    char **orig = (char**)malloc((size_t)N * sizeof(char*));
    char **rev  = (char**)malloc((size_t)N * sizeof(char*));
    char **m    = (char**)malloc((size_t)N * sizeof(char*)); /* min form */
    char **M    = (char**)malloc((size_t)N * sizeof(char*)); /* max form */
    int  *is_pal = (int*)calloc((size_t)N, sizeof(int));
    if (!orig || !rev || !m || !M || !is_pal) { perror("malloc"); return 1; }

    for (int i = 0; i < N; ++i) {
        if (scanf("%s", buf) != 1) return 0;
        orig[i] = dupstr(buf);
        rev[i]  = revdup(buf);

        int c = strcmp(orig[i], rev[i]);
        if (c <= 0) { m[i] = orig[i]; M[i] = rev[i]; }
        else        { m[i] = rev[i];  M[i] = orig[i]; }
        is_pal[i] = (c == 0);
    }

    /* Build sorted copies for binary searches */
    char **sorted_m = (char**)malloc((size_t)N * sizeof(char*));
    char **sorted_M = (char**)malloc((size_t)N * sizeof(char*));
    if (!sorted_m || !sorted_M) { perror("malloc"); return 1; }
    for (int i = 0; i < N; ++i) { sorted_m[i] = m[i]; sorted_M[i] = M[i]; }

    qsort(sorted_m, (size_t)N, sizeof(char*), cmp_strptr);
    qsort(sorted_M, (size_t)N, sizeof(char*), cmp_strptr);

    /* For each word:
       earliest = count of Mj < mi      => lower_bound(sorted_M, mi)
       latest   = count of mj < Mi  (-1 if not palindrome) => lower_bound(sorted_m, Mi) - (!is_pal)
    */
    for (int i = 0; i < N; ++i) {
        int earliest = lower_bound_str(sorted_M, N, m[i]);

        int latest = lower_bound_str(sorted_m, N, M[i]);
        if (!is_pal[i]) latest -= 1; /* exclude counting self's m[i] */

        if (latest < 0) latest = 0; /* safety, though shouldn't occur */

        printf("%d %d\n", earliest, latest);
    }

    /* Freeing individual strings is optional (program exit will reclaim). */
    free(sorted_m); free(sorted_M);
    free(is_pal);
    for (int i = 0; i < N; ++i) { free(orig[i]); free(rev[i]); }
    free(orig); free(rev); free(m); free(M); free(buf);
    return 0;
}
