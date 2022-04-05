#define SPTLT A(_SPC)
#define SPTLT1 KC_LWIN
#define TERM C(G(_1))
#define KILLAPP A(_F4)
#define CANCEL _ESC
#define CANCEL2 C(_RBR)

#define MAGIC _F19

#define FSCR G(_UP)
#define LSCR G(_LT)
#define RSCR G(_RT)

#define SPLITRT C(_F19)
#define OTHER C(A(_F19))
#define DELOTHER C(A(S(_F19)))

#define HELPKEY C(S(A(_H)))
#define FSEARCH C(_F)
#define BSEARCH C(S(_F))
#define FINDNXT _F3
#define FINDPRV S(_F3)
#define NENTRY A(_N)
#define PENTRY A(_P)
#define MX C(S(_P))
#define MCOLON A(S(_SCL))
#define RECENTF C(_P)
#define OPN C(_O)
#define OPN1 C(A(_O))
#define OPNHOME C(A(S(_O)))
#define OPNTR C(S(_E))
#define UPD _F5
#define SAVE C(_S)
#define LTAB C(S(_TAB))
#define RTAB C(_TAB)
#define PTAB C(_Q)
#define KILLTAB C(_W)
#define FILENAME A(S(_C))
#define FILEDIR C(A(S(_C)))

// navigation & editing 
#define CUT S(KC_DELETE)
#define COPY C(KC_INSERT)
#define PASTE S(KC_INSERT)
#define PASTEI C(S(_V))
#define ALL C(_A)
#define UNDO C(_Z)
#define REDO C(S(_Z))
#define EOW C(_RT)
#define BOW C(_LT)
#define EOSW A(_RT)
#define BOSW A(_LT)
#define EOL KC_END
#define XEOL SS_TAP(X_END)
#define XBOL SS_TAP(X_HOME)
#define BOL A(_M)
#define HARDBOL KC_HOME
#define _EOF C(KC_END)
#define _BOF C(KC_HOME)
#define DUPL C(S(_D))
#define COMMENT C(_SLS)
#define DELBOL C(S(_BSP))
#define DELEOL C(S(_DEL))
#define DELBOW C(_BSP)
#define DELEOW C(_DEL)
#define SWAPUP A(_UP)
#define SWAPDN A(_DN)
#define NLBELOW C(A(_4))

#define SWITCH KC_LALT


#define LABRC S(_COM)
#define RABRC S(_DOT)
#define QUE S(_SLS)
#define RUQUE S(_7)

#define EXECELL C(_ENT)
#define _DEBUG _F5
#define CLDEBUG S(_F5)
#define REDEBUG C(S(_F5))
