#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


void Front_Lights_init(void)
{
	DDRA |= (1 << PA0);
	PORTA &= ~(1 << PA0);
}


void Speaker_init(void)
{
	DDRC |= (1 << PC6);
}

void Direction_Motor_init(void)
{
	DDRB |= (1 << PB4); // S1
	DDRB |= (1 << PB5); // S2
}

void Traction_Motor_init(void)
{
  	TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);        //NON Inverted PWM
	TIMSK0 |= (1 << TOIE0);

	// OCR0A = (dutyCycle / 100 ) * 255;

	sei();

	TCCR0B|=(1<<CS00); //PRESCALER=no prescaler MODE 14(FAST PWM)
	DDRB |= (1<<PB3);   //PWM Pins as Out
	DDRB |= (1 << PB1); // S1
	DDRB |= (1 << PB2); // S2
}


ISR(TIMER0_OVF_vect)
{
	// OCR0A = (dutyCycle/100) *255;
}

ISR(PCINT0_vect)
{
	if ((PINA & (1 << PA1)) == 0)
	{
		PORTA &= ~(1 << PA0);
	}
	else
	{
		PORTA |= (1 << PA0);
	}
}

void Senzor_init()
{
	PCMSK0 |= (1 << PCINT1);
	PCICR |= (1 << PCIE0);
}


int main(void) {

	double speed = 0;

	Traction_Motor_init();
	Direction_Motor_init();
	Speaker_init();
	Front_Lights_init();
	USART0_init();
	Senzor_init();

	/* activate button */
	DDRD &= ~(1 << PD6);
	PORTD |= (1 << PD6);

	/*activate debugging led*/
	DDRD |= (1 << PD7);


	/*activate tx*/
	PORTD |= (1 << PD3);

	char c;

	while (1) {

			c = USART0_receive();

			switch(c) {

				case 'w': // Inainte
					PORTD |= (1 << PD7);
					OCR0A = speed;

					PORTB |= (1 << PB1);	/*Moves forward reverse to move backward*/
					PORTB &= ~(1 << PB2);
					break;
				case 's': // Inainte
					OCR0A = speed;
					PORTB |= (1 << PB2);	/*Moves backwards*/
					PORTB &= ~(1 << PB1);
					break;
				case 'l':
					PORTB |= (1 << PB4);
					PORTB &= ~(1 << PB5);
					break;
				case 'r':
					PORTB |= (1 << PB5);
					PORTB &= ~(1 << PB4);
					break;
				case 'h':
					PORTC |= (1 << PC6);
					break;
				case 'c':
					PORTC &= ~(1 << PC6);
					break;
				case 'f':
					PORTA ^= (1 << PA0);
					break;
				case 'z':
					PORTB &= ~(1 << PB2);
					PORTB &= ~(1 << PB1);
					break;
				case 'x':
					PORTB &= ~(1 << PB5);
					PORTB &= ~(1 << PB4);
					break;
				case '0':
					speed = 0;
					break;
				case '1':
					speed = 25;
					break;
				case '2':
					speed = 51;
					break;
				case '3':
					speed = 76;
					break;
				case '4':
					speed = 102;
					break;
				case '5':
					speed = 127;
					break;
				case '6':
					speed = 153;
					break;
				case '7':
					speed = 178;
					break;
				case '8':
					speed = 204;
					break;
				case '9':
					speed = 230;
					break;
				case '-':
					speed = 255;
					break;
				default: // NOP
					PORTB &= ~(1 << PB1);
					PORTB &= ~(1 << PB2);
					PORTB &= ~(1 << PB5);
					PORTB &= ~(1 << PB4);
					PORTC &= ~(1 << PC6);
					break;
			}


		_delay_ms(5);
	}

	return 0;
}
