/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_MARLINUI_LANDSCAPE)
  #define DWIN_WIDTH  480
  #define DWIN_HEIGHT 272
#else
  #define DWIN_WIDTH  272
  #define DWIN_HEIGHT 480
#endif

#define RECEIVED_NO_DATA         0x00
#define RECEIVED_SHAKE_HAND_ACK  0x01

#define FHONE                    0xAA

#define DWIN_SCROLL_UP   2
#define DWIN_SCROLL_DOWN 3

<<<<<<< HEAD
// Make sure DWIN_SendBuf is large enough to hold the largest string plus draw command and tail.
// Assume the narrowest (6 pixel) font and 2-byte gb2312-encoded characters.
extern uint8_t DWIN_SendBuf[11 + DWIN_WIDTH / 6 * 2];
extern uint8_t DWIN_BufTail[4];
extern uint8_t databuf[26];

inline void DWIN_Byte(size_t &i, const uint16_t bval) {
  DWIN_SendBuf[++i] = bval;
}

inline void DWIN_Word(size_t &i, const uint16_t wval) {
  DWIN_SendBuf[++i] = wval >> 8;
  DWIN_SendBuf[++i] = wval & 0xFF;
}

inline void DWIN_Long(size_t &i, const uint32_t lval) {
  DWIN_SendBuf[++i] = (lval >> 24) & 0xFF;
  DWIN_SendBuf[++i] = (lval >> 16) & 0xFF;
  DWIN_SendBuf[++i] = (lval >>  8) & 0xFF;
  DWIN_SendBuf[++i] = lval & 0xFF;
}

// Send the data in the buffer plus the packet tail
void DWIN_Send(size_t &i);

inline void DWIN_Text(size_t &i, const char * const string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, _MIN(strlen(string), rlimit));
  if (len == 0) return;
  memcpy(&DWIN_SendBuf[i+1], string, len);
  i += len;
}

inline void DWIN_Text(size_t &i, FSTR_P string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(DWIN_SendBuf) - i, _MIN(rlimit, strlen_P(FTOP(string))));
  if (len == 0) return;
  memcpy_P(&DWIN_SendBuf[i+1], string, len);
=======
// Make sure dwinSendBuf is large enough to hold the largest string plus draw command and tail.
// Assume the narrowest (6 pixel) font and 2-byte gb2312-encoded characters.
extern uint8_t dwinSendBuf[11 + DWIN_WIDTH / 6 * 2];
extern uint8_t dwinBufTail[4];
extern uint8_t databuf[26];

inline void dwinByte(size_t &i, const uint16_t bval) {
  dwinSendBuf[++i] = bval;
}

inline void dwinWord(size_t &i, const uint16_t wval) {
  dwinSendBuf[++i] = wval >> 8;
  dwinSendBuf[++i] = wval & 0xFF;
}

inline void dwinLong(size_t &i, const uint32_t lval) {
  dwinSendBuf[++i] = (lval >> 24) & 0xFF;
  dwinSendBuf[++i] = (lval >> 16) & 0xFF;
  dwinSendBuf[++i] = (lval >>  8) & 0xFF;
  dwinSendBuf[++i] = lval & 0xFF;
}

// Send the data in the buffer plus the packet tail
void dwinSend(size_t &i);

inline void dwinText(size_t &i, const char * const string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(dwinSendBuf) - i, _MIN(strlen(string), rlimit));
  if (len == 0) return;
  memcpy(&dwinSendBuf[i+1], string, len);
  i += len;
}

inline void dwinText(size_t &i, FSTR_P string, uint16_t rlimit=0xFFFF) {
  if (!string) return;
  const size_t len = _MIN(sizeof(dwinSendBuf) - i, _MIN(rlimit, strlen_P(FTOP(string))));
  if (len == 0) return;
  memcpy_P(&dwinSendBuf[i+1], string, len);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  i += len;
}

/*-------------------------------------- System variable function --------------------------------------*/

// Handshake (1: Success, 0: Fail)
<<<<<<< HEAD
bool DWIN_Handshake();

// DWIN startup
void DWIN_Startup();
=======
bool dwinHandshake();

// DWIN startup
void dwinStartup();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

