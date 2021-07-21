#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
typedef unsigned int u32_t;
typedef int cpuflg_t;
typedef struct {
    volatile u32_t lock;
}spinlock_t;

static inline void spinlock_init(spinlock_t* lock)
{
    lock->lock = 0;
}

static void spin_lock(spinlock_t* lock)
{
    __asm__ __volatile__(
        "1: \n"
        "lock;xchg %0, %1 \n"
        "cmpl $0, %0 \n"
        "jnz 2f \n"
        "jmp 3f \n"
        "2:\n"
        "cmpl $0, %1 \n"
        "jne 2b \n"
        "jmp 1b \n"
        "3:\n"
        :
        :"r"(1),"m"(*lock)
    );
}

static void spin_unlock(spinlock_t* lock)
{
    __asm__ __volatile__(
        "movl $0, %0\n"
        :
        :"m"(*lock)
    );
}
spinlock_t lock;

void thread_test()
{
    spin_lock(&lock);
    return;
}

static void spin_lock_disable_irq(spinlock_t * lock,cpuflg_t* flags)
{
    __asm__ __volatile__(
        "pushfq \n"
        "cli \n"
        "popq %0"
        "1: \n"
        "lock; xchg %1, %2 \n"
        "cmpl $0, %1 \n"
        "jnz 2f \n"
        "jmp 3f \n"
        "2: \n"
        "cmpl $0, %2 \n"
        "jne 2b \n"
        "jmp 1b \n"
        "3: \n"
        :
        :"=m"(*flags)
        :"r"(1),"m"(*lock)
    );
}

static void spin_unlock_disable_irq(spinlock_t * lock,cpuflg_t* flags)
{
    __asm__ __volatile__(
        "movl $0, %0 \n"
        "pushq %1 \n"
        "popfq \n"
        :
        :"m"(*lock),"m"(*flags)
    );
}

int main()
{
    
    spinlock_init(&lock);
    spin_lock(&lock);
    pthread_t tid;
    pthread_create(&tid, NULL, thread_test, NULL);
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 1000; j++)
        {
            usleep(1);
        }
    }
    spin_unlock(&lock);
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 1000; j++)
        {
            usleep(1);
        }
    }
    return 0;
}