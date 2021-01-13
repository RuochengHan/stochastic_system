#include <iostream>
#include <math.h>
#include <omp.h>
#include <random>

using namespace std;

__global__ void calcu(double *d_v_S) {
    
    double  miu = 0.1, sigma = 0.2, dt = 0.01;
    int nstep = 500, nsmall = 0;
    double sqrt_dt = sqrt(dt);
    double S[101];
    S[0] = 100.0;
    
    int tid = threadIdx.x;
    if (tid < nstep) {
        for (int k = 0; k < 100; k++ ) {
            S[k+1] = S[k] + miu * S[k] * dt + sigma * S[k] * sqrt_dt * 1;
        }
        d_v_S[tid] = S[100];
    }
}


int main() {
    mt19937 gen(time(0));
    normal_distribution<double> dist_norm(0.0, 1.0);
    
    double tstart = omp_get_wtime();
    double  miu = 0.1, sigma = 0.2, dt = 0.01;
    int nstep = 500, nsmall = 0;
    double sqrt_dt = sqrt(dt);
    double E_S = 0.0, EE_S = 0.0, std_S;
    
    /* move it on gpu */
    double v_S[500], *d_v_S;
    int size = nstep * sizeof(double);
    
    cudaMalloc((void **)&d_v_S, size);
    calcu<<<1,1>>>(d_v_S);
    
    /* address copy, v_S itself can represent address, so not &v_S */
    cudaMemcpy(v_S, d_v_S, size, cudaMemcpyDeviceToHost);
    cudaFree(d_v_S);
    
    for (int i = 0; i < nstep; i++) {
        cout << v_S[i] << endl;
    }

/*
    E_S += S[100];
    EE_S += S[100] * S[100];
    /* simulation result */
    E_S /= nstep;
    EE_S /= nstep;
    std_S = sqrt(EE_S - E_S * E_S);
    /* theoretical result */
    double theo_E_S = S[0] * exp(miu * 1);
    double theo_std_S = S[0] * exp(miu * 1) * sqrt(exp(sigma * sigma * 1)-1);
    
    cout << "mean value is " << E_S << " while theoretical one is " << theo_E_S << endl;
    cout << "standard deviation is " << std_S << " while theoretical one is " << theo_std_S << endl;
    double tend = omp_get_wtime();
    cout << "walltime = " << tend - tstart << endl;
*/
}
