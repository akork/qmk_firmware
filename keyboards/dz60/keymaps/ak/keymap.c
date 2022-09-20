// -*- compile-command: "cd ../../../../ && make dz60:ak-first:dfu" -*-

#include QMK_KEYBOARD_H

#include "keydef.h"
#include "win.h"

#define MODS_CTRL_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))

enum custom_keycodes {
    TEST = SAFE_RANGE,
    METASWITCH,
    OSL_NUM,
    OSL_EDI,
    OSL_SYM,
    OSL_BRA,
    OSL_REF,
    NL,
    LBRACES,
    SCOLON,
    OFFMETA,
    XCDCLIP,
    CAPS,
    SQUO,
    SPARENS,
    SBRACKS,
    BSEL,
    FSEL,
    ABORTSEL,
    PHONY,
    NEXT,
    NCOMMA,
    NNCOMMA,
    NNCOLON,
    NNDOT,
    NNSPACE,
    NNENTER,
    SELLINE,
    MRKLINE,
    NCOLON,
    C_ENT,
    COUT,
    CIN,
    NAMESPACE,
    PLUSPLUS,
    MOD_SWITCH,
    JOIN_L,
    DUP,
    L_SEL,
    L_DEL,
    W_SEL,
    PGDN,
    EQL_EQL,
    EQL_SPC,
    RT2_SPC,
    SEARCH,
    PYBLOCK,
    CBLOCK,
    BSP,
    YANK,
    KILL,
    LGUI,
    MAC,
    WIN,
    OSMETA,
    RCMD,
    LCMD,
    LALT,
    RALT,
    LCTL,
    RSFT,
    _VIMRC,
    CC_PLS,
    CC_MIN,
    CX_CC,
    LSWITCH,
    CMDTAB,
    SCMDTAB,
    ALTQUO,
    LBR_RBR_LFT,
    GQ,
    CX_CX,
    CX_G,
    CX_Z,
    CX_F,
    CX_1,
    CX_3,
    M0_CK,
    CXCJ_0,
    CXCJ_B,
    CXCJ_CC,
    CXCJ_D,
    CXCJ_CD,
    CXCJ_SD,
    CXCJ_MD,
    CXCJ_T,
    CXCJ_CT,
    CXCJ_MT,
    CX_P,
    CX_CP,
    CX_LBRC,
    CX_RBRC,
    COM_MIN,
    COM_SPC,
    COM_SPC_RU,
    COM_ENT,
    EENTER,
    EEENTER,
    PARENS,
    PARENS_ENDL,
    BRACES,
    BL_BRACES,
    BRACKS,
    BRACKS_RU,
    ABRACKS,
    QUOTES,
    DQUOTES,
    CCS, // complete current statement
    BEGS,
    NLA,
    EEOL,
    SCLSPC,  // semicolon space
    ENDSPC,
    ENDL,
    H_A,
    H_B,
    H_C,
    H_D,
    SPRNT,
    FPRNT,
    SWITCHPY,
    DDD,
    ALTF4,
    APPKILL,
    SEMACS,
    STERM,
    SBROWSER,
    SELDN,
    SELUP,
};

// -layers enum
enum {
    EN_LR,          // 0 0
    RU_LR,          // 1 0
    BRACES_LR,      // 2 0
    BRACES2_LR,      // 2 0
    EDI_LR,         // 5 1
    EDI2_LR,        // 5 0
    MACOS_LR,       // 2 0
    APPSWITCH_LR,   // 3 0
    NUM_LR,         // 4 1
    SEL_LR,         // 6 0
    SEL2_LR,        // 7 0
    SEL3_LR,        // 8 0
    SEL4_LR,        // 9 0
    SYM_LR,         //10 1
    RU_SYM_LR,      //11 1
    EEOL_LR,        //   1
    BRA_LR,         //12 1
    RU_BRA_LR,      //13 1
    REF_LR,         //14 1
};

static const uint32_t osl_mask = 0b1111110000100010000;
static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 1000;


