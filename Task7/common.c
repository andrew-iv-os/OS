// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.
#include "common.h"

u8int heap[0x10000];
int heap_pointer = 0;

void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

// Andrey Ivanov
void atoi(u8int *str,int value)
{	
    int i = 0;
    while (value > 0)
    {
        *str = '0' + value%10;
        value /= 10;
        str++;
    }
    *str = 0;
	return ;
}



// Andrey Ivanov
u8int *malloc(u32int size)
{
	heap_pointer+=size;
	return  heap+(heap_pointer-size);
}


