// Student Name: Yihao Zhong
// Net ID: yz7654 N-number: N16753551
// Reference:
// 1. feof() reference. https://www.tutorialspoint.com/c_standard_library/c_function_feof.htm
// 2. strtok_r reference. https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
//https://linux.die.net/man/3/strtok_r
//https://cboard.cprogramming.com/c-programming/91337-trying-use-strtok-function-parse-cl.html
// 3. struct reference. https://www.programiz.com/c-programming/c-structures
// wait for child: https://www.educative.io/answers/how-to-use-the-typedef-struct-in-c
// wait(): https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish
// https://www.youtube.com/watch?v=tcYo6hipaSA
// https://man7.org/linux/man-pages/man2/waitpid.2.html
// fork(): https://www.geeksforgeeks.org/fork-system-call/?id=discuss
// https://www.youtube.com/watch?v=cex9XrZCU14
// https://www.youtube.com/watch?v=Mqb2dVRe0uo
// setgrp(): https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-setpgrp-set-process-group-id
// signal: https://unix.stackexchange.com/questions/362559/list-of-terminal-generated-signals-eg-ctrl-c-sigint
// waitid: https://pubs.opengroup.org/onlinepubs/7908799/xsh/waitid.html
// echo -e : https://unix.stackexchange.com/questions/189787/difference-between-echo-and-echo-e
// exit(): https://www.cnblogs.com/nufangrensheng/archive/2013/03/01/2938508.html
// open(): https://man7.org/linux/man-pages/man2/open.2.html
// variable declare: https://www.geeksforgeeks.org/global-variables-in-c/?id=discuss
// strcmp: https://www.programiz.com/c-programming/library-function/string.h/strcmp
// WTFSTOPPED: https://docs.oracle.com/cd/E19455-01/817-5438/6mkt5pciv/index.html 
// debug: https://stackoverflow.com/questions/19641597/what-is-segmentation-fault-core-dumped 
// gdb: https://www.geeksforgeeks.org/how-to-find-segmentation-error-in-c-c-using-gdb/ 
// shell in C: https://brennan.io/2015/01/16/write-a-shell-in-c/ 
// shell function and flows: https://www.geeksforgeeks.org/making-linux-shell-c/ 
// design a shell: https://medium.com/swlh/tutorial-to-code-a-simple-shell-in-c-9405b2d3533e 
// fork and execv: https://stackoverflow.com/questions/28502305/writing-a-simple-shell-in-c-using-fork-execvp 
// C code for command line loop: http://www.dmulholl.com/lets-build/a-command-line-shell.html 
// group process id: https://man7.org/linux/man-pages/man3/tcsetpgrp.3.html
// pipe: https://linuxhint.com/pipe_system_call_c/ 
// parsing: https://stackoverflow.com/questions/9642732/parsing-command-line-arguments-in-c 
// mode: https://stackoverflow.com/questions/18415904/what-does-mode-t-0644-mean
// kill: https://www.ibm.com/docs/en/i/7.2?topic=ssw_ibm_i_72/apis/sigkill.htm 
// signal handler: https://www.geeksforgeeks.org/signals-c-language/ 
    // https://stackoverflow.com/questions/4458920/signal-handling-in-c
    // https://stackoverflow.com/questions/12953350/ignore-sigint-signal-in-child-process 
// ignore: https://stackoverflow.com/questions/49329057/how-to-redirect-signal-to-child-process-from-parent-process 
// error: https://www.computerhope.com/jargon/s/stderr.htm 
// relative path: https://desktop.arcgis.com/en/arcmap/10.3/tools/supplement/pathnames-explained-absolute-relative-unc-and-url.htm 
// basename and echo: https://stackoverflow.com/questions/11246189/how-to-convert-relative-path-to-absolute-path-in-unix 
// cat chdir and path: https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
// pipe and i/o redirection!! reference to the code architecture: https://stackoverflow.com/questions/40925272/shell-implementation-with-pipes-and-i-o-redirection-in-c 
// io redirection: reference to the code architecture  https://stackoverflow.com/questions/19846272/redirecting-i-o-implementation-of-a-shell-in-c 
    // https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection 
