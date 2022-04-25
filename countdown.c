#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

int timer_done = 0;
struct itimerval timer;

void timer_stop(int signum){
    timer_done = 1;
}

void timer_start(struct itimerval timer, float interval){
    timer.it_value.tv_sec = interval;
    timer.it_value.tv_usec =  0;
    timer.it_interval.tv_sec = interval/10;
    signal(SIGALRM, timer_stop);

    setitimer(ITIMER_REAL, &timer, NULL);
}

void display_time(int dots, char* output) {
    printf("%s\n", output);
}

int main(int argc, char *argv[]){
    float length = atof(argv[1]);
    int dots = length * 2;
    char output[10000];
    for (int i = 0; i < dots; i++){
        strcat(output, ".");
    }
    timer_start(timer, length+0.5);
    while (timer_done == 0){
        display_time(dots, &output);
        usleep(500000);
        fflush(0);
        dots --;
        int size = strlen(output);
        output[size-1] = '\0';
    }
    char *args[100];
    int i = 0;
    for (i; i < (argc-3); i++){
        args[i] = malloc(100);
        strcpy(args[i], argv[i + 3]);
    }
    if (argc > 3){
         args[i+1] = NULL;
    }
    int pid = fork();
    if (pid > 0){
        //wait(0);
        execv(argv[2], args);
    }
    else if (pid == 0){
        for (int i = 0; i < argc - 3; i++){
            free(args[i]);
        }
    }
    
    return 0;
}