
//===========================================================================================================================//
/* Implementing Geneticalgoritem - MetaHeyuristic method for solving 
optimal order QTY for problem type:
-------------------------------------
|t  |  1 | 2  | 3 | 4 | .......   t  |
|Dt | d1 | d2 |d3 |d4 | .......   dt |
-------------------------------------

h - he cost of one unit of suply for one period 
k - fixed order price
t - period number: 1,2,3 ...,t
Dt - demand in t period

Principal flow chart:
_________________________________________________________________________________________
 Genetic Method: population --> Evaluate --> selection(fitness) --> CrossOver --> Mutate
 ________________________________________________________________________________________
the consept of this Algorithm: 
============================
GA()
   initialize population
   calc fitness of population
   
   while (termination criteria is reached- number of generations) do
      selection
      crossover with probability 
      mutation with probability 
      fitness calculation
      find best
*/


//===============================================================================================================================================================|

//===============================================================================================================================================================|
void updateFitness(Chromosom *p[],int serial, float rank, float rankSm){
   // function convert inteagers to floats and update a specific chromosom Fitness var
               p[serial]->fitness = ( rank/rankSm ) ;  // rank 1 = max probability,...., rank 6 = min Probability
}
//===============================================================================================================================================================|
Chromosom *select(Chromosom *p[],int pSize){
// function selects random chromosoms - according accmmulated probability
int i=0;
float sum=0;                                 // sum of probability - used for to select chromosom
float pi = (rand() % 10000) / 10000.0;       // generate a random number between 0 to 1 
  while(i<pSize){
    sum=sum+p[i]->fitness;                   // accmmulated probability
    if (pi>sum) i++;
    else return p[i] ;                        // now i "points" to the chromosom scope select - return this chromosom index
    
  }
  return false;                               
}
//===============================================================================================================================================================|
Chromosom *evaluate(Chromosom *population[],int populationSize,int rankMethod){
  /* function gets a chromosom and update the cromosom-Fitness
   (The fitness  is based on Minimum-total-cost calculation)
   if rankMethod=1 the fitness will update with the ranking method
   usualu used when the objective function returns same or close solutions
  */
     int i=0;
     int j=0;
     float tmp1,tmp2;
    float fitnessSum=0;
    int rankSum=0;
    int Rank[2][populationSize];                          // Ranking: give a relative grade to each TC ==> this will be the probability
    Chromosom *bestCr;                                    // create a pointer to a chromosom
     for (i=0;i<populationSize;i++){                      // copy each chromosom TC to  tmp Rank array[][]:  
          Rank[0][i]=(int)population[i]->TC;              // TC of each Chromosom before sorting
          Rank[1][i]=population[i]->serial;               // Serial number of each chromosom before sorting the array
          rankSum=rankSum+i+1;                            // calc sum of Ranks for use to rank each chromosom, see below:
          fitnessSum=fitnessSum+Rank[0][i];                // calc sum of TC for use to calc fitness of each chromosom, see below:
     }
        //Ranking: 
        //first Sort TC from lowest to highest:
    for (i=0;i<populationSize-1;++i)
        for(j=i+1; j<populationSize; ++j)
            if(Rank[0][i] > Rank[0][j] ){        // The min Val always in left of array
				    tmp1 = Rank[0][i] ;          // swap TC
                    tmp2 = Rank[1][i] ;          // swap serials
				    Rank[0][i]  = Rank[0][j] ; 
                    Rank[1][i]  = Rank[1][j] ;
			        Rank[0][j]  = tmp1; 
                    Rank[1][j]  = tmp2;
            }
       bestCr=population[Rank[1][0]];  // best chromosom is the first in Rank[][] array --> chromosom serial: Rank[1][0] 

      if (rankMethod){    // update each chromosom fitnes:  
          for (i=0;i<populationSize;i++) {          
             Rank[0][i]=populationSize-i;                                     // translate TC to Rank
                                                                              // Rank = population size - i ( the array is sorted from minimum cost to max cost )
            updateFitness( population, Rank[1][i], Rank[0][i] , rankSum ); 
             }
         }
      else {
            for (i=0;i<populationSize;i++)                                    // update each chromosom fitness
            updateFitness( population, Rank[1][i], Rank[0][i] , fitnessSum ); 
          }
/*  Active print in develop mode: see if Rank is sorted correctly
if (rankMethod) printf("ranking activated = TRUE\n");
else printf("ranking activated = FALSE\n");
    printf("Evaluate Process:\n");
    printf("-----------------\n");
    for (i=0;i<(population[0]->cLength);i++){   
     printf("\n");
     printf("\tchromosom Serial: %d ", Rank[1][i]);
     printf( "  Rank/fitness before calc= %d",Rank[0][i];
     }
     printf("\n\tRanksum=[%d]",rankSum);
     printf("\n\tfitnessSum=[%5.2f]",fitnessSum);
 */
      
    //now each elemnt in rank holds the tc grade/rank, return the pointer to best chromosom found.
    return bestCr ;                               
}

