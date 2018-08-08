#ifndef __GDBSTUB_H__
#define __GDBSTUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

enum gdb_breakpoint_type_t {
	BP_TYPE_SOFTWARE_BREAKPOINT	= 0,
	BP_TYPE_HARDWARE_BREAKPOINT	= 1,
	BP_TYPE_WRITE_WATCHPOINT	= 2,
	BP_TYPE_READ_WATCHPOINT		= 3,
	BP_TYPE_ACCESS_WATCHPOINT	= 4,
};

struct gdb_breakpoint_t {
	enum gdb_breakpoint_type_t type;
	virtual_addr_t addr;
	virtual_size_t size;
	uint8_t instr[16];
	struct list_head entry;
};

struct gdb_cpu_t {
	const int nregs;
	void (*register_save)(struct gdb_cpu_t * cpu, void * regs);
	void (*register_restore)(struct gdb_cpu_t * cpu, void * regs);
	int  (*register_read)(struct gdb_cpu_t * cpu, char * buf, int n);
	int  (*register_write)(struct gdb_cpu_t * cpu, char * buf, int n);
	int  (*breakpoint_insert)(struct gdb_cpu_t * cpu, struct gdb_breakpoint_t * bp);
	int  (*breakpoint_remove)(struct gdb_cpu_t * cpu, struct gdb_breakpoint_t * bp);
	int  (*singlestep_active)(struct gdb_cpu_t * cpu);
	int  (*singlestep_finish)(struct gdb_cpu_t * cpu);
	int  (*memory_acess)(struct gdb_cpu_t * cpu, virtual_addr_t addr, virtual_size_t size, int rw);
	int  (*processor)(struct gdb_cpu_t * cpu);
	void (*breakpoint)(struct gdb_cpu_t * cpu);
	void * env;
};

struct gdb_iterface_t {
	int  (*read)(struct gdb_iterface_t * iface, char * buf, int count);
	int  (*write)(struct gdb_iterface_t * iface, const char * buf, int count);
	void (*flush)(struct gdb_iterface_t * iface);
	void * priv;
};

struct gdb_state_t {
	int trapping;
	int connected;
	struct gdb_breakpoint_t bp;
	struct gdb_cpu_t * cpu;
	struct gdb_iterface_t * iface;
};

int gdbserver_start(const char * device);
void gdbserver_stop(void);
void gdbserver_handle_exception(void * regs);

#ifdef __cplusplus
}
#endif

#endif /* __GDBSTUB_H__ */
