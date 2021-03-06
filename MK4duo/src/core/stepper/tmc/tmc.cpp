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
 * tmc->cpp
 *
 * Copyright (c) 2019 Alberto Cotronei @MagoKimbra
 */

#include "../../../../MK4duo.h"
#include "sanitycheck.h"

#if HAS_TRINAMIC

#if HAVE_DRV(TMC2208)
  #include <HardwareSerial.h>
#endif

TMC_Stepper tmc;

/** Public Parameters */
#if HAS_SENSORLESS && ENABLED(IMPROVE_HOMING_RELIABILITY)
  millis_l TMC_Stepper::sg_guard_period = 0;
#endif

/** Private Parameters */
uint16_t TMC_Stepper::report_status_interval = 0;

/** Public Function */
void TMC_Stepper::init() {

  #if TMC_HAS_SPI
    init_cs_pins();
  #endif

  #if HAVE_DRV(TMC2208)

    #define _TMC2208_DEFINE_HARDWARE(ST, L) driver[ST##_DRV] = new Driver(L, &ST##_HARDWARE_SERIAL, R_SENSE)
    #define TMC2208_DEFINE_HARDWARE(ST)     _TMC2208_DEFINE_HARDWARE(ST, DRV_##ST##_LABEL)

    #define _TMC2208_DEFINE_SOFTWARE(ST, L) driver[ST##_DRV] = new Driver(L, ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN, R_SENSE, ST##_SERIAL_RX_PIN > NoPin)
    #define TMC2208_DEFINE_SOFTWARE(ST)     _TMC2208_DEFINE_SOFTWARE(ST, DRV_##ST##_LABEL)

    // Stepper objects of TMC2208 steppers used
    #if X_HAS_DRV(TMC2208)
      #if ENABLED(X_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(X);
        X_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(X);
        driver[X_DRV]->tmc->beginSerial(115200);
      #endif
      config(X_DRV, X_STEALTHCHOP);
    #endif
    #if X2_HAS_DRV(TMC2208)
      #if ENABLED(X2_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(X2);
        X2_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(X2);
        driver[X2_DRV]->tmc->beginSerial(115200);
      #endif
      config(X2_DRV, X_STEALTHCHOP);
    #endif
    #if Y_HAS_DRV(TMC2208)
      #if ENABLED(Y_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(Y);
        Y_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(Y);
        driver[Y_DRV]->tmc->beginSerial(115200);
      #endif
      config(Y_DRV, Y_STEALTHCHOP);
    #endif
    #if Y2_HAS_DRV(TMC2208)
      #if ENABLED(Y2_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(Y2);
        Y2_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(Y2);
        driver[Y2_DRV]->tmc->beginSerial(115200);
      #endif
      config(driver[Y2_DRV], Y_STEALTHCHOP);
    #endif
    #if Z_HAS_DRV(TMC2208)
      #if ENABLED(Z_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(Z);
        Z_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(Z);
        driver[Z_DRV]->tmc->beginSerial(115200);
      #endif
      config(Z_DRV, Z_STEALTHCHOP);
    #endif
    #if Z2_HAS_DRV(TMC2208)
      #if ENABLED(Z2_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(Z2);
        Z2_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(Z2);
        driver[Z2_DRV]->tmc->beginSerial(115200);
      #endif
      config(Z2_DRV, Z_STEALTHCHOP);
    #endif
    #if Z3_HAS_DRV(TMC2208)
      #if ENABLED(Z3_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(Z3);
        Z3_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(Z3);
        driver[Z3_DRV]->tmc->beginSerial(115200);
      #endif
      config(Z3_DRV, Z_STEALTHCHOP);
    #endif
    #if E0_HAS_DRV(TMC2208)
      #if ENABLED(E0_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E0);
        E0_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E0);
        driver[E0_DRV]->tmc->beginSerial(115200);
      #endif
      config(E0_DRV, E0_STEALTHCHOP);
    #endif
    #if E1_HAS_DRV(TMC2208)
      #if ENABLED(E1_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E1);
        E1_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E1);
        driver[E1_DRV]->tmc->beginSerial(115200);
      #endif
      config(E1_DRV, E1_STEALTHCHOP);
    #endif
    #if E2_HAS_DRV(TMC2208)
      #if ENABLED(E2_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E2);
        E2_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E2);
        driver[E2_DRV]->tmc->beginSerial(115200);
      #endif
      config(E2_DRV, E2_STEALTHCHOP);
    #endif
    #if E3_HAS_DRV(TMC2208)
      #if ENABLED(E3_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E3);
        E3_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E3);
        driver[E3_DRV]->tmc->beginSerial(115200);
      #endif
      config(E3_DRV, E3_STEALTHCHOP);
    #endif
    #if E4_HAS_DRV(TMC2208)
      #if ENABLED(E4_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E4);
        E4_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E4);
        driver[E4_DRV]->tmc->beginSerial(115200);
      #endif
      config(E4_DRV, E4_STEALTHCHOP);
    #endif
    #if E5_HAS_DRV(TMC2208)
      #if ENABLED(E5_HARDWARE_SERIAL)
        TMC2208_DEFINE_HARDWARE(E5);
        E5_HARDWARE_SERIAL.begin(115200);
      #else
        TMC2208_DEFINE_SOFTWARE(E5);
        driver[E5_DRV]->tmc->beginSerial(115200);
      #endif
      config(E5_DRV, E5_STEALTHCHOP);
    #endif

  #elif HAVE_DRV(TMC2660)

    #if ENABLED(TMC_USE_SW_SPI)
      #define _TMC2660_DEFINE(ST, L)  driver[ST##_DRV] = new Driver(L, ST##_CS_PIN, R_SENSE, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
      #define TMC2660_DEFINE(ST)      _TMC2660_DEFINE(ST, DRV_##ST##_LABEL)
    #else
      #define _TMC2660_DEFINE(ST, L)  driver[ST##_DRV] = new Driver(L, ST##_CS_PIN, R_SENSE)
      #define TMC2660_DEFINE(ST)      _TMC2660_DEFINE(ST, DRV_##ST##_LABEL)
    #endif

    #if DISABLED(TMC_USE_SW_SPI)
      SPI.begin();
    #endif

    // Stepper objects of TMC2660 steppers used
    #if X_HAS_DRV(TMC2660)
      TMC2660_DEFINE(X);
      config(X_DRV);
    #endif
    #if X2_HAS_DRV(TMC2660)
      TMC2660_DEFINE(X2);
      config(X2_DRV);
    #endif
    #if Y_HAS_DRV(TMC2660)
      TMC2660_DEFINE(Y);
      config(Y_DRV);
    #endif
    #if Y2_HAS_DRV(TMC2660)
      TMC2660_DEFINE(Y2);
      config(driver[Y2_DRV]);
    #endif
    #if Z_HAS_DRV(TMC2660)
      TMC2660_DEFINE(Z);
      config(Z_DRV);
    #endif
    #if Z2_HAS_DRV(TMC2660)
      TMC2660_DEFINE(Z2);
      config(Z2_DRV);
    #endif
    #if Z3_HAS_DRV(TMC2660)
      TMC2660_DEFINE(Z3);
      config(Z3_DRV);
    #endif
    #if E0_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E0);
      config(E0_DRV);
    #endif
    #if E1_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E1);
      config(E1_DRV);
    #endif
    #if E2_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E2);
      config(E2_DRV);
    #endif
    #if E3_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E3);
      config(E3_DRV);
    #endif
    #if E4_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E4);
      config(E4_DRV);
    #endif
    #if E5_HAS_DRV(TMC2660)
      TMC2660_DEFINE(E5);
      config(E5_DRV);
    #endif

  #elif HAS_TMCX1X0

    #if ENABLED(TMC_USE_SW_SPI)
      #define _TMC_MODEL_DEFINE(ST, L)  driver[ST##_DRV] = new Driver(L, ST##_CS_PIN, R_SENSE, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
      #define TMC_MODEL_DEFINE(ST)      _TMC_MODEL_DEFINE(ST, DRV_##ST##_LABEL)
    #else
      #define _TMC_MODEL_DEFINE(ST, L)  driver[ST##_DRV] = new Driver(L, ST##_CS_PIN, R_SENSE)
      #define TMC_MODEL_DEFINE(ST)      _TMC_MODEL_DEFINE(ST, DRV_##ST##_LABEL)
    #endif

    #if DISABLED(TMC_USE_SW_SPI)
      SPI.begin();
    #endif

    // Stepper objects of TMC2130 steppers used
    #if X_HAS_DRV(TMC2130) || X_HAS_DRV(TMC2160) || X_HAS_DRV(TMC5130) || X_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(X);
      config(X_DRV, X_STEALTHCHOP);
    #endif
    #if X2_HAS_DRV(TMC2130) || X2_HAS_DRV(TMC2160) || X2_HAS_DRV(TMC5130) || X2_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(X2);
      config(X2_DRV, X_STEALTHCHOP);
    #endif
    #if Y_HAS_DRV(TMC2130) || Y_HAS_DRV(TMC2160) || Y_HAS_DRV(TMC5130) || Y_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(Y);
      config(Y_DRV, Y_STEALTHCHOP);
    #endif
    #if Y2_HAS_DRV(TMC2130) || Y2_HAS_DRV(TMC2160) || Y2_HAS_DRV(TMC5130) || Y2_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(Y2);
      config(driver[Y2_DRV], Y_STEALTHCHOP);
    #endif
    #if Z_HAS_DRV(TMC2130) || Z_HAS_DRV(TMC2160) || Z_HAS_DRV(TMC5130) || Z_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(Z);
      config(Z_DRV, Z_STEALTHCHOP);
    #endif
    #if Z2_HAS_DRV(TMC2130) || Z2_HAS_DRV(TMC2160) || Z2_HAS_DRV(TMC5130) || Z2_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(Z2);
      config(Z2_DRV, Z_STEALTHCHOP);
    #endif
    #if Z3_HAS_DRV(TMC2130) || Z3_HAS_DRV(TMC2160) || Z3_HAS_DRV(TMC5130) || Z3_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(Z3);
      config(Z3_DRV, Z_STEALTHCHOP);
    #endif
    #if E0_HAS_DRV(TMC2130) || E0_HAS_DRV(TMC2160) || E0_HAS_DRV(TMC5130) || E0_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E0);
      config(E0_DRV, E0_STEALTHCHOP);
    #endif
    #if E1_HAS_DRV(TMC2130) || E1_HAS_DRV(TMC2160) || E1_HAS_DRV(TMC5130) || E1_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E1);
      config(E1_DRV, E1_STEALTHCHOP);
    #endif
    #if E2_HAS_DRV(TMC2130) || E2_HAS_DRV(TMC2160) || E2_HAS_DRV(TMC5130) || E2_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E2);
      config(E2_DRV, E2_STEALTHCHOP);
    #endif
    #if E3_HAS_DRV(TMC2130) || E3_HAS_DRV(TMC2160) || E3_HAS_DRV(TMC5130) || E3_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E3);
      config(E3_DRV, E3_STEALTHCHOP);
    #endif
    #if E4_HAS_DRV(TMC2130) || E4_HAS_DRV(TMC2160) || E4_HAS_DRV(TMC5130) || E4_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E4);
      config(E4_DRV, E4_STEALTHCHOP);
    #endif
    #if E5_HAS_DRV(TMC2130) || E5_HAS_DRV(TMC2160) || E5_HAS_DRV(TMC5130) || E5_HAS_DRV(TMC5160)
      TMC_MODEL_DEFINE(E5);
      config(E5_DRV, E5_STEALTHCHOP);
    #endif

  #endif

  #if HAS_SENSORLESS
    #if X_HAS_SENSORLESS
      #if AXIS_HAS_STALLGUARD(X)
        driver[X_DRV]->tmc->homing_threshold(X_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(X2)
        driver[X2_DRV]->homing_threshold(X_STALL_SENSITIVITY);
      #endif
    #endif
    #if Y_SENSORLESS
      #if AXIS_HAS_STALLGUARD(Y)
        driver[Y_DRV]->homing_threshold(Y_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Y2)
        driver[Y2_DRV]->homing_threshold(Y_STALL_SENSITIVITY);
      #endif
    #endif
    #if Z_SENSORLESS
      #if AXIS_HAS_STALLGUARD(Z)
        driver[Z_DRV]->homing_threshold(Z_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Z2)
        driver[Z2_DRV]->homing_threshold(Z_STALL_SENSITIVITY);
      #endif
      #if AXIS_HAS_STALLGUARD(Z3)
        driver[Z3_DRV]->homing_threshold(Z_STALL_SENSITIVITY);
      #endif
    #endif
  #endif

  TMC_ADV();

}

