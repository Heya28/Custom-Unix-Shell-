#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// spilt the input string by space and return the array of strings. 
char** tokenize(char* inputLine){
    int tokenIndex=0;
    int tokenNo=0;
    int len=strlen(inputLine);
    char **alltokens = malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token=malloc(MAX_TOKEN_SIZE * sizeof(char));

    for(int i=0;i<len;i++){
        if(inputLine[i]==' '|| inputLine[i]=='\t' || inputLine[i]=='\n' ){
            if(tokenIndex!=0){
            token[tokenIndex]='\0'; // end of the token append. 
            alltokens[tokenNo]=malloc(MAX_TOKEN_SIZE*sizeof(char));
            strcpy(alltokens[tokenNo],token); // copy token char sequence ending with \0 to alltokens[tokenNo]
            tokenNo++;
            tokenIndex=0; // next word starts writing from index 0, essentially overwriting the previous. 
            }
        }else{
            token[tokenIndex]=inputLine[i];
            tokenIndex++;
        }
    }
    if(tokenIndex!=0){
        token[tokenIndex]='\0';  // incase of no separator at the end of inputline (edge case eg. ls)
        // allocate memory
        alltokens[tokenNo]=malloc(MAX_TOKEN_SIZE*sizeof(char));
        strcpy(alltokens[tokenNo],token);
        tokenNo++;
    }  
    alltokens[tokenNo]=NULL; // required for execvp(), otherwise it keeps reading garbage.
    free(token); // temporary memory not required after loop ends. 
    return alltokens;
}

int main(int argc, char* argv[]){
    char inputLine[1024];
    char **alltokens;
    while(1){
        // clear out old input
        memset(inputLine,0,sizeof(inputLine));
        // take input
        printf("$ ");
        scanf("%[^\n]");
        getchar(); // get rid of \n character before next iteration

        alltokens=tokenize(inputLine);
        // free alltokens memory
        for(int k=0; alltokens[k]!=NULL;k++){
            free(alltokens[k]);
        }
        free(alltokens);
    }
    return 0;
}