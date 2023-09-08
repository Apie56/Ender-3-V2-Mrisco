/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * Version: 3.12.1
 * Date: 2022/04/03
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
=======
 * Version: 4.1.1
 * Date: 2023/07/12
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
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

#if ENABLED(DWIN_LCD_PROUI)

#include "../../../MarlinCore.h" // for wait_for_user
#include "dwin.h"
#include "dwinui.h"
#include "dwin_popup.h"

<<<<<<< HEAD
void (*ClickPopup)()=nullptr;
void (*PopupChange)(bool state)=nullptr;
void (*Draw_Popup)()=nullptr;
uint16_t HighlightYPos = 280;

void Draw_Select_Highlight(const bool sel, const uint16_t ypos) {
  HighlightYPos = ypos;
  HMI_flag.select_flag = sel;
  const uint16_t c1 = sel ? HMI_data.Highlight_Color : HMI_data.PopupBg_Color,
                 c2 = sel ? HMI_data.PopupBg_Color : HMI_data.Highlight_Color;
  DWIN_Draw_Rectangle(0, c1, 25, ypos - 1, 126, ypos + 38);
  DWIN_Draw_Rectangle(0, c1, 24, ypos - 2, 127, ypos + 39);
  DWIN_Draw_Rectangle(0, c2, 145, ypos - 1, 246, ypos + 38);
  DWIN_Draw_Rectangle(0, c2, 144, ypos - 2, 247, ypos + 39);
}

void DWIN_Popup_Continue(const uint8_t icon, FSTR_P const fmsg1, FSTR_P const fmsg2) {
  HMI_SaveProcessID(WaitResponse);
  DWIN_Draw_Popup(icon, fmsg1, fmsg2, BTN_Continue);  // Button Continue
  DWIN_UpdateLCD();
}

void DWIN_Popup_ConfirmCancel(const uint8_t icon, FSTR_P const fmsg2) {
  DWIN_Draw_Popup(icon, F("Please confirm"), fmsg2);
  DWINUI::Draw_Button(BTN_Confirm, 26, 280);
  DWINUI::Draw_Button(BTN_Cancel, 146, 280);
  Draw_Select_Highlight(HMI_flag.select_flag);
  DWIN_UpdateLCD();
}

void Goto_Popup(void (*onPopupDraw)(), void (*onClickPopup)() /*= nullptr*/, void (*onPopupChange)(bool state) /*= nullptr*/) {
  Draw_Popup = onPopupDraw;
  ClickPopup = onClickPopup;
  PopupChange = onPopupChange;
  HMI_SaveProcessID(Popup);
  HMI_flag.select_flag = false;
  Draw_Popup();
}

void HMI_Popup() {
  if (!wait_for_user) {
    if (ClickPopup) ClickPopup();
=======
popupDrawFunc_t popupDraw = nullptr;
popupClickFunc_t popupClick = nullptr;
popupChangeFunc_t popupChange = nullptr;

uint16_t HighlightYPos = 280;

void drawSelectHighlight(const bool sel, const uint16_t ypos) {
  HighlightYPos = ypos;
  hmiFlag.select_flag = sel;
  const uint16_t c1 = sel ? hmiData.colorHighlight : hmiData.colorPopupBg,
                 c2 = sel ? hmiData.colorPopupBg : hmiData.colorHighlight;
  dwinDrawRectangle(0, c1, 25, ypos - 1, 126, ypos + 38);
  dwinDrawRectangle(0, c1, 24, ypos - 2, 127, ypos + 39);
  dwinDrawRectangle(0, c2, 145, ypos - 1, 246, ypos + 38);
  dwinDrawRectangle(0, c2, 144, ypos - 2, 247, ypos + 39);
}

void dwinPopupContinue(const uint8_t icon, FSTR_P const fmsg1, FSTR_P const fmsg2) {
  hmiSaveProcessID(ID_WaitResponse);
  dwinDrawPopup(icon, fmsg1, fmsg2, BTN_Continue);  // Button Continue
  dwinUpdateLCD();
}

void dwinPopupConfirmCancel(const uint8_t icon, FSTR_P const fmsg2) {
  dwinDrawPopup(icon, F("Please confirm"), fmsg2);
  DWINUI::drawButton(BTN_Confirm, 26, 280);
  DWINUI::drawButton(BTN_Cancel, 146, 280);
  drawSelectHighlight(hmiFlag.select_flag);
  dwinUpdateLCD();
}

void gotoPopup(const popupDrawFunc_t fnDraw, const popupClickFunc_t fnClick/*=nullptr*/, const popupChangeFunc_t fnChange/*=nullptr*/) {
  popupDraw = fnDraw;
  popupClick = fnClick;
  popupChange = fnChange;
  hmiSaveProcessID(ID_Popup);
  hmiFlag.select_flag = false;
  popupDraw();
}

void hmiPopup() {
  if (!wait_for_user) {
    if (popupClick) popupClick();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    return;
  }
  else {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_CW || encoder_diffState == ENCODER_DIFF_CCW) {
      const bool change = encoder_diffState != ENCODER_DIFF_CW;
<<<<<<< HEAD
      if (PopupChange) PopupChange(change); else Draw_Select_Highlight(change, HighlightYPos);
      DWIN_UpdateLCD();
=======
      if (popupChange) popupChange(change); else drawSelectHighlight(change, HighlightYPos);
      dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  }
}

#endif // DWIN_LCD_PROUI