#if HAS_LCD_BRIGHTNESS
  // Set the backlight brightness
  //  brightness: (0x00-0xFF)
<<<<<<< HEAD
  void DWIN_LCD_Brightness(const uint8_t brightness);
=======
  void dwinLCDBrightness(const uint8_t brightness);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// Set screen display direction
//  dir: 0=0°, 1=90°, 2=180°, 3=270°
<<<<<<< HEAD
void DWIN_Frame_SetDir(uint8_t dir);

// Update display
void DWIN_UpdateLCD();
=======
void dwinFrameSetDir(uint8_t dir);

// Update display
void dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

/*---------------------------------------- Drawing functions ----------------------------------------*/

// Clear screen
//  color: Clear screen color
<<<<<<< HEAD
void DWIN_Frame_Clear(const uint16_t color);
=======
void dwinFrameClear(const uint16_t color);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw a point
//  color: point color
//  width: point width   0x01-0x0F
//  height: point height 0x01-0x0F
//  x,y: upper left point
#if ENABLED(TJC_DISPLAY)
<<<<<<< HEAD
  void DWIN_Draw_Box(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xSize, uint16_t ySize);
  inline void DWIN_Draw_Point(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
    DWIN_Draw_Box(1, color, x, y, 1, 1);
  }
#else
  void DWIN_Draw_Point(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y);
=======
  void dwinDrawBox(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xSize, uint16_t ySize);
  inline void dwinDrawPoint(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y) {
    dwinDrawBox(1, color, x, y, 1, 1);
  }
#else
  void dwinDrawPoint(uint16_t color, uint8_t width, uint8_t height, uint16_t x, uint16_t y);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// Draw a line