static uint16_t ostimer, rcmd_timer;
static uint8_t rgblight_mode_current = RGBLIGHT_MODE_KNIGHT + 1;

static uint16_t oneshot_timer;
static uint8_t oneshot_down = 0, oneshot_fired = 0;
static uint8_t caps = 0,
    sel_off = 0,
    sel2_off = 0,
    braces_lr_off = 0,
    mod = 0,
    meta_up_signal = 0,
    py = 0;


void keyboard_post_init_user(void) {
    // Call the post init code.

    /* rgblight_disable(); */
    rgblight_disable();
    rgblight_mode_noeeprom(rgblight_mode_current + 7); // sets mode to Fast breathing without saving

    /* rgblight_enable_noeeprom(); // enables Rgb, without saving settings */
    /* rgblight_sethsv_noeeprom(250, 200, 100); // sets the color to teal/cyan without saving */
    /* rgblight_mode_noeeprom(rgblight_mode_current); // sets mode to Fast breathing without saving */
    /* rgblight_disable_noeeprom(); */
}

uint32_t layer_state_set_user(uint32_t state) {
    if (1UL << RU_LR & state) {
        rgblight_enable_noeeprom();
    } else {
        rgblight_disable_noeeprom();
    }
    return state;
}

void unregister_cmd_after_cmdtab(void) {
    uint8_t layer = biton32(layer_state);
    if (layer == MACOS_LR) {
        unregister_mods(MOD_LGUI);
        unregister_mods(MOD_LALT);
    }
};

void ru_turn_off(void) {
    if (1UL << RU_LR & layer_state) {
        send_string(SS_LGUI(" "));
        layer_off(RU_LR);
        layer_off(RU_BRA_LR);
        layer_off(RU_SYM_LR);
    }
}

void matrix_scan_user(void) {
    if (osl_mask & layer_state)
        if (!oneshot_down)
            if (oneshot_fired || (timer_elapsed(oneshot_timer) > oneshot_threshold))
                layer_and(~osl_mask);

    if (sel_off) {
        layer_and(~(15UL << SEL_LR));
        sel_off = 0;
    }
    if (sel2_off) {
        layer_off(SEL3_LR);
        layer_off(SEL4_LR);
        sel2_off = 0;
    }
    if (braces_lr_off) {
        layer_and(~(3UL << BRACES_LR));
        braces_lr_off = 0;
    }
}

/* void check_keys(uint16_t ...) { */

/* } */

