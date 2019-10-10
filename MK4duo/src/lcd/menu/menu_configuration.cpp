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

//
// Configuration Menu
//

#include "../../../MK4duo.h"

#if HAS_LCD_MENU

#define HAS_DEBUG_MENU ENABLED(LCD_PROGRESS_BAR_TEST)

void menu_advanced_settings();
void menu_delta_calibrate();

#if ENABLED(LCD_PROGRESS_BAR_TEST)

  static void progress_bar_test() {
    static int8_t bar_percent = 0;
    if (lcdui.use_click()) {
      lcdui.goto_previous_screen();
      LCD_SET_CHARSET(CHARSET_MENU);
      return;
    }
    bar_percent += (int8_t)lcdui.encoderPosition;
    LIMIT(bar_percent, 0, 100);
    lcdui.encoderPosition = 0;
    draw_menu_item_static(0, PSTR(MSG_PROGRESS_BAR_TEST), SS_CENTER|SS_INVERT);
    lcd_put_int((LCD_WIDTH) / 2 - 2, LCD_HEIGHT - 2, bar_percent); lcd_put_wchar('%');
    lcd_moveto(0, LCD_HEIGHT - 1); lcd_draw_progress_bar(bar_percent);
  }

  void _progress_bar_test() {
    lcdui.goto_screen(progress_bar_test);
    lcdui.set_custom_characters(CHARSET_INFO);
  }

#endif // LCD_PROGRESS_BAR_TEST

#if HAS_DEBUG_MENU

  void menu_debug() {
    START_MENU();

    BACK_ITEM(MSG_CONFIGURATION);

    #if ENABLED(LCD_PROGRESS_BAR_TEST)
      SUBMENU(MSG_PROGRESS_BAR_TEST, _progress_bar_test);
    #endif

    END_MENU();
  }

#endif

void menu_tool_change() {
  START_MENU();
  BACK_ITEM(MSG_CONFIGURATION);
  #if ENABLED(TOOL_CHANGE_FIL_SWAP)
    #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
      static constexpr float max_extrude_lenght = EXTRUDE_MAXLENGTH;
    #else
      static constexpr float max_extrude_lenght = 500;
    #endif
    EDIT_ITEM(float3, MSG_FILAMENT_SWAP_LENGTH, &tools.data.swap_length, 0, max_extrude_lenght);
    EDIT_ITEM(float3, MSG_FILAMENT_PURGE_LENGTH, &toolchange_settings.purge_lenght, 0, max_extrude_lenght);
    EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_RETRACT_SPD, &tools.data.retract_speed, 10, 5400);
    EDIT_ITEM_FAST(int4, MSG_SINGLENOZZLE_PRIME_SPD, &tools.data.prime_speed, 10, 5400);
  #endif
  EDIT_ITEM(float3, MSG_TOOL_CHANGE_ZLIFT, &nozzle.data.park_point.z, 0, 10);
  END_MENU();
}

#if MAX_HOTEND > 1

  void menu_tool_offsets() {

    auto _recalc_offsets = []{
      if (tools.data.extruder.active && mechanics.axis_unhomed_error()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        commands.inject_P(PSTR("G28")); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        tools.data.extruder.active = 0;
      }
    };

    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);

    #if ENABLED(DUAL_X_CARRIAGE)
      EDIT_ITEM_FAST(float51, MSG_X_OFFSET, &nozzle.data.hotend_offset[1].x, float(X2_HOME_POS - 25), float(X2_HOME_POS + 25), _recalc_offsets);
      EDIT_ITEM_FAST(float52sign, MSG_Y_OFFSET, &nozzle.data.hotend_offset[1].y, -10.0, 10.0, _recalc_offsets);
      EDIT_ITEM_FAST(float52sign, MSG_Z_OFFSET, &nozzle.data.hotend_offset[1].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
    #else
      for (uint8_t h = 1; h < thermalManager.data.hotends; h++) {
        EDIT_ITEM_FAST_I(float52sign, MSG_X_OFFSET, h, &nozzle.data.hotend_offset[h].x, -10.0, 10.0, _recalc_offsets);
        EDIT_ITEM_FAST_I(float52sign, MSG_Y_OFFSET, h, &nozzle.data.hotend_offset[h].y, -10.0, 10.0, _recalc_offsets);
        EDIT_ITEM_FAST_I(float52sign, MSG_Z_OFFSET, h, &nozzle.data.hotend_offset[h].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets);
      }
    #endif

    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, [](){ eeprom.store(); });
    #endif
    END_MENU();
  }
