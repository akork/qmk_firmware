// -*- qmk: t -*-
// Copyright 2022 Matthew Dews (@matthew-dews)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "action_layer.h"
#include QMK_KEYBOARD_H
#include "keydef.h"
#include "../../6x7.h" // чтобы clangd не ругался

enum custom_keycodes {
    TEST = SAFE_RANGE,
    OSMETA,
    APPSW,
    ALTTAB,
    SALTTAB,
    OSL_REF
};

enum custom_layer {
    __QWERTY,
    __OS,
    __APPSWITCH,
    __REF
};

static const layer_state_t osl_mask = 0b1000u;

static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 250;
static uint16_t ostimer;
static uint16_t oneshot_timer;

static uint8_t modtap_fired = 1;
static uint8_t oneshot_down = 0, oneshot_fired = 1;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        /* LAYOUT_all //%% plain:en */
        /* (S(_BSL),          S(_BSL), QUOTES,  S(_MIN), _TAB,    S(_5),   _______, S(_2),   _B,      _Y,      OSL_BRA, FINDNXT, KC_PSCR, KC_PSCR, S(_F10), */
        /*  BRACKS,           PARENS,  OSL_SYM, _O,      _EQL,    _MIN,             _BSP,    _G,      _C,      _R,      _F,      _K,      BRACES, _F3, */
        /*  OSL_NUM,  XDOT,   _A,      _E,      _I,      _U,               _L,      _H,      _T,      _N,      _S,      _RT,                    G(A(_ENT)), */
        /*  BSEL,     _A,     _J,      _Q,      _SLS,    _P,      _ESC,             _D,      _M,      _W,      _V,      _X,      _Z,      C(A(_Y)),KC_VOLU, */
        /*  OSL_EDI,                   _LGUI,   RALT,             _SPC,OSM(MOD_LSFT),OSMETA,           RCMD,   RALT,OSM(MOD_LCTL),OSL_REF, OSL_HELP), */

    /* [__OS] = LAYOUT_6x7( */
    /*     _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,  */
    /*     _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,  */
    /*     _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,  */
    /*     _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , _______ , _______ , _______ ,  */
    /*     _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ ,  */
    /* 	                                        _______ , _______ , _______ ,    _______ , _______ , _______ , */
    /* 	                                        _______ , _______ , _______ ,    _______ , _______ , _______ */
    /* 		     ), */


  
  [__QWERTY] = LAYOUT_6x7(
        _______ , KC_GRV  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,    KC_A    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_A    , 
	_A      , _A      , _A      , _O      , _EQL    , _MIN    , _SPC    ,    _SPC    , _BSP    , _G      , _C      , _R      , _F      , _K      ,
	_A      , _DOT    , _A      , _E      , _I      , _U      , _SPC    ,    _BSP    , _L      , _H      , _T      , _N      , _S      , _RT     ,
        _______ , KC_LCTL , KC_Z    , KC_X    , KC_C    , KC_V    ,                        KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_D    ,
        KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   ,                                            KC_A    , KC_B    , KC_C    , KC_D    , KC_E    ,
	                                        KC_A    , KC_B    , KC_C    ,    KC_A    , OSMETA  , OSL_REF    ,
	                                        KC_D    , KC_E    , KC_F    ,    KC_R    , KC_T    , KC_Y
			 ),
  [__OS] = LAYOUT_6x7(
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , APPSW   , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ , 
	                                        _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                        _______ , _______ , _______ ,    _______ , _______ , _______
		     ),
  [__APPSWITCH] = LAYOUT_6x7(
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , ALTTAB  , SALTTAB , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ , 
	                                        _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                        _______ , _______ , _______ ,    _______ , _______ , _______
			     ),
    [__REF] = LAYOUT_6x7(
        _______ , KC_GRV  , KC_1    , KC_2    , KC_3    , KC_4    , KC_5    ,    KC_A    , KC_6    , KC_7    , KC_8    , KC_9    , KC_0    , KC_A    , 
	_A      , _A      , _A      , _O      , _EQL    , _MIN    , _SPC    ,    _SPC    , _BSP    , _G      , _C      , _R      , _F      , _K      ,
	_A      , _DOT    , _A      , _E      , _I      , _U      , _SPC    ,    _BSP    , _L      , _A      , _A      , _N      , _S      , _RT     ,
        _______ , KC_LCTL , KC_Z    , KC_X    , KC_C    , KC_V    ,                        KC_N    , KC_M    , KC_COMM , KC_DOT  , KC_SLSH , KC_D    ,
        KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   ,                                            KC_A    , KC_B    , KC_C    , KC_D    , KC_E    ,
	                                        KC_A    , KC_B    , KC_C    ,    KC_A    , OSMETA  , KC_E    ,
	                                        KC_D    , KC_E    , KC_F    ,    KC_R    , KC_T    , KC_Y
    )

}; 