//===============================================================================================================================================================|
float calcTC(Chromosom *C, aProblem *P){   
/* function gets a chromosom wich is a posible solution and return the TotalCost
(The fitness  is based on Minimum-total-cost calculation)
*/ 
    float Invent[C->cLength];                           //temp array stores inventory in each period
    float Quantity[C->cLength];                         //temp array stores optimal quantity to buy/produce in each period  
    float Inventory    =0;                              // Inventory 
    int invitationCount=0;                              // couter for number of invitations
    int i=0;
    int counter=1;
    
    for(i=0;i<(C->cLength);i++){                        // Init arrays with zeroes inside a struct
        Invent[i]=0;
        Quantity[i]=0;
    }
     for (i=0; i<(C->cLength);i++){                     // update plan to buy the exact QTY according the demands
      if ( C->chromosom[i] ){
          Quantity[i]=P->D[i];
          counter=1;
      } 
       else {
        Quantity[i-counter]=Quantity[i-counter] + P->D[i];
        counter++;
      }              
    }                                                    // Now Quantity[] stores the final real amounts to buy in each period
 
for (i=0; i<P->t;i++){                                   // calculate inventory in each period
      if (Quantity[i])   {
                    Invent[i]=Quantity[i]-(P->D[i]);
                    invitationCount++;
                  }
      else          Invent[i]=Invent[i-1]-(P->D[i]);

      Inventory=Inventory+Invent[i];
    }
    return (float)( (Inventory * (P->h)) + (invitationCount * (P->k))  );        // return total cost
}
//===============================================================================================================================================================|
Chromosom *createChromosom(Chromosom *CHR,aProblem *P){
 static int SERIAL=0;                                                      //  save array serial as ID for each chromosom

 CHR=malloc((sizeof(Chromosom) + sizeof(int) * (P->t) ) ) ;
 CHR->currentSelection=0;
 CHR->fitness=0;
 CHR->serial=SERIAL++;
 CHR->TC=0;
 CHR->structSize=(sizeof(Chromosom) + sizeof(int) * (P->t) );
 CHR->cLength=(P->t);                                                      // lengh of chromosom

 CHR->chromosom[0]=1;                                                      // must buy in first period according the consept
 for (int i=1;i<(P->t);i++){
     CHR->chromosom[i]=(int)rand()%2;                                     // Enter Random Data for each Chromosom
 }
  
CHR->TC=calcTC(CHR,P);                                                     // calculate Total Cost for each chromosom   
return CHR;
}
//==============================================================================================================================================================|
void createPopulation(Chromosom *chr[],aProblem *p,int pSize){
// asign  struct chromosom for each pointer 
 for (int i=0;i<pSize;i++) chr[i]=createChromosom(chr[i],p);
}
//===============================================================================================================================================================|
void printPopulation(Chromosom *p[],int pSize){
  int i=0;
  int j=0;

  for(i=0;i<pSize;i++){
     printf("\nchromosom serial:%d, TotalCost:%4.1f, fitness:%5.3f, selection:%d---> ",p[i]->serial,p[i]->TC,p[i]->fitness,p[i]->currentSelection);
     for (j=0;j<p[0]->cLength;j++){
        printf("[%d]", p[i]->chromosom[j]);
        }
    }  
}
//===============================================================================================================================================================|
void crossOver(Chromosom *c1,Chromosom *c2,Chromosom *p[], aProblem *pb){
    int i=0;
    int y=0;
    /* function get two parents, recombine them and output two childrens
     the Mask used for the recombination is SINGLE-POINT float*/
    int cL=c1->cLength;
    int Mask[cL];  
    int tmpGen;                                           // temp gen   
    int rndFP= 1 + rand() % (cL);                         // create random Single float point for the mask - rand between 1-chromosom Length
    for (i=0;i<cL;i++) {
         if (i< rndFP) Mask[i]=1;                         // Init the mask used for recombine chromosoms
         else Mask[i]=0;
          }
                                                          //printf("\n float point: rndFP=%d, Mask:",rndFP);
                        printf("Random Mask:");
                        for (y=0;y<cL;y++) printf("[%d]",Mask[y]);

     for (i=0;i<cL;i++){
          if(Mask[i]==0) {                                // for having childe according the mask - swap between gens when Mask[i]==0;
              tmpGen=p[c1->serial]->chromosom[i];
              p[c1->serial]->chromosom[i]=p[c2->serial]->chromosom[i];
              p[c2->serial]->chromosom[i]= tmpGen;
          }
          
     }
        c1->TC=(float)calcTC(c1,pb);
        c2->TC=(float)calcTC(c2,pb);
}
//===============================================================================================================================================================|
int mutate(Chromosom *c, aProblem *p){             // function gets a chromosom as an input and mutate it - change random gen 
    int I=1 + (rand() % (c->cLength)) ;            // rand between 1 to chromosom size. (place 0 - shouldn't be 0 - must buy in period 1)
     if (c->chromosom[I]) c->chromosom[I]=0;       // if bit=1 change bit to 0 
     else c->chromosom[I]=1;                       // if bit=0 change bit to 1
     c->TC=calcTC(c,p);                            // update total cost of the chromosom
return c->serial;                                  // return the index of chromosom was mutate            
}