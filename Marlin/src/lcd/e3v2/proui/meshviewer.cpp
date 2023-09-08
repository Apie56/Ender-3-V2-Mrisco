/**
 * Mesh Viewer for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * version: 4.2.1
 * Date: 2023/05/05
=======
 * version: 5.2.1
 * Date: 2023/08/19
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
#if BOTH(DWIN_LCD_PROUI, HAS_MESH)
=======
#if ALL(DWIN_LCD_PROUI, HAS_MESH) && DISABLED(PROUI_EX)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

#include "../../../core/types.h"
#include "../../marlinui.h"
#include "dwin.h"
#include "dwin_popup.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "meshviewer.h"

<<<<<<< HEAD
#if ENABLED(USE_GRID_MESHVIEWER)
  #include "bedlevel_tools.h"
#endif

=======
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
bool meshredraw;                            // Redraw mesh points
uint8_t sizex, sizey;                       // Mesh XY size
uint8_t rmax;                               // Maximum radius
#define margin 25                           // XY Margins
#define rmin 5                              // Minimum radius
#define zmin -20                            // rmin at z=-0.20
#define zmax  20                            // rmax at z= 0.20
#define width DWIN_WIDTH - 2 * margin
#define r(z) ((z-zmin)*(rmax-rmin)/(zmax-zmin)+rmin)
#define px(xp) (margin + (xp)*(width)/(sizex - 1))
#define py(yp) (30 + DWIN_WIDTH - margin - (yp)*(width)/(sizey - 1))

#if ENABLED(TJC_DISPLAY)
  #define meshfont font8x16
#else
  #define meshfont font6x12
#endif

<<<<<<< HEAD
MeshViewerClass MeshViewer;

float MeshViewerClass::max, MeshViewerClass::min;

void MeshViewerClass::DrawMeshGrid(const uint8_t csizex, const uint8_t csizey) {
=======
MeshViewer meshViewer;

float MeshViewer::max, MeshViewer::min;

void MeshViewer::drawBackground(const uint8_t csizex, const uint8_t csizey) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  sizex = csizex;
  sizey = csizey;
  rmax = _MIN(margin - 2, 0.5*(width)/(sizex - 1));
  min = 100;
  max = -100;
<<<<<<< HEAD
  DWINUI::ClearMainArea();
  DWIN_Draw_Rectangle(0, HMI_data.SplitLine_Color, px(0), py(0), px(sizex - 1), py(sizey - 1));
  LOOP_S_L_N(x, 1, sizex - 1) DWIN_Draw_VLine(HMI_data.SplitLine_Color, px(x), py(sizey - 1), width);
  LOOP_S_L_N(y, 1, sizey - 1) DWIN_Draw_HLine(HMI_data.SplitLine_Color, px(0), py(y), width);
}

void MeshViewerClass::DrawMeshPoint(const uint8_t x, const uint8_t y, const float z) {
  const uint8_t fs = DWINUI::fontWidth(meshfont);
  int16_t v = isnan(z) ? 0 : round(z * 100);
  LIMIT(v, zmin, zmax);
  NOLESS(max, z);
  NOMORE(min, z);
  const uint16_t color = DWINUI::RainbowInt(v, zmin, zmax);
  DWINUI::Draw_FillCircle(color, px(x), py(y), r(v));
  TERN_(TJC_DISPLAY, delay(100));
  if (sizex < (ENABLED(TJC_DISPLAY) ? 8 : 9)) {
    if (v == 0) DWINUI::Draw_Float(meshfont, 1, 2, px(x) - 2*fs, py(y) - fs, 0);
    else DWINUI::Draw_Signed_Float(meshfont, 1, 2, px(x) - 3*fs, py(y) - fs, z);
  }
  else {
    char str_1[9];
    str_1[0] = 0;
    switch (v) {
      case -999 ... -100:
        DWINUI::Draw_Signed_Float(meshfont, 1, 1, px(x) - 3*fs, py(y) - fs, z);
        break;
      case -99 ... -1:
        sprintf_P(str_1, PSTR("-.%02i"), -v);
        break;
      case 0:
        DWIN_Draw_String(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, px(x) - 4, py(y) - fs, "0");
        break;
      case 1 ... 99:
        sprintf_P(str_1, PSTR(".%02i"), v);
        break;
      case 100 ... 999:
        DWINUI::Draw_Signed_Float(meshfont, 1, 1, px(x) - 3*fs, py(y) - fs, z);
        break;
    }
    if (str_1[0])
      DWIN_Draw_String(false, meshfont, DWINUI::textcolor, DWINUI::backcolor, px(x) - 2*fs, py(y) - fs, str_1);
  }
}

void MeshViewerClass::DrawMesh(bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey) {
  DrawMeshGrid(csizex, csizey);
  LOOP_L_N(y, csizey) {
    hal.watchdog_refresh();
    LOOP_L_N(x, csizex) DrawMeshPoint(x, y, zval[x][y]);
  }
}

void MeshViewerClass::Draw(bool withsave /*=false*/, bool redraw /*=true*/) {
  Title.ShowCaption(GET_TEXT_F(MSG_MESH_VIEWER));
  #if ENABLED(USE_GRID_MESHVIEWER)
    DWINUI::ClearMainArea();
    bedLevelTools.viewer_print_value = true;
    bedLevelTools.Draw_Bed_Mesh(-1, 1, 8, 10 + TITLE_HEIGHT);
  #else
    if (redraw) DrawMesh(bedlevel.z_values, GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
    else DWINUI::Draw_Box(1, HMI_data.Background_Color, {89,305,99,38});
  #endif
  if (withsave) {
    DWINUI::Draw_Button(BTN_Save, 26, 305);
    DWINUI::Draw_Button(BTN_Continue, 146, 305);
    Draw_Select_Highlight(HMI_flag.select_flag, 305);
  }
  else
    DWINUI::Draw_Button(BTN_Continue, 86, 305);

  #if ENABLED(USE_GRID_MESHVIEWER)
    bedLevelTools.Set_Mesh_Viewer_Status();
  #else
    char str_1[6], str_2[6] = "";
    ui.status_printf(0, F("Mesh minZ: %s, maxZ: %s"),
      dtostrf(min, 1, 2, str_1),
      dtostrf(max, 1, 2, str_2)
    );
  #endif
}

void Draw_MeshViewer() { MeshViewer.Draw(true, meshredraw); }

void onClick_MeshViewer() { if (HMI_flag.select_flag) SaveMesh(); HMI_ReturnScreen(); }

void Goto_MeshViewer(bool redraw) { 
  meshredraw = redraw;
  if (leveling_is_valid()) Goto_Popup(Draw_MeshViewer, onClick_MeshViewer);
  else HMI_ReturnScreen();
}

#endif // DWIN_LCD_PROUI && HAS_MESH
=======
  DWINUI::clearMainArea();
  dwinDrawRectangle(0, hmiData.colorSplitLine, px(0), py(0), px(sizex - 1), py(sizey - 1));
  for (uint8_t x = 1; x < sizex - 1; ++x) dwinDrawVLine(hmiData.colorSplitLine, px(x), py(sizey - 1), width);
  for (uint8_t y = 1; y < sizey - 1; ++y) dwinDrawHLine(hmiData.colorSplitLine, px(0), py(y), width);
}