// Use internal reference voltage for current calculations. This is the default.
// Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
// https://www.trinamic.com/products/integrated-circuits/details/tmc2130/
void TMC_Stepper::factory_parameters() {

  constexpr uint16_t  tmc_stepper_current[MAX_DRIVER]   = { X_CURRENT, Y_CURRENT, Z_CURRENT,
                                                            E0_CURRENT, E1_CURRENT, E2_CURRENT, E3_CURRENT, E4_CURRENT, E5_CURRENT,
                                                            X_CURRENT, Y_CURRENT, Z_CURRENT, Z_CURRENT },
                      tmc_stepper_microstep[MAX_DRIVER] = { X_MICROSTEPS, Y_MICROSTEPS, Z_MICROSTEPS,
                                                            E0_MICROSTEPS, E1_MICROSTEPS, E2_MICROSTEPS, E3_MICROSTEPS, E4_MICROSTEPS, E5_MICROSTEPS,
                                                            X_MICROSTEPS, Y_MICROSTEPS, Z_MICROSTEPS, Z_MICROSTEPS };
  constexpr uint32_t  tmc_hybrid_threshold[MAX_DRIVER]  = { X_HYBRID_THRESHOLD, Y_HYBRID_THRESHOLD, Z_HYBRID_THRESHOLD,
                                                            E0_HYBRID_THRESHOLD, E1_HYBRID_THRESHOLD, E2_HYBRID_THRESHOLD,
                                                            E3_HYBRID_THRESHOLD, E4_HYBRID_THRESHOLD, E5_HYBRID_THRESHOLD,
                                                            X_HYBRID_THRESHOLD, Y_HYBRID_THRESHOLD, Z_HYBRID_THRESHOLD, Z_HYBRID_THRESHOLD };
  constexpr bool      tmc_stealth_enabled[MAX_DRIVER]   = { X_STEALTHCHOP, Y_STEALTHCHOP, Z_STEALTHCHOP,
                                                            E0_STEALTHCHOP, E1_STEALTHCHOP, E2_STEALTHCHOP, E3_STEALTHCHOP, E4_STEALTHCHOP, E5_STEALTHCHOP,
                                                            X_STEALTHCHOP, Y_STEALTHCHOP, Z_STEALTHCHOP, Z_STEALTHCHOP };

  LOOP_DRV() {
    if (driver[d] && driver[d]->tmc) {
      driver[d]->tmc->rms_current(tmc_stepper_current[d]);
      driver[d]->tmc->microsteps(tmc_stepper_microstep[d]);
      #if ENABLED(HYBRID_THRESHOLD)
        driver[d]->tmc->set_pwm_thrs(tmc_hybrid_threshold[d]);
      #endif
      #if TMC_HAS_STEALTHCHOP
        driver[d]->tmc->stealthChop_enabled = tmc_stealth_enabled[d];
        driver[d]->tmc->refresh_stepping_mode();
      #endif
    }
  }

}

void TMC_Stepper::restore() {
  LOOP_DRV() if (driver[d] && driver[d]->tmc) driver[d]->tmc->push();
}

void TMC_Stepper::test_connection(const bool test_x, const bool test_y, const bool test_z, const bool test_e) {
  uint8_t axis_connection = 0;

  if (test_x) {
    #if AXIS_HAS_TMC(X)
      axis_connection += test_connection(X_DRV);
    #endif
    #if AXIS_HAS_TMC(X2)
      axis_connection += test_connection(X2_DRV);
    #endif
  }

  if (test_y) {
    #if AXIS_HAS_TMC(Y)
      axis_connection += test_connection(Y_DRV);
    #endif
    #if AXIS_HAS_TMC(Y2)
      axis_connection += test_connection(Y2_DRV);
    #endif
  }

  if (test_z) {
    #if AXIS_HAS_TMC(Z)
      axis_connection += test_connection(Z_DRV);
    #endif
    #if AXIS_HAS_TMC(Z2)
      axis_connection += test_connection(Z2_DRV);
    #endif
    #if AXIS_HAS_TMC(Z3)
      axis_connection += test_connection(Z3_DRV);
    #endif
  }

  if (test_e) {
    #if AXIS_HAS_TMC(E0)
      axis_connection += test_connection(E0_DRV);
    #endif
    #if AXIS_HAS_TMC(E1)
      axis_connection += test_connection(E1_DRV);
    #endif
    #if AXIS_HAS_TMC(E2)
      axis_connection += test_connection(E2_DRV);
    #endif
    #if AXIS_HAS_TMC(E3)
      axis_connection += test_connection(E3_DRV);
    #endif
    #if AXIS_HAS_TMC(E4)
      axis_connection += test_connection(E4_DRV);
    #endif
    #if AXIS_HAS_TMC(E5)
      axis_connection += test_connection(E5_DRV);
    #endif
  }

  if (axis_connection) lcdui.set_status_P(PSTR("TMC CONNECTION ERROR"));
}

