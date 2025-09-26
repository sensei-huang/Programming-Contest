#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef long double LD;
typedef struct { LD x, y; } Pt;

static inline LD cross(Pt a, Pt b){ return a.x*b.y - a.y*b.x; }
static inline LD norm(Pt a){ return sqrtl(a.x*a.x + a.y*a.y); }

int main(void){
    int n;
    if (scanf("%d", &n) != 1) return 0;
    Pt *p = (Pt*)malloc(sizeof(Pt)*n);
    for (int i = 0; i < n; ++i){
        int X, Y; scanf("%d %d", &X, &Y);
        p[i].x = (LD)X; p[i].y = (LD)Y;
    }

    // Find spiders with min and max polar angles (X,Y >= 1 so no wrap issues).
    int iMin = 0, iMax = 0;
    LD aMin = atan2l(p[0].y, p[0].x), aMax = aMin;
    for (int i = 1; i < n; ++i){
        LD a = atan2l(p[i].y, p[i].x);
        if (a < aMin) { aMin = a; iMin = i; }
        if (a > aMax) { aMax = a; iMax = i; }
    }

    // Unit rays u (min angle) and v (max angle)
    Pt u = p[iMin], v = p[iMax];
    LD nu = norm(u), nv = norm(v);
    u.x /= nu; u.y /= nu; v.x /= nv; v.y /= nv;

    // sin(theta) = u x v > 0 (problem guarantees not all collinear)
    const LD EPS = 1e-18L;
    LD sinth = cross(u, v);
    if (sinth <= 0) sinth = EPS;

    // Map all points to (s_i, t_i)
    LD *s = (LD*)malloc(sizeof(LD)*n);
    LD *t = (LD*)malloc(sizeof(LD)*n);
    for (int i = 0; i < n; ++i){
        s[i] = cross(p[i], v) / sinth;
        t[i] = cross(u, p[i]) / sinth;
        if (s[i] < 0 && s[i] > -1e-15L) s[i] = 0;
        if (t[i] < 0 && t[i] > -1e-15L) t[i] = 0;
    }

    LD best_xy = 0;

    // 1) Single-constraint candidates: x=1/(2s_k), y=1/(2t_k)
    for (int k = 0; k < n; ++k){
        LD sk = s[k], tk = t[k];
        if (sk <= 1e-18L || tk <= 1e-18L) continue; // needs both positive
        LD x = 1.0L / (2.0L * sk);
        LD y = 1.0L / (2.0L * tk);
        int ok = 1;
        for (int i = 0; i < n; ++i){
            if (x * s[i] + y * t[i] > 1.0L + 1e-12L){ ok = 0; break; }
        }
        if (ok){
            LD xy = x * y;
            if (xy > best_xy) best_xy = xy;
        }
    }

    // 2) Two-constraint (vertex) candidates: intersect lines i and j
    for (int i = 0; i < n; ++i){
        for (int j = i + 1; j < n; ++j){
            LD si = s[i], ti = t[i], sj = s[j], tj = t[j];
            LD D = si * tj - sj * ti;
            if (fabsl(D) < 1e-24L) continue;
            LD x = (tj - ti) / D;
            LD y = (si - sj) / D;
            if (x <= 0 || y <= 0) continue;
            int ok = 1;
            for (int k = 0; k < n; ++k){
                if (x * s[k] + y * t[k] > 1.0L + 1e-12L){ ok = 0; break; }
            }
            if (ok){
                LD xy = x * y;
                if (xy > best_xy) best_xy = xy;
            }
        }
    }

    LD area = 0.5L * sinth / best_xy;
    printf("%.12Lf\n", area);

    free(s); free(t); free(p);
    return 0;
}