//  color: Line segment color
//  xStart/yStart: Start point
//  xEnd/yEnd: End point
<<<<<<< HEAD
void DWIN_Draw_Line(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
=======
void dwinDrawLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw a Horizontal line
//  color: Line segment color
//  xStart/yStart: Start point
//  xLength: Line Length
<<<<<<< HEAD
inline void DWIN_Draw_HLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xLength) {
  DWIN_Draw_Line(color, xStart, yStart, xStart + xLength - 1, yStart);
=======
inline void dwinDrawHLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xLength) {
  dwinDrawLine(color, xStart, yStart, xStart + xLength - 1, yStart);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

// Draw a Vertical line
//  color: Line segment color
//  xStart/yStart: Start point
//  yLength: Line Length
<<<<<<< HEAD
inline void DWIN_Draw_VLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t yLength) {
  DWIN_Draw_Line(color, xStart, yStart, xStart, yStart + yLength - 1);
=======
inline void dwinDrawVLine(uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t yLength) {
  dwinDrawLine(color, xStart, yStart, xStart, yStart + yLength - 1);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

// Draw a rectangle
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xEnd/yEnd: lower right point
<<<<<<< HEAD
void DWIN_Draw_Rectangle(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
=======
void dwinDrawRectangle(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw a box
//  mode: 0=frame, 1=fill, 2=XOR fill
//  color: Rectangle color
//  xStart/yStart: upper left point
//  xSize/ySize: box size
<<<<<<< HEAD
inline void DWIN_Draw_Box(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xSize, uint16_t ySize) {
  DWIN_Draw_Rectangle(mode, color, xStart, yStart, xStart + xSize - 1, yStart + ySize - 1);
=======
inline void dwinDrawBox(uint8_t mode, uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xSize, uint16_t ySize) {
  dwinDrawRectangle(mode, color, xStart, yStart, xStart + xSize - 1, yStart + ySize - 1);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

// Move a screen area
//  mode: 0, circle shift; 1, translation
//  dir: 0=left, 1=right, 2=up, 3=down
//  dis: Distance
//  color: Fill color
//  xStart/yStart: upper left point
//  xEnd/yEnd: bottom right point
<<<<<<< HEAD
void DWIN_Frame_AreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
=======
void dwinFrameAreaMove(uint8_t mode, uint8_t dir, uint16_t dis,
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
                         uint16_t color, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);


/*---------------------------------------- Text related functions ----------------------------------------*/

// Draw a string
//  bShow: true=display background color; false=don't display background color
//  size: Font size
//  color: Character color
//  bColor: Background color
//  x/y: Upper-left coordinate of the string
//  *string: The string
//  rlimit: For draw less chars than string length use rlimit
<<<<<<< HEAD
void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit=0xFFFF);

inline void DWIN_Draw_String(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, FSTR_P const ftitle) {
  #ifdef __AVR__
    char ctitle[strlen_P(FTOP(ftitle)) + 1];
    strcpy_P(ctitle, FTOP(ftitle));
    DWIN_Draw_String(bShow, size, color, bColor, x, y, ctitle);
  #else
    DWIN_Draw_String(bShow, size, color, bColor, x, y, FTOP(ftitle));
=======
void dwinDrawString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, const char * const string, uint16_t rlimit=0xFFFF);

inline void dwinDrawString(bool bShow, uint8_t size, uint16_t color, uint16_t bColor, uint16_t x, uint16_t y, FSTR_P const ftitle) {
  #ifdef __AVR__
    char ctitle[strlen_P(FTOP(ftitle)) + 1];
    strcpy_P(ctitle, FTOP(ftitle));
    dwinDrawString(bShow, size, color, bColor, x, y, ctitle);
  #else
    dwinDrawString(bShow, size, color, bColor, x, y, FTOP(ftitle));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
}

/*---------------------------------------- Picture related functions ----------------------------------------*/

// Draw JPG and cached in #0 virtual display area
//  id: Picture ID
<<<<<<< HEAD
void DWIN_JPG_ShowAndCache(const uint8_t id);
=======
void dwinJPGShowAndCache(const uint8_t id);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw an Icon
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
<<<<<<< HEAD
void DWIN_ICON_Show(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);
=======
void dwinIconShow(uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw an Icon
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  libID: Icon library ID
//  picID: Icon ID
//  x/y: Upper-left point
<<<<<<< HEAD
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);
=======
void dwinIconShow(bool IBD, bool BIR, bool BFI, uint8_t libID, uint8_t picID, uint16_t x, uint16_t y);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Draw an Icon from SRAM
//  IBD: The icon background display: 0=Background filtering is not displayed, 1=Background display \\When setting the background filtering not to display, the background must be pure black
//  BIR: Background image restoration: 0=Background image is not restored, 1=Automatically use virtual display area image for background restoration
//  BFI: Background filtering strength: 0=normal, 1=enhanced, (only valid when the icon background display=0)
//  x/y: Upper-left point
//  addr: SRAM address
<<<<<<< HEAD
void DWIN_ICON_Show(bool IBD, bool BIR, bool BFI, uint16_t x, uint16_t y, uint16_t addr);
=======
void dwinIconShow(bool IBD, bool BIR, bool BFI, uint16_t x, uint16_t y, uint16_t addr);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Unzip the JPG picture to a virtual display area
//  n: Cache index
//  id: Picture ID
<<<<<<< HEAD
void DWIN_JPG_CacheToN(uint8_t n, uint8_t id);

// Unzip the JPG picture to virtual display area #1
//  id: Picture ID
inline void DWIN_JPG_CacheTo1(uint8_t id) { DWIN_JPG_CacheToN(1, id); }
=======
void dwinJPGCacheToN(uint8_t n, uint8_t id);

// Unzip the JPG picture to virtual display area #1
//  id: Picture ID
inline void dwinJPGCacheTo1(uint8_t id) { dwinJPGCacheToN(1, id); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

// Animate a series of icons
//  animID: Animation ID  up to 16
//  animate: animation on or off
//  libID: Icon library ID
//  picIDs: Icon starting ID
//  picIDe: Icon ending ID
//  x/y: Upper-left point
//  interval: Display time interval, unit 10mS
<<<<<<< HEAD
void DWIN_ICON_Animation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval);

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void DWIN_ICON_AnimationControl(uint16_t state);
=======
void dwinIconAnimation(uint8_t animID, bool animate, uint8_t libID, uint8_t picIDs, uint8_t picIDe, uint16_t x, uint16_t y, uint16_t interval);

// Animation Control
//  state: 16 bits, each bit is the state of an animation id
void dwinIconAnimationControl(uint16_t state);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
