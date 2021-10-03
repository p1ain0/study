#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/export.h>

static unsigned long cr0, cr3;

static unsigned long vaddr = 0;

static void get_pgtable_maroc(void)
{
    cr0 = read_cr0();
    cr3 = read_cr3_pa();
    printk("cr0 = 0x%lx, cr3 = 0x%lx\n", cr0, cr3);
    printk("PGDIR_SHIFT = 0x%x",PGDIR_SHIFT);
    printk("P4D_SHIFT = 0x%x", P4D_SHIFT);
    printk("PUD_SHIFT = 0x%x", PUD_SHIFT);
    printk("PMD_SHIFT = 0x%x", PMD_SHIFT);
    printk("PAGE_SHIFT = 0x%x", PAGE_SHIFT);

    printk("PTRS_PER_PGD = 0x%x", PTRS_PER_PGD);
    printk("PTRS_PER_P4D = 0x%x", PTRS_PER_P4D);
    printk("PTRS_PER_PUD = 0x%x", PTRS_PER_PUD);
    printk("PTRS_PER_PMD = 0x%x", PTRS_PER_PMD);
    printk("PTRS_PER_PTE = 0x%x", PTRS_PER_PTE);
    printk("PACE_MASK = 0x%lx", PAGE_MASK);
}

static unsigned long  vaddr2paddr(unsigned long vaddr)
{
    pgd_t* pgd;
    p4d_t* p4d;
    pud_t* pud;
    pmd_t* pmd;
    pte_t* pte;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;
    pgd = pgd_offset(current->mm, vaddr);
    printk("pgd = 0x%lx", pgd);
    p4d = p4d_offset(pgd, vaddr);
    printk("p4d = 0x%lx", p4d);
    pud = pud_offset(p4d, vaddr);
    printk("pud = 0x%lx", pud);
    pmd = pmd_offset(pud, vaddr);
    printk("pmd = 0x%lx", pmd);
    page_addr = pte_offset_kernel(pmd, vaddr);
    printk("page_addr = 0x%lx", page_addr);
    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;
    return paddr;

}

static int __init v2p_init(void)
{
    get_pgtable_maroc();
    int x;
    unsigned long c = vaddr2paddr((unsigned long) &x);
    printk("%p" ,&x);
    printk("%p", c);
    return 0;
}

static void __exit v2p_exit(void)
{

}

module_init(v2p_init);
module_exit(v2p_exit);

