#ifndef ESCLADE_SIMPLE_H_INCLUDED
#define ESCLADE_SIMPLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
using namespace std ;
typedef struct {

int chemin[53];
 double cost;

} path;

typedef  struct {
 double matrice_distance[52][52];

}problem ;

 typedef struct {
    int id;
    double x, y;

}City;




problem *  get_matrice_distance (City  cities[]);

double distance (City  *A , City * B );

void write_matrice(problem * p);

path * opt_2(problem * p , path * input,int i ,int k );


double cost_trajet (problem * p , path * trajet);
path * generate_trajet(problem* p);

void escalade_simple(problem * p,int iterations);
void escalade_complet(problem * p ,int N);
void write_path(problem * p , path* trajet);
void escalade_recuit_simule(problem * p , double t0 , double alpha , double t_arret);
void genetic_algorithm(problem * p, int num_generation );
vector<path*> selection(vector<path*> to_select_from);
path * cross_over(path* parent1 , path* parent2,problem * p );
vector<path*> new_generation(problem* p , vector<path*> to_crossover);
path* mutation(path* mutated,int pop_size,problem * p);
bool comparer(path* p1 , path* p2);

#endif // ESCLADE_SIMPLE_H_INCLUDED
