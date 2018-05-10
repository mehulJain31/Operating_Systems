// mehul jain 1001229017
// lab2 part b
// sorthed using latitude
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#define BUFFER_SIZE 1024*1024


struct Line_node // define the node for each line
{
    float latitude;
    char line[300];
    struct Line_node *next, *prev;
};
typedef struct Line_node node_t;
node_t *head = NULL;
node_t *globalHead = NULL;


struct proc_node 
{
    node_t *list;
    
    struct proc_node *next;
};
typedef struct proc_node node_p;

node_t *getNode(int index) // get the node
{
    int i;
    node_t *temp = head, *previous_temp;

    for(i = 0; i < index; i++)
    {
        if(temp == NULL)
        {
            break;
        }
        previous_temp = temp;
        temp = temp->next;
    }
    if(temp != NULL)
    {
        return temp;
    }
    else
    {
        return previous_temp;
    }
}

void merge(int start_index, int end_index) // merge function
{
    int i = start_index, j;
   
    float latitude;
   
    char line[300];
   
    node_t *prev, *temp, *prevtemp, *next, *localHead, *end, *new_node;

    end = getNode(end_index);
   
    next = getNode(start_index);

    while(next != NULL && next != end->next)
    {
        if(globalHead == NULL)
        {
            globalHead = malloc(sizeof(struct Line_node));
            
            strcpy(globalHead->line, next->line);
            
            globalHead->latitude = next->latitude;
            
            globalHead->next = NULL;

        }
        else if(globalHead->latitude >= next->latitude)
        {
            new_node = malloc(sizeof(struct Line_node));
            
            strcpy(new_node->line, next->line);
            
            new_node->latitude = next->latitude;
            
            new_node->next = globalHead;
            
            globalHead = new_node;
        }
        else
        {
            prevtemp = globalHead;
            
            temp = globalHead->next;

            if(temp == NULL)
            {
                new_node = malloc(sizeof(struct Line_node));
                
                strcpy(new_node->line, next->line);
                
                new_node->latitude = next->latitude;
                
                new_node->next = NULL;
                
                globalHead->next = new_node;
            }
            else
            {
                while(temp != NULL)
                {
                    if(temp->next == NULL)
                    {
                        if(temp->latitude >= next->latitude)
                        {
                            new_node = malloc(sizeof(struct Line_node));
                            
                            strcpy(new_node->line, next->line);
                            
                            new_node->latitude = next->latitude;
                            
                            new_node->next = temp;
                            
                            prevtemp->next = new_node;
                        }
                        else
                        {
                            new_node = malloc(sizeof(struct Line_node));
                           
                            strcpy(new_node->line, next->line);
                           
                            new_node->latitude = next->latitude;
                           
                            new_node->next = NULL;
                           
                            temp->next = new_node;
                        }

                        break;
                    }
                    else if(temp->latitude >= next->latitude)
                    {
                        new_node = malloc(sizeof(struct Line_node));
                        
                        strcpy(new_node->line, next->line);
                        
                        new_node->latitude = next->latitude;
                        
                        new_node->next = temp;
                        
                        prevtemp->next = new_node;

                        break;
                    }

                    prevtemp = temp;
                    temp = temp->next;

                }
            }
        }

        prev = next;
        next = next->next;

    }
}

void sort(void *arg) // sort function
{
    int start_index = ((int*)arg)[0];
    
    int end_index = ((int*)arg)[1];

    int i = start_index, j;
    
    float latitude;
    
    char line[300];
    
    node_t *prev, *temp, *prevtemp, *next, *localHead, *end;

    end = getNode(end_index);
    
    prev = getNode(start_index);
    
    localHead = prev;
    
    next = prev->next;

    while(next != NULL && next != end)
    {
        temp = localHead;

        while(temp != next && temp != NULL)
        {

            if(temp->latitude >= next->latitude)
            {
                strcpy(line, next->line);
                
                latitude = next->latitude;
                
                prevtemp = next;
                
                while(prevtemp != temp && prevtemp != NULL)
                {
                    strcpy(prevtemp->line, prevtemp->prev->line);
                    
                    prevtemp->latitude = prevtemp->prev->latitude;
                    
                    prevtemp = prevtemp->prev;
                }
                strcpy(temp->line, line);
                temp->latitude = latitude;
                break;
            }

            temp = temp->next;

        }


        if(next != NULL)
        {
            next = next->next;
        }
    }

}



int main(int argc, char *argv[]) // main function
{
    clock_t strt,end;
    
    double programTime;
    
    strt = clock(); //start of time
    
    node_t *next, *temp;
    
    char buffer[500], num[2];
    
    FILE *fp;
    
    char *token;
    
    int i, j, count = 1;

    int fd = open("/Documents/program_2", O_RDWR, 0);

    if ((fp=fopen("earthquakes.csv", "r"))==NULL)
    {
        printf ("Error: File cannot be opened");
        
        return 1;
    }

    fgets(buffer, 500, fp);
    fgets(buffer, 500, fp);
    
    token = strtok(buffer, ",");
    token = strtok (NULL, ",");

    head = malloc(sizeof(struct Line_node));
    strcpy(head->line, buffer);
    head->latitude = atof(token);
    head->next = NULL;
    head->prev = NULL;

    next = head;

    while ((fgets(buffer, 500, fp))) // kernel level threading system
    {
        count++;
       
        token = strtok(buffer, ",");
       
        token = strtok (NULL, ",");
       
        // Create new node
        temp = malloc(sizeof(struct Line_node));
       
        temp->latitude = atof(token);
       
        temp->next = NULL;
       
        temp->prev = next;
       
        strcpy(temp->line, buffer);

        next->next = temp;
        next = temp;
    }

    next->next = NULL;

    printf("\n\nPlease enter the number of threads: "); // taking the number of threads input
    scanf("%s", num);
    printf("\n");
    
    int procCount = atoi(num);
    int size = count / procCount;

    double seconds;
    struct timeval stop, start;
    time_t currtime1, currtime2;
    gettimeofday(&start, NULL);
    time(&currtime1);

    int id, err;
    int arr[2];
    pthread_t tid[10];


  
    for(i = 0; i < procCount; i++) // sort the data (kernel level threading)
    {

        if(i == (procCount - 1))
        {
            arr[0] = i * size;
            
            arr[1] = count - 1;
            
            err = pthread_create(&(tid[i]), NULL, (void*)&sort, (void*)arr); // sort using kernel threading
         }
        
        else
        {
            arr[0] = i * size;
           
            arr[1] = (i+1) * size - 1;
           
            err = pthread_create(&(tid[i]), NULL, (void*)&sort, (void*)arr); // sort using kernel threading
        }
    }

    for (i = 0; i < procCount; i++)
    { pthread_join(tid[i], NULL);}


  
    for(i = 0; i < procCount; i++) // merge the data
    {
        if(i == (procCount - 1))
        {
            merge(i*size, count-1);
        }
        else
        {
            merge(i*size, (i+1) * size - 1);
        }
    }

    next = globalHead;
    while(next != NULL)
    {
        printf("%f\n", next->latitude);
        
        next = next->next;
    }

    end = clock(); // end of timing
    
    programTime = ((double)(end-strt))/CLOCKS_PER_SEC;
    
    printf("\nTime taken by the program : %f\n",programTime); // print the program time take.

    return 0;

}// end of main