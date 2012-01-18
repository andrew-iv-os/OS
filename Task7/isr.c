#include "common.h"
#include "isr.h"
#include "monitor.h"

void isr13_handler(registers_t regs)
{
    monitor_write("13h interrupt error_code: ");
    monitor_write_dec(regs.err_code);
    monitor_put('\n');
    monitor_put(' CS:');
    monitor_write_hex(regs.cs);
    monitor_put('\n');
    monitor_put(' EIP:');
    monitor_write_hex(regs.eip);    
    monitor_put('\n');
}
