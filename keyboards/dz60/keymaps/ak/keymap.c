// -*- compile-command: "cd ../../../../ && make dz60:ak-first:dfu" -*-

#include QMK_KEYBOARD_H

#include "keydef.h"
#include "win.h"

#define MODS_CTRL_MASK  (MOD_BIT(KC_LSHIFT)|MOD_BIT(KC_RSHIFT))

enum custom_keycodes {
   TEST = SAFE_RANGE,
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
   DQUOTES,
   CCS, // complete current statement
   BEGS,
   CCSNL,
   SCLSPC,  // semicolon space
   ENDSPC,
   ENDL,
   H_A,
   H_B,
   H_C,
   H_D,
   HOLD,
   SPRNT,
   FPRNT,
   SWITCHPY,
   DDD,
   ALTF4,

};

// -layers enum
enum {
   EN_LR,
   RU_LR,
   MACOS_LR,
   APPSWITCH_LR,
   NUM_LR,
   EDI_LR,
   SEL_LR,
   SEL2_LR,
   SEL3_LR,
   SEL4_LR,
   SYM_LR,
   RU_SYM_LR,
   NEXT_LR,
   BRA_LR,
   RU_BRA_LR,
   IDE_LR,
   REF_LR,
};


static uint16_t timer, rcmd_timer;
static const uint16_t timer_threshold = 250;
static const uint16_t oneshot_threshold = 700, oneshot_next_threshold = 1500;
static uint8_t rgblight_mode_current = RGBLIGHT_MODE_KNIGHT + 1;

static uint16_t oneshot_timer, oneshot_next_timer;
static uint8_t oneshot_down = 0, oneshot_fired = 0,
   oneshot_next_down = 0, oneshot_next_fired = 0;
