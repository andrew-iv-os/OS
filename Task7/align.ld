SECTIONS
{
	. = 0x7e00;
	.text : {*(.text). = ALIGN(0x200);
	}
	.data : {	*(.data). = ALIGN(0x200);
	
	}
.bss : { *(.bss). = ALIGN(0x200);
	}
	
}
