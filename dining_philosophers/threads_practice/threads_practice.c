#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_product
{
    int     data;
    pthread_mutex_t mutex;
    struct s_product   *next;
} t_product;

t_product   *ft_lstlast(t_product *lst)
{
    while (lst && lst->next)
        lst = lst->next;
    return (lst);
}

t_product   *ft_lstbfr_last(t_product *stack)
{
    if (!stack || !stack->next)
        return (NULL);
    while (stack->next->next)
        stack = stack->next;
    return (stack);
}

t_product   *lst_new(int data)
{
    t_product   *lst;

    lst = malloc(sizeof(t_product));
    if (!lst)
    {
        free(lst);
        return (NULL);
    }
    lst->data = data;
    lst->next = NULL;
    return (lst);
}

void    ft_lstadd_back(t_product **lst, t_product *new)
{
    t_product   *last;

    if (!lst || !new)
        return ;
    if (!(*lst))
        *lst = new;
    else
    {
        last = ft_lstlast(*lst);
        last->next = new;
    }
}

////////////////////////////////////////////////////////////////////////////////

int is_there_space(t_product *products)
{
    int i = 0;
    while(products)
    {
        products = products->next;
        i++;
    }
    if (i >= 6)
        return (0);
    else
        return (1);
}

void    consume(t_product **products)
{
    t_product   *last, *bfr_last;
    if (products && (*products))
    {
        bfr_last = ft_lstbfr_last(*products);
        if (bfr_last)
        {
            last = ft_lstlast(*products);
            bfr_last->next = NULL;
            free(last);
            printf("CONSUMING NOW ...\n");
        }
    }
}

void    produce(t_product **products)
{
    if (products && (*products))
    {
        ft_lstadd_back(products, lst_new(1));
        printf("PRODUCING NOW ...\n");
    }
}

void    *producer(void *param)
{
    int i = -1;
    t_product *products = *((t_product **)param);
    pthread_mutex_init(&products->mutex, NULL);
    while (++i < 15)
    {
        pthread_mutex_lock(&products->mutex);
        produce(&products);
        pthread_mutex_unlock(&products->mutex);
        usleep(50000);
    }
    return (NULL);
}
void    *consumer(void *param)
{
    int i = -1;
    t_product *products = *((t_product **)param);
    pthread_mutex_init(&products->mutex, NULL);
    while (++i < 15)
    {
        pthread_mutex_lock(&products->mutex);
        consume(&products);
        pthread_mutex_unlock(&products->mutex);
        usleep(40000);
    }
    return (NULL);
}

int main ()
{
    pthread_t thread_id1;
    pthread_t thread_id2;
    t_product *products;
    products = (t_product *)calloc(sizeof(t_product), 6);
    pthread_create (&thread_id1, NULL, &producer, &products);
    pthread_create (&thread_id2, NULL, &consumer, &products);
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);
    pthread_exit(NULL);
    return 0;
}