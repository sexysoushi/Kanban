#include "Supplier.h"

void* Supplier_thread_fct()
{

	printf("Supplier: Manufacturing of a part ...\n");
	sleep(1+random()%10);
	
	pthread_exit(NULL);
}