#if ENABLED(MONITOR_DRIVER_STATUS)

  void TMC_Stepper::monitor_driver() {
    static millis_s next_poll_ms = millis();
    bool need_update_error_counters = expired(&next_poll_ms, MONITOR_DRIVER_STATUS_INTERVAL_MS);
    bool need_debug_reporting = false;
    if (need_update_error_counters) {
      next_poll_ms = millis();
      #if ENABLED(TMC_DEBUG)
        static millis_s next_debug_reporting_ms = millis();
        if (expired(&next_debug_reporting_ms, report_status_interval)) {
          need_debug_reporting = true;
          next_debug_reporting_ms = millis();
        }
      #endif
      if (need_update_error_counters || need_debug_reporting) {
        LOOP_DRV() {
          if (driver[d] && driver[d]->tmc) monitor_driver(DriverEnum(d), need_update_error_counters, need_debug_reporting);
        }

        #if ENABLED(TMC_DEBUG)
          if (need_debug_reporting) SERIAL_EOL();
        #endif
      }
    }
  }

#endif // ENABLED(MONITOR_DRIVER_STATUS)

#if HAS_SENSORLESS

  bool TMC_Stepper::enable_stallguard(const DriverEnum index) {
    bool old_stealthChop = driver[index]->tmc->en_pwm_mode();

    driver[index]->tmc->TCOOLTHRS(0xFFFFF);
    driver[index]->tmc->en_pwm_mode(false);
    driver[index]->tmc->diag1_stall(true);

    return old_stealthChop;
  }

  void TMC_Stepper::disable_stallguard(const DriverEnum index, const bool enable) {
    driver[index]->tmc->TCOOLTHRS(0);
    driver[index]->tmc->en_pwm_mode(enable);
    driver[index]->tmc->diag1_stall(false);
  }

#endif

#if ENABLED(TMC_DEBUG)

  /**
   * M922 [S<0|1>] [Pnnn] Enable periodic status reports
   */
  #if ENABLED(MONITOR_DRIVER_STATUS)
    void TMC_Stepper::set_report_interval(const uint16_t update_interval) {
      if ((report_status_interval = update_interval)) {
        SERIAL_EM("axis:pwm_scale"
        #if TMC_HAS_STEALTHCHOP
          "/current_scale"
        #endif
        #if TMC_HAS_STALLGUARD
            "/mech_load"
          #endif
          "|flags|warncount"
        );
      }
    }
  #endif

  /**
   * M922 report functions
   */
  void TMC_Stepper::report_all(bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    #define TMC_REPORT(LABEL, ITEM) do{ SERIAL_SM(ECHO, LABEL);  debug_loop(ITEM, print_x, print_y, print_z, print_e); }while(0)
    #define DRV_REPORT(LABEL, ITEM) do{ SERIAL_SM(ECHO, LABEL); status_loop(ITEM, print_x, print_y, print_z, print_e); }while(0)
    TMC_REPORT("\t",                  TMC_CODES);
    TMC_REPORT("Enabled\t",           TMC_ENABLED);
    TMC_REPORT("Set current",         TMC_CURRENT);
    TMC_REPORT("RMS current",         TMC_RMS_CURRENT);
    TMC_REPORT("MAX current",         TMC_MAX_CURRENT);
    TMC_REPORT("Run current",         TMC_IRUN);
    TMC_REPORT("Hold current",        TMC_IHOLD);
    #if HAVE_DRV(TMC2160) || HAVE_DRV(TMC5160)
      TMC_REPORT("Global scaler",     TMC_GLOBAL_SCALER);
    #endif
    TMC_REPORT("CS actual\t",         TMC_CS_ACTUAL);
    TMC_REPORT("PWM scale",           TMC_PWM_SCALE);
    TMC_REPORT("vsense\t",            TMC_VSENSE);
    TMC_REPORT("stealthChop",         TMC_STEALTHCHOP);
    TMC_REPORT("msteps\t",            TMC_MICROSTEPS);
    TMC_REPORT("tstep\t",             TMC_TSTEP);
    TMC_REPORT("pwm\threshold\t",     TMC_TPWMTHRS);
    TMC_REPORT("[mm/s]\t",            TMC_TPWMTHRS_MMS);
    TMC_REPORT("OT prewarn",          TMC_OTPW);
    #if ENABLED(MONITOR_DRIVER_STATUS)
      TMC_REPORT("OT prewarn has\n"
                 "been triggered",    TMC_OTPW_TRIGGERED);
    #endif
    TMC_REPORT("off time\t",          TMC_TOFF);
    TMC_REPORT("blank time",          TMC_TBL);
    TMC_REPORT("hysteresis\n-end\t",  TMC_HEND);
    TMC_REPORT("-start\t",            TMC_HSTRT);
    TMC_REPORT("Stallguard thrs",     TMC_SGT);

    DRV_REPORT("DRVSTATUS\t",         TMC_DRV_CODES);
    #if HAS_TMCX1X0
      DRV_REPORT("stallguard\t",      TMC_STALLGUARD);
      DRV_REPORT("sg_result\t",       TMC_SG_RESULT);
      DRV_REPORT("fsactive\t",        TMC_FSACTIVE);
    #endif
    DRV_REPORT("stst\t",              TMC_STST);
    DRV_REPORT("olb\t",               TMC_OLB);
    DRV_REPORT("ola\t",               TMC_OLA);
    DRV_REPORT("s2gb\t",              TMC_S2GB);
    DRV_REPORT("s2ga\t",              TMC_S2GA);
    DRV_REPORT("otpw\t",              TMC_DRV_OTPW);
    DRV_REPORT("ot\t",                TMC_OT);
    #if HAVE_DRV(TMC2208)
      DRV_REPORT("157C\t",            TMC_T157);
      DRV_REPORT("150C\t",            TMC_T150);
      DRV_REPORT("143C\t",            TMC_T143);
      DRV_REPORT("120C\t",            TMC_T120);
      DRV_REPORT("s2vsa\t",           TMC_S2VSA);
      DRV_REPORT("s2vsb\t",           TMC_S2VSB);
    #endif
    DRV_REPORT("Driver registers:",   TMC_DRV_STATUS_HEX);
    SERIAL_EOL();
  }

  void TMC_Stepper::get_registers(bool print_x, bool print_y, bool print_z, bool print_e) {
    #define _TMC_GET_REG(LABEL, ITEM) do{ SERIAL_MSG(LABEL); get_registers(ITEM, print_x, print_y, print_z, print_e); }while(0)
    #define TMC_GET_REG(NAME, TABS)   _TMC_GET_REG(STRINGIFY(NAME) TABS, TMC_GET_##NAME)
    _TMC_GET_REG("\t", TMC_AXIS_CODES);
    TMC_GET_REG(GCONF, "\t\t");
    TMC_GET_REG(IHOLD_IRUN, "\t");
    TMC_GET_REG(GSTAT, "\t\t");
    TMC_GET_REG(IOIN, "\t\t");
    TMC_GET_REG(TPOWERDOWN, "\t");
    TMC_GET_REG(TSTEP, "\t\t");
    TMC_GET_REG(TPWMTHRS, "\t");
    TMC_GET_REG(TCOOLTHRS, "\t");
    TMC_GET_REG(THIGH, "\t\t");
    TMC_GET_REG(CHOPCONF, "\t");
    TMC_GET_REG(COOLCONF, "\t");
    TMC_GET_REG(PWMCONF, "\t");
    TMC_GET_REG(PWM_SCALE, "\t");
    TMC_GET_REG(DRV_STATUS, "\t");
  }

#endif // ENABLED(TMC_DEBUG)

