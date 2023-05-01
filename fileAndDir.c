#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>


char *buildDir(){
    int newDir;
    char dirName[256] = "tanho.movies.";
    char randomNum[6];
    DIR* currDir = opendir(".");
    struct dirent *aDir;
    srand(time(0));

    sprintf(randomNum, "%d", rand() % 99999);
    strcat(dirName, randomNum);
    
    while((aDir = readdir(currDir)) != NULL){
        if(strncmp(dirName, aDir->d_name, strlen(dirName)) == 0){
            strcpy(dirName, "tanho.movies.");
            sprintf(randomNum, "%d", rand() % 99999);
            strcat(dirName, randomNum);
        }
    }// Go through all the entries and check if the directory already existed

    newDir = mkdir(dirName, 0750); // mode: -rwxr-x--
    if (!newDir)
        printf("Created directory with name %s.\n", dirName);
    else {
        printf("Unable to create directory\n");
        exit(1);
    }
    closedir(currDir);
    return dirName;
} // Build a new directory inside the current folder 

typedef struct movie
{
    char movieTitle[51];
    int movieYear;
    struct movie* pre;
    struct movie* next;
} movie;

struct movie* processFile(char *filePath){
    FILE* myDataFile = fopen(filePath, "r"); // Open up the data file
    char fileName[100];//stores user's input file name
    char* readBuffer = NULL; // Will store a line from the file, allocated on heap
    size_t len = 0; // ignored by getline, but required
    int movieCount = 0;
    struct movie* head = (movie*)malloc(sizeof(movie));
   
    //read all data from the file and process it. then stdout: "Processed file XYZ and parsed data for M movies"
    
    if (myDataFile == NULL) { printf("fopen() failed\n"); return -1; } // Error check fopen()
    struct movie* current = head;
    struct movie* pre = NULL;

    while (getline(&readBuffer, &len, myDataFile) != -1) { // Read the file line by line
        if (movieCount > 0) {
            //printf("%s\n", readBuffer); // Display the line
            char* saveptr1;
            char* saveptr2;
            char* tok = strtok_r(readBuffer, ",", &saveptr1); //get movie title
            strcpy(current->movieTitle, tok);//get name

            tok = strtok_r(NULL, ",", &saveptr1);
            current->movieYear = atoi(tok);//get year

            tok = strtok_r(NULL, ",", &saveptr1);   //skip the language
            tok = strtok_r(NULL, ",", &saveptr1);   //skip the rate

            current->next = (movie*)malloc(sizeof(movie));
            pre = current;
            current = current->next; 
            current->pre = pre;
            memset(readBuffer, '\0', sizeof(readBuffer)); // Clear the reading buffer
        }
        movieCount++;
    };
    movieCount--;
    return head;
}//Return a linked list of movies by parsing data from input file 
//some codes are from the previous assignment movie.c


void printList(struct movie* n){
    while (n != NULL) {
        printf("n->movieTitle: %s\n", n->movieTitle);
        printf("n->movieYear: %d\n", n->movieYear);
        n = n->next;
    }
}

void processCSV( char *filePath){
    printf("Now processing the chosen file named %s\n", filePath);
    char dirPath[256];
    strcpy(dirPath, buildDir());
    struct movie* head = processFile(filePath);
    int fileDescriptor;
    char txtPath[256];
    while (head->movieYear != 0) {
        char yearReleased[4]; // store the released year in string
        sprintf(yearReleased, "%d", head->movieYear); // convert the int year to str year
        sprintf(txtPath, "%s/%d.txt", dirPath, head->movieYear); // build the txt file path

        // open the file, if not exist create it, then write the movie tilte into the txt file
        fileDescriptor = open(txtPath, O_RDWR | O_CREAT | O_APPEND, 0640);
        write(fileDescriptor, head->movieTitle, strlen(head->movieTitle));
        write(fileDescriptor, "\n", 1);
        close(fileDescriptor);
        
        printf("txtPath: %s\n", txtPath);
        head = head->next;
    }

}

