// #include"mp3tf16p.h"

// MP3Player mp3(10,11);


// void setup() {
//   // put your setup code here, to run once:

//   // Serial.begin(9600);
//   // mp3.initialize();
//   // mp3.playTrackNumber(1, 20);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
  
//   mp3.serialPrintStatus(MP3_ALL_MESSAGE);

//   Serial.begin(9600);
//   mp3.initialize();
//   mp3.playTrackNumber(1, 30);
// }

#include "BirdChaser.h"
// Create the BirdChaser object
BirdChaser birdChaser(10, 11, 4);  // Use pins 10, 11 for MP3, and pin 4 for Tempo/Loop button

void setup() {
    Serial.begin(9600);
    birdChaser.initialize();  // Initialize the bird chaser system
}

void loop() {
    birdChaser.run(true);  // Main loop running bird chaser actions
}