#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int parent_pid; int task_n; int i; int j;
    int n=atoi(argv[1]); /*n is the amount of children*/
    parent_pid = getpid();
    /*output the information of parent*/
    if (strcmp(argv[2],"T")==0) 
        printf("Parent, pid %d : %d children, test mode\n",parent_pid,n);
    else if (strcmp(argv[2],"G")==0)
    {
        task_n = (argc-35)/2; 
        printf("Parent, pid %d : %d children, %d tasks, generation mode\n",parent_pid,n,task_n);
    }
    else
    {
        task_n = (argc-36)/2;
        printf("Parent, pid %d : %d children, %d tasks, evaluation mode\n",parent_pid,n,task_n);
    }


    /*(1).test mode*/
    if (strcmp(argv[2],"T")==0)
    {
        int n=atoi(argv[1]); int p;
        int pid[n];
        for (p=1; p<=n;p++) 
        {  
            if ((pid[p] = fork()) == 0) 
            {
                /* 1. group information*/
                char group[8]={'A','B','C','D','E','F','G','H'};
                struct Teams     /* establish Teams struct to store infomation of each team */
                {        
                    char name[15];
                    char country[15];
                    int place;
                }team[16],S[8],F[8];
                for(i=0;i<8;i++) /* Initialization of each team*/
                {
                    strcpy(team[2*i].name,argv[2*i+3]); strcpy(team[2*i].country,argv[2*i+19]); team[2*i].place=0;
                    strcpy(team[2*i+1].name,argv[2*i+4]); strcpy(team[2*i+1].country,argv[2*i+20]); team[2*i+1].place=1;
                }
                for(i=0;i<8;i++) /*output group information*/
                {
                    printf("child %d, pid %d: ", p, getpid());
                    printf("Group %c: %s (%s) and %s (%s)\n",group[i],team[2*i].name,team[2*i].country,team[2*i+1].name,team[2*i+1].country); /*易错点：group是字符数组而非字符串数组，应用%c*/
                }
                /*2.rivals information*/
                for(i=0;i<8;i++)  /*team's name*/
                {
                    strcpy(S[i].name,argv[2*i+35]);
                    strcpy(F[i].name,argv[2*i+36]);
                }
                for(i=0;i<8;i++)  /*team's place and country*/
                {
                    for(j=0;j<16;j++)
                    {
                        if (strcmp(S[i].name,team[j].name)==0)
                        {
                            strcpy(S[i].country,team[j].country);
                            S[i].place=team[j].place;
                        }
                        if (strcmp(F[i].name,team[j].name)==0)
                        {
                            strcpy(F[i].country,team[j].country);
                            F[i].place=team[j].place;
                        }
                    }
                }
                for(i=0;i<8;i++)  /*output rivals information*/
                {
                    printf("child %d, pid %d: ", p, getpid());
                    printf("%s (%s) against %s (%s)\n",S[i].name,S[i].country,F[i].name,F[i].country);
                }
                /*3.validity*/
                int valid=1;
                for(i=0;i<8;i++)
                {
                    if ((strcmp(S[i].country,F[i].country)==0)||(S[i].place==0)||(F[i].place==1))  /*contrains: the place and the country*/
                    {
                        valid=0;
                        break;
                    }    
                }
                if (valid==0)    /*output validity*/
                {
                    printf("child %d, pid %d: ", p, getpid());
                    printf("Invalid fixture\n");
                }
                else
                {
                    printf("child %d, pid %d: ", p, getpid());
                    printf("Valid fixture\n"); 
                }
                int cid;cid = wait(NULL);
                if (cid)
                    exit(0);  
            } 
        }
    }


    /*(2).generation mode*/
    if (strcmp(argv[2],"G")==0)
    {      
        int pid[n];
        struct Tasks      /* establish Tasks struct to store infomation of each task */
        {        
            int f_num;
            int seed;
        }task[task_n];
        for(i=0;i<task_n;i++)
        {
            task[i].f_num = atoi(argv[2*i+35]);
            task[i].seed = atoi(argv[2*i+36]);
        }

        struct Teams
        {
            int g_index;      /*the index of group*/    
            char name[15];
            char country[15];
            int place;
        }X[8],Y[8],S[8],F[8]; /*X[i] means the i team in fisrt place pool, Y[i] means the i team in second place pool*/
        for(i=0;i<8;i++)      /* Initialization of X[] and Y[]*/
        {
            X[i].g_index=i; strcpy(X[i].name,argv[2*i+3]); strcpy(X[i].country,argv[2*i+19]); X[i].place=0;
            Y[i].g_index=i; strcpy(Y[i].name,argv[2*i+4]); strcpy(Y[i].country,argv[2*i+20]); Y[i].place=1;
        }

        /*round-robin manner*/
        int p;
        for (p=0; p<n; p++) /*establish n children*/
        {  
            if ((pid[p] = fork()) == 0)
            {
                int count;
                for (count=p; count<task_n;count=count+n)   /*each child execute a task in the step of n*/
                {
                    /*printf("%d \n",count);*/
                    int valid_num=0,f;
                    for (f=0;f<task[count].f_num;f++)       /*the count task need to generate task[count].f_num fixtures*/
                    {
                        /*generate rivals information randomly*/
                        int x[8]={-1,-1,-1,-1,-1,-1,-1,-1}, y[8]={-1,-1,-1,-1,-1,-1,-1,-1};  /*Initialize S[i] and F[i]'s indexes as -1*/
                        int k=0;int seed=task[count].seed;srand(seed);                  
                        while(k<8)
                        {
                            int m_x=rand()%8;
                            int flag_x=0;
                            for(j=0;j<=k;j++)
                            {
                                if(x[j]==m_x)
                                {
                                    flag_x=1;  /*if there is a same number which has been stored in the array,change flag to 1 and break to generate a new m_x randomly*/
                                    break;
                                }
                            }
                            if(flag_x==0)
                            {
                                x[k]=m_x;      /*if there isn't a same number which has been stored in the array,assign m to next element*/
                                k++;
                            }
                        }
                        k=0;
                        while(k<8)
                        {
                            int m_y=rand()%8;
                            int flag_y=0;
                            for(j=0;j<=k;j++)
                            {
                                if(y[j]==m_y)
                                {
                                    flag_y=1;
                                    break;
                                }
                            }
                            if(flag_y==0)
                            {
                                y[k]=m_y;
                                k++;
                            }
                        }

                        /*S[] and F[]*/
                        for(i=0;i<8;i++)
                        {
                            S[i].g_index=Y[y[i]].g_index;strcpy(S[i].country,Y[y[i]].country);
                            F[i].g_index=X[x[i]].g_index;strcpy(F[i].country,X[x[i]].country);
                        }
                        /*the validity of fixtures*/
                        int valid=1;
                        for(i=0;i<8;i++)
                        {
                            if ((S[i].g_index==F[i].g_index) || (strcmp(S[i].country,F[i].country)==0))
                            {
                                valid = 0;
                            }
                        }
                        if (valid)
                        {
                            valid_num ++;
                        }
                    } 
                    /*caculate the probability*/
                    float probability;
                    probability = (valid_num*1.0) / (task[count].f_num*1.0);
                    printf("child %d, pid %d: ", (p+1), getpid());
                    printf("Seed %d with %d valid fixtures out of %d (%.2f%)\n",task[count].seed,valid_num,task[count].f_num,probability*100);
                }
                /*when amount of children > amount of tasks*/
                if (p >= task_n)
                {
                    printf("child %d, pid %d: ", (p+1), getpid());
                    printf("no task assigned\n"); 
                }
                int cid;cid = wait(NULL);
                if (cid)
                    exit(0);  
            }  
        }
    }



    /*(3).evaluation mode*/
    if (strcmp(argv[2],"E")==0)
    {    
        int pid[n];
        struct Tasks
        {        
            int g_index;
            int f_num;
            int seed;
        }task[task_n];
        for(i=0;i<task_n;i++)
        {
            task[i].f_num = atoi(argv[2*i+35]);
            task[i].seed = atoi(argv[2*i+36]);
        }

        struct Teams
        {
            int g_index;   
            char name[15];
            char country[15];
            int place;
        }X[8],Y[8],S[8],F[8],interested_team;
        for(i=0;i<8;i++)
        {
            X[i].g_index=i; strcpy(X[i].name,argv[2*i+3]); strcpy(X[i].country,argv[2*i+19]); X[i].place=0;
            Y[i].g_index=i; strcpy(Y[i].name,argv[2*i+4]); strcpy(Y[i].country,argv[2*i+20]); Y[i].place=1;
        }

        /*Initialization of interested_team*/
        strcpy(interested_team.name,argv[argc-1]);
        for(i=0;i<8;i++)
        {
            if ((strcmp(X[i].name,interested_team.name)==0))
            {
                interested_team.place=0;
                break;
            }
            else if ((strcmp(Y[i].name,interested_team.name)==1))
            {
                interested_team.place=0;
                break;
            }
        }

        /*round-robin manner*/
        int p;
        for (p=0; p<n; p++) 
        {  
            if ((pid[p] = fork()) == 0)
            {
                int count;
                for (count=p; count<task_n;count=count+n) 
                {
                    /*printf("%d \n",count);*/
                    int valid_num=0,f,h,len=0;
                    int potential_team[7]={-1,-1,-1,-1,-1,-1,-1}, potential_team_num[7]={0,0,0,0,0,0,0};
                    for (f=0;f<task[count].f_num;f++)
                    {
                        int x[8]={-1,-1,-1,-1,-1,-1,-1,-1}, y[8]={-1,-1,-1,-1,-1,-1,-1,-1}; /*indeses of S[] and F[] are instatialized as -1*/
                        int k=0;int seed=task[count].seed;srand(seed);                  
                        while(k<8)
                        {
                            int m_x=rand()%8;
                            int flag_x=0;
                            for(j=0;j<=k;j++)
                            {
                                if(x[j]==m_x)
                                {
                                    flag_x=1;
                                    break;
                                }
                            }
                            if(flag_x==0)
                            {
                                x[k]=m_x;
                                k++;
                            }
                        }
                        k=0;
                        while(k<8)
                        {
                            int m_y=rand()%8;
                            int flag_y=0;
                            for(j=0;j<=k;j++)
                            {
                                if(y[j]==m_y)
                                {
                                    flag_y=1;
                                    break;
                                }
                            }
                            if(flag_y==0)
                            {
                                y[k]=m_y;
                                k++;
                            }
                        }

                        /*S[] and F[]*/
                        for(i=0;i<8;i++)
                        {
                            S[i].g_index=Y[y[i]].g_index;strcpy(S[i].country,Y[y[i]].country);strcpy(S[i].name,Y[y[i]].name);
                            F[i].g_index=X[x[i]].g_index;strcpy(F[i].country,X[x[i]].country);strcpy(F[i].name,X[x[i]].name);
                        }
                        /*the validity of fictures*/
                        int valid=1; 
                        int flag; 
                        for(i=0;i<8;i++)
                        {
                            if ((S[i].g_index==F[i].g_index) || (strcmp(S[i].country,F[i].country)==0))
                            {
                                valid = 0;
                            }
                        }
                        if (valid)
                        {
                            valid_num ++;
                            flag=0;
                            for(i=0;i<8;i++)
                            {
                                if (interested_team.place == 1)
                                {
                                    for(j=0;j<8;j++)
                                    {
                                        if (strcmp(F[i].name,X[j].name)==0)
                                            break;
                                    }
                                    for(h=0;h<=len;h++)
                                    {
                                        if(potential_team[h]==j)
                                        {
                                            flag=1;
                                            potential_team_num[h]++;
                                            break;
                                        }
                                    }
                                    if(flag==0)
                                    {
                                        potential_team[len]=j;
                                        potential_team_num[len]++;
                                        len++;
                                    }
                                    break;
                                }
                                else if (interested_team.place == 0)
                                {
                                    for(j=0;j<8;j++)
                                    {
                                        if (strcmp(S[i].name,Y[j].name)==0)
                                        {                                            
                                            break;
                                        }
                                    }
                                    for(h=0;h<=len;h++)
                                    {
                                        if(potential_team[h]==j)
                                        {
                                            flag=1;
                                            potential_team_num[h]++;
                                            break;
                                        }
                                    }
                                    if(flag==0)
                                    {
                                        potential_team[len]=j;
                                        potential_team_num[len]++;
                                        len++;
                                    }
                                    break;
                                }                                
                            }
                        }
                    } 
                    /*caculate the probability*/
                    float probability;
                    probability = (valid_num*1.0) / (task[count].f_num*1.0);
                    printf("child %d, pid %d: ", (p+1), getpid());
                    printf("Seed %d with %d valid fixtures out of %d (%.2f%)\n",task[count].seed,valid_num,task[count].f_num,probability*100);
                                     
                    if (valid_num !=0 )
                    {
                        float probability_against[len+1];
                        for(i=0;i<len;i++)
                        {
                            probability_against[i] = (potential_team_num[i]*1.0)/(valid_num*1.0);
                            if (interested_team.place==0)
                            {
                                printf("child %d, pid %d: ", (p+1), getpid());
                                printf("%s against %s - %d out of %d (%.2f%)\n",Y[potential_team[i]].name,interested_team.name,potential_team_num[i],valid_num,probability_against[i]*100);
                            }
                            else if (interested_team.place==1)
                            {
                                printf("child %d, pid %d: ", (p+1), getpid());
                                printf("%s against %s - %d out of %d (%.2f%)\n",interested_team.name,Y[potential_team[i]].name,potential_team_num[i],valid_num,probability_against[i]*100);
                            }
                        }
                    }
                }
                /*when amount of children > amount of tasks*/
                if (p >= task_n)
                {
                    printf("child %d, pid %d: ", (p+1), getpid());
                    printf("no task assigned\n"); 
                }
                int cid;cid = wait(NULL);
                if (cid)
                    exit(0);  
            } 
        }
    }
}