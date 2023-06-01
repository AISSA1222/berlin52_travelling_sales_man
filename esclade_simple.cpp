
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esclade_simple.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#include <math.h>
using namespace std ;

problem* get_matrice_distance (City cities[]){

problem * p = new problem ;
 double distances[52][52]  ;

for( int i = 0;i<52 ; i++){

 for(int j=0;j<52;j++){

                double dx = cities[i].x - cities[j].x;
                double dy = cities[i].y - cities[j].y;
                distances[i][j] = sqrt(dx*dx + dy*dy);
                p->matrice_distance[i][j]=distances[i][j];
 }


}


 return p;

}

bool comparer(path* p1 , path* p2) {
    return p1->cost < p2->cost;
}

void write_matrice(problem * p ){

for(int i = 0; i < 52; i++) {

      for(int j = 0; j <52 ; j++) {
         printf("%8.2f", p->matrice_distance[i][j]);
      }
      printf("\n");
   }
}


double cost_trajet (problem * p , path * trajet){

double cost = 0.0;

for (int i = 0; i<52;i++){

cost += p->matrice_distance[trajet->chemin[i]][trajet->chemin[i+1]];

}

return cost;
}


//// genere un trajet aleatoire

path * generate_trajet(problem* p){

path * s_i = new path;
    int ta[53];

    for (int j=0 ; j<52;j++){

       ta[j]=j;
     s_i->chemin[j]=ta[j];

    }

int temp;
int j;
 for (int i = 0 ; i <52 ; i ++) {
        j = rand() % (i + 1);
        if(j!=0){
        temp = s_i->chemin[i];
        s_i->chemin[i] = s_i->chemin[j];
        s_i->chemin[j] = temp;}
    }

    s_i->chemin[52] = s_i->chemin[0];
    s_i->cost = cost_trajet(p,s_i);

return s_i;
}

/// algorithm escalade simple

void escalade_simple(problem * p , int iterations ){



path * current_trajet = generate_trajet(p);
path * voisin;

  int i = 0;
  while (i < iterations)
{
  write_path(p,current_trajet);
  voisin = generate_trajet(p);

  if (voisin->cost<current_trajet->cost){
   current_trajet  = voisin;
  }
  //else break;
  ++i;
}


}


/// affichage de trajet ////

void write_path(problem * p , path * trajet){
for(int i = 0;i<53;i++){
printf("%d->",trajet->chemin[i]);
}
printf("cost =%f\n",cost_trajet(p,trajet));
}
////



//// 2-opt algorithm

path * opt_2 (problem * p , path * input, int i , int k ){

path* trajetnew = new path;

for (int j = 1; j <= i ; j++) {
        trajetnew->chemin[j] = input->chemin[j];
    }
    int compteur = 0;
    for (int j = i+1; j <= k; j++) {
        trajetnew->chemin[j] =  input->chemin[k - compteur];
        compteur++;
    }
    for (int j = k + 1; j < 52; j++) {
         trajetnew->chemin[j] = input->chemin[j];
    }



    trajetnew->cost = cost_trajet(p,trajetnew);

    return trajetnew;



}


/// escalade complet


void escalade_complet(problem * p ,int N){
path * current =generate_trajet(p);
path * first ;
first = current;
path *voisin = new path ;
int c =0;

while(c<N){

for (int i = 1 ; i<51;++i){

for (int k=51 ; k > i;--k){

    voisin = opt_2(p,current,i,k);
   write_path(p,current);
   if (voisin->cost < current->cost) {current = voisin;}
}

}

c++;

}

//write_path(p,first);

}

/// la solution recuit simulé ;




void escalade_recuit_simule(problem * p , double t0 , double alpha , double t_arret ){

/// intitaliser les parametres tempurature (t0 : temperature initiale,t :temperature courante , t_arret : temperature
// d'arret ,alpha :taux de refroidessement )

double t ;
t = t0 ;
int essais = (53/2)*(53/2);
path * s_j;
/// generation d'un soultion initiale

path * s_i = generate_trajet(p);
////

while (t >t_arret){

for (int i = 0 ; i < essais  ; i++){

         write_path(p,s_i);

     s_j = generate_trajet(p);

     if (s_j->cost <  s_i->cost ) { s_i = s_j;}

     else {

       double dE = s_j->cost - s_i->cost;

    double a = (double) rand() / RAND_MAX;

     if ( a <= exp((-1*dE)/t) ) s_i = s_j; //probabilité à accepter

     }
}
t = t * alpha ;

}

}




vector<path*> selection(vector<path*> to_select_from){

vector<path*> pop ;
pop= to_select_from;

int indice = pop.size()/2;

for (int i =0 ; i<indice ; i++){

      pop.pop_back();

}

return pop ;


}

path * cross_over(path* parent1 , path* parent2,problem * p ){

        int i = 2;
    int j = 50;
    int random_num = rand() % (j - i + 1) + i;
    path* child = new path;
    vector<int> visited(52, 0); 
    for (int k = 0; k <= random_num; k++) {
        child->chemin[k] = parent1->chemin[k];
        visited[parent1->chemin[k]] = 1; 
    }
    int child_index = random_num + 1;
    for (int k = 0; k < 52; k++) {
        int city = parent2->chemin[k];
        if (!visited[city]) { 
            child->chemin[child_index] = city;
            visited[city] = 1; 
            child_index++;
            if (child_index >= 52) break; 
        }
    }

    return child;
}



path* mutation(path* mutated,int pop_size,problem * p){

   path * n = mutated;
   double  prob_mutation =  (double)rand() / RAND_MAX;
    //printf("\n %lf\n",prob_mutation);
   if (prob_mutation < 1.75/(100*sqrt(52))) return opt_2(p,n,2,50);
   else return n;

}


vector<path*> new_generation(problem* p , vector<path*> to_crossover){

        vector<path*> gen = to_crossover;
        int size_p = gen.size();
     for(int j=0 ; j< size_p-1;j++){
        path * new_indv ;
        double pc= (double) rand() / RAND_MAX;
        if (pc <= 0.6 ){ new_indv = cross_over(gen[j],gen[j+1],p);
                        gen.push_back(mutation(new_indv,100,p));
        }

        else{
                   if(cost_trajet(p,gen[j])<cost_trajet(p,gen[j+1])) gen.push_back(mutation(gen[j],100,p));
                  else  gen.push_back(mutation(gen[j+1],100,p));

        }

     }

     return gen;
}



void genetic_algorithm(problem * p, int num_generation ){
 vector<path*> population ;

 for(int j = 0;j< 100;j++){

 path* pa = generate_trajet(p);
 population.push_back(pa);

 }
 vector<path*> current ;
 vector<path*> selected ;// la debub de l'algorithm
current = population;
for (int i = 0 ; i<num_generation; i++){

sort(current.begin(),current.end(),comparer);
write_path(p,current.front());
selected= selection(current);
vector<path*> new_gen = new_generation(p,selected);
current = new_gen;

}

}