static uint8_t caps = 0,
   sel_off = 0,
   sel2_off = 0,
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
      layer_and(~(15UL << SEL_LR));
      sel_off = 0;
   }
   if (sel2_off) {
     layer_off(SEL3_LR);
     layer_off(SEL4_LR);
     sel2_off = 0;
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
   if (record->event.pressed) {
     if (((layer == SEL_LR) || (layer == SEL3_LR)) && keycode != S(_RT)) sel2_off = 1;
   if (((layer == SEL4_LR) || (layer == SEL2_LR)) && record->event.pressed) sel_off = 1;
   }
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
   /*    if (keycode != OSL_EDI) oneshot_fired_check(1UL << EDI_LR); */
   /*    if (keycode != OSL_NUM) oneshot_fired_check(1UL << NUM_LR); */
   /*    if (keycode != OSL_SYM) oneshot_fired_check(3UL << SYM_LR); */
   /*    if (keycode != OSL_BRA) oneshot_fired_check(3UL << BRA_LR); */
   /*    if (keycode != OSL_IDE) oneshot_fired_check(1UL << IDE_LR); */
   /*    if (keycode != OSL_REF) oneshot_fired_check(1UL << REF_LR); */
      oneshot_fired_check(1UL << EDI_LR);
      oneshot_fired_check(1UL << NUM_LR);
      oneshot_fired_check(3UL << SYM_LR);
      oneshot_fired_check(3UL << BRA_LR);
      oneshot_fired_check(1UL << IDE_LR);
      oneshot_fired_check(1UL << REF_LR);
      if ((keycode != NEXT) && (1UL << NEXT_LR & layer_state)) oneshot_next_fired = 1;
}
   switch (keycode) {
    case OSL_REF: return oneshot_process(record, REF_LR, 0);
    case OSL_IDE: return oneshot_process(record, IDE_LR, 0);
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
	     keycode == _TAB ||
	     keycode == _DN ||
	     keycode == _UP) {
            caps = 0;
            unregister_code(KC_LSHIFT);
            return 0;
         }
         if (keycode == MACMETA || // стоп слова с дальнейшей обработкой
	     keycode == BSEL) {
	   caps = 0;
            unregister_code(KC_LSHIFT);
         }
     if (0b11111111111111111111111111111000 & layer_state) {
       unregister_code(KC_LSHIFT);
     }

      }}

   /* turn off ru on certain keys */
   if (record->event.pressed) {
      if (keycode == _ENT ||
          keycode == G(_ENT) ||
          keycode == G(A(_ENT)) ||
          keycode == MACMETA ||
	  keycode == BSEL) {
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

      
   case MACMETA:
      if (record->event.pressed) {
         timer = timer_read();
         layer_on(MACOS_LR);
      } else {
	    layer_and(~(3UL << MACOS_LR));
         if (timer_elapsed(timer) < timer_threshold) {
            register_code(KC_ENTER);
            unregister_code(KC_ENTER);
            unregister_code(KC_LGUI);
            unregister_code(KC_LALT);
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
     case ALTF4:
       unregister_code(KC_LALT);
       register_code(KC_LALT);
       register_code(_F4);
       unregister_code(_F4);
       unregister_code(KC_LALT);
       register_code(KC_LALT);
       register_code(_TAB);
       unregister_code(_TAB);
       register_code(KC_LSHIFT);
       register_code(_TAB);
       unregister_code(_TAB);
       unregister_code(KC_LSHIFT);
       /* unregister_code(KC_LALT); */
       return 0;
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
	send_string(", ");
         return 0;	 
      case NNCOLON:
	send_string(":");
         return 0;	 
      case NNDOT:
	send_string(". ");
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
       send_string(SS_UP(X_LSHIFT) SS_TAP(X_LEFT) XBOL SS_DOWN(X_LSHIFT) SS_TAP(X_RIGHT) XEOL
		   SS_TAP(X_DOWN) XBOL SS_UP(X_LSHIFT));
       return 0;
     case MRKLINE:
       send_string(SS_UP(X_LSHIFT) XEOL SS_DOWN(X_LSHIFT) SS_TAP(X_LEFT) XBOL SS_UP(X_LSHIFT));
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
      case XCDCLIP:
         SEND_STRING("cd " SS_LCTRL("v") SS_TAP(X_ENTER));
         return 0;
      }}

   
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
      (_ESC,             S(_BSL), PARENS,  S(_MIN), _TAB,    S(_5),   _______, S(_2),   _B,      _Y,      OSL_BRA, FINDNXT, FINDPRV, _NO,      KC_END,
       BRACKS,           BSEL,    OSL_SYM, _O,      _EQL,    _MIN,             QUOTES,  _G,      _C,      _R,      _F,      _K,      NEXT,    _F3,
       OSM(MOD_LSFT),    _DOT,    _A,      _E,      _I,      _U,               _L,      _H,      _T,      _N,      _S,      _BSP,    G(A(_ENT)),
       _LSFT,    _A,     _J,      _Q,      _SLS,    _P,      _ESC,             _D,      _M,      _W,      _V,      _X,      _Z,      C(A(_Y)),_UP,
       OSL_EDI,                   _LGUI,   RALT,             _SPC,    OSL_NUM, MACMETA, RCMD,             _LCTL,            OSL_REF, OSL_REF, _LCTL),

      LAYOUT_all //%% plain:ru
      (_______,          _______, _SCL,    _______, _______, _______, _______, _______, _COM,    _E,      _______, _______, _______, _______, _______,
       _W,               _______, _______, _J,      _______, _______,          _Q,      _U,      _Z,      _H,      _A,      _P,      _RBR,    _O,
       _______,          _SLS,    _F,      _T,      _B,      _M,               _K,      _R,      _N,      _Y,      _C,      _______,          _______,
       _DOT, _______,    _QUO,    _I,      _S,      _G,      _______,          _L,      _V,      _D,      _X,      _LBR,    _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% mod:os
      (MOD_SWITCH,       _______, LSCR,    RSCR,    FSCR,    _______, _______, _______, KILLTAB, KILLAPP, G(A(_W)),G(_W),   G(_Z),   _______, _______,
       WIN,              _______, HOLD,    BROWSER, MTTASK2, _______,          PTAB,    FINDNXT, LTAB,    RTAB,    FINDPRV, G(_RBR), _______, _______,
       _______,          CUT,     TERM,    EMACS,   MTTASK,  MTTASK3,          HYPR(_K),CMDTAB,  SCMDTAB, SAVE,    FILENAME,XCDCLIP,          _______,
       _______, _______, G(_A),   _______, _______, FILEDIR, _______,          _______, UPD,     _______, _______, G(A(_C)),_______, _______, RGB_TOG,
       _______,                   _______, _______,          OFFMETA, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% appswitch
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, KILLAPP, ALTF4,   APPDN,   APPUP,   _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, _______, _______, APPDN,   APPUP,   _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:num
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, COUT,    CIN,              NAMESPACE,COUT,   _0,      _9,      CIN, _______, _______, _______,
       _CAP,             _______, _______, _______, _ESC,    _______,          _6,      _1,      _2,      _3,      _4,      _5,               _______,
       _______, _______, _______, _______, REDEBUG, CLDEBUG, _______,          _7,      _8,      _______, CC_PLS,  CC_MIN,  _______, _______, _______,
       _______,                   _______, _______,          SCLSPC, _______, CCS,               _______, _______,          _______, _______, _______),


      LAYOUT_all //%% oneshot:edi
      (BOSW,             BOSW,    EOSW,    POPMARK, _______, _BOF,    _EOF,    G(_LBR),  SWAPDN, _PGDN,   _PGUP,   SWAPUP,  _DEL,    _______, RESET,
       ALL,              _______, UNDO,    DELBOW,  DELEOW,  G(_O),            _______,  BOSW,   BOW,     FBOW,    EOW,     _______, _______, C(_F4),
       G(S(_D)),         CUT,     PASTE,   COPY,    EOL,     COMMENT,          _LT,     _DN,     _UP,     _RT,     DELEOL,  _______,          _______,
       G(_X),   _______, CC_PLS,  DELETE,  CUT,     DUPL,    _______,          BOL,     HARDBOL, DELEOL,  _DEL,    G(_DN),  _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel
      (_______,          _______, _______, _______, S(_BOF), S(_EOF), _______, _______, SWAPDN,  S(_PGDN),S(_PGUP),SWAPUP,  _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, _______, S(BOW),  S(FBOW), S(EOW),  _______, _______, _______,
       _______,          _______, _______, _______, S(EOL),  _______,          S(_LT),  S(_DN),  S(_UP),  S(_RT),  _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          S(BOL),  S(HARDBOL),_______,_______,_______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel2
      (_______,          _______, _______, SBRACKS, _DEL,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, PHONY,   SPARENS, _______, SQUO,             _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          DUPL,    PASTE,   COPY,    _______, COMMENT,          _______, _______, _______, _______, _______, _BSP,             _______,
       _______, _______, _______, _______, CUT,     DUPL,    _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       OSL_EDI,                   _______, _______,          _______, OSL_EDI, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel3
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, SELLINE, MRKLINE, _______, _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% sticky:sel4
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, ABORTSEL,NNCOLON, NNDOT,   _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______,_______,  _______, _______, _______,          _______, _______, _______, _______, NNCOMMA, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          NNSPACE, _______, NNENTER,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:sym
      (_______,          _______, _______, _______, _______, _______, _______, _LBR,    _QUO,    S(_GRV), _GRV,    _RBR,    _______, _______, _______,
       _______,          _______, FSEARCH, _ESC,    _BSL,    DDD,              S(_6),   S(_EQL), S(_SCL), _QUO,    S(_QUO), S(_7),   _______, _______,
       _______,          FPRNT,   _______, SPRNT,   LSWITCH, S(_1),            S(_QUO), CAPS,    DQUOTES, QUOTES,  S(_4),   S(_3),            _______,
       _______, _______, _______, _______, _______, _______, _______,          G(S(_G)),FINDPRV, FINDNXT, S(_8),   QUE,     _______, _______, _______,
       _______,                   _______, _______,          _______,  NEXT,   BRACES,           _______, _______,          _______, _______, _______),


      LAYOUT_all //%% oneshot:sym_ru
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, S(_GRV), _GRV,    _______, _______, _______, _______,
       _______,          _______, _______,G(_ENT),  _______, _______,          _______, S(_6),   S(_2),   _______, S(_DOT), _SLS,    _______, _______,
       _______,          _______, _______, _______, LSWITCH, _______,          S(_1),   _______, S(_8),   S(_EQL), S(_4),   S(_3),            _______,
       _______, _______, _______, _______, _______, _______, _______,          LCTL(_R),CTA(_S), LCTL(_W),_______, RUQUE,   _______, _______, _______,
       
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:next
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, NCOLON,  NCOMMA,  _______, _______, _______, _______, _______,
       _______,          _______, _______, _______, NCOMMA,  PYBLOCK,          _______, NCOLON,  _______, NCOLON,  _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          _______, _______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:bra
      (_______,          S(_1),   CCS,     S(_LBR), BRACES,  S(_RBR), _______, _______, FINDAT,  QUOTES,  _COM,    PYBLOCK, _______, _______, _______,
       _______,          S(_9),   S(_0),   ABRACKS, S(_COM), S(_DOT),          OPN1,    OPNTR,   OPN,     RECENTF, OPNHOME, DIRED,   _______, _______,
       C_ENT,            _LBR,    _RBR,    S(_ENT), CCS,     BEGS,             PALETTE, MX,      OPNHOME, S(_EQL), S(_4),   S(_3),            _______,
       _______, _______, _______, _______, _______, C(_ENT), HELPKEY,          LCTL(_R),CTA(_S), LCTL(_W),S(_8)   ,S(_SLS), _______, _______, _______,
       _______,                   _______, _______,          COM_SPC, C(_ENT), EXECELL,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:bra_ru 
      (_______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, S(_SLS), _______, _______, _______, _______,
       _______,          _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,          _______, S(_GRV),BRACKS_RU,_______, _GRV,             _______, _______, _______, _______, _______, _______,          _______,
       _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,        COM_SPC_RU,_______, _______,          _______, _______,          _______, _______, _______),

      LAYOUT_all //%% oneshot:ide
      (_______,          _______, _______, G(_B),   _SCL,    _______, _______, _______, KILLTAB, KILLAPP, G(S(_E)),CX_3,    HYPR(_LBR),_______,HYPR(_RBR),
       _______,          _______, E_CZ,    _______, _______, S(_F6),           _SCL,    SPLITRT, OTHER,   DELOTHER,OTHER,   HYPR(_G),_______, CX_LBRC,
       _______,          _______, E_CV,    E_CC,    _DEBUG,  A(S(_1)),         A(S(_SCL)),C(S(_6)),S(_F10), C(_ENT),G(_S),  S(A(_F10)),       G(_F2),
       _______, _______, _______, _______, E_CX,    _______, _______,          G(_L),C(A(S(_5))),CXCJ_CD,CXCJ_CC,  CXCJ_SD, CX_CC,   _______, _______,
       _______,                   _______, _______,          SCLSPC,  SCLSPC,  CCS,              _______, _______,          _______, RGB_HUI, RGB_HUD),

      LAYOUT_all //%% oneshot:ref
      (_______,          _______, _______, POPMARK,UNPOPMARK,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
       FSEL,             _______, UNDO,    _F5,     _______, _______,          _SCL,    SPLITRT, OTHER,   DELOTHER,_______, _______, _______, _______,
       _______,          G(_U),   PASTE,   _F2,     _F12,    S(_F12),          _C,      FSEARCH, _______, _______, _______, EQL_EQL,          _______,
       _______, _______, _______, _______, _______, C(_MIN), _______,          _______, _______, _______, _______, _______, _______, _______, _______,
       _______,                   _______, _______,          EQL_SPC, _EQL,    _______,          _______, _______,          _______, _______, _______),};
