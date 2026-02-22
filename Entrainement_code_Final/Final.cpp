#include "Final.h"

// ======== Affichage ========

void BarChargement500ms() {
  int barL = L - 80;
  int barH = 18;
  int x = 40;
  int y = H - 60;

  tft.drawRoundRect(x, y, barL, barH, 6, TFT_WHITE);

  int innerW = barL - 2;
  int innerH = barH - 2;

  for (int i = 1; i <= 10; i++) {
    int fillW = (innerW * i) / 10;
    tft.fillRoundRect(x + 1, y + 1, fillW, innerH, 5, TFT_WHITE);
    delay(50);
  }
}

void AfficheMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, tft.width(), tft.height(), TFT_WHITE);
  tft.setTextColor(TFT_VIOLET, TFT_BLACK);
  tft.setTextSize(2);

  tft.setCursor(60, 10);
  tft.print("Choisissez un Test");

  tft.setCursor(20, 60);
  tft.setTextColor(TFT_GOLD, TFT_BLACK);
  tft.print("BP1 = Addition d'appuie");

  tft.setCursor(20, 90);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.print("BP2 = Mode 2 (libre)");

  tft.setCursor(20, 120);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.print("BP3 = Test son");

  tft.setCursor(20, 150);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.print("BP4 = Test ecran");
}

// ===================== MODE 1 =====================
void Mode_Addition() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  tft.setCursor(20, 20);
  tft.print("MODE 1 : Addition BP1");

  tft.setCursor(20, 60);
  tft.print("Compteur = ");
  tft.print(compteurBP1);

  tft.setCursor(20, 120);
  tft.print("BP1 : +1");

  tft.setCursor(20, 150);
  tft.print("BP3 : reset");

  tft.setCursor(20, 180);
  tft.print("BP2 : retour menu");

  while (mode == 1) {
    if (digitalRead(BP1) == 1) {
      compteurBP1 = compteurBP1 + 1;

      tft.fillRect(20, 60, 300, 25, TFT_BLACK);
      tft.setCursor(20, 60);
      tft.print("Compteur = ");
      tft.print(compteurBP1);

      delay(20);  // anti-rebond appui
      while (digitalRead(BP1) == 1) {}
      delay(20);  // anti-rebond relâchement
    }
    if (digitalRead(BP3)) {
      compteurBP1 = 0;
      tft.fillRect(20, 60, 300, 25, TFT_BLACK);
      tft.setCursor(20, 60);
      tft.print("Compteur = ");
      tft.print(compteurBP1);
    }

    if (digitalRead(BP2) || digitalRead(BP4)) {
      mode = 0;
    }
  }
}

// ======== I2S ========
void i2s_init_simple() {
  i2s_config_t cfg = {};
  cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  cfg.sample_rate = SAMPLE_RATE;
  cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
  cfg.communication_format = I2S_COMM_FORMAT_I2S;
  cfg.dma_buf_count = 8;
  cfg.dma_buf_len = 256;
  cfg.tx_desc_auto_clear = true;

  i2s_pin_config_t pins = {};
  pins.bck_io_num = I2S_BCLK;
  pins.ws_io_num = I2S_LRC;
  pins.data_out_num = I2S_DOUT;
  pins.data_in_num = I2S_PIN_NO_CHANGE;

  i2s_driver_install(I2S_NUM_0, &cfg, 0, nullptr);
  i2s_set_pin(I2S_NUM_0, &pins);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

void beep_A4_50ms() {
  int dureeMs;
  int nombreEchantillons;
  int demiPeriode;
  int compteurDemiPeriode;
  int i;

  int16_t valeur;
  int16_t echantillonG;
  size_t octetsEcrits;

  dureeMs = 50;
  nombreEchantillons = (SAMPLE_RATE * dureeMs) / 1000;

  demiPeriode = SAMPLE_RATE / (2 * FREQ_BEEP);  // demi-période de 440 Hz (onde carrée)

  valeur = AMP_BEEP;
  compteurDemiPeriode = 0;

  //Création du beep via la boucle for
  for (i = 0; i < nombreEchantillons; i++) {

    echantillonG = valeur;

    i2s_write(I2S_NUM_0, &echantillonG, 2, &octetsEcrits, portMAX_DELAY);  //int16_t = 2 octets

    compteurDemiPeriode = compteurDemiPeriode + 1;

    if (compteurDemiPeriode >= demiPeriode) {
      compteurDemiPeriode = 0;

      if (valeur > 0) {
        valeur = -AMP_BEEP;
      } else {
        valeur = AMP_BEEP;
      }
    }
  }

  //retour au silence, peut evité certains bruit parasite(recommendation de Gpt)
  echantillonG = 0;
  for (i = 0; i < 20; i++) {
    i2s_write(I2S_NUM_0, &echantillonG, 2, &octetsEcrits, portMAX_DELAY);
  }
}
