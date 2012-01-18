#include "common.h"
#include "descriptor_tables.h"
#include "monitor.h"

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
#define LCM_A 69069
#define LCM_C 5

u32int random; 


void main(){
	
	init_idt();
	random = *((int*)0xB8000);
	monitor_clear();	
	monitor_write("TASK7 STARTED! \n");	
	bad_loop();
}
void bad_loop()
{
	int trash;
	while(1){
		trash = *((int*)random);
		random = LCM_A + LCM_C * random;
	}
}

	
	
	


void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (u32int)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate( 0, (u32int)isr0 );
    idt_set_gate( 1, (u32int)isr1 );
    idt_set_gate( 2, (u32int)isr2 );
    idt_set_gate( 3, (u32int)isr3 );
    idt_set_gate( 4, (u32int)isr4 );
    idt_set_gate( 5, (u32int)isr5 );
    idt_set_gate( 6, (u32int)isr6 );
    idt_set_gate( 7, (u32int)isr7 );
    idt_set_gate( 8, (u32int)isr8 );
    idt_set_gate( 9, (u32int)isr9 );
    idt_set_gate(10, (u32int)isr10);
    idt_set_gate(11, (u32int)isr11);
    idt_set_gate(12, (u32int)isr12);
    idt_set_gate(13, (u32int)isr13);
    idt_set_gate(14, (u32int)isr14);
    idt_set_gate(15, (u32int)isr15);
    idt_set_gate(16, (u32int)isr16);
    idt_set_gate(17, (u32int)isr17);
    idt_set_gate(18, (u32int)isr18);
    idt_set_gate(19, (u32int)isr19);
    idt_set_gate(20, (u32int)isr20);
    idt_set_gate(21, (u32int)isr21);
    idt_set_gate(22, (u32int)isr22);
    idt_set_gate(23, (u32int)isr23);
    idt_set_gate(24, (u32int)isr24);
    idt_set_gate(25, (u32int)isr25);
    idt_set_gate(26, (u32int)isr26);
    idt_set_gate(27, (u32int)isr27);
    idt_set_gate(28, (u32int)isr28);
    idt_set_gate(29, (u32int)isr29);
    idt_set_gate(30, (u32int)isr30);
    idt_set_gate(31, (u32int)isr31);

    idt_flush((u32int)&idt_ptr);
}

void idt_set_gate(u8int num, u32int base)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = 0x08;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = 0x8E /* | 0x60 */;
}
