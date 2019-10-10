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
// Mixer Menu
//

#include "../../../MK4duo.h"

#if HAS_LCD_MENU && ENABLED(COLOR_MIXING_EXTRUDER) && DISABLED(NEXTION)

#include "../ultralcd/dogm/ultralcd_dogm.h"
#include "../ultralcd/ultralcd.h"
#include "../ultralcd/lcdprint.h"

#define CHANNEL_MIX_EDITING !HAS_GRADIENT_MIX

#if HAS_GRADIENT_MIX

  void lcd_mixer_gradient_z_start_edit() {
    lcdui.defer_status_screen();
    ENCODER_RATE_MULTIPLY(true);
    if (lcdui.encoderPosition != 0) {
      mixer.gradient.start_z += float((int16_t)lcdui.encoderPosition) * 0.1;
      lcdui.encoderPosition = 0;
      NOLESS(mixer.gradient.start_z, 0);
      NOMORE(mixer.gradient.start_z, Z_MAX_BED);
    }
    if (lcdui.should_draw()) {
      char tmp[21];
      sprintf_P(tmp, PSTR(MSG_START_Z ": %4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      LCDPRINT(tmp);
    }

    if (lcdui.lcd_clicked) {
      if (mixer.gradient.start_z > mixer.gradient.end_z)
        mixer.gradient.end_z = mixer.gradient.start_z;
      mixer.refresh_gradient();
      lcdui.goto_previous_screen();
    }
  }

  void lcd_mixer_gradient_z_end_edit() {
    lcdui.defer_status_screen();
    ENCODER_RATE_MULTIPLY(true);
    if (lcdui.encoderPosition != 0) {
      mixer.gradient.end_z += float((int16_t)lcdui.encoderPosition) * 0.1;
      lcdui.encoderPosition = 0;
      NOLESS(mixer.gradient.end_z, 0);
      NOMORE(mixer.gradient.end_z, Z_MAX_BED);
    }

    if (lcdui.should_draw()) {
      char tmp[21];
      sprintf_P(tmp, PSTR(MSG_END_Z ": %4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      SETCURSOR(2, (LCD_HEIGHT - 1) / 2);
      LCDPRINT(tmp);
    }

    if (lcdui.lcd_clicked) {
      if (mixer.gradient.end_z < mixer.gradient.start_z)
        mixer.gradient.start_z = mixer.gradient.end_z;
      mixer.refresh_gradient();
      lcdui.goto_previous_screen();
    }
  }

  void lcd_mixer_edit_gradient_menu() {
    START_MENU();
    BACK_ITEM(MSG_GRADIENT);

    EDIT_ITEM_I(int8, MSG_START_VTOOL, NO_ITEM, &mixer.gradient.start_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);
    EDIT_ITEM(int8, MSG_END_VTOOL, NO_ITEM, &mixer.gradient.end_vtool, 0, MIXING_VIRTUAL_TOOLS - 1, mixer.refresh_gradient);

    char tmp[10];

    SUBMENU(MSG_START_Z ":", lcd_mixer_gradient_z_start_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.start_z), int(mixer.gradient.start_z * 10) % 10);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();

    SUBMENU(MSG_END_Z ":", lcd_mixer_gradient_z_end_edit);
    MENU_ITEM_ADDON_START(9);
      sprintf_P(tmp, PSTR("%4d.%d mm"), int(mixer.gradient.end_z), int(mixer.gradient.end_z * 10) % 10);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();

    END_MENU();
  }

  void _lcd_draw_mix(const uint8_t y) {
    char tmp[21];
    sprintf_P(tmp, PSTR(MSG_MIX ":    %3d%% %3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
    SETCURSOR(2, y);
    LCDPRINT(tmp);
  }

#endif // HAS_GRADIENT_MIX

static uint8_t v_index = 0;

void _lcd_mixer_select_vtool() {
  mixer.T(v_index);
  #if HAS_GRADIENT_MIX
    _lcd_draw_mix(LCD_HEIGHT - 1);
  #endif
}

#if CHANNEL_MIX_EDITING

  void _lcd_mixer_cycle_mix() {
    uint16_t bits = 0;
    MIXING_STEPPER_LOOP(i) if (mixer.collector[i]) SBI(bits, i);
    bits = (bits + 1) & (_BV(MIXING_STEPPERS) - 1);
    if (!bits) ++bits;
    MIXING_STEPPER_LOOP(i) mixer.collector[i] = TEST(bits, i) ? 10.0f : 0.0f;
    lcdui.refresh();
  }

  void _lcd_mixer_commit_vtool() {
    mixer.normalize();
    lcdui.goto_previous_screen();
  }

#endif

void lcd_mixer_mix_edit() {

  #if CHANNEL_MIX_EDITING

    #define EDIT_COLOR(N) 

    START_MENU();
    BACK_ITEM(MSG_MIXER);
    LOOP_DRV_MIX() 
      EDIT_ITEM_FAST_I(float52, MSG_MIX_COMPONENT, d, &mixer.collector[d], 0, 10);

    ACTION_ITEM(MSG_CYCLE_MIX, _lcd_mixer_cycle_mix);
    ACTION_ITEM(MSG_COMMIT_VTOOL, _lcd_mixer_commit_vtool);
    END_MENU();

  #elif HAS_GRADIENT_MIX

    if (lcdui.encoderPosition != 0) {
      mixer.mix[0] += (int16_t)lcdui.encoderPosition;
      lcdui.encoderPosition = 0;
      if (mixer.mix[0] < 0) mixer.mix[0] += 101;
      if (mixer.mix[0] > 100) mixer.mix[0] -= 101;
      mixer.mix[1] = 100 - mixer.mix[0];
    }
    _lcd_draw_mix((LCD_HEIGHT - 1) / 2);

    if (lcdui.lcd_clicked) {
      mixer.update_vtool_from_mix();
      lcdui.goto_previous_screen();
    }

  #else

    START_MENU();
    BACK_ITEM(MSG_MIXER);
    END_MENU();

  #endif
}

#if HAS_GRADIENT_MIX

  //
  // Toggle Dual-Mix
  //
  inline void _lcd_mixer_toggle_mix() {
    mixer.mix[0] = mixer.mix[0] == 100 ? 0 : 100;
    mixer.mix[1] = 100 - mixer.mix[0];
    mixer.update_vtool_from_mix();
    lcdui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  }

#endif

#if CHANNEL_MIX_EDITING

  //
  // Prepare and Edit Mix
  //
  inline void _lcd_goto_mix_edit() {
    mixer.refresh_collector(10, v_index);
    lcdui.goto_screen(lcd_mixer_mix_edit);
    lcd_mixer_mix_edit();
  }

#endif

#if HAS_GRADIENT_MIX

  //
  // Reverse Gradient
  //
  inline void _lcd_mixer_reverse_gradient() {
    const uint8_t sv = mixer.gradient.start_vtool;
    mixer.gradient.start_vtool = mixer.gradient.end_vtool;
    mixer.gradient.end_vtool = sv;
    mixer.refresh_gradient();
    lcdui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
  }

#endif

//
// Reset All V-Tools
//
void menu_mixer_vtools_reset_confirm() {
  do_select_screen(
    PSTR(MSG_BUTTON_RESET), PSTR(MSG_BUTTON_CANCEL),
    []{
      mixer.reset_vtools();
      LCD_MESSAGEPGM(MSG_VTOOLS_RESET);
      lcdui.return_to_status();
    },
    lcdui.goto_previous_screen,
    PSTR(MSG_RESET_VTOOLS), nullptr, PSTR("?")
  );
}

void menu_mixer() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  v_index = mixer.get_current_vtool();
  EDIT_ITEM(uint8, MSG_ACTIVE_VTOOL, &v_index, 0, MIXING_VIRTUAL_TOOLS - 1, _lcd_mixer_select_vtool
    #if HAS_GRADIENT_MIX
      , true
    #endif
  );

  #if HAS_GRADIENT_MIX
  {
    char tmp[10];
    MENU_ITEM_ADDON_START(10);
      mixer.update_mix_from_vtool();
      sprintf_P(tmp, PSTR("%3d;%3d%%"), int(mixer.mix[0]), int(mixer.mix[1]));
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();
    ACTION_ITEM(MSG_TOGGLE_MIX, _lcd_mixer_toggle_mix);
  }
  #else
    SUBMENU(MSG_MIX, _lcd_goto_mix_edit);
  #endif

  SUBMENU(MSG_RESET_VTOOLS, menu_mixer_vtools_reset_confirm);

  #if HAS_GRADIENT_MIX
  {
    char tmp[10];
    SUBMENU(MSG_GRADIENT, lcd_mixer_edit_gradient_menu);
    MENU_ITEM_ADDON_START(10);
      sprintf_P(tmp, PSTR("T%i->T%i"), mixer.gradient.start_vtool, mixer.gradient.end_vtool);
      LCDPRINT(tmp);
    MENU_ITEM_ADDON_END();
    ACTION_ITEM(MSG_REVERSE_GRADIENT, _lcd_mixer_reverse_gradient);
  }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && ENABLED(COLOR_MIXING_EXTRUDER)
