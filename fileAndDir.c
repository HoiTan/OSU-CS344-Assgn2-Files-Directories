#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processCSV(){

}

int main(int argc, char* argv[]){
    
    int userChoice;
    char userStr[20];
    do{
        printf("\n1. Select file to process\n"
                "2. Exit the program\n"
                "\nEnter a choice 1 or 2: ");
        scanf("%s", userStr);
        userChoice = atoi(userStr);
        printf("%d\n", userChoice);

        if (userChoice == 1){
            int userChoice2;
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
                    break;
                
                case 2:
                    break;
                
                case 3:

                    break;
                
                default: 
                    printf("Invalid input, please try again!\n");
            }
        }
            
            else if(userChoice != 2)
                printf("Invalid input, please try again!\n");

    } while(userChoice != 2);

    return 0;
}