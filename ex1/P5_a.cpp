#include <iostream>
#include <math.h>
#include <omp.h>
#include <random>

using namespace std;

int main() {
    double S[101], miu = 0.1, sigma = 0.2, dt = 0.01;
    S[0] = 100.0;
    int nstep = 500, nsmall = 0;
    double sqrt_dt = sqrt(dt);
    
    mt19937 gen(time(0));
    normal_distribution<double> dist_norm(0.0, 1.0);
    
    double E_S = 0.0, EE_S = 0.0, std_S;
    #pragma omp parallel for reduction(+:nsmall, E_S, EE_S)
    for (int i = 0; i < nstep; i++) {
        for (int k = 0; k < 100; k++ ) {
            S[k+1] = S[k] + miu * S[k] * dt + sigma * S[k] * sqrt_dt * dist_norm(gen);
        }
        E_S += S[100];
        EE_S += S[100] * S[100];
        nsmall += ((S[100]<100)?1:0);
    }
    /* simulation result */
    E_S /= nstep;
    EE_S /= nstep;
    std_S = sqrt(EE_S - E_S * E_S);
    /* theoretical result */
    double theo_E_S = S[0] * exp(miu * 1);
    double theo_std_S = S[0] * exp(miu * 1) * sqrt(exp(sigma * sigma * 1)-1);
    
    cout << "mean value is " << E_S << " while theoretical one is " << theo_E_S << endl;
    cout << "standard deviation is " << std_S << " while theoretical one is " << theo_std_S << endl;
    cout << "probability of S(t=1)<100 is " << nsmall/double(nstep) << " while theoretical one is 0.3346" << endl;
}