bool oneshot_process(keyrecord_t *record, uint8_t layer_n, bool check_ru) {
    if (record->event.pressed) {
        oneshot_timer = timer_read();
        oneshot_down = 1;
        oneshot_fired = 0;
        layer_on(layer_n);
        if (check_ru) {
            if (1UL << RU_LR & layer_state)
                layer_on(layer_n + 1);
        }
    } else {
        oneshot_down = 0;
    }
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t layer = layer_switch_get_layer(record->event.key); // from which layer keycode flew in
    // turn off ru_lr upon specific keys
    if (record->event.pressed) {
        if (keycode == _ENT ||
            keycode == G(_ENT) ||
            keycode == G(A(_ENT)) ||
            keycode == OSMETA ||
            keycode == BSEL ||
            keycode == OSL_REF) {
            if (1UL << RU_LR & layer_state)
                ru_turn_off();
        }
    }

    // select layers magic
    if (record->event.pressed) {
        if (((layer == SEL_LR) || (layer == SEL3_LR)) && keycode != S(_RT)) sel2_off = 1;
        if ((layer == SEL4_LR) || (layer == SEL2_LR)) sel_off = 1;
    }

    // for ctrl-tab in emacs
    if (layer == MACOS_LR) {
        if (keycode == LTAB ||
            keycode == RTAB) {
            meta_up_signal = 1;
        } else {
            meta_up_signal = 0;
        }
    }

    if (record->event.pressed) if ((3UL << BRACES_LR) & layer_state) braces_lr_off = 1;
    // oneshot processing (setting oneshot_fired flag)
    if (record->event.pressed) if (osl_mask & layer_state) oneshot_fired = 1;

    // oneshot keys down/up events processing
    switch (keycode) {
    case OSL_REF: return oneshot_process(record, REF_LR, 0);
    case OSL_EDI: return oneshot_process(record, EDI_LR, 0);
    case OSL_SYM: return oneshot_process(record, SYM_LR, 1);
    case OSL_BRA: return oneshot_process(record, BRA_LR, 1);
    case OSL_NUM: return oneshot_process(record, NUM_LR, 0);
    }

    // CAPS CAPS CAPS CAPS CAPS
    if (record->event.pressed) {
        if (caps == 1) { // стоп слова без дальнейшей обработки
            if (keycode == CAPS ||
                keycode == _ESC ||
                keycode == OSM(MOD_LSFT) ||
                keycode == _TAB
                ) {
                caps = 0;
                unregister_code(KC_LSHIFT);
                return 0;
            }
            if (keycode == OSMETA || // стоп слова с дальнейшей обработкой
                keycode == BSEL ||
                keycode == _UP ||
                keycode == _DN) {
                caps = 0;
                unregister_code(KC_LSHIFT);
            }
            // если активно что-то выше 3 слоя
            if (0b11111111111111111111111111111000 & layer_state) {
                unregister_code(KC_LSHIFT);
            }
        }
    }

    // :layer_triggers
    switch(keycode) {
    case PHONY:
        return 0;
    case METASWITCH:
        if (record->event.pressed) {
            /* register_code(KC_LALT); */
            /* register_code(KC_TAB); */
            /* unregister_code(KC_TAB); */
            /* unregister_code(KC_LALT); */
            send_string_with_delay(SS_DOWN(X_LALT) SS_TAP(X_TAB) SS_UP(X_LALT), 50);
            return 0;
        }
        return 0;
    case EENTER:
        if (record->event.pressed) {
            send_string(SS_TAP(X_ENTER));
            layer_or((1UL << BRACES2_LR));
            braces_lr_off = 0;
        }
        return 0;
    case EEENTER:
        if (record->event.pressed) {
            send_string(SS_TAP(X_UP) XEOL SS_TAP(X_ENTER));
        }
        return 0;
    case CAPS:
        if (record->event.pressed) {
            caps = 1; // layer emulates with flag
            register_code(KC_LSHIFT);
        }
        return 0;
    case LSWITCH:
        if (record->event.pressed) {
            if (1UL << RU_LR & layer_state) {
                ru_turn_off();
            } else {
                send_string(SS_LGUI(" "));
                layer_on(RU_LR);
            }
            return 0;
        }
    case BSEL:
        if (record->event.pressed) {
            layer_or(15UL << SEL_LR);
            send_string(SS_DOWN(X_LSHIFT) SS_TAP(X_RIGHT) SS_UP(X_LSHIFT));
        }
        return 0;
    case FSEL:
        if (record->event.pressed) {
            layer_on(SEL_LR);
            layer_on(SEL2_LR);
            send_string(SS_DOWN(X_LSHIFT) SS_TAP(X_RIGHT) SS_TAP(X_LEFT) XEOW SS_UP(X_LSHIFT));
        }
        return 0;
    case ABORTSEL:
        if (record->event.pressed) {
            send_string(SS_TAP(X_RIGHT));
        }
        return 0;
    case TEST:
        /* send_string(SS_DOWN(X_LSHIFT) SS_DOWN(X_LGUI) SS_TAP(X_LEFT) SS_UP(X_LGUI) SS_UP(X_LSHIFT)); */
        rgblight_mode_noeeprom(rgblight_mode_current++); // sets mode to Fast breathing without saving
        return false;

    case OSMETA:
        if (record->event.pressed) {
            ostimer = timer_read();
            layer_on(MACOS_LR);
        } else {
            layer_and(~(3UL << MACOS_LR));
            if (timer_elapsed(ostimer) < timer_threshold) {
                register_code(KC_ENTER);
                unregister_code(KC_ENTER);
                unregister_code(KC_LGUI);
                unregister_code(KC_LALT);


                /* layer_on(ENTER_LR); */
                /* enter_timer = timer_read(); */
            } else {
                unregister_code(KC_LGUI);
                unregister_code(KC_LALT);
                if (meta_up_signal) {
                    register_code(MAGIC);
                    unregister_code(MAGIC);
                }

            }
            /* clear_mods(); */
        }
        return 0;
    case OFFMETA:
        meta_up_signal = 0;
        layer_and(~(1UL << MACOS_LR));
        return 0;
    }

    // :cmd_tab
    if (1UL << MACOS_LR & layer_state) {
        if (record->event.pressed) {
            switch(keycode) {
            case G(A(_C)):
                register_code(KC_RGUI);
                register_code(KC_LALT);
                register_code(KC_C);
                unregister_code(KC_C);
                unregister_code(KC_LALT);
                unregister_code(KC_RGUI);
                return 0;
            case G(A(S(_C))):
                register_code(KC_RGUI);
                register_code(KC_LALT);
                register_code(KC_LSFT);
                register_code(KC_C);
                unregister_code(KC_C);
                unregister_code(KC_LSFT);
                unregister_code(KC_LALT);
                unregister_code(KC_RGUI);
                return 0;
            case CMDTAB:
                if (record->event.pressed) {
                    if (get_mods() & MOD_BIT(KC_LSHIFT)) {
                        del_mods(MOD_BIT(KC_LSHIFT));
                    }
                    /* del_mods(MOD_BIT(SWITCH)); // important! the next processing should enable alt from scratch */
                    register_code(SWITCH);
                    register_code(KC_TAB);
                    unregister_code(KC_TAB);
                    layer_or(1UL << APPSWITCH_LR);
                }

                return false;
            case SCMDTAB:
                if (record->event.pressed) {
                    register_code(SWITCH);
                    register_code(KC_LSHIFT);
                    register_code(KC_TAB);
                    unregister_code(KC_TAB);
                    unregister_code(KC_LSHIFT);
                }
                return false;
            case GQ:
                register_code(KC_LGUI);
                register_code(KC_Q);
                unregister_code(KC_Q);
                return false;
                /* default: */
                /* if (layer <= APPSWITCH_LR) { */
                /* unregister_mods(MOD_LGUI); */
                /* unregister_mods(MOD_LALT); // important! the next processing should enable alt from scratch */
                /* } */

            } // END: CMD-TAB like keycodes
        }
    }

    // RCMD & RALT doing smth usefull on single press:
    switch(keycode) {
    case RCMD:
        if (record->event.pressed) {
            rcmd_timer = timer_read();
            register_code(KC_RCTRL);
        } else {
            unregister_code(KC_RCTRL);
            if (timer_elapsed(rcmd_timer) < timer_threshold)
                send_string(SS_LCTRL("x") "k" SS_LCTRL("["));
        }
        return false;
    case RALT:
        if (record->event.pressed) {
            rcmd_timer = timer_read();
            register_code(KC_RALT);
        } else {
            unregister_code(KC_RALT);
            if (timer_elapsed(rcmd_timer) < timer_threshold)
                send_string(SS_LCTRL("x") "f" SS_LCTRL("["));
        }
        return false;
    }


    /* :main */
    if (record->event.pressed) {

        if (layer == SEL4_LR) { // some stuff for exiting selection mode in emacs for NN-commands
            send_string(SS_TAP(X_F19) SS_TAP(X_RIGHT));
        }
        clear_weak_mods();
        send_keyboard_report();
        switch(keycode) {
            /* case TEXVIEW:         */
            /* return 0 { */
            /* }; */
        case APPKILL: // for appswitcher (doesn't lift alt)
            send_string(SS_LCTL("w"));
            /* register_code(KC_LALT); */
            /* register_code(KC_LCTL); */
            /* register_code(_W); */
            /* unregister_code(_W); */
            /* unregister_code(KC_LCTL); */
            return 0;
        case SEMACS:
            unregister_code(KC_LALT);
            send_string(XEMACS);
            return 0;
        case STERM:
            unregister_code(KC_LALT);
            send_string(XTERM);
            return 0;
        case SBROWSER:
            unregister_code(KC_LALT);
            send_string(XBROWSER);
            return 0;
        case SCOLON:
            send_string(";" SS_TAP(X_LEFT));
            return 0;
        case NL:
            send_string(XEOL SS_TAP(X_ENTER));
            return 0;
        case NLA:
            send_string(XBOL SS_TAP(X_ENTER) SS_TAP(X_UP));
            return 0;
            /* case CELLDN: */
            /* send_string(SS_TAP(X_ESC) SS_TAP(X_DOWN) "e"); */
            /* return 0; */
            /* case CELLUP: */
            /* send_string(SS_TAP(X_ESC) SS_TAP(X_UP) "e"); */
            /* return 0; */
        case SWITCHPY:
            py = (py + 1) % 2;
            return 0;
        case CCS:
            send_string(XEOL ";");
            return 0;
        case BEGS:
            if (py == true) {
                send_string(XEOL ":" SS_TAP(X_ENTER));
            } else {
                send_string(XEOL " {}" SS_TAP(X_LEFT) SS_TAP(X_ENTER));
            }
            return 0;
        case PYBLOCK:
            send_string(SS_TAP(X_LEFT) XEOL ":" SS_TAP(X_ENTER));
            return 0;
        case SPRNT:
            send_string(XBOL "print(" XEOL ")");
            return 0;
        case FPRNT:
            send_string(XBOL "print(f'{" XEOL "=}')");
            return 0;
        case NCOMMA:
            send_string(", ");
            return 0;
        case NNCOMMA:
            send_string(",");
            return 0;
        case NNCOLON:
            send_string(":");
            return 0;
        case NNDOT:
            send_string(".");
            return 0;
        case NNSPACE:
            send_string(" ");
            return 0;
        case NNENTER:
            send_string(SS_TAP(X_ENTER));
            return 0;
        case NCOLON:
            send_string(": ");
            return 0;
        case SELLINE:
            send_string(SS_DOWN(X_LSHIFT) SS_TAP(X_LEFT) SS_TAP(X_DOWN) SS_UP(X_LSHIFT));
            return 0;
        case MRKLINE:
            send_string(SS_DOWN(X_LSHIFT) SS_TAP(X_LEFT) SS_TAP(X_UP) SS_UP(X_LSHIFT));
            return 0;
        case SELDN:
            send_string(SS_UP(X_LSHIFT) XBOL SS_DOWN(X_LSHIFT) SS_TAP(X_RIGHT) XEOL
                        SS_TAP(X_DOWN) XBOL SS_TAP(X_DOWN) SS_TAP(X_DOWN) SS_UP(X_LSHIFT));
            layer_or(3UL << SEL_LR);
            return 0;
        case SELUP:
            send_string(SS_UP(X_LSHIFT) XEOL SS_DOWN(X_LSHIFT) SS_TAP(X_UP) XBOL SS_TAP(X_UP) SS_UP(X_LSHIFT));
            layer_or(3UL << SEL_LR);
            return 0;
        case C_ENT:
            send_string(SS_LCTRL(SS_TAP(X_ENTER)) SS_TAP(X_ENTER));
            return 0;
        case SQUO:
            send_string(SS_LCTRL("x") "''" SS_TAP(X_LEFT) SS_LCTRL("v"));
            return 0;
        case SPARENS:
            send_string(SS_LCTRL("x") "()" SS_TAP(X_LEFT) SS_LCTRL("v"));
            return 0;
        case SBRACKS:
            send_string(SS_LCTRL("x") "[]" SS_TAP(X_LEFT) SS_LCTRL("v"));
            return 0;
        case MOD_SWITCH:
            mod ^= 1;
            return 0;
        case EQL_EQL:
            send_string("== ");
            return 0;
        case EQL_SPC:
            send_string("= ");
            return 0;
        case BRACES:
            SEND_STRING("{}" SS_TAP(X_LEFT));
            layer_on(BRACES_LR);
            return false;
        case PARENS:
            SEND_STRING("()" SS_TAP(X_LEFT));
            return false;
        case PARENS_ENDL:
            SEND_STRING("(" SS_DOWN(X_LGUI) SS_TAP(X_RIGHT) SS_UP(X_LGUI) ")");
            return false;
        case BRACKS:
            SEND_STRING("[]" SS_TAP(X_LEFT));
            return false;
        case ABRACKS:
            SEND_STRING("<>" SS_TAP(X_LEFT));
            return false;
        case BRACKS_RU:
            SEND_STRING("~`" SS_TAP(X_LEFT));
            return false;
        case QUOTES:
            SEND_STRING("''" SS_TAP(X_LEFT));
            return false;
        case DQUOTES:
            SEND_STRING("\"\"" SS_TAP(X_LEFT));
            return false;
        case DDD:
            send_string("...");
            return 0;
        case ALTQUO:
            SEND_STRING(SS_LALT("`"));
            return false;
        case CC_PLS:
            SEND_STRING(SS_LCTRL("c") "+");
            return false;
        case CC_MIN:
            SEND_STRING(SS_LCTRL("c") "-");
            return false;
        case COM_SPC:
            SEND_STRING(", ");
            return false;
        case COM_SPC_RU:
            send_string("? ");
            return 0;
        case COM_ENT:
            send_string("," SS_TAP(X_ENTER));
            return 0;
        case EEOL:
            send_string(XEOL);
            oneshot_fired = 0;
            oneshot_timer = timer_read();
            layer_and(~(3UL << SYM_LR));
            layer_or((1UL << EEOL_LR));
            return 0;
        }
    }


    if (caps == 1) {
        if (0b11111111111111111111111111111000 & layer_state) {
            /* unregister_code(KC_LSHIFT); */
        } else {
            register_code(KC_LSHIFT);
        }
    }

    return true;}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
    {

        //%% karabiner:
        /* (non_us_backslash,1,       2,       3,       4,       5,       6,       7,       8,       9,       0,       hyphen, equal_sign,,       delete_or_backspace, */
        /* tab,              q,       w,       e,       r,       t,                y,       u,       i,       o,       p,       open_bracket,close_bracket,return_or_enter, */
        /* caps_lock,        a,       s,       d,       f,       g,                h,       j,       k,       l,       semicolon,quote,           backslash, */
        /* left_shift,grave_accent_and_tilde,z,x,c,     v,       b,                n,       m,       comma,   period,  slash,   right_shift,,     , */
        /* ,                          ,        ,                 ,       ,         ,        ,                 ,                 ,        ,        ) */

        LAYOUT_all //%% plain:en
        (S(_BSL),          S(_BSL), QUOTES,  S(_MIN), _TAB,    S(_5),   _______, S(_2),   _B,      _Y,      OSL_BRA, FINDNXT, FINDPRV, _NO,     S(_F10),
         BRACKS,           BSEL,    OSL_SYM, _O,      _EQL,    _MIN,             _BSP,    _G,      _C,      _R,      _F,      _K,      BRACES, _F3,
         OSM(MOD_LSFT),    _DOT,    _A,      _E,      _I,      _U,               _L,      _H,      _T,      _N,      _S,      PARENS,  G(A(_ENT)),
         _LSFT,    _A,     _J,      _Q,      _SLS,    _P,      _ESC,             _D,      _M,      _W,      _V,      _X,      _Z,      C(A(_Y)),KC_VOLU,
         OSL_EDI,                   _LGUI,   RALT,             _SPC,    OSL_NUM, OSMETA,           RCMD,    METASWITCH,       OSL_REF, OSL_REF, KC_VOLD),

        LAYOUT_all //%% plain:ru
        (_______,          _______, _SCL,    _______, _______, _______, _______, _______, _COM,    _E,      _______, _______, _______, _______, _______,
         _W,               _______, _______, _J,      _Q,      _______,          _______, _U,      _Z,      _H,      _A,      _P,      _RBR,    _O,
         _______,          _SLS,    _F,      _T,      _B,      _M,               _K,      _R,      _N,      _Y,      _C,      _Q,               _______,
         _DOT, _______,    _QUO,    _I,      _S,      _G,      _______,          _L,      _V,      _D,      _X,      _LBR,    _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% mod:braces
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, EENTER,           _______, _______,          _______, _______, _______),

        LAYOUT_all //%% mod:braces2
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, EEENTER,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:edi
        (_______,          _______, _______, UNDO,    _______, _BOF,    _EOF,    G(_LBR), EOW_R,   _DEL,    HARDBOL, SWAPUP,  SWAPDN,  HELPKEY, RESET,
         ALL,              CUT,     PASTE,   DELBOW,  DELEOW,  DELEOSW,          _______, EOSW_L,  BOW_R,   BOW_L,   EOSW_R,  _BOL,    _______, C(_F4),
         G(S(_D)),         COPY,    PASTE,   NL,      _EOL,    COMMENT,          _LT,     _DN,     _UP,     _RT,     SELDN,   SELUP,            _______,
         G(_X),   _______, CC_PLS,  DELETE,  CUT,     DUPL,    _______,          _DEL,    _PGDN,   _PGUP,   _DEL,    G(_DN),  _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% mod:braces2
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, EEENTER,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% mod:os
        (MOD_SWITCH,       _______, LSCR,    RSCR,    FSCR,    _______, _______, _______, KILLTAB, KILLAPP, G(A(_W)),G(_W),   G(_Z),   _______, _______,
         WIN,              REDO,    BROWSER2,BROWSER, MTTASK2, KILLTAB,          PTAB,    FINDNXT, RTAB,    LTAB,    FINDPRV, G(_RBR), _______, _______,
         _______,          CUT,     TERM,    EMACS,   UNDO,    MTTASK3,          HYPR(_K),CMDTAB,  SCMDTAB,
         SAVE,    CWD,     _______,          _______,
         _______, _______, SFTLT,   SFTRT,   SFTRT,   MTTASK,  _______,          _______, UPD,     _______, _______, G(A(_C)),_______, _______, RGB_TOG,
         _______,                   _______, _______,          OFFMETA, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% appswitch
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, SBROWSER,_______, _______,          _______, KILLAPP, ALTF4,   APPDN,   APPUP,   _______, _______, _______,
         _______,          _______, STERM,   SEMACS,  _______, _______,          _______, _______, _______, APPDN,   APPKILL, APKILL,           _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:num
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, COUT,    CIN,              NAMESPACE,COUT,   _0,      _9,      CIN, _______, _______, _______,
         _CAP,             _______, _______, _______, _ESC,    _______,          _6,      _1,      _2,      _3,      _4,      _5,               _______,
         _______, _______, _______, _______, REDEBUG, CLDEBUG, _______,          _7,      _8,      _______, CC_PLS,  CC_MIN,  _______, _______, _______,
         _______,                   _______, _______,          SCLSPC, _______, CCS,               _______, _______,          _______, _______, _______),


        LAYOUT_all //%% sticky:sel
        (_______,          _______, _______, _______, S(_BOF), S(_EOF), _______, _______, S(EOW_R),SWAPDN,  S(HARDBOL), SWAPUP,_______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, S(EOSW_L),S(BOW_R),S(BOW_L),S(EOSW_R),  _______, _______, _______,
         _______,          _______, _______, _______, S(_EOL), _______,          S(_LT),  S(_DN),  S(_UP),  S(_RT),  _______, _______,          _______,
         _______, _______, SFTLT,   SFTRT,   _______, _______, _______,          S(_BOL), S(_PGDN),S(_PGUP),_______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% sticky:sel2
        (_______,          _______, _______, SBRACKS, _DEL,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, PHONY,   SPARENS, SBRACKS, SQUO,             _BSP,    _______, _______, _______, _______, _______, _______, _______,
         _______,          INDENT,  PASTE,   COPY,    _______, COMMENT,          _______, _______, _______, _______, _______, UNINDENT,         _______,
         _______, _______, _______, _______, CUT,     DUPL,    PHONY,            _______, _______, _______, _______, _______, _______, _______, _______,
         OSL_EDI,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% sticky:sel3
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, SELLINE, MRKLINE, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% sticky:sel4
        (_______,          _______, _______, _______, NNDOT,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, ABORTSEL,NNCOLON, NNCOMMA, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______,_______,  _______, _______, _______,          _______, _______, _______, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          NNSPACE, _______, NNENTER,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:sym
        (_______,          _______, _______, _______, _______, _______, _______, _LBR,    _QUO,    S(_GRV), _GRV,    C(_COM), _______, _______, _______,
         _______,          _______, FSEARCH, EEOL,    _BSL,    DDD,              S(_6),   S(_EQL), S(_SCL), _QUO,    S(_QUO), S(_7),   _______, _______,
         _______,          FPRNT,   _______, NLA,     CAPS,    S(_1),            S(_8),   LSWITCH, DQUOTES, _BSL,    S(_4),   S(_3),            _______,
         _______, _______, _______, _______, _______, _______, _______,          G(S(_G)),FINDNXT, FINDPRV, _SCL,    QUE,     _______, _______, _______,
         _______,                   _______, _______,          _______,  NEXT,   BRACES,           _______, _______,          _______, _______, _______),


        LAYOUT_all //%% oneshot:sym_ru
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, S(_GRV), _GRV,    _______, _______, _______, _______,
         _______,          _______, _______,G(_ENT),  _______, _______,          _______, S(_6),   S(_2),   _______, S(_DOT), _SLS,    _______, _______,
         _______,          _______, _______, _______, LSWITCH, _______,          S(_1),   _______, S(_8),   S(_EQL), S(_4),   S(_3),            _______,
         _______, _______, _______, _______, _______, _______, _______,          LCTL(_R),CTA(_S), LCTL(_W),_______, RUQUE,   _______, _______, _______,

         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:eeol
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _COM,    _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, S(_SCL), _______, _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, HELPKEY, _______, _SCL,    _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:bra
        (_______,          S(_1),   CCS,     S(_LBR), BRACES,  S(_RBR), _______, _______, FINDAT,  QUOTES,  _COM,    OCLIP,   _______, _______, _______,
         _______,          S(_9),   S(_0),   ABRACKS, S(_COM), S(_DOT),          ORC,     OTREE,   OCWD,    ORECENT, OHOME,   ODIRED,  _______, _______,
         C_ENT,            _LBR,    _RBR,    E_CC,    CCS,     BEGS,             PALETTE, MX,      OHOME,   _______, S(_4),   S(_3),            _______,
         _______, _______, _______, E_CV,    E_CX,    ORC,     HELPKEY,          LCTL(_R),CTA(_S), LCTL(_W),S(_8)   ,S(_SLS), E_CZ,    _______, _______,
         _______,                   _______, _______,          COM_SPC, ORC,     COM_ENT,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:bra_ru
        (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, S(_SLS), _______, _______, _______, _______,
         _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,          _______, S(_GRV),BRACKS_RU,_______, _GRV,             _______, _______, _______, _______, _______, _______,          _______,
         _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,        COM_SPC_RU,_______, _______,          _______, _______,          _______, _______, _______),

        LAYOUT_all //%% oneshot:ref
        (_______,          _______, _______, POPMARK, FSCR1,   _F11,    _______, _______, _______, _______, ALTSRCH, _______, _______, _______, _______,
         FSEL,             _______, EVALF,   EVALF,   _______, _______,          _______, SPLITRT, NEXTW,   PREVW,   DELOTHER,DELCUR,  _______, _______,
         DELCELL,          DELCELL, EVAL,    CELLUP,  CELLDN,  RECENTER,         _______, FSEARCH, FINDAT,  REPLACE, _______, _______,          _______,
         NEWABOVE,_______, NEWBELOW,_F5,     _F2,     _F12,    S(_F12),          _______, _______, _______, _______, _______, _______, _______, _______,
         _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),};
