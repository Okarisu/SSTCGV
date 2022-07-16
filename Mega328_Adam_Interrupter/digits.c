#include <avr/io.h>
#include "digits.h"

// functions to get N-th digit from a numeric value as a char

char dig8_0(uint8_t num)
{
    return num%10+48;
}

char dig8_1(uint8_t num)
{
    return (num%100)/10+48;
}

char dig8_2(uint8_t num)
{
    return (num%1000)/100+48;
}

char dig16_0(uint16_t num)
{
    return num%10+48;
}

char dig16_1(uint16_t num)
{
    return (num%100)/10+48;
}

char dig16_2(uint16_t num)
{
    return (num%1000)/100+48;
}

char dig16_3(uint16_t num)
{
    return (num%10000)/1000+48;
}

char dig32_0(unsigned long num)
{
    return num%10+48;
}

char dig32_1(unsigned long num)
{
    return (num%100)/10+48;
}

char dig32_2(unsigned long num)
{
    return (num%1000)/100+48;
}

char dig32_3(unsigned long num)
{
    return (num%10000)/1000+48;
}

char dig32_4(unsigned long num)
{
    return (num%100000)/10000+48;
}

char dig32_5(unsigned long num)
{
    return (num%1000000)/100000+48;
}

char dig32_6(unsigned long num)
{
    return (num%10000000)/1000000+48;
}

char dig32_7(unsigned long num)
{
    return (num%100000000)/10000000+48;
}
