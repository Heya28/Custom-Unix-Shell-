#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char** tokenize(char* inputLine){
    int tokenIndex=0;
    int tokenNo=0;
    int i;
    char **alltokens = malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token=malloc(MAX_TOKEN_SIZE * sizeof(char));

    for(int i=0;i<strlen(inputLine);i++){
        if(inputLine[i]==' '|| inputLine[i]=='\t' || inputLine[i]=='\n' ){
            token[tokenIndex]='\0'; // end of the token append. 
            alltokens[tokenNo]=malloc(MAX_TOKEN_SIZE*sizeof(char));
            strcpy(alltokens[tokenNo],token); // copy token char sequence ending with \0 to alltokens[tokenNo]
            tokenNo++;
            tokenIndex=0; // next word starts writing from index 0, essentially overwriting the previous. 
        }else{
            token[tokenIndex]=inputLine[i];
            tokenIndex++;
        }
    }
    alltokens[tokenNo]=NULL; // required for execvp(), otherwise it keeps reading garbage.
    return alltokens;
}