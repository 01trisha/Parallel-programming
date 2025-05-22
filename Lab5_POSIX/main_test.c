#include <math.h>
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define AMOUNT_OF_LISTS 5
#define WEIGHT_COEFFICIENT 2000

#define MIN_AMOUNT_OF_TASKS_TO_SHARE 20
#define TASKS_PER_PROCESS 2400

#define TAG_REQUEST 0
#define TAG_REPLY 1

double RES_PER_ITERATION = 0;
double GLOBAL_RESULT_SIN = 0;

int rankProc, countProc;

int *tasks;
int tasksInRemain;
int amountOfTasksAlreadyExecuted;

pthread_mutex_t mutexTasks;
pthread_mutex_t mutexTasksInRemain;

pthread_t recvThread;

double activeTime = 0;

void initTasksWeight() {
  pthread_mutex_lock(&mutexTasks);
  for (int i = 0; i < TASKS_PER_PROCESS; ++i) {
    tasks[i] = abs(50 - i % 100) *
               abs(rankProc - (TASKS_PER_PROCESS % countProc)) *
               WEIGHT_COEFFICIENT;
  }
  pthread_mutex_unlock(&mutexTasks);
}

void calculateTask() {
    double local_res = 0;

    pthread_mutex_lock(&mutexTasksInRemain);
    while (tasksInRemain > 0) {
        int current_task = --tasksInRemain;
        pthread_mutex_unlock(&mutexTasksInRemain);

        pthread_mutex_lock(&mutexTasks);
        int task_weight = tasks[current_task];
        pthread_mutex_unlock(&mutexTasks);

        double startActive = MPI_Wtime();
        for (int j = 0; j < task_weight; ++j) {
            local_res += sin(j);
        }
        double endActive = MPI_Wtime();
        activeTime += (endActive - startActive);

        pthread_mutex_lock(&mutexTasksInRemain);
        amountOfTasksAlreadyExecuted++;
    }

    RES_PER_ITERATION += local_res;
    pthread_mutex_unlock(&mutexTasksInRemain);
}

void *receiverThreadGo(void *args) {
  int tasksToSend;
  int rankRequestedTasks;

  while (1) {
    MPI_Recv(&rankRequestedTasks, 1, MPI_INT, MPI_ANY_SOURCE, TAG_REQUEST,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rankRequestedTasks == rankProc)
      break;

    pthread_mutex_lock(&mutexTasksInRemain);
    if (tasksInRemain >= MIN_AMOUNT_OF_TASKS_TO_SHARE) {
      tasksToSend = tasksInRemain / 2;
      tasksInRemain -= tasksToSend;

      MPI_Send(&tasksToSend, 1, MPI_INT, rankRequestedTasks, TAG_REPLY,
               MPI_COMM_WORLD);

      pthread_mutex_lock(&mutexTasks);
      MPI_Send(tasks + amountOfTasksAlreadyExecuted + tasksInRemain - 1,
               tasksToSend, MPI_INT, rankRequestedTasks, TAG_REPLY,
               MPI_COMM_WORLD);
      pthread_mutex_unlock(&mutexTasksInRemain);
      pthread_mutex_unlock(&mutexTasks);
    } else {
      tasksToSend = 0;
      MPI_Send(&tasksToSend, 1, MPI_INT, rankRequestedTasks, TAG_REPLY,
               MPI_COMM_WORLD);
      pthread_mutex_unlock(&mutexTasksInRemain);
    }
  }
  return NULL;
}

