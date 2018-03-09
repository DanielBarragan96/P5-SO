/*Implantación de semáforos utilizando los mecanismos de
comunicación entre procesos ya conocidos.
Utilizando mecanismos básicos de comunicación entre procesos como son señales1 y memoria compartida2, desarrolle una implementación de semáforos enteros.

Para esta implementación es necesario:
• Definir la estructura de datos semáforo para definir las variables semáforo.
• Las variables de tipo semáforo deben estar en memoria compartida.
• Definir las primitivas waitsem y signalsem. Ambas primitivas reciben como
argumento un semáforo.
• Definir una primitiva para la inicialización de las variables semáforo initsem.
• Garantizar la atomicidad3 en las llamadas waitsem, signalsem e initsem, puede utilizar la solución por hardware aquí mostrada.
• En el Ejemplo 3 puede verse la forma de cómo un programa deberá llamar estas primitivas y los argumentos que reciben.
• Un proceso puede bloquearse enviándosele una señal SIGSTOP y puede ser reanudado
con una señal SIGCONT


*/

void proceso(int i)
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
		// Llamada waitsignal implementada en la parte 3
		signalsem(sem);
		// Espera aleatoria fuera de la sección crítica
		sleep(rand()%3);
	}
	exit(0); // Termina el proceso
}

int main()
{
	// Incializar el contador del semáforo en 1 una vez que esté
	// en memoria compartida, de manera que solo a un proceso se le
	// permitirá entrar a la sección crítica
	initsem(sem,1);
	

}
