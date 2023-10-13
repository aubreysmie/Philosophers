#include <stdio.h>
#include <pthread.h>

void	*start_routine(char *special_word)
{
	printf("Today we love %s more than ever!\n", special_word);
}

int	main(void)
{
	pthread_t	t1;

	pthread_create(&t1, NULL, &start_routine, )
}