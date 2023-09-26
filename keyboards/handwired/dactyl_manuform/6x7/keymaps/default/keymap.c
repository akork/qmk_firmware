// -*- qmk: t -*-
// Copyright 2022 Matthew Dews (@matthew-dews)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "action_layer.h"
#include QMK_KEYBOARD_H
#include "keydef.h"
#include "win.h"
#include "../../6x7.h" // чтобы clangd не ругался

enum custom_keycodes {
    TEST_ = SAFE_RANGE,
    // :sysmeta
    SYSMETA_,
    APPSW__,
    SWNEXT_,
    SWPREV_,
    // :oneshot triggers
    REF__,
    BRA__,
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
    __OS,
    __APPSWITCH,
    __REF,
    __BRA
};

static const layer_state_t osl_mask = 0b11000u;

static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 1000;
static uint16_t ostimer;
static uint16_t oneshot_timer;

static uint8_t modtap_fired = 1;
static uint8_t oneshot_down = 0, oneshot_fired = 1;
static uint8_t sysmeta_up_signal = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        /* LAYOUT_all //%% plain:en */
        /* (S(_BSL),          S(_BSL), QUOTES,  S(_MIN), _TAB,    S(_5),   _______, S(_2),   _B,      _Y,      OSL_BRA, FINDNXT, KC_PSCR, KC_PSCR, S(_F10), */
        /*  BRACKS,           PARENS,  OSL_SYM, _O,      _EQL,    _MIN,             _BSP,    _G,      _C,      _R,      _F,      _K,      BRACES, _F3, */
        /*  OSL_NUM,  XDOT,   _A,      _E,      _I,      _U,               _L,      _H,      _T,      _N,      _S,      _RT,                    G(A(_ENT)), */
        /*  BSEL,     _A,     _J,      _Q,      _SLS,    _P,      _ESC,             _D,      _M,      _W,      _V,      _X,      _Z,      C(A(_Y)),KC_VOLU, */
        /*  OSL_EDI,                   _LGUI,   RALT,             _SPC,OSM(MOD_LSFT),SYSMETA_,           RCMD,   RALT,OSM(MOD_LCTL),REF__, OSL_HELP), */

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
	S(_BSL) , S(_BSL) , QUOTS   , S(_MIN) , _TAB    , S(_5)   , _SPC    ,    _SPC    , _______ , S(_2)   , _B      , _Y      , BRA__   , FINDNXT ,
	_A      , _A      , _A      , _O      , _EQL    , _MIN    , _SPC    ,    _SPC    , _BSP    , _G      , _C      , _R      , _F      , _K      ,
	_A      , _DOT    , _A      , _E      , _I      , _U      , _SPC    ,    _BSP    , _L      , _H      , _T      , _N      , _S      , _RT     ,
	_SPC    , _J      , _Q      , _SLS    , _P      , _ESC    ,                        _D      , _M      , _W      , _V      , _X      , _Z      ,
        KC_ESC  , KC_F1   , KC_F2   , KC_F3   , KC_F4   ,                                            KC_A    , KC_B    , KC_C    , KC_D    , REF__   ,
	                                        KC_A    , KC_B    , KC_C    ,    _DEL    , SYSMETA_, REF__   ,
                                                KC_D    , KC_E    , KC_F    ,    _ESC    , _BSP    , BRA__
			 ),

        /* (MOD_SWITCH,       _______, LSCR,    RSCR,    FSCR,    C(_R),   _______, _______, KILLTAB, KILLAPP, _______, _______, _______, _______, _______, */
        /*  WIN,              REDO,    BROWSER2,BROWSER, MTTASK2, KILLTAB,          PTAB,    A(_GRV), RTAB,    LTAB,    FINDPRV, _______, _______, _______, */
        /*  _______,          CUT,     TERM,    EMACS,   UNDO,    MTTASK3,          HYPR(_K),APPSW,   KILLTAB, SAVE,    WINTAB,  _______,          _______, */
        /*  _______, _______, G(_7),   G(_6),   G(_5),   C(_R),  _______,          _______,  C(_R),   KILLAPP,  C(_R),   CWD,     PATH,    _______, RGB_TOG, */
        /*  _______,                   _______, _______,          OFFMETA, _______, _______,          _______, _______,          _______, _______, _______), */

        /* LAYOUT_all //%% :appswitch */
        /* (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, */
        /*  _______,          _______, _______, SBROWSER,_______, _______,          _______, KILLAPP, _HOME,   APPDN,   APPUP,   _______, _______, _______, */
        /*  _______,          _______, STERM,   SEMACS,  _______, _______,          _______, CMDTAB,  SCMDTAB, APPKILL, KC_HOME, _______,          _______, */
        /*  _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, */
        /*  _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______), */


  
  [__OS] = LAYOUT_6x7( // l_os
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , RTAB    , LTAB    , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , APPSW__ , KILLTAB , C(_S)   , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , C(_R)   , _______ , KILLAPP , FILEDIR , _______ , 
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , FILEPTH , _______ , 
	                                        _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                        _______ , _______ , _______ ,    _______ , _______ , _______
		     ),
  [__APPSWITCH] = LAYOUT_6x7( // l_appswitch
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , _______ , _______ , _______ , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ , _______ ,    _______ , _______ , SWNEXT_ , SWPREV_ , _______ , SWHOME  , _______ , 
        _______ , _______ , _______ , _______ , _______ , _______ ,                        _______ , _______ , _______ , SWKILL  , _______ , _______ , 
        _______ , _______ , _______ , _______ , _______ ,                                            _______ , _______ , _______ , _______ , _______ , 
	                                        _______ , _______ , _______ ,    _______ , _______ , _______ ,
	                                        _______ , _______ , _______ ,    _______ , _______ , _______
			     ),
        /*   LAYOUT_all //%% oneshot:ref */
        /* (_______,          _______, _______, POPMARK, FSCR1,   _F11,    _______, _______, _______, _______, ALTSRCH, _______, _______, _______, _______, */
        /*  LMONITOR,RMONITOR,EVALF,   EVALF,   _______, _______,          _______, SPLITRT, NEXTW,   ZOOM,    PREVW,   DELCUR,  _______, _______, */
        /*  DELCELL,          DELCELL, EVAL,    CELLUP,  CELLDN,  RECENTER,         FINDAT,  FSEARCH, PALETTE, REPLACE, _______, _______,          _______, */
        /*  NEWABOVE,_______, _F6,     _F5,     _F2,     _F12,    S(_F12),          _______, _______, _F3,     _F4,     _______, _______, _______, _______, */
        /*  _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______), */

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
        /* LAYOUT_all //%% oneshot:bra */
        /* (_______,          S(_1),   CCS,     S(_LBR), BRACES,  S(_RBR), _______, _______, FINDAT,  QUOTES,  _COM,    OCLIP,   _______, _______, _______, */
        /*  _______,          S(_9),   S(_0),   ABRACKS, S(_COM), S(_DOT),          ORC,     OTREE,   OCWD,    ORECENT, OHOME,   ODIRED,  _______, _______, */
        /*  _______,          _LBR,    _RBR,    E_CC,    CCS,     HELPKEY,          PALETTE, MX,      OHOME,   _______, S(_4),   S(_3),            _______, */
        /*  _______, _______, DEREF,   E_CV,    E_CX,    ORC,     _F1,              LCTL(_R),CTA(_S), LCTL(_W),S(_8)   ,S(_SLS), E_CZ,    _______, _______, */
        /*  _______,                   _______, _______,          COM_SPC, ORC,     COM_ENT,          _______, _______,          _______, _______, _______), */

  
        /*     LAYOUT_all //%% oneshot:bra_ru */
        /* (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, S(_SLS), _______, _______, _______, _______, */
        /*  _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, */
        /*  _______,          _______, S(_GRV),BRACKS_RU,_______, _GRV,             _______, _______, _______, _______, _______, _______,          _______, */
        /*  _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, */
        /*  _______,                   _______, _______,        COM_SPC_RU,_______, _______,          _______, _______,          _______, _______, _______), */

    
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
    uint8_t layer = layer_switch_get_layer(record->event.key); // from which layer keycode flew in



    if (layer == __OS) {
        if (keycode == LTAB ||
            keycode == RTAB) {
            sysmeta_up_signal = 1;
        } else {
            sysmeta_up_signal = 0;
        }
    }

  switch (keycode) {
  case REF__: return oneshot_process(record, __REF, 0);
  case BRA__: return oneshot_process(record, __BRA, 0);
  }

    if (1U << __OS & layer_state) {
        if (record->event.pressed) {
            switch(keycode) {
            case APPSW__:
                if (record->event.pressed) {
                    if (get_mods() & MOD_BIT(KC_LSFT))
                        del_mods(MOD_BIT(KC_LSFT));
                    send_string(SS_DOWN(X_LALT) SS_TAP(X_TAB));
                    layer_or(1U << __APPSWITCH);
                }

                return false;
            case SWNEXT_:
                if (record->event.pressed) {
                    if (get_mods() & MOD_BIT(KC_LSFT))
                        del_mods(MOD_BIT(KC_LSFT));
                    /* del_mods(MOD_BIT(SWITCH)); // important! the next processing should enable alt from scratch */
                    send_string(SS_DOWN(X_LALT) SS_TAP(X_TAB));
                }

                return false;
            case SWPREV_:
                if (record->event.pressed) {
                    send_string(SS_DOWN(X_LSFT) SS_TAP(X_TAB) SS_UP(X_LSFT));
                }
                return false;
            } // END: ALT-TAB like keycodes
        }
    }
    modtap_fired = 1;


  
    switch (keycode) {

        case SYSMETA_:
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
                    if (sysmeta_up_signal) {
                        register_code(_F19);
                        unregister_code(_F19);
                    }
                }
            }
            layer_and(~(3U << __OS));
            /* layer_and(~(3U << MACOS_LR)); */
        }
        return 0;

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
