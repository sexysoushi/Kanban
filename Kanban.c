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
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE	
#define SEMPERM 0600	

/*******************************************************************************************************************/
/*  Pour Operation P et V 					     */

int sem1, sem2; // deux semaphores

struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

 /*  la valeur du semaphore est decrementee de 1 s'il est different
 *  de 0 sinon, le processus appelant est bloque et est place dans une 
 *  file d'attente liee au semaphore */
void P(int semid) {

sem_oper_P.sem_num = 0;
sem_oper_P.sem_op  = -1 ;
sem_oper_P.sem_flg = 0 ;

if ( semop(semid,&sem_oper_P,1) == -1) {
	printf("semid=%d ; \n", semid) ;
    perror("Erreur lors du P") ;
}
}

/* la valeur du semaphore est incremente de 1 s'il n'y a pas de processus  
   dans la file d'attente sinon, s reste inchangee et on libere le premier  
   processus de la file d'attente */
void V(int semid) {

sem_oper_V.sem_num = 0;
sem_oper_V.sem_op  = 1 ;
sem_oper_V.sem_flg  = 0 ;

if ( semop(semid,&sem_oper_V,1) == -1)
    perror("Erreur lors du V") ;
}

/**********************************************************************************************************************/
/* Création des sémaphore */
int sem_create (key_t semkey, int initval) {

 int status = 0;
 int semid_init ;
 union semun{
     int val ;  
   struct semid_ds *buf ; /* les deux champs suivants ne sont */
   ushort *array;         /* pas utilises dans notre situation nous ne les detaillons donc pas */
 } arg_ctl ;

 // On commence par creer le semaphore 
   if ((semid_init = semget(semkey, 3, IFLAGS)) > 0) {
		
	    	ushort array[3] = {0, 0, 0};
	    	arg_ctl.array = array;
	    	status = semctl(semid_init, 0, SETALL, arg_ctl);
    }
   if (semid_init == -1 || status == -1) { 
	perror("Erreur initsem");
	return (-1);
    } else return (semid_init);


 // Maintenant on affecte une valeur initiale au semaphore
 arg_ctl.val = initval ;
 if (semctl(semid_init,0,SETVAL,arg_ctl) == -1){
    perror("Erreur lors de l'initialisation du semaphore") ;
    exit(1) ;
        }

 printf("On cr\\'ee un s\\'emaphore dont le num\\'ero est %d\n",semid_init) ;

 return semid_init ;
}

/*Destruction du semaphore*/
void sem_delete(int semid){

  //printf("destruction du semaphore %d\n",semid) ;

  if(semctl(semid,0,IPC_RMID,0) == -1)
    perror("Erreur lors de la destruction du semaphore") ;
}
/*****************************************************************************************************************************/
 
/* affichage pour suivi du trajet */
void message(int i, char* s) {
   #define COLONNE 20
   int j, NbBlanc;
   NbBlanc=i*COLONNE;
   for (j=0; j<NbBlanc; j++) putchar(' ');
   printf("%s\n",s);
   fflush(stdout);
}

/* attente en seconde, ou aleatoire */
void attente(int n) {
   sleep(n); 	/*sleep(rand() % n);*/
}

/* creation du processus TGV */
void TGV(int i) {
   if (! fork()) {
	message(i, "depart Paris");
	attente(3);
	message(i, "arrivee Strasbourg");
	attente(3);
	V(sem1);
	message(i, "depart Strasbourg");
	attente(10);
	message(i, "arrivee Bâle");
	attente(3);
	message(i, "arret");
	exit(0);
   }
}

/* creation du processus TER */
void TER(int i) {
   if (! fork()) {
	message(i, "attente TGV");
	attente(3);
	P(sem1);
	message(i, "depart Strasbourg");
	attente(3);
	
	message(i, "arrivee Mulhouse");
	attente(3);
	V(sem2);
	message(i, "arret");
	exit(0);
   }
}

/* creation du processus taxi */
void Taxi(int i) {
   if (! fork()) {
	message(i, "attente TER");
	attente(3);
	P(sem2);
	message(i, "depart Mulhouse");
	attente(3);
	
	message(i, "arrivee Belfort");
	attente(3);
	message(i, "arret");
	exit(0);
   }
}

int main(int argc, char *argv[])
{
   int i,semid;

   printf("%10s%20s%20s\n\n", "TGV", "TER", "TAXI");
  

   sem1 = sem_create (SKEY, 0);
   sem2 = sem_create (SKEY, 0);

   TGV(0);
   TER(1);
   Taxi(2);
 
   for (i=1; i<=3; i++) wait(0);  
  
   sem_delete(sem1);
   sem_delete(sem2);

   return(0);
}