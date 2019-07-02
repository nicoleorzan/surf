#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void print_mat(double *matrix, int N, int M){
  for (int i=0; i<N; i++){
      for (int j=0; j<M; j++){
	fprintf(stdout, "%f ", matrix[i*M+j]);
      }
      fprintf(stdout, "\n");
  }  
}


double omega_funct(double omega_old, double alpha, double gamma, double theta, double P, double somma){
  return -alpha*omega_old - gamma*theta + P - somma;
}

int main(){

  int nodes = 2;
  int steps = 10000;
  
  double fR = 50;
  double wR = 2*M_PI*fR;
  double h = 0.01;
  double *theta = (double*) malloc(nodes * (steps+1) * sizeof(double));
  double *omega = (double*) malloc(nodes * (steps+1) * sizeof(double)); 
  double *gamma = (double*) malloc(nodes * sizeof(double));
  double *alpha = (double*) malloc(nodes * sizeof(double));
  double *somma = (double*) malloc(nodes * sizeof(double));
  double *K = (double*) malloc(nodes * nodes * sizeof(double));
  double *P = (double*) malloc(nodes * sizeof(double));

 
  char fileout[80];
  FILE *out;

  // variables initialization
  
  for (int i=0; i<nodes; i++){
    gamma[i] = 0.1;
    alpha[i] = 0.1;
    theta[i] = 0;
    omega[i] = 0;
    somma[i] = 0;
    for (int j=0; j<nodes; j++){
      K[i*nodes+j] = 1.5;
    }
  }
  P[0] = 1; //generator
  P[1] = -1.2; //consumer

  
  // integration using runge-kutta method
  
  fprintf(stderr, "Output file name = ");
  scanf("%80s", fileout);
  out = fopen(fileout, "w");
  fprintf(out,"#      time(sec)     omega0(t)        omega1(t)         theta0(t)        theta1(t) \n");
  fprintf(out, "%16.8d%16.8e%16.8e%16.8e%16.8e\n", 0, omega[0], omega[1], theta[0], theta[1]);
  
  double k1, k2, k3, k4;

  for (int t=1; t<=steps; t++){   
      
    for (int i=0; i<nodes; i++){

      somma[i] = 0;
      for (int j=0; j<nodes; j++){
	somma[i] +=  K[i*nodes+j]*sin( theta[i+(t-1)*nodes] - theta[j+(t-1)*nodes] );
      }

      k1 = omega_funct(omega[i+(t-1)*nodes], alpha[i], gamma[i], theta[i+(t-1)*nodes], P[i], somma[i]);
      k2 = omega_funct(omega[i+(t-1)*nodes]+0.5*k1*h, alpha[i], gamma[i], theta[i+(t-1)*nodes], P[i], somma[i]);
      k3 = omega_funct(omega[i+(t-1)*nodes]+0.5*k2*h, alpha[i], gamma[i], theta[i+(t-1)*nodes], P[i], somma[i]);
      k4 = omega_funct(omega[i+(t-1)*nodes]+k3*h, alpha[i], gamma[i], theta[i+(t-1)*nodes], P[i], somma[i]);
      
      omega[i+t*nodes] = omega[i+(t-1)*nodes] + h/6*(k1+2*k2+2*k3+k4);
      theta[i+t*nodes] = theta[i+(t-1)*nodes] + h*(omega[i+(t-1)*nodes]);
    }
    fprintf(out,  "%16.8f%16.8e%16.8e%16.8e%16.8e\n", t*h, omega[0+t*nodes], omega[1+t*nodes], theta[0+t*nodes], theta[1+t*nodes]);

  }
  

  fclose(out);
  free(theta);  free(gamma);  free(K);  free(P);  free(alpha);  free(omega); free(somma);

  return 0;
}
