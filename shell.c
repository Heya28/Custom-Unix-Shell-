#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_PATH_LENGTH 256

void free_tokens(char** alltokens){
    if(alltokens==NULL){
        return;
    }
    int i=0;
    while(alltokens[i]!=NULL){
        free(alltokens[i]);
        i++;
    }
    free(alltokens);
}

// spilt the input string by space and return the array of strings. 
char** tokenize(char* inputLine){
    int tokenIndex=0;
    int tokenNo=0;
    int len=strlen(inputLine);
    char **alltokens = malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token=malloc(MAX_TOKEN_SIZE * sizeof(char));

    // malloc failure // 
    if(alltokens==NULL || token==NULL){
        perror("malloc");
        free(alltokens);
        free(token);
        return NULL; // this is caught by if(alltokens==NULL)
    }

    for(int i=0;i<len;i++){
        if(inputLine[i]==' '|| inputLine[i]=='\t' || inputLine[i]=='\n' ){
            if(tokenIndex!=0){ // handles case: only enter ( \n\0 ) && more than one whitespace
            token[tokenIndex]='\0'; // end of the token append. 
            alltokens[tokenNo]=malloc(MAX_TOKEN_SIZE*sizeof(char));

            if(alltokens[tokenNo]==NULL){
                // malloc crash // 
                perror("malloc");
                free(token);
                for(int j=0;j<tokenNo;j++){
                    free(alltokens[j]);
                } 
                free(alltokens);
                return NULL;

            }
            strcpy(alltokens[tokenNo],token); // copy token char sequence ending with \0 to alltokens[tokenNo]
            tokenNo++;
            tokenIndex=0; // next word starts writing from index 0, essentially overwriting the previous. 
            }
        }else{
            token[tokenIndex]=inputLine[i];
            tokenIndex++;
        }
    } 
    alltokens[tokenNo]=NULL; // required for execvp(), otherwise it keeps reading garbage.
    free(token); // temporary memory not required after loop ends. 
    return alltokens;
}

int main(int argc, char* argv[]){
    char inputLine[MAX_INPUT_SIZE];
    char **alltokens;
    char cwd[MAX_PATH_LENGTH]; 

    while(1){
        // reap background proceeses // 
        int back_status;
        while(waitpid(-1,&back_status,WNOHANG)>0){ // return -1 incase of no children. 
            printf("Shell: Background process finished.\n");
            if(WIFEXITED(back_status)){
                    printf("EXITSTATUS: %d\n",WEXITSTATUS(back_status));
            }
        } 

        // print cwd // 
        if(getcwd(cwd,sizeof(cwd))==NULL){
            perror("getcwd");
            continue;
        }
        printf("%s $ ", cwd);

        // clear out old input // 
        memset(inputLine,0,sizeof(inputLine));

        // take input // 
        if(fgets(inputLine, MAX_INPUT_SIZE, stdin)==NULL){
            break; // exits in case scenario of EOF ( Ctrl+ D )
        } // stores both \n and \0 No need for edge case separator handling

        // get alltokens // 
        alltokens=tokenize(inputLine); // Handle the freeing of alltokens memory incase of empty inputline in main() instead of in tokenize() --> return of dangling pointer

        if(alltokens==NULL){
            continue;
        }

        if(alltokens[0]==NULL){
        free(alltokens); // empty inputLine: user only pressed Enter. Handles execvp(NULL,..) crash later
        continue;
        }

        // Handling cd command
        if(strcmp(alltokens[0], "cd")==0){
            if(alltokens[1]==NULL || alltokens[2]!=NULL){
                // unsuccessful command cd
                fprintf(stderr,"Invalid argument.\n");
                free_tokens(alltokens);
                continue;
            }else{
                // successful command cd
                int cd_rc=chdir(alltokens[1]);
                if(cd_rc==-1){
                    perror("chdir");
                }
                free_tokens(alltokens);
                continue;
            }
        }
        
        // background process
        bool background=false;
        int k=0;
        while(alltokens[k]!=NULL){
            k++;
        }
        if((k>0) && strcmp(alltokens[k-1],"&")==0){
            background=true;
            // remove & for execvp(...)
            free(alltokens[k-1]);
            alltokens[k-1]=NULL;
        }
        if(alltokens[0]==NULL){
            fprintf(stderr,"Invalid input.\n");
            free(alltokens);
            continue;
        }

        // fork exec wait reap
        pid_t rc=fork();
        if(rc<0){
            perror("fork failed");
            // shell should continue not exit(1)
            free_tokens(alltokens);
            continue;
        }else if(rc==0){
            // child replaces itself with requested program
            execvp(alltokens[0],alltokens);
            perror("execvp");
            exit(1);
        }else{
            // parent process 
            if(!background){
                int status;
                pid_t wait_rc=waitpid(rc,&status,0); // wait_rc stores the pid of child process.     
                if(wait_rc==-1){ 
                    perror("waitpid"); // no exit only report
                }
                if(WIFEXITED(status)){
                    printf("EXITSTATUS: %d\n",WEXITSTATUS(status));
                }
            }
        }

        // free alltokens memory
        free_tokens(alltokens);
    }
    return 0;
}