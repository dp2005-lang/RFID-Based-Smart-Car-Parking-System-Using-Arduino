#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN      10
#define RST_PIN     9
#define SERVO_PIN   2
#define BUZZER_PIN  5
#define GREEN_LED   6
#define RED_LED     7

String UID = "BC 0A D5 05";

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int IR1 = 4;   // Entry
int IR2 = 3;   // Exit

int availableSlots = 4;
const int MaxSlots = 4;

unsigned long lastActionTime = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(IR1, INPUT_PULLUP);
  pinMode(IR2, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(100);

  lcd.setCursor(0, 0);
  lcd.print(" ARDUINO ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2500);
  lcd.clear();

  SPI.begin();
  rfid.PCD_Init();
  updateDisplay();
}

void loop() {
  if (millis() - lastActionTime < 3000) {
    delay(50);
    return;
  }

  // Check Exit FIRST (Higher Priority for now)
  if (digitalRead(IR2) == LOW) {
    Serial.println("EXIT Sensor Triggered");
    lastActionTime = millis();

    if (availableSlots < MaxSlots) {
      smoothOpenGate();
      availableSlots++;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Car at EXIT");
      lcd.setCursor(0, 1);
      lcd.print("Gate Opening");
      delay(1500);

      waitForSensor(IR1, LOW, 8000);
      waitForSensor(IR1, HIGH, 8000);

      smoothCloseGate();
      showSlotUpdate("Car Exited", availableSlots);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" No Cars Inside ");
      delay(2000);
    }
    updateDisplay();
    return;
  }

  // Entry
  if (digitalRead(IR1) == LOW) {
    Serial.println("ENTRY Sensor Triggered");
    lastActionTime = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car at ENTRY");
    lcd.setCursor(0, 1);
    lcd.print("Scan RFID...");
    delay(700);

    if (availableSlots > 0) {
      if (rfunc() == 1) {
        availableSlots--;
        smoothOpenGate();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gate Opened!");
        lcd.setCursor(0, 1);
        lcd.print("Car Entering");
        delay(1500);

        waitForSensor(IR2, LOW, 8000);
        waitForSensor(IR2, HIGH, 8000);

        smoothCloseGate();
        showSlotUpdate("Car Entered", availableSlots);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parking Full!");
      delay(2000);
    }
    updateDisplay();
  }
}

// Helpers
void waitForSensor(int pin, int state, unsigned long timeout) {
  unsigned long start = millis();
  while (digitalRead(pin) != state && millis() - start < timeout) delay(50);
}

void showSlotUpdate(String action, int slots) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(action);
  lcd.setCursor(0, 1);
  lcd.print("Slots: ");
  lcd.print(slots);
  lcd.print("/");
  lcd.print(MaxSlots);
  delay(2300);
}

void smoothOpenGate() {
  for (int pos = 100; pos >= 0; pos -= 3) myservo.write(pos), delay(8);
}

void smoothCloseGate() {
  for (int pos = 0; pos <= 100; pos += 3) myservo.write(pos), delay(8);
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" WELCOME! ");
  lcd.setCursor(0, 1);
  lcd.print("Slots: ");
  lcd.print(availableSlots);
  lcd.print("/");
  lcd.print(MaxSlots);
  if (availableSlots == 0) lcd.print(" FULL");
  lastActionTime = millis();
}

int rfunc() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Welcome!");
  lcd.setCursor(1, 1);
  lcd.print("Scan RFID Card");

  unsigned long start = millis();
  while (millis() - start < 10000) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) break;
    delay(50);
  }

  if (millis() - start >= 10000) {
    lcd.clear();
    lcd.print("Timed Out!");
    delay(1500);
    return 0;
  }

  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) ID += "0";
    ID += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) ID += " ";
  }
  ID.toUpperCase();

  if (ID == UID) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS GRANTED");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH); delay(150); digitalWrite(BUZZER_PIN, LOW); delay(100);
    digitalWrite(BUZZER_PIN, HIGH); delay(150); digitalWrite(BUZZER_PIN, LOW);
    delay(700);
    digitalWrite(GREEN_LED, LOW);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return 1;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS DENIED");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH); delay(400); digitalWrite(BUZZER_PIN, LOW);
    delay(800);
    digitalWrite(RED_LED, LOW);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return 0;
  }
}
