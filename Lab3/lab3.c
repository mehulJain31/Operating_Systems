
// Lab 3 Mehul Jain
// 1001229017

/*Create a file repository - called STAR (Simple TAR) that has:

c - create a new repository  with name

a - add a file to repository

d -delete a file

t - trash a file

l - list directory

g - get a copy of the file (don't remove)

The directory should be at the end or the repository

Keep file attributes: up to 60 characters of the local name, size, time and date

You may use any programming language you wish.*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define SIZE_OF_BLOCK    256
#define FILE_SIZE 56
#define TIME_LENGTH   100
#define MAX_LINK     10

typedef struct Table_bp
{
  //size of file
  int size_file;

  //index to dataBlock
  int dataBlockPointer;

  //When last modified
  char DTmodified[TIME_LENGTH];
 
} Table_bp;

typedef struct Table_fn
{

  //name of file being stored
  char fileName[FILE_SIZE];

  //iNode Pointer
    Table_bp iNode;
} Table_fn;

struct Link
{
  char linkName[FILE_SIZE];
    Table_bp *linkedFile;
} Link;


struct FileSystem
{

  //Name of File System
  char name_of_fs[FILE_SIZE];
  
  //Number of Data blocks in System
  int number_of_blocks;
  
  //Maps available data slot for FNT
  int *FNTMap;
  
  //Maps available data slot for ABPT
  int *ABPTMap;

  //Maps available data slot for datablock CPM style                            
  int *dataBlockMap;

  //Keeping track of FNT blocks that are available                            
  int FNT;

  //Keep track of ABPT blocks avail                   
  int ABPT;
  
   //Structure for holding initial files
  struct Table_fn *files;
  
  struct Table_bp *pointer_table;
  
  char **dataBlocks;
  
  struct Link link[MAX_LINK];
} FileSystem;

struct FileSystem FS;

void formatFS(char name[FILE_SIZE],int pointer_entries_num,int FNT)
{


    printf(" File System created with \n name:%s\n no. of pointers: %d\n no of files:%d \n",name,pointer_entries_num,FNT);

    FS.ABPT=pointer_entries_num; // no of pointer entries

    //storing the name of the filesystem
    strcpy(FS.name_of_fs,name);

    FS.FNT=FNT; // no of files

    //initialization
    FS.files=malloc(FNT*sizeof(struct Table_fn));
    
    FS.pointer_table=malloc(pointer_entries_num*sizeof(struct Table_bp));  
    
    FS.FNTMap= malloc(FNT*sizeof(int*));
    
    FS.ABPTMap= malloc(pointer_entries_num*sizeof(int*));
}
  
void createFS(int number_of_blocks)
{
    
    int i;
    
    int j;
    char **d_holder;

    printf("Datablocks Selected: %d\n",number_of_blocks);
    FS.number_of_blocks=number_of_blocks;

    d_holder=(char**) malloc(SIZE_OF_BLOCK*sizeof(char*));
    for(i=0;i<number_of_blocks;i++)
    {
      d_holder[i]=(char*)malloc(number_of_blocks*sizeof(char));    
    } 

    FS.dataBlockMap= malloc(number_of_blocks*sizeof(int*));
    FS.dataBlocks=d_holder;
  
}


void run() //main_function 
{
    int choice=-1;

    char name_of_fs[FILE_SIZE];
    
    char deleter[FILE_SIZE];

    char trasher[FILE_SIZE];
  
  while(1)
  {

    printf("1) Exit\n");
    printf("2) Create FS\n"); //createfs
    printf("3) Format FS\n"); //formatefs
    printf("4) Delete a file\n"); //delete a file
    printf("5) Trash a file\n"); //delete a file
    
    printf("Choice?: ");
    scanf("%d",&choice); // take user choice
    printf("\n");
    switch(choice)
    {

      case 1: // exit if not selected 1 or 2
        exit(0);
      
      case 2: // creating the file system
        printf("Enter the number of data blocks in the system: \n");
        
        int block_number; // number of blocks
        
        scanf("%d",&block_number);  
        
        createFS(block_number); // create the file system with user specified number of blocks
        
        printf("***Disk Created***\n\n"); // disk created successfully
      
        break;
    
      case 3: // formatting the file system
        printf("***File system formatting in progress...\n"); 
        
        printf("File System Name: \n"); 
        
        scanf("%s",name_of_fs); // file system to be formatted
        
        printf("Block Pointers Number?: \n"); // number of block pointers
        
        int numBlockPointers;
        
        int numFiles;
        
        scanf("%d",&numBlockPointers);
        
        printf("Number of files?: \n"); // no of files
        
        scanf("%d",&numFiles);
        
        formatFS(name_of_fs,numBlockPointers,numFiles); // format the file system with the specified parameters
        
        printf("***Disk Formatted***\n\n"); // formatted successfully
        
        break;

        case 4:
          printf("File name?");

          scanf("%s",deleter);

          printf("%s File deleted\n\n\n",deleter); 

          break;

        case 5:
          printf("File name?");

          scanf("%s",trasher);

          printf("%s File Trashed\n\n\n",trasher); 
        
          break;
      
    }
  }
}

int main()
{
  run();
  return 0; 
}
