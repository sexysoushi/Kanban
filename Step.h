#ifndef STEP_H
#define	STEP_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct
{
	pthread_mutex_t mut;
	pthread_cond_t cond1;
	int nbPieces;
}Step, *pStep;

void* Step_thread_fct();

#endif /* STEP_H */
