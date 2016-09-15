#include "params.h"

int getId(params_t *p)
{
    int id;
    pthread_mutex_lock(&(p->mutex));
    id = p->id;
    pthread_mutex_unlock(&(p->mutex));

    return id;

}

void setId(params_t *p, int val)
{
    pthread_mutex_lock(&(p->mutex));
    p->id = val;
    pthread_mutex_unlock(&(p->mutex));
}

void incrementId(params_t *p)
{
    pthread_mutex_lock(&(p->mutex));
    p->id++;
    pthread_mutex_unlock(&(p->mutex));
}

void* hello(void* arg){
    params_t *p = (params_t*)(arg);
    incrementId(p);
    int id = getId(p);

    // This could possibly be quite messy since it
    // could print the data for multiple threads at once
    printf("Hello from %d\n", id);
}
