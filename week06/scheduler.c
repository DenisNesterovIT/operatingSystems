#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

// holds the scheduling data of one process
typedef struct{
	int idx; // process idx (index)
	int at, bt, rt, wt, ct, tat; // arrival time, burst time, response time, waiting time, completion time, turnaround time.
	int burst; // remaining burst (this should decrement when the process is being executed)
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
unsigned total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
unsigned data_size = sizeof(data);

void read_file(FILE* file){
    for (int i = 0; i < 3; i++)
    {
        char string[5];
        fscanf(file, "%s", string);
    }
    int flag = 0;
    int index = 0;
    while (!feof(file))
    {
        char string[10];
        fscanf(file,"%s", string);
        switch (flag)
        {
        case 0:
            data[index].idx = atoi(string);
            flag++;
            break;
        case 1:
            data[index].at = atoi(string);
            flag++;
            break;
        case 2:
            data[index].bt = atoi(string);
            flag = 0;
            index++;
            break;
        default:
            break;
        }
    }
    fclose(file);
    for (int i = 0; i < PS_MAX; i++)
    {
        ps[i] = 0;
    }

}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    if (process > 0 && kill(process, 0) == 0) {
        kill(process, SIGCONT);
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    if (process > 0 && kill(process, 0) == 0) {
        kill(process, SIGTSTP);
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    if (process > 0 && kill(process, 0) == 0) {
        kill(process, SIGTERM);
    }
    // TODO: send signal SIGTERM to the worker process if it is not in one of the states
    // (1.not created yet or 2.terminated)
}

// create a process using fork
void create_process(int new_process) {

    pid_t new_pid = fork();
    if (new_pid == 0) {
        char process_index_str[5];
        sprintf(process_index_str, "%d", new_process);
        char* worker_argv[] = {"./worker", process_index_str, NULL};
        execvp(worker_argv[0], worker_argv);
    } else {
        ps[new_process] = new_pid;
        running_process = new_process;
    }
    // TODO: stop the running process

    // TODO: fork a new process and add it to ps array

    // TODO: Now the idx of the running process is new_process 

    // TODO: The scheduler process runs the program "./worker {new_process}" 
    // using one of the exec functions like execvp

}

// find next process for running
ProcessData find_next_process() {

  // location of next process in {data} array
	int location = 0;

	for(int i=0; i < data_size; i++) {
        if (data[i].at < data[location].at) {
            location = i;
        }
        // TODO: find location of the next process to run from the {data} array
        // Considering the scheduling algorithm FCFS

	}



	// if next_process did not arrive so far, 
    // then we recursively call this function after incrementing total_time
	if(data[location].at > total_time){
        
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        
        // increment the time
        total_time++;
        return find_next_process(); 
	}

  // return the data of next process
	return data[location];
}


// reports the metrics and simulation results
void report(){
	printf("Simulation results.....\n");
	int sum_wt = 0;
	int sum_tat = 0;
	for (int i=0; i< data_size; i++){
		printf("process %d: \n", i);
		printf("	at=%d\n", data[i].at);
		printf("	bt=%d\n", data[i].bt);
		printf("	ct=%d\n", data[i].ct);
		printf("	wt=%d\n", data[i].wt);
		printf("	tat=%d\n", data[i].tat);
		printf("	rt=%d\n", data[i].rt);
		sum_wt += data[i].wt;
		sum_tat += data[i].tat;
	}

	printf("data size = %d\n", data_size);
	float avg_wt = (float)sum_wt/data_size;
	float avg_tat = (float)sum_tat/data_size;
	printf("Average results for this run:\n");
	printf("	avg_wt=%f\n", avg_wt);
	printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){

	for(int i = 0; i < data_size; i++)
		if (data[i].burst > 0)
		    return;

    // report simulation results
    report();

    // terminate the scheduler :)
	exit(EXIT_SUCCESS);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // increment the total time
    total_time++;

    if (running_process != -1) {
        data[running_process].burst--;
        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
        if (data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            terminate(ps[running_process]);
            waitpid(ps[running_process], NULL, 0);
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            running_process = -1;
        }
    }
    ProcessData next_process = find_next_process();
    if (running_process != next_process.idx) {
        if (running_process != -1) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(ps[running_process]);
        }
        running_process = next_process.idx;
        create_process(running_process);
        printf("Scheduler: Starting Process %d (Remaining Time: {%d})\n",running_process, data[running_process].burst);
        if (data[running_process].rt == 0) {
            resume(ps[running_process]);
            printf("Scheduler: Resuming Process %d (Remaining Time: {%d})\n",running_process, data[running_process].burst);
        } else {
            data[running_process].rt = total_time - data[running_process].at;
        }
    }

    check_burst();


}


int main(int argc, char *argv[]) {

    // read the data file
    FILE *in_file  = fopen(argv[1], "r");
    if (in_file == NULL) {   
		printf("File is not found or cannot open it!\n"); 
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while(1); // infinite loop
}