// file redirection demo in shell end: https://homepages.uc.edu/~thomam/Intro_Unix_Text/IO_Redir_Pipes.html 
// connection of pipe(): https://medium.com/swlh/understanding-pipes-in-unix-with-a-sample-implementation-9fe110d36412
//switch and case: https://stackoverflow.com/questions/48005819/can-we-use-switch-case-statement-with-strings-in-c 
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <signal.h>
#define COPYMODE 0600
// COMMAND STRUCT, contain base info of a command
typedef struct Command{
    int command_num; // id of a cmd
    char* argsFull;
    char* cmdName; //"args[0]"
    char** args;
    char* inputFile;
    char* outputFile;
    bool isAppend; // "<<"
} Command;

int exec(Command* input);
int susJob_num = 0;
char* jobList[101]; // max 100 jobs
int susJob_pid[101]; // max 100 jobs
char* curJob;
int prcInProgress;
void add_sus();

// signal handling

void void_handler(int s){
    if((s == SIGTSTP || s == SIGSTOP)){
        if((prcInProgress >=0)){
            add_sus();
        }
    }
    signal(s, void_handler);
}
// READING FROM USERS
char* read_from_user(){

    // create buffer space for input
    char* bufferSpace = malloc(sizeof(char) * 1000);
    size_t size = 0;

    // use getline() to read user input
    int n = getline(&bufferSpace, &size, stdin);
    if (bufferSpace[n-1] == '\n'){
        bufferSpace[n-1] = '\0';
    }
    else if (n == -1 ){
        printf("\n");
        exit(-1);
    }
    return bufferSpace;
}

void add_sus(){
    //jobList[susJob_num] = malloc(sizeof (Command*));
    jobList[susJob_num] = curJob;
    susJob_pid[susJob_num] = prcInProgress;
    susJob_num ++;
}
// error handling
void print_input_error(){
    fprintf(stderr,"Error: invalid command\n");
    fflush(stdout);
}
// error handling
void print_dir_error(){
    fprintf(stderr,"Error: invalid directory\n");
    fflush(stdout);
}
// error handling
void print_file_error(){
    fprintf(stderr, "Error: invalid file\n");
    fflush(stdout);
}
// error handling
void print_program_error() {
    fprintf(stderr, "Error: invalid program\n");
    fflush(stdout);
}
void print_job_error() {
    fprintf(stderr, "Error: invalid job\n");
    fflush(stdout);
}


// check the valid command, no special case
int isValidCmd(char* cmd){
    /*
    * 0 invalid
    * 1 valid
    */
    // null
    if (cmd == NULL)
        return 0;
    else if (!strcmp(cmd, "")){
        return 0;
    }
    // special case
    for(size_t i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == '`' || cmd[i] == '|'|| cmd[i] == '*')
            return 0;
    }
    return 1;

}

// check the valid of an input
int isValidInput(char* input){
    /*
     * 0 invalid
     * 1 valid
     */

    // check for null
    if (!input) {
        return 0;
    }
        // check for not input anything
    else if (!strcmp(input, "")){
        return 0;
    }
    // check for consective space
    for(int i = 0; *(input+i) != '\0'; i++){
        if(*(input+i) == ' ' && *(input+i+1) == ' ')
            return 0;
    }

    return 1;
}


