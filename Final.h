#ifndef Final_H
#define Final_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "driver/i2s.h"

// ========= PINS =========
#define BP1 16
#define BP2 17
#define BP3 18
#define BP4 3

#define I2S_DOUT 38
#define I2S_BCLK 37
#define I2S_LRC  36

// ========= PARAMETRE AUDIO =========
#define SAMPLE_RATE 22050
#define FREQ_BEEP   440
#define AMP_BEEP    8000

// ========= VARIABLES partagées =========
extern TFT_eSPI tft;
extern int L;
extern int H;

extern byte mode;
extern int compteurBP1;

// ========= FONCTIONS =========
void BarChargement500ms();
void AfficheMenu();

void i2s_init_simple();
void beep_A4_50ms();

void Mode_Addition();

#endif
