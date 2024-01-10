// -*- qmk: t -*-
// Copyright 2022 Matthew Dews (@matthew-dews)
// SPDX-License-Identifier: GPL-2.0-or-later
/* #define LAYER_STATE_32BIT */

#include "action_layer.h"
#include QMK_KEYBOARD_H
#include "keydef.h"
#include "win.h"
#include "../../6x7.h" // чтобы clangd не ругался

enum custom_keycodes {
    TEST_ = SAFE_RANGE,
    _PHONY,
    // :sysmeta
    SYSMETA_,
    APPSW__,
    SWNEXT_,
    SWPREV_,
    // :oneshot triggers
    TSK__,
    MGR__,
    NUM__,
    REF__,
    BRA__,
    MGREXIT_,
    MGRESC_,
    MGRINPT_,
    SAVE_ST,
    // :actions
    // :symbols
    PARENS,
    PARENSRU,
    BRACES,
    BRACESRU,
    BRCKS,
    BRCKSRU,
    ABRCKS,
    ABRCKSRU,
    QUOTS,
    QUOTSRU,
    DQUOTS,
    DQUOTSRU,

};

enum custom_layer {
    __QWERTY,
    __TSK,
    __MGR_EXIT,
    __MGR,
    __NUM,
    __REF,
    __BRA
};

static const layer_state_t osl_mask = 0b1110010u;

static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 1000;
static uint16_t oneshot_timer;

static uint8_t oneshot_down = 0, oneshot_fired = 1;
static uint8_t mgr_off = 0;
static uint8_t mgr_exit_off = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [__QWERTY  ] = LAYOUT_6x7
  (              S(_BSL) , S(_BSL) , QUOTS   , S(_MIN) , _TAB    , S(_5)   , _SPC    ,      _SPC    , _______ , S(_2)   , _B      , _Y      , BRA__   , FINDNXT ,
	         _A      , _A      , _A      , _O      , _EQL    , _MIN    , _SPC    ,      _SPC    , _BSP    , _G      , _C      , _R      , _F      , _K      ,
	         _A      , _DOT    , _A      , _E      , _I      , _U      , _SPC    ,      _BSP    , _L      , _H      , _T      , _N      , _S      , _RT     ,
	         _SPC    , _J      , _Q      , _SLS    , _P      , _ESC    ,                          _D      , _M      , _W      , _V      , _X      , _Z      ,
                 KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   ,                                              KC_A    , KC_B    , KC_C    , KC_D    , REF__
	         ,                             KC_A    , KC_B    , KC_C    ,                          _ENT    , TSK__   , _ENT
                 ,                             KC_D    , KC_E    , KC_F    ,                          _ESC    , _ESC    , NUM__
	      ),
  [__TSK     ] = LAYOUT_6x7 // l_tsk
  (              _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , RTAB    , LTAB    , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , MGR__   , KILLTAB , SAVE_ST, _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ ,                          _______ , C(_R)   , _______ , KILLAPP , FILEDIR , _______ ,
                 _______ , _______ , _______ , _______ , _______ ,                                              _______ , _______ , _______ , FILEPTH , _______
		 ,                             _______ , _______ , _______ ,                          _______ , _______ , _______
	         ,                             _______ , _______ , _______ ,                          _______ , _______ , _______
		     ),
  [__MGR_EXIT] = LAYOUT_6x7 // l_mgr_exit
  (              _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______ , MGREXIT_,
                 _______ , _______ , _______ , _______ , _______ , _______ ,                          _______ , _______ , _______ , _______ , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ ,                                              _______ , _______ , _______ , _______ , _______
                 ,                             _______ , _______ , _______ ,                          MGREXIT_, _______ , MGREXIT_
                 ,                             _______ , _______ , _______ ,                          _______ , _______ , _______
		             ),
  [__MGR     ] = LAYOUT_6x7 // l_mgr
  (              _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _______ , RTAB    , LTAB    , _______ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ , _______ ,      _______ , _______ , _DN     , MGREXIT_, _UP     , MGRINPT_ , _______ ,
                 _______ , _______ , _______ , _______ , _______ , _______ ,                          _______ , C(_R)   , _______ , KILLAPP , FILEDIR , _______ ,
                 _______ , _______ , _______ , _______ , _______ ,                                              _______ , _______ , _______ , FILEPTH , _______
                 ,                             _______ , _______ , _______ ,                          _______ , _______ , _______
                 ,                             _______ , _______ , _______ ,                          _______ , MGRESC_ , _______
		             ),
  [__NUM] = LAYOUT_6x7( // l_ref
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _0      , _9      , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _6      , _1      , _2      , _3      , _4      , _5      ,
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _7      , _8      , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______
			     ),
  [__REF] = LAYOUT_6x7( // l_ref
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , FWDSRCH , PALETTE , REPLACE , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______
			     ),
  [__BRA] = LAYOUT_6x7(
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , OTREE   , ODIRED  , OFILE   , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , C(_L)   , C(S(_P)), _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , _______ , _______ , _______ ,
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                            _______ , _______ , _______ ,    _______ , _______ , _______
		     ),
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

    if (mgr_off) {
      layer_and(~(1U << __MGR));
      mgr_off = 0;
    }
    if (mgr_exit_off) {
      layer_and(~(1U << __MGR_EXIT));
      send_string(SS_UP(X_LALT));
      mgr_exit_off = 0;
    }
}


