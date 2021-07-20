#include <stddef.h>
#include <assert.h>
typedef unsigned int u32_t;
typedef int s32_t;

typedef struct s_ATOMIC{
    volatile u32_t count;
}atomic_t;

static inline u32_t atomic_read(const atomic_t* v)
{
    return (*(volatile u32_t*)&v->count);
}

static inline void atomic_write(atomic_t* v, int i)
{
    v->count = i;
}

static inline void atomic_add(int i, atomic_t* v)
{
    __asm__ __volatile__("lock;" "addl %1, %0"
                       : "+m" (v->count)
                       : "ir" (i)
    );
}

static inline void atomic_sub(int i, atomic_t* v)
{
    __asm__ __volatile__("lock;" "subl %1, %0"
                        :"+m"(v->count)
                        :"ir"(i)
                        );
}

static inline void atomic_inc(atomic_t* v)
{
    __asm__ __volatile__("lock;" "incl %0"
                        :"+m"(v->count)
                        );
}

static inline void atomic_dec(atomic_t* v)
{
    __asm__ __volatile__("lock;" "decl %0"
                        :"+m"(v->count)
                        );
}

int main()
{
    atomic_t atom;
    atomic_write(&atom, 0);
    atomic_inc(&atom);
    assert(atomic_read(&atom)==1);
    atomic_dec(&atom);
    assert(atomic_read(&atom)==0);
    atomic_add(10, &atom);
    
    assert(atomic_read(&atom)==10);
    atomic_sub(10, &atom);

    assert(atomic_read(&atom)==0);
    return 0;
}
