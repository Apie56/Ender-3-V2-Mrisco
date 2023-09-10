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
#pragma once

#include "../../../inc/MarlinConfig.h"

#include "dwin_defines.h"
#include "dwinui.h"
#include "../common/encoder.h"
#include "../common/limits.h"
#include "../../../libs/BL24CXX.h"

#if HAS_CGCODE
  #include "custom_gcodes.h"
#endif

#if ENABLED(CV_LASER_MODULE)
  #include "cv_laser_module.h"
#endif

<<<<<<< HEAD
=======
#define EXT 0 // default extruder
#define FAN 0 // default fan

>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
namespace GET_LANG(LCD_LANGUAGE) {
  #define _MSG_PREHEAT(N) \
    LSTR MSG_PREHEAT_##N                  = _UxGT("Preheat ") PREHEAT_## N ##_LABEL; \
    LSTR MSG_PREHEAT_## N ##_SETTINGS     = _UxGT("Preheat ") PREHEAT_## N ##_LABEL _UxGT(" Conf");
<<<<<<< HEAD
  #if PREHEAT_COUNT > 3
    REPEAT_S(4, PREHEAT_COUNT, _MSG_PREHEAT)
  #endif
}

extern char DateTime[16+1];

enum processID : uint8_t {
  // Process ID
  MainMenu,
  Menu,
  SetInt,
  SetPInt,
  SetIntNoDraw,
  SetFloat,
  SetPFloat,
  PrintProcess,
  Popup,
  Leveling,
  Locked,
  Reboot,
  PrintDone,
  ESDiagProcess,
  WaitResponse,
  Homing,
  PidProcess,
  MPCProcess,
  NothingToDo
=======
  #if PREHEAT_COUNT > 1
    REPEAT_S(2, INCREMENT(PREHEAT_COUNT), _MSG_PREHEAT)
  #endif
}

extern char dateTime[16+1];

enum processID : uint8_t {
  // Process ID
  ID_MainMenu,
  ID_Menu,
  ID_SetInt,
  ID_SetPInt,
  ID_SetIntNoDraw,
  ID_SetFloat,
  ID_SetPFloat,
  ID_PrintProcess,
  ID_Popup,
  ID_Leveling,
  ID_Locked,
  ID_Reboot,
  ID_PrintDone,
  ID_ESDiagProcess,
  ID_WaitResponse,
  ID_Homing,
  ID_PIDProcess,
  ID_MPCProcess,
  ID_NothingToDo
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
};

#if HAS_PID_HEATING || ENABLED(MPC_AUTOTUNE)
  enum tempcontrol_t : uint8_t {
  #if HAS_PID_HEATING
    PIDTEMP_START = 0,
    PIDTEMPBED_START,
    PID_BAD_HEATER_ID,
    PID_TEMP_TOO_HIGH,
    PID_TUNING_TIMEOUT,
  #endif
  #if ENABLED(MPC_AUTOTUNE)
    MPCTEMP_START,
    MPC_TEMP_ERROR,
    MPC_INTERRUPTED,
  #endif
  AUTOTUNE_DONE
  };
#endif

#define DWIN_CHINESE 123
#define DWIN_ENGLISH 0

typedef struct {
<<<<<<< HEAD
  uint16_t Background_Color;
  uint16_t Cursor_Color;
  uint16_t TitleBg_Color;
  uint16_t TitleTxt_Color;
  uint16_t Text_Color;
  uint16_t Selected_Color;
  uint16_t SplitLine_Color;
  uint16_t Highlight_Color;
  uint16_t StatusBg_Color;
  uint16_t StatusTxt_Color;
  uint16_t PopupBg_Color;
  uint16_t PopupTxt_Color;
  uint16_t AlertBg_Color;
  uint16_t AlertTxt_Color;
  uint16_t PercentTxt_Color;
  uint16_t Barfill_Color;
  uint16_t Indicator_Color;
  uint16_t Coordinate_Color;
  int16_t HotendPidT;
  int16_t BedPidT;
  int16_t PidCycles;
  int16_t ExtMinT;
  int16_t BedLevT;
  bool Baud115K;
  bool FullManualTramming;
  bool MediaSort;
  bool MediaAutoMount;
  uint8_t z_after_homing;
  float ManualZOffset;
  uint32_t Led_Color;
  bool AdaptiveStepSmoothing;
  bool EnablePreview;
} HMI_data_t;

