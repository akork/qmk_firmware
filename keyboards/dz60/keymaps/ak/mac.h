#define SPTLT G(_SPC)
#define SPTLT1 G(_SPC)
#define TERM G(_1)
#define KILLAPP G(_Q)
#define CANCEL _ESC
#define CANCEL2 C(_RBR)
// #define FSCR G(_UP)
#define FSCR C(_F18)
#define LSCR C(A(_F18))
#define RSCR C(A(S(_F18)))

#define SPLITRT C(_F19)
#define OTHER C(A(_F19))
#define DELOTHER C(A(S(_F19)))

#define FSEARCH G(_F)
#define BSEARCH G(S(_F))
#define FINDNXT _F3
#define FINDPRV S(_F3)
#define NENTRY A(_N)
#define PENTRY A(_P)
#define MX G(S(_P))
#define MCOLON A(S(_SCL))
#define RECENTF G(_P)
#define OPEN G(_O)
#define OPEN1 G(S(_O))
#define UPD _F5
#define SAVE G(_S)
#define LTAB C(S(_TAB))
#define RTAB C(_TAB)
#define PTAB C(_Q)
#define KILLTAB C(_W)
#define FILENAME A(S(_C))
#define FILEDIR C(A(S(_C)))
#define FILE A(S(_C))
#define FILEDIR C(A(S(_C)))

#define CUT G(_X)
#define COPY G(_C)
#define PASTE G(_V)
#define PASTEI G(S(_V))
#define ALL G(_A)
#define UNDO G(_Z)
#define REDO G(S(_Z))
#define FEOW A(_RT)
#define BBOW A(_LT)
#define EOSW A(_RT)
#define BOSW A(_LT)
#define EOL G(_RT)
#define BOL A(_M)
#define HARDBOL G(_LT)
#define _EOF KC_END
#define _BOF KC_HOME
#define DUPL G(S(_D))
#define COMMENT G(_SLS)
#define DELBOL C(S(_BSP))
#define DELEOL C(S(_DEL))
#define DELBOW C(_BSP)
#define DELEOW C(_DEL)
#define SWAPUP A(_UP)
#define SWAPDN A(_DN)

#define SWITCH KC_LGUI


#define LABRC S(_COM)
#define RABRC S(_DOT)
#define QUE S(_SLS)
#define RUQUE S(_7)

#define DEBUG _F5
#define CLDEBUG S(_F5)
#define REDEBUG C(S(_F5))