bool process_record_user(uint16_t
			 keycode, keyrecord_t *record) {
    /* uint8_t layer = layer_switch_get_layer(record->event.key); // from which layer keycode flew in */



    /* if (layer == __MGR_EXIT) */
    /*   if (record->event.pressed) mgr_off = 1; */

  if (record->event.pressed) if (osl_mask & layer_state) oneshot_fired = 1;
  switch (keycode) {
  case TSK__: return oneshot_process(record, __TSK, 0);
  case NUM__: return oneshot_process(record, __NUM, 0);
  case REF__: return oneshot_process(record, __REF, 0);
  case BRA__: return oneshot_process(record, __BRA, 0);
  }


    if (record->event.pressed) {
      switch (keycode) {
      case MGR__:
	send_string(SS_DOWN(X_LCTL) SS_DOWN(X_LALT) SS_TAP(X_F13) SS_UP(X_LCTL));
	/* layer_or(3U << __MGR_EXIT); */
	layer_on(__MGR_EXIT);
	layer_on(__MGR);
	return 0;
      case MGREXIT_:
	mgr_off = 1;
	mgr_exit_off = 1;
	return 0;
      case MGRESC_:
	send_string(SS_TAP(X_ESC));
	mgr_off = 1;
	mgr_exit_off = 1;
	return 0;
      case MGRINPT_:
	mgr_off = 1;
	return 0;
      case SAVE_ST:
	send_string(SS_DOWN(X_LCTL) "s" SS_UP(X_LCTL));
	oneshot_fired = 0;
	return 0;
      }   
    }

    if (record->event.pressed) {
      switch (keycode) {
	// :actions

	// :symbols
      case PARENS:
	SEND_STRING("()" SS_TAP(X_LEFT));
	return false;
      case BRACES:
	SEND_STRING("{}" SS_TAP(X_LEFT));
	/* layer_on(BRACES_LR); */
	return false;
      case BRCKS:
	SEND_STRING("[]" SS_TAP(X_LEFT));
	return false;
      case BRCKSRU:
	SEND_STRING("~`" SS_TAP(X_LEFT));
	return false;
      case ABRCKS:
	SEND_STRING("<>" SS_TAP(X_LEFT));
	return false;
      case QUOTS:
	SEND_STRING("''" SS_TAP(X_LEFT));
	return false;
      case DQUOTS:
	SEND_STRING("\"\"" SS_TAP(X_LEFT));
	return false;
      }
    }
    return 1;
}
