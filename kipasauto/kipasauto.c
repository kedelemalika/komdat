#include <mega32.h>

#include <delay.h>

// Alphanumeric LCD functions
#include <alcd.h>
//sprintf
#include <stdio.h>

// Declare your global variables here
unsigned int adcout;
char temp[16];
int i;
// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE 0x40

// Membaca hasil ADC
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}
void slowprint(char *str)
{
for(i=0;str[i]!='\0';i++)
    {
    lcd_putchar(str[i]);
    delay_ms(10);    
    }    
}
void main(void)
{
// Declare your local variables here
char ucapana[]="Halo...";
char ucapanb[]="Kipas ini siap\nMelayani anda...";

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=In Bit1=Out Bit0=Out 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (0<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=T Bit1=0 Bit0=1 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (1<<PORTB0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 15,625 kHz
// Mode: Fast PWM top=0xFF
// OC0 output: Non-Inverted PWM
// Timer Period: 16,384 ms
// Output Pulse(s):
// OC0 Period: 16,384 ms Width: 0 us
TCCR0=(1<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0x00;

// ADC initialization
// ADC Clock frequency: 250,000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTC Bit 0
// RD - PORTC Bit 1
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 16
lcd_init(16);

//ucapan selamat datang
lcd_gotoxy(0,0);
slowprint(ucapana);
delay_ms(100);
lcd_clear();
lcd_gotoxy(0,0);
slowprint(ucapanb);
delay_ms(100);
lcd_clear();
//mulai
lcd_gotoxy(0,0);
lcd_putsf("Suhu : \nSpeed: ");
while (1)
      {
      // Place your code here
        lcd_gotoxy(7,0);
        adcout=read_adc(0)/2-1;
        sprintf(temp,"%03i%cC",adcout,0xdf);
        lcd_puts(temp);
        if(adcout>35)
        {
            lcd_gotoxy(7,1);
            lcd_putsf("||||");
            OCR0=255;
        }
        else if(adcout>30)
        {
            lcd_gotoxy(7,1);
            lcd_putsf("||| ");
            OCR0=189;
        }
        else if(adcout>25)
        {
            lcd_gotoxy(7,1);
            lcd_putsf("||  ");
            OCR0=124;
        }
        else if(adcout>20)
        {
            lcd_gotoxy(7,1);
            lcd_putsf("|   ");
            OCR0=63;
        }
        else
        {
            lcd_gotoxy(7,1);
            lcd_putsf("    ");
            OCR0=0x00;
        }
      }
}