void MeshViewer::drawPoint(const uint8_t x, const uint8_t y, const float z) {
  if (isnan(z)) return;
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    ui.refresh_backlight_timeout();
  #endif
  const uint8_t fs = DWINUI::fontWidth(meshfont);
  int16_t v = round(z * 100);
  NOLESS(max, z); NOMORE(min, z);
  const uint16_t color = DWINUI::rainbowInt(v, zmin, zmax);
  DWINUI::drawFillCircle(color, px(x), py(y), r(_MAX(_MIN(v,zmax),zmin)));
  if (sizex < (ENABLED(TJC_DISPLAY) ? 8 : 9)) {
    if (v == 0) dwinDrawString(false, meshfont, DWINUI::textColor, DWINUI::backColor, px(x) - 2*fs - 1, py(y) - fs, "0.00");
    else DWINUI::drawSignedFloat(meshfont, 1, 2, px(x) - 3*fs, py(y) - fs, z);
  }
  else {
    char str_1[9] = "0";
    switch (v) {
      case -99 ... -1:  // -.99 .. -.01 mm
        sprintf_P(str_1, PSTR("-.%02i"), -v);
        break;
      case 1 ... 99:  // .01 .. .99 mm
        sprintf_P(str_1, PSTR(".%02i"), v);
        break;
      case -999 ... -100:  // -9.9 .. -1.0 mm
      case  100 ...  999:  // 1.0 .. 9.9 mm
        dtostrf(z, 1, 1, str_1);
        break;
    }
    dwinDrawString(false, meshfont, DWINUI::textColor, DWINUI::backColor, px(x) - 2 * fs, py(y) - fs, str_1);
  }
  SERIAL_FLUSH();
  TERN_(TJC_DISPLAY, delay(100));
}

void MeshViewer::drawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey) {
  drawBackground(csizex, csizey);
  for (uint8_t y = 0; y < csizey; ++y) {
    hal.watchdog_refresh();
    for (uint8_t x = 0; x < csizex; ++x) drawPoint(x, y, zval[x][y]);
  }
}

void MeshViewer::drawViewer(const bool withsave/*=false*/, const bool redraw/*=true*/) {
  title.showCaption(GET_TEXT_F(MSG_MESH_VIEWER));
  if (redraw) drawMesh(bedlevel.z_values, GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y); // Draw complete mesh viewer
  else DWINUI::drawBox(1, hmiData.colorBackground, { 89, 305, 99, 38 }); // Erase "Continue" button
  if (withsave) {
    DWINUI::drawButton(BTN_Save, 26, 305);
    DWINUI::drawButton(BTN_Continue, 146, 305);
    drawSelectHighlight(hmiFlag.select_flag, 305);
  }
  else DWINUI::drawButton(BTN_Continue, 86, 305);
  char str_1[6], str_2[6] = "";
  ui.status_printf(0, F("Mesh minZ: %s, maxZ: %s"),
    dtostrf(min, 1, 2, str_1),
    dtostrf(max, 1, 2, str_2)
  );
}

void drawMeshViewer() { meshViewer.drawViewer(true, meshredraw); }

void onClick_MeshViewer() { if (hmiFlag.select_flag) saveMesh(); hmiReturnScreen(); }

void gotoMeshViewer(const bool redraw) {
  meshredraw = redraw;
  if (leveling_is_valid()) gotoPopup(drawMeshViewer, onClick_MeshViewer);
  else hmiReturnScreen();
}

#endif // DWIN_LCD_PROUI && HAS_MESH && DISABLED(PROUI_EX)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
