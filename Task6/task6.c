void print(){
	// while(1);
	/* *(short*)0xB8000 = 0x0700 + 'M';
	return;
	 
	
*/
short *mem;
mem = 0xB8000;
int i;
for( i =0 ;i <0x400 ; i++)
	{
		*mem = 0x0733;
		mem++;		
	};
	


}
