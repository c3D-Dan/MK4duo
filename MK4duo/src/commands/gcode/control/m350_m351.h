/**
 * MK4duo Firmware for 3D Printer, Laser and CNC
 *
 * Based on Marlin, Sprinter and grbl
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * mcode
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#if HAS_MICROSTEPS

  #define CODE_M350
  #define CODE_M351

  // M350 Set microstepping mode. Warning: Steps per unit remains unchanged. S code sets stepping mode for all drivers.
  inline void gcode_M350(void) {
    if (parser.seen('S')) for (int i = 0; i <= 4; i++) stepper.microstep_mode(i, parser.value_byte());
    LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.microstep_mode(i, parser.value_byte());
    if (parser.seen('B')) stepper.microstep_mode(4, parser.value_byte());
    stepper.microstep_readings();
  }

  /**
   * M351: Toggle MS1 MS2 pins directly with axis codes X Y Z E B
   *       S# determines MS1 or MS2, X# sets the pin high/low.
   */
  inline void gcode_M351(void) {
    if (parser.seen('S')) switch (parser.value_byte()) {
      case 1:
        LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.microstep_ms(i, parser.value_byte(), -1);
        if (parser.seen('B')) stepper.microstep_ms(4, parser.value_byte(), -1);
        break;
      case 2:
        LOOP_XYZE(i) if (parser.seen(axis_codes[i])) stepper.microstep_ms(i, -1, parser.value_byte());
        if (parser.seen('B')) stepper.microstep_ms(4, -1, parser.value_byte());
        break;
    }
    stepper.microstep_readings();
  }

#elif HAS_TRINAMIC

  #define CODE_M350

  inline void gcode_M350(void) {

    if (commands.get_target_tool(350)) return;

    #if DISABLED(DISABLE_M503)
      // No arguments? Show M350 report.
      if (!parser.seen("XYZE")) {
        tmc.print_M350();
        return;
      }
    #endif

    LOOP_XYZE(i) {
      if (uint16_t value = parser.ushortval(axis_codes[i])) {
        switch (i) {
          case X_AXIS:
            #if AXIS_HAS_TMC(X)
              driver[X_DRV]->tmc->microsteps(value);
            #endif
            #if AXIS_HAS_TMC(X2)
              driver[X2_DRV]->tmc->microsteps(value);
            #endif
            break;
          case Y_AXIS:
            #if AXIS_HAS_TMC(Y)
              driver[Y_DRV]->tmc->microsteps(value);
            #endif
            #if AXIS_HAS_TMC(Y2)
              driver[Y2_DRV]->tmc->microsteps(value);
            #endif
            break;
          case Z_AXIS:
            #if AXIS_HAS_TMC(Z)
              driver[Z_DRV]->tmc->microsteps(value);
            #endif
            #if AXIS_HAS_TMC(Z2)
              driver[Z2_DRV]->tmc->microsteps(value);
            #endif
            #if AXIS_HAS_TMC(Z3)
              driver[Z3_DRV]->tmc->microsteps(value);
            #endif
            break;
          case E_AXIS: {
            switch (TARGET_EXTRUDER) {
              #if AXIS_HAS_TMC(E0)
                case 0: driver[E0_DRV]->tmc->microsteps(value); break;
              #endif
              #if AXIS_HAS_TMC(E1)
                case 1: driver[E1_DRV]->tmc->microsteps(value); break;
              #endif
              #if AXIS_HAS_TMC(E2)
                case 2: driver[E2_DRV]->tmc->microsteps(value); break;
              #endif
              #if AXIS_HAS_TMC(E3)
                case 3: driver[E3_DRV]->tmc->microsteps(value); break;
              #endif
              #if AXIS_HAS_TMC(E4)
                case 4: driver[E4_DRV]->tmc->microsteps(value); break;
              #endif
              #if AXIS_HAS_TMC(E5)
                case 5: driver[E5_DRV]->tmc->microsteps(value); break;
              #endif
            }
          } break;
        }
      }
    }

  }

#endif // HAS_TRINAMIC
