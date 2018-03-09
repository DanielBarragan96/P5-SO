all:
	gcc -o narcos_hilos narcos_hilos.c -pthread
	gcc -o narcos_procesos narcos_procesos.c 
	gcc -o narcos_sem narcos_sem.c 

clean:

	-f narcos_hilos
	-f narcos_procesos
	-f narcos_sem
