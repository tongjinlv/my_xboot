/*
 * exception.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <arm64.h>
#include <interrupt/interrupt.h>
#include <xboot/gdbstub.h>

struct arm64_regs_t {
	uint64_t x[30];
	uint64_t lr;
	uint64_t sp;
	uint64_t pc;
	uint64_t pstate;
};

static void show_regs(struct arm64_regs_t * regs)
{
	int i;

	printf("pc : [<%016llx>] lr : [<%016llx>] pstate: %08llx\r\n", regs->pc, regs->lr, regs->pstate);
	printf("sp : %016llx\r\n", regs->sp);
	for(i = 29; i >= 0; i--)
	{
		printf("x%-2d: %016llx ", i, regs->x[i]);
		if(i % 2 == 0)
			printf("\r\n");
	}
	printf("\r\n");
	while(1);
}

void arm64_invalid_exception(struct arm64_regs_t * regs, int reason)
{
	const char * handler[] = {
		"Synchronous Abort",
		"IRQ",
		"FIQ",
		"Error"
	};

	printf("Invalid exception in %s handler detected, code 0x%02x\r\n", handler[reason & 0x3], reason);
	show_regs(regs);
}

void arm64_sync_exception(struct arm64_regs_t * regs)
{
	uint64_t esr, far;
	uint64_t ec, iss;

	esr = arm64_read_sysreg(esr_el1);
	far = arm64_read_sysreg(far_el1);
	ec = (esr >> 26) & 0x3f;
	iss = (esr >> 0) & 0x1ffffff;

	switch(ec)
	{
	case 0x3c:	/* BRK (AArch64) */
		if(iss == 0x401)
			regs->pc += 4;
		gdbserver_handle_exception(regs);
		return;

	default:
		break;
	}

	printf("Synchronous exception detected, ec:0x%x iss:0x%x far:0x%x\r\n", ec, iss, far);
	show_regs(regs);
}

void arm64_irq_exception(struct arm64_regs_t * regs)
{
	interrupt_handle_exception(regs);
}
