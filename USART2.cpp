# define F_CPU 1000000UL
#include <avr/io.h>
# include <util/delay.h>
# define Buadrate 9600
# define ubrr_value ((F_CPU/(Buadrate * 2UL))-1)
void lcd_init();
void disp_cmd(unsigned int cmd);
void disp_data(int cmd);
void disp_num(unsigned int num);
void disp_string(  char *str);
void usart_init();
char usart_rec();
void usart_send(char send);
int main(void)
{
	DDRD=0XFE;
	DDRB=0XFF;
	int i,j;
	lcd_init();
	usart_init();
	disp_cmd(0X86);
	disp_string("WELCOME");
	_delay_ms(2);
	disp_cmd(0X01);
	_delay_ms(10);
	
	while (1)
	{
		disp_cmd(0X80);
		for (i=0;i<=15;i++)
		{
		
		
		char val=usart_rec();
		usart_send(val);
		
		disp_data(val);
		
		_delay_ms(100);
		}
		disp_cmd(0XC0);
				for (j=0;j<=15;j++)
				{
					
					
					char val=usart_rec();
					usart_send(val);
					
					disp_data(val);
					
					_delay_ms(100);
				}
				_delay_ms(2);
				disp_cmd(0X01);
				_delay_ms(10);
		
	}
	return 0;
}
void disp_cmd(unsigned int cmd)
{
	PORTB=((cmd & 0XF0)+0X04);
	_delay_ms(1);
	PORTB=PORTB - 0X04;
	PORTB=(((cmd<<4)& 0XF0)+0X04);
	_delay_ms(1);
	PORTB=PORTB-0X04;
	
}
void disp_data( int cmd)
{
	PORTB=((cmd  & 0XF0)+0X05);
	_delay_ms(1);
	PORTB=PORTB - 0X04;
	PORTB=(((cmd <<4)& 0XF0)+0X05);
	_delay_ms(1);
	PORTB=PORTB-0X04;
	
}

void disp_string( char *str)
{
	int i=0;
	while (str[i]!='\0')
	{
		disp_data (str[i]);
		i++;
		
	}
}

void disp_num(unsigned int num)
{
	int a=0;
	if (num==0)
	{
		disp_data(48);
	}
	disp_cmd (0X04);
	while (num!=0)
	{
		a=num % 10;
		disp_data(a+48);
		num=num/10;
	}
	disp_cmd(0X06);
}
void lcd_init()
{
	disp_cmd(0X02);
	disp_cmd(0X28);
	disp_cmd(0X0C);
	disp_cmd(0X06);
}
void usart_init()
{
	
	UCSRB=0X18;
	UCSRC=0X8E;
	UBRRL=(ubrr_value);
	UBRRH=(ubrr_value>>8);
}

char usart_rec()

{
	while((UCSRA & 0X80)==0);
	return UDR;
}
void usart_send(char send)

{
	while((UCSRA & 0X20)==0);
	UDR=send;
}