Command** ParsingCenter(char* input){
    if(isValidInput(input) == 0){
        //print_input_error();
        return NULL;
    }
    
    // tokenlize
    input[strlen(input)] = '\0';
    //char* cmdName;
    char** argvList = malloc(sizeof  (char*) * 1001); // 2D array storage;
    char* token;

    curJob = input;
    char* temp = input;
    int i = 0;
    while((token = strtok_r(temp, " ", &temp))){
        if (token == NULL)
            break;
        // store in a 2D arr
        argvList[i] = malloc(sizeof (char)* (strlen(token)+1));
        argvList[i] = strdup( token);
        //argvList[i][strlen(token)] = '\0'; // strcpy need a termination
        i ++;
    }
    argvList[i] = NULL;

    // store it in to the Command struct, with pipe
    if (argvList == NULL){
        //print_input_error();
        return NULL;
    }
    // initialize the variable
    
    char** arg_temp;
    int arg_count;
    char* cmdTitle;
    int inputFileCount;
    int outputFileCount;

    // size to be 1001, randomly decided
    Command** commandCenter = malloc(sizeof (Command)* (1001));
    bool initiation = true;
    bool middle = false;
    bool isA = false;
    bool last = false;
    int pipeCount = 0;

    // loop all the input tokens
    
    while(*argvList != NULL){
        // initiaiion tells us whether this is a new cmd, under pipe or not.
        
        if ((initiation == true) || (middle = false)){
            // initialize
            arg_temp = malloc(sizeof (char*) * 1000);
            arg_count = 0;
            inputFileCount = 0; 
            outputFileCount = 0;
            isA = false;
            
            cmdTitle = NULL;
            arg_temp[arg_count] = malloc(sizeof (char*)*(strlen(*argvList)+1));
            cmdTitle = malloc(sizeof (char*)*(strlen(*argvList)+1));

            // check the command program
            if(isValidCmd(*argvList)) {
                arg_temp[arg_count] = strdup(*argvList);
                cmdTitle = strdup(*argvList);

                if((!strcmp(cmdTitle, "cd") || !strcmp(cmdTitle, "fg")) && (i!=2)){
                    //print_input_error();
                    return NULL;
                }
                else if((!strcmp(cmdTitle, "exit") || !strcmp(cmdTitle, "jobs")) && (i!=1)){
                    //print_input_error();
                    return NULL;
                
                }
                argvList++;
                arg_count++;
                initiation = false;
                middle = true;
                continue;
            }
            else{
                //print_input_error();
                return NULL;
            }
        }
    
        // if it is a pipe sign
        if (!strcmp(*argvList, "|")){
                // write to commands
                if (outputFileCount >0){
                    //print_input_error();
                    return NULL;
                }
                argvList++;
                if ((*argvList == NULL) || (strlen(*argvList)==0)){
                    //print_input_error();
                    return NULL;
                }
                // write to the struct since this is the end of last pipe
                if (commandCenter[pipeCount] == NULL)
                    commandCenter[pipeCount] = malloc(sizeof (Command));
                commandCenter[pipeCount]->args = arg_temp;
                commandCenter[pipeCount]->cmdName = cmdTitle;
                commandCenter[pipeCount]->command_num = pipeCount;
                commandCenter[pipeCount]->isAppend = isA;
                


                pipeCount ++;
                initiation = true;
                middle = false;
            }

                // redirection  out put
        else if(!strcmp(*argvList, ">")||!strcmp(*argvList, ">>")){
                if (outputFileCount >=1){
                    return NULL;
                }
                if(!strcmp(*argvList, ">>")){
                    isA = true;
                }
                argvList++;
                if ((*argvList == NULL) || (strlen(*argvList)==0)){
                    //print_input_error();
                    return NULL;
                }
                // put the file in the struct
                commandCenter[pipeCount] = malloc(sizeof(Command));
                commandCenter[pipeCount]->outputFile = strdup (*argvList);
                
                argvList++;
                arg_count++;
                outputFileCount++;
                if (*argvList != NULL){
                    if((strcmp(*argvList, "|")!=0) && (strcmp(*argvList, "<")!=0)){
                        return NULL;
                    }
                }

            }
                // redirection in put
        else if(!strcmp(*argvList, "<")){
                if (inputFileCount >=1){
                    return NULL;
                }
                if (pipeCount > 1){
                    //print_input_error();
                    return NULL;
                }
                argvList++;
                if ((*argvList == NULL) || (strlen(*argvList)==0)){
                    //print_input_error();
                    return NULL;
                }
                // store the input file to struct
                commandCenter[pipeCount] = malloc(sizeof(Command));
                commandCenter[pipeCount]->inputFile = strdup( *argvList);
                
                argvList++;
                inputFileCount ++;
                if (*argvList != NULL){
                    if((strcmp(*argvList, "|")!=0) && (strcmp(*argvList, ">>")!=0) && (strcmp(*argvList, ">")!=0)){
                        return NULL;
                    }
                }

        }
        else{
            last = true;
            
        }
            
        // none of the | << < >, then is a regular command token, put it in arg_temp[]
        if (last == true){
            arg_temp[arg_count] = malloc(sizeof (char*)*(strlen(*argvList)+1));
            arg_temp[arg_count] = strdup(*argvList);
            
            arg_count++;
            argvList++;
            last = false;
        }
        
    }

    // end of token loop, store in the struct.
    if ((initiation == false)){
        if (commandCenter[pipeCount] == NULL)
            commandCenter[pipeCount] = malloc(sizeof (Command));
        //commandCenter[pipeCount] ->argsFull = full;
        commandCenter[pipeCount]->args = arg_temp;
        commandCenter[pipeCount]->cmdName = cmdTitle;
        commandCenter[pipeCount]->command_num = pipeCount;
        commandCenter[pipeCount]->isAppend = isA;
        
    }

    // store the pipeCount variable, borrow space at the tail.
    commandCenter[pipeCount+1] = NULL;
    commandCenter[1000] = malloc(sizeof (Command));
    commandCenter[1000]->command_num = pipeCount;
    return commandCenter;
    
}