extern HMI_data_t HMI_data;
static constexpr size_t eeprom_data_size = sizeof(HMI_data_t) + TERN0(PROUI_EX, sizeof(PRO_data_t));
=======
  uint16_t colorBackground;
  uint16_t colorCursor;
  uint16_t colorTitleBg;
  uint16_t colorTitleTxt;
  uint16_t colorText;
  uint16_t colorSelected;
  uint16_t colorSplitLine;
  uint16_t colorHighlight;
  uint16_t colorStatusBg;
  uint16_t colorStatusTxt;
  uint16_t colorPopupBg;
  uint16_t colorPopupTxt;
  uint16_t colorAlertBg;
  uint16_t colorAlertTxt;
  uint16_t colorPercentTxt;
  uint16_t colorBarfill;
  uint16_t colorIndicator;
  uint16_t colorCoordinate;
  int16_t hotendPidT;
  int16_t bedPidT;
  int16_t pidCycles;
  int16_t extMinT;
  int16_t bedLevT;
  bool baud115K;
  bool fullManualTramming;
  bool mediaSort;
  bool mediaAutoMount;
  uint8_t zAfterHoming;
  float manualZOffset;
  uint32_t ledColor;
  bool adaptiveStepSmoothing;
  bool enablePreview;
} hmi_data_t;

extern hmi_data_t hmiData;
static constexpr size_t eeprom_data_size = sizeof(hmi_data_t) + TERN0(PROUI_EX, sizeof(PRO_data_t));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

typedef struct {
  int8_t Color[3];                    // Color components
  #if ANY(HAS_PID_HEATING, MPCTEMP)
<<<<<<< HEAD
    tempcontrol_t tempcontrol = AUTOTUNE_DONE;
  #endif
  uint8_t Select          = 0;        // Auxiliary selector variable
  AxisEnum axis           = X_AXIS;   // Axis Select
} HMI_value_t;
=======
    tempcontrol_t tempControl = AUTOTUNE_DONE;
  #endif
  uint8_t select          = 0;        // Auxiliary selector variable
  AxisEnum axis           = X_AXIS;   // Axis Select
} hmi_value_t;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69

typedef struct {
  bool printing_flag:1; // sd or host printing
  bool abort_flag:1;    // sd or host was aborted
  bool pause_flag:1;    // printing is paused
  bool select_flag:1;   // Popup button selected
<<<<<<< HEAD
  bool home_flag:1;     // homing in course
=======
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  bool config_flag:1;   // SD G-code file is a Configuration file
  #if PROUI_EX && HAS_LEVELING
    bool cancel_abl:1;  // cancel current abl
  #endif
<<<<<<< HEAD
} HMI_flag_t;

extern HMI_value_t HMI_value;
extern HMI_flag_t HMI_flag;
=======
} hmi_flag_t;

extern hmi_value_t hmiValue;
extern hmi_flag_t hmiFlag;
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
extern uint8_t checkkey;

// Popups
#if HAS_HOTEND || HAS_HEATED_BED
<<<<<<< HEAD
  void DWIN_Popup_Temperature(const int_fast8_t heater_id, const bool toohigh);
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  void Popup_PowerLossRecovery();
#endif

// Tool Functions
uint32_t GetHash(char * str);
#if ENABLED(EEPROM_SETTINGS)
  void WriteEeprom();
  void ReadEeprom();
  void ResetEeprom();
  #if HAS_MESH
    void SaveMesh();
  #endif
#endif
void RebootPrinter();
void DisableMotors();
void AutoLev();
void AutoHome();
=======
  void dwinPopupTemperature(const int_fast8_t heater_id, const bool toohigh);
#endif
#if ENABLED(POWER_LOSS_RECOVERY)
  void popupPowerLossRecovery();
#endif

// Tool Functions
#if ENABLED(EEPROM_SETTINGS)
  void writeEeprom();
  void readEeprom();
  void resetEeprom();
  #if HAS_MESH
    void saveMesh();
  #endif
#endif
void rebootPrinter();
void disableMotors();
void autoLev();
void autoHome();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#if HAS_PREHEAT
  #define _DOPREHEAT(N) void DoPreheat##N();
  REPEAT_1(PREHEAT_COUNT, _DOPREHEAT)
