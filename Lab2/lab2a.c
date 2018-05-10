// mehul jain 1001229017
// lab2 part a
//srted using latitude

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define BUFFER_SIZE 1024*1024


struct Line_node
{
    float latitude;
    
    char line[300];
    
    struct Line_node *next, *previous;
};
typedef struct Line_node node_t;
node_t *head = NULL;
node_t *headGlobal = NULL;


struct procNode
{
    node_t *list;
    struct procNode *next;
};
typedef struct procNode node_p;


node_t *getNode(int index)
{
    int i;
    node_t *temp = head;

    for(i = 0; i < index; i++)
    {
        temp = temp->next;
    }

    return temp;
}

void merge(int startIndex, int endIndex) // merging function
{
    int i = startIndex, j;
    
    float latitude; // latitude input
    
    char line[300];
    
    node_t *previous, *temp, *previous_temp, *next, *localHead, *end, *newNode;

    end = getNode(endIndex);
    next = getNode(startIndex);

    while(next != NULL && next != end->next)
    {
        if(headGlobal == NULL)
        {
            headGlobal = malloc(sizeof(struct Line_node));
            
            strcpy(headGlobal->line, next->line);
            
            headGlobal->latitude = next->latitude;
            
            headGlobal->next = NULL;

        }
        else if(headGlobal->latitude >= next->latitude)
        {
            
            newNode = malloc(sizeof(struct Line_node));
            
            strcpy(newNode->line, next->line);
            
            newNode->latitude = next->latitude;
            
            newNode->next = headGlobal;
            
            headGlobal = newNode;
        }
        else
        {
            previous_temp = headGlobal;
            
            temp = headGlobal->next;

            if(temp == NULL)
            {
                newNode = malloc(sizeof(struct Line_node));
            
                strcpy(newNode->line, next->line);
            
                newNode->latitude = next->latitude;
            
                newNode->next = NULL;
            
                headGlobal->next = newNode;
            }
            else
            {
                while(temp != NULL)
                {
                    if(temp->next == NULL)
                    {
                        if(temp->latitude >= next->latitude)
                        {
                            newNode = malloc(sizeof(struct Line_node));
                          
                            strcpy(newNode->line, next->line);
                          
                            newNode->latitude = next->latitude;
                          
                            newNode->next = temp;
                          
                            previous_temp->next = newNode;
                        }
                        else
                        {
                            newNode = malloc(sizeof(struct Line_node));
                          
                            strcpy(newNode->line, next->line);
                          
                            newNode->latitude = next->latitude;
                          
                            newNode->next = NULL;
                          
                            temp->next = newNode;
                        }

                        break;
                    }
                    else if(temp->latitude >= next->latitude)
                    {
                        newNode = malloc(sizeof(struct Line_node));
                        
                        strcpy(newNode->line, next->line);
                        
                        newNode->latitude = next->latitude;
                        
                        newNode->next = temp;
                        
                        previous_temp->next = newNode;

                        break;
                    }

                    previous_temp = temp;
                    temp = temp->next;

                }
            }
        }

        previous = next;
        next = next->next;

    }
}

void sort(int startIndex, int endIndex) // sorting function, used bubble sort
{
    

    int i = startIndex, j;
    float latitude;
    char line[300];
    node_t *previous, *temp, *previous_temp, *next, *localHead, *end;

    end = getNode(endIndex);
    previous = getNode(startIndex);
    localHead = previous;
    next = previous->next;

    while(next != NULL && next != end)
    {
        temp = localHead;

        while(temp != next && temp != NULL)
        {

            if(temp->latitude >= next->latitude)
            {
                strcpy(line, next->line);
                
                latitude = next->latitude;
                
                previous_temp = next;
                
                while(previous_temp != temp && previous_temp != NULL)
                {
                    strcpy(previous_temp->line, previous_temp->previous->line);
                
                    previous_temp->latitude = previous_temp->previous->latitude;
                
                    previous_temp = previous_temp->previous;
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
    time_t start,end;
    double prog_time;
    start = clock(); // start the clock for calculating the time

    node_t *next, *temp;
    char buffer[500], num[2];
    FILE *fp;
    char *token;
    int i, j, count = 1;

    int fd = open("/Documents/program_2", O_RDWR, 0); // open the directory


    if ((fp=fopen("earthquakes.csv", "r"))==NULL) // check for the file
    {
        printf ("Error: File cannot be opened"); // if the file cannt be opened
        return 1;
    }

    fgets(buffer, 500, fp);
    fgets(buffer, 500, fp);
    
    token = strtok(buffer, ",");
    token = strtok (NULL, ",");

    head = mmap(NULL, sizeof(struct Line_node), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0); // find the head of the file
    
    strcpy(head->line, buffer);
    
    head->latitude = atof(token);
    head->next = NULL;

    next = head;

    while ((fgets(buffer, 500, fp)))
    {
        count++;
    
        token = strtok(buffer, ",");
        token = strtok (NULL, ",");
    
        // Create new node
        temp = mmap(NULL, sizeof(struct Line_node), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);//malloc(sizeof(struct Line_node));
    
        temp->latitude = atof(token);
    
        temp->next = NULL;
    
        strcpy(temp->line, buffer);

        next->next = temp;
        next = temp;

    }

    next->next = NULL;

    printf("\n\nPlease enter the number of processes: "); // take the number of processes
    scanf("%s", num);
    printf("\n");
    int procCount = atoi(num);
    int size = count / procCount;
    int id;

    // Sorting
    for(i = 0; i < procCount; i++)
    {
        id = fork(); // fork into threads
        if(id == 0)
        {
            if(i == (procCount - 1)) // sorting using multithreading
            {
                sort(i*size, count-1);
            }
            else
            {
                sort(i*size, (i+1) * size - 1);
            }
            exit(0);
        }
    }


    
    for(i = 0; i < procCount; i++) // merging the threads
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

    next = headGlobal;
    while(next != NULL)
    {
        printf("%f\n", next->latitude);
        
        next = next->next;
    }

    end = clock(); // end of timing
    
    prog_time = (double)(end-start)/CLOCKS_PER_SEC; // calculate the program time
    
    printf("\nTotal time taken : %f \n", prog_time); // print the total time taken

    return 0;

} // end of main
