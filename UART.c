#include "lib/include.h"

extern void Configurar_UART7(void)
{
    SYSCTL->RCGCUART  = (1<<7);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<4);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOE->AFSEL = (1<<0) | (1<<1);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOE->PCTL = (GPIOE->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOE->DEN = (1<<0) | (1<<1);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART7->CTL = (0<<9) | (0<<8) | (0<<0);
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 (FREC) / (16 * 9600 (Baud rate)) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */

   


    UART7->IBRD = 54;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART7->FBRD = 17;
    //  UART Line Control (UARTLCRH) pag.916
    UART7->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART7->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART7->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 );
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0 );
    UART0->DR = c;
}
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}
//Experimento 2
//El envio es su nombre  (rave) 
// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r)
