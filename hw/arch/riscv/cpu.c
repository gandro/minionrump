/*-
 * Copyright (c) 2015 Sebastian Wicki.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <bmk/types.h>
#include <bmk/kernel.h>

#include <bmk-core/core.h>
#include <bmk-core/sched.h>
#include <bmk-core/printf.h>
#include <bmk-core/pgalloc.h>

#include <bmk-core/riscv/isr.h>

#include "encoding.h"

extern const char _tdata_start[], _tdata_end[];
extern const char _tbss_start[], _tbss_end[];
#define TDATASIZE (_tdata_end - _tdata_start)
#define TBSSSIZE (_tbss_end - _tbss_start)
#define TCBOFFSET \
    (((TDATASIZE + TBSSSIZE + sizeof(void *)-1)/sizeof(void *))*sizeof(void *))

int bmk_spldepth = 1;

int
bmk_cpu_intr_init(int intr)
{
	return BMK_EGENERIC;
}

void
bmk_cpu_intr_ack(void)
{
    /* XXX */
}

bmk_time_t
bmk_cpu_clock_now(void)
{
    return rdcycle();
}

void
bmk_cpu_nanohlt(void)
{

    /* XXX */
}

/* XXX Currently, RISC-V thread local storage is not properly documented,
 * but GCC assumes that the tp register points after the TCB, directly into
 * tdata
 */
void
bmk_platform_cpu_sched_settls(struct bmk_tcb *next)
{
    unsigned long tp = next->btcb_tp - TCBOFFSET;
	__asm__ __volatile__("mv tp, %0" : : "r"(tp));
}


void
bmk_cpu_riscv_trap(int code, void *pc, void *badaddr)
{
    bmk_printf("Trap %d at pc %p, addr %p\n", code, pc, badaddr);
    while(1);
}

void bmk_cpu_isr_timer(void)
{

}

void bmk_cpu_isr_htif(void)
{
	uintptr_t fromhost = swap_csr(mfromhost, 0);
	if (!fromhost)
		return;
}

void bmk_cpu_isr_sw(void)
{
    while(1);
}

/* Supposedly, the amount of physical memory can be read in address 0x0, but
 * this is not the case for spike. Thus, we use a fixed amount of memory
 */
static void
bmk_riscv_meminit(void)
{
	extern char _end[];
	unsigned long mem_start, mem_end;

	mem_start = bmk_round_page((unsigned long)_end);
	mem_end = (32 << 20);

	bmk_pgalloc_loadmem(mem_start, mem_end);
	bmk_memsize = mem_end - mem_start;
}

void
bmk_cpu_boot(void* argh)
{
	bmk_sched_init();

	bmk_printf_init(bmk_cons_putc, NULL);
	bmk_core_init(BMK_THREAD_STACK_PAGE_ORDER, PAGE_SHIFT);

	bmk_printf("rump kernel bare metal riscv bootstrap\n\n");

	bmk_riscv_meminit();
	spl0();

	bmk_run("");
}