bool oneshot_process(keyrecord_t *record, uint8_t layer_n, bool check_ru) {
    if (record->event.pressed) {
        oneshot_timer = timer_read();
        oneshot_down = 1;
        oneshot_fired = 0;
        layer_on(layer_n);
        if (check_ru) {
            /* if (1U << RU_LR & layer_state) */
                /* layer_on(layer_n + 1); */
        }
    } else {
        oneshot_down = 0;
    }
    return 0;
}


void matrix_scan_user(void) {
    if (osl_mask & layer_state)
        if (!oneshot_down)
            if (oneshot_fired || (timer_elapsed(oneshot_timer) > oneshot_threshold))
                layer_and(~osl_mask);
}


bool process_record_user(uint16_t
			 keycode, keyrecord_t *record) {
    /* uint8_t layer = layer_switch_get_layer(record->event.key); // from which layer keycode flew in */



  

  switch (keycode) {
  case OSL_REF: return oneshot_process(record, __REF, 0);
  }

    if (1U << __OS & layer_state) {
        if (record->event.pressed) {
            switch(keycode) {
            case APPSW:
                if (record->event.pressed) {
                    if (get_mods() & MOD_BIT(KC_LSFT))
                        del_mods(MOD_BIT(KC_LSFT));
                    send_string(SS_DOWN(X_LALT) SS_TAP(X_TAB));
                    layer_or(1U << __APPSWITCH);
                }

                return false;
            case ALTTAB:
                if (record->event.pressed) {
                    if (get_mods() & MOD_BIT(KC_LSFT))
                        del_mods(MOD_BIT(KC_LSFT));
                    /* del_mods(MOD_BIT(SWITCH)); // important! the next processing should enable alt from scratch */
                    send_string(SS_DOWN(X_LALT) SS_TAP(X_TAB));
                }

                return false;
            case SALTTAB:
                if (record->event.pressed) {
                    send_string(SS_DOWN(X_LSFT) SS_TAP(X_TAB) SS_UP(X_LSFT));
                }
                return false;
            } // END: ALT-TAB like keycodes
        }
    }
    modtap_fired = 1;


  
    switch (keycode) {

        case OSMETA:
        if (record->event.pressed) {
            ostimer = timer_read();
            layer_on(__OS);
            modtap_fired = 0;
        } else {
            if ((timer_elapsed(ostimer) < timer_threshold) && !modtap_fired) {
                register_code(KC_ENTER);
                unregister_code(KC_ENTER);
                unregister_code(KC_LGUI);
                unregister_code(KC_RGUI);
                unregister_code(KC_LALT);
                unregister_code(KC_RALT);
                unregister_code(KC_LSFT);
                unregister_code(KC_RSFT);
            } else {
                if ((1U << __OS) & layer_state) {
                    unregister_code(KC_LGUI);
                    unregister_code(KC_RGUI);
                    unregister_code(KC_LALT);
                    unregister_code(KC_RALT);
                    unregister_code(KC_LSFT);
                    unregister_code(KC_RSFT);
                    /* if (meta_up_signal) { */
                    /*     register_code(MAGIC); */
                    /*     unregister_code(MAGIC); */
                    /* } */
                }
            }
            layer_and(~(3U << __OS));
            /* layer_and(~(3U << MACOS_LR)); */
        }
        return 0;

    }

    return 1;
}
