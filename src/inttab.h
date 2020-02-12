#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_LOWER_BOUND 1
#define DEFAULT_HIGHER_BOUND 50
#define DEFAULT_PRESCISION 5
#define DEFAULT_COLUMN_WIDTH 20

#define DEFAULT_INCLUDED 0xFC

// Bits for including a value
#define INCLUDE_FP 0b10000000
#define INCLUDE_PF 0b01000000
#define INCLUDE_FA 0b00100000
#define INCLUDE_AF 0b00010000
#define INCLUDE_PA 0b00001000
#define INCLUDE_AP 0b00000100
#define INCLUDE_PG 0b00000010
#define INCLUDE_AG 0b00000001

double f_p (double ref_int, uint N) { return pow(1.0+ref_int, N); }
double p_f (double ref_int, uint N) { return 1.0/pow(1.0+ref_int, N); }
double f_a (double ref_int, uint N) { return (pow(1.0+ref_int, N) - 1.0)/ref_int; }
double a_f (double ref_int, uint N) { return ref_int/(pow(1.0+ref_int, N) - 1.0); }
double p_a (double ref_int, uint N) { return (pow(1.0+ref_int, N) - 1.0)/ref_int/pow(1.0+ref_int, N); }
double a_p (double ref_int, uint N) { return ref_int*pow(1.0+ref_int, N)/(pow(1.0+ref_int, N) - 1.0); }
double p_g (double ref_int, uint N) { return (pow(1.0+ref_int,N)-ref_int*N-1.0)/(ref_int*ref_int*pow(1.0+ref_int, N)); }
double a_g (double ref_int, uint N) { return (pow(1.0+ref_int,N)-ref_int*N-1.0)/(ref_int*(pow(1.0+ref_int, N) - 1.0)); }
