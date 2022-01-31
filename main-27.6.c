#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>                           // for memcpy() function
#include <time.h>                             // for random functions

#define true 1
#define false 0

typedef struct problemData{                   // data structure for holding a problem data 
int   t;                                      // plan scope
float h;                                      // store cost - one unit for one period
float k;
int   structSize;                             // for allocating memory                           	                   // order fixed price
int   D[];                                    // demands for each period 
}aProblem;

typedef struct Chromosom{                    // define a basic structure for chromosom
     int serial;                             // serial number of chromosom
     int cLength;                            // length of chromosom array = t = num of periods
     int currentSelection;                   // -1,0,1: select for: parent=-1,no change=0,
     float fitness;                          // In this program fitness calcs according to rank method
     float TC;                               // total cost of the soulution
     int structSize;                         // for allocating memory
     int chromosom[];  /* Chromosom. stores binarry data: buy in period i? [true/false] 
    (Flexible Array Member(FAM) variable length array must be last member of structure) */
}Chromosom;

#include "functions.h"                        // contains utility functions like file reading
#include "GA.h"                               // contains  Genetic Algorithm functions
#include "wwa.h"                              // contains WW algorithm functions

/* Genetic Algorithm 
also known as metahHyuristic method.
this program solves
 an potimal cost for MRP plan problem with genetic algoritem (metaHeyuristic).
 Then the program compers the solution with an optimal solution (Wagner whithin algorithm method).
 
 Genetic Algorithms have the ability to deliver a 
 “good-enough” solution “fast-enough”. 
 This makes genetic algorithms attractive for use in solving optimization problems
 ---------------------------------------------------------------------------------
 The file created by Eli Buhbut ID 032267411, june 2020
 A student in Operation Management 2 - course OpenUniversity  
 
//===============================================================================================================================================================|
                         --Function Prototypes--
//===============================================================================================================================================================*/
void updateFitness(Chromosom *p[],int serial, float rank, float rankSm);
void crossOver(Chromosom *c1,Chromosom *c2,Chromosom *p[], aProblem *pb);
Chromosom *select(Chromosom *p[],int pSize);
Chromosom *evaluate(Chromosom *population[],int populationSize,int rankMethod);
float calcTC(Chromosom *C, aProblem *P);
Chromosom *createChromosom(Chromosom *CHR,aProblem *P);
void createPopulation(Chromosom *chr[],aProblem *p,int pSize);
void printPopulation(Chromosom *p[],int pSize);                                                                          
FILE *myFileCreate(char *filePath);
void save2File(char *file, Chromosom *c, float ww[], float tc, aProblem *pr);
//aProblem *read_csv(char *filename);
void callWW(aP pb); 

/*===========================================================================
 |                     Global Vars: change program parameters from here                                     |
=============================================================================*/
int    numOfGenerations =5;                           // change here the number of generations to process
int    rankMethod = 1;                                 // define rankMethod or fitness
int    populationSize;                                 // holds the population size
float  percentOfCross=0.35;                            // define % of Chromosoms to cross over each generation

//==============================================================================================================================================================|
//==============================================================================================================================================================|
                         Main Program                                      
//==============================================================================================================================================================|

