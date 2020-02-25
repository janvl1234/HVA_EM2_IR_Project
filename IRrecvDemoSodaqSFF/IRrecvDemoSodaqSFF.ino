/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include "IRremote.h"

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, HIGH); // off
    // wait 10 sec to open the serial monitor
    while(!SerialUSB && (millis() < 10000));
    
    SerialUSB.begin(9600);
    // In case the interrupt driver crashes on setup, give a clue
    // to the user what's going on.
    SerialUSB.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver
    SerialUSB.println("Enabled IRin");
}

void loop() {
    if (irrecv.decode(&results)) {
        SerialUSB.println(results.value, HEX);
        if (results.value == 0x0061616e) {
            // Turn LED On
            digitalWrite(LED_BLUE, LOW);
        } else if (results.value == 0x00756974) {
            // Turn LED Off
            digitalWrite(LED_BLUE, HIGH);
        }
        irrecv.resume(); // Receive the next value
    }
    delay(100);
}