typedef struct moviesFile {
    char name[256];
    int size;
}moviesFile;

#define PREFIX "movies_"

int main(int argc, char* argv[]){
    
    int userChoice;     //records user's choice
    char userStr[20];   //collects user's choice
    time_t lastModifTime;
    struct moviesFile largestFile;  //stores info of largest file 
    memset(largestFile.name, '\0', sizeof(largestFile));
    struct moviesFile smallestFile; //stores info of smallest file 
    memset(smallestFile.name, '\0', sizeof(smallestFile));
    struct stat dirStat;
    int i = 0;
    char fileChoice[256];

    /////////////////////////////
    // Open the current directory
    DIR* currDir = opendir(".");
    struct dirent *aDir;

    // Go through all the entries
    while((aDir = readdir(currDir)) != NULL){
        //printf("%s  %lu\n", aDir->d_name);  
        if(strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0){
            // Get meta-data for the current entry
            stat(aDir->d_name, &dirStat); 
            // printf("aDir->d_name: %s\n", aDir->d_name);
            // printf("dirStat.st_size: %d\n", dirStat.st_size);

            if (largestFile.name[0] == '\0' || dirStat.st_size > largestFile.size){
                if (smallestFile.name[0] == '\0'){
                    strcpy(smallestFile.name, aDir->d_name);
                    smallestFile.size = dirStat.st_size;
                    printf("smallestFile.name: %s\n",smallestFile.name);
                }
                    strcpy(largestFile.name, aDir->d_name);
                    largestFile.size = dirStat.st_size;
                    printf("largestFile.name: %s\n",largestFile.name);
            }

            else if (dirStat.st_size < largestFile.size){
                if (dirStat.st_size < smallestFile.size){
                    strcpy(smallestFile.name, aDir->d_name);
                    smallestFile.size = dirStat.st_size;
                    printf("smallestFile.name: %s\n",smallestFile.name);
                }
            }
        }
    }
    // Close the directory
    closedir(currDir);
    //////////////////
    //some of the codes are from Exploration: Directories

    do{
        printf("\n1. Select file to process\n"
                "2. Exit the program\n"
                "\nEnter a choice 1 or 2: ");
        scanf("%s", userStr);
        userChoice = atoi(userStr);
        printf("%d\n", userChoice);

        if (userChoice == 1){
            int userChoice2;
            do{
                char userStr2[20];
                printf("\nWhich file you want to process?\n"
                        "Enter 1 to pick the largest file\n"
                        "Enter 2 to pick the smallest file\n"
                        "Enter 3 to specify the name of a file\n"
                        "\nEnter a choice from 1 to 3: ");
                
                scanf("%s", userStr2);
                userChoice2 = atoi(userStr2);
                int found = 0;// one for found the input file, zero for not 
                switch(userChoice2){
                    case 1: 
                        processCSV(largestFile.name);
                        break;
                    
                    case 2:                       
                        processCSV(smallestFile.name);
                        break;
                    
                    case 3:
                        printf("Enter the complete file name:");
                        scanf("%s", fileChoice);
                        currDir = opendir(".");
                        while((aDir = readdir(currDir)) != NULL){ //go through each file of the directory, if matches process the file
                            if(strncmp(fileChoice, aDir->d_name, strlen(aDir->d_name)) == 0){
                                processCSV(fileChoice);
                                found = 1;
                                break;
                            }
                        }
                        if (found != 1){
                            printf("\nThe file %s was not found. Try again\n", fileChoice);
                            userChoice2 = 0;
                        }
                        closedir(currDir);
                        break;
                    
                    default: 
                        printf("Invalid input, please try again!\n");
                        userChoice2 = 0;
                }
            } while(userChoice2 == 0);
        }
            
            else if(userChoice != 2)
                printf("Invalid input, please try again!\n");

    } while(userChoice != 2);

    return 0;
}