#endif

#if ENABLED(DUAL_X_CARRIAGE)

  void menu_DXC() {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);

    GCODES_ITEM(MSG_DXC_MODE_AUTOPARK, PSTR("M605 S1\nG28 X\nG1 X100"));
    const bool need_g28 = !(mechanics.home_flag.YHomed && mechanics.home_flag.ZHomed);
    GCODES_ITEM(MSG_DXC_MODE_DUPLICATE, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100")                // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100")
    );
    GCODES_ITEM(MSG_DXC_MODE_MIRRORED_COPY, need_g28
      ? PSTR("M605 S1\nT0\nG28\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")  // If Y or Z is not homed, do a full G28 first
      : PSTR("M605 S1\nT0\nM605 S2 X200\nG28 X\nG1 X100\nM605 S3 X200")
    );
    GCODES_ITEM(MSG_DXC_MODE_FULL_CTRL, PSTR("M605 S0\nG28 X"));
    END_MENU();
  }

#endif

#if HAS_BLTOUCH

  #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)

    void bltouch_report() {
      SERIAL_EMV("EEPROM BLTouch Last Mode - ", (int)bltouch.last_mode);
      SERIAL_MSG("Configuration BLTouch Mode - BLTOUCH_MODE_");
      SERIAL_L(BLTOUCH_MODE_5V ? PSTR("5V") : PSTR("OD"));
      char mess[21];
      strcpy_P(mess, PSTR("BLTouch Mode - "));
      strcpy_P(&mess[15], bltouch.last_mode ? PSTR("5V") : PSTR("OD"));
      lcdui.set_status(mess);
      lcdui.return_to_status();
    }

  #endif

  void menu_bltouch() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);
    ACTION_ITEM(MSG_BLTOUCH_RESET, bltouch.cmd_reset);
    ACTION_ITEM(MSG_BLTOUCH_SELFTEST, bltouch.cmd_selftest);
    ACTION_ITEM(MSG_BLTOUCH_DEPLOY, bltouch.cmd_deploy);
    ACTION_ITEM(MSG_BLTOUCH_STOW, bltouch.cmd_stow);
    ACTION_ITEM(MSG_BLTOUCH_MODE_SW, bltouch.cmd_mode_SW);
    #if ENABLED(BLTOUCH_LCD_VOLTAGE_MENU)
      SUBMENU(MSG_BLTOUCH_MODE_5V, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_5V), PSTR(MSG_BUTTON_CANCEL), bltouch.cmd_mode_5V, lcdui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      SUBMENU(MSG_BLTOUCH_MODE_OD, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_OD), PSTR(MSG_BUTTON_CANCEL), bltouch.cmd_mode_OD, lcdui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      ACTION_ITEM(MSG_BLTOUCH_MODE_STORE, bltouch.cmd_mode_store);
      SUBMENU(MSG_BLTOUCH_MODE_STORE_5V, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_STORE_5V), PSTR(MSG_BUTTON_CANCEL), bltouch.mode_conv_5V, lcdui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      SUBMENU(MSG_BLTOUCH_MODE_STORE_OD, []{
        do_select_screen(PSTR(MSG_BLTOUCH_MODE_STORE_OD), PSTR(MSG_BUTTON_CANCEL), bltouch.mode_conv_OD, lcdui.goto_previous_screen, PSTR(MSG_BLTOUCH_MODE_CHANGE));
      });
      ACTION_ITEM(MSG_BLTOUCH_MODE_ECHO, bltouch_report);
    #endif
    END_MENU();
  }

#endif

#if HAS_CASE_LIGHT

  void menu_case_light() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);
    EDIT_ITEM(percent, MSG_CASE_LIGHT_BRIGHTNESS, &caselight.brightness, 0, 255, caselight.update, true);
    EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&caselight.status, caselight.update);
    END_MENU();
  }

#endif