#endif
<<<<<<< HEAD
void DoCoolDown();
#if ENABLED(BAUD_RATE_GCODE)
  void SetBaud115K();
  void SetBaud250K();
#endif
#if HAS_LCD_BRIGHTNESS
  void TurnOffBacklight();
#endif
void ApplyExtMinT();
void ParkHead();
#if ENABLED(LCD_BED_TRAMMING)
  TERN(HAS_BED_PROBE, float, void) Tram(uint8_t point, bool stow_probe = true);
  #if ENABLED(HAS_BED_PROBE)
    void Trammingwizard();
  #endif
#endif
#if BOTH(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  void ApplyLEDColor();
#endif
#if ENABLED(AUTO_BED_LEVELING_UBL)
  void UBLMeshTilt();
  void UBLMeshSave();
  void UBLMeshLoad();
#endif
#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
  void HostShutDown();
#endif
#if DISABLED(HAS_BED_PROBE)
  void HomeZandDisable();
#endif

// Other
void Goto_PrintProcess();
void Goto_Main_Menu();
void Goto_Info_Menu();
void Goto_PowerLossRecovery();
void Goto_ConfirmToPrint();
void Draw_Main_Area();      // Redraw main area
void DWIN_DrawStatusLine(const char *text = ""); // Draw simple status text
void DWIN_RedrawDash();     // Redraw Dash and Status line
void DWIN_RedrawScreen();   // Redraw all screen elements
void HMI_MainMenu();        // Main process screen
void HMI_Printing();        // Print page
void HMI_ReturnScreen();    // Return to previous screen before popups
void HMI_WaitForUser();
void HMI_SaveProcessID(const uint8_t id);
void HMI_SDCardUpdate();
void EachMomentUpdate();
void update_variable();
void DWIN_InitScreen();
void DWIN_HandleScreen();
void DWIN_CheckStatusMessage();
void DWIN_HomingStart();
void DWIN_HomingDone();
#if HAS_MESH
  void DWIN_MeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval);
#endif
void DWIN_LevelingStart();
void DWIN_LevelingDone();
void DWIN_Print_Started();
void DWIN_Print_Pause();
void DWIN_Print_Resume();
void DWIN_Print_Finished();
void DWIN_Print_Aborted();
#if HAS_FILAMENT_SENSOR
  void DWIN_FilamentRunout(const uint8_t extruder);
#endif
void DWIN_Print_Header(const char *text);
void DWIN_SetColorDefaults();
void DWIN_CopySettingsTo(char * const buff);
void DWIN_CopySettingsFrom(const char * const buff);
void DWIN_SetDataDefaults();
void DWIN_RebootScreen();
inline void DWIN_Gcode(const int16_t codenum) { TERN_(HAS_CGCODE, custom_gcode(codenum)); };

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void DWIN_Popup_Pause(FSTR_P const fmsg, uint8_t button=0);
  void Draw_Popup_FilamentPurge();
  void Goto_FilamentPurge();
=======
void doCoolDown();
#if ENABLED(BAUD_RATE_GCODE)
  void setBaud115K();
  void setBaud250K();
#endif
#if HAS_LCD_BRIGHTNESS
  void turnOffBacklight();
  #if LCD_BACKLIGHT_TIMEOUT_MINS
    void dimBacklight();
  #endif
#endif
void applyExtMinT();
void parkHead();
#if ALL(LED_CONTROL_MENU, HAS_COLOR_LEDS)
  void applyLEDColor();
#endif
#if ENABLED(AUTO_BED_LEVELING_UBL)
  void ublMeshTilt();
  void ublMeshSave();
  void ublMeshLoad();
#endif
#if ENABLED(HOST_SHUTDOWN_MENU_ITEM) && defined(SHUTDOWN_ACTION)
  void hostShutDown();
#endif
#if DISABLED(HAS_BED_PROBE)
  void homeZandDisable();
#endif

// Other
void gotoPrintProcess();
void gotoMainMenu();
void gotoInfoMenu();
void gotoPowerLossRecovery();
void gotoConfirmToPrint();
void drawMainArea();      // Redraw main area
void dwinDrawStatusLine(const char *text); // Draw simple status text
void dwinRedrawDash();     // Redraw Dash and Status line
void dwinRedrawScreen();   // Redraw all screen elements
void hmiMainMenu();        // Main process screen
void hmiPrinting();        // Print page
void hmiReturnScreen();    // Return to previous screen before popups
void hmiWaitForUser();
void hmiSaveProcessID(const uint8_t id);
void hmiSDCardUpdate();
void eachMomentUpdate();
void updateVariable();
void dwinInitScreen();
void dwinHandleScreen();
void dwinCheckStatusMessage();
void dwinHomingStart();
void dwinHomingDone();
#if HAS_MESH
  void dwinMeshUpdate(const int8_t cpos, const int8_t tpos, const_float_t zval);
