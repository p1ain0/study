//关闭中断
void hal_cli()
{    
    __asm__ __volatile__("cli": : :"memory");
}

//开启中断
void hal_sti()
{    
    __asm__ __volatile__("sti": : :"memory");
}
typedef unsigned int u32_t;
typedef u32_t cpuflag_t;

static void hal_save_flag_cli(cpuflag_t* flags)
{
    __asm__ __volatile__(
        "pushfl \t\n"
        "cli \t\n"
        "popl %0\t\n"
        :"=m" (*flags)
        :
        :"memory"
    );
}

static void hal_restore_flag_cli(cpuflag_t* flags)
{
    __asm__ __volatile__(
        "pushl %0 \t\n"
        "popf \t\n"
        :
        :"m" (*flags)
        :"memory"
    );
}

//使用场景
void foo()
{    
    hal_cli();    //操作数据……    
    hal_sti();
}
void bar()
{    
    hal_cli();    //操作数据……    
    hal_sti();
}

int main()
{
    return 0;
}