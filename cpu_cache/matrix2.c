/*
 * =====================================================================================
 *
 *       Filename:  matrix2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07.12.2011 23:28:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Andrey Ivanov (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#define X 0x400
#define Y 0x400
#define Z 0x400
#define PAGE_MASK 0xfffffc0
#define PAGE_OFFSET_MASK 0x3f
#define PAGE_SIZE_LOG 6
volatile int  matr1[X*Y];
volatile int  matr2[Y*Z];
volatile int  matr3[X*Z];
#define GET_ADDR(x,y,bX) ((y& PAGE_MASK)<<bX<<PAGE_SIZE_LOG)+((x& PAGE_MASK)<<PAGE_SIZE_LOG)+((y&PAGE_OFFSET_MASK)<<PAGE_SIZE_LOG)+(x&PAGE_OFFSET_MASK)
int main()
{
int i,o,j;
int res;
memset((void *)matr1,1,X*Y*sizeof(int));
memset((void *)matr2,2,Y*Z*sizeof(int));
for(i=0;i<X;i++)
{
	for(j=0;j<Z;j++)
	{
		register int val=0;
		for(o=0;o<Y;o++)
		{
			val+=matr1[GET_ADDR(i,o,4)]*matr2[GET_ADDR(o,j,4)];
		}
		matr3[GET_ADDR(i,j,4)]=val;
	}
	}

	
return 0;
}