#endif
void dwinLevelingStart();
void dwinLevelingDone();
void dwinPrintStarted();
void dwinPrintPause();
void dwinPrintResume();
void dwinPrintFinished();
void dwinPrintAborted();
void dwinPrintHeader(const char *text);
void dwinSetColorDefaults();
void dwinCopySettingsTo(char * const buff);
void dwinCopySettingsFrom(const char * const buff);
void dwinSetDataDefaults();
void dwinRebootScreen();
inline void dwinGcode(const int16_t codenum) { TERN_(HAS_CGCODE, customGcode(codenum)); }

#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void dwinPopupPause(FSTR_P const fmsg, uint8_t button=0);
  void drawPopupFilamentPurge();
  void gotoFilamentPurge();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// Utility and extensions
#if HAS_LOCKSCREEN
<<<<<<< HEAD
  void DWIN_LockScreen();
  void DWIN_UnLockScreen();
  void HMI_LockScreen();
#endif
#if HAS_MESH
  void DWIN_MeshViewer();
#endif
#if HAS_ESDIAG
  void Draw_EndStopDiag();
#endif

// Menu drawing functions
void Draw_Print_File_Menu();
void Draw_Control_Menu();
void Draw_AdvancedSettings_Menu();
void Draw_Prepare_Menu();
void Draw_Move_Menu();
void Draw_Tramming_Menu();
#if HAS_HOME_OFFSET
  void Draw_HomeOffset_Menu();
#endif
#if HAS_BED_PROBE
  void Draw_ProbeSet_Menu();
#endif
void Draw_FilSet_Menu();
#if ENABLED(NOZZLE_PARK_FEATURE)
  void Draw_ParkPos_Menu();
#endif
void Draw_PhySet_Menu();
#if BOTH(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
  void Draw_CaseLight_Menu();
#endif
#if ENABLED(LED_CONTROL_MENU)
  void Draw_LedControl_Menu();
#endif
void Draw_Tune_Menu();
void Draw_Motion_Menu();
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void Draw_FilamentMan_Menu();
#endif
#if ENABLED(MESH_BED_LEVELING)
  void Draw_ManualMesh_Menu();
#endif
void Draw_Temperature_Menu();
void Draw_MaxSpeed_Menu();
void Draw_MaxAccel_Menu();
#if HAS_CLASSIC_JERK
  void Draw_MaxJerk_Menu();
#endif
void Draw_Steps_Menu();
#if EITHER(HAS_BED_PROBE, BABYSTEPPING)
  void Draw_ZOffsetWiz_Menu();
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void Draw_Homing_Menu();
#endif
#if ENABLED(FWRETRACT)
  void Draw_FWRetract_Menu();
#endif
#if HAS_MESH
  void Draw_MeshSet_Menu();
  void Draw_MeshInset_Menu();
  #if ENABLED(MESH_EDIT_MENU)
    void Draw_EditMesh_Menu();
  #endif
#endif
#if HAS_TRINAMIC_CONFIG
  void Draw_TrinamicConfig_menu();
=======
  void dwinLockScreen();
  void dwinUnLockScreen();
  void hmiLockScreen();
#endif
#if HAS_MESH
  void dwinMeshViewer();
#endif
#if HAS_ESDIAG
  void drawEndStopDiag();
#endif

// Menu drawing functions
void drawPrintFileMenu();
void drawControlMenu();
void drawAdvancedSettingsMenu();
void drawPrepareMenu();
void drawMoveMenu();

#if ENABLED(LCD_BED_TRAMMING)
void drawTrammingMenu();
#endif

#if HAS_HOME_OFFSET
  void drawHomeOffsetMenu();
#endif
#if HAS_BED_PROBE
  void drawProbeSetMenu();
#endif
void drawFilSetMenu();
#if ENABLED(NOZZLE_PARK_FEATURE)
  void drawParkPosMenu();
