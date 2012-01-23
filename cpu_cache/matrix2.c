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
#include <stdio.h>
#include <limits.h>
#define X 0x400
#define PAGE_OFFSET_MASK 0x3f
#define PAGE_MASK (~PAGE_OFFSET_MASK)
#define PAGE_SIZE_LOG4 6
#define PAGES_PER_LINE_LOG2 4
volatile int  matr1[X*X];
volatile int  matr2[X*X];
volatile int  matr3[X*X];
#define GET_ADDR( x, y) ( (y & PAGE_MASK) << (PAGES_PER_LINE_LOG2 + PAGE_SIZE_LOG4) ) + ( ( x & PAGE_MASK ) << PAGE_SIZE_LOG4 ) + ( ( y & PAGE_OFFSET_MASK ) << PAGE_SIZE_LOG4 ) + ( x & PAGE_OFFSET_MASK )
void mul(int *matr1,int *matr2,int *matr3)
{
	int i,o,j;
	for(i=0; i<X; i++){
		for(j=0; j<X; j++)
		{
			register int val=0;
			for(o=0; o<X; o++)
			{	
				val+=matr1[GET_ADDR(o,j)] * matr2[GET_ADDR(i,o)];
			}
			matr3[GET_ADDR(i,j)]=val;
		}
	}
}



void test1()
{
	int passed = 1;
	int i,o,j;
	int res;
	memset((void *)matr1 , 0 , X * X * sizeof(int));
	memset((void *)matr2 , 0 , X * X * sizeof(int));
	for(i=0; i < X; i++)
	{
		matr2[GET_ADDR(i,i)]=i+1;
			
	}
	for(i = 0; i < X; i++ )
		for(j = 0; j < X; j++ )
			matr1[GET_ADDR(i,j)]=1;	
	mul( matr1, matr2, matr3);
	for(i = 0; i < X; i++ )
		for(j = 0; j < X; j++ )
		{
			if(matr3[ GET_ADDR( i, j) ] != i+1) passed = 0;			
		}
	if(passed)
		puts("test1 passed \n");	
	else
		puts("test1 failed \n");	
}

void test2()
{
	int passed = 1;
	int i,o,j;
	int res;
	memset((void *)matr1 , 0 , X * X * sizeof(int));
	memset((void *)matr2 , 0 , X * X * sizeof(int));
	memset((void *)matr3 , 0 , X * X * sizeof(int));
	for(i=0; i < X; i++)
	{
		matr1[GET_ADDR(i,i)]=1;
		matr2[GET_ADDR(i,i)]=1;		
	}
	mul( matr1, matr2, matr3);
	for(i = 0; i < X; i++ )
		for(j = 0; j < X; j++ )
		{
			if(i == j && matr3[GET_ADDR( i, j)] != 1) passed =0;
			if(i != j && matr3[GET_ADDR( i, j)] != 0) passed =0;
		}
	if(passed)
		puts("test2 passed \n");	
	else
		puts("test2 failed \n");	
}




#define RUN_TEST 1

int main()
{
#if RUN_TEST	
	test1();
	test2();	
#else	
	memset((void *)matr1,1,X*X*sizeof(int));
	memset((void *)matr2,2,X*X*sizeof(int));
	mul(matr1,matr2,matr3);	
#endif
return 0;
}
