/*
Problem A
A Game of Sets

Set is a very popular card game amongst the math community. The game of Set is played with a deck of
specialized cards. Each card is described by four properties: number, shape, colour, and shading. Each property
has three options: the number is one of (1, 2, or 3), the shape is one of (diamond, oval, or squiggle), the colour is
one of (green, purple, or red), and the shading is one of (empty, solid, or striped). This gives a total of 81 distinct
cards (34).
Three different cards form a set if each property is either all the same or all different on the three cards. For
example, the following is a set since the numbers are different (2, 1, 3), the shapes are different (oval, squiggle,
diamond), the colours are the same (all purple), and the shadings are different (striped, solid, empty):
However, this is not a set since the shapes do not satisfy the requirement (two cards with diamonds and one
with squiggle):
Your hand contains several cards. Determine if there are three cards in your hand that form a set.
Input
The first line of input consists of a single integer n (3 ≤ n ≤ 81), which is the number of cards in your hand.
The next n lines describe the cards in your hand. Each card has four strings describing its properties: its
number (one, two, or three), its shape (diamond, oval, or squiggle), its colour (green, purple, or
red), and its shading (empty, solid, or striped).
No two cards are identical.
Output
If there are no sets in your hand, display 0. Otherwise, display the three indices of the three distinct cards that
form a set (the first card in the input is card 1, the second card in the input is card 2, and so on). You may display
the indices in any order. If there are multiple solutions, any will be accepted.

Sample Input 1 Sample Output 1
3
two oval purple striped
one squiggle purple solid
three diamond purple empty
1 2 3

Sample Input 2 Sample Output 2
3
two diamond red striped
one squiggle green solid
three diamond purple empty
0
Sample Input 3 Sample Output 3
5
one squiggle purple solid
two diamond red striped
one squiggle green empty
two oval purple empty
two squiggle green solid
5 2 4
*/

#include <stdio.h>
#include <string.h>

#define MAXN 81

static int map_number(const char *s) {
    if (!strcmp(s, "one"))   return 0;
    if (!strcmp(s, "two"))   return 1;
    if (!strcmp(s, "three")) return 2;
    return -1;
}
static int map_shape(const char *s) {
    if (!strcmp(s, "diamond"))  return 0;
    if (!strcmp(s, "oval"))     return 1;
    if (!strcmp(s, "squiggle")) return 2;
    return -1;
}
static int map_colour(const char *s) {
    if (!strcmp(s, "green"))  return 0;
    if (!strcmp(s, "purple")) return 1;
    if (!strcmp(s, "red"))    return 2;
    return -1;
}
static int map_shading(const char *s) {
    if (!strcmp(s, "empty"))   return 0;
    if (!strcmp(s, "solid"))   return 1;
    if (!strcmp(s, "striped")) return 2;
    return -1;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    /* -------- Stage 1: read input into a table (n x 4, each in {0,1,2}) -------- */
    int card[MAXN][4];
    for (int i = 0; i < n; ++i) {
        char a[32], b[32], c[32], d[32];
        if (scanf("%31s %31s %31s %31s", a, b, c, d) != 4) return 0;
        card[i][0] = map_number(a);
        card[i][1] = map_shape(b);
        card[i][2] = map_colour(c);
        card[i][3] = map_shading(d);
    }

    /* -------- Stage 2: search for any set (sum mod 3 == 0 per attribute) -------- */
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                int ok = 1;
                for (int p = 0; p < 4; ++p) {
                    int s = card[i][p] + card[j][p] + card[k][p];
                    if (s % 3 != 0) { ok = 0; break; }
                }
                if (ok) {
                    /* -------- Stage 3: output result (1-based indices) -------- */
                    printf("%d %d %d\n", i + 1, j + 1, k + 1);
                    return 0;
                }
            }
        }
    }

    /* No set found */
    printf("0\n");
    return 0;
}
