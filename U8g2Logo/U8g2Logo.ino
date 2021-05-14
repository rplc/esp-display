#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
  This is a page buffer example.    
*/

U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 0, /* data=*/ 4, /* cs=*/ 14, /* dc=*/ 12, /* reset=*/ 13);

//#define MINI_LOGO

void setup(void) {
  u8g2.begin();
  u8g2.setFlipMode(0);
  // TODO set rotation: https://github.com/olikraus/u8g2/wiki/u8g2reference#setdisplayrotation
}

void drawLogo(void)
{
  uint8_t mdy = 0;
  if ( u8g2.getDisplayHeight() < 59 )
    mdy = 5;
  

    u8g2.setFontMode(1);	// Transparent
    u8g2.setDrawColor(1);
#ifdef MINI_LOGO

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(0, 22, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb19_mn);
    u8g2.drawStr(14,8,"8");
    
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb16_mf);
    u8g2.drawStr(36,22,"g");
    u8g2.drawStr(48,22,"\xb2");
    
    u8g2.drawHLine(2, 25, 34);
    u8g2.drawHLine(3, 26, 34);
    u8g2.drawVLine(32, 22, 12);
    u8g2.drawVLine(33, 23, 12);
#else

    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(0, 30-mdy, "U");
    
    u8g2.setFontDirection(1);
    u8g2.setFont(u8g2_font_inb30_mn);
    u8g2.drawStr(21,8-mdy,"8");
        
    u8g2.setFontDirection(0);
    u8g2.setFont(u8g2_font_inb24_mf);
    u8g2.drawStr(51,30-mdy,"g");
    u8g2.drawStr(67,30-mdy,"\xb2");
    
    u8g2.drawHLine(2, 35-mdy, 47);
    u8g2.drawHLine(3, 36-mdy, 47);
    u8g2.drawVLine(45, 32-mdy, 12);
    u8g2.drawVLine(46, 33-mdy, 12);
    
#endif

}

void drawURL(void)
{
#ifndef MINI_LOGO
  u8g2.setFont(u8g2_font_4x6_tr);
  if ( u8g2.getDisplayHeight() < 59 )
  {
    u8g2.drawStr(89,20-5,"github.com");
    u8g2.drawStr(73,29-5,"/olikraus/u8g2");
  }
  else
  {
    u8g2.drawStr(1,54,"github.com/olikraus/u8g2");
  }
#endif
}

void loop(void) {
  u8g2.firstPage();
  do {
    drawLogo();
    drawURL();
  } while ( u8g2.nextPage() );
  delay(4000);
}
