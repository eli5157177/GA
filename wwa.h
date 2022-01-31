


//===========================================================================================================================//
/* Implementing Wagner - Whithin algoritem WWA
The algorithm find optimal order QTY for problem type:
-------------------------------------
|t  |  1 | 2  | 3 | 4 | .......   t  |
|Dt | d1 | d2 |d3 |d4 | .......   dt |
-------------------------------------

h - he cost of one unit of suply in period t
k - fixed order price
t - period number
Dt - demand in t period

*/
//===========================================================================================================================//
float cost(int D[],float h,float k,int ii,int  jj){
 /* function scans the cost options for period t (starting from last optimal index: bestI[]) 
 and returns the optimal index)   */

    if (jj>=ii) {
      printf("Errror function: cost: start index i=%d >= endIndex j=%d \n function will terminate :return 0", ii,jj);
      return 0;
    }
int m=1;          // m periods multiplyer (  m=1 means 1 period storage cost, m=2 means 2 periods storage cost .....)
int i=ii;        // do not change the variables coming to function
int j=jj+1;      // D[] array starts with 0, thus we want to correct index (period 1 = index 0 and so-on....)
float sum=k;     // min cost for any period cannot be less than k - Order fixed price

 while (j<i){   // Calculate store costs 
     sum=sum+D[j]*(m)*h;
  j++;
  m++;
  }
return sum;
}
//===========================================================================================================================//

float CalcSolution(int D[],int t,float h,float k,float Qt[], int bestI[]){              // function returns optimal soulution cost
    // fill Qt[],It[],
    float It[t];                                       			//temp array stores inventory in each period
    float Inventory=0;                       				        // Inventory cost
    int invitationCount=0;	
    int i=0;
    int p=1;

    for (i=0; i<t;i++){
      It[i]=0;                                             // init array with zeroes 
      Qt[bestI[i+1]]=1;                            	       // set true if should buy in this period

    }
    
     for (i=0; i<t;i++){                      				       // update plan to buy according the demands
      if ( Qt[i]==true ){

          Qt[i]=D[i];
          p=1;
      } 
       else {
        Qt[i-p]=Qt[i-p] + D[i];
        p++;
      }              
    }  // Now Qt[] stores the final soulution
 

for (i=0; i<t;i++){        			                                 // calculate inventory in each period
 
    
      if (Qt[i]>1)   {            // when converting from int to float may be small number in cell

                    It[i]=(Qt[i]-D[i]);
                    invitationCount++;
                  }
      else          It[i]=It[i-1]-D[i];

      Inventory=Inventory+It[i];
    }

    return ((Inventory*h) + (invitationCount*k));

}

//===========================================================================================================================//
float anlayze(int demands[],int t,float h,float k,float Qt[]){

int bestI[t+1]; // array stores the optimal INDEX for the minimum cost in every iterration
float F[t+1]; // array stores the optimal cost in every iterration: f[0]=0 always, F[1], F[2], ...,F[t]
F[0]=0;       // init F[0]
bestI[0]=0;
/* F[I] - array stores the optimal soulution in every iterration
    F0=0    
    F1=k
    F2= min{ F0 + k + D[t]*C*h, F1+k } 
    F3 =...
*/
int i=1;
int j=0;                                                  // start index - ww algoritem - starts from the last optimal cost
float si;
float minSi=FLT_MAX;                                      // FLT_MAX ==> a big enough number
    for(i=1;i<=t;i++){                                    // first start from F1  (by default F0=0)
        j=bestI[i-1];                                     // start index - ww algoritem - starts from the last optimal cost index
        //if (demands[i-1]==0){F[i]=F[i-1];i++;}             // if demands=0 in a specific period, use previous period best cost.
        while(j<i){
            si=F[j] + cost(demands,h,k,i,j);              
            if (si<= minSi){
                minSi=si;
                 F[i]=si;                                // now F store optimal costs of each period
                 bestI[i]=j;   	                         // now bestI store optimal index of each period
                 
              }
        j++;
        } 
    j=0;
    minSi= FLT_MAX;                                     //FLT_MAX is A const containing the maximum float number.
    }  // end for
                                                        // in the end of this function ----> F[] contains the optimal costs for each 
				// where last F[t]=optimal total Cost .

return ( CalcSolution(demands,t,h,k,Qt,bestI) );
}

//===========================================================================================================================//
float WWA(int demands[],int t,float h, float k,float Qt[]){  
    /*  
    t=number of Periods
    h=one unit unit storage cost for one period
    k=invitation fixed price
    Qt[] = solution array */

float TotalCost;
TotalCost=anlayze(demands,t,h,k,Qt);

return TotalCost;
}