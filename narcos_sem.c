//narcos_procesos.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
   #define atomic_xchg(A,B) __asm__ __volatile__( \
						" lock xchg %1,%0 ;\n" \
						: "=ir" (A) \
						: "m" (B), "ir" (A) \
						);

#define CICLOS 10

typedef struct
{
	int max_val;
	float* value;
}Semaphore;
	
int shmid;
float* shm;
char *pais[3]={"Peru","Bolvia","Colombia"};

void initsem(Semaphore* checkSem, int value)
{
	checkSem->max_val = value;
	checkSem->value = shm;
}

void waitsem(Semaphore* checkSem, float* shm)
{
	int l=0;
	while(l > *checkSem->value);
	*checkSem->value -= 1;
}

void signalsem(Semaphore* checkSem)
{
	if(checkSem->max_val > *checkSem->value)
	{
		*checkSem->value += 1;	
	}
}

void proceso(Semaphore* sem, int i)
{
	int k;	

	for(k=0;k<CICLOS;k++)
	{
		//printf("\nproceso %d con %s y sema: %f\n",i,pais[i],*shm);
		waitsem(sem,shm);
		printf("Entra %s ",pais[i]);
		//fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
		signalsem(sem);
		// Espera aleatoria fuera de la sección crítica
		sleep(rand()%3);
	}
	exit(0); // Termina el proceso
}

int main()
{
	int init =0;
	Semaphore sema;
	sema.max_val=0;
	Semaphore* sem = &sema;
	int pid;
	int status;

	int args[3];
	// Solicitar memoria compartida
	shmid=shmget(0x1234,sizeof(Semaphore),0666|IPC_CREAT);
	if(shmid==-1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	// Conectar la variable a la memoria compartida
	shm = shmat(shmid,NULL,0);
	
	if(shm==NULL)
	{
		perror("Error en el shmat\n");
		exit(2);
	}

	initsem(sem,0);//inicia en cero por que no está tomado
	
	for(int i=0;i<3;i++)
	{
		// Crea un nuevo proceso hijo que ejecuta la función proceso()
		pid=fork();
		if(pid==0)
		proceso(sem, i);
	}
	for(int i=0;i<3;i++)
		pid = wait(&status);
		
	// Eliminar la memoria compartida
	shmdt(sem);
}


