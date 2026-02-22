#include <SPI.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "driver/i2s.h"
#include "Final.h"

byte mode = 0;
int compteurBP1 = 0;  // pour mode 1

TFT_eSPI tft = TFT_eSPI();
int L = 0;
int H = 0;

void setup() {
  Serial.begin(115200);

  // boutons : relâché=0, appuyé=1
  pinMode(BP1, INPUT);
  pinMode(BP2, INPUT);
  pinMode(BP3, INPUT);
  pinMode(BP4, INPUT);

  tft.init();
  tft.setRotation(2);

  L = tft.width();
  H = tft.height();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(5);
  tft.setCursor(70, 150);
  tft.println("GameBoy");
  tft.setTextSize(2);
  tft.setCursor(120, 400);
  tft.print("par Logan");
  delay(600);

  i2s_init_simple();
  beep_A4_50ms();

  tft.fillScreen(TFT_BLACK);
  BarChargement500ms();
  AfficheMenu();
}

void loop() {
  // --------- MODE 0 : MENU ----------
  if (mode == 0) {
    if (digitalRead(BP1) == 1) {
      mode = 1;
      delay(20);
    } else if (digitalRead(BP2) == 1) {
      mode = 2;
      delay(20);
    } else if (digitalRead(BP3) == 1) {
      mode = 3;
      delay(20);
    } else if (digitalRead(BP4) == 1) {
      mode = 4;
      delay(20);
    }
  }

  // --------- MODE 1 ----------
  if (mode == 1) {
    Mode_Addition();
    AfficheMenu();
  }

  // --------- MODE 2 (libre) ----------
  if (mode == 2) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.print("MODE 2 : (Libre)");
    tft.setCursor(20, 60);
    tft.print("BP2/BP4 : retour menu");

    while (mode == 2) {
      if (digitalRead(BP2) == 1 || digitalRead(BP4) == 1)
        mode = 0;
      delay(5);
    }
    AfficheMenu();
  }

  // --------- MODE 3 ----------
  if (mode == 3) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.print("MODE 3 : Test son");
    tft.setCursor(20, 60);
    tft.print("BP3 : bip");
    tft.setCursor(20, 90);
    tft.print("BP2/BP4 : retour menu");

    while (mode == 3) {
      if (digitalRead(BP3) == 1)
        beep_A4_50ms();
      if (digitalRead(BP2) == 1 || digitalRead(BP4) == 1)
        mode = 0;
      delay(5);
    }
    AfficheMenu();
  }

  // --------- MODE 4 :
  if (mode == 4) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GOLD, TFT_VIOLET);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.print("MODE 4 : Test ecran");
    tft.setCursor(20, 60);
    tft.print("BP2/BP4 : retour menu");

    tft.drawRect(10, 110, tft.width() - 20, 60, TFT_WHITE);
    tft.setCursor(20, 130);
    tft.print("OK si tu vois ce cadre");

    while (mode == 4) {
      if (digitalRead(BP2) == 1 || digitalRead(BP4) == 1)
        mode = 0;
      delay(5);
    }
    AfficheMenu();
  }

  delay(5);
}
