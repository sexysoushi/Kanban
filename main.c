/* Application of the Kanban's method
 * 
 * Authors : Renaud Guillaume - Schiavi Barbara
 * Last modification : November 20, 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include "structures.h"



void traitantSIGINT(int s)
{
	exit(-1);
}

int main(int argc, char* argv[])
{
	
	signal(SIGINT,traitantSIGINT);

	return 0;
}
