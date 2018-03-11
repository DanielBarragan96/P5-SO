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
	int* value;
}Semaphore;


char *pais[3]={"Peru","Bolvia","Colombia"};

void initsem(Semaphore checkSem, int value)
{
	*checkSem.value = checkSem.max_val;
	checkSem.max_val = value;
}

void waitsem(Semaphore checkSem)
{
	int l=0;
	//do { atomic_xchg(l,*sem.value); } while(l<=0);
	while(l > *checkSem.value)
	{
	}
	*checkSem.value -= 1;
}

void signalsem(Semaphore checkSem)
{
	if(checkSem.max_val > *checkSem.value)
	{
		*checkSem.value += 1;		
	}
}

void proceso(Semaphore sem, int i)
{
	int k;
	for(k=0;k<CICLOS;k++)
	{
		// Llamada waitsem implementada en la parte 3
		waitsem(sem);
		printf("Entra %s ",pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf("- %s Sale\n",pais[i]);
		// Llamada signalsem implementada en la parte 3
		signalsem(sem);
		// Espera aleatoria fuera de la sección crítica
		sleep(rand()%3);
	}
	exit(0); // Termina el proceso
}

int main()
{
	Semaphore sem;
	int pid;
	int status;
	int shmid;
	int args[3];
	
	// Solicitar memoria compartida
	shmid=shmget(0x1234,sizeof(sem),0666|IPC_CREAT);
	
	if(shmid==-1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	
	// Conectar la variable a la memoria compartida
	int* shm_ptr = (int *) shmat(shmid,NULL,0);
	
	if(shm_ptr==NULL)
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