// built in cmd
int my_cd(char** cmd){
    /*
    * 0 invalid
    * 1 valid
    */
    if((cmd[0] == NULL) || (cmd[1]) == NULL){
        print_input_error();
        return 1;
    }
    else{
        // absolute path
        if((cmd[1][0] == '/') || (cmd[1][0] == '.')){
            if(chdir(cmd[1]) == -1){
                print_dir_error();
                return 1;
            }
        }
            // relative path
        else{
            char* temp = malloc(sizeof(char)*1000);
            strcpy(temp, "./"); strcat(temp, cmd[1]);
            if(chdir(temp) == -1){
                print_dir_error();
                return 1;
            }
        }
    }
    return 1;
}
// built in cmd
int my_exit(){
    /*
    * 0 invalid
    * 1 valid
    */
    if (susJob_num >0){
        fprintf(stderr, "Error: there are suspended jobs\n");
        return -3;
    }
    return -5;
}
// built in cmd
int my_jobs(){
    /*
    * 0 invalid
    * 1 valid
    */
    for(int i=0 ; i< susJob_num ; i++){
        //char * temp = strdup((jobList[i]->argsFull));
        printf("[%d]  %s\n",i+1, jobList[i]);
        fflush(stdout);
    }
    return 1;
}
// built in cmd
int my_fg(char** cmd){
    /*
    * 0 invalid
    * 1 valid
    */
    int t = (int) strtol(cmd[1], NULL, 10) - 1;
    if ((t <0) || (t > susJob_num-1)){
        print_job_error();
        return  1;
    }
    else{
        int savePID = susJob_pid[t];
        curJob = strdup(jobList[t]);
        int count =0;
        for(int k = 0; k < susJob_num-1; k++){
            if (k <t){
                count ++;
            }
            else{
                jobList[k] = jobList[k+1];
                susJob_pid[k] = susJob_pid[k+1];
            }
        }
        if (count >= susJob_num){
            return 1;
        }
        susJob_num --;
        kill(savePID, SIGCONT);
        waitpid(-1, NULL, WUNTRACED);
    }
    return 1;
}



