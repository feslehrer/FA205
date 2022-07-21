/*
 * nunchuk.c
 *
 * Created: 29.03.2018 13:56:31
 * Author: Rahm
 * letzte Änderung: 10.4.2018
 */ 

#include "controller.h"
#include "nunchuk.h"

void nunchuk_init(void)
{
  i2c_init();
  // Initialisierung des Nunchuk
  i2c_start();
  i2c_write(NUNCHUK_ADDR_W);
  i2c_write(0xF0);
  i2c_write(0x55);
  i2c_stop();     delay_100us(1);   // kurze Pause !!!
  i2c_start();
  i2c_write(NUNCHUK_ADDR_W);
  i2c_write(0xFB);
  i2c_write(0x00);
  i2c_stop();     delay_100us(1);   // kurze Pause !!!
}

void nunchuk_read( void )
{
  uint8_t nun_buf[6];
  // Initialisierung des RAM-Pointers
  i2c_start();
  i2c_write(NUNCHUK_ADDR_W);
  i2c_write(0x00);
  i2c_stop();     delay_100us(1);   // kurze Pause !!!
  
  // Lesen der Daten
  i2c_start();
  i2c_write(NUNCHUK_ADDR_R);
  nun_buf[0] = i2c_read(ACK);
  nun_buf[1] = i2c_read(ACK);
  nun_buf[2] = i2c_read(ACK);
  nun_buf[3] = i2c_read(ACK);
  nun_buf[4] = i2c_read(ACK);
  nun_buf[5] = i2c_read(NACK);
  i2c_stop();
  
  // Übergabe Joystick
  X_joy = nun_buf[0];
  Y_joy = nun_buf[1];
  // Berechnung der Beschleunigungen (Acceleration)
  X_accel = nun_buf[2]*4 + ((nun_buf[5]/4) & 0x01);
  Y_accel = nun_buf[3]*4 + ((nun_buf[5]/16) & 0x01);
  Z_accel = nun_buf[4]*4 + ((nun_buf[5]/64) & 0x01);
  
  // Ermitteln der Button Zustände
  C_button = (nun_buf[5]>>1) & 0x01;
  Z_button =  nun_buf[5]     & 0x01;
}

//Gibt den Inhalt der globalen Nunchuk-Variablen auf dem Display aus
void nunchuk_debug(void)
{
  lcd_setcursor(1,1);
  lcd_int(X_accel);
  lcd_setcursor(1,6);
  lcd_int(Y_accel);
  lcd_setcursor(1,11);
  lcd_int(Z_accel);

  lcd_setcursor(2,1);
  lcd_print("X:");
  lcd_byte(X_joy);
  lcd_setcursor(2,8);
  lcd_print("Y:");
  lcd_byte(Y_joy);
  
  lcd_setcursor(2,15);
  if (C_button) lcd_char('1');  else lcd_char('0');
  lcd_setcursor(2,16);
  if (Z_button) lcd_char('1');  else lcd_char('0');
}