#if DISABLED(DISABLE_M503)

  void TMC_Stepper::print_M350() {

    SERIAL_LM(CFG, "Stepper driver microsteps");

    #if AXIS_HAS_TMC(X) || AXIS_HAS_TMC(Y) || AXIS_HAS_TMC(Z)
      SERIAL_SM(CFG, "  M350");
      LOOP_DRV_XYZ() {
        if (driver[d] && driver[d]->tmc) {
          SERIAL_MSG(" ");
          driver[d]->printLabel();
          SERIAL_VAL(driver[d]->tmc->microsteps());
        }
      }
      SERIAL_EOL();
    #endif

    LOOP_DRV_EXTRUDER() {
      if (driver[d] && driver[d]->tmc) {
        SERIAL_SM(CFG, "  M350 ");
        driver[d]->printLabel();
        SERIAL_EMV(" E", driver[d]->tmc->microsteps());
      }
    }

  }

  void TMC_Stepper::print_M906() {

    SERIAL_LM(CFG, "Stepper driver current (mA)");

    #if AXIS_HAS_TMC(X) || AXIS_HAS_TMC(Y) || AXIS_HAS_TMC(Z)
      SERIAL_SM(CFG, "  M906");
      LOOP_DRV_XYZ() {
        if (driver[d] && driver[d]->tmc) {
          SERIAL_MSG(" ");
          driver[d]->printLabel();
          SERIAL_VAL(driver[d]->tmc->getMilliamps());
        }
      }
      SERIAL_EOL();
    #endif

    LOOP_DRV_EXTRUDER() {
      if (driver[d] && driver[d]->tmc) {
        SERIAL_SM(CFG, "  M906 ");
        driver[d]->printLabel();
        SERIAL_EMV(" E", driver[d]->tmc->getMilliamps());
      }
    }

  }

  void TMC_Stepper::print_M913() {

    #if ENABLED(HYBRID_THRESHOLD)

      SERIAL_LM(CFG, "Stepper driver Hybrid Threshold");

      #if AXIS_HAS_TMC(X) || AXIS_HAS_TMC(Y) || AXIS_HAS_TMC(Z)
        SERIAL_SM(CFG, "  M913");
        LOOP_DRV_XYZ() {
          if (driver[d] && driver[d]->tmc) {
            SERIAL_MSG(" ");
            driver[d]->printLabel();
            SERIAL_VAL(driver[d]->tmc->get_pwm_thrs());
          }
        }
        SERIAL_EOL();
      #endif

      LOOP_DRV_EXTRUDER() {
        if (driver[d] && driver[d]->tmc) {
          SERIAL_SM(CFG, "  M913 ");
          driver[d]->printLabel();
          SERIAL_EMV(" E", driver[d]->tmc->get_pwm_thrs());
        }
      }

    #endif // HYBRID_THRESHOLD

  }

  void TMC_Stepper::print_M914() {
    #if HAS_SENSORLESS
      SERIAL_LM(CFG, "Stepper driver StallGuard threshold:");
      #if X_HAS_SENSORLESS || Y_HAS_SENSORLESS || Z_HAS_SENSORLESS
        SERIAL_SM(CFG, "  M914");
        #if X_HAS_SENSORLESS
          driver[X_DRV]->printLabel();
          SERIAL_VAL(driver[X_DRV]->tmc->homing_threshold());
        #endif
        #if Y_HAS_SENSORLESS
          driver[Y_DRV]->printLabel();
          SERIAL_VAL(driver[Y_DRV]->tmc->homing_threshold());
        #endif
        #if Z_HAS_SENSORLESS
          driver[Z_DRV]->printLabel();
          SERIAL_VAL(driver[Z_DRV]->tmc->homing_threshold());
        #endif
        SERIAL_EOL();
      #endif
    #endif // HAS_SENSORLESS
  }

  void TMC_Stepper::print_M940() {
    #if TMC_HAS_STEALTHCHOP
      SERIAL_LM(CFG, "Stepper driver StealthChop:");
      SERIAL_SM(CFG, "  M940");
      #if AXIS_HAS_STEALTHCHOP(X)
        SERIAL_CHR(' ');
        driver[X_DRV]->printLabel();
        SERIAL_VAL(driver[X_DRV]->tmc->get_stealthChop_status());
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y)
        SERIAL_CHR(' ');
        driver[Y_DRV]->printLabel();
        SERIAL_VAL(driver[Y_DRV]->tmc->get_stealthChop_status());
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z)
        SERIAL_CHR(' ');
        driver[Z_DRV]->printLabel();
        SERIAL_VAL(driver[Z_DRV]->tmc->get_stealthChop_status());
      #endif
      #if AXIS_HAS_STEALTHCHOP(E0)
        SERIAL_CHR(' ');
        driver[E0_DRV]->printLabel();
        SERIAL_VAL(driver[E0_DRV]->tmc->get_stealthChop_status());
      #endif
      SERIAL_EOL();
    #endif // TMC_HAS_STEALTHCHOP
  }

#endif // DISABLED(DISABLE_M503)

#if ENABLED(MONITOR_DRIVER_STATUS)

  void TMC_Stepper::report_otpw(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_ELOGIC(" temperature prewarn triggered:", driver[index]->tmc->getOTPW());
  }

  void TMC_Stepper::clear_otpw(const DriverEnum index) {
    driver[index]->tmc->clear_otpw();
    driver[index]->printLabel();
    SERIAL_EM(" prewarn flag cleared");
  }

#endif

void TMC_Stepper::get_off_time(const DriverEnum index) {
  driver[index]->printLabel();
  SERIAL_EMV(" off_time:", driver[index]->tmc->toff());
}

void TMC_Stepper::set_off_time(const DriverEnum index, const uint8_t off_time_val) {
  driver[index]->tmc->toff(off_time_val);
}

#if HAVE_DRV(TMC2130)

  void TMC_Stepper::get_blank_time(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" blank_time: ", driver[index]->tmc->blank_time());
  }

  void TMC_Stepper::set_blank_time(const DriverEnum index, const uint8_t blank_time_val) {
    driver[index]->tmc->blank_time(blank_time_val);
  }

  void TMC_Stepper::get_hysteresis_end(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" hysteresis_end: ", driver[index]->tmc->hysteresis_end());
  }

  void TMC_Stepper::set_hysteresis_end(const DriverEnum index, const int8_t hysteresis_end_val) {
    driver[index]->tmc->hysteresis_end(hysteresis_end_val);
  }

  void TMC_Stepper::get_hysteresis_start(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" hysteresis_start: ", driver[index]->tmc->hysteresis_start());
  }

  void TMC_Stepper::set_hysteresis_start(const DriverEnum index, const uint8_t hysteresis_start_val) {
    driver[index]->tmc->hysteresis_start(hysteresis_start_val);
  }

  void TMC_Stepper::get_disable_I_comparator(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" disable_I_comparator: ", driver[index]->tmc->disfdcc());
  }

  void TMC_Stepper::set_disable_I_comparator(const DriverEnum index, const bool onoff) {
    driver[index]->tmc->disfdcc(onoff);
  }

  void TMC_Stepper::get_stealth_gradient(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" stealth_gradient: ", driver[index]->tmc->pwm_grad());
  }

  void TMC_Stepper::set_stealth_gradient(const DriverEnum index, const uint8_t stealth_gradient_val) {
    driver[index]->tmc->pwm_grad(stealth_gradient_val);
  }

  void TMC_Stepper::get_stealth_amplitude(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" stealth_amplitude: ", driver[index]->tmc->pwm_ampl());
  }

  void TMC_Stepper::set_stealth_amplitude(const DriverEnum index, const uint8_t stealth_amplitude_val) {
    driver[index]->tmc->pwm_ampl(stealth_amplitude_val);
  }

  void TMC_Stepper::get_stealth_freq(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" stealth_freq: ", driver[index]->tmc->pwm_freq());
  }

  void TMC_Stepper::set_stealth_freq(const DriverEnum index, const uint8_t stealth_freq_val) {
    driver[index]->tmc->pwm_freq(stealth_freq_val);
  }

  void TMC_Stepper::get_stealth_autoscale(const DriverEnum index) {
    driver[index]->printLabel();
    SERIAL_EMV(" stealth_autoscale: ", driver[index]->tmc->pwm_autoscale());
  }

  void TMC_Stepper::set_stealth_autoscale(const DriverEnum index, const bool onoff) {
    driver[index]->tmc->pwm_autoscale(onoff);
  }

#endif // HAVE_DRV(TMC2130)

/** Private Function */
bool TMC_Stepper::test_connection(const DriverEnum index) {
  if (driver[index]->tmc) {
    SERIAL_MSG("Testing ");
    driver[index]->printLabel();
    SERIAL_MSG(" connection... ");
    const uint8_t test_result = driver[index]->tmc->test_connection();

    if (test_result > 0) SERIAL_MSG("Error: All ");

    const char *stat;
    switch (test_result) {
      default:
      case 0: stat = PSTR("OK"); break;
      case 1: stat = PSTR("HIGH"); break;
      case 2: stat = PSTR("LOW"); break;
    }
    SERIAL_STR(stat);
    SERIAL_EOL();

    return test_result;
  }
}

