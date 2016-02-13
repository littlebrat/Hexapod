#include "PacketHandler.h"
#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define FIRST_CHAR 222
#define SECOND_CHAR 230
#define THIRD_CHAR 222
#define TERMINAL_CHAR 233
#define TIMEOUT 1000

uint8_t PacketHandler::receive()
{
    bool terminal_found = false;
    uint8_t header_bytes_found = 0;
    uint8_t tracker = 0;
    uint8_t expected_length = 0;
    unsigned long start_time = millis();

    Leg** legs_ptr;

    while(!terminal_found && millis() - start_time < TIMEOUT)
    {
        if(Serial.available()) {
            uint8_t incoming_byte = Serial.read();
            Serial.write(incoming_byte);
            // This function returns the number of header bytes found so far
            header_bytes_found = this->findHeader(incoming_byte, header_bytes_found);
            if(header_bytes_found == 3)
            {
                if(tracker == 0){
                    // Returns a pointer to the Leg array
                    // (initially contains empty Leg objects)
                    expected_length = findLength(incoming_byte) * 3;
                    size_command_list = expected_length / 3;
                    legs_ptr = this->findFlags(size_command_list, incoming_byte);
                }
                else if(incoming_byte == TERMINAL_CHAR)
                {
                    Serial.write(251);
                    // If the packet does not have expected length
                    if(tracker != expected_length){
                        return 0;
                    }
                    else{
                        command_list = legs_ptr;
                        return 1;
                    }
                }
                // If the packet length exceeds the expected
                else if( tracker > expected_length){
                    return 0;
                }
                if(tracker > 0){
                    // This function updates the respective angle of the respective
                    // leg, given by incoming_byte
                    // The leg to be updated is given by 'floor(tracker / 3)'
                    // The joint to be updated is given by 'tracker % 3'
                    this->updateLeg(incoming_byte, tracker, legs_ptr);
                }
                // Increment tracker
                tracker++;
            }
        }
    }
    return 0;
}

uint8_t PacketHandler::findHeader(uint8_t incoming_byte, uint8_t header_bytes_found)
{
    /*
    This method will receive the incoming byte from serial, and the number of
    bytes of the header found until this this point.
    It should return the header bytes found if everything runs as expected, and
    -1 otherwise.
    */
    if(header_bytes_found == 3)
    {
        return 3;
    }
    else if(header_bytes_found == 0 && incoming_byte == FIRST_CHAR)
    {
        return 1;
    }
    else if(header_bytes_found == 1 && incoming_byte == SECOND_CHAR)
    {
        return 2;
    }
    else if(header_bytes_found == 2 && incoming_byte == THIRD_CHAR)
    {
        return 3;
    }
    else return -1;
}

Leg** PacketHandler::findFlags(uint8_t length, uint8_t incoming_byte)
{
    /*
    This method receives the incoming byte for the header, and makes bitwise
    operations to find the leg indexes that are active in the incoming packet.
    It returns a pointer to an array of active leg objects.
    */
    Leg* legs[length];
    for(int i = 0; i < 8; i++){
        if(incoming_byte & (1 << i) != 0){
            // Create Leg object with id=i and insert it into the legs array.
            Leg myleg(i);
            legs[i] = &myleg;
        }
    }
    Leg** ptr = legs;
    return legs;
}

uint8_t PacketHandler::findLength(uint8_t incoming_byte)
{
    /*
    This method receives the header byte and returns the number of active legs.
    */
    uint8_t counter = 0;
    for(int i = 0; i < 8; i++){
        if(incoming_byte & (1 << i) != 0){
            counter++;
        }
    }
    return counter;
}

void PacketHandler::simulate_commands() {
    for(uint8_t i = 0; i < size_command_list; i++) {
        command_list[i]->simulate_actuate();
    }
}

void PacketHandler::updateLeg(uint8_t incoming_byte, uint8_t tracker, Leg** legs_ptr)
{
    uint8_t index = tracker - 1;
    if(index % 3 == 0){
        legs_ptr[index/3]->setShoulder(incoming_byte);
    }
    else if(index % 3 == 1){
        legs_ptr[index/3]->setElbow(incoming_byte);
    }
    else if(index % 3 == 2){
        legs_ptr[index/3]->setFoot(incoming_byte);
    }
}