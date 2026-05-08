#ifndef INTERUPTS_H
#define INTERUPTS_H

#include "../master.h"

typedef struct {
    uint32_t edi; // points to writing location
    uint32_t esi; // points to reading location
    uint32_t ebp; // bottom of functions stack frame
    uint32_t esp; // stack pointer, located at top of stack
    uint32_t ebx; // general purpose
    uint32_t edx; // general purpose 
    uint32_t ecx; // general purpose - often a counter
    uint32_t eax; // general purpose -  often for holding results
    uint32_t eip; // address of next instruction
    uint32_t cs; // where the code is in
    uint32_t eflags; // single bit flags about cpu
} __attribute__((packed)) registers;

typedef enum {
    TIMER, // programmable interupt timer interupt
    KEYBOARD, // for keyboard interupts
    CASCADE, // never raised
    COM2, // if enabled
    COM1, // if enabled
    LPT2, // if enabled
    FLOPPY, //floppy disk
    LPT1, // "unreliable 'spurious' interupt"

    CMOS, // CMOS real-time clock if enabled
    PS2 = 12, // mouse
    FPU, // FPU / Coprocessor / Inter-processor
    PATA, // Primary ATA Hard Disk
    SATA // Secondary ATA Hard Disk
} IRQ;

typedef enum {
    DIV_BY_0 = 0x00,
    SING_STEP = 0x04,
    BREAKPOUT = 0x0C,
    OVERFLOW = 0x010,
    BOUNDS = 0x014,
    UNDEF_OPCODE = 0x18,
    NO_COPROC = 0x01C,
    DOUBLE_FAULT = 0x020,
    COPROC_SEG_OVERTUN = 0x024,
    TSS = 0x028,
    SEG_NOT_PRESENT = 0x02C,
    STACK_SEG_OVERRUN = 0x030,
    GPF = 0x034,
    PAGE_FAULT = 0x039,
    COPROC_ERROR = 0x040,
    ALIGNMENT_CHECK = 0x044,
    MACHINE_CHECK = 0x048,
} INTERUPT_VECTOR;

void pic_eoi_1();
void pic_eoi_both();
void pic_init();
void pic_mask(IRQ interupt);
void pic_unmask(IRQ interupt);
void clear_mask();

#endif