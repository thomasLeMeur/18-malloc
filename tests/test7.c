#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void	*func( void* arg)
{
	(void)arg;
	printf("Enter in thread function\n");
	void *tmp = malloc(1024 * 1024);
	printf("Malloc %u succeeded\n", (int)arg);
	return NULL;
}

int main()
{
	pthread_t t[1024];

	printf("The progran begins\n");

	for (int i = 0 ; i < 1024 ; i++)
		pthread_create(&t[i], NULL, &func, (void *)i);

	printf("All threads are created\n");


	printf("The progran ends\n");
	for (int i = 0 ; i < 1024 ; i++)
		pthread_join(t[i], NULL);
}
