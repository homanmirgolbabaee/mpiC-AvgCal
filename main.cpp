
#include <mpi.h> 
#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <bits/stdc++.h> 
#include <cstdlib>
#define n 100
using namespace std;
int a[n]; 

int a2[1000]; 
  
int main(int argc, char* argv[]) 
{ 
    clock_t start, end; 
    start = clock(); 
    srand(time(0));
    for(int i = 0; i<n; i++){
        a[i] = rand();
    }
    int pid, np, 
        elements_per_process, 
        n_elements_recieved; 

  
    MPI_Status status; 
  
    MPI_Init(&argc, &argv); 
 
    MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
    MPI_Comm_size(MPI_COMM_WORLD, &np); 
    if (pid == 0) { 
        int index, i; 
        elements_per_process = n / np; 
  
        if (np > 1) { 

            for (i = 1; i < np - 1; i++) { 
                index = i * elements_per_process; 
  
                MPI_Send(&elements_per_process, 
                         1, MPI_INT, i, 0, 
                         MPI_COMM_WORLD); 
                MPI_Send(&a[index], 
                         elements_per_process, 
                         MPI_INT, i, 0, 
                         MPI_COMM_WORLD); 
            } 
  
            index = i * elements_per_process; 
            int elements_left = n - index; 
  
            MPI_Send(&elements_left, 
                     1, MPI_INT, 
                     i, 0, 
                     MPI_COMM_WORLD); 
            MPI_Send(&a[index], 
                     elements_left, 
                     MPI_INT, i, 0, 
                     MPI_COMM_WORLD); 
        } 
          int sum = 0; 
        for (i = 0; i < elements_per_process; i++) 
            sum += a[i]; 
          int tmp; 
        for (i = 1; i < np; i++) { 
            MPI_Recv(&tmp, 1, MPI_INT, 
                     MPI_ANY_SOURCE, 0, 
                     MPI_COMM_WORLD, 
                     &status); 
            int sender = status.MPI_SOURCE; 
  
            sum += tmp; 
        } 
  
        sum = sum / 100;
        printf("AVG is : %d\n", sum); 
    } 
    else { 
        MPI_Recv(&n_elements_recieved, 
                 1, MPI_INT, 0, 0, 
                 MPI_COMM_WORLD, 
                 &status); 
  
        MPI_Recv(&a2, n_elements_recieved, 
                 MPI_INT, 0, 0, 
                 MPI_COMM_WORLD, 
                 &status); 
  
        int partial_sum = 0; 
        for (int i = 0; i < n_elements_recieved; i++) 
            partial_sum += a2[i]; 
  
        MPI_Send(&partial_sum, 1, MPI_INT, 
                 0, 0, MPI_COMM_WORLD); 
    } 
  
    MPI_Finalize(); 
    end = clock(); 
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC); 
    cout << "Time taken by program is : " << fixed  
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 
    return 0; 
} 

