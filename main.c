/*
 * NEC_Decoder_inverted.c
 *
 * Created: 12/9/2024 9:44:49 AM
 * Author : princ
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "LCD/ALCD.h"
#include "IR/IR_types.h"

/**
*	SetUp INT2 on Falling Edge trigger
*	connected to the IR receiver signal.
*/
void setup_INT2();
/**
* Starts Timer2 with highest prescalar to measure 'width' of bits
*/
void start_TIM2();
/**
* Stops timer2 ticking
*/
void stop_TIM2();
/** 
*	Set's TIM2 overflow interrupt
*/
void init_TIM2_TOI();
/**
*	A call back whenever a packet is received and decoded with success
*/
void onPacketReceivedCallBack(uint8_t is_rep);


//IR variables
volatile union PACKET ir_packet;
volatile enum RECEIVING_STATE rec_state = NONE;
volatile uint8_t fedges_cnt = 0;
//LCD variables
char lcd_line1[16] 
	,lcd_line2[16];

int main(void)
{
    /* init peripherals */
	lcd_init(16);
	setup_INT2();
	init_TIM2_TOI();
	// Enable global interrupts
	sei();
	
	DDRA = 1;
	
	//init lcd lines
	snprintf(lcd_line1, sizeof(lcd_line1), "Addr:%04X", 0); // Format Addr:XXXX
	snprintf(lcd_line2, sizeof(lcd_line2), "Cmd:%04X", 0);      // Format Cmd:XXXX
	
	lcd_gotoxy(0,0);
	lcd_puts(lcd_line1);
	
	lcd_gotoxy(0,1);
    lcd_puts(lcd_line2);
	
	while (1) 
    {		
		
		_delay_ms(1000);
    }
}

void setup_INT2() {
	// Configure INT2 for falling edge
	MCUCSR &= ~(1 << ISC2); // Clear ISC2 bit for falling edge detection

	// Enable INT2 in GICR
	GICR |= (1 << INT2);

	
}

ISR(INT2_vect){
	static uint32_t raw_data; 
	static uint8_t cnt;
	fedges_cnt++;
	if(fedges_cnt == 1){
		//starting frame
		start_TIM2();
		
		raw_data = 0;
		rec_state = STARTED;
		
	}
	else if(fedges_cnt == 2){
		//hmmm
		TCNT2 = 0;
		
	}
	else{
		cnt = TCNT2;
		TCNT2 = 0;
		raw_data <<= 1;
		if(IS_ONE(cnt)){
			raw_data |= 1;
		}
		
		if(fedges_cnt == 34){
			//received 33 bits , including the starting frame bit..
			//verify integrity
			uint8_t address_L = (raw_data >> 16) 
			, address_U = (raw_data >> 24)
			, command_L = (raw_data)
			, command_U = (raw_data >> 8);
			if((((address_U | address_L) == 0xFF) && ((command_U | command_L) == 0xFF) )){
				rec_state = CORRECT;
				ir_packet.address = address_U;
				ir_packet.command = command_U;
				/*snprintf(lcd_line1, sizeof(lcd_line1), "Addr:%08X", address_U); // Format Addr:XXXX
				lcd_gotoxy(0,0);
				lcd_puts(lcd_line1);*/
			}
			else{
				rec_state = CORRUPTED;
			}
			stop_TIM2();
			fedges_cnt = 0;
			if(rec_state == CORRECT)
				onPacketReceivedCallBack(0);
		}
	}
	
}
void onPacketReceivedCallBack(uint8_t is_rep){
	if(rec_state != CORRECT)return;
	rec_state = NONE;
	
	if(is_rep){
		//ignore for now..
	}
	else{
		//display command..
		//indicator led
		PORTA = ~PORTA;
		snprintf(lcd_line1, sizeof(lcd_line1), "Addr:0x%04X", ir_packet.address); // Format Addr:XXXX
		snprintf(lcd_line2, sizeof(lcd_line2), "Cmd:0x%04X", ir_packet.command);      // Format Cmd:XXXX
		
		lcd_gotoxy(0,0);
		lcd_puts(lcd_line1);
		
		lcd_gotoxy(0,1);
		lcd_puts(lcd_line2);
		
	}
}

void init_TIM2_TOI(){
	// Enable Timer2 overflow interrupt
	TIMSK |= (1 << TOIE2);
}
void start_TIM2(){
	//set prescalar to 1024
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	//Normal mode
	TCCR2 &= ~((1 << WGM21) | (1 << WGM20));

	TCNT2 = 0; // Reset Timer2 counter to 0
}

void stop_TIM2(){
	TCCR2 = 0;
}
// ISR for Timer2 overflow
ISR(TIMER2_OVF_vect) {
	if(fedges_cnt == 2){
		//repition op code
		rec_state = CORRECT;
	}else{
		rec_state = CORRUPTED;
	}
	//reset states
	fedges_cnt = 0;
	stop_TIM2();
	if(rec_state == CORRECT)
		onPacketReceivedCallBack(1);
}