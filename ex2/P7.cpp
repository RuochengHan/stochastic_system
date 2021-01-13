#include <iostream>
#include <math.h>
#include <omp.h>
#include <random>

using namespace std;

int main() {
    double *R;
    double a = 1, b = 1, c = 1, sigma = 1, dt = 0.01, ntime;
    int nstep = 100, nt;
    double sqrt_dt = sqrt(dt);
    
    cout << "input time limit:" << endl;
    cin >> ntime;
    nt = int (ntime / dt);
    cout << nt << endl;
    R = (double*)malloc((nt+1)*sizeof(double));
    cout << "input R0 = " << endl;
    cin >> R[0];
    
    cout << R[0] << endl;
    
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist_norm(0.0, 1.0);
    
    double E_R = 0.0, EE_R = 0.0, std_R;
//    #pragma omp parallel for reduction(+:E_R, EE_R)
    for (int i = 0; i < nstep; i++) {
        for (int k = 0; k < nt; k++ ) {
            R[k+1] = R[k] + a * (b - c * R[k]) * dt + sigma * sqrt(R[k]) * sqrt_dt * dist_norm(gen);
            cout << R[k+1] << endl;
        }
        E_R += R[nt];
        EE_R += R[nt] * R[nt];
        cout << E_R << endl;
    }
    /* simulation result */
    E_R /= nstep;
    EE_R /= nstep;
    std_R = sqrt(EE_R - E_R * E_R);
    
    cout << "mean value is " << E_R << endl;
    cout << "standard deviation is " << std_R << endl;
}
