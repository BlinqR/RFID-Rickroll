#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>

// RST  48 (Adjustable)
// MISO 50 For Mega (12 For Uno)
// MOSI 51 For Mega (11 For Uno)
// SCK  52 For Mega (13 For Uno)
// SDA  53 (Adjustable)

#define RST 48
#define SS  53

// RS  1 (Adjustable)
// E   2 (Adjustable)
// D4  3 (Adjustable)
// D5  4 (Adjustable)
// D6  5 (Adjustable)
// D7  6 (Adjustable)
// VSS ground
// VDD anode
// V0  potentiometer
// RW  ground
// A   resistor to anode
// K   ground

#define RS 1
#define E  2
#define D4 3
#define D5 4
#define D6 5
#define D7 6
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Defines 2 LEDs and Buzzers
#define GREEN 10
#define RED 9
#define BUZZER 8
#define BUZZER_PIN 11

// Creates string containing Master Tag and string for tag to be scanned
String MasterTag = "471573B4";
String tagID = "";

//Creates Object reader
MFRC522 reader(SS, RST);

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Begins LCD
  lcd.begin(16, 2);

  // Begins SPI for RFID
  SPI.begin();
  reader.PCD_Init();
}

void playRickRoll() {
  // Define the note frequencies
  #define G3 196.00
  #define A3 220.00
  #define B3 246
  #define C4 261.63
  #define D4 293.66
  #define E4 329.63
  #define STOP 0

  // Array containing notes in order
  double melody[] = {
    G3, A3, C4, A3, E4, E4, D4, // Never gonna give you up
    STOP,
    G3, A3, C4, A3, D4, D4, C4, // Never gonna let you down
    STOP,
    G3, A3, C4, A3, C4, D4, B3, A3, G3, G3, D4, C4 // Never gonna tell a lie and desert you

  };

  // Array containing durations for each note (4 means quarter note, etc.)
  double noteDurations[] = {
    10, 10, 10, 10, 3, 3, 10,
    2,
    10, 10, 10, 10, 3, 3, 10,
    2,
    10, 10, 10, 10, 3, 3, 3, 10, 3, 4, 2.5, 4

  };

  // Play the melody
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(melody[0]); thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER_PIN, melody[thisNote], noteDuration);
      delay(noteDuration*1.2);
      noTone(BUZZER_PIN); // Stop sound
      
    }
}

void print (String text)
{
  lcd.clear();
  lcd.print(text);
  delay(100);
}

void printInLine(String text, int row)
{
  lcd.setCursor(0, row);
  lcd.print(text);
}

bool getID()
{
  if (!reader.PICC_IsNewCardPresent()) {
      return false; // No new card present
  }

  if (!reader.PICC_ReadCardSerial())
  {
    return false; // Failed to read card
  }

  tagID = "";

  for (byte i = 0; i < reader.uid.size; i++)
  {
    tagID += String(reader.uid.uidByte[i], HEX); // Convert byte to hex string
  }
  tagID.toUpperCase();
  reader.PICC_HaltA();
  return true;

}

void loop() {
  // put your main code here, to run repeatedly:
  printInLine("Scan your card", 0);
  printInLine("for a susprise!", 1);
  delay(500);
  lcd.clear();
  delay(500);

  while(getID())
  {
    int delayVal = 30;
    if(tagID == MasterTag)
    {
      print("Access Granted.");
      digitalWrite(GREEN, 1);

      for (int i = 0; i < 2; i++)
      {
        digitalWrite(BUZZER, 1);
        delay(50);
        digitalWrite(BUZZER, 0);
        delay(50);
      }
      delay(2000);
      playRickRoll();
      digitalWrite(GREEN, 0);
    }
    else
    {
      lcd.setCursor(0, 0);
      print("Access denied.");
      lcd.setCursor(12, 0);
      
      for(int i = 0; i < 12; i++)
      {
        digitalWrite(RED, 1);
        digitalWrite(BUZZER, 1);
        delay(delayVal);
        digitalWrite(RED, 0);
        digitalWrite(BUZZER, 0);
        delay(delayVal);
      }
    }
    delay(3000);
  }
  
}
