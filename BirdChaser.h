#include "Arduino.h"
#include "HardwareSerial.h"
#include "mp3tf16p.h"    // MP3 library

// Controller class to represent chaser functionalities
class BirdChaser {
  private:
    MP3Player mp3;
    unsigned long previousMillis = 0;
    int currentSequence = 0;  // Track the current loop sequence
    // tempo/ loop sequence button config
    int buttonPin;  // Button to change LoopSequence
    bool buttonPressed = false;
    unsigned long debounceDelay = 50;  // Delay for button debouncing
    unsigned long lastDebounceTime = 0;  // Timestamp for debouncing
  public:

      // timer sequences to trigger the sound loop
      enum LoopSequence {
          SEQ_5S,
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
          config.activeLoopSequence = 30000; // 30 seconds
          config.loopSequence = SEQ_30S;
          config.volumeValue = 30;
      }

      BirdChaser(int mp3TxPin, int mp3RxPin, int buttonPin) : mp3(mp3TxPin, mp3RxPin), buttonPin(buttonPin) {
        // place for button introducing
        pinMode(buttonPin, INPUT_PULLUP);
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
      void run(bool disableTimer= false) {
          unsigned long currentMillis = millis();
          

          if (!disableTimer && (currentMillis - previousMillis >= config.activeLoopSequence)) {
              previousMillis = currentMillis;

              // Play the first track as a placeholder (could play dynamically loaded tracks)
              mp3.playTrackNumber(1, config.volumeValue);

              // Log the status
              mp3.serialPrintStatus(MP3_ALL_MESSAGE);
          } else if (disableTimer == true) {
               // Play the test track
              mp3.playTrackNumber(1, config.volumeValue);
          }
      }

       // Function to set the configuration based on LoopSequence
      void setConfig(LoopSequence seq) {
          config.loopSequence = seq;
          switch (seq) {
              case SEQ_5S:
                  config.activeLoopSequence = 5000;   // 5 seconds
              case SEQ_30S:
                  config.activeLoopSequence = 30000;  // 30 seconds
                  break;
              case SEQ_2M:
                  config.activeLoopSequence = 120000; // 2 minutes
                  break;
              case SEQ_5M:
                  config.activeLoopSequence = 300000; // 5 minutes
                  break;
              case SEQ_15M:
                  config.activeLoopSequence = 900000; // 15 minutes
                  break;
              case SEQ_30M:
                  config.activeLoopSequence = 1800000; // 30 minutes
                  break;
              default:
                  config.activeLoopSequence = 30000;  // Default to 30 seconds
                  break;
          }
          Serial.print("Loop sequence set to: ");
          Serial.println(config.activeLoopSequence);
      }

};