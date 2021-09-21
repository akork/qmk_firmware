// -*- compile-command: "cd ../../../../ && make dz60:ak-first:dfu" -*-


// option + f12 terminal

#include QMK_KEYBOARD_H

#include "win.h"

#define MODS_CTRL_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))

// static bool isCmdSetByMeta = false;

enum custom_keycodes {
   TEST = SAFE_RANGE,
   OFFMETA,
   XCDCLIP,
   CAPS,
   SQUO,
   SPARENS,
   SBRACKS,
   STICKY_SEL,
   PHONY,
   NEXT,
   NCOMMA,
   NCOLON,
   C_ENT,
   COUT,
   CIN,
   NAMESPACE,
   PLUSPLUS,
   MOD_SWITCH,
   JOKER,
   JOIN_L,
   DUP,
   L_SEL,
   L_DEL,
   W_SEL,
   PGDN,
   EQL_EQL,
   EQL_SPC,
   ENDL_JOKER,
   RT2_SPC,
   SEARCH,
   LEADER,
   PYBLOCK,
   CBLOCK,
   BSP,
   YANK,
   KILL,
   OSL_NUM,
   OSL_EDI,
   OSL_SYM,
   OSL_BRA,
   OSL_IDE,
   OSL_REF,
   LGUI,
   MAC,
   WIN,
   MACMETA,
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
   CCS, // complete current statement
   CCSNL,
   SCLSPC,  // semicolon space
   ENDSPC,
   ENDL,
   H_A,
   H_B,
   H_C,
   H_D,

};

// -layers enum
enum {
   EN_LR,
   RU_LR,
   MACOS_LR,
   NUM_LR,
   EDI_LR,
   SEL_LR,
   SEL2_LR,
   SYM_LR,
   RU_SYM_LR,
   NEXT_LR, // 10
   BRA_LR,
   RU_BRA_LR,
   IDE_LR,
   REF_LR,
};

#define _LSFT KC_LSFT
#define _LCTL KC_LCTL
#define _LGUI KC_LGUI
#define _LALT KC_LALT

#define TR KC_TRNS

#define _U KC_U
#define _1 KC_1
#define _2 KC_2
#define _3 KC_3
#define _4 KC_4
#define _5 KC_5
#define _6 KC_6
#define _7 KC_7
#define _8 KC_8
#define _9 KC_9
#define _0 KC_0
#define _A KC_A
#define _B KC_B
#define _C KC_C
#define _D KC_D
#define _E KC_E
#define _F KC_F
#define _G KC_G
#define _H KC_H
#define _I KC_I
#define _J KC_J
#define _K KC_K
#define _L KC_L
#define _M KC_M
#define _N KC_N
#define _O KC_O
#define _P KC_P
#define _Q KC_Q
#define _R KC_R
#define _S KC_S
#define _T KC_T
#define _U KC_U
#define _V KC_V
#define _W KC_W
#define _X KC_X
#define _Y KC_Y
#define _Z KC_Z

#define _F1 KC_F1
#define _F2 KC_F2
#define _F3 KC_F3
#define _F4 KC_F4
#define _F5 KC_F5
#define _F6 KC_F6
#define _F7 KC_F7
#define _F8 KC_F8
#define _F9 KC_F9
#define _F10 KC_F10
#define _F11 KC_F11
#define _F12 KC_F12
#define _F13 KC_F13
#define _F14 KC_F14
#define _F15 KC_F15
#define _F16 KC_F16
#define _F17 KC_F17
#define _F18 KC_F18
#define _F19 KC_F19
#define _F20 KC_F20
#define _F21 KC_F21
#define _F22 KC_F22
#define _F23 KC_F23
#define _F24 KC_F24

#define _RT KC_RGHT
#define _LT KC_LEFT
#define _UP KC_UP
#define _DN KC_DOWN
#define _VUP KC__VOLUP
#define _VDN KC__VOLDOWN

