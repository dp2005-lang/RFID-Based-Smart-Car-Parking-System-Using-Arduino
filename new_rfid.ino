#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

#define BUZZER_PIN 5
#define GREEN_LED 6
#define RED_LED 7

// Authorized RFID UID
String authorizedUID = "BC 0A D5 05";

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

//---------------- BUZZER FUNCTIONS ----------------//

// Active HIGH buzzer
void buzzerOn() {
  digitalWrite(BUZZER_PIN, HIGH);
}

void buzzerOff() {
  digitalWrite(BUZZER_PIN, LOW);
}

void successBeep() {
  for (int i = 0; i < 2; i++) {
    buzzerOn();
    delay(150);
    buzzerOff();
    delay(100);
  }
}

void errorBeep() {
  for (int i = 0; i < 3; i++) {
    buzzerOn();
    delay(250);
    buzzerOff();
    delay(150);
  }
}

//--------------------------------------------------//

void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Keep buzzer OFF at startup
  buzzerOff();

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  lcd.setCursor(2, 0);
  lcd.print("Put your card");
}

void loop() {

  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  String cardUID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {

    if (mfrc522.uid.uidByte[i] < 0x10)
      cardUID += "0";

    cardUID += String(mfrc522.uid.uidByte[i], HEX);

    if (i != mfrc522.uid.size - 1)
      cardUID += " ";
  }

  cardUID.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(cardUID);

  lcd.clear();

  // ================= AUTHORIZED =================

  if (cardUID == authorizedUID) {

    lcd.setCursor(0, 0);
    lcd.print("ACCESS GRANTED");

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    successBeep();

    delay(1500);

    digitalWrite(GREEN_LED, LOW);
  }

  // ================= UNAUTHORIZED =================

  else {

    lcd.setCursor(0, 0);
    lcd.print("ACCESS DENIED");

    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    errorBeep();

    delay(1500);

    digitalWrite(RED_LED, LOW);
  }

  // Ensure buzzer is OFF
  buzzerOff();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Put your card");
}
