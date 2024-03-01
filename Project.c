#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

volatile unsigned char Sec_Units = 0;
volatile unsigned char Sec_Tenth = 0;
volatile unsigned char Min_Units = 0;
volatile unsigned char Min_Tenth = 0;
volatile unsigned char Hour_Units = 0;
volatile unsigned char Hour_Tenth = 0;


void StopWatch (void)
	{
	displaytime();

			if (Sec_Units == 10)
			{
				Sec_Tenth++;
				Sec_Units = 0;
			}

			if (Sec_Tenth == 6)
			{
				Sec_Tenth = Sec_Units = 0;
				Min_Units++;
			}

			if (Min_Units == 10)
			{
				Min_Units=0;
				Min_Tenth++;
			}

			if (Min_Tenth == 6)
			{
				Min_Tenth = Min_Units = 0;
				Hour_Units++;
			}
			if (Hour_Units == 10)
			{
				Hour_Units = Min_Tenth = Min_Units = 0 ;
				Hour_Tenth++;
			}
			if (Hour_Tenth == 10)
			{
				Hour_Tenth = Hour_Units = Min_Tenth = Min_Units = 0;
			}

}

void displaytime(void)
{
	PORTA = (1<<PA0);
	PORTC = (PORTC & 0xF0) | (Sec_Units & 0x0F);
	_delay_us(5);

	PORTA = (1<<PA1);
	PORTC = (PORTC & 0xF0) | (Sec_Tenth & 0x0F);
	_delay_us(5);

	PORTA = (1<<PA2);
	PORTC = (PORTC & 0xF0) | (Min_Units & 0x0F);
	_delay_us(5);

	PORTA = (1<<PA3);
	PORTC = (PORTC & 0xF0) | (Min_Tenth & 0x0F);
	_delay_us(5);

	PORTA = (1<<PA4);
	PORTC = (PORTC & 0xF0) | (Hour_Units & 0x0F);
	_delay_us(5);

	PORTA = (1<<PA5);
	PORTC = (PORTC & 0xF0) | (Hour_Tenth & 0x0F);
	_delay_us(5);

}


ISR(TIMER1_COMPA_vect)
{
	Sec_Units++;
}


ISR(INT0_vect)
{
	Sec_Units = 0;
	Sec_Tenth = 0;
	Min_Units = 0;
	Min_Tenth = 0;
	Hour_Units = 0;
	Hour_Tenth = 0;
}


void Timer1_init(void)
{
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS12) | (1 << CS10);
	TCNT1 = 0;
	TIMSK |= (1 << OCIE1A);
	OCR1A = 976;
}


ISR(INT1_vect)
{
	TCCR1B = 0;
}

ISR(INT2_vect)
{
	TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);
}


void INT1_init(void)
{
	DDRD &= ~(1<<PD3);
	MCUCR |= (1<<ISC11) | (1<<ISC00);
	GICR |= (1<<INT1);
}

void INT0_init(void)
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR |= (1<<INT0);
}


void INT2_init(void)
{
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2);
	GICR |= (1<<INT2);

}


int main(void)
{

	DDRC = 0x0F;
	DDRA = 0x0F;
	PORTC = 0x00;
	PORTA = 0x00;
	sei();
	Timer1_init();
	INT0_init();
	INT2_init();
	INT1_init();
	while(1)
	{
		StopWatch();
	}

}

