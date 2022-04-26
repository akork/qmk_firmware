#define SWITCH KC_LALT
#define APPUP A(_UP)
#define APPDN A(_DN)
#define MTTASK A(_SPC)
#define MTTASK2 KC_LWIN
#define MTTASK3 C(A(S(_SPC)))
#define TERM C(G(_1))
#define EMACS C(G(_3))
#define BROWSER C(G(_2))
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
#define RECENTF C(_P)
#define OPN A(_D)
#define OPN1 C(A(_O))
#define OPNHOME C(A(S(_O)))
#define OPNTR _F6
#define DIRED C(A(S(_E)))
#define UPD _F5
#define SAVE C(_S)
#define PATH A(S(_C))
#define CWD C(A(S(_C)))
#define NENTRY A(_N)
#define PENTRY A(_P)

// ??? some substitutions for emacs standard keys
#define E_CX _F18
#define E_CC C(_F18)
#define E_CV C(S(_F18))
#define E_CZ C(A(S(_F18)))

// motion
#define POPMARK _F17
#define UNPOPMARK C(_F17) //:TD
#define FSEARCH C(_F)
#define BSEARCH C(S(_F))
#define FINDNXT _F3
#define FINDPRV S(_F3)
#define FINDAT C(S(_G))

#define BOW C(_LT)
#define FBOW C(_RT)
#define EOW A(_RT)
#define EOSW A(_RT)
#define BOSW A(_LT)
#define EOL KC_END
#define XEOL SS_TAP(X_END)
#define XBOL SS_TAP(X_HOME)
#define XBOW SS_DOWN(X_LCTL) SS_TAP(X_LEFT) SS_UP(X_LCTL)
#define XEOW SS_DOWN(X_LCTL) SS_TAP(X_RIGHT) SS_UP(X_LCTL)
#define BOL A(_M)
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
#define SWAPUP A(_UP)
#define SWAPDN A(_DN)
#define NLBELOW C(A(_4))

// programming
#define EVAL C(_ENT)
#define EVALF S(_ENT) // forward
#define EVALA _F12 // above
#define _DEBUG _F5
#define CLDEBUG S(_F5)
#define REDEBUG C(S(_F5))

// some keys
#define LABRC S(_COM)
#define RABRC S(_DOT)
#define QUE S(_SLS)
#define RUQUE S(_7)

