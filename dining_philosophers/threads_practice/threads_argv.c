#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void    *print_hello(void *iteration)
{
    printf("Hello from thread %u - I was created in iteration %d\n", (int)(pthread_self()), (int)iteration);
    return (NULL);
}


int main(int ac, char **av)
{
    if (ac != 2)
        return (1);
    pthread_t tid[ac];
    for(int i = 0; i < atoi(av[1]); i++)
    {
        pthread_create(&tid[i], NULL, &print_hello, &tid[i]);
        printf("I am thread %d. Created new thread (%d) in iteration %d\n", (int)pthread_self(), (int)tid[i], i);
        if (i % 5 == 0)
            sleep(1);
    }   
    for(int i = 0;i < atoi(av[1]); i++)
        pthread_join(tid[i], NULL);
    return (0);
    pthread_exit(NULL);
}