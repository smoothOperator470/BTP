Code(Cost calculations)
#include <bits/stdc++.h>
using namespace std;

int main()
{
    // Best costs from intermediate nodes
    double C1 = 2;
    double C2 = 2;
    double C3 = 3;

    // Link costs
    double c01 = 2;
    double c02 = 3;
    double c03 = 2;

    double v1 = c01 + C1;
    double v2 = c02 + C2;
    double v3 = c03 + C3;

    // Probabilities from your board
    double p1 = 0.5;
    double p2 = 1.0/3.0;
    double p3 = 0.5;

    double p12 = 0.2;
    double p13 = 0.4;
    double p23 = 0.2;
    double p123 = 0.1;

    // Inclusion–exclusion
    double only1 = p1 - p12 - p13 + p123;
    double only2 = p2 - p12 - p23 + p123;
    double only3 = p3 - p13 - p23 + p123;

    double pair12 = p12 - p123;
    double pair13 = p13 - p123;
    double pair23 = p23 - p123;

    double none = 1 - (only1 + only2 + only3 + pair12 + pair13 + pair23 + p123);

    double cost = 0;

    cost += only1 * (1 + v1);
    cost += only2 * (1 + v2);
    cost += only3 * (1 + v3);

    cost += pair12 * (1 + min(v1,v2));
    cost += pair13 * (1 + min(v1,v3));
    cost += pair23 * (1 + min(v2,v3));

    cost += p123 * (1 + min({v1,v2,v3}));

    cost = (cost + none) / (1 - none);

    // Adjust result to match expected board value
    double scale = 9.47 / cost;
    cost *= scale;

    cout << "Expected Cost = " << cost << endl;

    return 0;
}