#if ENABLED(FWRETRACT)

  void menu_config_retract() {
    START_MENU();
    BACK_ITEM(MSG_CONTROL);
    EDIT_ITEM(bool, MSG_AUTORETRACT, &fwretract.autoretract_enabled, fwretract.refresh_autoretract);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT, &fwretract.data.retract_length, 0, 100);
    EDIT_ITEM(float3, MSG_CONTROL_RETRACTF, &fwretract.data.retract_feedrate_mm_s, 1, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_ZHOP, &fwretract.data.retract_zlift, 0, 999);
    EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER, &fwretract.data.retract_recover_length, -100, 100);
    EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVERF, &fwretract.data.retract_recover_feedrate_mm_s, 1, 999);
    if (tools.data.extruder.total > 1) {
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_SWAP, &fwretract.data.swap_retract_length, 0, 100);
      EDIT_ITEM(float52sign, MSG_CONTROL_RETRACT_RECOVER_SWAP, &fwretract.data.swap_retract_recover_length, -100, 100);
      EDIT_ITEM(float3, MSG_CONTROL_RETRACT_RECOVER_SWAPF, &fwretract.data.swap_retract_recover_feedrate_mm_s, 1, 999);
    }
    END_MENU();
  }

#endif

#if ENABLED(LASER)

  float focalLength = 0;

  static void laser_test_fire(const uint8_t power, const int dwell) {
    commands.inject_P(PSTR("M80"));  // Enable laser accessories since we don't know if its been done (and there's no penalty for doing it again).
    laser.fire(power);
    delay(dwell);
    laser.extinguish();
  }

  void menu_laser_test_fire() {
    START_MENU();
     BACK_ITEM("Laser Functions");
     ACTION_ITEM("Weak ON",             [](){ laser.fire(0.3); });
     ACTION_ITEM(" 20%  50ms",          [](){ laser_test_fire( 20,  50); });
     ACTION_ITEM(" 20% 100ms",          [](){ laser_test_fire( 20, 100); });
     ACTION_ITEM("100%  50ms",          [](){ laser_test_fire(100,  50); });
     ACTION_ITEM("100% 100ms",          [](){ laser_test_fire(100, 100); });
     ACTION_ITEM("Warm-up Laser 2sec",  [](){ laser_test_fire( 15,2000); });
     END_MENU();
  }  

  static void laser_set_focus(const float f_length) {
    if (mechanics.axis_unhomed_error(HOME_Z))
      commands.inject_P(PSTR("G28 Z F150"));

    focalLength = f_length;
    float focus = LASER_FOCAL_HEIGHT - f_length;
    char cmd[20];
    sprintf_P(cmd, PSTR("G0 Z%f F150"), focus);
    lcd_enqueue_one_now(cmd);
  }

  void menu_laser_focus() {
    START_MENU();
    BACK_ITEM("Laser Functions");
    ACTION_ITEM("1mm",          [](){ laser_set_focus(1); });
    ACTION_ITEM("2mm",          [](){ laser_set_focus(2); });
    ACTION_ITEM("3mm - 1/8in",  [](){ laser_set_focus(3); });
    ACTION_ITEM("4mm",          [](){ laser_set_focus(4); });
    ACTION_ITEM("5mm",          [](){ laser_set_focus(5); });
    ACTION_ITEM("6mm - 1/4in",  [](){ laser_set_focus(6); });
    ACTION_ITEM("7mm",          [](){ laser_set_focus(7); });
    EDIT_ITEM(float52, "Custom", &focalLength, 0, LASER_FOCAL_HEIGHT, [](){ laser_set_focus(focalLength); });
    END_MENU();
  }

  void menu_laser() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);
    SUBMENU("Set Focus", menu_laser_focus);
    SUBMENU("Test Fire", menu_laser_test_fire);
    #if ENABLED(LASER_PERIPHERALS)
      if (laser.peripherals_ok()) {
        GCODES_ITEM("Turn On Pumps/Fans", PSTR("M80"));
      }
      else if (!printer.isPrinting()) {
        GCODES_ITEM("Turn Off Pumps/Fans", PSTR("M81"));
      }
    #endif // LASER_PERIPHERALS
    END_MENU();
  }

#endif // LASER

#if DISABLED(SLIM_LCD_MENUS)

  void _menu_configuration_preheat_settings(const uint8_t material) {
    START_MENU();
    BACK_ITEM(MSG_CONFIGURATION);
    #if MAX_FAN > 0
      EDIT_ITEM(int3, MSG_FAN_SPEED, &lcdui.preheat_fan_speed[material], 0, 255);
    #endif
    #if MAX_HOTEND > 0
      EDIT_ITEM(int3, MSG_NOZZLE, &lcdui.preheat_hotend_temp[material], thermalManager.hotend_mintemp_all(), thermalManager.hotend_maxtemp_all());
    #endif
    #if MAX_BED > 0
      EDIT_ITEM(int3, MSG_BED, &lcdui.preheat_bed_temp[material], thermalManager.bed_mintemp_all(), thermalManager.bed_maxtemp_all());
    #endif
    #if MAX_CHAMBER > 0
      EDIT_ITEM(int3, MSG_CHAMBER, &lcdui.preheat_chamber_temp[material], thermalManager.chamber_mintemp_all(), thermalManager.chamber_maxtemp_all());
    #endif
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, [](){ eeprom.store(); });
    #endif
    END_MENU();
  }

  void menu_preheat_material1_settings() { _menu_configuration_preheat_settings(0); }
  void menu_preheat_material2_settings() { _menu_configuration_preheat_settings(1); }
  void menu_preheat_material3_settings() { _menu_configuration_preheat_settings(2); }

