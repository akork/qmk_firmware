// -*- qmk: t -*-
// Copyright 2022 Matthew Dews (@matthew-dews)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "action_layer.h"
#include QMK_KEYBOARD_H
#include "keydef.h"
#include "../../6x7.h" // чтобы clangd не ругался

enum custom_layer {
    _QWERTY,
    _LOWER,
    _RAISE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return false;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_6x7(
        _______ , KC_GRV  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,    _______ , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_LBRC ,
        _______ , KC_TAB  , KC_Q    , KC_W    , KC_E    , KC_R    , KC_T    ,    _______ , KC_Y    , KC_U    , KC_I    , KC_O    , KC_P    , KC_QUOT ,
        _______ , KC_LSFT , KC_A    , KC_S    , KC_D    , KC_F    , KC_G    ,    _______ , KC_H    , KC_J    , KC_K    , KC_L    , KC_SCLN , KC_LSFT ,
        _______ , KC_LCTL , KC_Z    , KC_X    , KC_C    , KC_V    ,                        KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_LCTL ,
        KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   ,                                            KC_F8   , KC_F9   , KC_F10  , KC_F11  , KC_F12  ,
	                                        KC_A    , KC_B    , KC_C    ,    KC_Q    , KC_W    , KC_E    ,
	                                        KC_D    , KC_E    , KC_F    ,    KC_R    , KC_T    , KC_Y
    )
};
