#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4); // Inisialisasi LCD I2C

const int tombolPeserta[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int ledPeserta[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int tombolReset = 12;
const int tombolMode = 13;
const int buzzerPeserta = A0;
const int buzzerOperator = A1;

int mode = 1;  // Mode default
int timerDurasi = 10;  // Durasi waktu default dalam detik
unsigned long startTime = 0;
bool timerAktif = false;

int pesertaAktif[3] = {-1, -1, -1}; // Penyimpanan tombol tercepat
int pesertaCounter = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  
  // Inisialisasi pin tombol dan LED
  for (int i = 0; i < 10; i++) {
    pinMode(tombolPeserta[i], INPUT_PULLUP);
    pinMode(ledPeserta[i], OUTPUT);
    digitalWrite(ledPeserta[i], LOW);
  }
  pinMode(tombolReset, INPUT_PULLUP);
  pinMode(tombolMode, INPUT_PULLUP);
  pinMode(buzzerPeserta, OUTPUT);
  pinMode(buzzerOperator, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Bel Cerdas Cermat");
  lcd.setCursor(0, 1);
  lcd.print("Mode: 1 (1 Pemenang)");
  delay(2000);
}

void loop() {
  // Mode Tombol
  if (digitalRead(tombolMode) == LOW) {
    delay(200);  // Debounce
    mode++;
    if (mode > 3) mode = 1;  // Reset mode ke 1
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mode: ");
    lcd.print(mode);
    if (mode == 1) lcd.print(" (1 Pemenang)");
    else if (mode == 2) lcd.print(" (3 Pemenang)");
    else if (mode == 3) lcd.print(" (Timer)");
    delay(500);
  }

  // Mode Reset
  if (digitalRead(tombolReset) == LOW) {
    delay(200);  // Debounce
    resetSystem();
  }

  // Cek tombol peserta
  for (int i = 0; i < 10; i++) {
    if (digitalRead(tombolPeserta[i]) == LOW) {
      delay(200);  // Debounce
      if (!pesertaSudahAktif(i)) {
        pesertaAktif[pesertaCounter] = i;
        pesertaCounter++;
        digitalWrite(ledPeserta[i], HIGH);
        tone(buzzerPeserta, 1000, 200);
        
        if (mode == 1 && pesertaCounter == 1) break;
        if (mode == 2 && pesertaCounter == 3) break;
      }
    }
  }

  // Mode Timer
  if (mode == 3 && !timerAktif) {
    timerAktif = true;
    startTime = millis();
    lcd.setCursor(0, 2);
    lcd.print("Waktu: ");
    lcd.print(timerDurasi);
    lcd.print(" detik");
  }
  
  if (timerAktif) {
    unsigned long elapsedTime = (millis() - startTime) / 1000;
    if (elapsedTime >= timerDurasi) {
      timerAktif = false;
      tone(buzzerOperator, 2000, 500);
      lcd.setCursor(0, 2);
      lcd.print("Waktu habis!      ");
    }
  }
}

void resetSystem() {
  pesertaCounter = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPeserta[i], LOW);
    pesertaAktif[i] = -1;
  }
  timerAktif = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reset berhasil!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(mode);
}

bool pesertaSudahAktif(int peserta) {
  for (int i = 0; i < pesertaCounter; i++) {
    if (pesertaAktif[i] == peserta) return true;
  }
  return false;
}
