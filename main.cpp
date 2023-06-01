#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include "esclade_simple.h"
#include<math.h>

using namespace std ;

int main(){

srand(time(NULL));
    FILE *fp;
    char line[100];
    int num_cities = 0;
    City cities[100];

    fp = fopen("berlin52", "r");
    if (fp == NULL) {
        printf("Error: failed to open file\n");
        exit(1);
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d %lf %lf", &cities[num_cities].id, &cities[num_cities].x, &cities[num_cities].y) == 3) {
            num_cities++;
        }
    }


    fclose(fp);


    problem * p ;

    p = get_matrice_distance(cities);

    printf("------------------------ PVC les solutions approchées ------------------------\n");
    while(1){
    
     printf("-------------------choisir un algorithme -------------------\n");
     printf(" ---------------- taper 1 pour escalade simple -------------\n");
     printf(" ---------------- taper 2 pour escalade complet -------------\n");
     printf(" ---------------- taper 3 pour recuit simulé -------------\n");
     printf(" ---------------- taper 4 pour algorithme genetique -------------\n");
     
     int option;
     scanf("%d",&option);
     
     if(option == 1 ){
         int N;
         printf("taper le nombre d'iterations \n");
         scanf("%d",&N);
         escalade_simple(p,N);
     
     } 
     
      else if(option == 2 ){
         int N;
         printf("taper le nombre d'iterations \n");
         scanf("%d",&N);
         escalade_complet(p,N);
     
     } 

      else if(option == 3 ){
         double t0 ,tf , alpha ;
         printf("temperature initiale t0 =  \n");
         scanf("%lf",&t0);
         printf("temperature initiale tf =  \n");
         scanf("%lf",&tf);
          printf("alpha taux de refroidessement  =  \n");
         scanf("%lf",&alpha);
         escalade_recuit_simule(p,t0,alpha,tf);
       
     } 
 
     else if(option == 4 ){
         int N_g;
         printf("donner le nombre des generations =  \n");
         scanf("%d",&N_g);
         
         genetic_algorithm(p , N_g);
 
     } 
     
     else break; 
}


    free(p);
return 0 ;
}