#if TMC_HAS_SPI

  void TMC_Stepper::init_cs_pins() {
    #if AXIS_HAS_SPI(X)
      OUT_WRITE(X_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(Y)
      OUT_WRITE(Y_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(Z)
      OUT_WRITE(Z_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(X2)
      OUT_WRITE(X2_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(Y2)
      OUT_WRITE(Y2_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(Z2)
      OUT_WRITE(Z2_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(Z3)
      OUT_WRITE(Z3_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E0)
      OUT_WRITE(E0_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E1)
      OUT_WRITE(E1_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E2)
      OUT_WRITE(E2_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E3)
      OUT_WRITE(E3_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E4)
      OUT_WRITE(E4_CS_PIN, HIGH);
    #endif
    #if AXIS_HAS_SPI(E5)
      OUT_WRITE(E5_CS_PIN, HIGH);
    #endif
  }

#endif // TMC_HAS_SPI

// Stepper config for type
#if HAVE_DRV(TMC2130)
  
  void TMC_Stepper::config(const DriverEnum index, const bool stealth/*=false*/) {

    driver[index]->tmc->begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    #if ENABLED(SQUARE_WAVE_STEPPING)
      chopconf.dedge = true;
    #endif
    driver[index]->tmc->CHOPCONF(chopconf.sr);

    driver[index]->tmc->iholddelay(10);
    driver[index]->tmc->TPOWERDOWN(128);

    driver[index]->tmc->en_pwm_mode(stealth);
    driver[index]->tmc->stealthChop_enabled = stealth;

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    driver[index]->tmc->PWMCONF(pwmconf.sr);

    driver[index]->tmc->GSTAT(); // Clear GSTAT

  }

#elif HAVE_DRV(TMC2160)

  void TMC_Stepper::config(const DriverEnum index, const bool stealth/*=false*/) {

    driver[index]->tmc->begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    #if ENABLED(SQUARE_WAVE_STEPPING)
      chopconf.dedge = true;
    #endif
    driver[index]->tmc->CHOPCONF(chopconf.sr);

    driver[index]->tmc->iholddelay(10);
    driver[index]->tmc->TPOWERDOWN(128);
    driver[index]->tmc->TCOOLTHRS(0xFFFFF);

    driver[index]->tmc->en_pwm_mode(stealth);
    driver[index]->tmc->stealthChop_enabled = stealth;

    TMC2160_n::PWMCONF_t pwmconf{0};
    pwmconf.pwm_lim = 12;
    pwmconf.pwm_reg = 8;
    pwmconf.pwm_autograd = true;
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_freq = 0b01;
    pwmconf.pwm_grad = 14;
    pwmconf.pwm_ofs = 36;
    driver[index]->tmc->PWMCONF(pwmconf.sr);

    driver[index]->tmc->GSTAT(); // Clear GSTAT

  }

#elif HAVE_DRV(TMC2208)

  void TMC_Stepper::config(const DriverEnum index, const bool stealth/*=false*/) {

    TMC2208_n::GCONF_t gconf{0};
    gconf.pdn_disable = true;       // Use UART
    gconf.mstep_reg_select = true;  // Select microsteps with UART
    gconf.i_scale_analog = false;
    gconf.en_spreadcycle = !stealth;
    driver[index]->tmc->GCONF(gconf.sr);
    driver[index]->tmc->stealthChop_enabled = stealth;

    TMC2208_n::CHOPCONF_t chopconf{0};
    chopconf.tbl = 0b01; // blank_time = 24
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    #if ENABLED(SQUARE_WAVE_STEPPING)
      chopconf.dedge = true;
    #endif
    driver[index]->tmc->CHOPCONF(chopconf.sr);

    driver[index]->tmc->iholddelay(10);
    driver[index]->tmc->TPOWERDOWN(128);

    TMC2208_n::PWMCONF_t pwmconf{0};
    pwmconf.pwm_lim = 12;
    pwmconf.pwm_reg = 8;
    pwmconf.pwm_autograd = true;
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_freq = 0b01;
    pwmconf.pwm_grad = 14;
    pwmconf.pwm_ofs = 36;
    driver[index]->tmc->PWMCONF(pwmconf.sr);

    driver[index]->tmc->GSTAT(0b111); // Clear
    delay(200);
  }

#elif HAVE_DRV(TMC2660)

  void TMC_Stepper::config(const DriverEnum index, const bool) {

    driver[index]->tmc->begin();

    TMC2660_n::CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    driver[index]->tmc->CHOPCONF(chopconf.sr);
 
    driver[index]->tmc->sdoff(0);
    #if ENABLED(SQUARE_WAVE_STEPPING)
      driver[index]->tmc->dedge(true);
    #endif
    driver[index]->tmc->intpol(INTERPOLATE);
    driver[index]->tmc->diss2g(true); // Disable short to ground protection. Too many false readings?

    #if ENABLED(TMC_DEBUG)
      driver[index]->tmc->rdsel(0b01);
    #endif

  }

#elif HAVE_DRV(TMC5130)

  void TMC_Stepper::config(const DriverEnum index, const bool stealth/*=false*/) {

    driver[index]->tmc->begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    #if ENABLED(SQUARE_WAVE_STEPPING)
      chopconf.dedge = true;
    #endif
    driver[index]->tmc->CHOPCONF(chopconf.sr);

    driver[index]->tmc->iholddelay(10);
    driver[index]->tmc->TPOWERDOWN(128);

    driver[index]->tmc->en_pwm_mode(stealth);
    driver[index]->tmc->stealthChop_enabled = stealth;

    PWMCONF_t pwmconf{0};
    pwmconf.pwm_freq = 0b01; // f_pwm = 2/683 f_clk
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_grad = 5;
    pwmconf.pwm_ampl = 180;
    driver[index]->tmc->PWMCONF(pwmconf.sr);

    driver[index]->tmc->GSTAT(); // Clear GSTAT

  }

#elif HAVE_DRV(TMC5160)
  
  void TMC_Stepper::config(const DriverEnum index, const bool stealth/*=false*/) {

    driver[index]->tmc->begin();

    CHOPCONF_t chopconf{0};
    chopconf.tbl = 1;
    chopconf.toff = chopper_timing.toff;
    chopconf.intpol = INTERPOLATE;
    chopconf.hend = chopper_timing.hend + 3;
    chopconf.hstrt = chopper_timing.hstrt - 1;
    #if ENABLED(SQUARE_WAVE_STEPPING)
      chopconf.dedge = true;
    #endif
    driver[index]->tmc->CHOPCONF(chopconf.sr);

    driver[index]->tmc->iholddelay(10);
    driver[index]->tmc->TPOWERDOWN(128);

    driver[index]->tmc->en_pwm_mode(stealth);
    driver[index]->tmc->stealthChop_enabled = stealth;

    TMC2160_n::PWMCONF_t pwmconf{0};
    pwmconf.pwm_lim = 12;
    pwmconf.pwm_reg = 8;
    pwmconf.pwm_autograd = true;
    pwmconf.pwm_autoscale = true;
    pwmconf.pwm_freq = 0b01;
    pwmconf.pwm_grad = 14;
    pwmconf.pwm_ofs = 36;
    driver[index]->tmc->PWMCONF(pwmconf.sr);

    driver[index]->tmc->GSTAT(); // Clear GSTAT

  }

#endif

#if ENABLED(MONITOR_DRIVER_STATUS)

  #if ENABLED(TMC_DEBUG)
    #if HAVE_DRV(TMC2208)
      uint32_t TMC_Stepper::get_pwm_scale(const DriverEnum index) { return driver[index]->tmc->pwm_scale_sum(); }
    #elif HAVE_DRV(TMC2660)
      uint32_t TMC_Stepper::get_pwm_scale(const DriverEnum index) { UNUSED(index); return 0; }
    #elif HAS_TMCX1X0
      uint32_t TMC_Stepper::get_pwm_scale(const DriverEnum index) { return driver[index]->tmc->PWM_SCALE(); }
    #endif
  #endif

  #if HAVE_DRV(TMC2208)

    TMC_driver_data TMC_Stepper::get_driver_data(const DriverEnum index) {
      constexpr uint8_t OTPW_bp = 0, OT_bp = 1;
      constexpr uint8_t S2G_bm = 0b11110; // 2..5
      TMC_driver_data data;
      const auto ds = data.drv_status = driver[index]->tmc->DRV_STATUS();
      data.is_otpw = TEST(ds, OTPW_bp);
      data.is_ot = TEST(ds, OT_bp);
      data.is_s2g = !!(ds & S2G_bm);
      #if ENABLED(TMC_DEBUG)
        constexpr uint32_t CS_ACTUAL_bm = 0x1F0000; // 16:20
        constexpr uint8_t STEALTH_bp = 30, STST_bp = 31;
        #ifdef __AVR__
          // 8-bit optimization saves up to 12 bytes of PROGMEM per axis
          uint8_t spart = ds >> 16;
          data.cs_actual = spart & (CS_ACTUAL_bm >> 16);
          spart = ds >> 24;
          data.is_stealth = TEST(spart, STEALTH_bp - 24);
          data.is_standstill = TEST(spart, STST_bp - 24);
        #else
          constexpr uint8_t CS_ACTUAL_sb = 16;
          data.cs_actual = (ds & CS_ACTUAL_bm) >> CS_ACTUAL_sb;
          data.is_stealth = TEST(ds, STEALTH_bp);
          data.is_standstill = TEST(ds, STST_bp);
        #endif
        data.sg_result_reasonable = false;
      #endif
      return data;
    }

  #elif HAVE_DRV(TMC2660)

    TMC_driver_data TMC_Stepper::get_driver_data(const DriverEnum index) {
      constexpr uint8_t OT_bp = 1, OTPW_bp = 2;
      constexpr uint8_t S2G_bm = 0b11000;
      TMC_driver_data data;
      const auto ds = data.drv_status = driver[index]->tmc->DRVSTATUS();
      uint8_t spart = ds & 0xFF;
      data.is_otpw = TEST(spart, OTPW_bp);
      data.is_ot = TEST(spart, OT_bp);
      data.is_s2g = !!(ds & S2G_bm);
      #if ENABLED(TMC_DEBUG)
        constexpr uint8_t STALL_GUARD_bp = 0;
        constexpr uint8_t STST_bp = 7, SG_RESULT_sp = 10;
        constexpr uint32_t SG_RESULT_bm = 0xFFC00; // 10:19
        data.is_stall = TEST(spart, STALL_GUARD_bp);
        data.is_standstill = TEST(spart, STST_bp);
        data.sg_result = (ds & SG_RESULT_bm) >> SG_RESULT_sp;
        data.sg_result_reasonable = true;
      #endif
      return data;
    }

  #elif HAS_TMCX1X0

    TMC_driver_data TMC_Stepper::get_driver_data(const DriverEnum index) {
      constexpr uint8_t OT_bp = 25, OTPW_bp = 26;
      constexpr uint32_t S2G_bm = 0x18000000;
      #if ENABLED(TMC_DEBUG)
        constexpr uint16_t SG_RESULT_bm = 0x3FF; // 0:9
        constexpr uint8_t STEALTH_bp = 14;
        constexpr uint32_t CS_ACTUAL_bm = 0x1F0000; // 16:20
        constexpr uint8_t STALL_GUARD_bp = 24;
        constexpr uint8_t STST_bp = 31;
      #endif
      TMC_driver_data data;
      const auto ds = data.drv_status = driver[index]->tmc->DRV_STATUS();
      #ifdef __AVR__
        // 8-bit optimization saves up to 70 bytes of PROGMEM per axis
        uint8_t spart;
        #if ENABLED(TMC_DEBUG)
          data.sg_result = ds & SG_RESULT_bm;
          spart = ds >> 8;
          data.is_stealth = TEST(spart, STEALTH_bp - 8);
          spart = ds >> 16;
          data.cs_actual = spart & (CS_ACTUAL_bm >> 16);
        #endif
        spart = ds >> 24;
        data.is_ot = TEST(spart, OT_bp - 24);
        data.is_otpw = TEST(spart, OTPW_bp - 24);
        data.is_s2g = !!(spart & (S2G_bm >> 24));
        #if ENABLED(TMC_DEBUG)
          data.is_stall = TEST(spart, STALL_GUARD_bp - 24);
          data.is_standstill = TEST(spart, STST_bp - 24);
          data.sg_result_reasonable = !data.is_standstill; // sg_result has no reasonable meaning while standstill
        #endif

      #else // !__AVR__

        data.is_ot = TEST(ds, OT_bp);
        data.is_otpw = TEST(ds, OTPW_bp);
        data.is_s2g = !!(ds & S2G_bm);
        #if ENABLED(TMC_DEBUG)
          constexpr uint8_t CS_ACTUAL_sb = 16;
          data.sg_result = ds & SG_RESULT_bm;
          data.is_stealth = TEST(ds, STEALTH_bp);
          data.cs_actual = (ds & CS_ACTUAL_bm) >> CS_ACTUAL_sb;
          data.is_stall = TEST(ds, STALL_GUARD_bp);
          data.is_standstill = TEST(ds, STST_bp);
          data.sg_result_reasonable = !data.is_standstill; // sg_result has no reasonable meaning while standstill
        #endif

      #endif // !__AVR__

      return data;
    }

  #endif

  void TMC_Stepper::monitor_driver(const DriverEnum index, const bool need_update_error_counters, const bool need_debug_reporting) {

    TMC_driver_data data = get_driver_data(index);
    if ((data.drv_status == 0xFFFFFFFF) || (data.drv_status == 0x0)) return;

    if (need_update_error_counters) {
      if (data.is_ot /* | data.s2ga | data.s2gb*/) driver[index]->tmc->error_count++;
      else if (driver[index]->tmc->error_count > 0) driver[index]->tmc->error_count--;

      #if ENABLED(STOP_ON_ERROR)
        if (driver[index]->tmc->error_count >= 10) {
          SERIAL_EOL();
          driver[index]->printLabel();
          SERIAL_MSG(" driver error detected: 0x");
          SERIAL_EV(data.drv_status, HEX);
          if (data.is_ot) SERIAL_EM("overtemperature");
          if (data.is_s2g) SERIAL_EM("coil short circuit");
          #if ENABLED(TMC_DEBUG)
            report_all(true, true, true, true);
          #endif
          printer.kill(PSTR("Driver error"));
        }
      #endif

      // Report if a warning was triggered
      if (data.is_otpw && driver[index]->tmc->otpw_count == 0) {
        char timestamp[14];
        duration_t elapsed = print_job_counter.duration();
        (void)elapsed.toDigital(timestamp, true);
        SERIAL_EOL();
        SERIAL_TXT(timestamp);
        SERIAL_MSG(": ");
        driver[index]->printLabel();
        SERIAL_MSG(" driver overtemperature warning! (");
        SERIAL_VAL(driver[index]->tmc->getMilliamps());
        SERIAL_EM("mA)");
      }

      #if CURRENT_STEP_DOWN > 0
        // Decrease current if is_otpw is true and driver is enabled and there's been more than 4 warnings
        if (data.is_otpw && driver[index]->tmc->otpw_count > 4) {
          uint16_t I_rms = driver[index]->tmc->getMilliamps();
          if (driver[index]->tmc->isEnabled() && I_rms > 100) {
            driver[index]->tmc->rms_current(I_rms - (CURRENT_STEP_DOWN));
            #if ENABLED(REPORT_CURRENT_CHANGE)
              driver[index]->printLabel();
              SERIAL_EMV(" current decreased to ", driver[index]->tmc->getMilliamps());
            #endif
          }
        }
      #endif

      if (data.is_otpw) {
        driver[index]->tmc->otpw_count++;
        driver[index]->tmc->flag_otpw = true;
      }
      else if (driver[index]->tmc->otpw_count > 0) driver[index]->tmc->otpw_count = 0;
    }

    #if ENABLED(TMC_DEBUG)
      if (need_debug_reporting) {
        const uint32_t pwm_scale = get_pwm_scale(index);
        driver[index]->printLabel();
        SERIAL_MV(":", pwm_scale);
        #if ENABLED(TMC_DEBUG)
          #if HAS_TMCX1X0 || HAVE_DRV(TMC2208)
            SERIAL_MV("/", data.cs_actual);
          #endif
          #if TMC_HAS_STALLGUARD
            SERIAL_CHR('/');
            if (data.sg_result_reasonable)
              SERIAL_VAL(data.sg_result);
            else
              SERIAL_CHR('-');
          #endif
        #endif
        SERIAL_CHR('|');
        if (driver[index]->tmc->error_count)      SERIAL_CHR('E');  // Error
        if (data.is_ot)           SERIAL_CHR('O');  // Over-temperature
        if (data.is_otpw)         SERIAL_CHR('W');  // over-temperature pre-Warning
        #if ENABLED(TMC_DEBUG)
          if (data.is_stall)      SERIAL_CHR('G');  // stallGuard
          if (data.is_stealth)    SERIAL_CHR('T');  // stealthChop
          if (data.is_standstill) SERIAL_CHR('I');  // standstIll
        #endif
        if (driver[index]->tmc->flag_otpw)        SERIAL_CHR('F');  // otpw Flag
        SERIAL_CHR('|');
        if (driver[index]->tmc->otpw_count > 0)   SERIAL_VAL(driver[index]->tmc->otpw_count);
        SERIAL_CHR('\t');
      }
    #endif
  }

#endif // MONITOR_DRIVER_STATUS

#if ENABLED(TMC_DEBUG)

  #if HAVE_DRV(TMC2160) || HAVE_DRV(TMC5160)
      void TMC_Stepper::print_vsense(const DriverEnum index) { UNUSED(index); }
    #else
      void TMC_Stepper::print_vsense(const DriverEnum index) { SERIAL_STR(driver[index]->tmc->vsense() ? PSTR("1=.18") : PSTR("0=.325")); }
    #endif

  #define PRINT_TMC_REGISTER(REG_CASE) case TMC_GET_##REG_CASE: print_hex_long(driver[index]->tmc->REG_CASE(), ':'); break

  #if HAVE_DRV(TMC2208)

    void TMC_Stepper::status(const DriverEnum index, const TMCdebugEnum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_VAL(driver[index]->tmc->pwm_scale_sum()); break;
        case TMC_STEALTHCHOP: SERIAL_LOGIC("", driver[index]->tmc->stealth()); break;
        case TMC_S2VSA: if (driver[index]->tmc->s2vsa()) SERIAL_CHR('X'); break;
        case TMC_S2VSB: if (driver[index]->tmc->s2vsb()) SERIAL_CHR('X'); break;
        default: break;
      }
    }

    void TMC_Stepper::parse_type_drv_status(const DriverEnum index, const TMCdrvStatusEnum i) {
      switch (i) {
        case TMC_T157: if (driver[index]->tmc->t157()) SERIAL_CHR('X'); break;
        case TMC_T150: if (driver[index]->tmc->t150()) SERIAL_CHR('X'); break;
        case TMC_T143: if (driver[index]->tmc->t143()) SERIAL_CHR('X'); break;
        case TMC_T120: if (driver[index]->tmc->t120()) SERIAL_CHR('X'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_VAL(driver[index]->tmc->cs_actual()); break;
        default: break;
      }
    }

    void TMC_Stepper::get_ic_registers(const DriverEnum index, const TMCgetRegistersEnum i) {
      UNUSED(index);
      UNUSED(i);
      SERIAL_CHR('\t');
    }

    void TMC_Stepper::get_registers(const DriverEnum index, const TMCgetRegistersEnum i) {
      switch (i) {
        case TMC_AXIS_CODES: SERIAL_CHR('\t'); driver[index]->printLabel(); break;
        PRINT_TMC_REGISTER(GCONF);
        PRINT_TMC_REGISTER(IHOLD_IRUN);
        PRINT_TMC_REGISTER(GSTAT);
        PRINT_TMC_REGISTER(IOIN);
        PRINT_TMC_REGISTER(TPOWERDOWN);
        PRINT_TMC_REGISTER(TSTEP);
        PRINT_TMC_REGISTER(TPWMTHRS);
        PRINT_TMC_REGISTER(CHOPCONF);
        PRINT_TMC_REGISTER(PWMCONF);
        PRINT_TMC_REGISTER(PWM_SCALE);
        PRINT_TMC_REGISTER(DRV_STATUS);
        default: get_ic_registers(index, i); break;
      }
      SERIAL_CHR('\t');
    }

  #elif HAVE_DRV(TMC2660)

    void TMC_Stepper::status(const DriverEnum index, const TMCdebugEnum i)                    { UNUSED(index); UNUSED(i); }
    void TMC_Stepper::parse_type_drv_status(const DriverEnum index, const TMCdrvStatusEnum i) { UNUSED(index); UNUSED(i); }
    void TMC_Stepper::get_ic_registers(const DriverEnum index, const TMCgetRegistersEnum i)   { UNUSED(index); UNUSED(i); }

    void TMC_Stepper::get_registers(const DriverEnum index, const TMCgetRegistersEnum i) {
      switch (i) {
        case TMC_AXIS_CODES: SERIAL_CHR('\t'); driver[index]->printLabel(); break;
        PRINT_TMC_REGISTER(DRVCONF);
        PRINT_TMC_REGISTER(DRVCTRL);
        PRINT_TMC_REGISTER(CHOPCONF);
        PRINT_TMC_REGISTER(DRVSTATUS);
        PRINT_TMC_REGISTER(SGCSCONF);
        PRINT_TMC_REGISTER(SMARTEN);
        default: SERIAL_CHR('\t'); break;
      }
      SERIAL_CHR('\t');
    }

  #elif HAS_TMCX1X0

    void TMC_Stepper::status(const DriverEnum index, const TMCdebugEnum i) {
      switch (i) {
        case TMC_PWM_SCALE: SERIAL_VAL(driver[index]->tmc->PWM_SCALE()); break;
        case TMC_SGT: SERIAL_VAL(driver[index]->tmc->sgt()); break;
        case TMC_STEALTHCHOP: SERIAL_LOGIC("", driver[index]->tmc->en_pwm_mode()); break;
        default: break;
      }
    }

    void TMC_Stepper::parse_type_drv_status(const DriverEnum index, const TMCdrvStatusEnum i) {
      switch (i) {
        case TMC_STALLGUARD: if (driver[index]->tmc->stallguard()) SERIAL_CHR('X'); break;
        case TMC_SG_RESULT:  SERIAL_VAL(driver[index]->tmc->sg_result());           break;
        case TMC_FSACTIVE:   if (driver[index]->tmc->fsactive())   SERIAL_CHR('X'); break;
        case TMC_DRV_CS_ACTUAL: SERIAL_VAL(driver[index]->tmc->cs_actual());        break;
        default: break;
      }
    }

    void TMC_Stepper::get_ic_registers(const DriverEnum index, const TMCgetRegistersEnum i) {
      UNUSED(index);
      switch (i) {
        PRINT_TMC_REGISTER(TCOOLTHRS);
        PRINT_TMC_REGISTER(THIGH);
        PRINT_TMC_REGISTER(COOLCONF);
        default: SERIAL_CHR('\t'); break;
      }
    }

    void TMC_Stepper::get_registers(const DriverEnum index, const TMCgetRegistersEnum i) {
      switch (i) {
        case TMC_AXIS_CODES: SERIAL_CHR('\t'); driver[index]->printLabel(); break;
        PRINT_TMC_REGISTER(GCONF);
        PRINT_TMC_REGISTER(IHOLD_IRUN);
        PRINT_TMC_REGISTER(GSTAT);
        PRINT_TMC_REGISTER(IOIN);
        PRINT_TMC_REGISTER(TPOWERDOWN);
        PRINT_TMC_REGISTER(TSTEP);
        PRINT_TMC_REGISTER(TPWMTHRS);
        PRINT_TMC_REGISTER(CHOPCONF);
        PRINT_TMC_REGISTER(PWMCONF);
        PRINT_TMC_REGISTER(PWM_SCALE);
        PRINT_TMC_REGISTER(DRV_STATUS);
        default: get_ic_registers(index, i); break;
      }
      SERIAL_CHR('\t');
    }

  #endif // HAS_TMCX1X0

  #if HAVE_DRV(TMC2660)
  
    void TMC_Stepper::status(const DriverEnum index, const TMCdebugEnum i, const float spmm) {
      SERIAL_CHR('\t');
      switch (i) {
        case TMC_CODES: driver[index]->printLabel(); break;
        case TMC_ENABLED: SERIAL_LOGIC("", driver[index]->tmc->isEnabled()); break;
        case TMC_CURRENT: SERIAL_VAL(driver[index]->tmc->getMilliamps()); break;
        case TMC_RMS_CURRENT: SERIAL_VAL(driver[index]->tmc->rms_current()); break;
        case TMC_MAX_CURRENT: SERIAL_VAL((float)driver[index]->tmc->rms_current() * 1.41, 0); break;
        case TMC_IRUN:
          SERIAL_VAL(driver[index]->tmc->cs(), DEC);
          SERIAL_MSG("/31");
          break;
        case TMC_VSENSE: SERIAL_STR(driver[index]->tmc->vsense() ? PSTR("1=.165") : PSTR("0=.310")); break;
        case TMC_MICROSTEPS: SERIAL_VAL(driver[index]->tmc->microsteps()); break;
        //case TMC_OTPW: SERIAL_LOGIC("", driver[index]->tmc->otpw()); break;
        //case TMC_OTPW_TRIGGERED: SERIAL_LOGIC("", driver[index]->tmc->getOTPW()); break;
        case TMC_SGT: SERIAL_VAL(driver[index]->tmc->sgt(), DEC); break;
        case TMC_TOFF: SERIAL_VAL(driver[index]->tmc->toff(), DEC); break;
        case TMC_TBL: SERIAL_VAL(driver[index]->tmc->blank_time(), DEC); break;
        case TMC_HEND: SERIAL_VAL(driver[index]->tmc->hysteresis_end(), DEC); break;
        case TMC_HSTRT: SERIAL_VAL(driver[index]->tmc->hysteresis_start(), DEC); break;
        default: break;
      }
    }

  #else

    void TMC_Stepper::status(const DriverEnum index, const TMCdebugEnum i, const float spmm) {
      SERIAL_CHR('\t');
      switch (i) {
        case TMC_CODES: driver[index]->printLabel(); break;
        case TMC_ENABLED: SERIAL_LOGIC("", driver[index]->tmc->isEnabled()); break;
        case TMC_CURRENT: SERIAL_VAL(driver[index]->tmc->getMilliamps()); break;
        case TMC_RMS_CURRENT: SERIAL_VAL(driver[index]->tmc->rms_current()); break;
        case TMC_MAX_CURRENT: SERIAL_VAL((float)driver[index]->tmc->rms_current() * 1.41, 0); break;
        case TMC_IRUN:
          SERIAL_VAL(driver[index]->tmc->irun());
          SERIAL_MSG("/31");
          break;
        case TMC_IHOLD:
          SERIAL_VAL(driver[index]->tmc->ihold());
          SERIAL_MSG("/31");
          break;
        case TMC_CS_ACTUAL:
          SERIAL_VAL(driver[index]->tmc->cs_actual());
          SERIAL_MSG("/31");
          break;
        case TMC_VSENSE: print_vsense(index); break;
        case TMC_MICROSTEPS: SERIAL_VAL(driver[index]->tmc->microsteps()); break;
        case TMC_TSTEP: {
          uint32_t tstep_value = driver[index]->tmc->TSTEP();
          if (tstep_value == 0xFFFFF) SERIAL_MSG("max");
          else SERIAL_VAL(tstep_value);
        } break;
        #if ENABLED(HYBRID_THRESHOLD)
          case TMC_TPWMTHRS: SERIAL_VAL(uint32_t(driver[index]->tmc->TPWMTHRS())); break;
          case TMC_TPWMTHRS_MMS: {
            const uint32_t tpwmthrs_val = driver[index]->tmc->get_pwm_thrs();
            if (tpwmthrs_val) SERIAL_VAL(tpwmthrs_val); else SERIAL_CHR('-');
          } break;
        #endif
        case TMC_OTPW: SERIAL_LOGIC("", driver[index]->tmc->otpw()); break;
        #if ENABLED(MONITOR_DRIVER_STATUS)
          case TMC_OTPW_TRIGGERED: SERIAL_LOGIC("", driver[index]->tmc->getOTPW()); break;
        #endif
        case TMC_TOFF: SERIAL_VAL(driver[index]->tmc->toff()); break;
        case TMC_TBL: SERIAL_VAL(driver[index]->tmc->blank_time()); break;
        case TMC_HEND: SERIAL_VAL(driver[index]->tmc->hysteresis_end()); break;
        case TMC_HSTRT: SERIAL_VAL(driver[index]->tmc->hysteresis_start()); break;
        default: status(index, i); break;
      }
    }

  #endif

  void TMC_Stepper::parse_drv_status(const DriverEnum index, const TMCdrvStatusEnum i) {
    SERIAL_CHR('\t');
    switch (i) {
      case TMC_DRV_CODES:     driver[index]->printLabel();                          break;
      case TMC_STST:          if (driver[index]->tmc->stst())     SERIAL_CHR('X');  break;
      case TMC_OLB:           if (driver[index]->tmc->olb())      SERIAL_CHR('X');  break;
      case TMC_OLA:           if (driver[index]->tmc->ola())      SERIAL_CHR('X');  break;
      case TMC_S2GB:          if (driver[index]->tmc->s2gb())     SERIAL_CHR('X');  break;
      case TMC_S2GA:          if (driver[index]->tmc->s2ga())     SERIAL_CHR('X');  break;
      case TMC_DRV_OTPW:      if (driver[index]->tmc->otpw())     SERIAL_CHR('X');  break;
      case TMC_OT:            if (driver[index]->tmc->ot())       SERIAL_CHR('X');  break;
      case TMC_DRV_STATUS_HEX: {
        const uint32_t drv_status = driver[index]->tmc->DRV_STATUS();
        SERIAL_CHR('\t');
        driver[index]->printLabel();
        SERIAL_CHR('\t');
        print_hex_long(drv_status, ':');
        if (drv_status == 0xFFFFFFFF || drv_status == 0) SERIAL_MSG("\t Bad response!");
        SERIAL_EOL();
        break;
      }
      default: parse_type_drv_status(index, i); break;
    }
  }

  void TMC_Stepper::debug_loop(const TMCdebugEnum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {

    if (print_x) {
      #if AXIS_HAS_TMC(X)
        status(X_DRV, i, mechanics.data.axis_steps_per_mm[X_AXIS]);
      #endif
      #if AXIS_HAS_TMC(X2)
        status(X2_DRV, i, mechanics.data.axis_steps_per_mm[X_AXIS]);
      #endif
    }

    if (print_y) {
      #if AXIS_HAS_TMC(Y)
        status(Y_DRV, i, mechanics.data.axis_steps_per_mm[Y_AXIS]);
      #endif
      #if AXIS_HAS_TMC(Y2)
        status(Y2_DRV, i, mechanics.data.axis_steps_per_mm[Y_AXIS]);
      #endif
    }

    if (print_z) {
      #if AXIS_HAS_TMC(Z)
        status(Z_DRV, i, mechanics.data.axis_steps_per_mm[Z_AXIS]);
      #endif
      #if AXIS_HAS_TMC(Z2)
        status(Z2_DRV,i, mechanics.data.axis_steps_per_mm[Z_AXIS]);
      #endif
      #if AXIS_HAS_TMC(Z3)
        status(Z3_DRV, i, mechanics.data.axis_steps_per_mm[Z_AXIS]);
      #endif
    }

    if (print_e) {
      #if AXIS_HAS_TMC(E0)
        status(E0_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(0)]);
      #endif
      #if AXIS_HAS_TMC(E1)
        status(E1_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(1)]);
      #endif
      #if AXIS_HAS_TMC(E2)
        status(E2_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(2)]);
      #endif
      #if AXIS_HAS_TMC(E3)
        status(E3_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(3)]);
      #endif
      #if AXIS_HAS_TMC(E4)
        status(E4_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(4)]);
      #endif
      #if AXIS_HAS_TMC(E5)
        status(E5_DRV, i, mechanics.data.axis_steps_per_mm[E_AXIS_N(5)]);
      #endif
    }

    SERIAL_EOL();
  }

  void TMC_Stepper::status_loop(const TMCdrvStatusEnum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {
    if (print_x) {
      #if AXIS_HAS_TMC(X)
        parse_drv_status(X_DRV, i);
      #endif
      #if AXIS_HAS_TMC(X2)
        parse_drv_status(X2_DRV, i);
      #endif
    }

    if (print_y) {
      #if AXIS_HAS_TMC(Y)
        parse_drv_status(Y_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Y2)
        parse_drv_status(Y2_DRV, i);
      #endif
    }

    if (print_z) {
      #if AXIS_HAS_TMC(Z)
        parse_drv_status(Z_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Z2)
        parse_drv_status(Z2_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Z3)
        parse_drv_status(Z3_DRV, i);
      #endif
    }

    if (print_e) {
      #if AXIS_HAS_TMC(E0)
        parse_drv_status(E0_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E1)
        parse_drv_status(E1_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E2)
        parse_drv_status(E2_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E3)
        parse_drv_status(E3_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E4)
        parse_drv_status(E4_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E5)
        parse_drv_status(E5_DRV, i);
      #endif
    }

    SERIAL_EOL();
  }

  void TMC_Stepper::get_registers(const TMCgetRegistersEnum i, const bool print_x, const bool print_y, const bool print_z, const bool print_e) {

    if (print_x) {
      #if AXIS_HAS_TMC(X)
        get_registers(X_DRV, i);
      #endif
      #if AXIS_HAS_TMC(X2)
        get_registers(X2_DRV, i);
      #endif
    }

    if (print_y) {
      #if AXIS_HAS_TMC(Y)
        get_registers(Y_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Y2)
        get_registers(Y2_DRV, i);
      #endif
    }

    if (print_z) {
      #if AXIS_HAS_TMC(Z)
        get_registers(Z_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Z2)
        get_registers(Z2_DRV, i);
      #endif
      #if AXIS_HAS_TMC(Z3)
        get_registers(Z3_DRV, i);
      #endif
    }

    if (print_e) {
      #if AXIS_HAS_TMC(E0)
        get_registers(E0_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E1)
        get_registers(E1_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E2)
        get_registers(E2_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E3)
        get_registers(E3_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E4)
        get_registers(E4_DRV, i);
      #endif
      #if AXIS_HAS_TMC(E5)
        get_registers(E5_DRV, i);
      #endif
    }

    SERIAL_EOL();
  }

#endif // TMC_DEBUG

#endif // HAS_TRINAMIC
