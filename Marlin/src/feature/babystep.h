/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfigPre.h"

<<<<<<< HEAD
#if ENABLED(INTEGRATED_BABYSTEPPING)
  #define BABYSTEPS_PER_SEC 1000UL
  #define BABYSTEP_TICKS ((STEPPER_TIMER_RATE) / (BABYSTEPS_PER_SEC))
#else
  #define BABYSTEPS_PER_SEC 976UL
  #define BABYSTEP_TICKS ((TEMP_TIMER_RATE) / (BABYSTEPS_PER_SEC))
#endif

#if IS_CORE || EITHER(BABYSTEP_XY, I2C_POSITION_ENCODERS)
=======
#define BABYSTEPS_PER_SEC 1000UL
#define BABYSTEP_TICKS ((STEPPER_TIMER_RATE) / (BABYSTEPS_PER_SEC))

#if ANY(IS_CORE, BABYSTEP_XY, I2C_POSITION_ENCODERS)
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #define BS_AXIS_IND(A) A
  #define BS_AXIS(I) AxisEnum(I)
#else
  #define BS_AXIS_IND(A) 0
  #define BS_AXIS(I) Z_AXIS
#endif

#if ENABLED(BABYSTEP_DISPLAY_TOTAL)
  #if ENABLED(BABYSTEP_XY)
    #define BS_TOTAL_IND(A) A
  #else
    #define BS_TOTAL_IND(A) 0
  #endif
#endif

class Babystep {
public:
  static volatile int16_t steps[BS_AXIS_IND(Z_AXIS) + 1];
  static int16_t accum;                                     // Total babysteps in current edit

<<<<<<< HEAD
=======
  #if ALL(EP_BABYSTEPPING, EMERGENCY_PARSER)
    static int16_t ep_babysteps;
  #endif

>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if ENABLED(BABYSTEP_DISPLAY_TOTAL)
    static int16_t axis_total[BS_TOTAL_IND(Z_AXIS) + 1];   // Total babysteps since G28
    static void reset_total(const AxisEnum axis) {
      if (TERN1(BABYSTEP_XY, axis == Z_AXIS))
        axis_total[BS_TOTAL_IND(axis)] = 0;
    }
  #endif

  static void add_steps(const AxisEnum axis, const int16_t distance);
  static void add_mm(const AxisEnum axis, const_float_t mm);

<<<<<<< HEAD
=======
  #if ENABLED(EP_BABYSTEPPING)
    // Step Z for M293 / M294
    static void z_up();
    static void z_down();
    #if ENABLED(EMERGENCY_PARSER)
      // Step Z according to steps accumulated by the EP
      FORCE_INLINE static void do_ep_steps() {
        if (ep_babysteps) {
          if (ep_babysteps > 0) { z_up();   ep_babysteps--; }
          else                  { z_down(); ep_babysteps++; }
        }
      }
    #endif
  #endif // EP_BABYSTEPPING

>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  #if ENABLED(BD_SENSOR)
    static void set_mm(const AxisEnum axis, const_float_t mm);
  #endif

  static bool has_steps() {
    return steps[BS_AXIS_IND(X_AXIS)] || steps[BS_AXIS_IND(Y_AXIS)] || steps[BS_AXIS_IND(Z_AXIS)];
  }

  //
  // Called by the Temperature or Stepper ISR to
  // apply accumulated babysteps to the axes.
  //
  static void task() {
<<<<<<< HEAD
    LOOP_LE_N(i, BS_AXIS_IND(Z_AXIS)) step_axis(BS_AXIS(i));
=======
    for (uint8_t i = 0; i <= BS_AXIS_IND(Z_AXIS); ++i) step_axis(BS_AXIS(i));
>>>>>>> 77d77f62dd0573ee9e1b843a8b08d6a809dc2b69
  }

private:
  static void step_axis(const AxisEnum axis);
};

extern Babystep babystep;
