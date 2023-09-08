/**
 * DWIN Enhanced implementation for PRO UI
 * Author: Miguel A. Risco-Castillo (MRISCOC)
<<<<<<< HEAD
 * Version: 3.25.3
 * Date: 2023/05/18
=======
 * Version: 4.2.3
 * Date: 2023/08/04
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(DWIN_LCD_PROUI)

#include "../../utf8.h"
#include "../../marlinui.h"
#include "../../../MarlinCore.h"
#include "../../../core/serial.h"
#include "../../../core/macros.h"
#include "../../../module/temperature.h"
#include "../../../module/printcounter.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../gcode/gcode.h"
#include "../../../gcode/queue.h"

#if HAS_MEDIA
  #include "../../../sd/cardreader.h"
  #include "file_header.h"
#endif

#if NEED_HEX_PRINT
  #include "../../../libs/hex_print.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "../../../feature/runout.h"
#endif

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../../feature/host_actions.h"
#endif

#if HAS_MESH || HAS_BED_PROBE
  #include "../../../feature/bedlevel/bedlevel.h"
  #include "bedlevel_tools.h"
#endif

#if HAS_BED_PROBE
  #include "../../../module/probe.h"
#endif

#if ENABLED(BLTOUCH)
  #include "../../../feature/bltouch.h"
#endif

<<<<<<< HEAD
#if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
=======
#if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #include "../../../feature/babystep.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif

#if ENABLED(PRINTCOUNTER)
  #include "printstats.h"
#endif

#if ENABLED(CASE_LIGHT_MENU)
  #include "../../../feature/caselight.h"
#endif

#if ENABLED(LED_CONTROL_MENU)
  #include "../../../feature/leds/leds.h"
#endif

#if HAS_TRINAMIC_CONFIG
  #include "../../../feature/tmc_util.h"
#endif

#include "dwin.h"
#include "menus.h"
#include "dwin_popup.h"

#if HAS_GCODE_PREVIEW
  #include "gcode_preview.h"
#endif

#if HAS_TOOLBAR
  #include "toolbar.h"
#endif

#if HAS_ESDIAG
  #include "endstop_diag.h"
#endif

#if HAS_PLOT
  #include "plot.h"
#endif

#if HAS_MESH
  #include "meshviewer.h"
#endif

#if HAS_LOCKSCREEN
  #include "lockscreen.h"
#endif

<<<<<<< HEAD
=======
#if ALL(PROUI_EX, LCD_BED_TRAMMING)
  #include "bed_tramming.h"
#endif

>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#define DEBUG_OUT ENABLED(DEBUG_DWIN)
#include "../../../core/debug_out.h"

#define PAUSE_HEAT

// Print speed limit
#define MIN_PRINT_SPEED  10
#define MAX_PRINT_SPEED 999

// Print flow limit
#define MIN_PRINT_FLOW   10
#define MAX_PRINT_FLOW   299

// Load and Unload limits
#define MAX_LOAD_UNLOAD  500

// Juntion deviation limits
#define MIN_JD_MM             0.001
#define MAX_JD_MM             TERN(LIN_ADVANCE, 0.3f, 0.5f)

#if HAS_TRINAMIC_CONFIG
  #define MIN_TMC_CURRENT 100
  #define MAX_TMC_CURRENT 3000
#endif

// Editable temperature limits
#define MIN_ETEMP  0
<<<<<<< HEAD
#define MAX_ETEMP  (thermalManager.hotend_maxtemp[0] - (HOTEND_OVERSHOOT))
=======
#define MAX_ETEMP  thermalManager.hotend_max_target(EXT)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#define MIN_BEDTEMP 0
#define MAX_BEDTEMP BED_MAX_TARGET

#define DWIN_VAR_UPDATE_INTERVAL         1024
#define DWIN_UPDATE_INTERVAL             1024

<<<<<<< HEAD
#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, HMI_data.ManualZOffset)

#if ENABLED(MEDIASORT_MENU_ITEM) && DISABLED(SDCARD_SORT_ALPHA)
  #error "MEDIASORT_MENU_ITEM requires SDCARD_SORT_ALPHA."
#endif

#if ENABLED(RUNOUT_TUNE_ITEM) && DISABLED(HAS_FILAMENT_SENSOR)
  #error "RUNOUT_TUNE_ITEM requires HAS_FILAMENT_SENSOR."
#endif

#if ENABLED(PLR_TUNE_ITEM) && DISABLED(POWER_LOSS_RECOVERY)
  #error "PLR_TUNE_ITEM requires POWER_LOSS_RECOVERY."
#endif

#if ENABLED(JD_TUNE_ITEM) && DISABLED(HAS_JUNCTION_DEVIATION)
  #error "JD_TUNE_ITEM requires HAS_JUNCTION_DEVIATION."
#endif

#if ENABLED(ADVK_TUNE_ITEM) && DISABLED(LIN_ADVANCE)
  #error "ADVK_TUNE_ITEM requires LIN_ADVANCE."
#endif

// Structs
HMI_value_t HMI_value;
HMI_flag_t HMI_flag{0};
HMI_data_t HMI_data;
=======
#define BABY_Z_VAR TERN(HAS_BED_PROBE, probe.offset.z, hmiData.manualZOffset)

// Structs
hmi_value_t hmiValue;
hmi_flag_t hmiFlag{0};
hmi_data_t hmiData;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

enum SelectItem : uint8_t {
  PAGE_PRINT = 0,
  PAGE_PREPARE,
  PAGE_CONTROL,
  PAGE_ADVANCE,
#if HAS_TOOLBAR
  PAGE_TOOLBAR,
#endif
  PAGE_COUNT,

  PRINT_SETUP = 0,
  PRINT_PAUSE_RESUME,
  PRINT_STOP,
  PRINT_COUNT
};

typedef struct {
  uint8_t now, last;
  void set(uint8_t v) { now = last = v; }
  void reset() { set(0); }
  bool changed() { bool c = (now != last); if (c) last = now; return c; }
  bool dec() { if (now) now--; return changed(); }
  bool inc(uint8_t v) { if (now < (v - 1)) now++; else now = (v - 1); return changed(); }
} select_t;
select_t select_page{0}, select_print{0};

<<<<<<< HEAD
#if ENABLED(LCD_BED_TRAMMING)
  constexpr float bed_tramming_inset_lfbr[] = BED_TRAMMING_INSET_LFRB;
#endif

bool hash_changed = true; // Flag to know if message status was changed
bool blink = false;
uint8_t checkkey = 255, last_checkkey = MainMenu;

char DateTime[16+1] =
=======
bool hash_changed = true; // Flag to know if message status was changed
bool blink = false;
uint8_t checkkey = 255, last_checkkey = ID_MainMenu;

char dateTime[16+1] =
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
{
  // YY year
  __DATE__[7], __DATE__[8],__DATE__[9], __DATE__[10],
  // First month letter, Oct Nov Dec = '1' otherwise '0'
  (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',
  // Second month letter
  (__DATE__[0] == 'J') ? ( (__DATE__[1] == 'a') ? '1' :       // Jan, Jun or Jul
                          ((__DATE__[2] == 'n') ? '6' : '7') ) :
<<<<<<< HEAD
  (__DATE__[0] == 'F') ? '2' :                                // Feb 
=======
  (__DATE__[0] == 'F') ? '2' :                                // Feb
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' :   // Mar or May
  (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' :   // Apr or Aug
  (__DATE__[0] == 'S') ? '9' :                                // Sep
  (__DATE__[0] == 'O') ? '0' :                                // Oct
  (__DATE__[0] == 'N') ? '1' :                                // Nov
  (__DATE__[0] == 'D') ? '2' :                                // Dec
  0,
  // First day letter, replace space with digit
  __DATE__[4]==' ' ? '0' : __DATE__[4],
  // Second day letter
  __DATE__[5],
  // Separator
  ' ','-',' ',
  // Time
  __TIME__[0],__TIME__[1],__TIME__[2],__TIME__[3],__TIME__[4],
  '\0'
};

// New menu system pointers
<<<<<<< HEAD
MenuClass *FileMenu = nullptr;
MenuClass *PrepareMenu = nullptr;
MenuClass *TrammingMenu = nullptr;
MenuClass *MoveMenu = nullptr;
MenuClass *ControlMenu = nullptr;
MenuClass *AdvancedSettings = nullptr;
#if HAS_HOME_OFFSET
  MenuClass *HomeOffMenu = nullptr;
#endif
#if HAS_BED_PROBE
  MenuClass *ProbeSetMenu = nullptr;
#endif
MenuClass *FilSetMenu = nullptr;
MenuClass *SelectColorMenu = nullptr;
MenuClass *GetColorMenu = nullptr;
MenuClass *TuneMenu = nullptr;
MenuClass *MotionMenu = nullptr;
MenuClass *FilamentMenu = nullptr;
#if ENABLED(MESH_BED_LEVELING)
  MenuClass *ManualMesh = nullptr;
#endif
#if HAS_PREHEAT
  MenuClass *PreheatMenu = nullptr;
  MenuClass *PreheatHotendMenu = nullptr;
#endif
MenuClass *TemperatureMenu = nullptr;
MenuClass *MaxSpeedMenu = nullptr;
MenuClass *MaxAccelMenu = nullptr;
#if HAS_CLASSIC_JERK
  MenuClass *MaxJerkMenu = nullptr;
#endif
MenuClass *StepsMenu = nullptr;
#if EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  MenuClass *HotendMPCMenu = nullptr;
#endif
#if ENABLED(PIDTEMP) && EITHER(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  MenuClass *HotendPIDMenu = nullptr;
#endif
#if ENABLED(PIDTEMPBED) && EITHER(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  MenuClass *BedPIDMenu = nullptr;
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  MenuClass *CaseLightMenu = nullptr;
#endif
#if ENABLED(LED_CONTROL_MENU)
  MenuClass *LedControlMenu = nullptr;
#endif
#if HAS_BED_PROBE
  MenuClass *ZOffsetWizMenu = nullptr;
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  MenuClass *HomingMenu = nullptr;
#endif
#if ENABLED(FWRETRACT)
  MenuClass *FWRetractMenu = nullptr;
#endif
#if PROUI_EX
  #if ENABLED(NOZZLE_PARK_FEATURE)
     MenuClass *ParkPosMenu = nullptr;
  #endif
  MenuClass *PhySetMenu = nullptr;
#endif
#if HAS_TOOLBAR
  MenuClass *TBSetupMenu = nullptr;
#endif
#if HAS_MESH
  MenuClass *MeshMenu = nullptr;
  #if ENABLED(MESH_EDIT_MENU)
    MenuClass *EditMeshMenu = nullptr;
  #endif
  #if PROUI_EX
    MenuClass *MeshInsetMenu = nullptr;
  #endif
#endif
#if ENABLED(SHAPING_MENU)
  MenuClass *InputShapingMenu = nullptr;
#endif
#if HAS_TRINAMIC_CONFIG
  MenuClass *TrinamicConfigMenu = nullptr;
#endif
#if ENABLED(CV_LASER_MODULE)
  MenuClass *LaserSettings = nullptr;
  MenuClass *LaserPrintMenu = nullptr;
#endif

// Updatable menuitems pointers
MenuItemClass *HotendTargetItem = nullptr;
MenuItemClass *BedTargetItem = nullptr;
MenuItemClass *FanSpeedItem = nullptr;
MenuItemClass *MMeshMoveZItem = nullptr;
MenuItemClass *EditZValueItem = nullptr;

bool Printing() { return (printingIsActive() || print_job_timer.isPaused()); }
bool SD_Printing() { return (Printing() && IS_SD_FILE_OPEN()); }
bool Host_Printing() { return (Printing() && !IS_SD_FILE_OPEN()); }
=======
Menu *fileMenu = nullptr;
Menu *prepareMenu = nullptr;
Menu *moveMenu = nullptr;
Menu *controlMenu = nullptr;
Menu *advancedSettings = nullptr;
#if HAS_HOME_OFFSET
  Menu *homeOffMenu = nullptr;
#endif
#if HAS_BED_PROBE
  Menu *probeSetMenu = nullptr;
#endif
Menu *filSetMenu = nullptr;
Menu *selectColorMenu = nullptr;
Menu *getColorMenu = nullptr;
Menu *tuneMenu = nullptr;
Menu *motionMenu = nullptr;
Menu *filamentMenu = nullptr;
#if ENABLED(MESH_BED_LEVELING)
  Menu *manualMesh = nullptr;
#endif
#if HAS_PREHEAT
  Menu *preheatMenu = nullptr;
  Menu *preheatHotendMenu = nullptr;
#endif
Menu *temperatureMenu = nullptr;
Menu *maxSpeedMenu = nullptr;
Menu *maxAccelMenu = nullptr;
#if HAS_CLASSIC_JERK
  Menu *maxJerkMenu = nullptr;
#endif
Menu *stepsMenu = nullptr;
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  Menu *hotendMPCMenu = nullptr;
#endif
#if ENABLED(PIDTEMP) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  Menu *hotendPIDMenu = nullptr;
#endif
#if ENABLED(PIDTEMPBED) && ANY(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
  Menu *bedPIDMenu = nullptr;
#endif
#if ENABLED(CASELIGHT_USES_BRIGHTNESS)
  Menu *caseLightMenu = nullptr;
#endif
#if ENABLED(LED_CONTROL_MENU)
  Menu *ledControlMenu = nullptr;
#endif
#if HAS_BED_PROBE
  Menu *zOffsetWizMenu = nullptr;
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  Menu *homingMenu = nullptr;
#endif
#if ENABLED(FWRETRACT)
  Menu *fwRetractMenu = nullptr;
#endif
#if PROUI_EX
  #if ENABLED(NOZZLE_PARK_FEATURE)
     Menu *parkPosMenu = nullptr;
  #endif
  #if ENABLED(LCD_BED_TRAMMING)
    Menu *trammingMenu = nullptr;
  #endif
  Menu *phySetMenu = nullptr;
#endif
#if HAS_TOOLBAR
  Menu *tbSetupMenu = nullptr;
#endif
#if HAS_MESH
  Menu *meshMenu = nullptr;
  #if ENABLED(MESH_EDIT_MENU)
    Menu *editMeshMenu = nullptr;
  #endif
  #if PROUI_EX
    Menu *meshInsetMenu = nullptr;
  #endif
#endif
#if ENABLED(SHAPING_MENU)
  Menu *inputShapingMenu = nullptr;
#endif
#if HAS_TRINAMIC_CONFIG
  Menu *trinamicConfigMenu = nullptr;
#endif
#if ENABLED(CV_LASER_MODULE)
  Menu *laserSettings = nullptr;
  Menu *laserPrintMenu = nullptr;
#endif

// Updatable menuitems pointers
MenuItem *hotendTargetItem = nullptr;
MenuItem *bedTargetItem = nullptr;
MenuItem *fanSpeedItem = nullptr;
MenuItem *mMeshMoveZItem = nullptr;
MenuItem *editZValueItem = nullptr;

bool isPrinting() { return (printingIsActive() || print_job_timer.isPaused()); }
bool sdPrinting() { return (isPrinting() && IS_SD_FILE_OPEN()); }
bool hostPrinting() { return (isPrinting() && !IS_SD_FILE_OPEN()); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

//-----------------------------------------------------------------------------
// Main Buttons
//-----------------------------------------------------------------------------

<<<<<<< HEAD
void ICON_Button(const bool selected, const int iconid, const frame_rect_t &ico, FSTR_P caption) {
  DWINUI::Draw_IconWB(iconid + selected, ico.x, ico.y);
  if (selected) DWINUI::Draw_Box(0, HMI_data.Highlight_Color, ico);
  const uint16_t x = ico.x + (ico.w - strlen_P(FTOP(caption)) * DWINUI::fontWidth()) / 2,
                 y = (ico.y + ico.h - 20) - DWINUI::fontHeight() / 2;
  DWINUI::Draw_String(x, y, caption);
=======
void iconButton(const bool selected, const int iconid, const frame_rect_t &ico, FSTR_P caption) {
  DWINUI::drawIconWB(iconid + selected, ico.x, ico.y);
  if (selected) DWINUI::drawBox(0, hmiData.colorHighlight, ico);
  const uint16_t x = ico.x + (ico.w - strlen_P(FTOP(caption)) * DWINUI::fontWidth()) / 2,
                 y = (ico.y + ico.h - 20) - DWINUI::fontHeight() / 2;
  DWINUI::drawString(x, y, caption);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Main Menu: "Print"
//
<<<<<<< HEAD
void ICON_Print() {
  constexpr frame_rect_t ico = { 17, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  ICON_Button(select_page.now == PAGE_PRINT, ICON_Print_0, ico, GET_TEXT_F(MSG_BUTTON_MEDIA));
=======
void iconPrint() {
  constexpr frame_rect_t ico = { 17, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_PRINT, ICON_Print_0, ico, GET_TEXT_F(MSG_BUTTON_MEDIA));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Main Menu: "Prepare"
//
<<<<<<< HEAD
void ICON_Prepare() {
  constexpr frame_rect_t ico = { 145, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  ICON_Button(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, GET_TEXT_F(MSG_PREPARE));
=======
void iconPrepare() {
  constexpr frame_rect_t ico = { 145, 110 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_PREPARE, ICON_Prepare_0, ico, GET_TEXT_F(MSG_PREPARE));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Main Menu: "Control"
//
<<<<<<< HEAD
void ICON_Control() {
  constexpr frame_rect_t ico = { 17, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  ICON_Button(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, GET_TEXT_F(MSG_CONTROL));
=======
void iconControl() {
  constexpr frame_rect_t ico = { 17, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_CONTROL, ICON_Control_0, ico, GET_TEXT_F(MSG_CONTROL));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Main Menu: "Advanced Settings"
//
<<<<<<< HEAD
void ICON_AdvSettings() {
  constexpr frame_rect_t ico = { 145, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  ICON_Button(select_page.now == PAGE_ADVANCE, ICON_Info_0, ico, GET_TEXT_F(MSG_BUTTON_ADVANCED));
=======
void iconAdvSettings() {
  constexpr frame_rect_t ico = { 145, 226 - TERN0(HAS_TOOLBAR,TBYOFFSET), 110, 100};
  iconButton(select_page.now == PAGE_ADVANCE, ICON_Info_0, ico, GET_TEXT_F(MSG_BUTTON_ADVANCED));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Printing: "Tune"
//
<<<<<<< HEAD
void ICON_Tune() {
  constexpr frame_rect_t ico = { 8, 232, 80, 100 };
  ICON_Button(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, GET_TEXT_F(MSG_TUNE));
=======
void iconTune() {
  constexpr frame_rect_t ico = { 8, 232, 80, 100 };
  iconButton(select_print.now == PRINT_SETUP, ICON_Setup_0, ico, GET_TEXT_F(MSG_TUNE));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Printing: "Pause"
//
<<<<<<< HEAD
void ICON_Pause() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, GET_TEXT_F(MSG_BUTTON_PAUSE));
=======
void iconPause() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  iconButton(select_print.now == PRINT_PAUSE_RESUME, ICON_Pause_0, ico, GET_TEXT_F(MSG_BUTTON_PAUSE));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Printing: "Resume"
//
<<<<<<< HEAD
void ICON_Resume() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  ICON_Button(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, GET_TEXT_F(MSG_BUTTON_RESUME));
=======
void iconResume() {
  constexpr frame_rect_t ico = { 96, 232, 80, 100 };
  iconButton(select_print.now == PRINT_PAUSE_RESUME, ICON_Continue_0, ico, GET_TEXT_F(MSG_BUTTON_RESUME));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Printing: "Stop"
//
<<<<<<< HEAD
void ICON_Stop() {
  constexpr frame_rect_t ico = { 184, 232, 80, 100 };
  ICON_Button(select_print.now == PRINT_STOP, ICON_Stop_0, ico, GET_TEXT_F(MSG_BUTTON_STOP));
}

//
//PopUps
//
void Popup_window_PauseOrStop() {
  DWIN_Popup_ConfirmCancel(ICON_BLTouch, select_print.now == PRINT_PAUSE_RESUME ? GET_TEXT_F(MSG_PAUSE_PRINT) : GET_TEXT_F(MSG_STOP_PRINT));
}

#if HAS_HOTEND || HAS_HEATED_BED
  void DWIN_Popup_Temperature(const int_fast8_t heater_id, const bool toohigh) {
    FSTR_P heaterstr = heater_id == H_BED ? F("Bed temperature") : F("Nozzle temperature");
    FSTR_P lowhighstr = toohigh ? F("is too high") : F("is too low");
    HMI_SaveProcessID(WaitResponse);
    DWIN_Show_Popup(toohigh ? ICON_TempTooHigh : ICON_TempTooLow, heaterstr, lowhighstr, BTN_Continue);
=======
void iconStop() {
  constexpr frame_rect_t ico = { 184, 232, 80, 100 };
  iconButton(select_print.now == PRINT_STOP, ICON_Stop_0, ico, GET_TEXT_F(MSG_BUTTON_STOP));
}

//
// PopUps
//
void popupPauseOrStop() {
  dwinPopupConfirmCancel(ICON_BLTouch, select_print.now == PRINT_PAUSE_RESUME ? GET_TEXT_F(MSG_PAUSE_PRINT) : GET_TEXT_F(MSG_STOP_PRINT));
}

#if HAS_HOTEND || HAS_HEATED_BED
  void dwinPopupTemperature(const int_fast8_t heater_id, const bool toohigh) {
    FSTR_P heaterstr = heater_id == H_BED ? F("Bed temperature") : F("Nozzle temperature");
    FSTR_P lowhighstr = toohigh ? F("is too high") : F("is too low");
    hmiSaveProcessID(ID_WaitResponse);
    dwinShowPopup(toohigh ? ICON_TempTooHigh : ICON_TempTooLow, heaterstr, lowhighstr, BTN_Continue);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

//
// Draw status line
//
<<<<<<< HEAD
void DWIN_DrawStatusLine(const char *text) {
  DWIN_Draw_Rectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
  if (text) DWINUI::Draw_CenteredString(HMI_data.StatusTxt_Color, STATUS_Y + 2, text);
}
void DWIN_DrawStatusLine(FSTR_P fstr) { DWIN_DrawStatusLine(FTOP(fstr)); }

// Clear & reset status line
void DWIN_ResetStatusLine() {
  ui.status_message[0] = 0;
  DWIN_CheckStatusMessage();
}

// Djb2 hash algorithm
uint32_t GetHash(char * str) {
  uint32_t hash = 5381;
  char c;
  while ((c = *str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

void DWIN_CheckStatusMessage() {
  static uint32_t old_hash = 0;
  uint32_t hash = GetHash(&ui.status_message[0]);
  hash_changed = hash != old_hash;
  old_hash = hash;
};

void DWIN_DrawStatusMessage() {
  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    // Get the UTF8 character count of the string
    uint8_t slen = utf8_strlen(ui.status_message);
=======
void dwinDrawStatusLine(const char *text) {
  dwinDrawRectangle(1, hmiData.colorStatusBg, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
  if (text) DWINUI::drawCenteredString(hmiData.colorStatusTxt, STATUS_Y + 2, text);
}
void dwinDrawStatusLine(FSTR_P fstr) { dwinDrawStatusLine(FTOP(fstr)); }

// Clear & reset status line
void dwinResetStatusLine() {
  ui.status_message.clear();
  dwinCheckStatusMessage();
}

// Check for a change in the status message
void dwinCheckStatusMessage() {
  static MString<>::hash_t old_hash = 0x0000;
  const MString<>::hash_t hash = ui.status_message.hash();
  hash_changed = hash != old_hash;
  old_hash = hash;
}

void dwinDrawStatusMessage() {
  #if ENABLED(STATUS_MESSAGE_SCROLLING)

    // Get the UTF8 character count of the string
    uint8_t slen = ui.status_message.glyphs();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

    // If the string fits the status line do not scroll it
    if (slen <= LCD_WIDTH) {
      if (hash_changed) {
<<<<<<< HEAD
        DWIN_DrawStatusLine(ui.status_message);
=======
        dwinDrawStatusLine(ui.status_message);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        hash_changed = false;
      }
    }
    else {
      // String is larger than the available line space

      // Get a pointer to the next valid UTF8 character
      // and the string remaining length
      uint8_t rlen;
<<<<<<< HEAD
      const char *stat = MarlinUI::status_and_len(rlen);
      DWIN_Draw_Rectangle(1, HMI_data.StatusBg_Color, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
      DWINUI::MoveTo(0, STATUS_Y + 2);
      DWINUI::Draw_String(HMI_data.StatusTxt_Color, stat, LCD_WIDTH);

      // If the string doesn't completely fill the line...
      if (rlen < LCD_WIDTH) {
        DWINUI::Draw_Char(HMI_data.StatusTxt_Color, '.');  // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;                  // Amount of space left in characters
        if (--chars) {                                     // Draw a second dot if there's space
          DWINUI::Draw_Char(HMI_data.StatusTxt_Color, '.');
          if (--chars)
            DWINUI::Draw_String(HMI_data.StatusTxt_Color, ui.status_message, chars); // Print a second copy of the message
        }
      }
      MarlinUI::advance_status_scroll();
=======
      const char *stat = ui.status_and_len(rlen);
      dwinDrawRectangle(1, hmiData.colorStatusBg, 0, STATUS_Y, DWIN_WIDTH, STATUS_Y + 20);
      DWINUI::moveTo(0, STATUS_Y + 2);
      DWINUI::drawString(hmiData.colorStatusTxt, stat, LCD_WIDTH);

      // If the string doesn't completely fill the line...
      if (rlen < LCD_WIDTH) {
        DWINUI::drawChar(hmiData.colorStatusTxt, '.');    // Always at 1+ spaces left, draw a dot
        uint8_t chars = LCD_WIDTH - rlen;                  // Amount of space left in characters
        if (--chars) {                                     // Draw a second dot if there's space
          DWINUI::drawChar(hmiData.colorStatusTxt, '.');
          if (--chars)
            DWINUI::drawString(hmiData.colorStatusTxt, ui.status_message, chars); // Print a second copy of the message
        }
      }
      ui.advance_status_scroll();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }

  #else

    if (hash_changed) {
<<<<<<< HEAD
      ui.status_message[LCD_WIDTH] = 0;
      DWIN_DrawStatusLine(ui.status_message);
=======
      ui.status_message.trunc(LCD_WIDTH);
      dwinDrawStatusLine(ui.status_message);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      hash_changed = false;
    }

  #endif
}

<<<<<<< HEAD
void Draw_Print_Labels() {
  DWINUI::Draw_String( 46, 173, GET_TEXT_F(MSG_INFO_PRINT_TIME));
  DWINUI::Draw_String(181, 173, GET_TEXT_F(MSG_REMAINING_TIME));
}

void Draw_Print_ProgressBar() {
  const uint8_t _percent_done = ui.get_progress_percent();
  DWINUI::Draw_IconWB(ICON_Bar, 15, 93);
  DWIN_Draw_Rectangle(1, HMI_data.Barfill_Color, 16 + (_percent_done * 240) / 100, 93, 256, 113);
  DWINUI::Draw_Int(HMI_data.PercentTxt_Color, HMI_data.Background_Color, 3, 117, 133, _percent_done);
  DWINUI::Draw_String(HMI_data.PercentTxt_Color, 142, 133, F("%"));
}

void Draw_Print_ProgressElapsed() {
  char buf[10];
  duration_t elapsed = print_job_timer.duration(); // print timer
  sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(elapsed.value / 3600), ((uint16_t)elapsed.value % 3600) / 60);
  DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 47, 192, buf);
}

void Draw_Print_ProgressRemain() {
  const uint32_t _remain_time = ui.get_remaining_time();
  char buf[10];
  sprintf_P(buf, PSTR("%02i:%02i "), (uint16_t)(_remain_time / 3600), ((uint16_t)_remain_time % 3600) / 60);
  DWINUI::Draw_String(HMI_data.Text_Color, HMI_data.Background_Color, 181, 192, buf);
}

void ICON_ResumeOrPause() {
  if (checkkey == PrintProcess) (print_job_timer.isPaused() || HMI_flag.pause_flag) ? ICON_Resume() : ICON_Pause();
}

// Update filename on print
void DWIN_Print_Header(const char *text = nullptr) {
  static char headertxt[31] = "";  // Print header text
  if (text) {
    const int8_t size = _MIN(30U, strlen_P(text));
    LOOP_L_N(i, size) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == PrintProcess || checkkey == PrintDone) {
    DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::Draw_CenteredString(60, headertxt);
  }
}

void Draw_PrintProcess() {
  #if ENABLED(CV_LASER_MODULE)
    Title.ShowCaption(laser_device.is_laser_device() ? GET_TEXT_F(MSG_ENGRAVING) : GET_TEXT_F(MSG_PRINTING));
  #else
    Title.ShowCaption(GET_TEXT_F(MSG_PRINTING));
  #endif
  DWINUI::ClearMainArea();
  DWIN_Print_Header(nullptr);
  Draw_Print_Labels();
  DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
  DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
  Draw_Print_ProgressBar();
  Draw_Print_ProgressElapsed();
  Draw_Print_ProgressRemain();
  ICON_Tune();
  ICON_ResumeOrPause();
  ICON_Stop();
}

void Goto_PrintProcess() {
  if (checkkey == PrintProcess)
    ICON_ResumeOrPause();
  else {
    checkkey = PrintProcess;
    Draw_PrintProcess();
    TERN_(DASH_REDRAW, DWIN_RedrawDash());
  }
  DWIN_UpdateLCD();
}

void Draw_PrintDone() {
  ui.set_progress_done();
  ui.reset_remaining_time();
  Title.ShowCaption(GET_TEXT_F(MSG_PRINT_DONE));
  DWINUI::ClearMainArea();
  DWIN_Print_Header(nullptr);
  #if HAS_GCODE_PREVIEW
    const bool haspreview = Preview_Valid();
    if (haspreview) {
      Preview_Show();
      DWINUI::Draw_Button(BTN_Continue, 86, 295);
=======
void drawPrintLabels() {
  DWINUI::drawString( 46, 173, GET_TEXT_F(MSG_INFO_PRINT_TIME));
  DWINUI::drawString(181, 173, GET_TEXT_F(MSG_REMAINING_TIME));
}

void drawPrintProgressBar() {
  const uint8_t _percent_done = ui.get_progress_percent();
  DWINUI::drawIconWB(ICON_Bar, 15, 93);
  dwinDrawRectangle(1, hmiData.colorBarfill, 16 + (_percent_done * 240) / 100, 93, 256, 113);
  DWINUI::drawInt(hmiData.colorPercentTxt, hmiData.colorBackground, 3, 117, 133, _percent_done);
  DWINUI::drawString(hmiData.colorPercentTxt, 142, 133, F("%"));
}

void drawPrintProgressElapsed() {
  MString<12> buf;
  duration_t elapsed = print_job_timer.duration(); // Print timer
  buf.setf(F("%02i:%02i "), uint16_t(elapsed.value / 3600), (uint16_t(elapsed.value) % 3600) / 60);
  DWINUI::drawString(hmiData.colorText, hmiData.colorBackground, 47, 192, buf);
}

#if ENABLED(SHOW_REMAINING_TIME)
  void drawPrintProgressRemain() {
    const uint32_t _remain_time = ui.get_remaining_time();
    MString<12> buf;
    buf.setf(F("%02i:%02i "), _remain_time / 3600, (_remain_time % 3600) / 60);
    DWINUI::drawString(hmiData.colorText, hmiData.colorBackground, 181, 192, buf);
  }
#endif

void iconResumeOrPause() {
  if (checkkey == ID_PrintProcess) (print_job_timer.isPaused() || hmiFlag.pause_flag) ? iconResume() : iconPause();
}

// Update filename on print
void dwinPrintHeader(const char *text = nullptr) {
  static char headertxt[31] = "";  // Print header text
  if (text) {
    const int8_t size = _MIN(30U, strlen_P(text));
    for (uint8_t i = 0; i < size; ++i) headertxt[i] = text[i];
    headertxt[size] = '\0';
  }
  if (checkkey == ID_PrintProcess || checkkey == ID_PrintDone) {
    dwinDrawRectangle(1, hmiData.colorBackground, 0, 60, DWIN_WIDTH, 60+16);
    DWINUI::drawCenteredString(60, headertxt);
  }
}

void drawPrintProcess() {
  #if ENABLED(CV_LASER_MODULE)
    title.showCaption(laser_device.is_laser_device() ? GET_TEXT_F(MSG_ENGRAVING) : GET_TEXT_F(MSG_PRINTING));
  #else
    title.showCaption(GET_TEXT_F(MSG_PRINTING));
  #endif
  DWINUI::clearMainArea();
  dwinPrintHeader(nullptr);
  drawPrintLabels();
  DWINUI::drawIcon(ICON_PrintTime, 15, 173);
  DWINUI::drawIcon(ICON_RemainTime, 150, 171);
  drawPrintProgressBar();
  drawPrintProgressElapsed();
  TERN_(SHOW_REMAINING_TIME, drawPrintProgressRemain());
  iconTune();
  iconResumeOrPause();
  iconStop();
}

void gotoPrintProcess() {
  if (checkkey == ID_PrintProcess)
    iconResumeOrPause();
  else {
    checkkey = ID_PrintProcess;
    drawPrintProcess();
    TERN_(DASH_REDRAW, dwinRedrawDash());
  }
  dwinUpdateLCD();
}

void drawPrintDone() {
  TERN_(SET_PROGRESS_PERCENT, ui.set_progress_done());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  title.showCaption(GET_TEXT_F(MSG_PRINT_DONE));
  DWINUI::clearMainArea();
  dwinPrintHeader(nullptr);
  #if HAS_GCODE_PREVIEW
    const bool haspreview = gPreview.isValid();
    if (haspreview) {
      gPreview.show();
      DWINUI::drawButton(BTN_Continue, 86, 295);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #else
    constexpr bool haspreview = false;
  #endif

  if (!haspreview) {
<<<<<<< HEAD
    Draw_Print_ProgressBar();
    Draw_Print_Labels();
    DWINUI::Draw_Icon(ICON_PrintTime, 15, 173);
    DWINUI::Draw_Icon(ICON_RemainTime, 150, 171);
    Draw_Print_ProgressElapsed();
    Draw_Print_ProgressRemain();
    DWINUI::Draw_Button(BTN_Continue, 86, 273);
  }
}

void Goto_PrintDone() {
  DEBUG_ECHOLNPGM("Goto_PrintDone");
  wait_for_user = true;
  if (checkkey != PrintDone) {
    checkkey = PrintDone;
    Draw_PrintDone();
    DWIN_UpdateLCD();
  }
}

void Draw_Main_Menu() {
  DWINUI::ClearMainArea();
  #if ENABLED(CV_LASER_MODULE)
    Title.ShowCaption(laser_device.is_laser_device() ? F("Laser Engraver") : F("3D Printer"));
  #else
    Title.ShowCaption(GET_TEXT_F(MSG_MAIN_MENU));
  #endif
  DWINUI::Draw_Icon(ICON_LOGO, 71, 52);   // CREALITY logo
  ICON_Print();
  ICON_Prepare();
  ICON_Control();
  ICON_AdvSettings();
  TERN_(HAS_TOOLBAR, Draw_ToolBar());
}

void Goto_Main_Menu() {
  if (checkkey == MainMenu) return;
  checkkey = MainMenu;
  Draw_Main_Menu();
  DWIN_UpdateLCD();
=======
    drawPrintProgressBar();
    drawPrintLabels();
    DWINUI::drawIcon(ICON_PrintTime, 15, 173);
    DWINUI::drawIcon(ICON_RemainTime, 150, 171);
    drawPrintProgressElapsed();
    TERN_(SHOW_REMAINING_TIME, drawPrintProgressRemain());
    DWINUI::drawButton(BTN_Continue, 86, 273);
  }
}

void gotoPrintDone() {
  DEBUG_ECHOLNPGM("gotoPrintDone");
  wait_for_user = true;
  if (checkkey != ID_PrintDone) {
    checkkey = ID_PrintDone;
    drawPrintDone();
    dwinUpdateLCD();
  }
}

void drawMainMenu() {
  DWINUI::clearMainArea();
  #if ENABLED(CV_LASER_MODULE)
    title.showCaption(laser_device.is_laser_device() ? F("Laser Engraver") : F("3D Printer"));
  #else
    title.showCaption(GET_TEXT_F(MSG_MAIN_MENU));
  #endif
  DWINUI::drawIcon(ICON_LOGO, 71, 52);   // CREALITY logo
  iconPrint();
  iconPrepare();
  iconControl();
  iconAdvSettings();
  TERN_(HAS_TOOLBAR, drawToolBar());
}

void gotoMainMenu() {
  if (checkkey == ID_MainMenu) return;
  checkkey = ID_MainMenu;
  drawMainMenu();
  dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

// Draw X, Y, Z and blink if in an un-homed or un-trusted state
void _update_axis_value(const AxisEnum axis, const uint16_t x, const uint16_t y, const bool force) {
  const bool draw_qmark = axis_should_home(axis),
             draw_empty = NONE(HOME_AFTER_DEACTIVATE, DISABLE_REDUCED_ACCURACY_WARNING) && !draw_qmark && !axis_is_trusted(axis);

  // Check for a position change
  static xyz_pos_t oldpos = { -1, -1, -1 };

<<<<<<< HEAD
  #if BOTH(IS_FULL_CARTESIAN, SHOW_REAL_POS)
=======
  #if ALL(IS_FULL_CARTESIAN, SHOW_REAL_POS)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    const float p = planner.get_axis_position_mm(axis);
  #else
    const float p = current_position[axis];
  #endif

  const bool changed = oldpos[axis] != p;
  if (changed) oldpos[axis] = p;

  if (force || changed || draw_qmark || draw_empty) {
    if (blink && draw_qmark)
<<<<<<< HEAD
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("  - ? -"));
    else if (blink && draw_empty)
      DWINUI::Draw_String(HMI_data.Coordinate_Color, HMI_data.Background_Color, x, y, F("       "));
    else
      DWINUI::Draw_Signed_Float(HMI_data.Coordinate_Color, HMI_data.Background_Color, 3, 2, x, y, p);
  }
}

void _draw_iconblink(bool &flag, const bool sensor, const uint8_t icon1, const uint8_t icon2, const uint16_t x, const uint16_t y) {
=======
      DWINUI::drawString(hmiData.colorCoordinate, hmiData.colorBackground, x, y, F("  - ? -"));
    else if (blink && draw_empty)
      DWINUI::drawString(hmiData.colorCoordinate, hmiData.colorBackground, x, y, F("       "));
    else
      DWINUI::drawSignedFloat(hmiData.colorCoordinate, hmiData.colorBackground, 3, 2, x, y, p);
  }
}

void _drawIconBlink(bool &flag, const bool sensor, const uint8_t icon1, const uint8_t icon2, const uint16_t x, const uint16_t y) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if DISABLED(NO_BLINK_IND)
    if (flag != sensor) {
      flag = sensor;
      if (!flag) {
<<<<<<< HEAD
        DWIN_Draw_Box(1, HMI_data.Background_Color, x, y, 20, 20);
        DWINUI::Draw_Icon(icon1, x, y);
      }
    }
    if (flag) {
      DWIN_Draw_Box(1, blink ? HMI_data.SplitLine_Color : HMI_data.Background_Color, x, y, 20, 20);
      DWINUI::Draw_Icon(icon2, x, y);
=======
        dwinDrawBox(1, hmiData.colorBackground, x-1, y-1, 22, 22);
        DWINUI::drawIcon(icon1, x, y);
      }
    }
    if (flag) {
      dwinDrawBox(1, blink ? hmiData.colorSplitLine : hmiData.colorBackground, x-1, y-1, 22, 22);
      DWINUI::drawIcon(icon2, x, y);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #else
    if (flag != sensor) {
      flag = sensor;
<<<<<<< HEAD
      DWIN_Draw_Box(1, HMI_data.Background_Color, x, y, 20, 20);
      DWINUI::Draw_Icon(flag ? icon2 : icon1, x, y);
=======
      dwinDrawBox(1, hmiData.colorBackground, x, y, 20, 20);
      DWINUI::drawIcon(flag ? icon2 : icon1, x, y);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif
}

<<<<<<< HEAD
void _draw_ZOffsetIcon() {
  #if HAS_LEVELING
    static bool _leveling_active = false;
    _draw_iconblink(_leveling_active, planner.leveling_active, ICON_Zoffset, ICON_SetZOffset, 186, 416);
  #else
    DWINUI::Draw_Icon(ICON_Zoffset, 187, 416);
  #endif
}

#if HAS_FILAMENT_SENSOR && PROUI_EX
  void _draw_runout_icon() {
    static bool _runout_active = false;
    _draw_iconblink(_runout_active, FilamentSensorDevice::poll_runout_state(0), ICON_StepE, ICON_FilRunOut, 112, 416);
  }
#endif

void _draw_feedrate() {
=======
void _drawZOffsetIcon() {
  #if HAS_LEVELING
    static bool _leveling_active = false;
    _drawIconBlink(_leveling_active, planner.leveling_active, ICON_Zoffset, ICON_SetZOffset, 186, 416);
  #else
    DWINUI::drawIcon(ICON_Zoffset, 187, 416);
  #endif
}

#if HAS_PROUI_RUNOUT_SENSOR
  void _drawRunoutIcon() {
    static bool _runout_active = false;
    _drawIconBlink(_runout_active, !!FilamentSensorBase::poll_runout_states(), ICON_StepE, ICON_FilRunOut, 112, 416);
  }
#endif

void _drawFeedrate() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if ENABLED(SHOW_SPEED_IND)
    int16_t _value;
    if (blink) {
      _value = feedrate_percentage;
<<<<<<< HEAD
      DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 4 * STAT_CHR_W + 2, 384, F(" %"));
    }
    else {
      _value = CEIL(feedrate_mm_s * feedrate_percentage / 100);
      DWIN_Draw_Box(1, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, 20, 20);
    }
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _value);
=======
      DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 4 * STAT_CHR_W + 2, 384, F(" %"));
    }
    else {
      _value = round(feedrate_mm_s * feedrate_percentage / 100);
      dwinDrawBox(1, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 384, 20, 20);
    }
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, _value);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #else
    static int16_t _feedrate = 100;
    if (_feedrate != feedrate_percentage) {
      _feedrate = feedrate_percentage;
<<<<<<< HEAD
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
=======
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, _feedrate);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif
}

<<<<<<< HEAD
void _draw_xyz_position(const bool force) {
=======
void _drawXYZPosition(const bool force) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  _update_axis_value(X_AXIS,  27, 459, force);
  _update_axis_value(Y_AXIS, 112, 459, force);
  _update_axis_value(Z_AXIS, 197, 459, force);
}

<<<<<<< HEAD
void update_variable() {
  TERN_(DEBUG_DWIN,DWINUI::Draw_Int(Color_Yellow, Color_Bg_Black, 2, DWIN_WIDTH-6*DWINUI::fontWidth(), 6, checkkey));
  TERN_(DEBUG_DWIN,DWINUI::Draw_Int(Color_Yellow, Color_Bg_Black, 2, DWIN_WIDTH-3*DWINUI::fontWidth(), 6, last_checkkey));

  _draw_xyz_position(false);
=======
void updateVariable() {
  TERN_(DEBUG_DWIN,DWINUI::drawInt(COLOR_YELLOW, COLOR_BG_BLACK, 2, DWIN_WIDTH-6*DWINUI::fontWidth(), 6, checkkey));
  TERN_(DEBUG_DWIN,DWINUI::drawInt(COLOR_YELLOW, COLOR_BG_BLACK, 2, DWIN_WIDTH-3*DWINUI::fontWidth(), 6, last_checkkey));

  _drawXYZPosition(false);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

  TERN_(CV_LASER_MODULE, if (laser_device.is_laser_device()) return);

  #if HAS_HOTEND
    static celsius_t _hotendtemp = 0, _hotendtarget = 0;
<<<<<<< HEAD
    const celsius_t hc = thermalManager.wholeDegHotend(0),
                    ht = thermalManager.degTargetHotend(0);
=======
    const celsius_t hc = thermalManager.wholeDegHotend(EXT),
                    ht = thermalManager.degTargetHotend(EXT);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    const bool _new_hotend_temp = _hotendtemp != hc,
               _new_hotend_target = _hotendtarget != ht;
    if (_new_hotend_temp) _hotendtemp = hc;
    if (_new_hotend_target) _hotendtarget = ht;
  #endif
  #if HAS_HEATED_BED
    static celsius_t _bedtemp = 0, _bedtarget = 0;
    const celsius_t bc = thermalManager.wholeDegBed(),
                    bt = thermalManager.degTargetBed();
    const bool _new_bed_temp = _bedtemp != bc,
               _new_bed_target = _bedtarget != bt;
    if (_new_bed_temp) _bedtemp = bc;
    if (_new_bed_target) _bedtarget = bt;
  #endif
  #if HAS_FAN
    static uint8_t _fanspeed = 0;
<<<<<<< HEAD
    const bool _new_fanspeed = _fanspeed != thermalManager.fan_speed[0];
    if (_new_fanspeed) _fanspeed = thermalManager.fan_speed[0];
  #endif

  if (IsMenu(TuneMenu) || IsMenu(TemperatureMenu)) {
    // Tune page temperature update
    TERN_(HAS_HOTEND, if (_new_hotend_target) HotendTargetItem->redraw());
    TERN_(HAS_HEATED_BED, if (_new_bed_target) BedTargetItem->redraw());
    TERN_(HAS_FAN, if (_new_fanspeed) FanSpeedItem->redraw());
=======
    const bool _new_fanspeed = _fanspeed != thermalManager.fan_speed[FAN];
    if (_new_fanspeed) _fanspeed = thermalManager.fan_speed[FAN];
  #endif

  if (isMenu(tuneMenu) || isMenu(temperatureMenu)) {
    // Tune page temperature update
    TERN_(HAS_HOTEND, if (_new_hotend_target) hotendTargetItem->redraw());
    TERN_(HAS_HEATED_BED, if (_new_bed_target) bedTargetItem->redraw());
    TERN_(HAS_FAN, if (_new_fanspeed) fanSpeedItem->redraw());
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

  // Bottom temperature update

  #if HAS_HOTEND
    if (_new_hotend_temp)
<<<<<<< HEAD
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[0];
    if (_flow != planner.flow_percentage[0]) {
      _flow = planner.flow_percentage[0];
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
=======
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 384, _hotendtemp);
    if (_new_hotend_target)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 384, _hotendtarget);

    static int16_t _flow = planner.flow_percentage[EXT];
    if (_flow != planner.flow_percentage[EXT]) {
      _flow = planner.flow_percentage[EXT];
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 417, _flow);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif

  #if HAS_HEATED_BED
    if (_new_bed_temp)
<<<<<<< HEAD
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  _draw_feedrate();

  #if HAS_FAN
    if (_new_fanspeed)
      DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
=======
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 417, _bedtemp);
    if (_new_bed_target)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 417, _bedtarget);
  #endif

  _drawFeedrate();

  #if HAS_FAN
    if (_new_fanspeed)
      DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 195 + 2 * STAT_CHR_W, 384, _fanspeed);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif

  static float _offset = 0;
  if (BABY_Z_VAR != _offset) {
    _offset = BABY_Z_VAR;
<<<<<<< HEAD
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, _offset);
  }

  #if HAS_FILAMENT_SENSOR && PROUI_EX
    _draw_runout_icon();
  #endif

  _draw_ZOffsetIcon();
=======
    DWINUI::drawSignedFloat(DWIN_FONT_STAT, hmiData.colorIndicator,  hmiData.colorBackground, 2, 2, 204, 417, _offset);
  }

  #if HAS_PROUI_RUNOUT_SENSOR
    _drawRunoutIcon();
  #endif

  _drawZOffsetIcon();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

/**
 * Memory card and file management
 */

bool DWIN_lcd_sd_status = false;

#if ENABLED(MEDIASORT_MENU_ITEM)
<<<<<<< HEAD
  void SetMediaSort() {
    Toggle_Chkb_Line(HMI_data.MediaSort);
    card.setSortOn(HMI_data.MediaSort);
  }
#endif

void SetMediaAutoMount() { Toggle_Chkb_Line(HMI_data.MediaAutoMount); }
=======
  void setMediaSort() {
    toggleCheckboxLine(hmiData.mediaSort);
    card.setSortOn(hmiData.mediaSort);
  }
#endif

#if DISABLED(HAS_SD_EXTENDER)
  void setMediaAutoMount() { toggleCheckboxLine(hmiData.mediaAutoMount); }
#endif
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

inline uint16_t nr_sd_menu_items() {
  return _MIN(card.get_num_items() + !card.flag.workDirIsRoot, MENU_MAX_ITEMS);
}

<<<<<<< HEAD
void make_name_without_ext(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
=======
void makeNameWithoutExt(char *dst, char *src, size_t maxlen=MENU_CHAR_LIMIT) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  size_t pos = strlen(src);  // index of ending nul

  // For files, remove the extension
  // which may be .gcode, .gco, or .g
  if (!card.flag.filenameIsDir)
    while (pos && src[pos] != '.') pos--; // find last '.' (stop at 0)

  if (!pos) pos = strlen(src);  // pos = 0 ('.' not found) restore pos

  size_t len = pos;   // nul or '.'
  if (len > maxlen) { // Keep the name short
    pos        = len = maxlen; // move nul down
    dst[--pos] = '.'; // insert dots
    dst[--pos] = '.';
    dst[--pos] = '.';
  }

  dst[len] = '\0';    // end it

  // Copy down to 0
  while (pos--) dst[pos] = src[pos];
}

<<<<<<< HEAD
void SDCard_Up() {
=======
void sdCardUp() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  card.cdup();
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

<<<<<<< HEAD
void SDCard_Folder(char * const dirname) {
=======
void sdCardFolder(char * const dirname) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  card.cd(dirname);
  DWIN_lcd_sd_status = false; // On next DWIN_Update
}

void onClickSDItem() {
  const uint16_t hasUpDir = !card.flag.workDirIsRoot;
<<<<<<< HEAD
  if (hasUpDir && CurrentMenu->selected == 1) return SDCard_Up();
  else {
    const uint16_t filenum = CurrentMenu->selected - 1 - hasUpDir;
    card.selectFileByIndexSorted(filenum);

    // Enter that folder!
    if (card.flag.filenameIsDir) return SDCard_Folder(card.filename);

    if (card.fileIsBinary())
      return DWIN_Popup_Confirm(ICON_Error, F("Please check filenames"), F("Only G-code can be printed"));
    else
      return Goto_ConfirmToPrint();
=======
  if (hasUpDir && currentMenu->selected == 1) return sdCardUp();
  else {
    const uint16_t filenum = currentMenu->selected - 1 - hasUpDir;
    card.selectFileByIndexSorted(filenum);

    // Enter that folder!
    if (card.flag.filenameIsDir) return sdCardFolder(card.filename);

    if (card.fileIsBinary())
      return dwinPopupConfirm(ICON_Error, F("Please check filenames"), F("Only G-code can be printed"));
    else
      return gotoConfirmToPrint();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
}

#if ENABLED(SCROLL_LONG_FILENAMES)
  char shift_name[LONG_FILENAME_LENGTH + 1] = "";

<<<<<<< HEAD
  void Draw_SDItem_Shifted(uint8_t &shift) {
=======
  void drawSDItemShifted(uint8_t &shift) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    // Shorten to the available space
    const size_t lastchar = shift + MENU_CHAR_LIMIT;
    const char c = shift_name[lastchar];
    shift_name[lastchar] = '\0';

<<<<<<< HEAD
    const uint8_t row = FileMenu->line();
    Erase_Menu_Text(row);
    Draw_Menu_Line(row, 0, &shift_name[shift]);
=======
    const uint8_t row = fileMenu->line();
    eraseMenuText(row);
    drawMenuLine(row, 0, &shift_name[shift]);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

    shift_name[lastchar] = c;
  }

<<<<<<< HEAD
  void FileMenuIdle(bool reset=false) {
=======
  void fileMenuIdle(bool reset=false) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    static bool hasUpDir = false;
    static uint8_t last_itemselected = 0;
    static int8_t shift_amt = 0, shift_len = 0;
    if (reset) {
      last_itemselected = 0;
      hasUpDir = !card.flag.workDirIsRoot; // is a SubDir
      return;
    }
<<<<<<< HEAD
    const uint8_t selected = FileMenu->selected;
    if (last_itemselected != selected) {
      if (last_itemselected >= 1 + hasUpDir) FileMenu->Items()[last_itemselected]->redraw(true);
=======
    const uint8_t selected = fileMenu->selected;
    if (last_itemselected != selected) {
      if (last_itemselected >= 1 + hasUpDir) fileMenu->items()[last_itemselected]->redraw(true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      last_itemselected = selected;
      if (selected >= 1 + hasUpDir) {
        const int8_t filenum = selected - 1 - hasUpDir; // Skip "Back" and ".."
        card.selectFileByIndexSorted(filenum);
<<<<<<< HEAD
        make_name_without_ext(shift_name, card.longest_filename(), LONG_FILENAME_LENGTH);
=======
        makeNameWithoutExt(shift_name, card.longest_filename(), LONG_FILENAME_LENGTH);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        shift_len = strlen(shift_name);
        shift_amt = 0;
      }
    }
    else if ((selected >= 1 + hasUpDir) && (shift_len > MENU_CHAR_LIMIT)) {
      uint8_t shift_new = _MIN(shift_amt + 1, shift_len - MENU_CHAR_LIMIT); // Try to shift by...
<<<<<<< HEAD
      Draw_SDItem_Shifted(shift_new);             // Draw the item
=======
      drawSDItemShifted(shift_new);             // Draw the item
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      if (shift_new == shift_amt)                 // Scroll reached the end
        shift_new = -1;                           // Reset
      shift_amt = shift_new;                      // Set new scroll
    }
  }
#else
  char shift_name[FILENAME_LENGTH + 1] = "";
#endif

<<<<<<< HEAD
void onDrawFileName(MenuItemClass* menuitem, int8_t line) {
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && menuitem->pos == 1) {
    Draw_Menu_Line(line, ICON_Folder, "..");
=======
void onDrawFileName(MenuItem* menuitem, int8_t line) {
  const bool is_subdir = !card.flag.workDirIsRoot;
  if (is_subdir && menuitem->pos == 1) {
    drawMenuLine(line, ICON_Folder, "..");
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
  else {
    uint8_t icon;
    card.selectFileByIndexSorted(menuitem->pos - is_subdir - 1);
<<<<<<< HEAD
    make_name_without_ext(shift_name, card.longest_filename());
    icon = card.flag.filenameIsDir ? ICON_Folder : card.fileIsBinary() ? ICON_Binary : ICON_File;
    Draw_Menu_Line(line, icon, shift_name);
  }
}

void Draw_Print_File_Menu() {
  checkkey = Menu;
  if (card.isMounted()) {
    if (SET_MENU(FileMenu, MSG_MEDIA_MENU, nr_sd_menu_items() + 1)) {
      BACK_ITEM(Goto_Main_Menu);
      LOOP_L_N(i, nr_sd_menu_items()) {
        MenuItemAdd(onDrawFileName, onClickSDItem);
      }
    }
    UpdateMenu(FileMenu);
    TERN_(DASH_REDRAW, DWIN_RedrawDash());
  }
  else {
    if (SET_MENU(FileMenu, MSG_MEDIA_MENU, 1)) BACK_ITEM(Goto_Main_Menu);
    UpdateMenu(FileMenu);
    DWIN_Draw_Rectangle(1, HMI_data.AlertBg_Color, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::Draw_CenteredString(font12x24, HMI_data.AlertTxt_Color, MBASE(3), GET_TEXT_F(MSG_MEDIA_NOT_INSERTED));
  }
  TERN_(SCROLL_LONG_FILENAMES, FileMenuIdle(true));
=======
    makeNameWithoutExt(shift_name, card.longest_filename());
    icon = card.flag.filenameIsDir ? ICON_Folder : card.fileIsBinary() ? ICON_Binary : ICON_File;
    drawMenuLine(line, icon, shift_name);
  }
}

void drawPrintFileMenu() {
  checkkey = ID_Menu;
  if (card.isMounted()) {
    if (SET_MENU(fileMenu, MSG_MEDIA_MENU, nr_sd_menu_items() + 1)) {
      BACK_ITEM(gotoMainMenu);
      for (uint8_t i = 0; i < nr_sd_menu_items(); ++i) {
        menuItemAdd(onDrawFileName, onClickSDItem);
      }
    }
    updateMenu(fileMenu);
    TERN_(DASH_REDRAW, dwinRedrawDash());
  }
  else {
    if (SET_MENU(fileMenu, MSG_MEDIA_MENU, 1)) BACK_ITEM(gotoMainMenu);
    updateMenu(fileMenu);
    dwinDrawRectangle(1, hmiData.colorAlertBg, 10, MBASE(3) - 10, DWIN_WIDTH - 10, MBASE(4));
    DWINUI::drawCenteredString(font12x24, hmiData.colorAlertTxt, MBASE(3), GET_TEXT_F(MSG_MEDIA_NOT_INSERTED));
  }
  TERN_(SCROLL_LONG_FILENAMES, fileMenuIdle(true));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//
// Watch for media mount / unmount
//
<<<<<<< HEAD
void HMI_SDCardUpdate() {
  if (HMI_flag.home_flag) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    ResetMenu(FileMenu);
    if (IsMenu(FileMenu)) {
      CurrentMenu = nullptr;
      Draw_Print_File_Menu();
    }
    if (!DWIN_lcd_sd_status && SD_Printing()) ui.abort_print();  // Media removed while printing
=======
void hmiSDCardUpdate() {
  if (checkkey == ID_Homing) return;
  if (DWIN_lcd_sd_status != card.isMounted()) {
    DWIN_lcd_sd_status = card.isMounted();
    resetMenu(fileMenu);
    if (isMenu(fileMenu)) {
      currentMenu = nullptr;
      drawPrintFileMenu();
    }
    if (!DWIN_lcd_sd_status && sdPrinting()) ui.abort_print();  // Media removed while printing
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
}

/**
 * Dash board and indicators
 */

<<<<<<< HEAD
void DWIN_Draw_Dashboard() {

  DWIN_Draw_Rectangle(1, HMI_data.Background_Color, 0, STATUS_Y + 21, DWIN_WIDTH, DWIN_HEIGHT - 1);

  DWIN_Draw_Rectangle(1, HMI_data.SplitLine_Color, 0, 449, DWIN_WIDTH, 451);

  DWINUI::Draw_Icon(ICON_MaxSpeedX,  10, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedY,  95, 456);
  DWINUI::Draw_Icon(ICON_MaxSpeedZ, 180, 456);
  _draw_xyz_position(true);
=======
void dwinDrawDashboard() {

  dwinDrawRectangle(1, hmiData.colorBackground, 0, STATUS_Y + 21, DWIN_WIDTH, DWIN_HEIGHT - 1);

  dwinDrawRectangle(1, hmiData.colorSplitLine, 0, 449, DWIN_WIDTH, 451);

  DWINUI::drawIcon(ICON_MaxSpeedX,  10, 456);
  DWINUI::drawIcon(ICON_MaxSpeedY,  95, 456);
  DWINUI::drawIcon(ICON_MaxSpeedZ, 180, 456);
  _drawXYZPosition(true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

  TERN_(CV_LASER_MODULE, if (laser_device.is_laser_device()) return);

  #if HAS_HOTEND
<<<<<<< HEAD
    DWINUI::Draw_Icon(ICON_HotendTemp, 10, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 384, thermalManager.wholeDegHotend(0));
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(0));

    DWINUI::Draw_Icon(ICON_StepE, 112, 417);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[0]);
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::Draw_Icon(ICON_BedTemp, 10, 416);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::Draw_Icon(ICON_Speed, 113, 383);
  DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  IF_DISABLED(SHOW_SPEED_IND, DWINUI::Draw_String(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 116 + 5 * STAT_CHR_W + 2, 384, F("%")));

  #if HAS_FAN
    DWINUI::Draw_Icon(ICON_FanSpeed, 187, 383);
    DWINUI::Draw_Int(DWIN_FONT_STAT, HMI_data.Indicator_Color, HMI_data.Background_Color, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[0]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::Draw_Icon(planner.leveling_active ? ICON_SetZOffset : ICON_Zoffset, 187, 416);
    DWINUI::Draw_Signed_Float(DWIN_FONT_STAT, HMI_data.Indicator_Color,  HMI_data.Background_Color, 2, 2, 204, 417, BABY_Z_VAR);
=======
    DWINUI::drawIcon(ICON_HotendTemp, 10, 383);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 384, thermalManager.wholeDegHotend(EXT));
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 25 + 3 * STAT_CHR_W + 5, 384, F("/"));
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 384, thermalManager.degTargetHotend(EXT));

    DWINUI::drawIcon(ICON_StepE, 112, 417);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 417, planner.flow_percentage[EXT]);
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 417, F("%"));
  #endif

  #if HAS_HEATED_BED
    DWINUI::drawIcon(ICON_BedTemp, 10, 416);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 28, 417, thermalManager.wholeDegBed());
    DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 25 + 3 * STAT_CHR_W + 5, 417, F("/"));
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 25 + 4 * STAT_CHR_W + 6, 417, thermalManager.degTargetBed());
  #endif

  DWINUI::drawIcon(ICON_Speed, 113, 383);
  DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 116 + 2 * STAT_CHR_W, 384, feedrate_percentage);
  IF_DISABLED(SHOW_SPEED_IND, DWINUI::drawString(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 116 + 5 * STAT_CHR_W + 2, 384, F("%")));

  #if HAS_FAN
    DWINUI::drawIcon(ICON_FanSpeed, 187, 383);
    DWINUI::drawInt(DWIN_FONT_STAT, hmiData.colorIndicator, hmiData.colorBackground, 3, 195 + 2 * STAT_CHR_W, 384, thermalManager.fan_speed[FAN]);
  #endif

  #if HAS_ZOFFSET_ITEM
    DWINUI::drawIcon(planner.leveling_active ? ICON_SetZOffset : ICON_Zoffset, 187, 416);
    DWINUI::drawSignedFloat(DWIN_FONT_STAT, hmiData.colorIndicator,  hmiData.colorBackground, 2, 2, 204, 417, BABY_Z_VAR);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif

}

<<<<<<< HEAD
void Draw_Info_Menu() {
  DWINUI::ClearMainArea();
  Title.ShowCaption(GET_TEXT_F(MSG_INFO_SCREEN));
  Draw_Menu_Line(0, ICON_Back, GET_TEXT_F(MSG_BACK), false, true);
=======
void drawInfoMenu() {
  DWINUI::clearMainArea();
  title.showCaption(GET_TEXT_F(MSG_INFO_SCREEN));
  drawMenuLine(0, ICON_Back, GET_TEXT_F(MSG_BACK), false, true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  char machine_size[21];
  machine_size[0] = '\0';
  sprintf_P(machine_size, PSTR("%ix%ix%i"), (int16_t)X_BED_SIZE, (int16_t)Y_BED_SIZE, (int16_t)Z_MAX_POS);

<<<<<<< HEAD
  DWINUI::Draw_CenteredString(92,  GET_TEXT_F(MSG_INFO_MACHINENAME));
  DWINUI::Draw_CenteredString(112, F(TERN_(CF,"CF ") MACHINE_NAME));
  DWINUI::Draw_CenteredString(145, GET_TEXT_F(MSG_INFO_SIZE));
  DWINUI::Draw_CenteredString(165, machine_size);

  LOOP_L_N(i, 4 - 2 * ENABLED(PROUI_EX)) {
    DWINUI::Draw_Icon(ICON_Step + i, ICOX, 90 + i * MLINE);
    DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(i + 2), 240);
  }

  #if PROUI_EX
    ProEx.Init();
  #else
    DWINUI::Draw_CenteredString(198, GET_TEXT_F(MSG_INFO_FWVERSION));
    DWINUI::Draw_CenteredString(218, F(SHORT_BUILD_VERSION));
    DWINUI::Draw_CenteredString(251, GET_TEXT_F(MSG_INFO_BUILD));
    DWINUI::Draw_CenteredString(271, F(DateTime));
=======
  DWINUI::drawCenteredString(92,  GET_TEXT_F(MSG_INFO_MACHINENAME));
  DWINUI::drawCenteredString(112, F(MACHINE_NAME));
  DWINUI::drawCenteredString(145, GET_TEXT_F(MSG_INFO_SIZE));
  DWINUI::drawCenteredString(165, machine_size);

  for (uint8_t i = 0; i < 4 - 2 * ENABLED(PROUI_EX); ++i) {
    DWINUI::drawIcon(ICON_Step + i, ICOX, 90 + i * MLINE);
    dwinDrawHLine(hmiData.colorSplitLine, 16, MYPOS(i + 2), 240);
  }

  #if PROUI_EX
    proUIEx.init();
  #else
    DWINUI::drawCenteredString(198, GET_TEXT_F(MSG_INFO_FWVERSION));
    DWINUI::drawCenteredString(218, F(SHORT_BUILD_VERSION));
    DWINUI::drawCenteredString(251, GET_TEXT_F(MSG_INFO_BUILD));
    DWINUI::drawCenteredString(271, F(dateTime));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif

}

// Main Process
<<<<<<< HEAD
void HMI_MainMenu() {
=======
void hmiMainMenu() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;

  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_page.inc(PAGE_COUNT)) {
      switch (select_page.now) {
<<<<<<< HEAD
        case PAGE_PRINT: ICON_Print(); break;
        case PAGE_PREPARE: ICON_Print(); ICON_Prepare(); break;
        case PAGE_CONTROL: ICON_Prepare(); ICON_Control(); break;
        case PAGE_ADVANCE: ICON_Control(); ICON_AdvSettings(); break;
        TERN_(HAS_TOOLBAR, case PAGE_TOOLBAR: ICON_AdvSettings(); Goto_ToolBar();  break);
=======
        case PAGE_PRINT: iconPrint(); break;
        case PAGE_PREPARE: iconPrint(); iconPrepare(); break;
        case PAGE_CONTROL: iconPrepare(); iconControl(); break;
        case PAGE_ADVANCE: iconControl(); iconAdvSettings(); break;
        TERN_(HAS_TOOLBAR, case PAGE_TOOLBAR: iconAdvSettings(); gotoToolBar();  break);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_page.dec()) {
      switch (select_page.now) {
<<<<<<< HEAD
        case PAGE_PRINT: ICON_Print(); ICON_Prepare(); break;
        case PAGE_PREPARE: ICON_Prepare(); ICON_Control(); break;
        case PAGE_CONTROL: ICON_Control(); ICON_AdvSettings(); break;
        case PAGE_ADVANCE: ICON_AdvSettings(); break;
=======
        case PAGE_PRINT: iconPrint(); iconPrepare(); break;
        case PAGE_PREPARE: iconPrepare(); iconControl(); break;
        case PAGE_CONTROL: iconControl(); iconAdvSettings(); break;
        case PAGE_ADVANCE: iconAdvSettings(); break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_page.now) {
      case PAGE_PRINT:
<<<<<<< HEAD
        if (HMI_data.MediaAutoMount) {
          card.mount();
          safe_delay(800);
        };
        Draw_Print_File_Menu();
        break;
      case PAGE_PREPARE: Draw_Prepare_Menu(); break;
      case PAGE_CONTROL: Draw_Control_Menu(); break;
      case PAGE_ADVANCE: Draw_AdvancedSettings_Menu(); break;
    }
  }
  DWIN_UpdateLCD();
}

// Pause or Stop popup
void onClick_PauseOrStop() {
  switch (select_print.now) {
    case PRINT_PAUSE_RESUME: if (HMI_flag.select_flag) ui.pause_print(); break; // confirm pause
    case PRINT_STOP: if (HMI_flag.select_flag) ui.abort_print(); break; // stop confirmed then abort print
    default: break;
  }
  return Goto_PrintProcess();
}

// Printing
void HMI_Printing() {
=======
        if (ENABLED(HAS_SD_EXTENDER) || hmiData.mediaAutoMount) {
          card.mount();
          safe_delay(800);
        };
        drawPrintFileMenu();
        break;
      case PAGE_PREPARE: drawPrepareMenu(); break;
      case PAGE_CONTROL: drawControlMenu(); break;
      case PAGE_ADVANCE: drawAdvancedSettingsMenu(); break;
    }
  }
  dwinUpdateLCD();
}

// Pause or Stop popup
void onClickPauseOrStop() {
  switch (select_print.now) {
    case PRINT_PAUSE_RESUME: if (hmiFlag.select_flag) ui.pause_print(); break; // confirm pause
    case PRINT_STOP: if (hmiFlag.select_flag) ui.abort_print(); break; // stop confirmed then abort print
    default: break;
  }
  return gotoPrintProcess();
}

// Printing
void hmiPrinting() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState == ENCODER_DIFF_NO) return;
  // Avoid flicker by updating only the previous menu
  if (encoder_diffState == ENCODER_DIFF_CW) {
    if (select_print.inc(PRINT_COUNT)) {
      switch (select_print.now) {
<<<<<<< HEAD
        case PRINT_SETUP: ICON_Tune(); break;
        case PRINT_PAUSE_RESUME: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_STOP: ICON_ResumeOrPause(); ICON_Stop(); break;
=======
        case PRINT_SETUP: iconTune(); break;
        case PRINT_PAUSE_RESUME: iconTune(); iconResumeOrPause(); break;
        case PRINT_STOP: iconResumeOrPause(); iconStop(); break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_CCW) {
    if (select_print.dec()) {
      switch (select_print.now) {
<<<<<<< HEAD
        case PRINT_SETUP: ICON_Tune(); ICON_ResumeOrPause(); break;
        case PRINT_PAUSE_RESUME: ICON_ResumeOrPause(); ICON_Stop(); break;
        case PRINT_STOP: ICON_Stop(); break;
=======
        case PRINT_SETUP: iconTune(); iconResumeOrPause(); break;
        case PRINT_PAUSE_RESUME: iconResumeOrPause(); iconStop(); break;
        case PRINT_STOP: iconStop(); break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      }
    }
  }
  else if (encoder_diffState == ENCODER_DIFF_ENTER) {
    switch (select_print.now) {
<<<<<<< HEAD
      case PRINT_SETUP: Draw_Tune_Menu(); break;
=======
      case PRINT_SETUP: drawTuneMenu(); break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      case PRINT_PAUSE_RESUME:
        if (print_job_timer.isPaused()) {  // if printer is already in pause
          ui.resume_print();
          break;
        }
        else
<<<<<<< HEAD
          return Goto_Popup(Popup_window_PauseOrStop, onClick_PauseOrStop);
      case PRINT_STOP:
        return Goto_Popup(Popup_window_PauseOrStop, onClick_PauseOrStop);
      default: break;
    }
  }
  DWIN_UpdateLCD();
=======
          return gotoPopup(popupPauseOrStop, onClickPauseOrStop);
      case PRINT_STOP:
        return gotoPopup(popupPauseOrStop, onClickPauseOrStop);
      default: break;
    }
  }
  dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

#include "../../../libs/buzzer.h"

<<<<<<< HEAD
void Draw_Main_Area() {
  switch (checkkey) {
    case MainMenu:               Draw_Main_Menu(); break;
    case PrintProcess:           Draw_PrintProcess(); break;
    case PrintDone:              Draw_PrintDone(); break;
    #if HAS_ESDIAG
      case ESDiagProcess:        Draw_EndStopDiag(); break;
    #endif
    case Popup:                  Draw_Popup(); break;
    #if HAS_LOCKSCREEN
      case Locked:               lockScreen.draw(); break;
    #endif
    case Menu:
      #if HAS_TOOLBAR
        if (CurrentMenu == &ToolBar) Draw_Main_Menu(); else ReDrawMenu();
      #else
        ReDrawMenu();
      #endif
      break;
    case SetInt:
    case SetPInt:
    case SetIntNoDraw:
    case SetFloat:
    case SetPFloat:              ReDrawMenu(true); break;
=======
void drawMainArea() {
  switch (checkkey) {
    case ID_MainMenu:               drawMainMenu(); break;
    case ID_PrintProcess:           drawPrintProcess(); break;
    case ID_PrintDone:              drawPrintDone(); break;
    #if HAS_ESDIAG
      case ID_ESDiagProcess:        drawEndStopDiag(); break;
    #endif
    case ID_Popup:                  popupDraw(); break;
    #if HAS_LOCKSCREEN
      case ID_Locked:               lockScreen.draw(); break;
    #endif
    case ID_Menu:
      #if HAS_TOOLBAR
        if (currentMenu == &toolBar) drawMainMenu(); else redrawMenu();
      #else
        redrawMenu();
      #endif
      break;
    case ID_SetInt:
    case ID_SetPInt:
    case ID_SetIntNoDraw:
    case ID_SetFloat:
    case ID_SetPFloat:              redrawMenu(true); break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    default: break;
  }
}

<<<<<<< HEAD
void HMI_WaitForUser() {
  EncoderState encoder_diffState = get_encoder_state();
  if (encoder_diffState != ENCODER_DIFF_NO && !ui.backlight) {
    if (checkkey == WaitResponse) HMI_ReturnScreen();
    return ui.refresh_brightness();
  }
  if (!wait_for_user) {
    switch (checkkey) {
      case PrintDone:
        select_page.reset();
        Goto_Main_Menu();
        break;
      #if HAS_BED_PROBE
        case Leveling:
          #if PROUI_EX
            ProEx.StopLeveling();
          #else
            HMI_ReturnScreen();
=======
void hmiWaitForUser() {
  encoderReceiveAnalyze();
  if (!wait_for_user) {
    switch (checkkey) {
      case ID_PrintDone:
        select_page.reset();
        gotoMainMenu();
        break;
      #if HAS_BED_PROBE
        case ID_Leveling:
          #if PROUI_EX
            proUIEx.stopLeveling();
          #else
            hmiReturnScreen();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          #endif
          break;
      #endif
      default:
<<<<<<< HEAD
        HMI_ReturnScreen();
=======
        hmiReturnScreen();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        break;
    }
  }
}

#if DISABLED(PROUI_EX)
  // Draws boot screen
<<<<<<< HEAD
  void HMI_Init() {
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 1100
    #endif
    DWINUI::Draw_Box(1, Color_Black, { 5, 220, DWIN_WIDTH - 5, DWINUI::fontHeight() });
    DWINUI::Draw_CenteredString(Color_White, 220, F("Professional Firmware "));
    for (uint16_t t = 15; t <= 257; t += 11) {
      DWINUI::Draw_Icon(ICON_Bar, 15, 260);
      DWIN_Draw_Rectangle(1, HMI_data.Background_Color, t, 260, 257, 280);
      DWIN_UpdateLCD();
=======
  void hmiInit() {
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 1100
    #endif
    DWINUI::drawBox(1, COLOR_BLACK, { 5, 220, DWIN_WIDTH - 5, DWINUI::fontHeight() });
    DWINUI::drawCenteredString(COLOR_WHITE, 220, F("Professional Firmware "));
    for (uint16_t t = 15; t <= 257; t += 11) {
      DWINUI::drawIcon(ICON_Bar, 15, 260);
      dwinDrawRectangle(1, hmiData.colorBackground, t, 260, 257, 280);
      dwinUpdateLCD();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      safe_delay((BOOTSCREEN_TIMEOUT) / 22);
    }
  }
#endif

<<<<<<< HEAD
void EachMomentUpdate() {
=======
void eachMomentUpdate() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  static millis_t next_var_update_ms = 0, next_rts_update_ms = 0, next_status_update_ms = 0;
  const millis_t ms = millis();

  if (ELAPSED(ms, next_var_update_ms)) {
    next_var_update_ms = ms + DWIN_VAR_UPDATE_INTERVAL;
    blink = !blink;
<<<<<<< HEAD
    update_variable();
    #if HAS_ESDIAG
      if (checkkey == ESDiagProcess) ESDiag.Update();
    #endif
    #if HAS_PLOT
      if (checkkey == PidProcess) {
        IF_ENABLED(PIDTEMP, if (HMI_value.tempcontrol == PIDTEMP_START) plot.Update(thermalManager.wholeDegHotend(0)));
        IF_ENABLED(PIDTEMPBED, if (HMI_value.tempcontrol == PIDTEMPBED_START) plot.Update(thermalManager.wholeDegBed()));
      }
      IF_ENABLED(MPCTEMP, if (checkkey == MPCProcess) plot.Update(thermalManager.wholeDegHotend(0)));
=======
    updateVariable();
    #if HAS_ESDIAG
      if (checkkey == ID_ESDiagProcess) esDiag.update();
    #endif
    #if HAS_PLOT
      if (checkkey == ID_PIDProcess) {
        TERN_(PIDTEMP, if (hmiValue.tempControl == PIDTEMP_START) plot.update(thermalManager.wholeDegHotend(EXT)));
        TERN_(PIDTEMPBED, if (hmiValue.tempControl == PIDTEMPBED_START) plot.update(thermalManager.wholeDegBed()));
      }
      TERN_(MPCTEMP, if (checkkey == ID_MPCProcess) plot.update(thermalManager.wholeDegHotend(EXT)));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    #endif
  }

  #if HAS_STATUS_MESSAGE_TIMEOUT
    bool did_expire = ui.status_reset_callback && (*ui.status_reset_callback)();
    did_expire |= ui.status_message_expire_ms && ELAPSED(ms, ui.status_message_expire_ms);
    if (did_expire) ui.reset_status();
  #endif

  if (ELAPSED(ms, next_status_update_ms)) {
    next_status_update_ms = ms + 500;
<<<<<<< HEAD
    DWIN_DrawStatusMessage();
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (IsMenu(FileMenu)) FileMenuIdle();
    #endif
  }

  if (!PENDING(ms, next_rts_update_ms)) {
    next_rts_update_ms = ms + DWIN_UPDATE_INTERVAL;

    if ((HMI_flag.printing_flag != Printing()) && !HMI_flag.home_flag) {
      HMI_flag.printing_flag = Printing();
      DEBUG_ECHOLNPGM("printing_flag: ", HMI_flag.printing_flag);
      if (HMI_flag.printing_flag)
        DWIN_Print_Started();
      else if (HMI_flag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Finished();
    }

    if ((print_job_timer.isPaused() != HMI_flag.pause_flag) && !HMI_flag.home_flag) {
      HMI_flag.pause_flag = print_job_timer.isPaused();
      DEBUG_ECHOLNPGM("pause_flag: ", HMI_flag.pause_flag);
      if (HMI_flag.pause_flag)
        DWIN_Print_Pause();
      else if (HMI_flag.abort_flag)
        DWIN_Print_Aborted();
      else
        DWIN_Print_Resume();
    }

    if (checkkey == PrintProcess) { // print process
=======
    dwinDrawStatusMessage();
    #if ENABLED(SCROLL_LONG_FILENAMES)
      if (isMenu(fileMenu)) fileMenuIdle();
    #endif
  }

  #if LCD_BACKLIGHT_TIMEOUT_MINS
    if (ui.backlight_off_ms && ELAPSED(ms, ui.backlight_off_ms)) {
      #ifdef NEOPIXEL_BKGD_INDEX_FIRST
        neo.set_background_off();
        neo.show();
      #endif
      dimBacklight();
      ui.backlight_off_ms = 0;
    }
  #endif

  if (!PENDING(ms, next_rts_update_ms)) {
    next_rts_update_ms = ms + DWIN_UPDATE_INTERVAL;

    if ((hmiFlag.printing_flag != isPrinting()) && (checkkey != ID_Homing) && (checkkey != ID_Leveling)) {
      hmiFlag.printing_flag = isPrinting();
      DEBUG_ECHOLNPGM("printing_flag: ", hmiFlag.printing_flag);
      if (hmiFlag.printing_flag)
        dwinPrintStarted();
      else if (hmiFlag.abort_flag)
        dwinPrintAborted();
      else
        dwinPrintFinished();
    }

    if ((print_job_timer.isPaused() != hmiFlag.pause_flag) && (checkkey != ID_Homing)) {
      hmiFlag.pause_flag = print_job_timer.isPaused();
      DEBUG_ECHOLNPGM("pause_flag: ", hmiFlag.pause_flag);
      if (hmiFlag.pause_flag)
        dwinPrintPause();
      else if (hmiFlag.abort_flag)
        dwinPrintAborted();
      else
        dwinPrintResume();
    }

    if (checkkey == ID_PrintProcess) { // print process
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

      // Progress percent
      static uint8_t _percent_done = 255;
      if (_percent_done != ui.get_progress_percent()) {
        _percent_done = ui.get_progress_percent();
<<<<<<< HEAD
        Draw_Print_ProgressBar();
      }

      // Remaining time
      static uint32_t _remain_time = 0;
      if (_remain_time != ui.get_remaining_time()) {
        _remain_time = ui.get_remaining_time();
        Draw_Print_ProgressRemain();
      }
=======
        drawPrintProgressBar();
      }

      // Remaining time
      #if ENABLED(SHOW_REMAINING_TIME)
        static uint32_t _remain_time = 0;
        if (_remain_time != ui.get_remaining_time()) {
          _remain_time = ui.get_remaining_time();
          drawPrintProgressRemain();
        }
      #endif
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

      // Elapse print time
      static uint16_t _printtime = 0;
      const uint16_t min = (print_job_timer.duration() % 3600) / 60;
      if (_printtime != min) { // 1 minute update
        _printtime = min;
<<<<<<< HEAD
        Draw_Print_ProgressElapsed();
=======
        drawPrintProgressElapsed();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      }

    }
    #if ENABLED(POWER_LOSS_RECOVERY)
      else if (DWIN_lcd_sd_status && recovery.dwin_flag) { // resume print before power off
<<<<<<< HEAD
        return Goto_PowerLossRecovery();
      }
    #endif // POWER_LOSS_RECOVERY
  }
  DWIN_UpdateLCD();
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery() {
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 70, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 147, F("It looks like the last"));
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 167, F("file was interrupted."));
    DWINUI::Draw_Button(BTN_Cancel,    26, 280);
    DWINUI::Draw_Button(BTN_Continue, 146, 280);
    MediaFile *dir = nullptr;
    const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
    card.selectFileByName(filename);
    DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 207, card.longest_filename());
    DWIN_Print_Header(card.longest_filename()); // Save filename
    Draw_Select_Highlight(HMI_flag.select_flag);
    DWIN_UpdateLCD();
  }

  void onClick_PowerLossRecovery() {
    if (HMI_flag.select_flag) {
      queue.inject(F("M1000C"));
      select_page.reset();
      return Goto_Main_Menu();
    }
    else {
      HMI_SaveProcessID(NothingToDo);
=======
        return gotoPowerLossRecovery();
      }
    #endif // POWER_LOSS_RECOVERY
  }
  dwinUpdateLCD();
}

#if ENABLED(POWER_LOSS_RECOVERY)
  void popupPowerLossRecovery() {
    DWINUI::clearMainArea();
    drawPopupBkgd();
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 70, GET_TEXT_F(MSG_OUTAGE_RECOVERY));
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 147, F("It looks like the last"));
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 167, F("file was interrupted."));
    DWINUI::drawButton(BTN_Cancel,    26, 280);
    DWINUI::drawButton(BTN_Continue, 146, 280);
    MediaFile *dir = nullptr;
    const char * const filename = card.diveToFile(true, dir, recovery.info.sd_filename);
    card.selectFileByName(filename);
    DWINUI::drawCenteredString(hmiData.colorPopupTxt, 207, card.longest_filename());
    dwinPrintHeader(card.longest_filename()); // Save filename
    drawSelectHighlight(hmiFlag.select_flag);
    dwinUpdateLCD();
  }

  void onClickPowerLossRecovery() {
    if (hmiFlag.select_flag) {
      queue.inject(F("M1000C"));
      select_page.reset();
      return gotoMainMenu();
    }
    else {
      hmiSaveProcessID(ID_NothingToDo);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      select_print.set(PRINT_SETUP);
      queue.inject(F("M1000"));
    }
  }

<<<<<<< HEAD
  void Goto_PowerLossRecovery() {
    recovery.dwin_flag = false;
    LCD_MESSAGE(MSG_CONTINUE_PRINT_JOB);
    Goto_Popup(Popup_PowerLossRecovery, onClick_PowerLossRecovery);
=======
  void gotoPowerLossRecovery() {
    recovery.dwin_flag = false;
    LCD_MESSAGE(MSG_CONTINUE_PRINT_JOB);
    gotoPopup(popupPowerLossRecovery, onClickPowerLossRecovery);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // POWER_LOSS_RECOVERY


<<<<<<< HEAD
void DWIN_HandleScreen() {
  switch (checkkey) {
    case MainMenu:        HMI_MainMenu(); break;
    case Menu:            HMI_Menu(); break;
    case SetInt:          HMI_SetDraw(); break;
    case SetFloat:        HMI_SetDraw(); break;
    case SetPInt:         HMI_SetPInt(); break;
    case SetPFloat:       HMI_SetPFloat(); break;
    case SetIntNoDraw:    HMI_SetNoDraw(); break;
    case PrintProcess:    HMI_Printing(); break;
    case Popup:           HMI_Popup(); break;
    case Leveling:        TERN_(PROUI_EX, HMI_WaitForUser();) break;
    #if HAS_LOCKSCREEN
      case Locked:        HMI_LockScreen(); break;
    #endif
    case PrintDone:
    TERN_(HAS_ESDIAG, case ESDiagProcess:)
    case WaitResponse:    HMI_WaitForUser(); break;
    case Homing:
    case PidProcess:
    case NothingToDo:     break;
=======
void dwinHandleScreen() {
  switch (checkkey) {
    case ID_MainMenu:        hmiMainMenu(); break;
    case ID_Menu:            hmiMenu(); break;
    case ID_SetInt:          hmiSetDraw(); break;
    case ID_SetFloat:        hmiSetDraw(); break;
    case ID_SetPInt:         hmiSetPInt(); break;
    case ID_SetPFloat:       hmiSetPFloat(); break;
    case ID_SetIntNoDraw:    hmiSetNoDraw(); break;
    case ID_PrintProcess:    hmiPrinting(); break;
    case ID_Popup:           hmiPopup(); break;
    case ID_Leveling:        TERN_(PROUI_EX, hmiWaitForUser();) break;
    #if HAS_LOCKSCREEN
      case ID_Locked:        hmiLockScreen(); break;
    #endif
    case ID_PrintDone:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
    case ID_WaitResponse:    hmiWaitForUser(); break;
    case ID_Homing:
    case ID_PIDProcess:
    case ID_NothingToDo:     break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    default: break;
  }
}

<<<<<<< HEAD
bool IDisPopUp() {    // If ID is popup...
  return  (checkkey == NothingToDo)
       || (checkkey == WaitResponse)
       || (checkkey == Homing)
       || (checkkey == Leveling)
       || (checkkey == PidProcess)
       || TERN0(HAS_ESDIAG, (checkkey == ESDiagProcess))
       || (checkkey == Popup);
}

void HMI_SaveProcessID(const uint8_t id) {
  if (checkkey != id) {
    if (!IDisPopUp()) last_checkkey = checkkey; // if previous is not a popup
    if ((id == Popup)
         || TERN0(HAS_ESDIAG, (id == ESDiagProcess))
         || (id == PrintDone)
         || (id == Leveling)
         || (id == WaitResponse)) wait_for_user = true;
    checkkey = id;
  }
}

void HMI_ReturnScreen() {
  checkkey = last_checkkey;
  wait_for_user = false;
  Draw_Main_Area();
}

void DWIN_HomingStart() {
  DEBUG_ECHOLNPGM("DWIN_HomingStart");
  HMI_flag.home_flag = true;
  HMI_SaveProcessID(Homing);
  Title.ShowCaption(GET_TEXT_F(MSG_HOMING));
  DWIN_Show_Popup(ICON_BLTouch, GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT));
}

void DWIN_HomingDone() {
  DEBUG_ECHOLNPGM("DWIN_HomingDone");
  HMI_flag.home_flag = false;
  #if DISABLED(HAS_BED_PROBE) && EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    planner.synchronize();
    babystep.add_mm(Z_AXIS, HMI_data.ManualZOffset);
=======
bool idIsPopUp() {    // If ID is popup...
  switch (checkkey) {
    case ID_NothingToDo:
    case ID_WaitResponse:
    case ID_Popup:
    case ID_Homing:
    case ID_Leveling:
    case ID_PIDProcess:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
      return true;
    default: break;
  }
  return false;
}

void hmiSaveProcessID(const uint8_t id) {
  if (checkkey == id) return;
  if (!idIsPopUp()) last_checkkey = checkkey; // if previous is not a popup
  checkkey = id;
  switch (id) {
    case ID_Popup:
    case ID_WaitResponse:
    case ID_PrintDone:
    case ID_Leveling:
    TERN_(HAS_ESDIAG, case ID_ESDiagProcess:)
      wait_for_user = true;
    default: break;
  }
}

void hmiReturnScreen() {
  checkkey = last_checkkey;
  wait_for_user = false;
  drawMainArea();
}

void dwinHomingStart() {
  DEBUG_ECHOLNPGM("dwinHomingStart");
  hmiSaveProcessID(ID_Homing);
  title.showCaption(GET_TEXT_F(MSG_HOMING));
  dwinShowPopup(ICON_BLTouch, GET_TEXT_F(MSG_HOMING), GET_TEXT_F(MSG_PLEASE_WAIT));
}

void dwinHomingDone() {
  DEBUG_ECHOLNPGM("dwinHomingDone");
  #if DISABLED(HAS_BED_PROBE) && ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
    planner.synchronize();
    babystep.add_mm(Z_AXIS, hmiData.manualZOffset);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
  #if ENABLED(CV_LASER_MODULE)
    if (laser_device.is_laser_device()) laser_device.laser_home();
  #endif
<<<<<<< HEAD
  if (last_checkkey == PrintDone) 
    Goto_PrintDone();
  else
    HMI_ReturnScreen();
}

void DWIN_LevelingStart() {
  DEBUG_ECHOLNPGM("DWIN_LevelingStart");
  #if HAS_BED_PROBE
    HMI_SaveProcessID(Leveling);
    TERN_(PROUI_EX,HMI_flag.cancel_abl = 0);
    Title.ShowCaption(GET_TEXT_F(MSG_BED_LEVELING));
    #if PROUI_EX
      MeshViewer.DrawMeshGrid(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
      DWINUI::Draw_Button(BTN_Cancel, 86, 305);
    #else
      DWIN_Show_Popup(ICON_AutoLeveling, GET_TEXT_F(MSG_BED_LEVELING), GET_TEXT_F(MSG_PLEASE_WAIT), TERN(PROUI_EX, BTN_Cancel, 0));
    #endif
    #if BOTH(AUTO_BED_LEVELING_UBL, PREHEAT_BEFORE_LEVELING)
      #if HAS_BED_PROBE
        if (!DEBUGGING(DRYRUN)) probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, HMI_data.BedLevT);
      #else
        #if HAS_HOTEND
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetHotend(0) < LEVELING_NOZZLE_TEMP) {
            thermalManager.setTargetHotend(LEVELING_NOZZLE_TEMP, 0);
            thermalManager.wait_for_hotend(0);
          }
        #endif
        #if HAS_HEATED_BED
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetBed() < HMI_data.BedLevT) {
            thermalManager.setTargetBed(HMI_data.BedLevT);
=======
  if (last_checkkey == ID_PrintDone)
    gotoPrintDone();
  else
    hmiReturnScreen();
}

void dwinLevelingStart() {
  DEBUG_ECHOLNPGM("dwinLevelingStart");
  #if HAS_BED_PROBE
    hmiSaveProcessID(ID_Leveling);
    TERN_(PROUI_EX,hmiFlag.cancel_abl = 0);
    title.showCaption(GET_TEXT_F(MSG_BED_LEVELING));
    #if PROUI_EX
      meshViewer.drawBackground(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y);
      DWINUI::drawButton(BTN_Cancel, 86, 305);
    #else
      dwinShowPopup(ICON_AutoLeveling, GET_TEXT_F(MSG_BED_LEVELING), GET_TEXT_F(MSG_PLEASE_WAIT), TERN(PROUI_EX, BTN_Cancel, 0));
    #endif
    #if ALL(AUTO_BED_LEVELING_UBL, PREHEAT_BEFORE_LEVELING)
      #if HAS_BED_PROBE
        if (!DEBUGGING(DRYRUN)) probe.preheat_for_probing(LEVELING_NOZZLE_TEMP, hmiData.bedLevT);
      #else
        #if HAS_HOTEND
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetHotend(EXT) < LEVELING_NOZZLE_TEMP) {
            thermalManager.setTargetHotend(LEVELING_NOZZLE_TEMP, EXT);
            thermalManager.wait_for_hotend(EXT);
          }
        #endif
        #if HAS_HEATED_BED
          if (!DEBUGGING(DRYRUN) && thermalManager.degTargetBed() < hmiData.bedLevT) {
            thermalManager.setTargetBed(hmiData.bedLevT);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
            thermalManager.wait_for_bed_heating();
          }
        #endif
      #endif
    #endif
  #elif ENABLED(MESH_BED_LEVELING)
<<<<<<< HEAD
    Draw_ManualMesh_Menu();
  #endif
}

void DWIN_LevelingDone() {
  DEBUG_ECHOLNPGM("DWIN_LevelingDone");
  #if HAS_MESH
    #if PROUI_EX && HAS_BED_PROBE
      ProEx.LevelingDone();
    #else
      Goto_MeshViewer(true);
=======
    drawManualMeshMenu();
  #endif
}

void dwinLevelingDone() {
  DEBUG_ECHOLNPGM("dwinLevelingDone");
  #if HAS_MESH
    #if PROUI_EX && HAS_BED_PROBE
      proUIEx.levelingDone();
    #else
      gotoMeshViewer(true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    #endif
  #endif
}

#if HAS_MESH
<<<<<<< HEAD
  void DWIN_MeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval) {
    char str_1[6] = "";
    ui.status_printf(0, F(S_FMT " %i/%i Z=%s"), GET_TEXT_F(MSG_PROBING_POINT), cpos, tpos, dtostrf(zval, 1, 2, str_1));
=======
  void dwinMeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval) {
    ui.set_status(
      &MString<32>(GET_TEXT_F(MSG_PROBING_POINT), ' ', cpos, '/', tpos, F(" Z="), p_float_t(zval, 2))
    );
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

// PID/MPC process

#if HAS_PLOT && ANY(HAS_PID_HEATING, MPC_AUTOTUNE)
  celsius_t _maxtemp, _target;
<<<<<<< HEAD
  void DWIN_Draw_PID_MPC_Popup() {
    frame_rect_t gfrm = {40, 180, DWIN_WIDTH - 80, 120};
    DWINUI::ClearMainArea();
    Draw_Popup_Bkgd();
    // Draw labels
    switch (HMI_value.tempcontrol) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_MPC_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("MPC target:    Celsius"));
          break;
      #endif
      #if EITHER(PIDTEMP, PIDTEMPBED)
        TERN_(PIDTEMP,    case PIDTEMP_START:)
        TERN_(PIDTEMPBED, case PIDTEMPBED_START:)
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 100, GET_TEXT_F(MSG_PID_AUTOTUNE));
          DWINUI::Draw_String(HMI_data.PopupTxt_Color, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:    Celsius"));
=======
  void dwinDrawPIDMPCPopup() {
    constexpr frame_rect_t gfrm = { 40, 180, DWIN_WIDTH - 80, 120 };
    DWINUI::clearMainArea();
    drawPopupBkgd();
    // Draw labels
    switch (hmiValue.tempControl) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 100, GET_TEXT_F(MSG_MPC_AUTOTUNE));
          DWINUI::drawString(hmiData.colorPopupTxt, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("MPC target:    Celsius"));
          break;
      #endif
      #if ANY(PIDTEMP, PIDTEMPBED)
        TERN_(PIDTEMP,    case PIDTEMP_START:)
        TERN_(PIDTEMPBED, case PIDTEMPBED_START:)
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 100, GET_TEXT_F(MSG_PID_AUTOTUNE));
          DWINUI::drawString(hmiData.colorPopupTxt, gfrm.x, gfrm.y - DWINUI::fontHeight() - 4, F("PID target:    Celsius"));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          break;
      #endif
      default: break;
    }
<<<<<<< HEAD
    switch (HMI_value.tempcontrol) {
      #if EITHER(PIDTEMP, MPC_AUTOTUNE)
        TERN_(PIDTEMP,      case PIDTEMP_START:)
        TERN_(MPC_AUTOTUNE, case MPCTEMP_START:)
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for Nozzle is running."));
=======
    switch (hmiValue.tempControl) {
      #if ANY(PIDTEMP, MPC_AUTOTUNE)
        TERN_(PIDTEMP,      case PIDTEMP_START:)
        TERN_(MPC_AUTOTUNE, case MPCTEMP_START:)
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 120, F("for Nozzle is running."));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
<<<<<<< HEAD
          DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 120, F("for BED is running."));
=======
          DWINUI::drawCenteredString(hmiData.colorPopupTxt, 120, F("for BED is running."));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          break;
      #endif
      default: break;
    }
    // Set values
<<<<<<< HEAD
    switch (HMI_value.tempcontrol) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[0];
=======
    switch (hmiValue.tempControl) {
      #if ENABLED(MPC_AUTOTUNE)
        case MPCTEMP_START:
          _maxtemp = thermalManager.hotend_maxtemp[EXT];
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          _target = 200;
          break;
      #endif
      #if ENABLED(PIDTEMP)
        case PIDTEMP_START:
<<<<<<< HEAD
          _maxtemp = thermalManager.hotend_maxtemp[0];
          _target = HMI_data.HotendPidT;
=======
          _maxtemp = thermalManager.hotend_maxtemp[EXT];
          _target = hmiData.hotendPidT;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          break;
      #endif
      #if ENABLED(PIDTEMPBED)
        case PIDTEMPBED_START:
          _maxtemp = BED_MAXTEMP;
<<<<<<< HEAD
          _target = HMI_data.BedPidT;
=======
          _target = hmiData.bedPidT;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          break;
      #endif
      default: break;
    }
<<<<<<< HEAD
    plot.Draw(gfrm, _maxtemp, _target);
    DWINUI::Draw_Int(HMI_data.PopupTxt_Color, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, _target);
  }
#endif

#if HAS_PID_HEATING

  void DWIN_M303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp){
    if (seenC) HMI_data.PidCycles = c;
    if (seenS) {
      switch (hid) {
        OPTCODE(PIDTEMP,    case 0 ... HOTENDS - 1: HMI_data.HotendPidT = temp; break)
        OPTCODE(PIDTEMPBED, case H_BED:             HMI_data.BedPidT = temp;    break)
=======
    plot.draw(gfrm, _maxtemp, _target);
    DWINUI::drawInt(hmiData.colorPopupTxt, 3, gfrm.x + 90, gfrm.y - DWINUI::fontHeight() - 4, _target);
  }
#endif // HAS_PLOT && ANY(HAS_PID_HEATING, MPC_AUTOTUNE)

#if HAS_PID_HEATING

  void dwinM303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp){
    if (seenC) hmiData.pidCycles = c;
    if (seenS) {
      switch (hid) {
        OPTCODE(PIDTEMP,    case 0 ... HOTENDS - 1: hmiData.hotendPidT = temp; break)
        OPTCODE(PIDTEMPBED, case H_BED:             hmiData.bedPidT = temp;    break)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        default: break;
      }
    }
  }

<<<<<<< HEAD
  void DWIN_PidTuning(tempcontrol_t result) {
    HMI_value.tempcontrol = result;
    switch (result) {
      case PIDTEMPBED_START:
        HMI_SaveProcessID(PidProcess);
        #if HAS_PLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for BED is running."));
        #endif
        break;
      case PIDTEMP_START:
        HMI_SaveProcessID(PidProcess);
        #if HAS_PLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for Nozzle is running."));
=======
  void dwinPidTuning(tempcontrol_t result) {
    hmiValue.tempControl = result;
    switch (result) {
      case PIDTEMPBED_START:
        hmiSaveProcessID(ID_PIDProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for BED is running."));
        #endif
        break;
      case PIDTEMP_START:
        hmiSaveProcessID(ID_PIDProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE), F("for Nozzle is running."));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        #endif
        break;
      case PID_BAD_HEATER_ID:
        checkkey = last_checkkey;
<<<<<<< HEAD
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_BAD_HEATER_ID));
        break;
      case PID_TUNING_TIMEOUT:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case PID_TEMP_TOO_HIGH:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_TEMP_TOO_HIGH));
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
=======
        dwinPopupConfirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_BAD_HEATER_ID));
        break;
      case PID_TUNING_TIMEOUT:
        checkkey = last_checkkey;
        dwinPopupConfirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), GET_TEXT_F(MSG_PID_TIMEOUT));
        break;
      case PID_TEMP_TOO_HIGH:
        checkkey = last_checkkey;
        dwinPopupConfirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), GET_TEXT_F(MSG_TEMP_TOO_HIGH));
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
        dwinPopupConfirm(ICON_TempTooLow, GET_TEXT_F(MSG_PID_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        break;
      default:
        checkkey = last_checkkey;
        break;
    }
  }

#endif // HAS_PID_HEATING

#if ENABLED(MPC_AUTOTUNE)

<<<<<<< HEAD
  void DWIN_MPCTuning(tempcontrol_t result) {
    HMI_value.tempcontrol = result;
    switch (result) {
      case MPCTEMP_START:
        HMI_SaveProcessID(MPCProcess);
        #if HAS_PLOT
          DWIN_Draw_PID_MPC_Popup();
        #else
          DWIN_Draw_Popup(ICON_TempTooHigh, GET_TEXT_F(MSG_MPC_AUTOTUNE), F("for Nozzle is running."));
=======
  void dwinMPCTuning(tempcontrol_t result) {
    hmiValue.tempControl = result;
    switch (result) {
      case MPCTEMP_START:
        hmiSaveProcessID(ID_MPCProcess);
        #if HAS_PLOT
          dwinDrawPIDMPCPopup();
        #else
          dwinDrawPopup(ICON_TempTooHigh, GET_TEXT_F(MSG_MPC_AUTOTUNE), F("for Nozzle is running."));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        #endif
        break;
      case MPC_TEMP_ERROR:
        checkkey = last_checkkey;
<<<<<<< HEAD
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), F(STR_MPC_TEMPERATURE_ERROR));
=======
        dwinPopupConfirm(ICON_TempTooHigh, GET_TEXT_F(MSG_PID_AUTOTUNE_FAILED), F(STR_MPC_TEMPERATURE_ERROR));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        ui.reset_alert_level();
        break;
      case MPC_INTERRUPTED:
        checkkey = last_checkkey;
<<<<<<< HEAD
        DWIN_Popup_Confirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), F(STR_MPC_AUTOTUNE_INTERRUPTED));
=======
        dwinPopupConfirm(ICON_TempTooHigh, GET_TEXT_F(MSG_ERROR), F(STR_MPC_AUTOTUNE_INTERRUPTED));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        ui.reset_alert_level();
        break;
      case AUTOTUNE_DONE:
        checkkey = last_checkkey;
<<<<<<< HEAD
        DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_MPC_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
=======
        dwinPopupConfirm(ICON_TempTooLow, GET_TEXT_F(MSG_MPC_AUTOTUNE), GET_TEXT_F(MSG_BUTTON_DONE));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
        ui.reset_alert_level();
        break;
      default:
        checkkey = last_checkkey;
        ui.reset_alert_level();
        break;
    }
  }

#endif // MPC_AUTOTUNE

// Started a Print Job
<<<<<<< HEAD
void DWIN_Print_Started() {
  DEBUG_ECHOLNPGM("DWIN_Print_Started: ", SD_Printing());
  TERN_(HAS_GCODE_PREVIEW, if (Host_Printing()) Preview_Invalidate());
  ui.progress_reset();
  ui.reset_remaining_time();
  HMI_flag.pause_flag = false;
  HMI_flag.abort_flag = false;
  select_print.reset();
  #if PROUI_EX
    if (!fileprop.isConfig) Goto_PrintProcess();
  #else
    Goto_PrintProcess();
=======
void dwinPrintStarted() {
  DEBUG_ECHOLNPGM("dwinPrintStarted: ", sdPrinting());
  TERN_(HAS_GCODE_PREVIEW, if (hostPrinting()) gPreview.invalidate());
  TERN_(SET_PROGRESS_PERCENT, ui.progress_reset());
  TERN_(SET_REMAINING_TIME, ui.reset_remaining_time());
  hmiFlag.pause_flag = false;
  hmiFlag.abort_flag = false;
  select_print.reset();
  #if PROUI_EX
    if (!fileprop.isConfig) gotoPrintProcess();
  #else
    gotoPrintProcess();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
}

// Pause a print job
<<<<<<< HEAD
void DWIN_Print_Pause() {
  DEBUG_ECHOLNPGM("DWIN_Print_Pause");
  ICON_ResumeOrPause();
}

// Resume print job
void DWIN_Print_Resume() {
  DEBUG_ECHOLNPGM("DWIN_Print_Resume");
  ICON_ResumeOrPause();
=======
void dwinPrintPause() {
  DEBUG_ECHOLNPGM("dwinPrintPause");
  iconResumeOrPause();
}

// Resume print job
void dwinPrintResume() {
  DEBUG_ECHOLNPGM("dwinPrintResume");
  iconResumeOrPause();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  LCD_MESSAGE(MSG_RESUME_PRINT);
}

// Ended print job
<<<<<<< HEAD
void DWIN_Print_Finished() {
  DEBUG_ECHOLNPGM("DWIN_Print_Finished");
  HMI_flag.abort_flag = false;
  HMI_flag.pause_flag = false;
  wait_for_heatup = false;
  #if PROUI_EX
    if (!fileprop.isConfig) Goto_PrintDone(); else fileprop.isConfig = false;
  #else
    Goto_PrintDone();
=======
void dwinPrintFinished() {
  DEBUG_ECHOLNPGM("dwinPrintFinished");
  hmiFlag.abort_flag = false;
  hmiFlag.pause_flag = false;
  wait_for_heatup = false;
  #if PROUI_EX
    if (!fileprop.isConfig) gotoPrintDone(); else fileprop.isConfig = false;
  #else
    gotoPrintDone();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
}

// Print was aborted
<<<<<<< HEAD
void DWIN_Print_Aborted() {
  DEBUG_ECHOLNPGM("DWIN_Print_Aborted");
  #if PROUI_EX
    if (all_axes_homed()) {
      char cmd[25] = "";
      const int16_t zpos = current_position.z + PRO_data.Park_point.z;
      sprintf_P(cmd, PSTR("G0Z%i\nG0F2000Y%i"), zpos, PRO_data.Park_point.y);
      queue.inject(cmd);
    }
  #endif
  hostui.notify("Print Aborted");
  DWIN_Print_Finished();
}

#if HAS_FILAMENT_SENSOR
  // Filament Runout process
  void DWIN_FilamentRunout(const uint8_t extruder) { LCD_MESSAGE(MSG_RUNOUT_SENSOR); }
#endif

void DWIN_SetColorDefaults() {
  HMI_data.Background_Color = Def_Background_Color;
  HMI_data.Cursor_Color     = Def_Cursor_Color;
  HMI_data.TitleBg_Color    = Def_TitleBg_Color;
  HMI_data.TitleTxt_Color   = Def_TitleTxt_Color;
  HMI_data.Text_Color       = Def_Text_Color;
  HMI_data.Selected_Color   = Def_Selected_Color;
  HMI_data.SplitLine_Color  = Def_SplitLine_Color;
  HMI_data.Highlight_Color  = Def_Highlight_Color;
  HMI_data.StatusBg_Color   = Def_StatusBg_Color;
  HMI_data.StatusTxt_Color  = Def_StatusTxt_Color;
  HMI_data.PopupBg_Color    = Def_PopupBg_Color;
  HMI_data.PopupTxt_Color   = Def_PopupTxt_Color;
  HMI_data.AlertBg_Color    = Def_AlertBg_Color;
  HMI_data.AlertTxt_Color   = Def_AlertTxt_Color;
  HMI_data.PercentTxt_Color = Def_PercentTxt_Color;
  HMI_data.Barfill_Color    = Def_Barfill_Color;
  HMI_data.Indicator_Color  = Def_Indicator_Color;
  HMI_data.Coordinate_Color = Def_Coordinate_Color;
}

void DWIN_SetDataDefaults() {
  DEBUG_ECHOLNPGM("DWIN_SetDataDefaults");
  DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  TERN_(PIDTEMP, HMI_data.HotendPidT = DEF_HOTENDPIDT);
  TERN_(PIDTEMPBED, HMI_data.BedPidT = DEF_BEDPIDT);
  TERN_(HAS_PID_HEATING, HMI_data.PidCycles = DEF_PIDCYCLES);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    HMI_data.ExtMinT = EXTRUDE_MINTEMP;
    ApplyExtMinT();
  #endif
  #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    HMI_data.BedLevT = LEVELING_BED_TEMP;
  #endif
  TERN_(BAUD_RATE_GCODE, HMI_data.Baud115K = (BAUDRATE == 115200));
  #if BOTH(LCD_BED_TRAMMING, HAS_BED_PROBE)
    HMI_data.FullManualTramming = DISABLED(BED_TRAMMING_USE_PROBE);
  #endif
  #if ENABLED(MEDIASORT_MENU_ITEM)
    HMI_data.MediaSort = true;
    card.setSortOn(true);
  #endif
  HMI_data.MediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    HMI_data.z_after_homing = DEF_Z_AFTER_HOMING;
  #endif
  #if DISABLED(HAS_BED_PROBE)
    HMI_data.ManualZOffset = 0;
  #endif
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    #if ENABLED(LED_COLOR_PRESETS)
      leds.set_default();
      ApplyLEDColor();
    #else
      HMI_data.Led_Color = Def_Leds_Color;
      leds.set_color(
        (HMI_data.Led_Color >> 16) & 0xFF,
        (HMI_data.Led_Color >>  8) & 0xFF,
        (HMI_data.Led_Color >>  0) & 0xFF
        OPTARG(HAS_WHITE_LED, (HMI_data.Led_Color >> 24) & 0xFF)
      );
    #endif
  #endif
  TERN_(ADAPTIVE_STEP_SMOOTHING, HMI_data.AdaptiveStepSmoothing = true);
  TERN_(HAS_GCODE_PREVIEW, HMI_data.EnablePreview = true);
=======
void dwinPrintAborted() {
  DEBUG_ECHOLNPGM("dwinPrintAborted");
  #ifdef SD_FINISHED_RELEASECOMMAND
    queue.inject(SD_FINISHED_RELEASECOMMAND);
  #endif
  hostui.notify("Print Aborted");
  dwinPrintFinished();
}

void dwinSetColorDefaults() {
  hmiData.colorBackground = defColorBackground;
  hmiData.colorCursor     = defColorCursor;
  hmiData.colorTitleBg    = defColorTitleBg;
  hmiData.colorTitleTxt   = defColorTitleTxt;
  hmiData.colorText       = defColorText;
  hmiData.colorSelected   = defColorSelected;
  hmiData.colorSplitLine  = defColorSplitLine;
  hmiData.colorHighlight  = defColorHighlight;
  hmiData.colorStatusBg   = defColorStatusBg;
  hmiData.colorStatusTxt  = defColorStatusTxt;
  hmiData.colorPopupBg    = defColorPopupBg;
  hmiData.colorPopupTxt   = defColorPopupTxt;
  hmiData.colorAlertBg    = defColorAlertBg;
  hmiData.colorAlertTxt   = defColorAlertTxt;
  hmiData.colorPercentTxt = defColorPercentTxt;
  hmiData.colorBarfill    = defColorBarfill;
  hmiData.colorIndicator  = defColorIndicator;
  hmiData.colorCoordinate = defColorCoordinate;
}

void dwinSetDataDefaults() {
  DEBUG_ECHOLNPGM("dwinSetDataDefaults");
  dwinSetColorDefaults();
  DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
  TERN_(PIDTEMP, hmiData.hotendPidT = DEF_HOTENDPIDT);
  TERN_(PIDTEMPBED, hmiData.bedPidT = DEF_BEDPIDT);
  TERN_(HAS_PID_HEATING, hmiData.pidCycles = DEF_PIDCYCLES);
  #if ENABLED(PREVENT_COLD_EXTRUSION)
    hmiData.extMinT = EXTRUDE_MINTEMP;
    applyExtMinT();
  #endif
  #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    hmiData.bedLevT = LEVELING_BED_TEMP;
  #endif
  TERN_(BAUD_RATE_GCODE, hmiData.baud115K = (BAUDRATE == 115200));
  #if ALL(LCD_BED_TRAMMING, HAS_BED_PROBE)
    hmiData.fullManualTramming = DISABLED(BED_TRAMMING_USE_PROBE);
  #endif
  #if ENABLED(MEDIASORT_MENU_ITEM)
    hmiData.mediaSort = true;
    card.setSortOn(true);
  #endif
  hmiData.mediaAutoMount = ENABLED(HAS_SD_EXTENDER);
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    hmiData.zAfterHoming = DEF_Z_AFTER_HOMING;
  #endif
  #if DISABLED(HAS_BED_PROBE)
    hmiData.manualZOffset = 0;
  #endif
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    #if ENABLED(LED_COLOR_PRESETS)
      leds.set_default();
      applyLEDColor();
    #else
      hmiData.ledColor = Def_Leds_Color;
      leds.set_color(
        (hmiData.ledColor >> 16) & 0xFF,
        (hmiData.ledColor >>  8) & 0xFF,
        (hmiData.ledColor >>  0) & 0xFF
        OPTARG(HAS_WHITE_LED, (hmiData.ledColor >> 24) & 0xFF)
      );
    #endif
  #endif
  TERN_(ADAPTIVE_STEP_SMOOTHING, hmiData.adaptiveStepSmoothing = true);
  TERN_(HAS_GCODE_PREVIEW, hmiData.enablePreview = true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if PROUI_EX
    PRO_data.x_bed_size = DEF_X_BED_SIZE;
    PRO_data.y_bed_size = DEF_Y_BED_SIZE;
    PRO_data.x_min_pos  = DEF_X_MIN_POS;
    PRO_data.y_min_pos  = DEF_Y_MIN_POS;
    PRO_data.x_max_pos  = DEF_X_MAX_POS;
    PRO_data.y_max_pos  = DEF_Y_MAX_POS;
    PRO_data.z_max_pos  = DEF_Z_MAX_POS;
    #if HAS_MESH
      PRO_data.grid_max_points = DEF_GRID_MAX_POINTS;
      PRO_data.mesh_min_x = DEF_MESH_MIN_X;
      PRO_data.mesh_max_x = DEF_MESH_MAX_X;
      PRO_data.mesh_min_y = DEF_MESH_MIN_Y;
      PRO_data.mesh_max_y = DEF_MESH_MAX_Y;
<<<<<<< HEAD
    #endif
    #if HAS_BED_PROBE
=======
      meshViewer.meshmode = ENABLED(USE_GRID_MESHVIEWER);
      meshViewer.meshfont = TERN(TJC_DISPLAY, font8x16, font6x12);
    #endif
    #if HAS_BED_PROBE
      PRO_data.probezfix = DEF_PROBEZFIX;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      PRO_data.zprobefeedslow = DEF_Z_PROBE_FEEDRATE_SLOW;
      PRO_data.multiple_probing = MULTIPLE_PROBING;
    #endif
    TERN_(HAS_EXTRUDERS, PRO_data.Invert_E0 = DEF_INVERT_E0_DIR);
    #if ENABLED(NOZZLE_PARK_FEATURE)
      PRO_data.Park_point = DEF_NOZZLE_PARK_POINT;
    #endif
    #if HAS_FILAMENT_SENSOR
<<<<<<< HEAD
=======
      runout.enabled = false;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      PRO_data.Runout_active_state = FIL_RUNOUT_STATE;
      PRO_data.FilamentMotionSensor = DEF_FIL_MOTION_SENSOR;
    #endif
    PRO_data.hotend_maxtemp = HEATER_0_MAXTEMP;
    #if HAS_TOOLBAR
      const uint8_t _def[] = DEF_TBOPT;
<<<<<<< HEAD
      LOOP_L_N(i,TBMaxOpt) PRO_data.TBopt[i] = _def[i];
    #endif
    ProEx.SetData();
  #endif
}

void DWIN_CopySettingsTo(char * const buff) {
  DEBUG_ECHOLNPGM("DWIN_CopySettingsTo");
  DEBUG_ECHOLNPGM("HMI_data: ", sizeof(HMI_data_t));
  memcpy(buff, &HMI_data, sizeof(HMI_data_t));
  #if PROUI_EX
    DEBUG_ECHOLNPGM("PRO_data: ", sizeof(PRO_data_t));
    memcpy(buff + sizeof(HMI_data_t), &PRO_data, sizeof(PRO_data_t));
  #endif
}

void DWIN_CopySettingsFrom(const char * const buff) {
  DEBUG_ECHOLNPGM("DWIN_CopySettingsFrom");
  memcpy(&HMI_data, buff, sizeof(HMI_data_t));
  TERN_(PROUI_EX, memcpy(&PRO_data, buff + sizeof(HMI_data_t), sizeof(PRO_data)));
  if (HMI_data.Text_Color == HMI_data.Background_Color) DWIN_SetColorDefaults();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  TERN_(PREVENT_COLD_EXTRUSION, ApplyExtMinT());
  feedrate_percentage = 100;
  TERN_(BAUD_RATE_GCODE, if (HMI_data.Baud115K) SetBaud115K(); else SetBaud250K());
  TERN_(MEDIASORT_MENU_ITEM, card.setSortOn(HMI_data.MediaSort));
  #if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
      (HMI_data.Led_Color >> 16) & 0xFF,
      (HMI_data.Led_Color >>  8) & 0xFF,
      (HMI_data.Led_Color >>  0) & 0xFF
      OPTARG(HAS_WHITE_LED, (HMI_data.Led_Color >> 24) & 0xFF)
    );
  #endif
  TERN_(PROUI_EX, ProEx.LoadSettings());
=======
      for (uint8_t i = 0; i < TBMaxOpt; ++i) PRO_data.TBopt[i] = _def[i];
    #endif
    proUIEx.setData();
  #endif
}

void dwinCopySettingsTo(char * const buff) {
  DEBUG_ECHOLNPGM("dwinCopySettingsTo");
  DEBUG_ECHOLNPGM("hmiData: ", sizeof(hmi_data_t));
  memcpy(buff, &hmiData, sizeof(hmi_data_t));
  #if PROUI_EX
    DEBUG_ECHOLNPGM("PRO_data: ", sizeof(PRO_data_t));
    memcpy(buff + sizeof(hmi_data_t), &PRO_data, sizeof(PRO_data_t));
  #endif
}

void dwinCopySettingsFrom(const char * const buff) {
  DEBUG_ECHOLNPGM("dwinCopySettingsFrom");
  memcpy(&hmiData, buff, sizeof(hmi_data_t));
  #if PROUI_EX
    memcpy(&PRO_data, buff + sizeof(hmi_data_t), sizeof(PRO_data));
    proUIEx.loadSettings();
    TERN_(HAS_MESH, meshViewer.meshfont = TERN(TJC_DISPLAY, font8x16, font6x12));
  #endif
  if (hmiData.colorText == hmiData.colorBackground) dwinSetColorDefaults();
  DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
  TERN_(PREVENT_COLD_EXTRUSION, applyExtMinT());
  feedrate_percentage = 100;
  TERN_(BAUD_RATE_GCODE, if (hmiData.baud115K) setBaud115K(); else setBaud250K());
  TERN_(MEDIASORT_MENU_ITEM, card.setSortOn(hmiData.mediaSort));
  #if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
    leds.set_color(
      (hmiData.ledColor >> 16) & 0xFF,
      (hmiData.ledColor >>  8) & 0xFF,
      (hmiData.ledColor >>  0) & 0xFF
      OPTARG(HAS_WHITE_LED, (hmiData.ledColor >> 24) & 0xFF)
    );
  #endif
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

// Initialize or re-initialize the LCD
void MarlinUI::init_lcd() {
  DEBUG_ECHOLNPGM("MarlinUI::init_lcd");
  delay(750);   // wait to wakeup screen
<<<<<<< HEAD
  const bool hs = DWIN_Handshake(); UNUSED(hs);
  #if ENABLED(DEBUG_DWIN)
    SERIAL_ECHOPGM("DWIN_Handshake ");
    SERIAL_ECHOLNF(hs ? F("ok.") : F("error."));
  #endif
  DWIN_Frame_SetDir(1);
  Encoder_Configuration();
}

void DWIN_InitScreen() {
  DEBUG_ECHOLNPGM("DWIN_InitScreen");
  DWINUI::init();
  DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
  #if PROUI_EX
    ProEx.Init();
    safe_delay(2000);
  #else
    HMI_Init();
  #endif
  DWINUI::onTitleDraw = Draw_Title;
  InitMenu();
  checkkey = 255;
  hash_changed = true;
  DWIN_DrawStatusLine();
  DWIN_Draw_Dashboard();
  Goto_Main_Menu();
}

void MarlinUI::update() {
  HMI_SDCardUpdate();   // SD card update
  EachMomentUpdate();   // Status update
  DWIN_HandleScreen();  // Rotary encoder update
=======
  const bool hs = dwinHandshake(); UNUSED(hs);
  #if ENABLED(DEBUG_DWIN)
    SERIAL_ECHOPGM("dwinHandshake ");
    SERIAL_ECHOLN(hs ? F("ok.") : F("error."));
  #endif
  dwinFrameSetDir(1);
  encoderConfiguration();
}

void dwinInitScreen() {
  DEBUG_ECHOLNPGM("dwinInitScreen");
  DWINUI::init();
  DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
  #if PROUI_EX
    proUIEx.init();
    safe_delay(1500);
  #else
    hmiInit();
  #endif
  DWINUI::onTitleDraw = drawTitle;
  initMenu();
  checkkey = 255;
  hash_changed = true;
  dwinDrawStatusLine("");
  dwinDrawDashboard();
  gotoMainMenu();
}

void MarlinUI::update() {
  hmiSDCardUpdate();   // SD card update
  eachMomentUpdate();  // Status update
  dwinHandleScreen();  // Rotary encoder update
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

void MarlinUI::refresh() { /* Nothing to see here */ }

#if HAS_LCD_BRIGHTNESS
<<<<<<< HEAD
  void MarlinUI::_set_brightness() { DWIN_LCD_Brightness(backlight ? brightness : 0); }
#endif

void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component) {
  DWIN_Draw_Popup(ICON_BLTouch, GET_TEXT_F(MSG_PRINTER_KILLED), lcd_error);
  DWINUI::Draw_CenteredString(HMI_data.PopupTxt_Color, 270, GET_TEXT_F(MSG_TURN_OFF));
  DWIN_UpdateLCD();
}

void DWIN_RebootScreen() {
  DWIN_Frame_Clear(Color_Bg_Black);
  DWIN_JPG_ShowAndCache(0);
  DWINUI::Draw_CenteredString(Color_White, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  DWIN_UpdateLCD();
  safe_delay(500);
}

void DWIN_RedrawDash() {
  hash_changed = true;
  DWIN_DrawStatusMessage();
  DWIN_Draw_Dashboard();
}

void DWIN_RedrawScreen() {
  Draw_Main_Area();
  DWIN_RedrawDash();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void DWIN_Popup_Pause(FSTR_P const fmsg, uint8_t button /*= 0*/) {
    HMI_SaveProcessID(button ? WaitResponse : NothingToDo);
    DWIN_Show_Popup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), fmsg, button);
=======
  void MarlinUI::_set_brightness() { dwinLCDBrightness(backlight ? brightness : 0); }
#endif

void MarlinUI::kill_screen(FSTR_P const lcd_error, FSTR_P const lcd_component) {
  dwinDrawPopup(ICON_BLTouch, GET_TEXT_F(MSG_PRINTER_KILLED), lcd_error);
  DWINUI::drawCenteredString(hmiData.colorPopupTxt, 270, GET_TEXT_F(MSG_TURN_OFF));
  dwinUpdateLCD();
}

void dwinRebootScreen() {
  dwinFrameClear(COLOR_BG_BLACK);
  dwinJPGShowAndCache(0);
  DWINUI::drawCenteredString(COLOR_WHITE, 220, GET_TEXT_F(MSG_PLEASE_WAIT_REBOOT));
  dwinUpdateLCD();
  safe_delay(500);
}

void dwinRedrawDash() {
  hash_changed = true;
  dwinDrawStatusMessage();
  dwinDrawDashboard();
}

void dwinRedrawScreen() {
  drawMainArea();
  dwinRedrawDash();
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void dwinPopupPause(FSTR_P const fmsg, uint8_t button/*=0*/) {
    hmiSaveProcessID(button ? ID_WaitResponse : ID_NothingToDo);
    dwinShowPopup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), fmsg, button);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

  void MarlinUI::pause_show_message(const PauseMessage message, const PauseMode mode/*=PAUSE_MODE_SAME*/, const uint8_t extruder/*=active_extruder*/) {
    //if (mode == PAUSE_MODE_SAME) return;
    pause_mode = mode;
    switch (message) {
<<<<<<< HEAD
      case PAUSE_MESSAGE_PARKING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING));    break;                // M125
      case PAUSE_MESSAGE_CHANGING: DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT));   break;                // pause_print (M125, M600)
      case PAUSE_MESSAGE_WAITING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING), BTN_Continue); break;
      case PAUSE_MESSAGE_INSERT:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT), BTN_Continue); break;
      case PAUSE_MESSAGE_LOAD:     DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD));   break;
      case PAUSE_MESSAGE_UNLOAD:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;                // Unload of pause and Unload of M702
      case PAUSE_MESSAGE_PURGE:
        #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
          DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_CONT_PURGE));
        #else
          DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE));
        #endif
        break;
      case PAUSE_MESSAGE_OPTION:   Goto_FilamentPurge(); break;
      case PAUSE_MESSAGE_RESUME:   DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT), BTN_Continue);   break;
      case PAUSE_MESSAGE_HEATING:  DWIN_Popup_Pause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_STATUS:   HMI_ReturnScreen(); break;                                                      // Exit from Pause, Load and Unload
=======
      case PAUSE_MESSAGE_PARKING:  dwinPopupPause(GET_TEXT_F(MSG_PAUSE_PRINT_PARKING));    break;                // M125
      case PAUSE_MESSAGE_CHANGING: dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INIT));   break;                // pause_print (M125, M600)
      case PAUSE_MESSAGE_WAITING:  dwinPopupPause(GET_TEXT_F(MSG_ADVANCED_PAUSE_WAITING), BTN_Continue); break;
      case PAUSE_MESSAGE_INSERT:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_INSERT), BTN_Continue); break;
      case PAUSE_MESSAGE_LOAD:     dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_LOAD));   break;
      case PAUSE_MESSAGE_UNLOAD:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_UNLOAD)); break;                // Unload of pause and Unload of M702
      case PAUSE_MESSAGE_PURGE:
        #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)
          dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_CONT_PURGE));
        #else
          dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE));
        #endif
        break;
      case PAUSE_MESSAGE_OPTION:   gotoFilamentPurge(); break;
      case PAUSE_MESSAGE_RESUME:   dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_RESUME)); break;
      case PAUSE_MESSAGE_HEAT:     dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEAT), BTN_Continue);   break;
      case PAUSE_MESSAGE_HEATING:  dwinPopupPause(GET_TEXT_F(MSG_FILAMENT_CHANGE_HEATING)); break;
      case PAUSE_MESSAGE_STATUS:   hmiReturnScreen(); break;                                                      // Exit from Pause, Load and Unload
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      default: break;
    }
  }

<<<<<<< HEAD
  void Draw_Popup_FilamentPurge() {
    DWIN_Draw_Popup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE_CONTINUE));
    DWINUI::Draw_Button(BTN_Purge, 26, 280);
    DWINUI::Draw_Button(BTN_Continue, 146, 280);
    Draw_Select_Highlight(true);
  }

  void onClick_FilamentPurge() {
    if (HMI_flag.select_flag)
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // "Purge More" button
    else {
      HMI_SaveProcessID(NothingToDo);
=======
  void drawPopupFilamentPurge() {
    dwinDrawPopup(ICON_BLTouch, GET_TEXT_F(MSG_ADVANCED_PAUSE), GET_TEXT_F(MSG_FILAMENT_CHANGE_PURGE_CONTINUE));
    DWINUI::drawButton(BTN_Purge, 26, 280);
    DWINUI::drawButton(BTN_Continue, 146, 280);
    drawSelectHighlight(true);
  }

  void onClickFilamentPurge() {
    if (hmiFlag.select_flag)
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;  // "Purge More" button
    else {
      hmiSaveProcessID(ID_NothingToDo);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT;  // "Continue" button
    }
  }

<<<<<<< HEAD
  void Goto_FilamentPurge() {
    pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
    Goto_Popup(Draw_Popup_FilamentPurge, onClick_FilamentPurge);
=======
  void gotoFilamentPurge() {
    pause_menu_response = PAUSE_RESPONSE_WAIT_FOR;
    gotoPopup(drawPopupFilamentPurge, onClickFilamentPurge);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // ADVANCED_PAUSE_FEATURE

#if HAS_MESH
<<<<<<< HEAD
  void DWIN_MeshViewer() {
    if (!leveling_is_valid())
      DWIN_Popup_Continue(ICON_BLTouch, GET_TEXT_F(MSG_MESH_VIEWER), GET_TEXT_F(MSG_NO_VALID_MESH));
    else {
      HMI_SaveProcessID(WaitResponse);
      MeshViewer.Draw(false, true);
=======
  void dwinMeshViewer() {
    if (!leveling_is_valid())
      dwinPopupContinue(ICON_BLTouch, GET_TEXT_F(MSG_MESH_VIEWER), GET_TEXT_F(MSG_NO_VALID_MESH));
    else {
      hmiSaveProcessID(ID_WaitResponse);
      meshViewer.drawViewer(false, true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  }
#endif // HAS_MESH


#if HAS_LOCKSCREEN

<<<<<<< HEAD
  void DWIN_LockScreen() {
    if (checkkey != Locked) {
      lockScreen.rprocess = checkkey;
      checkkey = Locked;
=======
  void dwinLockScreen() {
    if (checkkey != ID_Locked) {
      lockScreen.rprocess = checkkey;
      checkkey = ID_Locked;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      lockScreen.init();
    }
  }

<<<<<<< HEAD
  void DWIN_UnLockScreen() {
    if (checkkey == Locked) {
      checkkey = lockScreen.rprocess;
      Draw_Main_Area();
    }
  }

  void HMI_LockScreen() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    lockScreen.onEncoder(encoder_diffState);
    if (lockScreen.isUnlocked()) DWIN_UnLockScreen();
  }

#endif //  HAS_LOCKSCREEN

#if HAS_GCODE_PREVIEW
  void SetPreview() { Toggle_Chkb_Line(HMI_data.EnablePreview); }

  void onClick_ConfirmToPrint() {
    DWIN_ResetStatusLine();
    if (HMI_flag.select_flag) {     // Confirm
      Goto_Main_Menu();
      return card.openAndPrintFile(card.filename);
    }
    else
      HMI_ReturnScreen();
  }
#endif

void Goto_ConfirmToPrint() {
=======
  void dwinUnLockScreen() {
    if (checkkey == ID_Locked) {
      checkkey = lockScreen.rprocess;
      drawMainArea();
    }
  }

  void hmiLockScreen() {
    EncoderState encoder_diffState = get_encoder_state();
    if (encoder_diffState == ENCODER_DIFF_NO) return;
    lockScreen.onEncoder(encoder_diffState);
    if (lockScreen.isUnlocked()) dwinUnLockScreen();
  }

#endif // HAS_LOCKSCREEN

#if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
  void setPreview() { toggleCheckboxLine(hmiData.enablePreview); }
#endif

#if ANY(HAS_GCODE_PREVIEW, ONE_CLICK_PRINT)
  void onClickConfirmToPrint() {
    dwinResetStatusLine();
    if (hmiFlag.select_flag) {     // Confirm
      gotoMainMenu();
      return card.openAndPrintFile(card.filename);
    }
    else
      hmiReturnScreen();
  }
#endif

#if ENABLED(ONE_CLICK_PRINT)
  void confirmToPrintPopup() {
    dwinPopupConfirmCancel(ICON_Info_0, GET_TEXT_F(MSG_START_PRINT));
    dwinResetStatusLine();
    ui.set_status_P(card.longest_filename(), true);
  }
#endif

void gotoConfirmToPrint() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if PROUI_EX
    fileprop.clear();
    fileprop.setname(card.filename);
    card.openFileRead(fileprop.name, 100);
    getFileHeader();
    card.closefile();
    if (fileprop.isConfig) return card.openAndPrintFile(card.filename);
  #endif
  #if ENABLED(CV_LASER_MODULE)
    if(fileprop.isLaser)
<<<<<<< HEAD
      if (laser_device.is_laser_device()) return Draw_LaserPrint_Menu(); else return Draw_LaserSettings_Menu();
    else
      LaserOn(false); // If it is not laser file turn off laser mode
  #endif
  #if HAS_GCODE_PREVIEW
    if (HMI_data.EnablePreview) return Goto_Popup(Preview_DrawFromSD, onClick_ConfirmToPrint);
=======
      if (laser_device.is_laser_device()) return drawLaserPrintMenu(); else return drawLaserSettingsMenu();
    else
      laserOn(false); // If it is not laser file turn off laser mode
  #endif
  #if HAS_GCODE_PREVIEW
    if (hmiData.enablePreview) return gotoPopup(gPreview.draw, onClickConfirmToPrint);
  #endif
  #if ENABLED(ONE_CLICK_PRINT)
    return gotoPopup(confirmToPrintPopup, onClickConfirmToPrint);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
  card.openAndPrintFile(card.filename); // Direct print SD file
}

#if HAS_ESDIAG
<<<<<<< HEAD
  void Draw_EndStopDiag() {
    HMI_SaveProcessID(ESDiagProcess);
    ESDiag.Draw();
=======
  void drawEndStopDiag() {
    hmiSaveProcessID(ID_ESDiagProcess);
    esDiag.draw();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

//=============================================================================
// MENU SUBSYSTEM
//=============================================================================

// Tool functions

#if ENABLED(EEPROM_SETTINGS)
<<<<<<< HEAD
  void WriteEeprom() {
    DWIN_DrawStatusLine(GET_TEXT_F(MSG_STORE_EEPROM));
    DWIN_UpdateLCD();
    DONE_BUZZ(settings.save());
  }

  void ReadEeprom() {
    const bool success = settings.load();
    DWIN_RedrawScreen();
    DONE_BUZZ(success);
  }

  void ResetEeprom() {
    settings.reset();
    DWIN_RedrawScreen();
=======

  void writeEeprom() {
    dwinDrawStatusLine(GET_TEXT_F(MSG_STORE_EEPROM));
    dwinUpdateLCD();
    DONE_BUZZ(settings.save());
  }

  void readEeprom() {
    const bool success = settings.load();
    dwinRedrawScreen();
    DONE_BUZZ(success);
  }

  void resetEeprom() {
    settings.reset();
    dwinRedrawScreen();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    DONE_BUZZ(true);
  }

  #if HAS_MESH
<<<<<<< HEAD
    void SaveMesh() { TERN(AUTO_BED_LEVELING_UBL, UBLMeshSave(), WriteEeprom()); }
  #endif
#endif

// Reset Printer
void RebootPrinter() {
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  DWIN_RebootScreen();
  hal.reboot();
}

void Goto_Info_Menu() {
  Draw_Info_Menu();
  DWIN_UpdateLCD();
  HMI_SaveProcessID(WaitResponse);
}

void DisableMotors() { queue.inject(F("M84")); }

void AutoLev() {   // Always reacquire the Z "home" position
  queue.inject(F(TERN(AUTO_BED_LEVELING_UBL, "G29P1", "G29")));
}

void AutoHome() { queue.inject_P(G28_STR); }

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void HomeX() { queue.inject(F("G28X")); }
  void HomeY() { queue.inject(F("G28Y")); }
  void HomeZ() { queue.inject(F("G28Z")); }
  #if BOTH(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    void ApplyZAfterHoming() { HMI_data.z_after_homing = MenuData.Value; };
    void SetZAfterHoming() { SetIntOnClick(0, 20, HMI_data.z_after_homing, ApplyZAfterHoming); }
=======
    void saveMesh() { TERN(AUTO_BED_LEVELING_UBL, ublMeshSave(), writeEeprom()); }
  #endif

#endif // EEPROM_SETTINGS

// Reset Printer
void rebootPrinter() {
  wait_for_heatup = wait_for_user = false;    // Stop waiting for heating/user
  thermalManager.disable_all_heaters();
  planner.finish_and_disable();
  dwinRebootScreen();
  hal.reboot();
}

void gotoInfoMenu() {
  drawInfoMenu();
  dwinUpdateLCD();
  hmiSaveProcessID(ID_WaitResponse);
}

void disableMotors() { queue.inject(F("M84")); }

void autoLev() {   // Always reacquire the Z "home" position
  queue.inject(F(TERN(AUTO_BED_LEVELING_UBL, "G29P1", "G29")));
}

void autoHome() { queue.inject_P(G28_STR); }

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void homeX() { queue.inject(F("G28X")); }
  void homeY() { queue.inject(F("G28Y")); }
  void homeZ() { queue.inject(F("G28Z")); }
  #if ALL(INDIVIDUAL_AXIS_HOMING_SUBMENU, MESH_BED_LEVELING)
    void applyZAfterHoming() { hmiData.zAfterHoming = menuData.value; };
    void setZAfterHoming() { setIntOnClick(0, 20, hmiData.zAfterHoming, applyZAfterHoming); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
#endif

#if HAS_ZOFFSET_ITEM

<<<<<<< HEAD
  void ApplyZOffset() { TERN_(EEPROM_SETTINGS, settings.save()); }
  void LiveZOffset() {
    #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      const_float_t step_zoffset = round((MenuData.Value / 100.0f) * planner.settings.axis_steps_per_mm[Z_AXIS]) - babystep.accum;
      if (BABYSTEP_ALLOWED()) babystep.add_steps(Z_AXIS, step_zoffset);
      //SERIAL_ECHOLNF(F("BB Steps: "), step_zoffset);
    #endif
  }
  void SetZOffset() {
    #if EITHER(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      babystep.accum = round(planner.settings.axis_steps_per_mm[Z_AXIS] * BABY_Z_VAR);
    #endif
    SetPFloatOnClick(Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX, 2, ApplyZOffset, LiveZOffset);
  }
#endif // HAS_ZOFFSET_ITEM

void SetMoveZto0() {
  #if ENABLED(Z_SAFE_HOMING)
    char cmd[54], str_1[5], str_2[5];
    sprintf_P(cmd, PSTR("G28XYO\nG28Z\nG0X%sY%sF5000\nG0Z0F300\nM400"),
      dtostrf(Z_SAFE_HOMING_X_POINT, 1, 1, str_1),
      dtostrf(Z_SAFE_HOMING_Y_POINT, 1, 1, str_2)
    );
    gcode.process_subcommands_now(cmd);
  #else
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(false));
    gcode.process_subcommands_now(F("G28Z\nG0Z0F300\nM400"));
  #endif
  ui.reset_status();
  DONE_BUZZ(true);
}

#if DISABLED(HAS_BED_PROBE)
  void HomeZandDisable() {
    SetMoveZto0();
    DisableMotors();
  }
#endif

#if HAS_PREHEAT
  #define _DoPreheat(N) void DoPreheat##N() { ui.preheat_all(N-1); }\
                        void DoPreheatHotend##N() { ui.preheat_hotend(N-1); }
  REPEAT_1(PREHEAT_COUNT, _DoPreheat)
#endif

void DoCoolDown() { thermalManager.cooldown(); }

bool EnableLiveMove = false;
void SetLiveMove() { Toggle_Chkb_Line(EnableLiveMove); }
void AxisMove(AxisEnum axis) {
  #if HAS_HOTEND
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(0)) {
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      return DWIN_Popup_Confirm(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
=======
  void applyZOffset() { TERN_(EEPROM_SETTINGS, settings.save()); }
  void liveZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      const_float_t step_zoffset = round((menuData.value / 100.0f) * planner.settings.axis_steps_per_mm[Z_AXIS]) - babystep.accum;
      if (BABYSTEP_ALLOWED()) babystep.add_steps(Z_AXIS, step_zoffset);
      //SERIAL_ECHOLN(F("BB Steps: "), step_zoffset);
    #endif
  }
  void setZOffset() {
    #if ANY(BABYSTEP_ZPROBE_OFFSET, JUST_BABYSTEP)
      babystep.accum = round(planner.settings.axis_steps_per_mm[Z_AXIS] * BABY_Z_VAR);
    #endif
    setPFloatOnClick(Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX, 2, applyZOffset, liveZOffset);
  }

  void setMoveZto0() {
    #if HAS_LEVELING && ANY(RESTORE_LEVELING_AFTER_G28, ENABLE_LEVELING_AFTER_G28)
      set_bed_leveling_enabled(false);
    #endif
    #if ENABLED(Z_SAFE_HOMING)
      gcode.process_subcommands_now(TS(F("G28Z\nG0F5000X"), Z_SAFE_HOMING_X_POINT, F("Y"), Z_SAFE_HOMING_Y_POINT, F("\nG0Z0F300\nM400")));
    #else
      gcode.process_subcommands_now(F("G28Z\nG0Z0F300\nM400"));
    #endif
    ui.reset_status();
    DONE_BUZZ(true);
  }

  #if !HAS_BED_PROBE
    void homeZandDisable() {
      setMoveZto0();
      disableMotors();
    }
  #endif

#endif // HAS_ZOFFSET_ITEM

#if HAS_PREHEAT
  #define _doPreheat(N) void DoPreheat##N() { ui.preheat_all(N-1); }\
                        void DoPreheatHotend##N() { ui.preheat_hotend(N-1); }
  REPEAT_1(PREHEAT_COUNT, _doPreheat)
#endif

void doCoolDown() { thermalManager.cooldown(); }

bool enableLiveMove = false;
void setLiveMove() { toggleCheckboxLine(enableLiveMove); }
void axisMove(AxisEnum axis) {
  #if HAS_HOTEND
    if (axis == E_AXIS && thermalManager.tooColdToExtrude(EXT)) {
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      return dwinPopupConfirm(ICON_TempTooLow, GET_TEXT_F(MSG_HOTEND_TOO_COLD), GET_TEXT_F(MSG_PLEASE_PREHEAT));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif
  planner.synchronize();
  if (!planner.is_full()) planner.buffer_line(current_position, manual_feedrate_mm_s[axis]);
}
<<<<<<< HEAD
void LiveMove() {
  if (!EnableLiveMove) return;
  *MenuData.P_Float = MenuData.Value / MINUNITMULT;
  AxisMove(HMI_value.axis);
}
void ApplyMove() {
  if (EnableLiveMove) return;
  AxisMove(HMI_value.axis);
}

#if ENABLED(CV_LASER_MODULE)
  void SetMoveX() {
    HMI_value.axis = X_AXIS;
    if (!laser_device.is_laser_device()) { SetPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
    else SetPFloatOnClick(X_MIN_POS - laser_device.homepos.x, X_MAX_POS - laser_device.homepos.x, UNITFDIGITS, ApplyMove, LiveMove);
  }
  void SetMoveY() {
    HMI_value.axis = Y_AXIS;
    if (!laser_device.is_laser_device()) SetPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove);
    else SetPFloatOnClick(Y_MIN_POS - laser_device.homepos.y, Y_MAX_POS - laser_device.homepos.y, UNITFDIGITS, ApplyMove, LiveMove);
  }
  void SetMoveZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick(laser_device.is_laser_device() ? -Z_MAX_POS : Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
#else
  void SetMoveX() { HMI_value.axis = X_AXIS; SetPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
  void SetMoveY() { HMI_value.axis = Y_AXIS; SetPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
  void SetMoveZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove); }
#endif

#if HAS_HOTEND
  void SetMoveE() {
    #define E_MIN_POS (current_position.e - (EXTRUDE_MAXLENGTH))
    #define E_MAX_POS (current_position.e + (EXTRUDE_MAXLENGTH))
    HMI_value.axis = E_AXIS; SetPFloatOnClick(E_MIN_POS, E_MAX_POS, UNITFDIGITS, ApplyMove, LiveMove);
  }
  void MoveE100() {
    current_position.e+=100;
    AxisMove(E_AXIS);
=======
void liveMove() {
  if (!enableLiveMove) return;
  *menuData.floatPtr = menuData.value / MINUNITMULT;
  axisMove(hmiValue.axis);
}
void applyMove() {
  if (enableLiveMove) return;
  axisMove(hmiValue.axis);
}

#if ENABLED(CV_LASER_MODULE)
  void setMoveX() {
    hmiValue.axis = X_AXIS;
    if (!laser_device.is_laser_device()) { setPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
    else setPFloatOnClick(X_MIN_POS - laser_device.homepos.x, X_MAX_POS - laser_device.homepos.x, UNITFDIGITS, applyMove, liveMove);
  }
  void setMoveY() {
    hmiValue.axis = Y_AXIS;
    if (!laser_device.is_laser_device()) setPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, applyMove, liveMove);
    else setPFloatOnClick(Y_MIN_POS - laser_device.homepos.y, Y_MAX_POS - laser_device.homepos.y, UNITFDIGITS, applyMove, liveMove);
  }
  void setMoveZ() { hmiValue.axis = Z_AXIS; setPFloatOnClick(laser_device.is_laser_device() ? -Z_MAX_POS : Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
#else
  void setMoveX() { hmiValue.axis = X_AXIS; setPFloatOnClick(X_MIN_POS, X_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
  void setMoveY() { hmiValue.axis = Y_AXIS; setPFloatOnClick(Y_MIN_POS, Y_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
  void setMoveZ() { hmiValue.axis = Z_AXIS; setPFloatOnClick(Z_MIN_POS, Z_MAX_POS, UNITFDIGITS, applyMove, liveMove); }
#endif

#if HAS_HOTEND
  void setMoveE() {
    #define E_MIN_POS (current_position.e - (EXTRUDE_MAXLENGTH))
    #define E_MAX_POS (current_position.e + (EXTRUDE_MAXLENGTH))
    hmiValue.axis = E_AXIS; setPFloatOnClick(E_MIN_POS, E_MAX_POS, UNITFDIGITS, applyMove, liveMove);
  }
  void moveE100() {
    current_position.e+=100;
    axisMove(E_AXIS);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
<<<<<<< HEAD
  void SetPwrLossr() {
    Toggle_Chkb_Line(recovery.enabled);
=======
  void setPwrLossr() {
    toggleCheckboxLine(recovery.enabled);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    recovery.changed();
  }
#endif

#if ENABLED(BAUD_RATE_GCODE)
<<<<<<< HEAD
  void SetBaudRate() {
    Toggle_Chkb_Line(HMI_data.Baud115K);
    if (HMI_data.Baud115K) SetBaud115K(); else SetBaud250K();
  }
  void SetBaud115K() { queue.inject(F("M575B115")); }
  void SetBaud250K() { queue.inject(F("M575B250")); }
#endif

#if HAS_LCD_BRIGHTNESS
  void ApplyBrightness() { ui.set_brightness(MenuData.Value); }
  void LiveBrightness() { DWIN_LCD_Brightness(MenuData.Value); }
  void SetBrightness() { SetIntOnClick(LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.brightness, ApplyBrightness, LiveBrightness); }
  void TurnOffBacklight() { HMI_SaveProcessID(WaitResponse); ui.set_brightness(0); DWIN_RedrawScreen(); }
#endif

#if ENABLED(CASE_LIGHT_MENU)
  void SetCaseLight() {
    Toggle_Chkb_Line(caselight.on);
    caselight.update_enabled();
  }
  #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
    void LiveCaseLightBrightness() { caselight.brightness = MenuData.Value; caselight.update_brightness(); }
    void SetCaseLightBrightness() { SetIntOnClick(0, 255, caselight.brightness, nullptr, LiveCaseLightBrightness); }
=======
  void setBaudRate() {
    toggleCheckboxLine(hmiData.baud115K);
    if (hmiData.baud115K) setBaud115K(); else setBaud250K();
  }
  void setBaud115K() { queue.inject(F("M575B115")); }
  void setBaud250K() { queue.inject(F("M575B250")); }
#endif

#if HAS_LCD_BRIGHTNESS
  void applyBrightness() { ui.set_brightness(menuData.value); }
  void liveBrightness() { dwinLCDBrightness(menuData.value); }
  void setBrightness() { setIntOnClick(LCD_BRIGHTNESS_MIN, LCD_BRIGHTNESS_MAX, ui.brightness, applyBrightness, liveBrightness); }
  void turnOffBacklight() { if (ui.backlight) ui.set_brightness(0); }
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    void dimBacklight() { if (ui.backlight) { dwinLCDBrightness(LCD_BRIGHTNESS_DIM); ui.backlight = false; } }
  #endif
#endif

#if ENABLED(CASE_LIGHT_MENU)
  void setCaseLight() {
    toggleCheckboxLine(caselight.on);
    caselight.update_enabled();
  }
  #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
    void liveCaseLightBrightness() { caselight.brightness = menuData.value; caselight.update_brightness(); }
    void setCaseLightBrightness() { setIntOnClick(0, 255, caselight.brightness, nullptr, liveCaseLightBrightness); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
#endif

#if ENABLED(LED_CONTROL_MENU)
<<<<<<< HEAD
  #if !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
    void SetLedStatus() {
      leds.toggle();
      Show_Chkb_Line(leds.lights_on);
    }
  #endif
  #if HAS_COLOR_LEDS
    void ApplyLEDColor() { HMI_data.Led_Color = TERN0(HAS_WHITE_LED, (leds.color.w << 24)) | (leds.color.r << 16) | (leds.color.g << 8) | leds.color.b; }
    void LiveLEDColor(uint8_t *color) { *color = MenuData.Value; leds.update(); }
    void LiveLEDColorR() { LiveLEDColor(&leds.color.r); }
    void LiveLEDColorG() { LiveLEDColor(&leds.color.g); }
    void LiveLEDColorB() { LiveLEDColor(&leds.color.b); }
    void SetLEDColorR() { SetIntOnClick(0, 255, leds.color.r, ApplyLEDColor, LiveLEDColorR); }
    void SetLEDColorG() { SetIntOnClick(0, 255, leds.color.g, ApplyLEDColor, LiveLEDColorG); }
    void SetLEDColorB() { SetIntOnClick(0, 255, leds.color.b, ApplyLEDColor, LiveLEDColorB); }
    #if HAS_WHITE_LED
      void LiveLEDColorW() { LiveLEDColor(&leds.color.w); }
      void SetLEDColorW() { SetIntOnClick(0, 255, leds.color.w, ApplyLEDColor, LiveLEDColorW); }
=======
  #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
    void setLedStatus() {
      leds.toggle();
      showChkbLine(leds.lights_on);
    }
  #endif
  #if HAS_COLOR_LEDS
    void applyLEDColor() { hmiData.ledColor = TERN0(HAS_WHITE_LED, (leds.color.w << 24)) | (leds.color.r << 16) | (leds.color.g << 8) | leds.color.b; }
    void liveLEDColor(uint8_t *color) { *color = menuData.value; leds.update(); }
    void liveLEDColorR() { liveLEDColor(&leds.color.r); }
    void liveLEDColorG() { liveLEDColor(&leds.color.g); }
    void liveLEDColorB() { liveLEDColor(&leds.color.b); }
    void setLEDColorR() { setIntOnClick(0, 255, leds.color.r, applyLEDColor, liveLEDColorR); }
    void setLEDColorG() { setIntOnClick(0, 255, leds.color.g, applyLEDColor, liveLEDColorG); }
    void setLEDColorB() { setIntOnClick(0, 255, leds.color.b, applyLEDColor, liveLEDColorB); }
    #if HAS_WHITE_LED
      void liveLEDColorW() { liveLEDColor(&leds.color.w); }
      void setLEDColorW() { setIntOnClick(0, 255, leds.color.w, applyLEDColor, liveLEDColorW); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    #endif
  #endif
#endif

#if ENABLED(SOUND_MENU_ITEM)
<<<<<<< HEAD
  void SetEnableSound() {
    Toggle_Chkb_Line(ui.sound_on);
=======
  void setEnableSound() {
    toggleCheckboxLine(ui.sound_on);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

#if HAS_HOME_OFFSET
<<<<<<< HEAD
  void ApplyHomeOffset() { set_home_offset(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  void SetHomeOffsetX() { HMI_value.axis = X_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetY() { HMI_value.axis = Y_AXIS; SetPFloatOnClick(-50, 50, UNITFDIGITS, ApplyHomeOffset); }
  void SetHomeOffsetZ() { HMI_value.axis = Z_AXIS; SetPFloatOnClick( -2,  2, UNITFDIGITS, ApplyHomeOffset); }
#endif

#if HAS_BED_PROBE
  void SetProbeOffsetX() { SetPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, ProEx.ApplyPhySet, nullptr)); }
  void SetProbeOffsetY() { SetPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, ProEx.ApplyPhySet, nullptr)); }
  void SetProbeOffsetZ() { SetPFloatOnClick(-10, 10, 2); }

  #if PROUI_EX
    void SetProbeZSpeed()  { SetPIntOnClick(60, Z_PROBE_FEEDRATE_FAST); }
    void ApplyProbeMultiple() { PRO_data.multiple_probing = (MenuData.Value > 1) ? MenuData.Value : 0; }
    void SetProbeMultiple()  { SetIntOnClick(0, 5, PRO_data.multiple_probing, ApplyProbeMultiple); }
  #endif

  void ProbeTest() {
    LCD_MESSAGE(MSG_M48_TEST);
    queue.inject(F("G28O\nM48 P10"));
  }
  void ProbeStow() { probe.stow(); }
  void ProbeDeploy() { probe.deploy(); }

  #if HAS_BLTOUCH_HS_MODE
    void SetHSMode() { Toggle_Chkb_Line(bltouch.high_speed_mode); }
=======
  void applyHomeOffset() { set_home_offset(hmiValue.axis, menuData.value / MINUNITMULT); }
  void setHomeOffsetX() { hmiValue.axis = X_AXIS; setPFloatOnClick(-50, 50, UNITFDIGITS, applyHomeOffset); }
  void setHomeOffsetY() { hmiValue.axis = Y_AXIS; setPFloatOnClick(-50, 50, UNITFDIGITS, applyHomeOffset); }
  void setHomeOffsetZ() { hmiValue.axis = Z_AXIS; setPFloatOnClick( -2,  2, UNITFDIGITS, applyHomeOffset); }
#endif

#if HAS_BED_PROBE
  void setProbeOffsetX() { setPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, proUIEx.applyPhySet, nullptr)); }
  void setProbeOffsetY() { setPFloatOnClick(-60, 60, UNITFDIGITS, TERN(PROUI_EX, proUIEx.applyPhySet, nullptr)); }
  void setProbeOffsetZ() { setPFloatOnClick(-10, 10, 2); }

  #if PROUI_EX
    void setProbeZSpeed()  { setPIntOnClick(60, Z_PROBE_FEEDRATE_FAST); }
    void applyProbeMultiple() { PRO_data.multiple_probing = (menuData.value > 1) ? menuData.value : 0; }
    void setProbeMultiple()  { setIntOnClick(0, 5, PRO_data.multiple_probing, applyProbeMultiple); }
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    void probeTest() {
      LCD_MESSAGE(MSG_M48_TEST);
      queue.inject(F("M48 P10"));
    }
  #endif

  void probeStow() { probe.stow(); }
  void probeDeploy() { probe.deploy(); }

  #if HAS_BLTOUCH_HS_MODE
    void setHSMode() { toggleCheckboxLine(bltouch.high_speed_mode); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif

#endif

#if PROUI_EX && ENABLED(NOZZLE_PARK_FEATURE)
<<<<<<< HEAD
  void SetParkPosX()   { SetPIntOnClick(X_MIN_POS, X_MAX_POS); }
  void SetParkPosY()   { SetPIntOnClick(Y_MIN_POS, Y_MAX_POS); }
  void SetParkZRaise() { SetPIntOnClick(Z_MIN_POS, 50); }
#endif

#if HAS_FILAMENT_SENSOR
  void SetRunoutEnable() {
    runout.reset();
    Toggle_Chkb_Line(runout.enabled);
  }

  #if PROUI_EX
    void LiveRunoutActive() { ProEx.DrawRunoutActive(true); }
    void SetRunoutActive() {
      uint8_t val;
      val = PRO_data.FilamentMotionSensor ? 2 : PRO_data.Runout_active_state ? 1 : 0;
      SetOnClick(SetIntNoDraw, 0, 2, 0, val, ProEx.ApplyRunoutActive, LiveRunoutActive);
      ProEx.DrawRunoutActive(true);
=======
  void setParkPosX()   { setPIntOnClick(X_MIN_POS, X_MAX_POS); }
  void setParkPosY()   { setPIntOnClick(Y_MIN_POS, Y_MAX_POS); }
  void setParkZRaise() { setPIntOnClick(Z_MIN_POS, 50); }
#endif

#if HAS_FILAMENT_SENSOR
  void setRunoutEnable() {
    runout.reset();
    toggleCheckboxLine(runout.enabled);
  }

  #if HAS_PROUI_RUNOUT_SENSOR
    void liveRunoutActive() { proUIEx.drawRunoutActive(true); }
    void setRunoutActive() {
      uint8_t val;
      val = PRO_data.FilamentMotionSensor ? 2 : PRO_data.Runout_active_state ? 1 : 0;
      setOnClick(ID_SetIntNoDraw, 0, 2, 0, val, proUIEx.applyRunoutActive, liveRunoutActive);
      proUIEx.drawRunoutActive(true);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif

  #if HAS_FILAMENT_RUNOUT_DISTANCE
<<<<<<< HEAD
    void ApplyRunoutDistance() { runout.set_runout_distance(MenuData.Value / MINUNITMULT); }
    void SetRunoutDistance() { SetFloatOnClick(0, 999, UNITFDIGITS, runout.runout_distance(), ApplyRunoutDistance); }
=======
    void applyRunoutDistance() { runout.set_runout_distance(menuData.value / MINUNITMULT); }
    void setRunoutDistance() { setFloatOnClick(0, 999, UNITFDIGITS, runout.runout_distance(), applyRunoutDistance); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)
<<<<<<< HEAD
  void SetFilLoad()   { SetPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
  void SetFilUnload() { SetPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void ApplyExtMinT() { thermalManager.extrude_min_temp = HMI_data.ExtMinT; thermalManager.allow_cold_extrude = (HMI_data.ExtMinT == 0); }
  void SetExtMinT() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP, ApplyExtMinT); }
#endif

void SetSpeed() { SetPIntOnClick(MIN_PRINT_SPEED, MAX_PRINT_SPEED); }

#if HAS_HOTEND
  void ApplyHotendTemp() { thermalManager.setTargetHotend(MenuData.Value, 0); }
  void SetHotendTemp() { SetIntOnClick(MIN_ETEMP, MAX_ETEMP, thermalManager.degTargetHotend(0), ApplyHotendTemp); }
#endif

#if HAS_HEATED_BED
  void ApplyBedTemp() { thermalManager.setTargetBed(MenuData.Value); }
  void SetBedTemp() { SetIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP, thermalManager.degTargetBed(), ApplyBedTemp); }
#endif

#if HAS_FAN
  void ApplyFanSpeed() { thermalManager.set_fan_speed(0, MenuData.Value); }
  void SetFanSpeed() { SetIntOnClick(0, 255, thermalManager.fan_speed[0], ApplyFanSpeed); }
=======
  void setFilLoad()   { setPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
  void setFilUnload() { setPFloatOnClick(0, MAX_LOAD_UNLOAD, UNITFDIGITS); }
#endif

#if ENABLED(PREVENT_COLD_EXTRUSION)
  void applyExtMinT() { thermalManager.extrude_min_temp = hmiData.extMinT; thermalManager.allow_cold_extrude = (hmiData.extMinT == 0); }
  void setExtMinT() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP, applyExtMinT); }
#endif

void setSpeed() { setPIntOnClick(MIN_PRINT_SPEED, MAX_PRINT_SPEED); }

#if HAS_HOTEND
  void applyHotendTemp() { thermalManager.setTargetHotend(menuData.value, 0); }
  void setHotendTemp() { setIntOnClick(MIN_ETEMP, MAX_ETEMP, thermalManager.degTargetHotend(EXT), applyHotendTemp); }
#endif

#if HAS_HEATED_BED
  void applyBedTemp() { thermalManager.setTargetBed(menuData.value); }
  void setBedTemp() { setIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP, thermalManager.degTargetBed(), applyBedTemp); }
#endif

#if HAS_FAN
  void applyFanSpeed() { thermalManager.set_fan_speed(0, menuData.value); }
  void setFanSpeed() { setIntOnClick(0, 255, thermalManager.fan_speed[FAN], applyFanSpeed); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)

<<<<<<< HEAD
  void ChangeFilament() {
    HMI_SaveProcessID(NothingToDo);
=======
  void changeFilament() {
    hmiSaveProcessID(ID_NothingToDo);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    queue.inject(F("M600 B2"));
  }

  #if ENABLED(NOZZLE_PARK_FEATURE)
<<<<<<< HEAD
    void ParkHead() {
=======
    void parkHead() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      LCD_MESSAGE(MSG_FILAMENT_PARK_ENABLED);
      queue.inject(F("G28O\nG27"));
    }
  #endif

  #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
<<<<<<< HEAD
    void UnloadFilament() {
=======
    void unloadFilament() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      LCD_MESSAGE(MSG_FILAMENTUNLOAD);
      queue.inject(F("M702 Z20"));
    }

<<<<<<< HEAD
    void LoadFilament() {
=======
    void loadFilament() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      LCD_MESSAGE(MSG_FILAMENTLOAD);
      queue.inject(F("M701 Z20"));
    }
  #endif

#endif // ADVANCED_PAUSE_FEATURE

<<<<<<< HEAD
void SetFlow() { SetPIntOnClick(MIN_PRINT_FLOW, MAX_PRINT_FLOW, []{ planner.refresh_e_factor(0); }); }

// Bed Tramming
#if ENABLED(LCD_BED_TRAMMING)
  TERN(HAS_BED_PROBE, float, void) Tram(uint8_t point, bool stow_probe/*=true*/) {
    char cmd[100] = "";
    #if HAS_BED_PROBE
      static bool inLev = false;
      float xpos = 0, ypos = 0, zval = 0;
      char str_1[6] = "", str_2[6] = "", str_3[6] = "";
      if (inLev) return NAN;
    #else
      int16_t xpos = 0, ypos = 0;
    #endif
    gcode.process_subcommands_now(F("G28O"));
    switch (point) {
      case 0:
        LCD_MESSAGE(MSG_LEVBED_FL);
        xpos = bed_tramming_inset_lfbr[0];
        ypos = bed_tramming_inset_lfbr[1];
        break;
      case 1:
        LCD_MESSAGE(MSG_LEVBED_FR);
        xpos = X_BED_SIZE - bed_tramming_inset_lfbr[2];
        ypos = bed_tramming_inset_lfbr[1];
        break;
      case 2:
        LCD_MESSAGE(MSG_LEVBED_BR);
        xpos = X_BED_SIZE - bed_tramming_inset_lfbr[2];
        ypos = Y_BED_SIZE - bed_tramming_inset_lfbr[3];
        break;
      case 3:
        LCD_MESSAGE(MSG_LEVBED_BL);
        xpos = bed_tramming_inset_lfbr[0];
        ypos = Y_BED_SIZE - bed_tramming_inset_lfbr[3];
        break;
      case 4:
        LCD_MESSAGE(MSG_LEVBED_C);
        xpos = X_BED_SIZE / 2;
        ypos = Y_BED_SIZE / 2;
        break;
    }

    #if HAS_BED_PROBE

      if (HMI_data.FullManualTramming) {
        set_bed_leveling_enabled(false);
        #define FMT "M420S0\nG90\nG0Z" STRINGIFY(BED_TRAMMING_Z_HOP) "F300\nG0X%sY%sF5000\nG0Z" STRINGIFY(BED_TRAMMING_HEIGHT) "F300"
        sprintf_P(cmd, PSTR(FMT),
          dtostrf(xpos, 1, 1, str_1),
          dtostrf(ypos, 1, 1, str_2)
        );
        gcode.process_subcommands_now(cmd);
      }
      else {
        LIMIT(xpos, MESH_MIN_X, MESH_MAX_X);
        LIMIT(ypos, MESH_MIN_Y, MESH_MAX_Y);
        if (stow_probe) probe.stow();
        inLev = true;
        zval = probe.probe_at_point(xpos, ypos, stow_probe ? PROBE_PT_STOW : PROBE_PT_RAISE);
        if (!isnan(zval)) {
          ui.status_printf(0, F("X:%s, Y:%s, Z:%s"),
            dtostrf(xpos, 1, 1, str_1),
            dtostrf(ypos, 1, 1, str_2),
            dtostrf(zval, 1, 2, str_3)
          );
        }
        else LCD_MESSAGE(MSG_M48_OUT_OF_BOUNDS);
        inLev = false;
      }
      return zval;

    #else // !HAS_BED_PROBE

      #define FMT "M420S0\nG28O\nG90\nG0Z" STRINGIFY(BED_TRAMMING_Z_HOP) "F300\nG0X%iY%iF5000\nG0Z" STRINGIFY(BED_TRAMMING_HEIGHT) "F300"
      sprintf_P(cmd, PSTR(FMT), xpos, ypos);
      queue.inject(cmd);

    #endif
  }

  void TramFL() { Tram(0); }
  void TramFR() { Tram(1); }
  void TramBR() { Tram(2); }
  void TramBL() { Tram(3); }
  void TramC () { Tram(4); }

  #if HAS_BED_PROBE

    void Trammingwizard() {
      if (HMI_data.FullManualTramming) {
        LCD_MESSAGE_F("Disable manual tramming");
        return;
      } else LCD_MESSAGE_F("Bed tramming wizzard");
      bed_mesh_t zval = {0};
      probe.stow();
      zval[0][0] = Tram(0, false);  // First tram point can do Homing
      checkkey = NothingToDo;       // After home disable user input
      MeshViewer.DrawMeshGrid(2, 2);
      MeshViewer.DrawMeshPoint(0, 0, zval[0][0]);
      zval[1][0] = Tram(1, false);
      MeshViewer.DrawMeshPoint(1, 0, zval[1][0]);
      zval[1][1] = Tram(2, false);
      MeshViewer.DrawMeshPoint(1, 1, zval[1][1]);
      zval[0][1] = Tram(3, false);
      probe.stow();
      MeshViewer.DrawMeshPoint(0, 1, zval[0][1]);
      DWINUI::Draw_CenteredString(140, F("Calculating average"));
      DWINUI::Draw_CenteredString(160, F("and relative heights"));
      safe_delay(1000);
      float avg = 0.0f;
      LOOP_L_N(x, 2) LOOP_L_N(y, 2) avg += zval[x][y];
      avg /= 4.0f;
      LOOP_L_N(x, 2) LOOP_L_N(y, 2) zval[x][y] -= avg;
      MeshViewer.DrawMesh(zval, 2, 2);
      ui.reset_status();

      #ifndef BED_TRAMMING_PROBE_TOLERANCE
        #define BED_TRAMMING_PROBE_TOLERANCE 0.05
      #endif

      if (ABS(MeshViewer.max - MeshViewer.min) < BED_TRAMMING_PROBE_TOLERANCE) {
        DWINUI::Draw_CenteredString(140, F("Corners leveled"));
        DWINUI::Draw_CenteredString(160, F("Tolerance achieved!"));
      }
      else {
        uint8_t p = 0;
        float max = 0;
        FSTR_P plabel;
        bool s = true;
        LOOP_L_N(x, 2) LOOP_L_N(y, 2) {
          const float d = ABS(zval[x][y]);
          if (max < d) {
            s = (zval[x][y] >= 0);
            max = d;
            p = x + 2 * y;
          }
        }
        switch (p) {
          case 0b00 : plabel = GET_TEXT_F(MSG_LEVBED_FL); break;
          case 0b01 : plabel = GET_TEXT_F(MSG_LEVBED_FR); break;
          case 0b10 : plabel = GET_TEXT_F(MSG_LEVBED_BL); break;
          case 0b11 : plabel = GET_TEXT_F(MSG_LEVBED_BR); break;
          default   : plabel = F(""); break;
        }
        DWINUI::Draw_CenteredString(120, F("Corners not leveled"));
        DWINUI::Draw_CenteredString(140, F("Knob adjustment required"));
        DWINUI::Draw_CenteredString(Color_Green, 160, s ? F("Lower") : F("Raise"));
        DWINUI::Draw_CenteredString(Color_Green, 180, plabel);
      }
      DWINUI::Draw_Button(BTN_Continue, 86, 305);
      checkkey = Menu;
      HMI_SaveProcessID(WaitResponse);
    }

    void SetManualTramming() {
      Toggle_Chkb_Line(HMI_data.FullManualTramming);
    }

  #endif // HAS_BED_PROBE
#endif

#if ENABLED(MESH_BED_LEVELING)

  void ManualMeshStart() {
    LCD_MESSAGE(MSG_UBL_BUILD_MESH_MENU);
    gcode.process_subcommands_now(F("G28XYO\nG28Z\nM211S0\nG29S1"));
    #ifdef MANUAL_PROBE_START_Z
      const uint8_t line = CurrentMenu->line(MMeshMoveZItem->pos);
      DWINUI::Draw_Signed_Float(HMI_data.Text_Color, HMI_data.Background_Color, 3, 2, VALX - 2 * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(line), MANUAL_PROBE_START_Z);
    #endif
  }

  void LiveMeshMoveZ() {
    *MenuData.P_Float = MenuData.Value / POW(10, 2);
=======
void setFlow() { setPIntOnClick(MIN_PRINT_FLOW, MAX_PRINT_FLOW, []{ planner.refresh_e_factor(EXT); }); }


#if ENABLED(MESH_BED_LEVELING)

  void manualMeshStart() {
    LCD_MESSAGE(MSG_UBL_BUILD_MESH_MENU);
    gcode.process_subcommands_now(F("G28XYO\nG28Z\nM211S0\nG29S1"));
    #ifdef MANUAL_PROBE_START_Z
      const uint8_t line = currentMenu->line(mMeshMoveZItem->pos);
      DWINUI::drawSignedFloat(hmiData.colorText, hmiData.colorBackground, 3, 2, VALX - 2 * DWINUI::fontWidth(DWIN_FONT_MENU), MBASE(line), MANUAL_PROBE_START_Z);
    #endif
  }

  void liveMeshMoveZ() {
    *menuData.floatPtr = menuData.value / POW(10, 2);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    if (!planner.is_full()) {
      planner.synchronize();
      planner.buffer_line(current_position, manual_feedrate_mm_s[Z_AXIS]);
    }
  }
<<<<<<< HEAD
  void SetMMeshMoveZ() { SetPFloatOnClick(-1, 1, 2, planner.synchronize, LiveMeshMoveZ); }

  void ManualMeshContinue() {
    gcode.process_subcommands_now(F("G29S2"));
    MMeshMoveZItem->redraw();
  }

  void ManualMeshSave() {
=======
  void setMMeshMoveZ() { setPFloatOnClick(-1, 1, 2, planner.synchronize, liveMeshMoveZ); }

  void manualMeshContinue() {
    gcode.process_subcommands_now(F("G29S2"));
    mMeshMoveZItem->redraw();
  }

  void manualMeshSave() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    LCD_MESSAGE(MSG_UBL_STORAGE_MESH_MENU);
    queue.inject(F("M211S1\nM500"));
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT
  #if HAS_HOTEND
<<<<<<< HEAD
    void SetPreheatEndTemp() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif
  #if HAS_HEATED_BED
    void SetPreheatBedTemp() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif
  #if HAS_FAN
    void SetPreheatFanSpeed() { SetPIntOnClick(0, 255); }
  #endif
#endif

void ApplyMaxSpeed() { planner.set_max_feedrate(HMI_value.axis, MenuData.Value / MINUNITMULT); }
#if HAS_X_AXIS
  void SetMaxSpeedX() { HMI_value.axis = X_AXIS, SetFloatOnClick(min_feedrate_edit_values.x, max_feedrate_edit_values.x, UNITFDIGITS, planner.settings.max_feedrate_mm_s[X_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_Y_AXIS
  void SetMaxSpeedY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(min_feedrate_edit_values.y, max_feedrate_edit_values.y, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Y_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_Z_AXIS
  void SetMaxSpeedZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(min_feedrate_edit_values.z, max_feedrate_edit_values.z, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Z_AXIS], ApplyMaxSpeed); }
#endif
#if HAS_HOTEND
  void SetMaxSpeedE() { HMI_value.axis = E_AXIS; SetFloatOnClick(min_feedrate_edit_values.e, max_feedrate_edit_values.e, UNITFDIGITS, planner.settings.max_feedrate_mm_s[E_AXIS], ApplyMaxSpeed); }
#endif

void ApplyMaxAccel() { planner.set_max_acceleration(HMI_value.axis, MenuData.Value); }
#if HAS_X_AXIS
  void SetMaxAccelX() { HMI_value.axis = X_AXIS, SetIntOnClick(min_acceleration_edit_values.x, max_acceleration_edit_values.x, planner.settings.max_acceleration_mm_per_s2[X_AXIS], ApplyMaxAccel); }
#endif
#if HAS_Y_AXIS
  void SetMaxAccelY() { HMI_value.axis = Y_AXIS, SetIntOnClick(min_acceleration_edit_values.y, max_acceleration_edit_values.y, planner.settings.max_acceleration_mm_per_s2[Y_AXIS], ApplyMaxAccel); }
#endif
#if HAS_Z_AXIS
  void SetMaxAccelZ() { HMI_value.axis = Z_AXIS, SetIntOnClick(min_acceleration_edit_values.z, max_acceleration_edit_values.z, planner.settings.max_acceleration_mm_per_s2[Z_AXIS], ApplyMaxAccel); }
#endif
#if HAS_HOTEND
  void SetMaxAccelE() { HMI_value.axis = E_AXIS; SetIntOnClick(min_acceleration_edit_values.e, max_acceleration_edit_values.e, planner.settings.max_acceleration_mm_per_s2[E_AXIS], ApplyMaxAccel); }
#endif

#if HAS_CLASSIC_JERK
  void ApplyMaxJerk() { planner.set_max_jerk(HMI_value.axis, MenuData.Value / MINUNITMULT); }
  #if HAS_X_AXIS
    void SetMaxJerkX() { HMI_value.axis = X_AXIS, SetFloatOnClick(min_jerk_edit_values.x, max_jerk_edit_values.x, UNITFDIGITS, planner.max_jerk.x, ApplyMaxJerk); }
  #endif
  #if HAS_Y_AXIS
    void SetMaxJerkY() { HMI_value.axis = Y_AXIS, SetFloatOnClick(min_jerk_edit_values.y, max_jerk_edit_values.y, UNITFDIGITS, planner.max_jerk.y, ApplyMaxJerk); }
  #endif
  #if HAS_Z_AXIS
    void SetMaxJerkZ() { HMI_value.axis = Z_AXIS, SetFloatOnClick(min_jerk_edit_values.z, max_jerk_edit_values.z, UNITFDIGITS, planner.max_jerk.z, ApplyMaxJerk); }
  #endif
  #if HAS_HOTEND
    void SetMaxJerkE() { HMI_value.axis = E_AXIS; SetFloatOnClick(min_jerk_edit_values.e, max_jerk_edit_values.e, UNITFDIGITS, planner.max_jerk.e, ApplyMaxJerk); }
  #endif
#elif HAS_JUNCTION_DEVIATION
  void ApplyJDmm() { TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk()); }
  void SetJDmm() { SetPFloatOnClick(MIN_JD_MM, MAX_JD_MM, 3, ApplyJDmm); }
#endif

#if ENABLED(LIN_ADVANCE)
  void SetLA_K() { SetPFloatOnClick(0, 10, 3); }
#endif

#if HAS_X_AXIS
  void SetStepsX() { HMI_value.axis = X_AXIS, SetPFloatOnClick( min_steps_edit_values.x, max_steps_edit_values.x, UNITFDIGITS); }
#endif
#if HAS_Y_AXIS
  void SetStepsY() { HMI_value.axis = Y_AXIS, SetPFloatOnClick( min_steps_edit_values.y, max_steps_edit_values.y, UNITFDIGITS); }
#endif
#if HAS_Z_AXIS
  void SetStepsZ() { HMI_value.axis = Z_AXIS, SetPFloatOnClick( min_steps_edit_values.z, max_steps_edit_values.z, UNITFDIGITS); }
#endif
#if HAS_HOTEND
  void SetStepsE() { HMI_value.axis = E_AXIS; SetPFloatOnClick( min_steps_edit_values.e, max_steps_edit_values.e, UNITFDIGITS); }
#endif

#if PROUI_EX
  void SetBedSizeX() { HMI_value.axis = NO_AXIS_ENUM, SetPIntOnClick(X_BED_MIN, X_MAX_POS, ProEx.ApplyPhySet); }
  void SetBedSizeY() { HMI_value.axis = NO_AXIS_ENUM, SetPIntOnClick(Y_BED_MIN, Y_MAX_POS, ProEx.ApplyPhySet); }
  void SetMinPosX()  { HMI_value.axis = X_AXIS,       SetPIntOnClick(     -100,       100, ProEx.ApplyPhySet); }
  void SetMinPosY()  { HMI_value.axis = Y_AXIS,       SetPIntOnClick(     -100,       100, ProEx.ApplyPhySet); }
  void SetMaxPosX()  { HMI_value.axis = X_AXIS,       SetPIntOnClick(X_BED_MIN,       999, ProEx.ApplyPhySet); }
  void SetMaxPosY()  { HMI_value.axis = Y_AXIS,       SetPIntOnClick(Y_BED_MIN,       999, ProEx.ApplyPhySet); }
  void SetMaxPosZ()  { HMI_value.axis = Z_AXIS,       SetPIntOnClick(      100,       999, ProEx.ApplyPhySet); }
#endif

#if BOTH(PROUI_EX, HAS_EXTRUDERS)
  void SetInvertE0() {
    stepper.disable_e_steppers();
    Toggle_Chkb_Line(PRO_data.Invert_E0);
=======
    void setPreheatEndTemp() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif
  #if HAS_HEATED_BED
    void setPreheatBedTemp() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif
  #if HAS_FAN
    void setPreheatFanSpeed() { setPIntOnClick(0, 255); }
  #endif
#endif

void applyMaxSpeed() { planner.set_max_feedrate(hmiValue.axis, menuData.value / MINUNITMULT); }
#if HAS_X_AXIS
  void setMaxSpeedX() { hmiValue.axis = X_AXIS, setFloatOnClick(min_feedrate_edit_values.x, max_feedrate_edit_values.x, UNITFDIGITS, planner.settings.max_feedrate_mm_s[X_AXIS], applyMaxSpeed); }
#endif
#if HAS_Y_AXIS
  void setMaxSpeedY() { hmiValue.axis = Y_AXIS, setFloatOnClick(min_feedrate_edit_values.y, max_feedrate_edit_values.y, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Y_AXIS], applyMaxSpeed); }
#endif
#if HAS_Z_AXIS
  void setMaxSpeedZ() { hmiValue.axis = Z_AXIS, setFloatOnClick(min_feedrate_edit_values.z, max_feedrate_edit_values.z, UNITFDIGITS, planner.settings.max_feedrate_mm_s[Z_AXIS], applyMaxSpeed); }
#endif
#if HAS_HOTEND
  void setMaxSpeedE() { hmiValue.axis = E_AXIS; setFloatOnClick(min_feedrate_edit_values.e, max_feedrate_edit_values.e, UNITFDIGITS, planner.settings.max_feedrate_mm_s[E_AXIS], applyMaxSpeed); }
#endif

void applyMaxAccel() { planner.set_max_acceleration(hmiValue.axis, menuData.value); }
#if HAS_X_AXIS
  void setMaxAccelX() { hmiValue.axis = X_AXIS, setIntOnClick(min_acceleration_edit_values.x, max_acceleration_edit_values.x, planner.settings.max_acceleration_mm_per_s2[X_AXIS], applyMaxAccel); }
#endif
#if HAS_Y_AXIS
  void setMaxAccelY() { hmiValue.axis = Y_AXIS, setIntOnClick(min_acceleration_edit_values.y, max_acceleration_edit_values.y, planner.settings.max_acceleration_mm_per_s2[Y_AXIS], applyMaxAccel); }
#endif
#if HAS_Z_AXIS
  void setMaxAccelZ() { hmiValue.axis = Z_AXIS, setIntOnClick(min_acceleration_edit_values.z, max_acceleration_edit_values.z, planner.settings.max_acceleration_mm_per_s2[Z_AXIS], applyMaxAccel); }
#endif
#if HAS_HOTEND
  void setMaxAccelE() { hmiValue.axis = E_AXIS; setIntOnClick(min_acceleration_edit_values.e, max_acceleration_edit_values.e, planner.settings.max_acceleration_mm_per_s2[E_AXIS], applyMaxAccel); }
#endif

#if HAS_CLASSIC_JERK
  void applyMaxJerk() { planner.set_max_jerk(hmiValue.axis, menuData.value / MINUNITMULT); }
  #if HAS_X_AXIS
    void setMaxJerkX() { hmiValue.axis = X_AXIS, setFloatOnClick(min_jerk_edit_values.x, max_jerk_edit_values.x, UNITFDIGITS, planner.max_jerk.x, applyMaxJerk); }
  #endif
  #if HAS_Y_AXIS
    void setMaxJerkY() { hmiValue.axis = Y_AXIS, setFloatOnClick(min_jerk_edit_values.y, max_jerk_edit_values.y, UNITFDIGITS, planner.max_jerk.y, applyMaxJerk); }
  #endif
  #if HAS_Z_AXIS
    void setMaxJerkZ() { hmiValue.axis = Z_AXIS, setFloatOnClick(min_jerk_edit_values.z, max_jerk_edit_values.z, UNITFDIGITS, planner.max_jerk.z, applyMaxJerk); }
  #endif
  #if HAS_HOTEND
    void setMaxJerkE() { hmiValue.axis = E_AXIS; setFloatOnClick(min_jerk_edit_values.e, max_jerk_edit_values.e, UNITFDIGITS, planner.max_jerk.e, applyMaxJerk); }
  #endif
#elif HAS_JUNCTION_DEVIATION
  void applyJDmm() { TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk()); }
  void setJDmm() { setPFloatOnClick(MIN_JD_MM, MAX_JD_MM, 3, applyJDmm); }
#endif

#if ENABLED(LIN_ADVANCE)
  void setLA_K() { setPFloatOnClick(0, 10, 3); }
#endif

#if HAS_X_AXIS
  void setStepsX() { hmiValue.axis = X_AXIS, setPFloatOnClick( min_steps_edit_values.x, max_steps_edit_values.x, UNITFDIGITS); }
#endif
#if HAS_Y_AXIS
  void setStepsY() { hmiValue.axis = Y_AXIS, setPFloatOnClick( min_steps_edit_values.y, max_steps_edit_values.y, UNITFDIGITS); }
#endif
#if HAS_Z_AXIS
  void setStepsZ() { hmiValue.axis = Z_AXIS, setPFloatOnClick( min_steps_edit_values.z, max_steps_edit_values.z, UNITFDIGITS); }
#endif
#if HAS_HOTEND
  void setStepsE() { hmiValue.axis = E_AXIS; setPFloatOnClick( min_steps_edit_values.e, max_steps_edit_values.e, UNITFDIGITS); }
#endif

#if PROUI_EX
  void setBedSizeX() { hmiValue.axis = NO_AXIS_ENUM, setPIntOnClick(X_BED_MIN, X_MAX_POS, proUIEx.applyPhySet); }
  void setBedSizeY() { hmiValue.axis = NO_AXIS_ENUM, setPIntOnClick(Y_BED_MIN, Y_MAX_POS, proUIEx.applyPhySet); }
  void setMinPosX()  { hmiValue.axis = X_AXIS,       setPIntOnClick(     -100,       100, proUIEx.applyPhySet); }
  void setMinPosY()  { hmiValue.axis = Y_AXIS,       setPIntOnClick(     -100,       100, proUIEx.applyPhySet); }
  void setMaxPosX()  { hmiValue.axis = X_AXIS,       setPIntOnClick(X_BED_MIN,       999, proUIEx.applyPhySet); }
  void setMaxPosY()  { hmiValue.axis = Y_AXIS,       setPIntOnClick(Y_BED_MIN,       999, proUIEx.applyPhySet); }
  void setMaxPosZ()  { hmiValue.axis = Z_AXIS,       setPIntOnClick(      100,       999, proUIEx.applyPhySet); }
#endif

#if ALL(PROUI_EX, HAS_EXTRUDERS)
  void setInvertE0() {
    stepper.disable_e_steppers();
    toggleCheckboxLine(PRO_data.Invert_E0);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    current_position.e = 0;
    sync_plan_position_e();
  }
#endif

#if ENABLED(FWRETRACT)
<<<<<<< HEAD
  void SetRetractLength() { SetPFloatOnClick( 0, 10, UNITFDIGITS); }
  void SetRetractSpeed() { SetPFloatOnClick( 1, 90, UNITFDIGITS); }
  void SetZRaise() { SetPFloatOnClick( 0, 2, 2); }
  void SetRecoverSpeed() { SetPFloatOnClick( 1, 90, UNITFDIGITS); }
  void SetAddRecover() { SetPFloatOnClick(-5, 5, UNITFDIGITS); }
#endif

#if HAS_TOOLBAR
  void LiveTBSetupItem() {
    UpdateTBSetupItem(static_cast<MenuItemClass*>(CurrentMenu->SelectedItem()), MenuData.Value);
    DrawTBSetupItem(true);
  }
  void ApplyTBSetupItem() {
    DrawTBSetupItem(false);
    if (static_cast<MenuItemClass*>(CurrentMenu->SelectedItem())->icon) {
      uint8_t *Pint = (uint8_t *)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
      *Pint = MenuData.Value;
    }
  }
  void SetTBSetupItem() {
    const uint8_t val = *(uint8_t *)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    SetOnClick(SetIntNoDraw, 0, ToolBar.OptCount() - 1, 0, val, ApplyTBSetupItem, LiveTBSetupItem);
    DrawTBSetupItem(true);
  }
  void onDrawTBSetupItem(MenuItemClass* menuitem, int8_t line) {
    uint8_t val = *(uint8_t *)static_cast<MenuItemPtrClass*>(menuitem)->value;
    UpdateTBSetupItem(menuitem, val);
=======
  void doRetract() { 
    current_position.e-=fwretract.settings.retract_length;
    axisMove(E_AXIS);
  }
  void setRetractLength() { setPFloatOnClick( 0, 10, UNITFDIGITS); }
  void setRetractSpeed() { setPFloatOnClick( 1, 90, UNITFDIGITS); }
  void setZRaise() { setPFloatOnClick( 0, 2, 2); }
  void setRecoverSpeed() { setPFloatOnClick( 1, 90, UNITFDIGITS); }
  void setAddRecover() { setPFloatOnClick(-5, 5, UNITFDIGITS); }
#endif

#if HAS_TOOLBAR
  void liveTBSetupItem() {
    updateTBSetupItem(static_cast<MenuItem*>(currentMenu->selectedItem()), menuData.value);
    drawTBSetupItem(true);
  }
  void applyTBSetupItem() {
    drawTBSetupItem(false);
    if (static_cast<MenuItem*>(currentMenu->selectedItem())->icon) {
      uint8_t *Pint = (uint8_t *)static_cast<MenuItemPtr*>(currentMenu->selectedItem())->value;
      *Pint = menuData.value;
    }
  }
  void setTBSetupItem() {
    const uint8_t val = *(uint8_t *)static_cast<MenuItemPtr*>(currentMenu->selectedItem())->value;
    setOnClick(ID_SetIntNoDraw, 0, toolBar.OptCount() - 1, 0, val, applyTBSetupItem, liveTBSetupItem);
    drawTBSetupItem(true);
  }
  void onDrawTBSetupItem(MenuItem* menuitem, int8_t line) {
    uint8_t val = *(uint8_t *)static_cast<MenuItemPtr*>(menuitem)->value;
    updateTBSetupItem(menuitem, val);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    onDrawMenuItem(menuitem, line);
  }
#endif // HAS_TOOLBAR

// Special Menuitem Drawing functions =================================================

<<<<<<< HEAD
void onDrawSelColorItem(MenuItemClass* menuitem, int8_t line) {
  const uint16_t color = *(uint16_t*)static_cast<MenuItemPtrClass*>(menuitem)->value;
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  DWIN_Draw_Rectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  onDrawMenuItem(menuitem, line);
}

void onDrawGetColorItem(MenuItemClass* menuitem, int8_t line) {
=======
void onDrawSelColorItem(MenuItem* menuitem, int8_t line) {
  const uint16_t color = *(uint16_t*)static_cast<MenuItemPtr*>(menuitem)->value;
  dwinDrawRectangle(0, hmiData.colorHighlight, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  onDrawMenuItem(menuitem, line);
}

void onDrawGetColorItem(MenuItem* menuitem, int8_t line) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  const uint8_t i = menuitem->icon;
  uint16_t color;
  switch (i) {
    case 0: color = RGB(31, 0, 0); break; // Red
    case 1: color = RGB(0, 63, 0); break; // Green
    case 2: color = RGB(0, 0, 31); break; // Blue
    default: color = 0; break;
  }
<<<<<<< HEAD
  DWIN_Draw_Rectangle(0, HMI_data.Highlight_Color, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  DWIN_Draw_Rectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  DWINUI::Draw_String(LBLX, MBASE(line) - 1, menuitem->caption);
  Draw_Menu_IntValue(HMI_data.Background_Color, line, 4, HMI_value.Color[i]);
  DWIN_Draw_HLine(HMI_data.SplitLine_Color, 16, MYPOS(line + 1), 240);
}

#if BOTH(HAS_FILAMENT_SENSOR, PROUI_EX)
  void onDrawRunoutActive(MenuItemClass* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    if (PRO_data.FilamentMotionSensor)
      DWINUI::Draw_String(VALX - 8, MBASE(line), GET_TEXT_F(MSG_MOTION));
    else
      DWINUI::Draw_String(VALX + 8, MBASE(line), PRO_data.Runout_active_state ? GET_TEXT_F(MSG_HIGH) : GET_TEXT_F(MSG_LOW));
  }
#endif

#if BOTH(HAS_MESH, PROUI_EX)
  void onDrawMeshPoints(MenuItemClass* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    ProEx.DrawMeshPoints(false, line, PRO_data.grid_max_points);
=======
  dwinDrawRectangle(0, hmiData.colorHighlight, ICOX + 1, MBASE(line) - 1 + 1, ICOX + 18, MBASE(line) - 1 + 18);
  dwinDrawRectangle(1, color, ICOX + 2, MBASE(line) - 1 + 2, ICOX + 17, MBASE(line) - 1 + 17);
  DWINUI::drawString(LBLX, MBASE(line) - 1, menuitem->caption);
  drawMenuIntValue(hmiData.colorBackground, line, 4, hmiValue.Color[i]);
  dwinDrawHLine(hmiData.colorSplitLine, 16, MYPOS(line + 1), 240);
}

#if HAS_PROUI_RUNOUT_SENSOR
  void ondrawRunoutActive(MenuItem* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    if (PRO_data.FilamentMotionSensor)
      DWINUI::drawString(VALX - 8, MBASE(line), GET_TEXT_F(MSG_MOTION));
    else
      DWINUI::drawString(VALX + 8, MBASE(line), PRO_data.Runout_active_state ? GET_TEXT_F(MSG_HIGH) : GET_TEXT_F(MSG_LOW));
  }
#endif

#if ALL(HAS_MESH, PROUI_EX)
  void onDrawMeshPoints(MenuItem* menuitem, int8_t line) {
    onDrawMenuItem(menuitem, line);
    proUIEx.drawMeshPoints(false, line, PRO_data.grid_max_points);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

// Menu Creation and Drawing functions ======================================================

<<<<<<< HEAD
void ReturnToPreviousMenu() {
  #if ENABLED(CV_LASER_MODULE)
    if (PreviousMenu == LaserPrintMenu) return Draw_LaserPrint_Menu();
  #endif
  if (PreviousMenu == AdvancedSettings) return Draw_AdvancedSettings_Menu();
  if (PreviousMenu == FilSetMenu) return Draw_FilSet_Menu();
  if (PreviousMenu == TuneMenu) return Draw_Tune_Menu();
  if (PreviousMenu == FileMenu) return Draw_Print_File_Menu();
}

void Draw_Prepare_Menu() {
  checkkey = Menu;
  if (SET_MENU(PrepareMenu, MSG_PREPARE, 9 + PREHEAT_COUNT)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENT_MAN, onDrawSubMenu, Draw_FilamentMan_Menu);
    #endif
    MENU_ITEM(ICON_Axis, MSG_MOVE_AXIS, onDrawSubMenu, Draw_Move_Menu);
    #if ENABLED(LCD_BED_TRAMMING)
      MENU_ITEM(ICON_Tram, MSG_BED_TRAMMING, onDrawSubMenu, Draw_Tramming_Menu);
    #endif
    MENU_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, onDrawMenuItem, DisableMotors);
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
      MENU_ITEM(ICON_Homing, MSG_HOMING, onDrawSubMenu, Draw_Homing_Menu);
    #else
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      MENU_ITEM(ICON_ManualMesh, MSG_MANUAL_MESH, onDrawSubMenu, Draw_ManualMesh_Menu);
    #elif HAS_BED_PROBE
      MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
    #endif
    #if HAS_ZOFFSET_ITEM
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, onDrawSubMenu, Draw_ZOffsetWiz_Menu);
      #elif ENABLED(BABYSTEPPING)
        EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
=======
void returnToPreviousMenu() {
  #if ENABLED(CV_LASER_MODULE)
    if (previousMenu == laserPrintMenu) return drawLaserPrintMenu();
  #endif
  if (previousMenu == advancedSettings) return drawAdvancedSettingsMenu();
  if (previousMenu == filSetMenu) return drawFilSetMenu();
  if (previousMenu == tuneMenu) return drawTuneMenu();
  if (previousMenu == fileMenu) return drawPrintFileMenu();
}

void drawPrepareMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(prepareMenu, MSG_PREPARE, 9 + PREHEAT_COUNT)) {
    BACK_ITEM(gotoMainMenu);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENT_MAN, onDrawSubMenu, drawFilamentManMenu);
    #endif
    MENU_ITEM(ICON_Axis, MSG_MOVE_AXIS, onDrawSubMenu, drawMoveMenu);
    #if ALL(PROUI_EX, LCD_BED_TRAMMING)
      MENU_ITEM(ICON_Tram, MSG_BED_TRAMMING, onDrawSubMenu, drawTrammingMenu);
    #endif
    MENU_ITEM(ICON_CloseMotor, MSG_DISABLE_STEPPERS, onDrawMenuItem, disableMotors);
    #if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
      MENU_ITEM(ICON_Homing, MSG_HOMING, onDrawSubMenu, drawHomingMenu);
    #else
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
    #endif
    #if ENABLED(MESH_BED_LEVELING)
      MENU_ITEM(ICON_ManualMesh, MSG_MANUAL_MESH, onDrawSubMenu, drawManualMeshMenu);
    #elif HAS_BED_PROBE
      MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, autoLev);
    #endif
    #if HAS_ZOFFSET_ITEM
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_SetZOffset, MSG_PROBE_WIZARD, onDrawSubMenu, drawZOffsetWizMenu);
      #elif ENABLED(BABYSTEPPING)
        EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      #endif
    #endif
    #if HAS_PREHEAT
      #define _ITEM_PREHEAT(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheat##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT)
    #endif
<<<<<<< HEAD
    MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, DoCoolDown);
  }
  ui.reset_status(true);
  UpdateMenu(PrepareMenu);
}

#if ENABLED(LCD_BED_TRAMMING)
  void Draw_Tramming_Menu() {
    checkkey = Menu;
    if (SET_MENU(TrammingMenu, MSG_BED_TRAMMING, 8)) {
      BACK_ITEM(Draw_Prepare_Menu);
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_ProbeSet, MSG_TRAMMING_WIZARD, onDrawMenuItem, Trammingwizard);
        EDIT_ITEM(ICON_ProbeSet, MSG_BED_TRAMMING_MANUAL, onDrawChkbMenu, SetManualTramming, &HMI_data.FullManualTramming);
      #else
        MENU_ITEM_F(ICON_MoveZ0, "Home Z and disable", onDrawMenuItem, HomeZandDisable);
      #endif
      MENU_ITEM(ICON_Axis, MSG_LEVBED_FL, onDrawMenuItem, TramFL);
      MENU_ITEM(ICON_Axis, MSG_LEVBED_FR, onDrawMenuItem, TramFR);
      MENU_ITEM(ICON_Axis, MSG_LEVBED_BR, onDrawMenuItem, TramBR);
      MENU_ITEM(ICON_Axis, MSG_LEVBED_BL, onDrawMenuItem, TramBL);
      MENU_ITEM(ICON_Axis, MSG_LEVBED_C , onDrawMenuItem, TramC );
    }
    UpdateMenu(TrammingMenu);
  }
#endif

void Draw_Control_Menu() {
  checkkey = Menu;
  if (SET_MENU(ControlMenu, MSG_CONTROL, 10)) {
    BACK_ITEM(Goto_Main_Menu);
    MENU_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawSubMenu, Draw_Temperature_Menu);
    MENU_ITEM(ICON_Motion, MSG_MOTION, onDrawSubMenu, Draw_Motion_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawMenuItem, ReadEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, ResetEeprom);
    #endif
    MENU_ITEM(ICON_Reboot, MSG_RESET_PRINTER, onDrawMenuItem, RebootPrinter);
    #if ENABLED(CASE_LIGHT_MENU)
      #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
        MENU_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawSubMenu, Draw_CaseLight_Menu);
      #else
        EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
      #endif
    #endif
    #if ENABLED(LED_CONTROL_MENU)
      MENU_ITEM(ICON_LedControl, MSG_LED_CONTROL, onDrawSubMenu, Draw_LedControl_Menu);
    #endif
    MENU_ITEM(ICON_Info, MSG_INFO_SCREEN, onDrawSubMenu, Goto_Info_Menu);
  }
  ui.reset_status(true);
  UpdateMenu(ControlMenu);
}

void Draw_AdvancedSettings_Menu() {
  checkkey = Menu;
  if (SET_MENU(AdvancedSettings, MSG_ADVANCED_SETTINGS, 26)) {
    BACK_ITEM(Goto_Main_Menu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
    #endif
    #if ENABLED(CV_LASER_MODULE)
      MENU_ITEM(ICON_LaserSet, MSG_LASER_MENU, onDrawSubMenu, Draw_LaserSettings_Menu);
    #endif
    #if HAS_MESH
      MENU_ITEM(ICON_ProbeSet, MSG_MESH_LEVELING, onDrawSubMenu, Draw_MeshSet_Menu);
    #endif
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, Draw_ProbeSet_Menu);
    #endif
    MENU_ITEM(ICON_FilSet, MSG_FILAMENT_SET, onDrawSubMenu, Draw_FilSet_Menu);
    #if ENABLED(PIDTEMP) && EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID " Settings", onDrawSubMenu, Draw_HotendPID_Menu);
    #endif
    #if EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
      MENU_ITEM_F(ICON_MPCNozzle, "MPC Settings", onDrawSubMenu, Draw_HotendMPC_Menu);
    #endif
    #if ENABLED(PIDTEMPBED) && EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDBed, STR_BED_PID " Settings", onDrawSubMenu, Draw_BedPID_Menu);
    #endif
    #if PROUI_EX
      MENU_ITEM(ICON_PhySet, MSG_PHY_SET, onDrawSubMenu, Draw_PhySet_Menu);
    #endif
    #if HAS_TRINAMIC_CONFIG
      MENU_ITEM(ICON_TMCSet, MSG_TMC_DRIVERS, onDrawSubMenu, Draw_TrinamicConfig_menu);
    #endif
    #if HAS_TOOLBAR
      MENU_ITEM(ICON_TBSetup, MSG_TOOLBAR_SETUP, onDrawSubMenu, Draw_TBSetup_Menu);
    #endif
    #if HAS_ESDIAG
      MENU_ITEM_F(ICON_ESDiag, "End-stops diag.", onDrawSubMenu, Draw_EndStopDiag);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, Goto_PrintStats);
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, PrintStatsReset);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
      MENU_ITEM(ICON_Host, MSG_HOST_SHUTDOWN, onDrawMenuItem, HostShutDown);
    #endif
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_SOUND_ENABLE, onDrawChkbMenu, SetEnableSound, &ui.sound_on);
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
    #if HAS_GCODE_PREVIEW
      EDIT_ITEM(ICON_File, MSG_HAS_PREVIEW, onDrawChkbMenu, SetPreview, &HMI_data.EnablePreview);
    #endif
    #if ENABLED(MEDIASORT_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_MEDIA_SORT, onDrawChkbMenu, SetMediaSort, &HMI_data.MediaSort);
    #endif
    EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, SetMediaAutoMount, &HMI_data.MediaAutoMount);
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM_F(ICON_SetBaudRate, "115K baud", onDrawChkbMenu, SetBaudRate, &HMI_data.Baud115K);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if HAS_CUSTOM_COLORS
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, Draw_SelectColors_Menu);
    #endif
  }
  ui.reset_status(true);
  UpdateMenu(AdvancedSettings);
}

void Draw_Move_Menu() {
  checkkey = Menu;
  if (SET_MENU(MoveMenu, MSG_MOVE_AXIS, 7)) {
    BACK_ITEM(Draw_Prepare_Menu);
    EDIT_ITEM(ICON_Axis, MSG_LIVE_MOVE, onDrawChkbMenu, SetLiveMove, &EnableLiveMove);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, SetMoveX, &current_position.x);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, SetMoveY, &current_position.y);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MoveZ, MSG_MOVE_Z, onDrawPFloatMenu, SetMoveZ, &current_position.z);
    #endif
    #if HAS_HOTEND
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      EDIT_ITEM(ICON_Extruder, MSG_MOVE_E, onDrawPFloatMenu, SetMoveE, &current_position.e);
      MENU_ITEM_F(ICON_Extruder, F("Move Ext. 100mm"), onDrawMenuItem, MoveE100);
    #endif
  }
  UpdateMenu(MoveMenu);
=======
    MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, doCoolDown);
  }
  ui.reset_status(true);
  updateMenu(prepareMenu);
}

#if ALL(PROUI_EX, LCD_BED_TRAMMING)

  void setManualTramming() {
    toggleCheckboxLine(hmiData.fullManualTramming);
  }

  void drawTrammingMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(trammingMenu, MSG_BED_TRAMMING, 8)) {
      BACK_ITEM(drawPrepareMenu);
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_ProbeSet, MSG_TRAMMING_WIZARD, onDrawMenuItem, trammingwizard);
        EDIT_ITEM(ICON_ProbeSet, MSG_BED_TRAMMING_MANUAL, onDrawChkbMenu, setManualTramming, &hmiData.fullManualTramming);
      #else
        MENU_ITEM_F(ICON_MoveZ0, "Home Z and disable", onDrawMenuItem, homeZandDisable);
      #endif
      MENU_ITEM(ICON_Axis, MSG_LEVBED_FL, onDrawMenuItem, []{ tram(LF); });
      MENU_ITEM(ICON_Axis, MSG_LEVBED_FR, onDrawMenuItem, []{ tram(RF); });
      MENU_ITEM(ICON_Axis, MSG_LEVBED_BR, onDrawMenuItem, []{ tram(RB); });
      MENU_ITEM(ICON_Axis, MSG_LEVBED_BL, onDrawMenuItem, []{ tram(LB); });
      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        MENU_ITEM(ICON_Axis, MSG_LEVBED_C , onDrawMenuItem, []{ tram(TRAM_C); } );
      #endif
    }
    updateMenu(trammingMenu);
  }

#endif

void drawControlMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(controlMenu, MSG_CONTROL, 10)) {
    BACK_ITEM(gotoMainMenu);
    MENU_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawSubMenu, drawTemperatureMenu);
    MENU_ITEM(ICON_Motion, MSG_MOTION, onDrawSubMenu, drawMotionMenu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      MENU_ITEM(ICON_ReadEEPROM, MSG_LOAD_EEPROM, onDrawMenuItem, readEeprom);
      MENU_ITEM(ICON_ResumeEEPROM, MSG_RESTORE_DEFAULTS, onDrawMenuItem, resetEeprom);
    #endif
    MENU_ITEM(ICON_Reboot, MSG_RESET_PRINTER, onDrawMenuItem, rebootPrinter);
    #if ENABLED(CASE_LIGHT_MENU)
      #if ENABLED(CASELIGHT_USES_BRIGHTNESS)
        MENU_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawSubMenu, drawCaseLightMenu);
      #else
        EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, setCaseLight, &caselight.on);
      #endif
    #endif
    #if ENABLED(LED_CONTROL_MENU)
      MENU_ITEM(ICON_LedControl, MSG_LED_CONTROL, onDrawSubMenu, drawLedControlMenu );
    #endif
    MENU_ITEM(ICON_Info, MSG_INFO_SCREEN, onDrawSubMenu, gotoInfoMenu);
  }
  ui.reset_status(true);
  updateMenu(controlMenu);
}

void drawAdvancedSettingsMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(advancedSettings, MSG_ADVANCED_SETTINGS, 26)) {
    BACK_ITEM(gotoMainMenu);
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
    #endif
    #if ENABLED(CV_LASER_MODULE)
      MENU_ITEM(ICON_LaserSet, MSG_LASER_MENU, onDrawSubMenu, drawLaserSettingsMenu);
    #endif
    #if HAS_MESH
      MENU_ITEM(ICON_ProbeSet, MSG_MESH_LEVELING, onDrawSubMenu, drawMeshSetMenu);
    #endif
    #if HAS_BED_PROBE
      MENU_ITEM(ICON_ProbeSet, MSG_ZPROBE_SETTINGS, onDrawSubMenu, drawProbeSetMenu);
    #endif
    MENU_ITEM(ICON_FilSet, MSG_FILAMENT_SET, onDrawSubMenu, drawFilSetMenu);
    #if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID " Settings", onDrawSubMenu, drawHotendPIDMenu);
    #endif
    #if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
      MENU_ITEM_F(ICON_MPCNozzle, "MPC Settings", onDrawSubMenu, drawHotendMPCMenu);
    #endif
    #if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
      MENU_ITEM_F(ICON_PIDBed, STR_BED_PID " Settings", onDrawSubMenu, drawBedPIDMenu);
    #endif
    #if PROUI_EX
      MENU_ITEM(ICON_PhySet, MSG_PHY_SET, onDrawSubMenu, drawPhySetMenu);
    #endif
    #if HAS_TRINAMIC_CONFIG
      MENU_ITEM(ICON_TMCSet, MSG_TMC_DRIVERS, onDrawSubMenu, drawTrinamicConfigMenu);
    #endif
    #if HAS_TOOLBAR
      MENU_ITEM(ICON_TBSetup, MSG_TOOLBAR_SETUP, onDrawSubMenu, drawTBSetupMenu);
    #endif
    #if HAS_ESDIAG
      MENU_ITEM_F(ICON_ESDiag, "End-stops diag.", onDrawSubMenu, drawEndStopDiag);
    #endif
    #if ENABLED(PRINTCOUNTER)
      MENU_ITEM(ICON_PrintStats, MSG_INFO_STATS_MENU, onDrawSubMenu, gotoPrintStats);
      MENU_ITEM(ICON_PrintStatsReset, MSG_INFO_PRINT_COUNT_RESET, onDrawSubMenu, printStatsReset);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, dwinLockScreen);
    #endif
    #if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
      MENU_ITEM(ICON_Host, MSG_HOST_SHUTDOWN, onDrawMenuItem, hostShutDown);
    #endif
    #if ENABLED(SOUND_MENU_ITEM)
      EDIT_ITEM(ICON_Sound, MSG_SOUND_ENABLE, onDrawChkbMenu, setEnableSound, &ui.sound_on);
    #endif
    #if ENABLED(POWER_LOSS_RECOVERY)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, setPwrLossr, &recovery.enabled);
    #endif
    #if ALL(HAS_GCODE_PREVIEW, PREVIEW_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_HAS_PREVIEW, onDrawChkbMenu, setPreview, &hmiData.enablePreview);
    #endif
    #if ENABLED(MEDIASORT_MENU_ITEM)
      EDIT_ITEM(ICON_File, MSG_MEDIA_SORT, onDrawChkbMenu, setMediaSort, &hmiData.mediaSort);
    #endif
    #if DISABLED(HAS_SD_EXTENDER)
      EDIT_ITEM(ICON_File, MSG_MEDIA_UPDATE, onDrawChkbMenu, setMediaAutoMount, &hmiData.mediaAutoMount);
    #endif
    #if ENABLED(BAUD_RATE_GCODE)
      EDIT_ITEM_F(ICON_SetBaudRate, "115K baud", onDrawChkbMenu, setBaudRate, &hmiData.baud115K);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, setBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, turnOffBacklight);
    #endif
    #if HAS_CUSTOM_COLORS
      MENU_ITEM(ICON_Scolor, MSG_COLORS_SELECT, onDrawSubMenu, drawSelectColorsMenu);
    #endif
  }
  ui.reset_status(true);
  updateMenu(advancedSettings);
}

void drawMoveMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(moveMenu, MSG_MOVE_AXIS, 7)) {
    BACK_ITEM(drawPrepareMenu);
    EDIT_ITEM(ICON_Axis, MSG_LIVE_MOVE, onDrawChkbMenu, setLiveMove, &enableLiveMove);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, setMoveX, &current_position.x);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, setMoveY, &current_position.y);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MoveZ, MSG_MOVE_Z, onDrawPFloatMenu, setMoveZ, &current_position.z);
    #endif
    #if HAS_HOTEND
      gcode.process_subcommands_now(F("G92E0"));  // reset extruder position
      EDIT_ITEM(ICON_Extruder, MSG_MOVE_E, onDrawPFloatMenu, setMoveE, &current_position.e);
      MENU_ITEM_F(ICON_Extruder, F("Move Ext. 100mm"), onDrawMenuItem, moveE100);
    #endif
  }
  updateMenu(moveMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  if (!all_axes_trusted()) LCD_MESSAGE_F("WARNING: current position is unknown, home axes");
}

#if HAS_HOME_OFFSET

<<<<<<< HEAD
  void Draw_HomeOffset_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomeOffMenu, MSG_SET_HOME_OFFSETS, 4)) {
      BACK_ITEM(Draw_PhySet_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_HomeOffsetX, MSG_HOME_OFFSET_X, onDrawPFloatMenu, SetHomeOffsetX, &home_offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_HomeOffsetY, MSG_HOME_OFFSET_Y, onDrawPFloatMenu, SetHomeOffsetY, &home_offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_HomeOffsetZ, MSG_HOME_OFFSET_Z, onDrawPFloatMenu, SetHomeOffsetZ, &home_offset.z);
      #endif
    }
    UpdateMenu(HomeOffMenu);
  }

#endif

#if HAS_BED_PROBE

  void Draw_ProbeSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(ProbeSetMenu, MSG_ZPROBE_SETTINGS, 11)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_ProbeOffsetX, MSG_ZPROBE_XOFFSET, onDrawPFloatMenu, SetProbeOffsetX, &probe.offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_ProbeOffsetY, MSG_ZPROBE_YOFFSET, onDrawPFloatMenu, SetProbeOffsetY, &probe.offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_ProbeOffsetZ, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetProbeOffsetZ, &probe.offset.z);
      #endif
      #if PROUI_EX
        EDIT_ITEM(ICON_ProbeZSpeed, MSG_Z_FEED_RATE, onDrawPIntMenu, SetProbeZSpeed, &PRO_data.zprobefeedslow);
        EDIT_ITEM(ICON_ProbeMultiple, MSG_ZPROBE_MULTIPLE, onDrawPInt8Menu, SetProbeMultiple, &PRO_data.multiple_probing);
      #endif
      #if ENABLED(BLTOUCH)
        MENU_ITEM(ICON_ProbeStow, MSG_MANUAL_STOW, onDrawMenuItem, ProbeStow);
        MENU_ITEM(ICON_ProbeDeploy, MSG_MANUAL_DEPLOY, onDrawMenuItem, ProbeDeploy);
        MENU_ITEM(ICON_BltouchReset, MSG_BLTOUCH_RESET, onDrawMenuItem, bltouch._reset);
        #if HAS_BLTOUCH_HS_MODE
          EDIT_ITEM(ICON_HSMode, MSG_ENABLE_HS_MODE, onDrawChkbMenu, SetHSMode, &bltouch.high_speed_mode);
        #endif
      #endif
      MENU_ITEM(ICON_ProbeTest, MSG_M48_TEST, onDrawMenuItem, ProbeTest);
    }
    UpdateMenu(ProbeSetMenu);
  }

#endif

void Draw_FilSet_Menu() {
  checkkey = Menu;
  if (SET_MENU(FilSetMenu, MSG_FILAMENT_SET, 9)) {
    BACK_ITEM(Draw_AdvancedSettings_Menu);
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
      #if PROUI_EX
        MENU_ITEM(ICON_Runout, MSG_RUNOUT_ACTIVE, onDrawRunoutActive, SetRunoutActive);
      #endif
    #endif
    #if HAS_FILAMENT_RUNOUT_DISTANCE
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_DISTANCE_MM, onDrawPFloatMenu, SetRunoutDistance, &runout.runout_distance());
    #endif
    #if BOTH(PROUI_EX, HAS_EXTRUDERS)
      EDIT_ITEM(ICON_InvertE0, MSG_INVERT_EXTRUDER, onDrawChkbMenu, SetInvertE0, &PRO_data.Invert_E0);
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      EDIT_ITEM(ICON_ExtrudeMinT, MSG_EXTRUDER_MIN_TEMP, onDrawPIntMenu, SetExtMinT, &HMI_data.ExtMinT);
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      EDIT_ITEM(ICON_FilLoad, MSG_FILAMENT_LOAD, onDrawPFloatMenu, SetFilLoad, &fc_settings[0].load_length);
      EDIT_ITEM(ICON_FilUnload, MSG_FILAMENT_UNLOAD, onDrawPFloatMenu, SetFilUnload, &fc_settings[0].unload_length);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
    #endif
  }
  UpdateMenu(FilSetMenu);
=======
  void drawHomeOffsetMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(homeOffMenu, MSG_SET_HOME_OFFSETS, 4)) {
      BACK_ITEM(drawPhySetMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_HomeOffsetX, MSG_HOME_OFFSET_X, onDrawPFloatMenu, setHomeOffsetX, &home_offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_HomeOffsetY, MSG_HOME_OFFSET_Y, onDrawPFloatMenu, setHomeOffsetY, &home_offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_HomeOffsetZ, MSG_HOME_OFFSET_Z, onDrawPFloatMenu, setHomeOffsetZ, &home_offset.z);
      #endif
    }
    updateMenu(homeOffMenu);
  }

#endif // HAS_HOME_OFFSET

#if HAS_BED_PROBE

  void drawProbeSetMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(probeSetMenu, MSG_ZPROBE_SETTINGS, 11)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_ProbeOffsetX, MSG_ZPROBE_XOFFSET, onDrawPFloatMenu, setProbeOffsetX, &probe.offset.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_ProbeOffsetY, MSG_ZPROBE_YOFFSET, onDrawPFloatMenu, setProbeOffsetY, &probe.offset.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_ProbeOffsetZ, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setProbeOffsetZ, &probe.offset.z);
      #endif
      #if PROUI_EX
        EDIT_ITEM(ICON_ProbeZSpeed, MSG_Z_FEED_RATE, onDrawPIntMenu, setProbeZSpeed, &PRO_data.zprobefeedslow);
        EDIT_ITEM(ICON_ProbeMultiple, MSG_ZPROBE_MULTIPLE, onDrawPInt8Menu, setProbeMultiple, &PRO_data.multiple_probing);
      #endif
      #if ENABLED(BLTOUCH)
        MENU_ITEM(ICON_ProbeStow, MSG_MANUAL_STOW, onDrawMenuItem, probeStow);
        MENU_ITEM(ICON_ProbeDeploy, MSG_MANUAL_DEPLOY, onDrawMenuItem, probeDeploy);
        MENU_ITEM(ICON_BltouchReset, MSG_BLTOUCH_RESET, onDrawMenuItem, bltouch._reset);
        #if HAS_BLTOUCH_HS_MODE
          EDIT_ITEM(ICON_HSMode, MSG_ENABLE_HS_MODE, onDrawChkbMenu, setHSMode, &bltouch.high_speed_mode);
        #endif
      #endif
      MENU_ITEM(ICON_ProbeTest, MSG_M48_TEST, onDrawMenuItem, probeTest);
    }
    updateMenu(probeSetMenu);
  }

#endif // HAS_BED_PROBE

void drawFilSetMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(filSetMenu, MSG_FILAMENT_SET, 9)) {
    BACK_ITEM(drawAdvancedSettingsMenu);
    #if HAS_FILAMENT_SENSOR
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, setRunoutEnable, &runout.enabled);
      #if HAS_PROUI_RUNOUT_SENSOR
        MENU_ITEM(ICON_Runout, MSG_RUNOUT_ACTIVE, ondrawRunoutActive, setRunoutActive);
      #endif
      #if HAS_FILAMENT_RUNOUT_DISTANCE
        EDIT_ITEM(ICON_Runout, MSG_RUNOUT_DISTANCE_MM, onDrawPFloatMenu, setRunoutDistance, &runout.runout_distance());
      #endif
    #endif
    #if ALL(PROUI_EX, HAS_EXTRUDERS)
      EDIT_ITEM(ICON_InvertE0, MSG_INVERT_EXTRUDER, onDrawChkbMenu, setInvertE0, &PRO_data.Invert_E0);
    #endif
    #if ENABLED(PREVENT_COLD_EXTRUSION)
      EDIT_ITEM(ICON_ExtrudeMinT, MSG_EXTRUDER_MIN_TEMP, onDrawPIntMenu, setExtMinT, &hmiData.extMinT);
    #endif
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      EDIT_ITEM(ICON_FilLoad, MSG_FILAMENT_LOAD, onDrawPFloatMenu, setFilLoad, &fc_settings[EXT].load_length);
      EDIT_ITEM(ICON_FilUnload, MSG_FILAMENT_UNLOAD, onDrawPFloatMenu, setFilUnload, &fc_settings[EXT].unload_length);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, drawFWRetractMenu);
    #endif
  }
  updateMenu(filSetMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

#if PROUI_EX
  #if ENABLED(NOZZLE_PARK_FEATURE)
<<<<<<< HEAD
    void Draw_ParkPos_Menu() {
      checkkey = Menu;
      if (SET_MENU(ParkPosMenu, MSG_FILAMENT_PARK_ENABLED, 4)) {
        BACK_ITEM(Draw_PhySet_Menu);
        EDIT_ITEM(ICON_ParkPosX, MSG_PARK_XPOSITION, onDrawPIntMenu, SetParkPosX, &PRO_data.Park_point.x);
        EDIT_ITEM(ICON_ParkPosY, MSG_PARK_YPOSITION, onDrawPIntMenu, SetParkPosY, &PRO_data.Park_point.y);
        EDIT_ITEM(ICON_ParkPosZ, MSG_PARK_ZRAISE, onDrawPIntMenu, SetParkZRaise, &PRO_data.Park_point.z);
      }
      UpdateMenu(ParkPosMenu);
    }
  #endif

  void Draw_PhySet_Menu() {
    checkkey = Menu;
    if (SET_MENU(PhySetMenu, MSG_PHY_SET, 10)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      EDIT_ITEM(ICON_BedSizeX, MSG_PHY_XBEDSIZE, onDrawPIntMenu, SetBedSizeX, &PRO_data.x_bed_size);
      EDIT_ITEM(ICON_BedSizeY, MSG_PHY_YBEDSIZE, onDrawPIntMenu, SetBedSizeY, &PRO_data.y_bed_size);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMINPOS, onDrawPIntMenu, SetMinPosX, &PRO_data.x_min_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMINPOS, onDrawPIntMenu, SetMinPosY, &PRO_data.y_min_pos);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMAXPOS, onDrawPIntMenu, SetMaxPosX, &PRO_data.x_max_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMAXPOS, onDrawPIntMenu, SetMaxPosY, &PRO_data.y_max_pos);
      EDIT_ITEM(ICON_MaxPosZ, MSG_PHY_ZMAXPOS, onDrawPIntMenu, SetMaxPosZ, &PRO_data.z_max_pos);
      #if HAS_HOME_OFFSET
        MENU_ITEM(ICON_HomeOffset, MSG_SET_HOME_OFFSETS, onDrawSubMenu, Draw_HomeOffset_Menu);
      #endif
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_ParkPos, MSG_FILAMENT_PARK_ENABLED, onDrawSubMenu, Draw_ParkPos_Menu);
      #endif
    }
    UpdateMenu(PhySetMenu);
  }
#endif

#if BOTH(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)

  void Draw_CaseLight_Menu() {
    checkkey = Menu;
    if (SET_MENU(CaseLightMenu, MSG_CASE_LIGHT, 3)) {
      BACK_ITEM(Draw_Control_Menu);
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
      EDIT_ITEM(ICON_Brightness, MSG_CASE_LIGHT_BRIGHTNESS, onDrawPInt8Menu, SetCaseLightBrightness, &caselight.brightness);
    }
    UpdateMenu(CaseLightMenu);
=======
    void drawParkPosMenu() {
      checkkey = ID_Menu;
      if (SET_MENU(parkPosMenu, MSG_FILAMENT_PARK_ENABLED, 4)) {
        BACK_ITEM(drawPhySetMenu);
        EDIT_ITEM(ICON_ParkPosX, MSG_PARK_XPOSITION, onDrawPIntMenu, setParkPosX, &PRO_data.Park_point.x);
        EDIT_ITEM(ICON_ParkPosY, MSG_PARK_YPOSITION, onDrawPIntMenu, setParkPosY, &PRO_data.Park_point.y);
        EDIT_ITEM(ICON_ParkPosZ, MSG_PARK_ZRAISE, onDrawPIntMenu, setParkZRaise, &PRO_data.Park_point.z);
      }
      updateMenu(parkPosMenu);
    }
  #endif

  void drawPhySetMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(phySetMenu, MSG_PHY_SET, 10)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      EDIT_ITEM(ICON_BedSizeX, MSG_PHY_XBEDSIZE, onDrawPIntMenu, setBedSizeX, &PRO_data.x_bed_size);
      EDIT_ITEM(ICON_BedSizeY, MSG_PHY_YBEDSIZE, onDrawPIntMenu, setBedSizeY, &PRO_data.y_bed_size);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMINPOS, onDrawPIntMenu, setMinPosX, &PRO_data.x_min_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMINPOS, onDrawPIntMenu, setMinPosY, &PRO_data.y_min_pos);
      EDIT_ITEM(ICON_MaxPosX, MSG_PHY_XMAXPOS, onDrawPIntMenu, setMaxPosX, &PRO_data.x_max_pos);
      EDIT_ITEM(ICON_MaxPosY, MSG_PHY_YMAXPOS, onDrawPIntMenu, setMaxPosY, &PRO_data.y_max_pos);
      EDIT_ITEM(ICON_MaxPosZ, MSG_PHY_ZMAXPOS, onDrawPIntMenu, setMaxPosZ, &PRO_data.z_max_pos);
      #if HAS_HOME_OFFSET
        MENU_ITEM(ICON_HomeOffset, MSG_SET_HOME_OFFSETS, onDrawSubMenu, drawHomeOffsetMenu);
      #endif
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_ParkPos, MSG_FILAMENT_PARK_ENABLED, onDrawSubMenu, drawParkPosMenu);
      #endif
    }
    updateMenu(phySetMenu);
  }
#endif

#if ALL(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)

  void drawCaseLightMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(caseLightMenu, MSG_CASE_LIGHT, 3)) {
      BACK_ITEM(drawControlMenu);
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, setCaseLight, &caselight.on);
      EDIT_ITEM(ICON_Brightness, MSG_CASE_LIGHT_BRIGHTNESS, onDrawPInt8Menu, setCaseLightBrightness, &caselight.brightness);
    }
    updateMenu(caseLightMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif

#if ENABLED(LED_CONTROL_MENU)

<<<<<<< HEAD
  void Draw_LedControl_Menu() {
    checkkey = Menu;
    if (SET_MENU(LedControlMenu, MSG_LED_CONTROL, 10)) {
      BACK_ITEM(Draw_Control_Menu);
      #if !BOTH(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
        EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, SetLedStatus, &leds.lights_on);
=======
  void drawLedControlMenu () {
    checkkey = ID_Menu;
    if (SET_MENU(ledControlMenu, MSG_LED_CONTROL, 10)) {
      BACK_ITEM(drawControlMenu);
      #if !ALL(CASE_LIGHT_MENU, CASE_LIGHT_USE_NEOPIXEL)
        EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, setLedStatus, &leds.lights_on);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      #endif
      #if HAS_COLOR_LEDS
        #if ENABLED(LED_COLOR_PRESETS)
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_WHITE, onDrawMenuItem,  leds.set_white);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_RED, onDrawMenuItem,    leds.set_red);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_ORANGE, onDrawMenuItem, leds.set_orange);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_YELLOW, onDrawMenuItem, leds.set_yellow);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_GREEN, onDrawMenuItem,  leds.set_green);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_BLUE, onDrawMenuItem,   leds.set_blue);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_INDIGO, onDrawMenuItem, leds.set_indigo);
          MENU_ITEM(ICON_LedControl, MSG_SET_LEDS_VIOLET, onDrawMenuItem, leds.set_violet);
        #else
<<<<<<< HEAD
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_RED, onDrawPInt8Menu, SetLEDColorR, &leds.color.r);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_GREEN, onDrawPInt8Menu, SetLEDColorG, &leds.color.g);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_BLUE, onDrawPInt8Menu, SetLEDColorB, &leds.color.b);
          #if ENABLED(HAS_WHITE_LED)
            EDIT_ITEM(ICON_LedControl, MSG_COLORS_WHITE, onDrawPInt8Menu, SetLEDColorW, &leds.color.w);
=======
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_RED, onDrawPInt8Menu, setLEDColorR, &leds.color.r);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_GREEN, onDrawPInt8Menu, setLEDColorG, &leds.color.g);
          EDIT_ITEM(ICON_LedControl, MSG_COLORS_BLUE, onDrawPInt8Menu, setLEDColorB, &leds.color.b);
          #if ENABLED(HAS_WHITE_LED)
            EDIT_ITEM(ICON_LedControl, MSG_COLORS_WHITE, onDrawPInt8Menu, setLEDColorW, &leds.color.w);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
          #endif
        #endif
      #endif
    }
<<<<<<< HEAD
    UpdateMenu(LedControlMenu);
=======
    updateMenu(ledControlMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // LED_CONTROL_MENU

<<<<<<< HEAD
void Draw_Tune_Menu() {
  #if ENABLED(CV_LASER_MODULE)
    if (laser_device.is_laser_device()) return LCD_MESSAGE_F("Not available in laser mode");
  #endif
  checkkey = Menu;
  if (SET_MENU(TuneMenu, MSG_TUNE, 17)) {
    BACK_ITEM(Goto_PrintProcess);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, SetSpeed, &feedrate_percentage);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_HotendTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_BedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, SetFanSpeed, &thermalManager.fan_speed[0]);
    #endif
    #if ALL(HAS_ZOFFSET_ITEM, HAS_BED_PROBE, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    #elif ALL(HAS_ZOFFSET_ITEM, MESH_BED_LEVELING, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    #endif
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
    #endif
    #if ENABLED(RUNOUT_TUNE_ITEM)
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, SetRunoutEnable, &runout.enabled);
    #endif
    #if ENABLED(PLR_TUNE_ITEM)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, SetPwrLossr, &recovery.enabled);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, Draw_FWRetract_Menu);
    #endif
    #if ENABLED(JD_TUNE_ITEM)
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(ADVK_TUNE_ITEM)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, DWIN_LockScreen);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, SetBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, TurnOffBacklight);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, SetCaseLight, &caselight.on);
    #elif ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, SetLedStatus, &leds.lights_on);
    #endif
  }
  UpdateMenu(TuneMenu);
}

#if ENABLED(ADAPTIVE_STEP_SMOOTHING)
  void SetAdaptiveStepSmoothing() {
    Toggle_Chkb_Line(HMI_data.AdaptiveStepSmoothing);
=======
void drawTuneMenu() {
  #if ENABLED(CV_LASER_MODULE)
    if (laser_device.is_laser_device()) return LCD_MESSAGE_F("Not available in laser mode");
  #endif
  checkkey = ID_Menu;
  if (SET_MENU(tuneMenu, MSG_TUNE, 17)) {
    BACK_ITEM(gotoPrintProcess);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, setSpeed, &feedrate_percentage);
    #if HAS_HOTEND
      hotendTargetItem = EDIT_ITEM(ICON_HotendTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setHotendTemp, &thermalManager.temp_hotend[EXT].target);
    #endif
    #if HAS_HEATED_BED
      bedTargetItem = EDIT_ITEM(ICON_BedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      fanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, setFanSpeed, &thermalManager.fan_speed[FAN]);
    #endif
    #if ALL(HAS_ZOFFSET_ITEM, HAS_BED_PROBE, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    #elif ALL(HAS_ZOFFSET_ITEM, MESH_BED_LEVELING, BABYSTEPPING)
      EDIT_ITEM(ICON_Zoffset, MSG_HOME_OFFSET_Z, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    #endif
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, setFlow, &planner.flow_percentage[EXT]);
    #if ENABLED(ADVANCED_PAUSE_FEATURE)
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, changeFilament);
    #endif
    #if ENABLED(RUNOUT_TUNE_ITEM)
      EDIT_ITEM(ICON_Runout, MSG_RUNOUT_ENABLE, onDrawChkbMenu, setRunoutEnable, &runout.enabled);
    #endif
    #if ENABLED(PLR_TUNE_ITEM)
      EDIT_ITEM(ICON_Pwrlossr, MSG_OUTAGE_RECOVERY, onDrawChkbMenu, setPwrLossr, &recovery.enabled);
    #endif
    #if ENABLED(FWRETRACT)
      MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawSubMenu, drawFWRetractMenu);
    #endif
    #if ENABLED(JD_TUNE_ITEM)
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, setJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(ADVK_TUNE_ITEM)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, setLA_K, &planner.extruder_advance_K[EXT]);
    #endif
    #if HAS_LOCKSCREEN
      MENU_ITEM(ICON_Lock, MSG_LOCKSCREEN, onDrawMenuItem, dwinLockScreen);
    #endif
    #if HAS_LCD_BRIGHTNESS
      EDIT_ITEM(ICON_Brightness, MSG_BRIGHTNESS, onDrawPInt8Menu, setBrightness, &ui.brightness);
      MENU_ITEM(ICON_Brightness, MSG_BRIGHTNESS_OFF, onDrawMenuItem, turnOffBacklight);
    #endif
    #if ENABLED(CASE_LIGHT_MENU)
      EDIT_ITEM(ICON_CaseLight, MSG_CASE_LIGHT, onDrawChkbMenu, setCaseLight, &caselight.on);
    #elif ENABLED(LED_CONTROL_MENU) && DISABLED(CASE_LIGHT_USE_NEOPIXEL)
      EDIT_ITEM(ICON_LedControl, MSG_LEDS, onDrawChkbMenu, setLedStatus, &leds.lights_on);
    #endif
  }
  updateMenu(tuneMenu);
}

#if ENABLED(ADAPTIVE_STEP_SMOOTHING)
  void setAdaptiveStepSmoothing() {
    toggleCheckboxLine(hmiData.adaptiveStepSmoothing);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

#if ENABLED(SHAPING_MENU)
<<<<<<< HEAD
  void ApplyShapingFreq() { stepper.set_shaping_frequency(HMI_value.axis, MenuData.Value / 100); }
  void ApplyShapingZeta() { stepper.set_shaping_damping_ratio(HMI_value.axis, MenuData.Value / 100); }

  #if ENABLED(INPUT_SHAPING_X)
    void onDrawShapingXFreq(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(X_AXIS)); }
    void onDrawShapingXZeta(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(X_AXIS)); }
    void SetShapingXFreq() { HMI_value.axis = X_AXIS; SetFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(X_AXIS), ApplyShapingFreq); }
    void SetShapingXZeta() { HMI_value.axis = X_AXIS; SetFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(X_AXIS), ApplyShapingZeta); }
  #endif

  #if ENABLED(INPUT_SHAPING_Y)
    void onDrawShapingYFreq(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(Y_AXIS)); }
    void onDrawShapingYZeta(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(Y_AXIS)); }
    void SetShapingYFreq() { HMI_value.axis = Y_AXIS; SetFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(Y_AXIS), ApplyShapingFreq); }
    void SetShapingYZeta() { HMI_value.axis = Y_AXIS; SetFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(Y_AXIS), ApplyShapingZeta); }
  #endif

  void Draw_InputShaping_menu() {
    checkkey = Menu;
    if (SET_MENU(InputShapingMenu, MSG_INPUT_SHAPING, 5)) {
      BACK_ITEM(Draw_Motion_Menu);
      #if ENABLED(INPUT_SHAPING_X)
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_FREQ, onDrawShapingXFreq, SetShapingXFreq);
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_ZETA, onDrawShapingXZeta, SetShapingXZeta);
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_FREQ, onDrawShapingYFreq, SetShapingYFreq);
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_ZETA, onDrawShapingYZeta, SetShapingYZeta);
      #endif
    }
    UpdateMenu(InputShapingMenu);
=======
  void applyShapingFreq() { stepper.set_shaping_frequency(hmiValue.axis, menuData.value * 0.01); }
  void applyShapingZeta() { stepper.set_shaping_damping_ratio(hmiValue.axis, menuData.value * 0.01); }

  #if ENABLED(INPUT_SHAPING_X)
    void onDrawShapingXFreq(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(X_AXIS)); }
    void onDrawShapingXZeta(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(X_AXIS)); }
    void setShapingXFreq() { hmiValue.axis = X_AXIS; setFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(X_AXIS), applyShapingFreq); }
    void setShapingXZeta() { hmiValue.axis = X_AXIS; setFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(X_AXIS), applyShapingZeta); }
  #endif

  #if ENABLED(INPUT_SHAPING_Y)
    void onDrawShapingYFreq(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_frequency(Y_AXIS)); }
    void onDrawShapingYZeta(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, stepper.get_shaping_damping_ratio(Y_AXIS)); }
    void setShapingYFreq() { hmiValue.axis = Y_AXIS; setFloatOnClick(0, 200, 2, stepper.get_shaping_frequency(Y_AXIS), applyShapingFreq); }
    void setShapingYZeta() { hmiValue.axis = Y_AXIS; setFloatOnClick(0, 1, 2, stepper.get_shaping_damping_ratio(Y_AXIS), applyShapingZeta); }
  #endif

  void drawInputShaping_menu() {
    checkkey = ID_Menu;
    if (SET_MENU(inputShapingMenu, MSG_INPUT_SHAPING, 5)) {
      BACK_ITEM(drawMotionMenu);
      #if ENABLED(INPUT_SHAPING_X)
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_FREQ, onDrawShapingXFreq, setShapingXFreq);
        MENU_ITEM(ICON_ShapingX, MSG_SHAPING_A_ZETA, onDrawShapingXZeta, setShapingXZeta);
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_FREQ, onDrawShapingYFreq, setShapingYFreq);
        MENU_ITEM(ICON_ShapingY, MSG_SHAPING_B_ZETA, onDrawShapingYZeta, setShapingYZeta);
      #endif
    }
    updateMenu(inputShapingMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

#if HAS_TRINAMIC_CONFIG
  #if AXIS_IS_TMC(X)
<<<<<<< HEAD
    void SetXTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperX.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Y)
    void SetYTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperY.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Z)
    void SetZTMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperZ.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(E0)
    void SetETMCCurrent() { SetPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperE0.refresh_stepper_current(); }); }
  #endif

  void Draw_TrinamicConfig_menu() {
    checkkey = Menu;
    if (SET_MENU(TrinamicConfigMenu, MSG_TMC_DRIVERS, 5)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if AXIS_IS_TMC(X)
        EDIT_ITEM(ICON_TMCXSet, MSG_TMC_ACURRENT, onDrawPIntMenu, SetXTMCCurrent, &stepperX.val_mA);
      #endif
      #if AXIS_IS_TMC(Y)
        EDIT_ITEM(ICON_TMCYSet, MSG_TMC_BCURRENT, onDrawPIntMenu, SetYTMCCurrent, &stepperY.val_mA);
      #endif
      #if AXIS_IS_TMC(Z)
        EDIT_ITEM(ICON_TMCZSet, MSG_TMC_CCURRENT, onDrawPIntMenu, SetZTMCCurrent, &stepperZ.val_mA);
      #endif
      #if AXIS_IS_TMC(E0)
        EDIT_ITEM(ICON_TMCESet, MSG_TMC_ECURRENT, onDrawPIntMenu, SetETMCCurrent, &stepperE0.val_mA);
      #endif
    }
    UpdateMenu(TrinamicConfigMenu);
  }
#endif

void Draw_Motion_Menu() {
  checkkey = Menu;
  if (SET_MENU(MotionMenu, MSG_MOTION, 10)) {
    BACK_ITEM(Draw_Control_Menu);
    MENU_ITEM(ICON_MaxSpeed, MSG_SPEED, onDrawSubMenu, Draw_MaxSpeed_Menu);
    MENU_ITEM(ICON_MaxAccelerated, MSG_ACCELERATION, onDrawSubMenu, Draw_MaxAccel_Menu);
    #if HAS_CLASSIC_JERK
      MENU_ITEM(ICON_MaxJerk, MSG_JERK, onDrawSubMenu, Draw_MaxJerk_Menu);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, SetJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, SetLA_K, &planner.extruder_advance_K[0]);
    #endif
    #if ENABLED(SHAPING_MENU)
      MENU_ITEM(ICON_InputShaping, MSG_INPUT_SHAPING, onDrawSubMenu, Draw_InputShaping_menu);
    #endif
    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      EDIT_ITEM(ICON_UBLActive, MSG_STEP_SMOOTHING, onDrawChkbMenu, SetAdaptiveStepSmoothing, &HMI_data.AdaptiveStepSmoothing);
    #endif
    MENU_ITEM(ICON_Step, MSG_STEPS_PER_MM, onDrawSubMenu, Draw_Steps_Menu);
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, SetFlow, &planner.flow_percentage[0]);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, SetSpeed, &feedrate_percentage);
  }
  UpdateMenu(MotionMenu);
}

#if HAS_PREHEAT
  void Draw_PreheatHotend_Menu() {
    checkkey = Menu;
    if (SET_MENU(PreheatHotendMenu, MSG_PREHEAT_HOTEND, 1 + PREHEAT_COUNT)) {
      BACK_ITEM(Draw_FilamentMan_Menu);
      #define _ITEM_PREHEAT_HE(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheatHotend##N);
      REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT_HE)
    }
    UpdateMenu(PreheatHotendMenu);
  }
#endif

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void Draw_FilamentMan_Menu() {
    checkkey = Menu;
    if (SET_MENU(FilamentMenu, MSG_FILAMENT_MAN, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, onDrawMenuItem, ParkHead);
      #endif
      #if HAS_PREHEAT
        MENU_ITEM(ICON_SetEndTemp, MSG_PREHEAT_HOTEND, onDrawSubMenu, Draw_PreheatHotend_Menu);
      #endif
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, ChangeFilament);
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        MENU_ITEM(ICON_FilUnload, MSG_FILAMENTUNLOAD, onDrawMenuItem, UnloadFilament);
        MENU_ITEM(ICON_FilLoad, MSG_FILAMENTLOAD, onDrawMenuItem, LoadFilament);
      #endif
    }
    UpdateMenu(FilamentMenu);
=======
    void setXTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperX.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Y)
    void setYTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperY.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(Z)
    void setZTMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperZ.refresh_stepper_current(); }); }
  #endif
  #if AXIS_IS_TMC(E0)
    void setETMCCurrent() { setPIntOnClick(MIN_TMC_CURRENT, MAX_TMC_CURRENT, []{ stepperE0.refresh_stepper_current(); }); }
  #endif

  void drawTrinamicConfigMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(trinamicConfigMenu, MSG_TMC_DRIVERS, 5)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if AXIS_IS_TMC(X)
        EDIT_ITEM(ICON_TMCXSet, MSG_TMC_ACURRENT, onDrawPIntMenu, setXTMCCurrent, &stepperX.val_mA);
      #endif
      #if AXIS_IS_TMC(Y)
        EDIT_ITEM(ICON_TMCYSet, MSG_TMC_BCURRENT, onDrawPIntMenu, setYTMCCurrent, &stepperY.val_mA);
      #endif
      #if AXIS_IS_TMC(Z)
        EDIT_ITEM(ICON_TMCZSet, MSG_TMC_CCURRENT, onDrawPIntMenu, setZTMCCurrent, &stepperZ.val_mA);
      #endif
      #if AXIS_IS_TMC(E0)
        EDIT_ITEM(ICON_TMCESet, MSG_TMC_ECURRENT, onDrawPIntMenu, setETMCCurrent, &stepperE0.val_mA);
      #endif
    }
    updateMenu(trinamicConfigMenu);
  }
#endif

void drawMotionMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(motionMenu, MSG_MOTION, 10)) {
    BACK_ITEM(drawControlMenu);
    MENU_ITEM(ICON_MaxSpeed, MSG_SPEED, onDrawSubMenu, drawMaxSpeedMenu);
    MENU_ITEM(ICON_MaxAccelerated, MSG_ACCELERATION, onDrawSubMenu, drawMaxAccelMenu);
    #if HAS_CLASSIC_JERK
      MENU_ITEM(ICON_MaxJerk, MSG_JERK, onDrawSubMenu, drawMaxJerkMenu);
    #elif HAS_JUNCTION_DEVIATION
      EDIT_ITEM(ICON_JDmm, MSG_JUNCTION_DEVIATION, onDrawPFloat3Menu, setJDmm, &planner.junction_deviation_mm);
    #endif
    #if ENABLED(LIN_ADVANCE)
      EDIT_ITEM(ICON_MaxAccelerated, MSG_ADVANCE_K, onDrawPFloat3Menu, setLA_K, &planner.extruder_advance_K[EXT]);
    #endif
    #if ENABLED(SHAPING_MENU)
      MENU_ITEM(ICON_InputShaping, MSG_INPUT_SHAPING, onDrawSubMenu, drawInputShaping_menu);
    #endif
    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      EDIT_ITEM(ICON_UBLActive, MSG_STEP_SMOOTHING, onDrawChkbMenu, setAdaptiveStepSmoothing, &hmiData.adaptiveStepSmoothing);
    #endif
    MENU_ITEM(ICON_Step, MSG_STEPS_PER_MM, onDrawSubMenu, drawStepsMenu);
    EDIT_ITEM(ICON_Flow, MSG_FLOW, onDrawPIntMenu, setFlow, &planner.flow_percentage[EXT]);
    EDIT_ITEM(ICON_Speed, MSG_SPEED, onDrawPIntMenu, setSpeed, &feedrate_percentage);
  }
  updateMenu(motionMenu);
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  #if HAS_PREHEAT
    void drawPreheatHotendMenu() {
      checkkey = ID_Menu;
      if (SET_MENU(preheatHotendMenu, MSG_PREHEAT_HOTEND, 1 + PREHEAT_COUNT + 1)) {
        BACK_ITEM(drawFilamentManMenu);
        #define _ITEM_PREHEAT_HE(N) MENU_ITEM(ICON_Preheat##N, MSG_PREHEAT_##N, onDrawMenuItem, DoPreheatHotend##N);
        REPEAT_1(PREHEAT_COUNT, _ITEM_PREHEAT_HE)
        MENU_ITEM(ICON_Cool, MSG_COOLDOWN, onDrawMenuItem, []{thermalManager.setTargetHotend(0, 0);});
      }
      updateMenu(preheatHotendMenu);
    }
  #endif

  void drawFilamentManMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(filamentMenu, MSG_FILAMENT_MAN, 7)) {
      BACK_ITEM(drawPrepareMenu);
      #if ENABLED(NOZZLE_PARK_FEATURE)
        MENU_ITEM(ICON_Park, MSG_FILAMENT_PARK_ENABLED, onDrawMenuItem, parkHead);
      #endif
      #if HAS_PREHEAT
        MENU_ITEM(ICON_SetEndTemp, MSG_PREHEAT_HOTEND, onDrawSubMenu, drawPreheatHotendMenu);
      #endif
      #if ENABLED(FWRETRACT)
        MENU_ITEM(ICON_FWRetract, MSG_FWRETRACT, onDrawMenuItem, doRetract);
      #endif
      MENU_ITEM(ICON_FilMan, MSG_FILAMENTCHANGE, onDrawMenuItem, changeFilament);
      #if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)
        MENU_ITEM(ICON_FilUnload, MSG_FILAMENTUNLOAD, onDrawMenuItem, unloadFilament);
        MENU_ITEM(ICON_FilLoad, MSG_FILAMENTLOAD, onDrawMenuItem, loadFilament);
      #endif
    }
    updateMenu(filamentMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // ADVANCED_PAUSE_FEATURE

#if ENABLED(MESH_BED_LEVELING)

<<<<<<< HEAD
  void Draw_ManualMesh_Menu() {
    checkkey = Menu;
    if (SET_MENU(ManualMesh, MSG_UBL_MANUAL_MESH, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_ManualMesh, MSG_LEVEL_BED, onDrawMenuItem, ManualMeshStart);
      MMeshMoveZItem = EDIT_ITEM(ICON_Zoffset, MSG_MOVE_Z, onDrawPFloat2Menu, SetMMeshMoveZ, &current_position.z);
      MENU_ITEM(ICON_Axis, MSG_UBL_CONTINUE_MESH, onDrawMenuItem, ManualMeshContinue);
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);
      MENU_ITEM(ICON_MeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, ManualMeshSave);
    }
    UpdateMenu(ManualMesh);
=======
  void drawManualMeshMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(manualMesh, MSG_UBL_MANUAL_MESH, 6)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_ManualMesh, MSG_LEVEL_BED, onDrawMenuItem, manualMeshStart);
      mMeshMoveZItem = EDIT_ITEM(ICON_Zoffset, MSG_MOVE_Z, onDrawPFloat2Menu, setMMeshMoveZ, &current_position.z);
      MENU_ITEM(ICON_Axis, MSG_UBL_CONTINUE_MESH, onDrawMenuItem, manualMeshContinue);
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, dwinMeshViewer);
      MENU_ITEM(ICON_MeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, manualMeshSave);
    }
    updateMenu(manualMesh);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // MESH_BED_LEVELING

#if HAS_PREHEAT

<<<<<<< HEAD
  void Draw_Preheat_Menu(bool NotCurrent) {
    checkkey = Menu;
    if (NotCurrent) {
      BACK_ITEM(Draw_Temperature_Menu);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetPreheatEndTemp, &ui.material_preset[HMI_value.Select].hotend_temp);
      #endif
      #if HAS_HEATED_BED
        EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetPreheatBedTemp, &ui.material_preset[HMI_value.Select].bed_temp);
      #endif
      #if HAS_FAN
        EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPIntMenu, SetPreheatFanSpeed, &ui.material_preset[HMI_value.Select].fan_speed);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(PreheatMenu);
  }

  #define _Preheat_Menu(N) \
    void Draw_Preheat## N ##_Menu() { \
      HMI_value.Select = (N) - 1; \
      Draw_Preheat_Menu(SET_MENU(PreheatMenu, MSG_PREHEAT_## N ##_SETTINGS, 5)); \
    }
  REPEAT_1(PREHEAT_COUNT, _Preheat_Menu)

#endif // HAS_PREHEAT

void Draw_Temperature_Menu() {
  checkkey = Menu;
  if (SET_MENU(TemperatureMenu, MSG_TEMPERATURE, 4 + PREHEAT_COUNT)) {
    BACK_ITEM(Draw_Control_Menu);
    #if HAS_HOTEND
      HotendTargetItem = EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, SetHotendTemp, &thermalManager.temp_hotend[0].target);
    #endif
    #if HAS_HEATED_BED
      BedTargetItem = EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      FanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, SetFanSpeed, &thermalManager.fan_speed[0]);
    #endif
    #if HAS_PREHEAT
      #define _ITEM_SETPREHEAT(N) MENU_ITEM(ICON_SetPreheat##N, MSG_PREHEAT_## N ##_SETTINGS, onDrawSubMenu, Draw_Preheat## N ##_Menu);
      REPEAT_1(PREHEAT_COUNT, _ITEM_SETPREHEAT)
    #endif
  }
  UpdateMenu(TemperatureMenu);
}

void Draw_MaxSpeed_Menu() {
  checkkey = Menu;
  if (SET_MENU(MaxSpeedMenu, MSG_MAX_SPEED, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxSpeedX, MSG_VMAX_A, onDrawPFloatMenu, SetMaxSpeedX, &planner.settings.max_feedrate_mm_s[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxSpeedY, MSG_VMAX_B, onDrawPFloatMenu, SetMaxSpeedY, &planner.settings.max_feedrate_mm_s[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxSpeedZ, MSG_VMAX_C, onDrawPFloatMenu, SetMaxSpeedZ, &planner.settings.max_feedrate_mm_s[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxSpeedE, MSG_VMAX_E, onDrawPFloatMenu, SetMaxSpeedE, &planner.settings.max_feedrate_mm_s[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxSpeedMenu);
}

void Draw_MaxAccel_Menu() {
  checkkey = Menu;
  if (SET_MENU(MaxAccelMenu, MSG_AMAX_EN, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxAccX, MSG_AMAX_A, onDrawPInt32Menu, SetMaxAccelX, &planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxAccY, MSG_AMAX_B, onDrawPInt32Menu, SetMaxAccelY, &planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxAccZ, MSG_AMAX_C, onDrawPInt32Menu, SetMaxAccelZ, &planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxAccE, MSG_AMAX_E, onDrawPInt32Menu, SetMaxAccelE, &planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
  }
  UpdateMenu(MaxAccelMenu);
=======
  void drawPreheatMenu(bool NotCurrent) {
    checkkey = ID_Menu;
    if (NotCurrent) {
      BACK_ITEM(drawTemperatureMenu);
      #if HAS_HOTEND
        EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setPreheatEndTemp, &ui.material_preset[hmiValue.select].hotend_temp);
      #endif
      #if HAS_HEATED_BED
        EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setPreheatBedTemp, &ui.material_preset[hmiValue.select].bed_temp);
      #endif
      #if HAS_FAN
        EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPIntMenu, setPreheatFanSpeed, &ui.material_preset[hmiValue.select].fan_speed);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
    updateMenu(preheatMenu);
  }

  #define _PreheatMenu(N) \
    void drawPreheat## N ##Menu() { \
      hmiValue.select = (N) - 1; \
      drawPreheatMenu(SET_MENU(preheatMenu, MSG_PREHEAT_## N ##_SETTINGS, 5)); \
    }
  REPEAT_1(PREHEAT_COUNT, _PreheatMenu)

#endif // HAS_PREHEAT

void drawTemperatureMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(temperatureMenu, MSG_TEMPERATURE, 4 + PREHEAT_COUNT)) {
    BACK_ITEM(drawControlMenu);
    #if HAS_HOTEND
      hotendTargetItem = EDIT_ITEM(ICON_SetEndTemp, MSG_UBL_SET_TEMP_HOTEND, onDrawPIntMenu, setHotendTemp, &thermalManager.temp_hotend[EXT].target);
    #endif
    #if HAS_HEATED_BED
      bedTargetItem = EDIT_ITEM(ICON_SetBedTemp, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedTemp, &thermalManager.temp_bed.target);
    #endif
    #if HAS_FAN
      fanSpeedItem = EDIT_ITEM(ICON_FanSpeed, MSG_FAN_SPEED, onDrawPInt8Menu, setFanSpeed, &thermalManager.fan_speed[FAN]);
    #endif
    #if HAS_PREHEAT
      #define _ITEM_SETPREHEAT(N) MENU_ITEM(ICON_SetPreheat##N, MSG_PREHEAT_## N ##_SETTINGS, onDrawSubMenu, drawPreheat## N ##Menu);
      REPEAT_1(PREHEAT_COUNT, _ITEM_SETPREHEAT)
    #endif
  }
  updateMenu(temperatureMenu);
}

void drawMaxSpeedMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(maxSpeedMenu, MSG_MAX_SPEED, 5)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxSpeedX, MSG_VMAX_A, onDrawPFloatMenu, setMaxSpeedX, &planner.settings.max_feedrate_mm_s[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxSpeedY, MSG_VMAX_B, onDrawPFloatMenu, setMaxSpeedY, &planner.settings.max_feedrate_mm_s[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxSpeedZ, MSG_VMAX_C, onDrawPFloatMenu, setMaxSpeedZ, &planner.settings.max_feedrate_mm_s[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxSpeedE, MSG_VMAX_E, onDrawPFloatMenu, setMaxSpeedE, &planner.settings.max_feedrate_mm_s[E_AXIS]);
    #endif
  }
  updateMenu(maxSpeedMenu);
}

void drawMaxAccelMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(maxAccelMenu, MSG_AMAX_EN, 5)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_MaxAccX, MSG_AMAX_A, onDrawPInt32Menu, setMaxAccelX, &planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_MaxAccY, MSG_AMAX_B, onDrawPInt32Menu, setMaxAccelY, &planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_MaxAccZ, MSG_AMAX_C, onDrawPInt32Menu, setMaxAccelZ, &planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_MaxAccE, MSG_AMAX_E, onDrawPInt32Menu, setMaxAccelE, &planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    #endif
  }
  updateMenu(maxAccelMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

#if HAS_CLASSIC_JERK

<<<<<<< HEAD
  void Draw_MaxJerk_Menu() {
    checkkey = Menu;
    if (SET_MENU(MaxJerkMenu, MSG_JERK, 5)) {
      BACK_ITEM(Draw_Motion_Menu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkX, MSG_VA_JERK, onDrawPFloatMenu, SetMaxJerkX, &planner.max_jerk.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkY, MSG_VB_JERK, onDrawPFloatMenu, SetMaxJerkY, &planner.max_jerk.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkZ, MSG_VC_JERK, onDrawPFloatMenu, SetMaxJerkZ, &planner.max_jerk.z);
      #endif
      #if HAS_HOTEND
        EDIT_ITEM(ICON_MaxSpeedJerkE, MSG_VE_JERK, onDrawPFloatMenu, SetMaxJerkE, &planner.max_jerk.e);
      #endif
    }
    UpdateMenu(MaxJerkMenu);
=======
  void drawMaxJerkMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(maxJerkMenu, MSG_JERK, 5)) {
      BACK_ITEM(drawMotionMenu);
      #if HAS_X_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkX, MSG_VA_JERK, onDrawPFloatMenu, setMaxJerkX, &planner.max_jerk.x);
      #endif
      #if HAS_Y_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkY, MSG_VB_JERK, onDrawPFloatMenu, setMaxJerkY, &planner.max_jerk.y);
      #endif
      #if HAS_Z_AXIS
        EDIT_ITEM(ICON_MaxSpeedJerkZ, MSG_VC_JERK, onDrawPFloatMenu, setMaxJerkZ, &planner.max_jerk.z);
      #endif
      #if HAS_HOTEND
        EDIT_ITEM(ICON_MaxSpeedJerkE, MSG_VE_JERK, onDrawPFloatMenu, setMaxJerkE, &planner.max_jerk.e);
      #endif
    }
    updateMenu(maxJerkMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // HAS_CLASSIC_JERK

<<<<<<< HEAD
void Draw_Steps_Menu() {
  checkkey = Menu;
  if (SET_MENU(StepsMenu, MSG_STEPS_PER_MM, 5)) {
    BACK_ITEM(Draw_Motion_Menu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_StepX, MSG_A_STEPS, onDrawPFloatMenu, SetStepsX, &planner.settings.axis_steps_per_mm[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_StepY, MSG_B_STEPS, onDrawPFloatMenu, SetStepsY, &planner.settings.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_StepZ, MSG_C_STEPS, onDrawPFloatMenu, SetStepsZ, &planner.settings.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_StepE, MSG_E_STEPS, onDrawPFloatMenu, SetStepsE, &planner.settings.axis_steps_per_mm[E_AXIS]);
    #endif
  }
  UpdateMenu(StepsMenu);
=======
void drawStepsMenu() {
  checkkey = ID_Menu;
  if (SET_MENU(stepsMenu, MSG_STEPS_PER_MM, 5)) {
    BACK_ITEM(drawMotionMenu);
    #if HAS_X_AXIS
      EDIT_ITEM(ICON_StepX, MSG_A_STEPS, onDrawPFloatMenu, setStepsX, &planner.settings.axis_steps_per_mm[X_AXIS]);
    #endif
    #if HAS_Y_AXIS
      EDIT_ITEM(ICON_StepY, MSG_B_STEPS, onDrawPFloatMenu, setStepsY, &planner.settings.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if HAS_Z_AXIS
      EDIT_ITEM(ICON_StepZ, MSG_C_STEPS, onDrawPFloatMenu, setStepsZ, &planner.settings.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if HAS_HOTEND
      EDIT_ITEM(ICON_StepE, MSG_E_STEPS, onDrawPFloatMenu, setStepsE, &planner.settings.axis_steps_per_mm[E_AXIS]);
    #endif
  }
  updateMenu(stepsMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
}

//=============================================================================
// UI editable custom colors
//=============================================================================

#if HAS_CUSTOM_COLORS

<<<<<<< HEAD
  void RestoreDefaultColors() {
    DWIN_SetColorDefaults();
    DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
    DWIN_RedrawScreen();
  }

  void SelColor() {
    MenuData.P_Int = (int16_t*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    HMI_value.Color[0] = GetRColor(*MenuData.P_Int);  // Red
    HMI_value.Color[1] = GetGColor(*MenuData.P_Int);  // Green
    HMI_value.Color[2] = GetBColor(*MenuData.P_Int);  // Blue
    Draw_GetColor_Menu();
  }

  void LiveRGBColor() {
      HMI_value.Color[CurrentMenu->line() - 2] = MenuData.Value;
      uint16_t color = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
      DWIN_Draw_Rectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
  }
  void SetRGBColor() {
    const uint8_t color = static_cast<MenuItemClass*>(CurrentMenu->SelectedItem())->icon;
    SetIntOnClick(0, (color == 1) ? 63 : 31, HMI_value.Color[color], nullptr, LiveRGBColor);
  }

  void DWIN_ApplyColor() {
    *MenuData.P_Int = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
    DWINUI::SetColors(HMI_data.Text_Color, HMI_data.Background_Color, HMI_data.StatusBg_Color);
    Draw_SelectColors_Menu();
    hash_changed = true;
    LCD_MESSAGE(MSG_COLORS_APPLIED);
    DWIN_Draw_Dashboard();
  }

  void DWIN_ApplyColor(const int8_t element, const bool ldef /* = false*/) {
    const uint16_t color = RGB(HMI_value.Color[0], HMI_value.Color[1], HMI_value.Color[2]);
    switch (element) {
      case  2: HMI_data.Background_Color = ldef ? Def_Background_Color : color; DWINUI::SetBackgroundColor(HMI_data.Background_Color); break;
      case  3: HMI_data.Cursor_Color     = ldef ? Def_Cursor_Color     : color; break;
      case  4: HMI_data.TitleBg_Color    = ldef ? Def_TitleBg_Color    : color; break;
      case  5: HMI_data.TitleTxt_Color   = ldef ? Def_TitleTxt_Color   : color; break;
      case  6: HMI_data.Text_Color       = ldef ? Def_Text_Color       : color; DWINUI::SetTextColor(HMI_data.Text_Color); break;
      case  7: HMI_data.Selected_Color   = ldef ? Def_Selected_Color   : color; break;
      case  8: HMI_data.SplitLine_Color  = ldef ? Def_SplitLine_Color  : color; break;
      case  9: HMI_data.Highlight_Color  = ldef ? Def_Highlight_Color  : color; break;
      case 10: HMI_data.StatusBg_Color   = ldef ? Def_StatusBg_Color   : color; break;
      case 11: HMI_data.StatusTxt_Color  = ldef ? Def_StatusTxt_Color  : color; break;
      case 12: HMI_data.PopupBg_Color    = ldef ? Def_PopupBg_Color    : color; break;
      case 13: HMI_data.PopupTxt_Color   = ldef ? Def_PopupTxt_Color   : color; break;
      case 14: HMI_data.AlertBg_Color    = ldef ? Def_AlertBg_Color    : color; break;
      case 15: HMI_data.AlertTxt_Color   = ldef ? Def_AlertTxt_Color   : color; break;
      case 16: HMI_data.PercentTxt_Color = ldef ? Def_PercentTxt_Color : color; break;
      case 17: HMI_data.Barfill_Color    = ldef ? Def_Barfill_Color    : color; break;
      case 18: HMI_data.Indicator_Color  = ldef ? Def_Indicator_Color  : color; break;
      case 19: HMI_data.Coordinate_Color = ldef ? Def_Coordinate_Color : color; break;
=======
  void restoreDefaultColors() {
    dwinSetColorDefaults();
    DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
    dwinRedrawScreen();
  }

  void selColor() {
    menuData.intPtr = (int16_t*)static_cast<MenuItemPtr*>(currentMenu->selectedItem())->value;
    hmiValue.Color[0] = GetRColor(*menuData.intPtr);  // Red
    hmiValue.Color[1] = GetGColor(*menuData.intPtr);  // Green
    hmiValue.Color[2] = GetBColor(*menuData.intPtr);  // Blue
    drawGetColorMenu();
  }

  void liveRGBColor() {
    hmiValue.Color[currentMenu->line() - 2] = menuData.value;
    uint16_t color = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
    dwinDrawRectangle(1, color, 20, 315, DWIN_WIDTH - 20, 335);
  }
  void setRGBColor() {
    const uint8_t color = static_cast<MenuItem*>(currentMenu->selectedItem())->icon;
    setIntOnClick(0, (color == 1) ? 63 : 31, hmiValue.Color[color], nullptr, liveRGBColor);
  }

  void dwinApplyColor() {
    *menuData.intPtr = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
    DWINUI::setColors(hmiData.colorText, hmiData.colorBackground, hmiData.colorStatusBg);
    drawSelectColorsMenu();
    hash_changed = true;
    LCD_MESSAGE(MSG_COLORS_APPLIED);
    dwinDrawDashboard();
  }

  void dwinApplyColor(const int8_t element, const bool ldef /* = false*/) {
    const uint16_t color = RGB(hmiValue.Color[0], hmiValue.Color[1], hmiValue.Color[2]);
    switch (element) {
      case  2: hmiData.colorBackground = ldef ? defColorBackground : color; DWINUI::setBackgroundColor(hmiData.colorBackground); break;
      case  3: hmiData.colorCursor     = ldef ? defColorCursor     : color; break;
      case  4: hmiData.colorTitleBg    = ldef ? defColorTitleBg    : color; break;
      case  5: hmiData.colorTitleTxt   = ldef ? defColorTitleTxt   : color; break;
      case  6: hmiData.colorText       = ldef ? defColorText       : color; DWINUI::setTextColor(hmiData.colorText); break;
      case  7: hmiData.colorSelected   = ldef ? defColorSelected   : color; break;
      case  8: hmiData.colorSplitLine  = ldef ? defColorSplitLine  : color; break;
      case  9: hmiData.colorHighlight  = ldef ? defColorHighlight  : color; break;
      case 10: hmiData.colorStatusBg   = ldef ? defColorStatusBg   : color; break;
      case 11: hmiData.colorStatusTxt  = ldef ? defColorStatusTxt  : color; break;
      case 12: hmiData.colorPopupBg    = ldef ? defColorPopupBg    : color; break;
      case 13: hmiData.colorPopupTxt   = ldef ? defColorPopupTxt   : color; break;
      case 14: hmiData.colorAlertBg    = ldef ? defColorAlertBg    : color; break;
      case 15: hmiData.colorAlertTxt   = ldef ? defColorAlertTxt   : color; break;
      case 16: hmiData.colorPercentTxt = ldef ? defColorPercentTxt : color; break;
      case 17: hmiData.colorBarfill    = ldef ? defColorBarfill    : color; break;
      case 18: hmiData.colorIndicator  = ldef ? defColorIndicator  : color; break;
      case 19: hmiData.colorCoordinate = ldef ? defColorCoordinate : color; break;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      default: break;
    }
  }

<<<<<<< HEAD
  void Draw_SelectColors_Menu() {
    checkkey = Menu;
    if (SET_MENU(SelectColorMenu, MSG_COLORS_SELECT, 20)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      MENU_ITEM(ICON_StockConfiguration, MSG_RESTORE_DEFAULTS, onDrawMenuItem, RestoreDefaultColors);
      EDIT_ITEM_F(0, "Screen Background", onDrawSelColorItem, SelColor, &HMI_data.Background_Color);
      EDIT_ITEM_F(0, "Cursor", onDrawSelColorItem, SelColor, &HMI_data.Cursor_Color);
      EDIT_ITEM_F(0, "Title Background", onDrawSelColorItem, SelColor, &HMI_data.TitleBg_Color);
      EDIT_ITEM_F(0, "Title Text", onDrawSelColorItem, SelColor, &HMI_data.TitleTxt_Color);
      EDIT_ITEM_F(0, "Text", onDrawSelColorItem, SelColor, &HMI_data.Text_Color);
      EDIT_ITEM_F(0, "Selected", onDrawSelColorItem, SelColor, &HMI_data.Selected_Color);
      EDIT_ITEM_F(0, "Split Line", onDrawSelColorItem, SelColor, &HMI_data.SplitLine_Color);
      EDIT_ITEM_F(0, "Highlight", onDrawSelColorItem, SelColor, &HMI_data.Highlight_Color);
      EDIT_ITEM_F(0, "Status Background", onDrawSelColorItem, SelColor, &HMI_data.StatusBg_Color);
      EDIT_ITEM_F(0, "Status Text", onDrawSelColorItem, SelColor, &HMI_data.StatusTxt_Color);
      EDIT_ITEM_F(0, "Popup Background", onDrawSelColorItem, SelColor, &HMI_data.PopupBg_Color);
      EDIT_ITEM_F(0, "Popup Text", onDrawSelColorItem, SelColor, &HMI_data.PopupTxt_Color);
      EDIT_ITEM_F(0, "Alert Background", onDrawSelColorItem, SelColor, &HMI_data.AlertBg_Color);
      EDIT_ITEM_F(0, "Alert Text", onDrawSelColorItem, SelColor, &HMI_data.AlertTxt_Color);
      EDIT_ITEM_F(0, "Percent Text", onDrawSelColorItem, SelColor, &HMI_data.PercentTxt_Color);
      EDIT_ITEM_F(0, "Bar Fill", onDrawSelColorItem, SelColor, &HMI_data.Barfill_Color);
      EDIT_ITEM_F(0, "Indicator value", onDrawSelColorItem, SelColor, &HMI_data.Indicator_Color);
      EDIT_ITEM_F(0, "Coordinate value", onDrawSelColorItem, SelColor, &HMI_data.Coordinate_Color);
    }
    UpdateMenu(SelectColorMenu);
  }

  void Draw_GetColor_Menu() {
    checkkey = Menu;
    if (SET_MENU(GetColorMenu, MSG_COLORS_GET, 5)) {
      BACK_ITEM(DWIN_ApplyColor);
      MENU_ITEM(ICON_Cancel, MSG_BUTTON_CANCEL, onDrawMenuItem, Draw_SelectColors_Menu);
      MENU_ITEM(0, MSG_COLORS_RED, onDrawGetColorItem, SetRGBColor);
      MENU_ITEM(1, MSG_COLORS_GREEN, onDrawGetColorItem, SetRGBColor);
      MENU_ITEM(2, MSG_COLORS_BLUE, onDrawGetColorItem, SetRGBColor);
    }
    UpdateMenu(GetColorMenu);
    DWIN_Draw_Rectangle(1, *MenuData.P_Int, 20, 315, DWIN_WIDTH - 20, 335);
  }

#endif
=======
  void drawSelectColorsMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(selectColorMenu, MSG_COLORS_SELECT, 20)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      MENU_ITEM(ICON_StockConfiguration, MSG_RESTORE_DEFAULTS, onDrawMenuItem, restoreDefaultColors);
      EDIT_ITEM_F(0, "Screen Background", onDrawSelColorItem, selColor, &hmiData.colorBackground);
      EDIT_ITEM_F(0, "Cursor", onDrawSelColorItem, selColor, &hmiData.colorCursor);
      EDIT_ITEM_F(0, "Title Background", onDrawSelColorItem, selColor, &hmiData.colorTitleBg);
      EDIT_ITEM_F(0, "Title Text", onDrawSelColorItem, selColor, &hmiData.colorTitleTxt);
      EDIT_ITEM_F(0, "Text", onDrawSelColorItem, selColor, &hmiData.colorText);
      EDIT_ITEM_F(0, "Selected", onDrawSelColorItem, selColor, &hmiData.colorSelected);
      EDIT_ITEM_F(0, "Split Line", onDrawSelColorItem, selColor, &hmiData.colorSplitLine);
      EDIT_ITEM_F(0, "Highlight", onDrawSelColorItem, selColor, &hmiData.colorHighlight);
      EDIT_ITEM_F(0, "Status Background", onDrawSelColorItem, selColor, &hmiData.colorStatusBg);
      EDIT_ITEM_F(0, "Status Text", onDrawSelColorItem, selColor, &hmiData.colorStatusTxt);
      EDIT_ITEM_F(0, "Popup Background", onDrawSelColorItem, selColor, &hmiData.colorPopupBg);
      EDIT_ITEM_F(0, "Popup Text", onDrawSelColorItem, selColor, &hmiData.colorPopupTxt);
      EDIT_ITEM_F(0, "Alert Background", onDrawSelColorItem, selColor, &hmiData.colorAlertBg);
      EDIT_ITEM_F(0, "Alert Text", onDrawSelColorItem, selColor, &hmiData.colorAlertTxt);
      EDIT_ITEM_F(0, "Percent Text", onDrawSelColorItem, selColor, &hmiData.colorPercentTxt);
      EDIT_ITEM_F(0, "Bar Fill", onDrawSelColorItem, selColor, &hmiData.colorBarfill);
      EDIT_ITEM_F(0, "Indicator value", onDrawSelColorItem, selColor, &hmiData.colorIndicator);
      EDIT_ITEM_F(0, "Coordinate value", onDrawSelColorItem, selColor, &hmiData.colorCoordinate);
    }
    updateMenu(selectColorMenu);
  }

  void drawGetColorMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(getColorMenu, MSG_COLORS_GET, 5)) {
      BACK_ITEM(dwinApplyColor);
      MENU_ITEM(ICON_Cancel, MSG_BUTTON_CANCEL, onDrawMenuItem, drawSelectColorsMenu);
      MENU_ITEM(0, MSG_COLORS_RED, onDrawGetColorItem, setRGBColor);
      MENU_ITEM(1, MSG_COLORS_GREEN, onDrawGetColorItem, setRGBColor);
      MENU_ITEM(2, MSG_COLORS_BLUE, onDrawGetColorItem, setRGBColor);
    }
    updateMenu(getColorMenu);
    dwinDrawRectangle(1, *menuData.intPtr, 20, 315, DWIN_WIDTH - 20, 335);
  }

#endif // HAS_CUSTOM_COLORS
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

//=============================================================================
// Nozzle and Bed PID/MPC
//=============================================================================

<<<<<<< HEAD
#if EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)

  #if ENABLED(MPC_EDIT_MENU)
    void SetHeaterPower() { SetPFloatOnClick(1, 200, 1); }
    void SetBlkHeatCapacity() { SetPFloatOnClick(0, 40, 2); }
    void SetSensorResponse() { SetPFloatOnClick(0, 1, 4); }
    void SetAmbientXfer() { SetPFloatOnClick(0, 1, 4); }
    #if ENABLED(MPC_INCLUDE_FAN)
      void onDrawFanAdj(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 4, thermalManager.temp_hotend[0].fanCoefficient()); }
      void ApplyFanAdj() { thermalManager.temp_hotend[0].applyFanAdjustment(MenuData.Value / POW(10, 4)); }
      void SetFanAdj() { SetFloatOnClick(0, 1, 4, thermalManager.temp_hotend[0].fanCoefficient(), ApplyFanAdj); }
    #endif
  #endif

  void Draw_HotendMPC_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendMPCMenu, "MPC Settings", 7)) {
      MPC_t &mpc = thermalManager.temp_hotend[0].mpc;
      BACK_ITEM(Draw_AdvancedSettings_Menu);
=======
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)

  #if ENABLED(MPC_EDIT_MENU)
    void setHeaterPower() { setPFloatOnClick(1, 200, 1); }
    void setBlkHeatCapacity() { setPFloatOnClick(0, 40, 2); }
    void SetSensorResponse() { setPFloatOnClick(0, 1, 4); }
    void setAmbientXfer() { setPFloatOnClick(0, 1, 4); }
    #if ENABLED(MPC_INCLUDE_FAN)
      void onDrawFanAdj(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 4, thermalManager.temp_hotend[EXT].fanCoefficient()); }
      void applyFanAdj() { thermalManager.temp_hotend[EXT].applyFanAdjustment(menuData.value / POW(10, 4)); }
      void setFanAdj() { setFloatOnClick(0, 1, 4, thermalManager.temp_hotend[EXT].fanCoefficient(), applyFanAdj); }
    #endif
  #endif

  void drawHotendMPCMenu() {
    checkkey = ID_Menu;
    if (SET_MENU_F(hotendMPCMenu, "MPC Settings", 7)) {
      MPC_t &mpc = thermalManager.temp_hotend[EXT].mpc;
      BACK_ITEM(drawAdvancedSettingsMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      #if ENABLED(MPC_AUTOTUNE_MENU)
        MENU_ITEM(ICON_MPCNozzle, MSG_MPC_AUTOTUNE, onDrawMenuItem, []{ thermalManager.MPC_autotune(active_extruder, Temperature::MPCTuningType::AUTO); });
      #endif
      #if ENABLED(MPC_EDIT_MENU)
<<<<<<< HEAD
        EDIT_ITEM(ICON_MPCHeater, MSG_MPC_POWER, onDrawPFloatMenu, SetHeaterPower, &mpc.heater_power);
        EDIT_ITEM(ICON_MPCHeatCap, MSG_MPC_BLOCK_HEAT_CAPACITY, onDrawPFloat2Menu, SetBlkHeatCapacity, &mpc.block_heat_capacity);
        EDIT_ITEM(ICON_MPCValue, MSG_SENSOR_RESPONSIVENESS, onDrawPFloat4Menu, SetSensorResponse, &mpc.sensor_responsiveness);
        EDIT_ITEM(ICON_MPCValue, MSG_MPC_AMBIENT_XFER_COEFF, onDrawPFloat4Menu, SetAmbientXfer, &mpc.ambient_xfer_coeff_fan0);
        #if ENABLED(MPC_INCLUDE_FAN)
          EDIT_ITEM(ICON_MPCFan, MSG_MPC_AMBIENT_XFER_COEFF_FAN, onDrawFanAdj, SetFanAdj, &mpc.fan255_adjustment);
        #endif
      #endif
    }
    UpdateMenu(HotendMPCMenu);
=======
        EDIT_ITEM(ICON_MPCHeater, MSG_MPC_POWER, onDrawPFloatMenu, setHeaterPower, &mpc.heater_power);
        EDIT_ITEM(ICON_MPCHeatCap, MSG_MPC_BLOCK_HEAT_CAPACITY, onDrawPFloat2Menu, setBlkHeatCapacity, &mpc.block_heat_capacity);
        EDIT_ITEM(ICON_MPCValue, MSG_SENSOR_RESPONSIVENESS, onDrawPFloat4Menu, SetSensorResponse, &mpc.sensor_responsiveness);
        EDIT_ITEM(ICON_MPCValue, MSG_MPC_AMBIENT_XFER_COEFF, onDrawPFloat4Menu, setAmbientXfer, &mpc.ambient_xfer_coeff_fan0);
        #if ENABLED(MPC_INCLUDE_FAN)
          EDIT_ITEM(ICON_MPCFan, MSG_MPC_AMBIENT_XFER_COEFF_FAN, onDrawFanAdj, setFanAdj, &mpc.fan255_adjustment);
        #endif
      #endif
    }
    updateMenu(hotendMPCMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // MPC_EDIT_MENU || MPC_AUTOTUNE_MENU

#if HAS_PID_HEATING && ENABLED(PID_AUTOTUNE_MENU)
<<<<<<< HEAD
  void SetPID(celsius_t t, heater_id_t h) {
    char cmd[53] = "";
    char str_1[5] = "", str_2[5] = "";
    sprintf_P(cmd, PSTR("G28OXY\nG0Z5F300\nG0X%sY%sF5000\nM84\nM400"),
      dtostrf(X_CENTER, 1, 1, str_1),
      dtostrf(Y_CENTER, 1, 1, str_2)
    );
    gcode.process_subcommands_now(cmd);
    thermalManager.PID_autotune(t, h, HMI_data.PidCycles, true);
  }
  void SetPidCycles() { SetPIntOnClick(3, 50); }
#endif

#if HAS_PID_HEATING && ENABLED(PID_EDIT_MENU)
  void SetKp() { SetPFloatOnClick(0, 1000, 2); }
  void ApplyPIDi() {
    *MenuData.P_Float = scalePID_i(MenuData.Value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void ApplyPIDd() {
    *MenuData.P_Float = scalePID_d(MenuData.Value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void SetKi() {
    MenuData.P_Float = (float*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    const float value = unscalePID_i(*MenuData.P_Float);
    SetFloatOnClick(0, 1000, 2, value, ApplyPIDi);
  }
  void SetKd() {
    MenuData.P_Float = (float*)static_cast<MenuItemPtrClass*>(CurrentMenu->SelectedItem())->value;
    const float value = unscalePID_d(*MenuData.P_Float);
    SetFloatOnClick(0, 1000, 2, value, ApplyPIDd);
  }
  void onDrawPIDi(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_i(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }
  void onDrawPIDd(MenuItemClass* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_d(*(float*)static_cast<MenuItemPtrClass*>(menuitem)->value)); }
#endif

#if ENABLED(PIDTEMP) && EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void HotendPID() { SetPID(HMI_data.HotendPidT, H_E0); }
    void SetHotendPidT() { SetPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif

  void Draw_HotendPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(HotendPIDMenu, STR_HOTEND_PID " Settings", 8)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID, onDrawMenuItem, HotendPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetHotendPidT, &HMI_data.HotendPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_hotend[0].pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_hotend[0].pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_hotend[0].pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(HotendPIDMenu);
=======
  void setPID(celsius_t t, heater_id_t h) {
    gcode.process_subcommands_now(TS(F("G28OXY\nG0Z5F300\nG0X"), X_CENTER, F("Y"), Y_CENTER, F("F5000\nM84\nM400")));
    thermalManager.PID_autotune(t, h, hmiData.pidCycles, true);
  }
  void setPidCycles() { setPIntOnClick(3, 50); }
#endif

#if HAS_PID_HEATING && ENABLED(PID_EDIT_MENU)

  void setKp() { setPFloatOnClick(0, 1000, 2); }
  void applyPIDi() {
    *menuData.floatPtr = scalePID_i(menuData.value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void applyPIDd() {
    *menuData.floatPtr = scalePID_d(menuData.value / POW(10, 2));
    TERN_(PIDTEMP, thermalManager.updatePID());
  }
  void setKi() {
    menuData.floatPtr = (float*)static_cast<MenuItemPtr*>(currentMenu->selectedItem())->value;
    const float value = unscalePID_i(*menuData.floatPtr);
    setFloatOnClick(0, 1000, 2, value, applyPIDi);
  }
  void setKd() {
    menuData.floatPtr = (float*)static_cast<MenuItemPtr*>(currentMenu->selectedItem())->value;
    const float value = unscalePID_d(*menuData.floatPtr);
    setFloatOnClick(0, 1000, 2, value, applyPIDd);
  }
  void onDrawPIDi(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_i(*(float*)static_cast<MenuItemPtr*>(menuitem)->value)); }
  void onDrawPIDd(MenuItem* menuitem, int8_t line) { onDrawFloatMenu(menuitem, line, 2, unscalePID_d(*(float*)static_cast<MenuItemPtr*>(menuitem)->value)); }

#endif // HAS_PID_HEATING && PID_EDIT_MENU

#if ENABLED(PIDTEMP) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void hotendPID() { setPID(hmiData.hotendPidT, H_E0); }
    void setHotendPidT() { setPIntOnClick(MIN_ETEMP, MAX_ETEMP); }
  #endif

  void drawHotendPIDMenu() {
    checkkey = ID_Menu;
    if (SET_MENU_F(hotendPIDMenu, STR_HOTEND_PID " Settings", 8)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDNozzle, STR_HOTEND_PID, onDrawMenuItem, hotendPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, setHotendPidT, &hmiData.hotendPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, setPidCycles, &hmiData.pidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, setKp, &thermalManager.temp_hotend[EXT].pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, setKi, &thermalManager.temp_hotend[EXT].pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, setKd, &thermalManager.temp_hotend[EXT].pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
    updateMenu(hotendPIDMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // PIDTEMP && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)

<<<<<<< HEAD
#if ENABLED(PIDTEMPBED) && EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void BedPID() { SetPID(HMI_data.BedPidT, H_BED); }
    void SetBedPidT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  void Draw_BedPID_Menu() {
    checkkey = Menu;
    if (SET_MENU_F(BedPIDMenu, STR_BED_PID " Settings", 8)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDBed, STR_BED_PID, onDrawMenuItem,BedPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, SetBedPidT, &HMI_data.BedPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, SetPidCycles, &HMI_data.PidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, SetKp, &thermalManager.temp_bed.pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, SetKi, &thermalManager.temp_bed.pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, SetKd, &thermalManager.temp_bed.pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, WriteEeprom);
      #endif
    }
    UpdateMenu(BedPIDMenu);
  }

#endif // PIDTEMPBED
=======
#if ENABLED(PIDTEMPBED) && ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)

  #if ENABLED(PID_AUTOTUNE_MENU)
    void bedPID() { setPID(hmiData.bedPidT, H_BED); }
    void setBedPidT() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  void drawBedPIDMenu() {
    checkkey = ID_Menu;
    if (SET_MENU_F(bedPIDMenu, STR_BED_PID " Settings", 8)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if ENABLED(PID_AUTOTUNE_MENU)
        MENU_ITEM_F(ICON_PIDBed, STR_BED_PID, onDrawMenuItem,bedPID);
        EDIT_ITEM(ICON_Temperature, MSG_TEMPERATURE, onDrawPIntMenu, setBedPidT, &hmiData.bedPidT);
        EDIT_ITEM(ICON_PIDCycles, MSG_PID_CYCLE, onDrawPIntMenu, setPidCycles, &hmiData.pidCycles);
      #endif
      #if ENABLED(PID_EDIT_MENU)
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KP, onDrawPFloat2Menu, setKp, &thermalManager.temp_bed.pid.Kp);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KI, onDrawPIDi, setKi, &thermalManager.temp_bed.pid.Ki);
        EDIT_ITEM_F(ICON_PIDValue, "Set" STR_KD, onDrawPIDd, setKd, &thermalManager.temp_bed.pid.Kd);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(ICON_WriteEEPROM, MSG_STORE_EEPROM, onDrawMenuItem, writeEeprom);
      #endif
    }
    updateMenu(bedPIDMenu);
  }

#endif // PIDTEMPBED && (PID_AUTOTUNE_MENU || PID_EDIT_MENU)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

//=============================================================================
//
//=============================================================================

#if HAS_BED_PROBE

<<<<<<< HEAD
  void Draw_ZOffsetWiz_Menu() {
    checkkey = Menu;
    if (SET_MENU(ZOffsetWizMenu, MSG_PROBE_WIZARD, 4)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      MENU_ITEM_F(ICON_MoveZ0, "Move Z to Home", onDrawMenuItem, SetMoveZto0);
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, SetZOffset, &BABY_Z_VAR);
    }
    UpdateMenu(ZOffsetWizMenu);
=======
  void drawZOffsetWizMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(zOffsetWizMenu, MSG_PROBE_WIZARD, 4)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      MENU_ITEM_F(ICON_MoveZ0, "Move Z to Home", onDrawMenuItem, setMoveZto0);
      EDIT_ITEM(ICON_Zoffset, MSG_ZPROBE_ZOFFSET, onDrawPFloat2Menu, setZOffset, &BABY_Z_VAR);
    }
    updateMenu(zOffsetWizMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    if (!axis_is_trusted(Z_AXIS)) LCD_MESSAGE_F("WARNING: Z position unknown, move Z to home");
  }

#endif

#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)

<<<<<<< HEAD
  void Draw_Homing_Menu() {
    checkkey = Menu;
    if (SET_MENU(HomingMenu, MSG_HOMING, 6)) {
      BACK_ITEM(Draw_Prepare_Menu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      #if HAS_X_AXIS
        MENU_ITEM(ICON_HomeX, MSG_AUTO_HOME_X, onDrawMenuItem, HomeX);
      #endif
      #if HAS_Y_AXIS
        MENU_ITEM(ICON_HomeY, MSG_AUTO_HOME_Y, onDrawMenuItem, HomeY);
      #endif
      #if HAS_Z_AXIS
        MENU_ITEM(ICON_HomeZ, MSG_AUTO_HOME_Z, onDrawMenuItem, HomeZ);
      #endif
      #if ENABLED(MESH_BED_LEVELING)
        EDIT_ITEM(ICON_ZAfterHome, MSG_Z_AFTER_HOME, onDrawPInt8Menu, SetZAfterHoming, &HMI_data.z_after_homing);
      #endif
    }
    UpdateMenu(HomingMenu);
  }

#endif

#if ENABLED(FWRETRACT)

  void Draw_FWRetract_Menu() {
    checkkey = Menu;
    if (SET_MENU(FWRetractMenu, MSG_FWRETRACT, 6)) {
      BACK_ITEM(ReturnToPreviousMenu);
      EDIT_ITEM(ICON_FWRetLength, MSG_CONTROL_RETRACT, onDrawPFloatMenu, SetRetractLength, &fwretract.settings.retract_length);
      EDIT_ITEM(ICON_FWRetSpeed, MSG_SINGLENOZZLE_RETRACT_SPEED, onDrawPFloatMenu, SetRetractSpeed, &fwretract.settings.retract_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRetZRaise, MSG_CONTROL_RETRACT_ZHOP, onDrawPFloat2Menu, SetZRaise, &fwretract.settings.retract_zraise);
      EDIT_ITEM(ICON_FWRecSpeed, MSG_SINGLENOZZLE_UNRETRACT_SPEED, onDrawPFloatMenu, SetRecoverSpeed, &fwretract.settings.retract_recover_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRecExtra, MSG_CONTROL_RETRACT_RECOVER, onDrawPFloatMenu, SetAddRecover, &fwretract.settings.retract_recover_extra);
    }
    UpdateMenu(FWRetractMenu);
=======
  void drawHomingMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(homingMenu, MSG_HOMING, 6)) {
      BACK_ITEM(drawPrepareMenu);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      #if HAS_X_AXIS
        MENU_ITEM(ICON_HomeX, MSG_AUTO_HOME_X, onDrawMenuItem, homeX);
      #endif
      #if HAS_Y_AXIS
        MENU_ITEM(ICON_HomeY, MSG_AUTO_HOME_Y, onDrawMenuItem, homeY);
      #endif
      #if HAS_Z_AXIS
        MENU_ITEM(ICON_HomeZ, MSG_AUTO_HOME_Z, onDrawMenuItem, homeZ);
      #endif
      #if ENABLED(MESH_BED_LEVELING)
        EDIT_ITEM(ICON_ZAfterHome, MSG_Z_AFTER_HOME, onDrawPInt8Menu, setZAfterHoming, &hmiData.zAfterHoming);
      #endif
    }
    updateMenu(homingMenu);
  }

#endif // INDIVIDUAL_AXIS_HOMING_SUBMENU

#if ENABLED(FWRETRACT)

  void drawFWRetractMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(fwRetractMenu, MSG_FWRETRACT, 6)) {
      BACK_ITEM(returnToPreviousMenu);
      EDIT_ITEM(ICON_FWRetLength, MSG_CONTROL_RETRACT, onDrawPFloatMenu, setRetractLength, &fwretract.settings.retract_length);
      EDIT_ITEM(ICON_FWRetSpeed, MSG_SINGLENOZZLE_RETRACT_SPEED, onDrawPFloatMenu, setRetractSpeed, &fwretract.settings.retract_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRetZRaise, MSG_CONTROL_RETRACT_ZHOP, onDrawPFloat2Menu, setZRaise, &fwretract.settings.retract_zraise);
      EDIT_ITEM(ICON_FWRecSpeed, MSG_SINGLENOZZLE_UNRETRACT_SPEED, onDrawPFloatMenu, setRecoverSpeed, &fwretract.settings.retract_recover_feedrate_mm_s);
      EDIT_ITEM(ICON_FWRecExtra, MSG_CONTROL_RETRACT_RECOVER, onDrawPFloatMenu, setAddRecover, &fwretract.settings.retract_recover_extra);
    }
    updateMenu(fwRetractMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif

//=============================================================================
// Mesh Bed Leveling
//=============================================================================

#if HAS_MESH

  #if PROUI_EX
<<<<<<< HEAD
    void ApplyMeshPoints() { ProEx.ApplyMeshPoints(); ReDrawMenu(); }
    void LiveMeshPoints() { ProEx.DrawMeshPoints(true, CurrentMenu->line(), MenuData.Value); }
    void SetMeshPoints() {
      SetOnClick(SetIntNoDraw, GRID_MIN, GRID_LIMIT, 0, PRO_data.grid_max_points, ApplyMeshPoints, LiveMeshPoints);
      ProEx.DrawMeshPoints(true, CurrentMenu->line(), PRO_data.grid_max_points);
    }
    void ApplyMeshLimits() { ProEx.CheckMeshInsets(); ProEx.ApplyMeshLimits(); ReDrawItem(); }
    void SetMeshInset() { SetPFloatOnClick(MIN_MESH_INSET, MAX_MESH_INSET, UNITFDIGITS, ApplyMeshLimits); }
    void MaxMeshArea() {
=======
    void applyMeshPoints() { proUIEx.applyMeshPoints(); redrawMenu(); }
    void liveMeshPoints() { proUIEx.drawMeshPoints(true, currentMenu->line(), menuData.value); }
    void setMeshPoints() {
      setOnClick(ID_SetIntNoDraw, GRID_MIN, GRID_LIMIT, 0, PRO_data.grid_max_points, applyMeshPoints, liveMeshPoints);
      proUIEx.drawMeshPoints(true, currentMenu->line(), PRO_data.grid_max_points);
    }
    void applyMeshLimits() { proUIEx.checkMeshInsets(); proUIEx.applyMeshLimits(); redrawItem(); }
    void setMeshInset() { setPFloatOnClick(MIN_MESH_INSET, MAX_MESH_INSET, UNITFDIGITS, applyMeshLimits); }
    void maxMeshArea() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      #if HAS_BED_PROBE
        PRO_data.mesh_min_x = _MAX(PROBING_MARGIN_LEFT, probe.offset.x);
        PRO_data.mesh_max_x = _MIN(X_BED_SIZE - PROBING_MARGIN_RIGHT, X_MAX_POS + probe.offset.x);
        PRO_data.mesh_min_y = _MAX(PROBING_MARGIN_FRONT, probe.offset.y);
        PRO_data.mesh_max_y = _MIN(Y_BED_SIZE - PROBING_MARGIN_BACK, Y_MAX_POS + probe.offset.y);
      #else
        PRO_data.mesh_min_x = PROBING_MARGIN_LEFT;
        PRO_data.mesh_max_x = X_BED_SIZE - PROBING_MARGIN_RIGHT;
        PRO_data.mesh_min_y = PROBING_MARGIN_FRONT;
        PRO_data.mesh_max_y = Y_BED_SIZE - PROBING_MARGIN_BACK;
      #endif
<<<<<<< HEAD
      ProEx.ApplyMeshLimits();
      ReDrawMenu();
    }
    void CenterMeshArea() {
=======
      proUIEx.applyMeshLimits();
      redrawMenu();
    }
    void centerMeshArea() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      float max = PRO_data.mesh_min_x;
      if (max < X_BED_SIZE - PRO_data.mesh_max_x) max = X_BED_SIZE - PRO_data.mesh_max_x;
      if (max < PRO_data.mesh_min_y) max = PRO_data.mesh_min_y;
      if (max < Y_BED_SIZE - PRO_data.mesh_max_y) max = Y_BED_SIZE - PRO_data.mesh_max_y;
      PRO_data.mesh_min_x = max;
      PRO_data.mesh_max_x = X_BED_SIZE - max;
      PRO_data.mesh_min_y = max;
      PRO_data.mesh_max_y = Y_BED_SIZE - max;
<<<<<<< HEAD
      ProEx.ApplyMeshLimits();
      ReDrawMenu();
    }
  #endif

  void ApplyMeshFadeHeight() { set_z_fade_height(planner.z_fade_height); }
  void SetMeshFadeHeight() { SetPFloatOnClick(0, 100, 1, ApplyMeshFadeHeight); }

  #if ENABLED(ACTIVATE_MESH_ITEM)

    void SetMeshActive() {
=======
      proUIEx.applyMeshLimits();
      redrawMenu();
    }
  #endif

  void applyMeshFadeHeight() { set_z_fade_height(planner.z_fade_height); }
  void setMeshFadeHeight() { setPFloatOnClick(0, 100, 1, applyMeshFadeHeight); }

  #if ENABLED(ACTIVATE_MESH_ITEM)

    void setMeshActive() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
      const bool val = planner.leveling_active;
      set_bed_leveling_enabled(!planner.leveling_active);
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        if (!val) {
          if (planner.leveling_active && bedlevel.storage_slot >= 0)
            ui.status_printf(0, GET_TEXT_F(MSG_MESH_ACTIVE), bedlevel.storage_slot);
          else
            ui.set_status(GET_TEXT_F(MSG_UBL_MESH_INVALID));
        }
        else ui.reset_status(true);
      #else
        UNUSED(val);
      #endif
<<<<<<< HEAD
      Show_Chkb_Line(planner.leveling_active);
=======
      showCheckboxLine(planner.leveling_active);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }

  #endif

<<<<<<< HEAD
  #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    void SetBedLevT() { SetPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    bool AutoMovToMesh = false;
    #define Z_OFFSET_MIN -3
    #define Z_OFFSET_MAX  3

    void LiveEditMesh() { ((MenuItemPtrClass*)EditZValueItem)->value = &bedlevel.z_values[HMI_value.Select ? bedLevelTools.mesh_x : MenuData.Value][HMI_value.Select ? MenuData.Value : bedLevelTools.mesh_y]; EditZValueItem->redraw(); }
    void LiveEditMeshZ() { *MenuData.P_Float = MenuData.Value / POW(10, 2); if (AutoMovToMesh) bedLevelTools.MoveToZ(); }
    void ApplyEditMeshX() { bedLevelTools.mesh_x = MenuData.Value; if (AutoMovToMesh) bedLevelTools.MoveToXY(); }
    void ApplyEditMeshY() { bedLevelTools.mesh_y = MenuData.Value; if (AutoMovToMesh) bedLevelTools.MoveToXY(); }

    void ResetMesh() { bedLevelTools.mesh_reset(); EditZValueItem->redraw(); LCD_MESSAGE(MSG_MESH_RESET); }
    void SetEditMeshX() { HMI_value.Select = 0; SetIntOnClick(0, GRID_MAX_POINTS_X - 1, bedLevelTools.mesh_x, ApplyEditMeshX, LiveEditMesh); }
    void SetEditMeshY() { HMI_value.Select = 1; SetIntOnClick(0, GRID_MAX_POINTS_Y - 1, bedLevelTools.mesh_y, ApplyEditMeshY, LiveEditMesh); }
    void SetEditZValue() { SetPFloatOnClick(Z_OFFSET_MIN, Z_OFFSET_MAX, 2, nullptr, LiveEditMeshZ); if (AutoMovToMesh) bedLevelTools.MoveToXYZ(); }

    void SetAutoMovToMesh() {
      Toggle_Chkb_Line(AutoMovToMesh);
=======
  #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
    void setBedLevT() { setPIntOnClick(MIN_BEDTEMP, MAX_BEDTEMP); }
  #endif

  #if ENABLED(MESH_EDIT_MENU)
    bool autoMovToMesh = false;
    #define Z_OFFSET_MIN -3
    #define Z_OFFSET_MAX  3

    void liveEditMesh() { ((MenuItemPtr*)editZValueItem)->value = &bedlevel.z_values[hmiValue.select ? bedLevelTools.mesh_x : menuData.value][hmiValue.select ? menuData.value : bedLevelTools.mesh_y]; editZValueItem->redraw(); }
    void liveEditMeshZ() { *menuData.floatPtr = menuData.value / POW(10, 2); if (autoMovToMesh) bedLevelTools.moveToZ(); }
    void applyEditMeshX() { bedLevelTools.mesh_x = menuData.value; if (autoMovToMesh) bedLevelTools.moveToXY(); }
    void applyEditMeshY() { bedLevelTools.mesh_y = menuData.value; if (autoMovToMesh) bedLevelTools.moveToXY(); }

    void resetMesh() { bedLevelTools.meshReset(); LCD_MESSAGE(MSG_MESH_RESET); }
    void setEditMeshX() { hmiValue.select = 0; setIntOnClick(0, GRID_MAX_POINTS_X - 1, bedLevelTools.mesh_x, applyEditMeshX, liveEditMesh); }
    void setEditMeshY() { hmiValue.select = 1; setIntOnClick(0, GRID_MAX_POINTS_Y - 1, bedLevelTools.mesh_y, applyEditMeshY, liveEditMesh); }
    void setEditZValue() { setPFloatOnClick(Z_OFFSET_MIN, Z_OFFSET_MAX, 2, nullptr, liveEditMeshZ); if (autoMovToMesh) bedLevelTools.moveToXYZ(); }

    void setAutoMovToMesh() {
      toggleCheckboxLine(autoMovToMesh);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif

#endif // HAS_MESH

#if ENABLED(AUTO_BED_LEVELING_UBL)

<<<<<<< HEAD
  void ApplyUBLSlot() { bedlevel.storage_slot = MenuData.Value; }
  void SetUBLSlot() { SetIntOnClick(0, settings.calc_num_meshes() - 1, bedlevel.storage_slot, ApplyUBLSlot); }
  void onDrawUBLSlot(MenuItemClass* menuitem, int8_t line) {
=======
  void applyUBLSlot() { bedlevel.storage_slot = menuData.value; }
  void setUBLSlot() { setIntOnClick(0, settings.calc_num_meshes() - 1, bedlevel.storage_slot, applyUBLSlot); }
  void onDrawUBLSlot(MenuItem* menuitem, int8_t line) {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    NOLESS(bedlevel.storage_slot, 0);
    onDrawIntMenu(menuitem, line, bedlevel.storage_slot);
  }

<<<<<<< HEAD
  void ApplyUBLTiltGrid() { bedLevelTools.tilt_grid = MenuData.Value; }
  void SetUBLTiltGrid() { SetIntOnClick(1, 3, bedLevelTools.tilt_grid, ApplyUBLTiltGrid); }

  void UBLMeshTilt() {
    NOLESS(bedlevel.storage_slot, 0);
    char buf[9];
    sprintf_P(buf, PSTR("G29J%i"), bedLevelTools.tilt_grid > 1 ? bedLevelTools.tilt_grid : 0);
    gcode.process_subcommands_now(buf);
    LCD_MESSAGE(MSG_UBL_MESH_TILTED);
  }

  void UBLSmartFillMesh() {
    LOOP_L_N(x, TERN(PROUI_EX, GRID_LIMIT, GRID_MAX_POINTS_X)) bedlevel.smart_fill_mesh();
    LCD_MESSAGE(MSG_UBL_MESH_FILLED);
  }

  void UBLMeshSave() {
=======
  void applyUBLTiltGrid() { bedLevelTools.tilt_grid = menuData.value; }
  void setUBLTiltGrid() { setIntOnClick(1, 3, bedLevelTools.tilt_grid, applyUBLTiltGrid); }

  void ublMeshTilt() {
    NOLESS(bedlevel.storage_slot, 0);
    gcode.process_subcommands_now(TS(F("G29J"), bedLevelTools.tilt_grid > 1 ? bedLevelTools.tilt_grid : 0));
    LCD_MESSAGE(MSG_UBL_MESH_TILTED);
  }

  void ublSmartFillMesh() {
    for (uint8_t i = 0; i < TERN(PROUI_EX, GRID_LIMIT, GRID_MAX_POINTS_X); ++i) bedlevel.smart_fill_mesh();
    LCD_MESSAGE(MSG_UBL_MESH_FILLED);
  }

  void ublMeshSave() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    NOLESS(bedlevel.storage_slot, 0);
    settings.store_mesh(bedlevel.storage_slot);
    ui.status_printf(0, GET_TEXT_F(MSG_MESH_SAVED), bedlevel.storage_slot);
    DONE_BUZZ(true);
  }

<<<<<<< HEAD
  void UBLMeshLoad() {
=======
  void ublMeshLoad() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    NOLESS(bedlevel.storage_slot, 0);
    settings.load_mesh(bedlevel.storage_slot);
  }

<<<<<<< HEAD
#endif  // AUTO_BED_LEVELING_UBL

#if HAS_MESH
  void Draw_MeshSet_Menu() {
    checkkey = Menu;
    if (SET_MENU(MeshMenu, MSG_MESH_LEVELING, 15)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      #if PROUI_EX
        MENU_ITEM(ICON_MeshPoints, MSG_MESH_POINTS, onDrawMeshPoints, SetMeshPoints);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_INSET, onDrawSubMenu, Draw_MeshInset_Menu);
      #endif
      #if BOTH(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
        EDIT_ITEM(ICON_Temperature, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, SetBedLevT, &HMI_data.BedLevT);
      #endif
      EDIT_ITEM(ICON_SetZOffset, MSG_Z_FADE_HEIGHT, onDrawPFloatMenu, SetMeshFadeHeight, &planner.z_fade_height);
      #if ENABLED(ACTIVATE_MESH_ITEM)
        EDIT_ITEM(ICON_UBLActive, MSG_ACTIVATE_MESH, onDrawChkbMenu, SetMeshActive, &planner.leveling_active);
      #endif
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, AutoLev);
      #endif
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EDIT_ITEM(ICON_UBLSlot, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, SetUBLSlot, &bedlevel.storage_slot);
        MENU_ITEM(ICON_UBLMeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, UBLMeshSave);
        MENU_ITEM(ICON_UBLMeshLoad, MSG_UBL_LOAD_MESH, onDrawMenuItem, UBLMeshLoad);
        EDIT_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILTING_GRID, onDrawPInt8Menu, SetUBLTiltGrid, &bedLevelTools.tilt_grid);
        MENU_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILT_MESH, onDrawMenuItem, UBLMeshTilt);
        MENU_ITEM(ICON_UBLSmartFill, MSG_UBL_SMART_FILLIN, onDrawMenuItem, UBLSmartFillMesh);
      #endif
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_MeshEdit, MSG_EDIT_MESH, onDrawSubMenu, Draw_EditMesh_Menu);
      #endif
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, DWIN_MeshViewer);
    }
    UpdateMenu(MeshMenu);
  }

  #if ENABLED(MESH_EDIT_MENU)
    void Draw_EditMesh_Menu() {
      if (!leveling_is_valid()) { LCD_MESSAGE(MSG_UBL_MESH_INVALID); return; }
      set_bed_leveling_enabled(false);
      checkkey = Menu;
      if (SET_MENU(EditMeshMenu, MSG_EDIT_MESH, 7)) {
        bedLevelTools.mesh_x = bedLevelTools.mesh_y = 0;
        BACK_ITEM(Draw_MeshSet_Menu);
        MENU_ITEM(ICON_MeshReset, MSG_MESH_RESET, onDrawMenuItem, ResetMesh);
        EDIT_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_MOVING, onDrawChkbMenu, SetAutoMovToMesh, &AutoMovToMesh);
        EDIT_ITEM(ICON_MeshEditX, MSG_MESH_X, onDrawPInt8Menu, SetEditMeshX, &bedLevelTools.mesh_x);
        EDIT_ITEM(ICON_MeshEditY, MSG_MESH_Y, onDrawPInt8Menu, SetEditMeshY, &bedLevelTools.mesh_y);
        EditZValueItem = EDIT_ITEM(ICON_MeshEditZ, MSG_MESH_EDIT_Z, onDrawPFloat2Menu, SetEditZValue, &bedlevel.z_values[bedLevelTools.mesh_x][bedLevelTools.mesh_y]);
        TERN_(HAS_BED_PROBE, MENU_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_PROBING, onDrawMenuItem, bedLevelTools.ProbeXY));
      }
      UpdateMenu(EditMeshMenu);
=======
#endif // AUTO_BED_LEVELING_UBL

#if HAS_MESH
  void drawMeshSetMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(meshMenu, MSG_MESH_LEVELING, 16)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      #if PROUI_EX
        MENU_ITEM(ICON_MeshPoints, MSG_MESH_POINTS, onDrawMeshPoints, setMeshPoints);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_INSET, onDrawSubMenu, drawMeshInsetMenu);
      #endif
      #if ALL(HAS_HEATED_BED, PREHEAT_BEFORE_LEVELING)
        EDIT_ITEM(ICON_Temperature, MSG_UBL_SET_TEMP_BED, onDrawPIntMenu, setBedLevT, &hmiData.bedLevT);
      #endif
      EDIT_ITEM(ICON_SetZOffset, MSG_Z_FADE_HEIGHT, onDrawPFloatMenu, setMeshFadeHeight, &planner.z_fade_height);
      #if ENABLED(ACTIVATE_MESH_ITEM)
        EDIT_ITEM(ICON_UBLActive, MSG_ACTIVATE_MESH, onDrawChkbMenu, setMeshActive, &planner.leveling_active);
      #endif
      #if HAS_BED_PROBE
        MENU_ITEM(ICON_Level, MSG_AUTO_MESH, onDrawMenuItem, autoLev);
      #endif
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        EDIT_ITEM(ICON_UBLSlot, MSG_UBL_STORAGE_SLOT, onDrawUBLSlot, setUBLSlot, &bedlevel.storage_slot);
        MENU_ITEM(ICON_UBLMeshSave, MSG_UBL_SAVE_MESH, onDrawMenuItem, ublMeshSave);
        MENU_ITEM(ICON_UBLMeshLoad, MSG_UBL_LOAD_MESH, onDrawMenuItem, ublMeshLoad);
        EDIT_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILTING_GRID, onDrawPInt8Menu, setUBLTiltGrid, &bedLevelTools.tilt_grid);
        MENU_ITEM(ICON_UBLTiltGrid, MSG_UBL_TILT_MESH, onDrawMenuItem, ublMeshTilt);
        MENU_ITEM(ICON_UBLSmartFill, MSG_UBL_SMART_FILLIN, onDrawMenuItem, ublSmartFillMesh);
      #endif
      #if ENABLED(MESH_EDIT_MENU)
        MENU_ITEM(ICON_MeshReset, MSG_MESH_RESET, onDrawMenuItem, resetMesh);
        MENU_ITEM(ICON_MeshEdit, MSG_EDIT_MESH, onDrawSubMenu, drawEditMeshMenu);
      #endif
      MENU_ITEM(ICON_MeshViewer, MSG_MESH_VIEW, onDrawSubMenu, dwinMeshViewer);
    }
    updateMenu(meshMenu);
  }

  #if ENABLED(MESH_EDIT_MENU)
    void drawEditMeshMenu() {
      if (!leveling_is_valid()) { LCD_MESSAGE(MSG_UBL_MESH_INVALID); return; }
      set_bed_leveling_enabled(false);
      checkkey = ID_Menu;
      if (SET_MENU(editMeshMenu, MSG_EDIT_MESH, 6)) {
        bedLevelTools.mesh_x = bedLevelTools.mesh_y = 0;
        BACK_ITEM(drawMeshSetMenu);
        EDIT_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_MOVING, onDrawChkbMenu, setAutoMovToMesh, &autoMovToMesh);
        EDIT_ITEM(ICON_MeshEditX, MSG_MESH_X, onDrawPInt8Menu, setEditMeshX, &bedLevelTools.mesh_x);
        EDIT_ITEM(ICON_MeshEditY, MSG_MESH_Y, onDrawPInt8Menu, setEditMeshY, &bedLevelTools.mesh_y);
        editZValueItem = EDIT_ITEM(ICON_MeshEditZ, MSG_MESH_EDIT_Z, onDrawPFloat2Menu, setEditZValue, &bedlevel.z_values[bedLevelTools.mesh_x][bedLevelTools.mesh_y]);
        TERN_(HAS_BED_PROBE, MENU_ITEM(ICON_UBLActive, MSG_PROBE_WIZARD_PROBING, onDrawMenuItem, bedLevelTools.probeXY));
      }
      updateMenu(editMeshMenu);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    }
  #endif

  #if PROUI_EX
<<<<<<< HEAD
    void Draw_MeshInset_Menu() {
      checkkey = Menu;
      if (SET_MENU(MeshInsetMenu, MSG_MESH_INSET, 7)) {
        BACK_ITEM(Draw_MeshSet_Menu);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_X, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_min_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_X, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_max_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_Y, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_min_y);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_Y, onDrawPFloatMenu, SetMeshInset, &PRO_data.mesh_max_y);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_AMAX, onDrawMenuItem, MaxMeshArea);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_CENTER, onDrawMenuItem, CenterMeshArea);
      }
      UpdateMenu(MeshInsetMenu);
    }
  #endif

#endif  // HAS_MESH
=======
    void drawMeshInsetMenu() {
      checkkey = ID_Menu;
      if (SET_MENU(meshInsetMenu, MSG_MESH_INSET, 7)) {
        BACK_ITEM(drawMeshSetMenu);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_X, onDrawPFloatMenu, setMeshInset, &PRO_data.mesh_min_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_X, onDrawPFloatMenu, setMeshInset, &PRO_data.mesh_max_x);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MIN_Y, onDrawPFloatMenu, setMeshInset, &PRO_data.mesh_min_y);
        EDIT_ITEM(ICON_ProbeMargin, MSG_MESH_MAX_Y, onDrawPFloatMenu, setMeshInset, &PRO_data.mesh_max_y);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_AMAX, onDrawMenuItem, maxMeshArea);
        MENU_ITEM(ICON_ProbeMargin, MSG_MESH_CENTER, onDrawMenuItem, centerMeshArea);
      }
      updateMenu(meshInsetMenu);
    }
  #endif

#endif // HAS_MESH
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

//=============================================================================
// CV Laser Module support
//=============================================================================
#if ENABLED(CV_LASER_MODULE)

  // Make the current position 0,0,0
<<<<<<< HEAD
  void SetHome() {
    laser_device.homepos += current_position;
    set_all_homed();
    gcode.process_subcommands_now(F("G92X0Y0Z0"));
    ReDrawMenu();
  }

  void LaserOn(const bool turn_on) {
    laser_device.laser_set(turn_on);
    DWIN_Draw_Dashboard();
  }

  void LaserToggle() {
    LaserOn(!laser_device.is_laser_device());
    Show_Chkb_Line(laser_device.is_laser_device());
  }

  void LaserPrint() {
=======
  void setHome() {
    laser_device.homepos += current_position;
    set_all_homed();
    gcode.process_subcommands_now(F("G92X0Y0Z0"));
    redrawMenu();
  }

  void laserOn(const bool turn_on) {
    laser_device.laser_set(turn_on);
    dwinDrawDashboard();
  }

  void laserToggle() {
    laserOn(!laser_device.is_laser_device());
    showCheckboxLine(laser_device.is_laser_device());
  }

  void laserPrint() {
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
    if (!laser_device.is_laser_device()) return;
    thermalManager.disable_all_heaters(); // 关闭加热107011 -20211012
    print_job_timer.reset();  //107011 -20211009 清除前一次的打印时间
    laser_device.laser_power_open(); // 打开激光, 以最弱的激光输出。专业固件是最好的
    // queue.inject_P(PSTR("M999\nG92.9Z0")); // 107011 -20211013
    card.openAndPrintFile(card.filename);
  }

<<<<<<< HEAD
  void LaserRunRange() {
    if (!laser_device.is_laser_device()) return;
    if (!all_axes_trusted()) return LCD_MESSAGE_F("First set home");
    DWIN_Show_Popup(ICON_TempTooHigh, "LASER", "Run Range", BTN_Cancel);
    HMI_SaveProcessID(WaitResponse);
    laser_device.laser_range();
  }

  void Draw_LaserSettings_Menu() {
    EnableLiveMove = true;
    checkkey = Menu;
    if (SET_MENU(LaserSettings, MSG_LASER_MENU, 7)) {
      BACK_ITEM(ReturnToPreviousMenu);
      EDIT_ITEM(ICON_LaserToggle, MSG_LASER_TOGGLE, onDrawChkbMenu, LaserToggle, &laser_device.laser_enabled);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, AutoHome);
      EDIT_ITEM_F(ICON_LaserFocus, "Laser Focus", onDrawPFloatMenu, SetMoveZ, &current_position.z);
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, SetMoveX, &current_position.x);
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, SetMoveY, &current_position.y);
      MENU_ITEM_F(ICON_SetHome, "Set as Home", onDrawMenuItem, SetHome);
    }
    UpdateMenu(LaserSettings);
  }

  void Draw_LaserPrint_Menu() {
    if (!laser_device.is_laser_device()) return Goto_Main_Menu();
    checkkey = Menu;
    if (SET_MENU(LaserPrintMenu, MSG_LASER_MENU, 4)) {
      BACK_ITEM(Draw_Print_File_Menu);
      MENU_ITEM(ICON_SetHome, MSG_CONFIGURATION, onDrawSubMenu, Draw_LaserSettings_Menu);
      MENU_ITEM_F(ICON_LaserPrint, "Engrave", onDrawMenuItem, LaserPrint);
      MENU_ITEM_F(ICON_LaserRunRange, "Run Range", onDrawMenuItem, LaserRunRange);
    }
    UpdateMenu(LaserPrintMenu);
    char buf[23], str_1[5], str_2[5];
    sprintf_P(buf, PSTR("XMIN: %s XMAX: %s"), dtostrf(LASER_XMIN, 1, 1, str_1), dtostrf(LASER_XMAX, 1, 1, str_2));
    DWINUI::Draw_String(LBLX, MBASE(4) + 10, buf);
    sprintf_P(buf, PSTR("YMIN: %s YMAX: %s"), dtostrf(LASER_YMIN, 1, 1, str_1), dtostrf(LASER_YMAX, 1, 1, str_2));
    DWINUI::Draw_String(LBLX, MBASE(5) - 10, buf);
=======
  void laserRunRange() {
    if (!laser_device.is_laser_device()) return;
    if (!all_axes_trusted()) return LCD_MESSAGE_F("First set home");
    dwinShowPopup(ICON_TempTooHigh, "LASER", "Run Range", BTN_Cancel);
    hmiSaveProcessID(ID_WaitResponse);
    laser_device.laser_range();
  }

  void drawLaserSettingsMenu() {
    enableLiveMove = true;
    checkkey = ID_Menu;
    if (SET_MENU(laserSettings, MSG_LASER_MENU, 7)) {
      BACK_ITEM(returnToPreviousMenu);
      EDIT_ITEM(ICON_LaserToggle, MSG_LASER_TOGGLE, onDrawChkbMenu, laserToggle, &laser_device.laser_enabled);
      MENU_ITEM(ICON_Homing, MSG_AUTO_HOME, onDrawMenuItem, autoHome);
      EDIT_ITEM_F(ICON_LaserFocus, "Laser Focus", onDrawPFloatMenu, setMoveZ, &current_position.z);
      EDIT_ITEM(ICON_MoveX, MSG_MOVE_X, onDrawPFloatMenu, setMoveX, &current_position.x);
      EDIT_ITEM(ICON_MoveY, MSG_MOVE_Y, onDrawPFloatMenu, setMoveY, &current_position.y);
      MENU_ITEM_F(ICON_SetHome, "Set as Home", onDrawMenuItem, setHome);
    }
    updateMenu(laserSettings);
  }

  void drawLaserPrintMenu() {
    if (!laser_device.is_laser_device()) return gotoMainMenu();
    checkkey = ID_Menu;
    if (SET_MENU(laserPrintMenu, MSG_LASER_MENU, 4)) {
      BACK_ITEM(drawPrintFileMenu);
      MENU_ITEM(ICON_SetHome, MSG_CONFIGURATION, onDrawSubMenu, drawLaserSettingsMenu);
      MENU_ITEM_F(ICON_LaserPrint, "Engrave", onDrawMenuItem, laserPrint);
      MENU_ITEM_F(ICON_LaserRunRange, "Run Range", onDrawMenuItem, laserRunRange);
    }
    updateMenu(laserPrintMenu);
    char buf[23], str_1[5], str_2[5];
    sprintf_P(buf, PSTR("XMIN: %s XMAX: %s"), dtostrf(LASER_XMIN, 1, 1, str_1), dtostrf(LASER_XMAX, 1, 1, str_2));
    DWINUI::drawString(LBLX, MBASE(4) + 10, buf);
    sprintf_P(buf, PSTR("YMIN: %s YMAX: %s"), dtostrf(LASER_YMIN, 1, 1, str_1), dtostrf(LASER_YMAX, 1, 1, str_2));
    DWINUI::drawString(LBLX, MBASE(5) - 10, buf);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif // CV_LASER_MODULE

//=============================================================================
<<<<<<< HEAD
// ToolBar
//=============================================================================
#if HAS_TOOLBAR

  void Draw_TBSetup_Menu() {
    checkkey = Menu;
    if (SET_MENU(TBSetupMenu, MSG_TOOLBAR_SETUP, TBMaxOpt + 1)) {
      BACK_ITEM(Draw_AdvancedSettings_Menu);
      LOOP_L_N(i, TBMaxOpt) EDIT_ITEM_F(0, "", onDrawTBSetupItem, SetTBSetupItem, &PRO_data.TBopt[i]);
    }
    UpdateMenu(TBSetupMenu);
  }

  void Exit_ToolBar() {
    select_page.set(PAGE_ADVANCE);
    ICON_AdvSettings();
    checkkey = MainMenu;
    ToolBar.draw();
    DWIN_UpdateLCD();
  }

  void Goto_ToolBar() {
    checkkey = Menu;
    ToolBar.draw();
=======
// toolBar
//=============================================================================
#if HAS_TOOLBAR

  void drawTBSetupMenu() {
    checkkey = ID_Menu;
    if (SET_MENU(tbSetupMenu, MSG_TOOLBAR_SETUP, TBMaxOpt + 1)) {
      BACK_ITEM(drawAdvancedSettingsMenu);
      for (uint8_t i = 0; i < TBMaxOpt; ++i) EDIT_ITEM_F(0, "", onDrawTBSetupItem, setTBSetupItem, &PRO_data.TBopt[i]);
    }
    updateMenu(tbSetupMenu);
  }

  void exitToolBar() {
    select_page.set(PAGE_ADVANCE);
    iconAdvSettings();
    checkkey = ID_MainMenu;
    toolBar.draw();
    dwinUpdateLCD();
  }

  void gotoToolBar() {
    checkkey = ID_Menu;
    toolBar.draw();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

#endif  // HAS_TOOLBAR

//=============================================================================
// More Host support
//=============================================================================

#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)

<<<<<<< HEAD
  void PopUp_HostShutDown() { DWIN_Popup_ConfirmCancel(ICON_Info_1, GET_TEXT_F(MSG_HOST_SHUTDOWN)); }
  void onClick_HostShutDown() {
    if (HMI_flag.select_flag) hostui.shutdown();
    HMI_ReturnScreen();
  }
  void HostShutDown() { Goto_Popup(PopUp_HostShutDown, onClick_HostShutDown); }
=======
  void popUpHostShutDown() { dwinPopupConfirmCancel(ICON_Info_1, GET_TEXT_F(MSG_HOST_SHUTDOWN)); }
  void onClickHostShutDown() {
    if (hmiFlag.select_flag) hostui.shutdown();
    hmiReturnScreen();
  }
  void hostShutDown() { gotoPopup(popUpHostShutDown, onClickHostShutDown); }
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

#endif

//=============================================================================

#if DEBUG_DWIN
<<<<<<< HEAD
  void DWIN_Debug(const char *msg) {
    SERIAL_ECHOLNPGM_P(msg);
    DWIN_Show_Popup(0, "Debug", msg, BTN_Continue);
    wait_for_user_response();
    Draw_Main_Area();
=======
  void dwinDebug(const char *msg) {
    SERIAL_ECHOLNPGM_P(msg);
    dwinShowPopup(0, "Debug", msg, BTN_Continue);
    wait_for_user_response();
    drawMainArea();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }
#endif

#endif // DWIN_LCD_PROUI
