typedef enum {LXFALSE=0, LXTRUE} LXBOOL;
typedef signed char YMINT;
typedef signed char YM;

#define BEGIN_YAKU_TABLE YMINT (*ymfuc[])() = {
#define YAKU_TABLE(a,b)  { b },
#define END_YAKU_TABLE };
