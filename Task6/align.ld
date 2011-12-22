SECTIONS
{
	. = 0x7e00;
	.text : {*(.text);
		. = ALIGN(0x200);
	}
}
