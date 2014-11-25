#include "Step.h"

void* Step_thread_fct(void* structure)
{
	pStep arg;
	arg = (pStep) structure;
	
	printf("Begining of Step\n");
	
	
	printf("Step: Manufacturing of a part ...\n");
	sleep(1+random()%10);
	printf("Fisnish step\n");
	
	pthread_exit(NULL);
}