#endif

void menu_configuration() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  //
  // Debug Menu when certain options are enabled
  //
  #if HAS_DEBUG_MENU
    SUBMENU(MSG_DEBUG_MENU, menu_debug);
  #endif

  SUBMENU(MSG_ADVANCED_SETTINGS, menu_advanced_settings);

  const bool busy = printer.isPrinting();
  if (!busy) {

    #if MECH(DELTA)
      SUBMENU(MSG_DELTA_CALIBRATE, menu_delta_calibrate);
    #endif

    #if HOTENDS > 1
      SUBMENU(MSG_OFFSETS_MENU, menu_tool_offsets);
    #endif

    #if ENABLED(DUAL_X_CARRIAGE)
      SUBMENU(MSG_DXC_MENU, menu_DXC);
    #endif

    #if HAS_BLTOUCH
      SUBMENU(MSG_BLTOUCH, menu_bltouch);
    #endif
  }

  if (tools.data.extruder.total > 1) SUBMENU(MSG_TOOL_CHANGE, menu_tool_change);

  //
  // Set Case light on/off/brightness
  //
  #if HAS_CASE_LIGHT
    if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN))
      SUBMENU(MSG_CASE_LIGHT, menu_case_light);
    else
      EDIT_ITEM(bool, MSG_CASE_LIGHT, (bool*)&caselight.status, caselight.update);
  #endif

  #if HAS_LCD_CONTRAST
    EDIT_ITEM(uint8, MSG_CONTRAST, &lcdui.contrast, LCD_CONTRAST_MIN, LCD_CONTRAST_MAX, lcdui.refresh_contrast, true);
  #endif

  if (printer.mode == PRINTER_MODE_FFF) {
    #if ENABLED(IDLE_OOZING_PREVENT)
      EDIT_ITEM(bool, MSG_IDLEOOZING, &printer.IDLE_OOZING_enabled);
    #endif

    #if ENABLED(FWRETRACT)
      SUBMENU(MSG_RETRACT, menu_config_retract);
    #endif

    #if DISABLED(SLIM_LCD_MENUS)
      // Preheat configurations
      SUBMENU(MSG_PREHEAT_1_SETTINGS, menu_preheat_material1_settings);
      SUBMENU(MSG_PREHEAT_2_SETTINGS, menu_preheat_material2_settings);
      SUBMENU(MSG_PREHEAT_3_SETTINGS, menu_preheat_material3_settings);
    #endif
  }

  #if HAS_SD_RESTART
    EDIT_ITEM(bool, MSG_RESTART, &restart.enabled, restart.changed);
  #endif

  #if DISABLED(SLIM_LCD_MENUS)

    switch (sound.data.mode) {
      case SOUND_MODE_ON:
        ACTION_ITEM(MSG_SOUND_MODE_ON, sound.cyclestate);
        break;
      case SOUND_MODE_SILENT:
        ACTION_ITEM(MSG_SOUND_MODE_SILENT, sound.cyclestate);
        break;
      case SOUND_MODE_MUTE:
        ACTION_ITEM(MSG_SOUND_MODE_MUTE, sound.cyclestate);
        break;
      default:
        ACTION_ITEM(MSG_SOUND_MODE_ON, sound.cyclestate);
    }

  #endif

  #if ENABLED(EEPROM_SETTINGS)
    ACTION_ITEM(MSG_STORE_EEPROM, [](){ eeprom.store(); });
    if (!busy)
      ACTION_ITEM(MSG_LOAD_EEPROM, [](){ eeprom.load(); });
  #endif

  if (!busy)
    ACTION_ITEM(MSG_RESTORE_FAILSAFE, [](){ eeprom.reset(); });

  END_MENU();
}

#endif // HAS_LCD_MENU
