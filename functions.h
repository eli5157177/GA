

//=======================================================================================================================================================|
aProblem *createProblem(aProblem *s,int t,float h,float k, int De[]) { 
    // Allocating memory according to user provided array     
    s = malloc( sizeof(*s) + sizeof(int) * t); 
  
    s->t = t; 
    s->h=h;
    s->k=k; 
    for (int i=0;i<t;i++) s->D[i]=De[i];
  
    // Assigning size according to number of demands = num of periods
    // A copy of user provided array Demands[]. 
    s->structSize = (sizeof(*s) + sizeof(int) * t); 
  
    return s; 
}
//=======================================================================================================================================================|
 void printProblem(aProblem *pp){
   int i=0;
   printf("Number of periods=%d\t holdCost =%4.1f\t\tinvititionCost=%4.1f ",pp->t,pp->h,pp->k);
   printf("\nDemands:\n");
   for (i=0;i<pp->t;i++) printf("period %d: [%d] ",i+1,pp->D[i]);
   printf("\n\n\n");
 }
//=======================================================================================================================================================|
FILE *myFileCreate(char *filePath){   
    /* function recive file name and open it for write mode
If File dont exist the program will create it.
In any case all previous data will delete!!
*/  
  FILE *fptr;
    
    fptr = fopen(filePath,"w");
   
    if (fptr==NULL){
        printf("function: myFileCreate() in  header: functions.h - Error opening file !");
        return false;
    } 
    
return (fptr);
}
//=======================================================================================================================================================|
void save2File(char *file, Chromosom *cr, float ww[], float tc, aProblem *pr ){

  int i=1;
  int counter=1;
  float tmp[pr->t];
    for (int a=0; a<pr->t; a++) tmp[a]=0; // init array eith zeroes

   FILE *fp =  myFileCreate("soulution.csv"); 
  
   if (fp){
// ----------printf GA soulution to file: sulution.csv:---------------
    fprintf(fp,"\nOptimal Solution GA:\n");
// update plan to buy the exact QTY according the demands:   
       for (i=0; i<(cr->cLength);i++){  
          if ( cr->chromosom[i] ){

          tmp[i]=pr->D[i]; 
          counter=1;
      } 
       else {
          tmp[i-counter]=tmp[i-counter] + pr->D[i];
          counter++;
      }              
    }   
                                                                // ***(Now tmp[] stores the final real amounts to buy in each period)
     fprintf(fp,"t,");
     for (i=0;i<cr->cLength;i++) fprintf(fp,"%d,",i+1);         // print periods
     fprintf(fp,"\n");  
     fprintf(fp,"Q:,");                                         // new line
     for (i=0;i<cr->cLength;i++) fprintf(fp,"%4.1f,",tmp[i]);   // print solution- quantity to buy in the i period
      fprintf(fp,"\n");
      fprintf(fp,"TotalCost:,");
      fprintf(fp,"%f",cr->TC);
//--------------print WW solution to file: solution.csv :
     fprintf(fp,"\nOptimal Solution WWA:\n");

     fprintf(fp,"t,");
     for (i=0;i<cr->cLength;i++) fprintf(fp,"%d,",i+1);          // print periods
     fprintf(fp,"\n");  
     fprintf(fp,"Q:,");                                          // new line
     for (i=0;i<cr->cLength;i++) fprintf(fp,"%4.1f,",ww[i]);     // print solution- quantity to buy in the i period
      fprintf(fp,"\n");
      fprintf(fp,"TotalCost:,");
      fprintf(fp,"%f",tc);
} 
else return;
printf("\nSoulution File: soulution.csv Created Successfuly!");
fclose(fp);
}
//=======================================================================================================================================================|
// read data from file:

aProblem *readCsv(char *filename, aProblem *p){

	FILE *fp;
  char s[100];
  int i,j;
  int t;
  float h;
  int k;
  int tmp;
  int counter=0;
  int di= 8;  // file start index of demands
  int demands[60];
  fp = fopen(filename, "r");
  
  // Return if could not open file 
    if (fp == NULL) 
      return NULL; 
     // read the problem data from file: t,h,k,Demdnds[]
     while ( !feof(fp) )         
        if( fgets (s, 60, fp)!=NULL ){
          if (s[0]=='t') sscanf(&s[2], "%d", &t);
          if (s[0]=='h') sscanf(&s[2], "%f", &h);
          if (s[0]=='k') sscanf(&s[2], "%d", &k);
          if (s[0]=='D') 
             for (i=0;i<t;i++ ) {

               sscanf(&s[di], "%d", &demands[i]);
               tmp=demands[i];
               if (tmp==0)tmp=1;               // in Case demand equals to 0, still take s place of a digit!
               while(tmp){tmp/=10;++counter;} // count number of digits= num of chars for the next demand
               di=di+counter+1;
               counter=0;
             }
         //puts(s);                          // actived to see hoe the file is readen
        }
        
    fclose(fp); 
    //printf("%d, %f, %d",t,h,k); // show chars from the file
    //for (j=0;j<t;j++) printf("\n%d,",demands[j]);
    return ( createProblem(p,t,h,k,demands) );  // save the data from the file in program data Record type
}

//=======================================================================================================================================================|

int callUser(){
    // function calls user iterface 
    int choice;
   
        printf ("\n\n-------------------------------------------------------------------------------------------\n");
        printf ("      1          -->    Run Default Data (continue..) \n");
        printf ("      2          -->    Read New Problem Data from file: \\input.csv \n");    
        printf ("      Other key  -->    EXIT           \n");
        printf ("------------------------------------------------------------------------------------------\n");

        printf ("Enter your choice :");
        scanf    ("%d", &choice);
        
  return choice;
}

