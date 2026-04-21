//PACKET TRANSMISSION CODE 
// sim_network.cpp
// Compile: g++ -std=c++17 sim_network.cpp -O2 -o sim_network

#include <bits/stdc++.h>
using namespace std;

/* ran2 RNG (same as your original program) */
double ran2(long &idum) {
    const int IM1 = 2147483563;
    const int IM2 = 2147483399;
    const double AM = (1.0/IM1);
    const int IA1 = 40014;
    const int IA2 = 40692;
    const int IQ1 = 53668;
    const int IQ2 = 52774;
    const int IR1 = 12211;
    const int IR2 = 3791;
    const int NTAB = 32;
    const int IMM1 = IM1 - 1;
    const int NDIV = 1 + IMM1/NTAB;
    const double RNMX = 1.0 - 1.2e-7;

    static long idum2 = 123456789;
    static long iv[NTAB];
    static long iy = 0;

    int j;
    long k;
    double temp;

    if (idum <= 0) {
        if (-(idum) < 1) idum = 1;
        else idum = -(idum);
        idum2 = idum;
        for (j = NTAB + 7; j >= 0; j--) {
            k = idum / IQ1;
            idum = IA1 * (idum - k * IQ1) - k * IR1;
            if (idum < 0) idum += IM1;
            if (j < NTAB) iv[j] = idum;
        }
        iy = iv[0];
    }

    k = idum / IQ1;
    idum = IA1 * (idum - k * IQ1) - k * IR1;
    if (idum < 0) idum += IM1;

    k = idum2 / IQ2;
    idum2 = IA2 * (idum2 - k * IQ2) - k * IR2;
    if (idum2 < 0) idum2 += IM2;

    j = iy / NDIV;
    iy = iv[j] - idum2;
    iv[j] = idum;
    if (iy < 1) iy += IMM1;

    temp = AM * iy;
    if (temp > RNMX) return RNMX;
    return temp;
}

int main() {

    const int TARGET_PACKETS = 10000;
    long seed = -static_cast<long>(time(nullptr) % 1000000 + 1);

    /* first hop probabilities */
    double p1 = 1.0/2.0;
    double p2 = 1.0/3.0;
    double p3 = 1.0/2.0;

    /* outcome probabilities */
    double p_none = (1-p1)*(1-p2)*(1-p3);      // 1/6
    double p1_only = p1*(1-p2)*(1-p3);         // 1/6
    double p2_only = (1-p1)*p2*(1-p3);         // 1/12
    double p3_only = (1-p1)*(1-p2)*p3;         // 1/6
    double p12 = p1*p2*(1-p3);                 // 1/12
    double p13 = p1*(1-p2)*p3;                 // 1/6
    double p23 = (1-p1)*p2*p3;                 // 1/12
    double p123 = p1*p2*p3;                    // 1/12

    /* aggregated downstream ETX costs (from your derivation) */
    const double C1 = 8.5;
    const double C2 = 9.0;
    const double C3 = 9.5;

    double total_cost = 0.0;

    for (int i = 0; i < TARGET_PACKETS; ++i) {

        double cost = 0.0;
        bool done = false;

        while (!done) {

            cost += 1.0;   // transmission from node 0

            double r = ran2(seed);

            if (r < p_none) {
                continue;
            }
            else if (r < p_none + p1_only) {
                cost += C1;
                done = true;
            }
            else if (r < p_none + p1_only + p2_only) {
                cost += C2;
                done = true;
            }
            else if (r < p_none + p1_only + p2_only + p3_only) {
                cost += C3;
                done = true;
            }
            else if (r < p_none + p1_only + p2_only + p3_only + p12) {
                cost += min(C1, C2);
                done = true;
            }
            else if (r < p_none + p1_only + p2_only + p3_only + p12 + p13) {
                cost += min(C1, C3);
                done = true;
            }
            else if (r < p_none + p1_only + p2_only + p3_only + p12 + p13 + p23) {
                cost += min(C2, C3);
                done = true;
            }
            else {
                cost += min({C1, C2, C3});
                done = true;
            }
        }

        total_cost += cost;
    }

    double avg = total_cost / TARGET_PACKETS;

    cout << fixed << setprecision(4);
    cout << "Packets simulated = " << TARGET_PACKETS << endl;
    cout << "Average ETX = " << avg << endl;
    cout << "Expected = 9.47" << endl;

    return 0;
}