#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED
#endif // PARAMS_H_INCLUDED
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"



struct params {
        pthread_mutex_t mutex;
        int id, algorithm;
        ProcessList *processList;

};

typedef struct params params_t;
