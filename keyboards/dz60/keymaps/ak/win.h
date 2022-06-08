#define SWITCH KC_LALT
#define APPUP A(_UP)
#define APPDN A(_DN)
#define MTTASK A(_SPC)
#define MTTASK2 KC_LWIN
#define MTTASK3 C(A(S(_SPC)))
#define TERM C(G(_1))
#define XTERM SS_DOWN(X_LCTL) SS_DOWN(X_LGUI) "1" SS_UP(X_LGUI) SS_UP(X_LCTL)
#define EMACS C(G(_3))
#define XEMACS SS_DOWN(X_LCTL) SS_DOWN(X_LGUI) "3" SS_UP(X_LGUI) SS_UP(X_LCTL)
#define BROWSER C(G(_2))
#define XBROWSER SS_DOWN(X_LCTL) SS_DOWN(X_LGUI) "2" SS_UP(X_LGUI) SS_UP(X_LCTL)
#define KILLAPP A(_F4)
#define APKILL C(_W)
#define FSCR G(_UP)
#define LSCR G(_LT)
#define RSCR G(_RT)

// tabs
#define LTAB C(S(_TAB))
#define RTAB C(_TAB)
#define PTAB C(_Q)
#define KILLTAB C(_W)
#define MAGIC _F19

// layout
#define SPLITRT C(_F19)
#define OTHER C(A(_F19))
#define DELOTHER C(A(S(_F19)))

#define HELPKEY C(S(A(_H)))
#define MX C(S(_P))
#define MCOLON A(S(_SCL))
#define PALETTE C(_L)

#define ORECENT C(_P)
#define OCWD A(_D)
#define ORC C(A(_O))
#define OHOME C(A(S(_O)))
#define OCLIP C(_O)
#define OTREE _F6
#define ODIRED C(A(S(_E)))

#define PATH A(S(_C))
#define CWD C(A(S(_C)))

#define UPD _F5
#define SAVE C(_S)
#define NENTRY A(_N)
#define PENTRY A(_P)

// ??? some substitutions for emacs standard keys
#define E_CX _F18
#define E_CC C(_F18)
#define E_CV C(S(_F18))
#define E_CZ C(A(S(_F18)))

// motion
#define CELLDN C(_F8)
#define CELLUP C(_F9)
#define RECENTER C(_F10)
#define POPMARK _F17
#define UNPOPMARK C(_F17) //:TD
#define FSEARCH C(_F)
#define BSEARCH C(S(_F))
#define ALTSRCH _F3
#define FINDNXT C(_G)
#define FINDPRV C(S(_G))
#define FINDAT C(S(_G))
#define REPLACE C(S(_F))

#define BOW_L C(_LT)
#define BOW_R C(_RT)
#define EOSW_R A(_RT)
#define EOSW_L A(_LT)
#define EOW_R C(A(S(_RT)))
#define _EOL KC_END
#define XEOL SS_TAP(X_END)
#define XBOL SS_TAP(X_HOME)
#define XBOW SS_DOWN(X_LCTL) SS_TAP(X_LEFT) SS_UP(X_LCTL)
#define XEOW SS_DOWN(X_LCTL) SS_TAP(X_RIGHT) SS_UP(X_LCTL)
#define _BOL A(_M)
#define HARDBOL KC_HOME
#define _EOF C(KC_END)
#define _BOF C(KC_HOME)

// editing
#define DELETE S(KC_DELETE)
#define CUT C(_X)
#define COPY C(KC_INSERT)
#define PASTE S(KC_INSERT)
#define PASTEI C(S(_V))
#define ALL C(_A)
#define UNDO C(_Z)
#define REDO C(S(_Z))
#define DUPL C(S(_D))
#define COMMENT C(_SLS)
#define DELBOL C(S(_BSP))
#define DELEOL C(S(_DEL))
#define DELBOW C(_BSP)
#define DELEOW C(_DEL)
#define DELEOSW C(A(S(_DEL)))
#define SWAPUP A(_UP)
#define SWAPDN A(_DN)
#define NLBELOW C(A(_4))
#define INDENT _TAB
#define UNINDENT S(_TAB)
#define SFTRT C(_RBR)
#define SFTLT C(_LBR)

// programming
#define EVAL C(_ENT)
#define EVALF S(_ENT) // forward
#define EVALA _F12 // above
#define EVALCUR C(A(_F11))
#define _DEBUG _F5
#define CLDEBUG S(_F5)
#define REDEBUG C(S(_F5))

// some keys
#define LABRC S(_COM)
#define RABRC S(_DOT)
#define QUE S(_SLS)
#define RUQUE S(_7)
