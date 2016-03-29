/*
 * packet.c
 *
 *  Created on: Feb 21, 2016
 *      Author: Parker
 */

#include "packet.h"


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define functions to send packets
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Base function to send all packets (or strings to setup WiFi module)
 *********************************************************************************************/
uint8_t sendPacket( char* packet, uint8_t length ) {
	int i;
	for( i=0; i < length; i++ ) {
		// Wait for data register to be empty before adding the next char
		while( !(WIFI_USART->SR & 0x00000040) );

		// Put into TX register
		USART_SendData( WIFI_USART, *packet );
		packet++;
	}

	// WiFi module expects \r\n at the end of every string
	while( !(WIFI_USART->SR & 0x00000040) );
	USART_SendData( WIFI_USART, '\r' );

	while( !(WIFI_USART->SR & 0x00000040) );
	USART_SendData( WIFI_USART, '\n' );

	return SUCCESS;
}


/*********************************************************************************************
 * Send Ack
 *********************************************************************************************/
uint8_t sendAck( Header* header, uint8_t success ) {

	// Fill Ack struct
	Ack* ack = pvPortMalloc( sizeof(Ack) );
	ack->success = success;

	// Update the length of the packet in the header
	header->length = (HEADER_LENGTH + ACK_LENGTH);

	// Convert structs into strings
	char* header_char = (char*) header;
	char* ack_char = (char*) ack;

	// Package header and payload together
	char* ack_packet = pvPortMalloc( sizeof((uint8_t)(header->length)) );
	pack( ack_packet, header_char, 0 );
	pack( ack_packet, ack_char, HEADER_LENGTH );

	// Send packet
	sendPacket( "AT+CIPSEND=6", 12 );
	while(received_string[0] != 'O');
	uint8_t result = sendPacket( ack_packet, header->length );

	// Free variables
	vPortFree(ack);
	vPortFree(ack_packet);

	return result;
}


/*********************************************************************************************
 * Send Ping
 *********************************************************************************************/
uint8_t sendPing( Header* header ) {

	// Update the length of the packet in the header
	header->length = (HEADER_LENGTH + PING_LENGTH);

	// Convert structs into strings
	char* header_char = (char*) header;

	// Package header and payload together
	char* ping_packet = pvPortMalloc( sizeof((uint8_t)(header->length)) );
	pack( ping_packet, header_char, 0 );

	// Send packet
	sendPacket( "AT+CIPSEND=5", 12 );
	while(received_string[0] != 'O');
	uint8_t result = sendPacket( ping_packet, header->length );

	// Free variables
	vPortFree(ping_packet);

	return result;
}


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Define functions to handle packets
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Base function to handle all packets
 *********************************************************************************************/
PacketResult handlePacket( char* packet ) {

	// Extract the header
	Header* header = pvPortMalloc( sizeof(Header) );
	char* header_char = (char*) header;
	unpack( packet, header_char, 0 );

	// Convert string back to Header struct
	header = (Header*) header_char;

	// Create packet result struct
	PacketResult packet_result;
	packet_result.type = header->type;

	// Sort by mode of operation
	if( (header->mode == allModes) || (header->mode == my_mode ) ) {

		// Based on packet type, call the correct handle function
		switch( header->type ) {
			case ack:
				packet_result.result = handleAck( header, packet );
				break;
			case ping:
				packet_result.result = handlePing( header, packet );
				break;
			case setMode:
				// TODO: Add handler
				break;
			case powerConsumption:
				// TODO: Add handler
				break;
			default:
				// If there is no handle function, send back negative Ack
				header->dest = header->addr;
				header->addr = MY_ADDR;
				header->mode = allModes;
				header->type = ack;
				sendAck( header, FAILURE );
				packet_result.result = FAILURE;
				break;
		}
	} else {
		packet_result.result = WRONG_MODE;
	}

	// Free variables
	vPortFree( header );

	return packet_result;
}


/*********************************************************************************************
 * Handle Ack
 *********************************************************************************************/
uint8_t handleAck( Header* header, char* packet ) {

	// Extract the pay load
	Ack* ack = pvPortMalloc( sizeof(Ack) );
	char* ack_char = (char*) ack;
	unpack( packet, ack_char, HEADER_LENGTH );

	// Convert string back to Ack struct
	ack = (Ack*) ack_char;

	// Indicate success or failure from packet
	uint8_t result = ack->success;

	// Free variables
	vPortFree( ack );

	return result;
}


/*********************************************************************************************
 * Handle Ping
 *********************************************************************************************/
uint8_t handlePing( Header* header, char* packet ) {

	// Extract the pay load
	Ping* ping = pvPortMalloc( sizeof(Ping) );
	char* ping_char = (char*) ping;
	unpack( packet, ping_char, HEADER_LENGTH );

	// Convert string back to Ping struct
	ping = (Ping*) ping_char;

	// Restructure header to send back ack
	header->dest = header->addr;
	header->addr = MY_ADDR;
	header->mode = allModes;
	header->type = ack;

	// Send Ack
	uint8_t result = sendAck( header, SUCCESS );

	// Free variables
	vPortFree( ping );

	return result;
}


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Helper Functions
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*********************************************************************************************
 * Package string into container
 *********************************************************************************************/
void pack( char* container, char* field, uint8_t start ) {

	// Iterate through field, copying each char into the container
	// Note: By adding 48, the integer value is represented as its ascii counterpart
	int i;
	for( i=0; i<=sizeof(field); i++) {
		container[start + i] = field[i] + 48;
	}
}


/*********************************************************************************************
 * Unpack strings from the container
 *********************************************************************************************/
void unpack( char* container, char* field, uint8_t start ) {

	// Note: By subtracting 48, the ascii is converted back to the correct uint
	// Note: Each packet is of the form: +IPD,(length):(packet)

	// Begin by creating an offset to the actual packet
	int offset = 6;
	while( container[offset] != ':' ) {
		offset++;
	}
	offset++;

	// Iterate through container, copying each char into the field
	int i;
	for( i=0; i<=sizeof(field); i++) {
		field[i] = container[start + offset + i] - 48;
	}
}


/*********************************************************************************************
 * USART IRQ Handler for WiFi module
 *********************************************************************************************/
void USART3_IRQHandler( void ) {

	// Make sure USART recieve interrupt flag was set
	if( USART_GetITStatus( WIFI_USART, USART_IT_RXNE ) ) {

		// Counter to track packet length
		static uint8_t counter = 0;

		// Character retrieved from data register
		char t = WIFI_USART->DR;

		// Check if string has ended or it exceeded the maximum packet length
		if( (t != '\n') && (counter < MAX_LENGTH)  && (t != '~') ) {
			received_string[counter] = t;
			counter++;
		} else {

			// Reset counter
			counter = 0;

			// If received_string is a packet, put it in the queue
			if( received_string[0] == '+' ) {
				xQueueSendToBackFromISR( xPacketQueue, &received_string, pdFALSE);
				process_packet = TRUE;
			}

			// Clear received_string
			received_string[0] = '\n';
		}
	}
}


/*********************************************************************************************
 * Copy original string to a new copy
 *********************************************************************************************/
void copyString( volatile char* original, char* new, uint8_t length ) {

	// Iterate through string and copy each char
	int i;
	for( i=0; i<=length; i++ ) {
		new[i] = original[i];
	}
}