#define _CSL KC_BSLS
#define _COM KC_COMM
#define _MIN KC_MINS
#define _TAB KC_TAB
#define _TB KC_TAB
#define _CAP KC_CAPS
#define _LBR KC_LBRC
#define _RBR KC_RBRC
#define _NO KC_NO
#define _BSP KC_BSPC
#define _ESC KC_ESC
#define _BSL KC_BSLS
#define _SPC KC_SPC
#define _DOT KC_DOT
#define _SLS KC_SLSH
#define _QUO KC_QUOT
#define _ENT KC_ENT
#define _DOW KC_DOWN
#define _UP KC_UP
#define _EQL KC_EQL
#define _SCL KC_SCLN
#define _GRV KC_GRV
#define CTA(x) LCTL(LALT(x))
#define ALT(x) LALT(x)
#define CTL(x) LCTL(x)
#define GUI(x) LGUI(x)
#define GUA(x) LGUI(LALT(x))
#define ALG(x) LALT(LGUI(x))
#define GACS(x) LGUI(LALT(LCTL(LSFT(x))))
#define _PGUP KC_PGUP
#define _PGDN KC_PGDN
#define _HOME KC_HOME
#define _END KC_END
#define _DEL KC_DELETE
#define GR(x) RGUI(x)
/* #define S(x) RSHIFT(x) */

#define SS_RGUI(string) SS_DOWN(X_RGUI) string SS_UP(X_RGUI)

static uint16_t timer, rcmd_timer;
static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 700, oneshot_next_threshold = 1500;
static uint8_t rgblight_mode_current = RGBLIGHT_MODE_KNIGHT + 1;

static uint16_t oneshot_timer, oneshot_next_timer, esc_promise_timer;
static uint8_t oneshot_down = 0, oneshot_fired = 0,
   oneshot_next_down = 0, oneshot_next_fired = 0;
static uint8_t caps = 0,
   sel_off = 0,
   mod = 0,
   meta_up_signal = 0;



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

void oneshot_check(uint32_t layer_mask) {
   if (layer_mask & layer_state) {
      if ((!oneshot_down && oneshot_fired) ||
          (!oneshot_down && timer_elapsed(oneshot_timer) > oneshot_threshold)) {
         layer_and(~layer_mask);
      }
   }
}

void oneshot_fired_check(uint32_t layer_mask) {
   if (layer_mask & layer_state) oneshot_fired = 1;
}

