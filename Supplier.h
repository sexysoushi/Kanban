#ifndef SUPPLIER_H
#define	SUPPLIER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct
{
	pthread_mutex_t mut;
	pthread_cond_t cond1;
}Supplier, *pSupplier;

void* Supplier_thread_fct();

#endif /* SUPPLIER_H */
