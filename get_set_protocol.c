#ifndef __PROTO_H_
#define __PROTO_H_

#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// Constants for OPCODE and TYPE values
#define GET_OPCODE 'G'
#define SET_OPCODE 'S'

#define INT_TYPE 'I'
#define CHAR_TYPE 'C'
#define STR_TYPE 'S'

// Function to calculate the checksum
unsigned char calculate_checksum(unsigned char *packet, int length) {
    int checksum = 0;
    for (int i = 1; i < length - 1; i++) {
        checksum += packet[i];
    }
    return (~checksum) & 0xFF;
}

// Function to encode a packet
void encode_packet(char opcode, char data_type, int length, char *value, unsigned char *packet) {
    packet[0] = '$'; // Start of Frame
    packet[1] = opcode;
    packet[2] = data_type;
    packet[3] = length;

    // Copy value to the packet, handle escape characters
    int j = 4;
    for (int i = 0; i < length; i++) {
        if (value[i] == '$') {
            packet[j++] = '/';
        }
        packet[j++] = value[i];

    }

    // Calculate checksum
    packet[j] = calculate_checksum(packet, j + 1);
    printf("\n%s()-",__FUNCTION__);
}

// Function to decode a packet
void decode_packet(unsigned char *packet, int length) {
    // Verify checksum
    unsigned char received_checksum = packet[length - 1];
    unsigned char calculated_checksum = calculate_checksum(packet, length);

    if (received_checksum != calculated_checksum) {
        printf("Checksum error! Packet may be corrupted.\n");
        return;
    }

    char opcode = packet[1];
    char data_type = packet[2];
    int data_length = packet[3];

    // Extract value from the packet, handle escape characters
    char value[data_length + 1];
    int j = 0;
    for (int i = 4; i < length - 1; i++) {
        if (packet[i] == '/' && packet[i + 1] == '$') {
            value[j++] = '$';
            i++;
        } else {
            value[j++] = packet[i];
        }
    }
    value[j] = '\0';

    // Using atoi to convert string to integer
    int number = atoi(value);

    if(data_type==STR_TYPE)
        printf("\n%s()-OPCODE: %c, TYPE: %c, LENGTH: %d, VALUE: %s\n",__FUNCTION__, opcode, data_type, data_length, value);

    else if(data_type==INT_TYPE)
        printf("\n%s()-OPCODE: %c, TYPE: %c, LENGTH: %d, VALUE: %d\n",__FUNCTION__, opcode, data_type, data_length, number);
}

int main() {
    char set_packet[500]={0};
    char get_packet[500]={0};

    // Encoding a SET packet
    encode_packet(SET_OPCODE, STR_TYPE, strlen("Manikanta"), "Manikanta", set_packet);
    printf("Encoded SET Packet: %s\n", set_packet);

    // Decoding the SET packet
    decode_packet(set_packet, strlen("Manikanta") + 5);

    // Encoding a GET packet
    encode_packet(GET_OPCODE, INT_TYPE, strlen("42"), "42", get_packet);
    printf("\nEncoded GET Packet: %s\n", get_packet);

    // Decoding the GET packet
    decode_packet(get_packet, strlen("42") + 5);

    return 0;
}

#endif // __PROTO_H_
