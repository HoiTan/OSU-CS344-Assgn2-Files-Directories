#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void processCSV(char *filePath){
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
    
    newDir = mkdir(dirName, 0750);
    if (!newDir)
        printf("Created directory with name %s.\n", dirName);
    else {
        printf("Unable to create directory\n");
        exit(1);
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

                switch(userChoice2){
                    case 1: 
                        printf("\nNow processing the chosen file named %s\n", largestFile.name);
                        processCSV(largestFile.name);
                        break;
                    
                    case 2:
                        printf("\nNow processing the chosen file named %s\n", smallestFile.name);
                        processCSV(smallestFile.name);
                        break;
                    
                    case 3:
                        printf("Enter the complete file name:");
                        scanf("%s", fileChoice);
                        currDir = opendir(".");
                        closedir(currDir);
                        userChoice2 = 0;
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