void *workerThreadGo(void *args) {
  tasks = (int *)malloc(TASKS_PER_PROCESS * sizeof(int));

  double startTime;
  double minTime, maxTime;

  for (int iterCounter = 0; iterCounter < AMOUNT_OF_LISTS; ++iterCounter) {
    initTasksWeight();

    pthread_mutex_lock(&mutexTasksInRemain);
    tasksInRemain = TASKS_PER_PROCESS;
    pthread_mutex_unlock(&mutexTasksInRemain);
    amountOfTasksAlreadyExecuted = 0;
    activeTime = 0;
    int amountOfAdditionalTasks;

    startTime = MPI_Wtime();

    calculateTask();

    for (int currentProc = 0; currentProc < countProc; ++currentProc) {
      if (currentProc == rankProc)
        continue;

      MPI_Send(&rankProc, 1, MPI_INT, currentProc, TAG_REQUEST, MPI_COMM_WORLD);
      MPI_Recv(&amountOfAdditionalTasks, 1, MPI_INT, currentProc, TAG_REPLY,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (amountOfAdditionalTasks > 0) {
        MPI_Recv(tasks, amountOfAdditionalTasks, MPI_INT, currentProc,
                 TAG_REPLY, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pthread_mutex_lock(&mutexTasksInRemain);
        tasksInRemain = amountOfAdditionalTasks;
        pthread_mutex_unlock(&mutexTasksInRemain);

        calculateTask();
      }
    }

    double endt = MPI_Wtime();
    double resTime = endt - startTime;

    MPI_Allreduce(&resTime, &minTime, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&resTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    int totalExecuted;
    MPI_Reduce(&amountOfTasksAlreadyExecuted, &totalExecuted, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rankProc == 0) {
      printf("=================================================\n");
      printf("Iteration number: %d\n", iterCounter);
      printf("Disbalance time: %f\n", maxTime - minTime);
      printf("Disbalance percentage: %f\n",
             (maxTime - minTime) / maxTime * 100);
      printf("----------------------------------------------\n");

      int expectedTotal = TASKS_PER_PROCESS * countProc;
      if (totalExecuted != expectedTotal) {
        printf("ERROR: Not all tasks were executed! Expected %d, got %d\n", expectedTotal, totalExecuted);
      } else {
        printf("All tasks were successfully executed.\n");
      }
    }

    for (int currentProc = 0; currentProc < countProc; ++currentProc) {
      if (rankProc == currentProc) {
        printf("\t\tCurrent proc is: %d\n", rankProc);
        printf("Amount of executed tasks: %d\n", amountOfTasksAlreadyExecuted);
        printf("Result of calculating is: %f\n", RES_PER_ITERATION);
        printf("Time per iteration: %f seconds\n", resTime);
        printf("Active time: %f seconds\n", activeTime);
        printf("Idle time percentage: %f%%\n", (1.0 - activeTime / resTime) * 100);
      }
      MPI_Barrier(MPI_COMM_WORLD);
    }
  }

  MPI_Send(&rankProc, 1, MPI_INT, rankProc, 0, MPI_COMM_WORLD);

  MPI_Allreduce(&RES_PER_ITERATION, &GLOBAL_RESULT_SIN, 1, MPI_DOUBLE, MPI_SUM,
                MPI_COMM_WORLD);

  free(tasks);

  return NULL;
}

void createAndGoThreads() {
  pthread_mutex_init(&mutexTasks, NULL);
  pthread_mutex_init(&mutexTasksInRemain, NULL);

  pthread_attr_t attributes;
  if (pthread_attr_init(&attributes) != 0) {
    MPI_Finalize();
    perror("Can't init attributes");
    abort();
  }

  if (pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE) != 0) {
    MPI_Finalize();
    perror("Error in setting attributes");
    abort();
  }

  if (pthread_create(&recvThread, &attributes, receiverThreadGo, NULL) != 0) {
    MPI_Finalize();
    perror("Can't create thread");
    abort();
  }

  pthread_attr_destroy(&attributes);

  workerThreadGo(NULL);
  pthread_join(recvThread, NULL);

  pthread_mutex_destroy(&mutexTasks);
  pthread_mutex_destroy(&mutexTasksInRemain);
}

int main(int argc, char **argv) {
  int reqiredLevel = MPI_THREAD_MULTIPLE;
  int providedLevel;

  MPI_Init_thread(&argc, &argv, reqiredLevel, &providedLevel);
  if (providedLevel != reqiredLevel) {
    MPI_Finalize();
    perror("Can't load required level");
    return 0;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &countProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);

  double startTime = MPI_Wtime();
  createAndGoThreads();
  double endTime = MPI_Wtime();

  double resTime = endTime - startTime;

  if (rankProc == 0) {
    printf("=======================================\n");
    printf("Time for all lists: %f seconds\n", resTime);
    printf("Final result of all sin sums: %f\n", GLOBAL_RESULT_SIN);
  }

  MPI_Finalize();
  return 0;
}

