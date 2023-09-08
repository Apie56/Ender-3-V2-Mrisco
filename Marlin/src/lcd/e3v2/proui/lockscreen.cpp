/**
 * Lock screen implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * Version: 2.3.3
 * Date: 2022/11/20
=======
 * Version: 4.1.3
 * Date: 2023/07/12
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

<<<<<<< HEAD
#if BOTH(DWIN_LCD_PROUI, HAS_LOCKSCREEN)
=======
#if ALL(DWIN_LCD_PROUI, HAS_LOCKSCREEN)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

#include "dwin_defines.h"
#include "dwinui.h"
#include "dwin.h"
#include "lockscreen.h"

<<<<<<< HEAD
LockScreenClass lockScreen;

uint8_t LockScreenClass::lock_pos = 0;
bool LockScreenClass::unlocked = false;
uint8_t LockScreenClass::rprocess = 0;

void LockScreenClass::init() {
=======
LockScreen lockScreen;

uint8_t LockScreen::lock_pos = 0;
bool LockScreen::unlocked = false;
uint8_t LockScreen::rprocess = 0;

void LockScreen::init() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  lock_pos = 0;
  unlocked = false;
  draw();
}

<<<<<<< HEAD
void LockScreenClass::draw() {
  Title.SetCaption(GET_TEXT_F(MSG_LOCKSCREEN));
  DWINUI::ClearMainArea();
  DWINUI::Draw_Icon(ICON_LOGO, 71, 120);  // CREALITY logo
  DWINUI::Draw_CenteredString(Color_White, 180, GET_TEXT_F(MSG_LOCKSCREEN_LOCKED));
  DWINUI::Draw_CenteredString(Color_White, 200, GET_TEXT_F(MSG_LOCKSCREEN_UNLOCK));
  DWINUI::Draw_CenteredString(Color_White, 240, F("-> | <-"));
  DWIN_Draw_Box(1, HMI_data.Barfill_Color, 0, 260, DWIN_WIDTH, 20);
  DWIN_Draw_VLine(Color_Yellow, lock_pos * DWIN_WIDTH / 255, 260, 20);
  DWIN_UpdateLCD();
}

void LockScreenClass::onEncoder(EncoderState encoder_diffState) {
=======
void LockScreen::draw() {
  title.setCaption(GET_TEXT_F(MSG_LOCKSCREEN));
  DWINUI::clearMainArea();
  DWINUI::drawIcon(ICON_LOGO, 71, 120);  // CREALITY logo
  DWINUI::drawCenteredString(COLOR_WHITE, 180, GET_TEXT_F(MSG_LOCKSCREEN_LOCKED));
  DWINUI::drawCenteredString(COLOR_WHITE, 200, GET_TEXT_F(MSG_LOCKSCREEN_UNLOCK));
  DWINUI::drawCenteredString(COLOR_WHITE, 240, F("-> | <-"));
  dwinDrawBox(1, hmiData.colorBarfill, 0, 260, DWIN_WIDTH, 20);
  dwinDrawVLine(COLOR_YELLOW, lock_pos * DWIN_WIDTH / 255, 260, 20);
  dwinUpdateLCD();
}

void LockScreen::onEncoder(EncoderState encoder_diffState) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  switch (encoder_diffState) {
    case ENCODER_DIFF_CW:    lock_pos += 8; break;
    case ENCODER_DIFF_CCW:   lock_pos -= 8; break;
    case ENCODER_DIFF_ENTER: unlocked = (lock_pos == 128); break;
    default: break;
  }
<<<<<<< HEAD
  DWIN_Draw_Box(1, HMI_data.Barfill_Color, 0, 260, DWIN_WIDTH, 20);
  DWIN_Draw_VLine(Color_Yellow, lock_pos * DWIN_WIDTH / 255, 260, 20);
  DWIN_UpdateLCD();
=======
  dwinDrawBox(1, hmiData.colorBarfill, 0, 260, DWIN_WIDTH, 20);
  dwinDrawVLine(COLOR_YELLOW, lock_pos * DWIN_WIDTH / 255, 260, 20);
  dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

#endif // DWIN_LCD_PROUI && HAS_LOCKSCREEN