void matrix_scan_user(void) {
   oneshot_check(1UL << NUM_LR);
   oneshot_check(1UL << IDE_LR);
   oneshot_check(1UL << REF_LR);
   oneshot_check(1UL << EDI_LR);
   oneshot_check(3UL << SYM_LR);
   oneshot_check(3UL << BRA_LR);

   if (biton32(layer_state) == NEXT_LR) {
      if ((!oneshot_next_down && oneshot_next_fired) ||
          (!oneshot_next_down && timer_elapsed(oneshot_next_timer) > oneshot_next_threshold)) {
         layer_off(NEXT_LR);
      }
   }
   if (sel_off) {
      layer_and(~(3UL << SEL_LR));
      sel_off = 0;
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
   if (layer == SEL2_LR && record->event.pressed) sel_off = 1;
   if (layer == MACOS_LR) {
      if (keycode == LTAB ||
          keycode == RTAB) {
         meta_up_signal = 1;
      } else {
         meta_up_signal = 0;
      }
   }

   // oneshot processing:
   if (record->event.pressed) {
      if (keycode != OSL_EDI) oneshot_fired_check(1UL << EDI_LR);
      if (keycode != OSL_NUM) oneshot_fired_check(1UL << NUM_LR);
      if (keycode != OSL_SYM) oneshot_fired_check(3UL << SYM_LR);
      if (keycode != OSL_BRA) oneshot_fired_check(3UL << BRA_LR);
      if (keycode != OSL_IDE) oneshot_fired_check(1UL << IDE_LR);
      if (keycode != OSL_REF) oneshot_fired_check(1UL << REF_LR);
   }
   switch (keycode) {
   case OSL_REF: return oneshot_process(record, REF_LR, 0);
   case OSL_IDE: return oneshot_process(record, IDE_LR, 0);
   case OSL_EDI: return oneshot_process(record, EDI_LR, 0);
   case OSL_SYM: return oneshot_process(record, SYM_LR, 1);
   case OSL_BRA: return oneshot_process(record, BRA_LR, 1);
   case OSL_NUM: return oneshot_process(record, NUM_LR, 0);
   }

   if (record->event.pressed) {
      if (caps == 1) {
         if (keycode == CAPS ||
             keycode == _ESC ||
             keycode == MACMETA) {
            caps = 0;
            unregister_code(KC_LSHIFT);
            return 0;
         }

         if ((keycode >= OSL_NUM &&
              keycode <= OSL_REF) ||
             keycode == _SPC) {
            caps = 0;
            unregister_code(KC_LSHIFT);
         }
      }}

   /* turn off ru on certain keys */
   if (record->event.pressed) {
      if (keycode == _ENT ||
          keycode == G(_ENT) ||
          keycode == G(A(_ENT)) ||
          keycode == MACMETA) {
         if (1UL << RU_LR & layer_state)
            ru_turn_off();
      }
   }

   // :layer_triggers
   switch(keycode) {
   case PHONY:
      return 0;
   case CAPS:
      if (record->event.pressed) {
         caps = 1;
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
   case NEXT:
      /* layer_off(SYM_LR); */
      /* layer_off(SYM_RU_LR); */
      if (record->event.pressed) {
         send_string(SS_TAP(X_RIGHT));
         oneshot_next_timer = timer_read();
         oneshot_next_down = 1;
         oneshot_next_fired = 0;
         layer_on(NEXT_LR);
      } else {
         oneshot_next_down = 0;
      }
      return 0;
   case STICKY_SEL:
      if (record->event.pressed) {
         layer_on(SEL_LR);
         layer_on(SEL2_LR);
      } else {
      }
      return 0;
   case LEADER:
      if (record->event.pressed) {
         send_string(SS_LCTRL(SS_RGUI("x")));
      } else { }
      return false;
   case JOKER:
      if (record->event.pressed) {
         esc_promise_timer = timer_read();
         register_code(_LGUI);
      } else {
         unregister_code(_LGUI);
      }
      return 0;
   case TEST:
      send_string(SS_DOWN(X_LSHIFT) SS_DOWN(X_LGUI) SS_TAP(X_LEFT) SS_UP(X_LGUI) SS_UP(X_LSHIFT));
      /* rgblight_mode_noeeprom(rgblight_mode_current++); // sets mode to Fast breathing without saving */
      return false;
   case MACMETA:
      if (record->event.pressed) {
         timer = timer_read();
         layer_on(MACOS_LR);
      } else {
         layer_off(MACOS_LR);
         if (timer_elapsed(timer) < timer_threshold) {
            register_code(KC_ENTER);
            unregister_code(KC_ENTER);
            unregister_code(KC_LGUI);
            unregister_code(KC_LALT);
         } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_LALT);
            if (meta_up_signal) {
               register_code(KC_LCTRL);
               register_code(KC_LALT);
               register_code(KC_LSHIFT);
               register_code(_F24);
               unregister_code(_F24);
               unregister_code(KC_LCTRL);
               unregister_code(KC_LALT);
               unregister_code(KC_LSHIFT);
            }

         }
         /* clear_mods(); */
      }
      return 0;
   case OFFMETA:
      meta_up_signal = 0;
      return 0;
   }

   // :cmd_tab
   if (biton32(layer_state) == MACOS_LR) {
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
               del_mods(MOD_BIT(SWITCH));
               register_code(SWITCH);
               register_code(KC_TAB);
               unregister_code(KC_TAB);
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
         default:
            unregister_cmd_after_cmdtab();
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
      clear_weak_mods();
      send_keyboard_report();
      switch(keycode) {
      case NCOMMA:
         send_string(", ");
         return 0;
      case NCOLON:
         send_string(": ");
         return 0;
      case C_ENT:
         send_string(SS_LCTRL(SS_TAP(X_ENTER)) SS_TAP(X_ENTER));
         return 0;
      case SQUO:
         send_string(SS_LGUI("x") "''" SS_TAP(X_LEFT) SS_LGUI("v"));
         return 0;
      case SPARENS:
         send_string(SS_LGUI("x") "()" SS_TAP(X_LEFT) SS_LGUI("v"));
         return 0;
      case SBRACKS:
         send_string(SS_LGUI("x") "[]" SS_TAP(X_LEFT) SS_LGUI("v"));
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
      case XCDCLIP:
         SEND_STRING("cd " SS_LCTRL("v") SS_TAP(X_ENTER));
         return 0;
      }}



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
      (_ESC,             _ESC,    NEXT,    S(_MIN), _ESC,    S(_5),   _______, S(_EQL), _B,      _Y,    OSL_BRA,   FINDNXT, FINDPRV,_NO,      KC_END,
       STICKY_SEL, OSM(MOD_LSFT), OSL_SYM, _O,      _BSP,    _MIN,             OSL_IDE,    _G,      _C,      _R,      _F,      _K,      _SLS,    _F3,
       OSL_NUM,          _DOT,    _A,      _E,      _I,      _U,               _L,      _H,      _T,      _N,      _S,      OSL_REF,          G(A(_ENT)),
       _LSFT,    _A,     _J,      _Q,      S(_2),   _P,      _TAB,             _D,      _M,      _W,      _V,      _X,      _Z,      C(A(_Y)),_UP,
       _LCTL,                     _LGUI,   RALT,             _SPC,   OSL_EDI,  MACMETA, RCMD,             _VDN,             _VUP,    MACMETA, _F20),

      LAYOUT_all //%% plain:ru
      (_______,          _______, _SCL,    _______, _M,      _______, _______, _______, _COM,    _E,      _______, _______, _______, _______, _______,
       _______,          _______, _______, _J,      _______, _______,          _Q,      _U,      _Z,      _H,      _A,      _P,      _RBR,    _O,
       _______,          _SLS,    _F,      _T,      _B,      _E,          _K,      _R,      _N,      _Y,      _C,      _W,               _______,
       _DOT, _______,    _QUO,    _I,      _S,      _G,      _______,          _L,      _V,      _D,      _X,      _LBR,    _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% mod:os
      (MOD_SWITCH,       _______, LSCR,    FSCR,    RSCR,    _______, _______, _______, KILLTAB, KILLAPP, G(A(_W)),G(_W),   G(_Z),   _______, _______,
       WIN,              _______, C(_G),   CUT,     SPTLT1,    _______,          PTAB,    FINDNXT, LTAB,    RTAB,    FINDPRV, G(_RBR), _______, _______,
       _______,          CUT,     TERM,    COPY,    SPTLT,   FILE,             HYPR(_K),CMDTAB,  SCMDTAB, SAVE,    FILE,    XCDCLIP,          _______,
       _______, _______, G(_A),   _______, _______, FILEDIR, _______,          _______, UPD,     _______, _______, G(A(_C)),_______, _______, RGB_TOG,
       _______,                   _______, _______,          OFFMETA, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:num
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, COUT,    CIN,              NAMESPACE,COUT,   _0,      _9,      CIN, _______, _______, _______,
       _CAP,             _______, _______, _______, CANCEL,  CANCEL2,          _6,      _1,      _2,      _3,      _4,      _5,               _______,
       _______, _______, _______, _______, REDEBUG, CLDEBUG, _______,          _7,      _8,      _______, CC_PLS,  CC_MIN,  _______, _______, _______,
       _______,                   _______, _______,          SCLSPC, _______, CCS,               _______, _______,          _______, _______, _______),


      LAYOUT_all //%% oneshot:edi
      (BOSW,             EOSW,    DELBOW,  DELEOW,  _BOF,    _EOF,    _______, G(_LBR), C(G(_DN)),SWAPDN,SWAPUP,  C(G(_UP)),_ESC,   _______,  RESET,
       _______,          _______, UNDO,    CUT,     _______,  G(_O),            C(A(G(_5))),_PGDN, BBOW,   FEOW,    _PGUP,   C(_K),   _______, _______,
       G(S(_D)),         ALL,     PASTE,   COPY,    DELBOW,   COMMENT,          _LT,     _DN,     _UP,     _RT,     EOL,      _DEL,               _______,
       G(_X),   _______, CC_PLS,  CC_MIN,  COMMENT, DUPL,     _______,          BOL,     HARDBOL, DELEOL,  DELBOL,  G(_DN),_______, _______, _______,
       _______,                   _______, _______,          _BSP,    _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel
      (_______,          _______, _______, _______, _______, _______, _______, G(_LBR), G(_RBR), SWAPDN, SWAPUP,   _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, S(_PGDN),S(BBOW), S(FEOW), S(_PGUP),_______,_______,_______,
       _______,          _______, _______, _______, _______, _______,          S(_LT),  S(_DN),  S(_UP),  S(_RT),  S(EOL),   _______,         _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, S(HARDBOL),_______,_______,_______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel2
      (_______,          _______, _______, _______, SPARENS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, PHONY,   CUT,     PHONY,   SQUO,             _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          DUPL,    PASTE,   COPY,    COMMENT, _BSP,             _______, _______, _______, _______, _______, _______,          _______,
       _______, _______, _______, _______, CUT,     DUPL,    _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          _DEL,    OSL_EDI, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:sym
      (_______,          _______, _______, _______, _______, _______, _______, _LBR,    _QUO,    S(_GRV), _GRV,    _RBR,    _______, _______, _______,
       _______,          _______, DEBUG,   A(_O),   _BSL,    S(_BSL),          S(_6),   S(_7),   S(_QUO), S(_EQL), S(_DOT), _BSL,    _______, _______,
       _______,          S(_2),   _______, _______, LSWITCH, S(_1),            BRACKS,  PARENS,  QUOTES,  S(_SCL),S(_4),    S(_3),            _______,
       _______, _______, _______, _______, _______, _______, _______,          G(S(_G)),FINDPRV, FINDNXT, S(_8),   QUE,     _______, _______, _______,
       _______,                   _______, _______,          _______,  NEXT,    CAPS,            _______, _______,          _______, _______, _______),


      LAYOUT_all //%% oneshot:sym_ru
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, S(_GRV), _GRV,    _______, _______, _______, _______,
       _______,          _______, _______,G(_ENT),  _______, _______,          _______, S(_6),   S(_2),   _______, S(_DOT), _SLS,    _______, _______,
       _______,          _______, _______, _______, LSWITCH, _______,          S(_1),   S(_6),   S(_8),   S(_EQL), S(_4),   S(_3),            _______,
       _______, _______, _______, _______, _______, _______, _______,          LCTL(_R),CTA(_S), LCTL(_W),_______, RUQUE,   _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:next
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, NCOLON,  NCOMMA,  _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, CCS,     _______,          _______, _______, _______, _______, _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:bra
      (_______,          _______,PARENS_ENDL,S(_LBR),BRACES, S(_RBR), _______, OPEN1,   BSEARCH, FSEARCH, _COM,    PYBLOCK, _______, _______, _______,
       _______,          S(_0),   S(_9),   ABRACKS, S(_COM), S(_DOT),          OPEN1,   OPEN,    OPEN1,   RECENTF, PYBLOCK, PYBLOCK, _______, _______,
       C_ENT,            _RBR,    _LBR,    C(_ENT), C_ENT,   S(_ENT),          MCOLON,  MX,      _______, S(_EQL), S(_4),   S(_3),           _______,
       _______, _______, _______, _______, _______, C(_ENT), _______,          LCTL(_R),CTA(_S), LCTL(_W),S(_8)   ,S(_SLS), _______, _______, _______,
       _______,                   _______, _______,          COM_SPC, C(_ENT), G(_ENT),          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:bra_ru
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, S(_SLS), _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, S(_GRV),BRACKS_RU,_______, _GRV,             _______, _______, _______, _______, _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,        COM_SPC_RU,_______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:ide
      (_______,          _______, _______, G(_B),   _SCL,    _______, _______, _______, KILLTAB, KILLAPP, G(S(_E)),CX_3,    HYPR(_LBR),_______,HYPR(_RBR),
       _______,          _______, _______, _______, _______, S(_F6),           CX_G, G(C(S(_J))),OTHER,   DELOTHER,OTHER,   HYPR(_G),_______, CX_LBRC,
       _______,          _______, _______, _______, DEBUG,   A(S(_1)),         A(S(_SCL)),C(S(_6)),S(_F10), C(_ENT),G(_S),  S(A(_F10)),       G(_F2),
       _______, _______, _______, _______, _______, _______, _______,          G(_L),C(A(S(_5))),CXCJ_CD,CXCJ_CC,  CXCJ_SD, CX_CC,   _______, _______,
       _______,                   _______, _______,          SCLSPC,  SCLSPC,  CCS,              _______, _______,          _______, RGB_HUI, RGB_HUD),

      LAYOUT_all //%% oneshot:ref
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, PASTEI,  _F5,     _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          G(_U),   PASTE,   _F2,     _F12,    S(_F12),          _C,      _______, _______, _______, _______, EQL_EQL,          _______,
       _______, _______, _______, _______, _______, C(_MIN), _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          EQL_SPC, _EQL,    _______,          _______, _______,          _______, _______, _______),};
