#include "BirdChaser.h"
// Create the BirdChaser object
BirdChaser birdChaser(10, 11, 5);  // Use pins 10, 11 for MP3, and pin 4 for Tempo/Loop button

void setup() {
    Serial.begin(9600);
    birdChaser.initialize();  // Initialize the bird chaser system
}

void loop() {
    birdChaser.handleButtonPress(); // button pressed to change LoopTimer settings
    birdChaser.handleVolumeButtons();
    birdChaser.run();  // Main loop running bird chaser actions
}