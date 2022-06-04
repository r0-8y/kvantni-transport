#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <complex.h>

// funkcija za izradu kompleksne matrice od matrice realnih brojeva
double *make_complex(double *matrix, int rows, int columns)
{

  double *C_complex = (double *)malloc(2 * rows * columns * sizeof(double));

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      // svaki drugi element nove matrice postavlja na vrijednost stare matrice,
      // imaginarni dio ostaje nula
      *(C_complex + i * columns * 2 + j * 2) = *(matrix + i * columns + j);
    }
  }

  return C_complex;
}

// funkcija za oduzimanje kompleksnih matrica
double *substract_complex(double *first, double *second, int rows, int columns)
{
  double *result = (double *)malloc(rows * columns * sizeof(double));

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      // ide element po element u svakoj od dvije matrice, oduzima ih te sprema u rezultat
      // posebno realni, posebno imaginarni dio
      *(result + i * columns + j) = *(first + i * columns + j) - *(second + i * columns + j);
    }
  }

  return result;
}

// funkcija za printanje matrice
void print_matrix(double *matrix, int rows, int columns, int compl )
{
  printf("\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      if (compl &&j % 2 != 0)
      {
        printf("%.0lfi ", *(matrix + i * columns + j));
      }
      else
      {
        printf("%.0lf ", *(matrix + i * columns + j));
      }
    }
    printf("\n");
  }
  printf("\n");
}

int main()
{
  double *A, *B, *C, *D, *E, *F;
  double alpha, beta;
  int k, m, n;

  alpha = 1.0;
  beta = 0.0;

  m = n = k = 2;

  // alokacija prostora za realne matrice
  A = (double *)malloc(m * n * sizeof(double));
  B = (double *)malloc(m * n * sizeof(double));
  C = (double *)malloc(m * n * sizeof(double));

  // inicijalizacija matrica
  for (int i = 0; i < m * n; i++)
  {
    A[i] = i;
    B[i] = i;
  }

  printf("Matrica A:\n");
  print_matrix(A, m, n, 0);
  printf("Matrica B:\n");
  print_matrix(B, m, n, 0);

  // racunanje umnoska matrica A i B
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);

  printf("Rezultat mnozenja, matrica C:\n");
  print_matrix(C, m, n, 0);

  double *C_complex = make_complex(C, m, n);

  printf("Kompleksna matrica C:\n");
  print_matrix(C_complex, m, n * 2, 1);

  // alokacija prostora za realne matrice
  D = (double *)malloc(2 * 2 * sizeof(double));
  E = (double *)malloc(1 * 4 * sizeof(double));
  F = (double *)malloc(2 * 4 * sizeof(double));

  // inicijalizacija matrica
  for (int i = 0; i < 4; i++)
  {
    D[i] = E[i] = i;
  }

  printf("Matrica D:\n");
  print_matrix(D, m, n, 1);
  printf("Matrica E:\n");
  print_matrix(E, 1, 4, 1);

  // racunanje umnoska kompleksnih matrica D i E
  cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 2, 2, 1, &alpha, D, 2, E, 1, &beta, F, 2);

  printf("Rezultat mnozenja, kompleksna matrica F:\n");
  print_matrix(F, 2, 4, 1);

  // oduzimanje kompleksnih matrica F i C
  double *G = substract_complex(F, C_complex, 2, 4);

  printf("Krajnji rezultat, kompleksna matrica result:\n");
  print_matrix(G, 2, 4, 1);

  // oslobadanje memorije
  free(A);
  free(B);
  free(C);
  free(C_complex);
  free(D);
  free(E);
  free(F);
  free(G);

  return 0;
}
