/*
*Commande client (semaphore)
*qui déclanche automatiquement la production (thread)
*conteneur (semaphore et thread ?)
*carte magnetique (thread? c'est un compteur plus tot?)
*boite au lettre (?)
*homme flux récupere aléatoirement des cartes magnétiques
*
*flux et produits doivent etre parametrable dans la console avant lancement de l'application 
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE	
#define SEMPERM 0600

/*********************************************************************/
/*  Pour Operation P et V */

int semid_init ;			/* Identificateur du tableau de sémaphores */
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

/*********************************************************************/

int initsem(key_t semkey) 
{
	int status = 0;		

	union sem_un 
	{
		int val;
		struct semid_ds *stat;
		ushort * array;
	} ctl_arg;

	if ((semid_init = semget(semkey, 2, IFLAGS)) > 0) 
	{
		ushort array[2] = {0, 0}; //création tableau non vide
		ctl_arg.array = array;
		status = semctl(semid_init, 0, SETALL, ctl_arg);
	}

	if (semid_init == -1 || status == -1) 
	{ 
		perror("Erreur initsem");
		return (-1);
	}
	else 
		return (semid_init);
}

 
int P(int sem_num) 
{
	sem_oper_P.sem_num = semnum;
	sem_oper_P.sem_op = -1;
	sem_oper_P.sem_flg = 0;
	
	return semop(semid_init, &sem_oper_P, 1);
}

int V(int sem_num) 
{
	sem_oper_V.sem_num = semnum;
	sem_oper_V.sem_op = 1;
	sem_oper_V.sem_flg = 0;
	
	return semop(semid_init, &sem_oper_V, 1);
}

 
/* affichage pour suivi du trajet */
void message(int i, char* s) 
{
   	#define COLONNE 20
   	int j, NbBlanc;
   
	NbBlanc=i*COLONNE;
   	for (j=0; j<NbBlanc; j++) putchar(' ');
   	
	printf("%s\n",s);
   	fflush(stdout);
}

/* attente en seconde, ou aleatoire */
void attente(int n) 
{
   	sleep(n); 	/*sleep(rand() % n);*/
}

/* creation du processus TGV */
void TGV(int i) 
{
   	if (! fork()) 
	{
		message(i, "depart Paris");
		attente(3);
		message(i, "arrivee Strasbourg");
		attente(3);
		V(0);
		message(i, "depart Strasbourg");
		attente(10);
		message(i, "arrivee Bâle");
		attente(3);
		message(i, "arret");
		exit(0);
   	}
}

/* creation du processus TER */
void TER(int i) 
{
   	if (! fork()) 
	{
		message(i, "attente TGV");
		attente(3);
		P(0);
		message(i, "depart Strasbourg");
		attente(3);
	
		message(i, "arrivee Mulhouse");
		attente(3);
		V(1);
		message(i, "arret");
		exit(0);
   	}
}

/* creation du processus taxi */
void Taxi(int i) 
{
   	if (! fork()) 
	{
		message(i, "attente TER");
		attente(3);
		P(1);
		message(i, "depart Mulhouse");
		attente(3);
	
		message(i, "arrivee Belfort");
		attente(3);
		message(i, "arret");
		exit(0);
   	}
}

void traitantSIGINT(int s)
{
	semctl(semid, 0, IPC_RMID, 0);
	exit(-1);
}

int main(int argc, char *argv[])
{	
    int i, semid;
    
    signal(SIGINT,traitantSIGINT);	/* Masque sur le signal SIGINT */
	
	if ((semid = initsem(SKEY)) < 0)	// Création d'un ensemble de sémaphore
		return(1);

   	printf("%10s%20s%20s\n\n", "TGV", "TER", "TAXI");
  
   	TGV(0);
   	TER(1);
   	Taxi(2);
 
   	for (i=1; i<=3; i++) wait(0);  

	semctl(semid, 0, IPC_RMID, 0);
  
   	return(0);
}