// ultimate execuation center
int execCenter(Command** cmdCenter){
    /*
    * 0 invalid
    * 1 valid
     * -1 fail to fork
    */
    int pipeCount = cmdCenter[1000]->command_num;
    int** fd = malloc(sizeof(int*) * (pipeCount));
    //printf(" %d ", pipeCount);

    // create pipe and file descriptor storage
    for(int i = 0; i< pipeCount; i++){
        fd[i] = (int*) malloc(sizeof(int)*2);
        int st = pipe(fd[i]);
        if(st == -1){
            return -1;
        }
    }
    int execResult;
    //pid_t wpid;
    int pid_all[pipeCount+1];
    int pidCount = 0;
    // execuation loop
    
    for(int i = 0; i<= pipeCount; i++){
        // redirect to built in command
        //curJob = malloc(sizeof (Command*));
        //curJob = cmdCenter[i]->argsFull;
        char* nm = malloc(sizeof(char)*1000);
        nm = strdup(cmdCenter[i]->cmdName);
        
        
        if(!strcmp(nm, "cd")){
            execResult = my_cd(cmdCenter[i]->args);
        }
        else if(!strcmp(nm, "exit")){
            execResult = my_exit();
        }
        else if(!strcmp(nm, "jobs")){
            execResult = my_jobs();
        }
        else if(!strcmp(nm, "fg")){
            execResult = my_fg(cmdCenter[i]->args);
        }
        else{
            // relative or basename
            if((cmdCenter[i]->args[0][0] != '.')&&(cmdCenter[i]->args[0][0] != '/')){
                // relative
                if (strstr(cmdCenter[i]->args[0], "/") != NULL){
                    char prefix[500] = "./";
                    strcat(prefix, cmdCenter[i]->args[0]);
                    cmdCenter[i]->cmdName = prefix;
                }
                    // basename
                else {
                    char prefix[500] = "/usr/bin/";
                    strcat(prefix, cmdCenter[i]->args[0]);
                    cmdCenter[i]->cmdName = prefix;
                }
            }

            // fork() to create children
            Command * input = cmdCenter[i];
            int child = fork();
            prcInProgress = child;
            //curJob = input;
            pid_all[i] = child;
            pidCount++;
            //int status = 1;
            if (child == 0){

                //int curPrc = getpid();
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);


                // I/O redirection and input output file dup2
                if(input-> command_num == 0 ){
                    if(input ->inputFile != NULL){
                        int redirect_in;
                        
                        //printf(input->inputFile);
                        if((redirect_in = open(input ->inputFile, O_RDONLY,COPYMODE)) < 0 ){
                            print_file_error();
                            return -1;
                        }
                        else{
                            dup2(redirect_in, STDIN_FILENO);
                            close(redirect_in);
                        }
                    }
                }
                // the end outfile
                if (input-> command_num == pipeCount) {
                    if(input ->outputFile!= NULL){
                        if(input -> isAppend == true) { //append mode <<
                            int file_flag = O_WRONLY | O_APPEND| O_CREAT;
                            int redirect_o = open(input ->outputFile, file_flag,  COPYMODE);
                            dup2(redirect_o, STDOUT_FILENO);
                            close(redirect_o);
                        }
                        else { // overwrite to mode <
                            int file_flag = O_WRONLY| O_CREAT | O_TRUNC;
                            int redirect_o = open(input ->outputFile, file_flag, COPYMODE);
                            dup2(redirect_o, STDOUT_FILENO);
                            close(redirect_o);
                        }
                    }
                }

                // Entering the pipe if we have any
                if (pipeCount > 0) {
                    // if it is start of pipe
                    // first child redirects its output to the write end of the pipe
                    if(input->command_num == 0){
                        dup2(fd[input->command_num][1], STDOUT_FILENO);
                    }

                        // if it is the last of pipe
                        // last child redirects its input from the read end of the pipe
                    else if (input->command_num == pipeCount){
                        dup2(fd[input->command_num - 1][0], STDIN_FILENO);
                    }

                        // middle of some pipes
                        // output to the write end / input from the read end
                    else {
                        dup2(fd[input->command_num - 1][0], STDIN_FILENO);
                        dup2(fd[input->command_num][1], STDOUT_FILENO);
                    }

                }
                // close fd for child
                for(int i = 0; i<pipeCount;i++){
                    close(fd[i][0]);
                    close(fd[i][1]);
                }
                // printf("the path is %s\n", input->args[0]);
                // while(input->args != NULL){
                //     printf("the arg is %s\n", *(input->args));
                //     (input->args) ++;
                // }

                execResult = execv(input->cmdName, input->args);

                //exit(-1);// If we get here, something is wrong.
            }

            
        }
        if (execResult == -1){
            print_program_error();
            exit(-1);
        }
        
    }
    int status;
    // close fd for parent
    for(int i = 0; i< pipeCount; i++){
        close(fd[i][0]); close(fd[i][1]);
    }
    // wait and collect children status
    for(int j = 0; j <= pidCount; j++){
        waitpid(pid_all[j], &status, WUNTRACED);
        
    }
    if (WIFSTOPPED(status)){
        //printf("enter %d", 1);
        // send to scheduler
        void_handler(SIGSTOP);
    }
    //while ((wpid = waitpid(-1, NULL, WUNTRACED)) > 0);
    prcInProgress = -1;

    free(fd);
    return execResult;
}



int main(){

    /*
     * Initiation
     */
    int loopStatus = 1;
    char* input;


    // char baseName[PATH_MAX];

    // PROMPTING THE SHELL LOOP
    //    loop until user hit exit command

    // SHELL LOOP
    while(1){
        //jobList = malloc(sizeof(Command)*1000);
        signal(SIGINT, SIG_IGN); // ignore it
        signal(SIGQUIT, SIG_IGN);
        signal(SIGSTOP, void_handler); // send to handler
        signal(SIGTSTP, void_handler);
        // INITIATE

        fprintf(stdout, "[nyush %s]$ ", basename(getcwd(NULL, 0)));
        fflush(stdout);
        // READ

        input = read_from_user();
        // PARSE
        Command ** command = malloc(sizeof (Command*)*1000);
        command = ParsingCenter(input);
        if(command != NULL){
            loopStatus = execCenter(command);
        }
        else{
            print_input_error();
        }
        //printf("%d", loopStatus);

        // EXECUTE
        if(loopStatus == -5) // exit
            break;

        // RINSE

    }
    //free (jobList);
}
