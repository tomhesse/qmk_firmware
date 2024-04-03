/* Copyright 2024 Tom Hesse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Define available layers
enum layers {
    _BASE,
    _FUNC
};

enum custom_keycodes {
    M_JIGL = SAFE_RANGE // Used for mouse jiggler macro
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap (Base Layer) Default Layer
   * ,----------------------------------------------------------------.
   * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|Backsp |Home|
   * |----------------------------------------------------------------|
   * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  |End |
   * |----------------------------------------------------------------|
   * |Caps   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return |PgUp|
   * |----------------------------------------------------------------|
   * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift | Up|PgDn|
   * |----------------------------------------------------------------|
   * |Ctrl|Win |Alt |        Space          |Alt| FN|Ctrl|Lef|Dow|Rig |
   * `----------------------------------------------------------------'
   */
[_BASE] = LAYOUT_65_ansi(
  QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_HOME,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
  KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
  KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),

/* Keymap Function Layer
   * ,----------------------------------------------------------------.
   * |Boot| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|      |Pscr|
   * |----------------------------------------------------------------|
   * |     |   |   |   |   |   |   |   |   |   |   |   |   |     |Mute|
   * |----------------------------------------------------------------|
   * |       |   |   |   |   |   |   |   |   |   |   |   |       |VlUp|
   * |----------------------------------------------------------------|
   * |        |   |   |   |   |   |   |   |   |   |   |      |Stp|VlDn|
   * |----------------------------------------------------------------|
   * |    |    |    |     Mouse Jiggler    |    |   |    |Prv|Ply|Nxt |
   * `----------------------------------------------------------------'
   */

[_FUNC] = LAYOUT_65_ansi(
  QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS, KC_PSCR,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_VOLU,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_MSTP, KC_VOLD,
  KC_TRNS, KC_TRNS, KC_TRNS,                   M_JIGL,                             KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT),

};

// Send KC_DEL when shift is pressed while pressing KC_BSPC
const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    NULL // Null terminate the array of overrides!
};

// Variables used by mouse jiggler macro
bool mouse_jiggle_active = false;
bool mouse_jiggle_direction = false; // Used to alternate direction
uint16_t mouse_jiggle_frequency = 15000; // How often to move the mouse (15 seconds)
uint16_t mouse_jiggle_timer = 0;

// Process user input
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_JIGL:
            if (record->event.pressed) {
                mouse_jiggle_active = !mouse_jiggle_active;
                rgblight_toggle_noeeprom();
            }
            break;
        default:
            if (record->event.pressed) {
                if (mouse_jiggle_active) {
                    mouse_jiggle_active = !mouse_jiggle_active;
                    rgblight_toggle_noeeprom();
                }
            }
    }
    return true;
}

// Execute code when matric is scanned
void matrix_scan_user(void) {
    if (mouse_jiggle_active) {
        if (timer_elapsed(mouse_jiggle_timer) > mouse_jiggle_frequency) {
            mouse_jiggle_timer = timer_read();
            if (mouse_jiggle_direction) {
                tap_code(KC_MS_LEFT);
            } else {
                tap_code(KC_MS_RIGHT);
            }
            mouse_jiggle_direction = !mouse_jiggle_direction;
        }
    }
}
