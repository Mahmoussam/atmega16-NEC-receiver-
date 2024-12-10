/*
 * IR_types.h
 *
 * Created: 12/9/2024 9:38:33 PM
 *  Author: princ
 */ 
#ifndef IR_TYPES
#define IR_TYPES

#define THRESHOLD_WIDTH_TICKS 25
#define IS_ONE(TICKS) (TICKS > 25) 

enum RECEIVING_STATE{
	CORRECT // packet received and verified
	,CORRUPTED //corrupted packet
	,STARTED //receiving undergoing
	,NONE 
};
union PACKET{
	struct {
		uint8_t address :8;
		uint8_t command :8;
	};
	uint16_t data;
};

#endif