#endif
void drawPhySetMenu();
#if ALL(CASE_LIGHT_MENU, CASELIGHT_USES_BRIGHTNESS)
  void drawCaseLightMenu();
#endif
#if ENABLED(LED_CONTROL_MENU)
  void drawLedControlMenu ();
#endif
void drawTuneMenu();
void drawMotionMenu();
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  void drawFilamentManMenu();
#endif
#if ENABLED(MESH_BED_LEVELING)
  void drawManualMeshMenu();
#endif
void drawTemperatureMenu();
void drawMaxSpeedMenu();
void drawMaxAccelMenu();
#if HAS_CLASSIC_JERK
  void drawMaxJerkMenu();
#endif
void drawStepsMenu();
#if ANY(HAS_BED_PROBE, BABYSTEPPING)
  void drawZOffsetWizMenu();
#endif
#if ENABLED(INDIVIDUAL_AXIS_HOMING_SUBMENU)
  void drawHomingMenu();
#endif
#if ENABLED(FWRETRACT)
  void drawFWRetractMenu();
#endif
#if HAS_MESH
  void drawMeshSetMenu();
  #if ENABLED(PROUI_EX)
    void drawMeshInsetMenu();
  #endif
  #if ENABLED(MESH_EDIT_MENU)
    void drawEditMeshMenu();
  #endif
#endif
#if HAS_TRINAMIC_CONFIG
  void drawTrinamicConfigMenu();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// Custom colors editing
#if HAS_CUSTOM_COLORS
<<<<<<< HEAD
  void DWIN_ApplyColor();
  void DWIN_ApplyColor(const int8_t element, const bool ldef=false);
  void Draw_SelectColors_Menu();
  void Draw_GetColor_Menu();
=======
  void dwinApplyColor();
  void dwinApplyColor(const int8_t element, const bool ldef=false);
  void drawSelectColorsMenu();
  void drawGetColorMenu();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// PID
#if HAS_PID_HEATING
  #include "../../../module/temperature.h"
<<<<<<< HEAD
  void DWIN_M303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp);
  void DWIN_PidTuning(tempcontrol_t result);
#endif
#if ENABLED(PIDTEMP)
  #if ENABLED(PID_AUTOTUNE_MENU)
    void HotendPID();
  #endif
  #if EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void Draw_HotendPID_Menu();
=======
  void dwinM303(const bool seenC, const int c, const bool seenS, const heater_id_t hid, const celsius_t temp);
  void dwinPidTuning(tempcontrol_t result);
#endif
#if ENABLED(PIDTEMP)
  #if ENABLED(PID_AUTOTUNE_MENU)
    void hotendPID();
  #endif
  #if ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void drawHotendPIDMenu();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
#endif
#if ENABLED(PIDTEMPBED)
  #if ENABLED(PID_AUTOTUNE_MENU)
<<<<<<< HEAD
    void BedPID();
  #endif
  #if EITHER(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void Draw_BedPID_Menu();
=======
    void bedPID();
  #endif
  #if ANY(PID_AUTOTUNE_MENU, PID_EDIT_MENU)
    void drawBedPIDMenu();
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #endif
#endif

// MPC
<<<<<<< HEAD
#if EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  void Draw_HotendMPC_Menu();
#endif
#if ENABLED(MPC_AUTOTUNE)
  void DWIN_MPCTuning(tempcontrol_t result);
=======
#if ANY(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
  void drawHotendMPCMenu();
#endif
#if ENABLED(MPC_AUTOTUNE)
  void dwinMPCTuning(tempcontrol_t result);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif

// CV Laser Module
#if ENABLED(CV_LASER_MODULE)
<<<<<<< HEAD
  void LaserOn(const bool turn_on);
  void Draw_LaserSettings_Menu();
  void Draw_LaserPrint_Menu();
#endif

// ToolBar
#if HAS_TOOLBAR
  void Draw_TBSetup_Menu();
  void Goto_ToolBar();
  void Exit_ToolBar();
#endif

#if DEBUG_DWIN
  void DWIN_Debug(const char *msg);
=======
  void laserOn(const bool turn_on);
  void drawLaserSettingsMenu();
  void drawLaserPrintMenu();
#endif

// toolBar
#if HAS_TOOLBAR
  void drawTBSetupMenu();
  void gotoToolBar();
  void exitToolBar();
#endif

#if DEBUG_DWIN
  void dwinDebug(const char *msg);
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
#endif