void main(){

  

 // readCsv("input.csv");
  






int i=0;
int j=0;
int gn=0;
int pr=0;

//----------------------------------------------------------------------------------------------------------------------
int   t = 6;                                         // plan scope - number of periods: num of (weeks,month,years ect..)
float h =0.3;                                        // holding cost for one unit in one period
float k =100;                           	           // invitation fixed price    
int Demands[]={250,200,180,420,350,250};             // demands for each period
//-----------------------------------------------------------------------------------------------------------------------
//If the fillowing line enabled:
// read the data of the problem from file and fill vars:


populationSize=t;                                                      // population: number of online active soulutions. by default = t
int numOfCross = (percentOfCross*populationSize);                      // define 35% of Chromosoms to be crossed. example: population size = 6 --> recombine 2 times (each time pick 2 parents)
 Chromosom  *bestChromosom;
 Chromosom  *tmpChr;                                                    // a tmp Pointer
 Chromosom  *parent1;                                                   // a Pointer to parent chromosom 1
 Chromosom  *parent2;                                                   // a Pointer to parent chromosom 2
                                             // a Pointer to the best Chromosom = best solution found
/* Intializes random number generator - based on local time
    This is a pseudo-random engine. Replace this randomizing engine for proffessional use */
   srand(time(NULL)); 


aProblem *pb =createProblem(pb,t,h,k,Demands);                         // creatig a problem data record
printf("\nGeneticAlgoritem implemention:\nlooking a sulution for problem:\n----------------------------------\n");
printProblem(pb);

//Now Create chromosom population for genetic Algorithm use
Chromosom *population[populationSize];                                           //an array of pointers to each chromosom - this will store the population
createPopulation(population,pb,populationSize);                                  // create the population 
bestChromosom = evaluate(population,populationSize,rankMethod);                  // Initializing progress - calc fitness, function also returns the best current chromosom = the best soulution so far
Chromosom *solution=malloc(population[0]->structSize );                          // chromosom will contain the best solution found by GA
memcpy(solution,bestChromosom,population[0]->structSize);                        // set the firt best slution of the initial population

    printf("\n\nInitial Population :(current best Total cost = %5.1f)\n",solution->TC);
    printf("--------------------------------------------------------\n");
    printPopulation(population,populationSize);
                                                                                 //printf("\nbest Total cost: TC=%5.1f",) - enable print total cost command in develop mode
for (gn=1; gn<= numOfGenerations; gn++){                                         // main progress loop - repeat number of generations as pre defined by user
    
    // select parents and cross over them - repeat  [numOfCross] times
     printf("\n\nGeneration --> %d\n",gn);
     printf("-----------------\n");
 
     for (pr=0;pr<numOfCross;pr++){ 
       
        parent1 = select(population,populationSize);				                                                             // select parent 1 - select in accordance with probability
        parent2 = select(population,populationSize);
             printf("\nParents serial: %d + %d Were selected for crossOver ",parent1->serial, parent2->serial );
        crossOver(parent1,parent2,population,pb);                                                     					      // crossOver the selected parents - create new childerens
        printf("\n\tchromosom serial: %d was mutate",mutate( select(population,populationSize),pb ) );			      // random select chromosom to mutation
      }
        printf("\n");
        
      tmpChr = evaluate(population,populationSize,rankMethod); // Call the evaluation process                                                     // save the best solution so far
             if(tmpChr->TC < solution->TC ) memcpy(solution,tmpChr,population[0]->structSize);

 printPopulation(population,populationSize);
 printf("\n\n         best Chromosom: Total Cost =%5.1f ",solution->TC);
   for (i=0;i<bestChromosom->cLength;i++) printf("[%d]", solution->chromosom[i]);                   			        // show the solution presented by the best chromossom so far

  }

  // printing solution in to file: \\solution.csv
  // this file will store in the current active directory
  


  // end of Genetic Algoritem
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                         
   Now Comparing GA solution to the optimal solution : WWA           
                                                                             
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
  
  callWW(pb);

   free(population);
   free(solution);
   free(pb);
}
//=============================================================================================================================================================================//
void callWW(aProblem pb){
int i=0;

printf("\nCalculate optimal solution according Wagner within algoritem:\n");
printf("----------------------------------------------------");
float Qt[pb->t];                                                // solution Array 
for (i=0;i<pb->t;i++) Qt[i]=0;                                  // init array
float wwaTC= WWA(pb->D,pb->t,pb->h,pb->k,Qt);                   // Call WW algoritem - calc the optimal soulution            
for (i=0;i<t;i++)printf("\nperiod %d buy:%4.0f",i+1,Qt[i]);
printf("\n");
printf ("WWA optimal Total Cost TC=%4.0f$",wwaTC);   


  // printing solution in to file: \\solution.csv
  // this file will store in the current active directory
save2File("solution.csv",solution,Qt,wwaTC,pb );
     
   
   //system("pause"); // halt if excutable is running in a microsoft windows OS
}