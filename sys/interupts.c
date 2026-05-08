#include "../drivers/drivers.h"
#include "interupts.h"
//pic = programmable interupt controller
#define PIC1 0x20 //io address for pic1
#define PIC2 0xA0 //io address for pic2
// the ports above are for the command
#define PIC1_DATA 0x21 
#define PIC2_DATA 0xA1
#define PIC_INIT 0x11
// data ports send the data 

#define PIC_EOI 0x20 // end of interupt command

void pic_eoi_1() { 
    outb(PIC1, PIC_EOI);
}

void pic_eoi_both() { // if slave pic gets eoi, both get eoi
    outb(PIC2, PIC_EOI);
    outb(PIC1, PIC_EOI);
}

void pic_init() {
    outb(PIC1, PIC_INIT); // sends initialization request
    outb(PIC2, PIC_INIT);

    outb(PIC1_DATA, 0x20); // sets offset 
    outb(PIC2_DATA, 0x28);

    outb(PIC1_DATA, 0x04); // telling PIC1 that there is a second pic
    outb(PIC2_DATA, 0x02); // PIC2 cascade identity, where it is connected to PIC1
    
    outb(PIC1_DATA, 0x01); // 8086 mode
    outb(PIC2_DATA, 0x01); // 8086 mode

    outb(PIC1_DATA, 0x00); // unmasking both PICs, all interupts can go through
    outb(PIC2_DATA, 0x00); 
}

void pic_mask(IRQ interupt) { // to not allow this interupt to go through
    byte port;
    int mask;
    if(interupt > 7) {
        port = 0xA1;
        mask = 1 << (interupt - 8);
    } else {
        port = 0x21;
        mask = 1 << interupt;
    }
    mask |= inb(port);
    outb(port, mask);
}

void pic_unmask(IRQ interupt) {
    byte port;
    int mask;
        if(interupt > 7) {
        port = 0xA1;
        mask = 1 << (interupt - 8);
    } else {
        port = 0x21;
        mask = 1 << interupt;
    }
    mask = ~mask;
    outb(port, mask);
}

void clear_mask() {
    outb(0xA1, 0x00);
    outb(0x21, 0x00);
}

void pic_send_irq(IRQ interupt) {
    if(interupt > 7) {
        outb(0xa0, PIC_EOI);
    }
    outb(0x20, PIC_EOI);
}
