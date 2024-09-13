#include "Arduino.h"
#include "HardwareSerial.h"
#include "mp3tf16p.h"    // MP3 library

// Controller class to represent chaser functionalities
class BirdChaser {
  private:
    MP3Player mp3;
    unsigned long previousMillis = 0;
    int currentTrack = 0; // Track 1st on the line
    int maxTrackNumber = 8; // Max track number on the SD card
    int currentSequence = 0;  // Track the current loop sequence
    // tempo/ loop sequence button config
    int buttonPin;  // Button to change LoopSequence
    unsigned long debounceDelay = 500;  // Delay for button debouncing
    unsigned long lastDebounceTime = 0;  // Timestamp for debouncing
    unsigned long lastVolumeDownDebounceTime = 0;
    unsigned long lastVolumeUpDebounceTime = 0;
    // Volume buttons
    int volumeDownPin = 2;  // Pin for volume down button
    int volumeUpPin = 3;    // Pin for volume up button
    

  public:

      // timer sequences to trigger the sound loop
      enum LoopSequence {
          SEQ_10S,
          SEQ_30S,
          SEQ_2M,
          SEQ_5M,
          SEQ_15M,
          SEQ_30M
      };

      struct Config {
          unsigned long activeLoopSequence;
          LoopSequence loopSequence;
          int volumeValue;
          String mp3Tracks[10];  // Adjust for the number of MP3 tracks available
      } config;

      // Set default configuration in case config file is not found
      void setDefaultConfig() {
          config.activeLoopSequence = 10000; // 10 seconds
          config.loopSequence = SEQ_10S;
          config.volumeValue = 30;
      }

      BirdChaser(int mp3TxPin, int mp3RxPin, int buttonPin) : mp3(mp3TxPin, mp3RxPin), buttonPin(buttonPin) {
        // Set button pins as input with internal pull-up resistors
        pinMode(buttonPin, INPUT);
        pinMode(volumeDownPin, INPUT);
        pinMode(volumeUpPin, INPUT);
      }

      // Initialize the app by loading config and preparing the MP3 player
      void initialize() {
          setDefaultConfig();
          // Initialize MP3 player
          mp3.initialize();
          // mp3.setVolume(config.volumeValue);

          Serial.println("Bird Chaser initialized.");
      }

      // Main working loop of the Bird Chaser
      void run(bool disableTimer = false) {
          unsigned long currentMillis = millis();

          if (!disableTimer && (currentMillis - previousMillis >= config.activeLoopSequence)) {
              previousMillis = currentMillis;

              // Loop through all tracks on the SD card and play in sequence
              if (currentTrack % maxTrackNumber == 0) {
                  currentTrack = 0;
              }
              currentTrack++;
              // Play the current track
              mp3.playTrackNumber(currentTrack, config.volumeValue, false);

              // Log the status
              mp3.serialPrintStatus(MP3_ALL_MESSAGE);
          } else if (disableTimer == true) {
              // Loop through all tracks on the SD card and play in sequence
              if (currentTrack % maxTrackNumber == 0) {
                  currentTrack = 0;
              }
              currentTrack++;
              // Play the test track
              mp3.playTrackNumber(currentTrack, config.volumeValue);
          }
      }

      // Function to set the configuration based on LoopSequence
      void setConfig(LoopSequence seq) {
          config.loopSequence = seq;
          switch (seq) {
              case SEQ_30S:
                  config.activeLoopSequence = 30000;   // 30 seconds
                  break;
              case SEQ_2M:
                  config.activeLoopSequence = 120000;  // 2 minutes
                  break;
              case SEQ_5M:
                  config.activeLoopSequence = 300000;  // 5 minutes
                  break;
              case SEQ_15M:
                  config.activeLoopSequence = 900000;  // 15 minutes
                  break;
              case SEQ_30M:
                  config.activeLoopSequence = 1800000; // 30 minutes
                  break;
              default:
                  config.activeLoopSequence = 10000;  // Default to 10 seconds
                  break;
          }
          Serial.print("Loop sequence set to: ");
          Serial.println(config.activeLoopSequence);
      }

      // Handle the button press to change the loop sequence
      void handleButtonPress() {
          int reading = digitalRead(buttonPin);
          if (reading == HIGH && (millis() - lastDebounceTime > debounceDelay)) {

              Serial.print("Button Pressed!");
              lastDebounceTime = millis();  // Update the debounce time

              // Cycle through the loop sequences
              currentSequence++;

              if (currentSequence > SEQ_30M) {  // Reset to the first option after the last one
                  currentSequence = SEQ_10S;
              }
              // Update the config based on the current sequence
              setConfig(static_cast<LoopSequence>(currentSequence));
          }
      }

      // Handle the volume control buttons
      void handleVolumeButtons() {
          unsigned long currentMillis = millis();
          int volumeDownReading = digitalRead(volumeDownPin);
          int volumeUpReading = digitalRead(volumeUpPin);

          // Check if the volume down button is pr
          if (volumeDownReading == HIGH && (currentMillis - lastVolumeDownDebounceTime > debounceDelay)) {
              lastVolumeDownDebounceTime = currentMillis;  // Update the debounce time
              if(config.volumeValue>1){
              config.volumeValue--;
                Serial.print("Volume Down ( - ). current: ");
                Serial.println(config.volumeValue);
              // mp3.volumeDown();  // Decrease volume
              // delay(debounceDelay);  // Debounce delay
              }else{
                Serial.println("Sound is already on MINIMUM!");
              }
          }

          // Check if the volume up button is pressed
          if (volumeUpReading == HIGH && (currentMillis - lastVolumeUpDebounceTime > debounceDelay)) {
              lastVolumeUpDebounceTime = currentMillis;  // Update the debounce time
              if(config.volumeValue<30){
                config.volumeValue++;
                Serial.print("Volume Up ( + ). current: ");
                Serial.println(config.volumeValue);
              // mp3.volumeUp();  // Increase volume
              // delay(debounceDelay);  // Debounce delay
              }else {
                Serial.println("Sound is already on MAX!");
              }
          }
      }
};
