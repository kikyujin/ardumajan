from asyncio import SendfileNotAvailableError
import re
import random, string

def deb_print(s):
#    print(s)
    True

def list_to_str(a):
    s = ''
    for i in range(len(a)):
        s = s + a[i]
    return s


def pailex(lex):
    st = []
    ich = 0
    ln = len(lex)
    while ich < ln:
        ch = lex[ich]
        ich = ich + 1       # 次に進める
        deb_print("ch = '" + ch + "', len(st)" + str(len(st)))
        if ch == '(':
            # メンツ定義
            print("if (")
        elif ch == ')':
            # メンツ定義終わり
            deb_print("= len(st)" + str(len(st)))
            if len(st) >= 1:
                print(st[0])
                i = 1
                while i < len(st):
                    print('&&' + st[i])
                    i = i + 1

            print(") { return LXTRUE; }")
            st = []
        elif ch == '[':
            # 先読み
            if lex[ich].isdecimal():
                # '['の後は数字だ
                ch = lex[ich]       # 最初の数字を保持
                ich = ich + 1
                if lex[ich] == '-':
                    # 範囲指定
                    ich = ich + 1
                    st.append('lxIsNumArea(' + ch + ',' + lex[ich] + ')')
                    deb_print('= lxIsNumArea(' + ch + ',' + lex[ich] + ')')
                    ich = ich + 2       # ']'を飛ばす
                else:
                    # 数字？
                    t = '(lxIsNum(' + ch + ')'
                    while lex[ich].isdecimal():
                        t += '||lxIsNum(' + lex[ich] + ')'
                        ich = ich + 1
                    t += ')'
                    st.append(t)
                    ich = ich + 1       # ']'を飛ばす
            else:
                #「どれか」
                orp = len(st)
                st.append("(")
                deb_print('= (, len(st)' + str(len(st)))
        elif ch == ']':
            deb_print("enum end orp = " + str(orp) + "len(st) = " + str(len(st)))
            j = orp
            while j < len(st):
                deb_print('j = ' + str(j) + st[j])
                j = j + 1
            #「どれか」終わり
            # 要素列挙してor
            st[orp] += st[orp + 1]
            j =  orp + 2
            while j < len(st):
                st[orp] += '||' + st[j]
                j = j + 1
            st[orp] += ')'
            del st[orp + 1:len(st)]
        elif ch == '!':
            # 面前（現状無視）
            False
        elif ch == '.' or ch == '|':
            # 任意の条件
            False
        elif ch == 'T' or ch == 't':
            st.append('lxIsToit()')
            deb_print ('= lxIsToit(), len(st)' + str(len(st)))
        elif ch == 'K' or ch == 'k':
            st.append('lxIsKot()')
            deb_print ('= lxIsKot(), len(st)' + str(len(st)))
        elif ch == 'S' or ch == 's':
            st.append('lxIsShunt()')
            deb_print ('= lxIsShunt(), len(st)' + str(len(st)))
        elif ch == 'F':
            st.append('lxIsFeng()')
            deb_print ('= lxIsFeng(), len(st)' + str(len(st)))
        elif ch == 'H':
            st.append('lxIsSany()')
            deb_print ('= lxIsSany(), len(st)' + str(len(st)))
        elif ch == 'M':
            st.append('lxIsManz()')
            deb_print ('= lxIsManz(), len(st)' + str(len(st)))
        elif ch == 'P':
            st.append('lxIsPinz()')
            deb_print ('= lxIsPinz(), len(st)' + str(len(st)))
        elif ch == 'Z':
            st.append('lxIsSouz()')
            deb_print ('= lxIsSouz(), len(st)' + str(len(st)))
        elif ch == 'x':
            st.append('lxIsFengNum()')
            deb_print ('= lxIsFengNum(), len(st)' + str(len(st)))
        elif ch == 'y':
            st.append('!lxIsFengNum()')
            deb_print ('= !lxIsFengNum(), len(st)' + str(len(st)))
        elif ch == 'r':
            # 内部記録メンツの参照
            # 先読み
            ch = lex[ich]
            ich = ich + 1       # 次に進める
            if ch == 'k':
                st.append('lxIsRecMent()')
                deb_print ('= lxIsRecMent(), len(st)' + str(len(st)))
            elif ch == 'p':
                st.append('lxIsRecPait()')
                deb_print ('= lxIsRecPait(), len(st)' + str(len(st)))
            elif ch == 'n':
                st.append('lxIsRecNum()')
                deb_print ('= lxIsRecNum(), len(st)' + str(len(st)))
            else:
                deb_print ('r?')
                return False
        else:
            deb_print (ch + '?')
            return False

    print("return LXFALSE;")
    return True


define_table = {}

def fnDefine(s):
    # (define) LABEL pppppp
    if len(s) < 2:
        return False

    define_table[s[0]] = list_to_str(s[1:])
    return True

def randomDefine(s):
    # 適当な名前でメンツ定義をする
    nm = ''.join(random.choices(string.ascii_letters, k=6))
    while nm in define_table.keys():
        nm = ''.join(random.choices(string.ascii_letters, k=6))
    define_table[nm] = s
    return nm

class Yaku:
    def __init__(self, name, han):
        self.name = name
        self.han = han
        self.cmd = []

yaku_table = []

def fnYaku(s):
    # (yaku) LABEL han 'cmd pppp ['cmd ppppp]
    if len(s) < 4:
        return False

    y = Yaku(s[0], s[1])
    yaku_table.append(y)

    i = 2
    while i < len(s):
        ch = s[i][0]
        if ch == "'":
            # 頭が'ならコマンド
            y.cmd.append(s[i])
        elif ch.isalpha():
            # アルファベット列はシンボル
            y.cmd.append(s[i])
        elif ch == '(':
            # メンツ定義
            y.cmd.append(randomDefine(s[i]))
        else:
            # エラー
            return False
        i = i + 1

    return True

def subparse(cmd, lv):
    c = cmd[0]
    if c == "'all":
        # 全部のメンツが条件と一致する
        print("for (; YM_NOT_END(p%s); YM_GETNEXT(p%s)) {" % (lv, lv))
        print("  YM_PREPARE(p%s);" % (lv))
        print("  if (!lx_is" + cmd[1] + "()) { return 0; }");
        print("}")
        print("res = 1;")
    elif c == "'count":
        # 条件にあうメンツをカウントする
        print("for (; YM_NOT_END(p%s); YM_GETNEXT(p%s)) {" % (lv,lv))
        print("  YM_PREPARE(p%s);" %(lv))
        print("  if (lx_is" + y.cmd[1] + "()) { han++; }");
        print("}")
        print("res = 1;")
    elif c == "'mrep":
        # 'mrep ment cond
        print("res = 1;")
        print("for (; YM_NOT_END(p%s); YM_GETNEXT(p%s)) {" % (lv,lv))
        # 1. mentにマッチしたものをメモリー
        print("  YM_PREPARE(p%s);" %(lv))
        print("  if (lx_is" + y.cmd[1] + "()) {")
        print("    YM_RECORD(p%s);" %(lv))
        # 2. 当該以降のメンツでcmdをリピート
        print("    YM p%s = YM_GETNEXT(p%s);" %(lv+1, lv))
        if subparse(cmd[2:], lv+1) == False:
            return False
        print("    if (res != 0) { break; }")
        print("  }")
        # cmdがn(!=0) で n / リピート終了で 0
        print("}")
    elif c == "'eq":
        print("for (; YM_NOT_END(p%s); YM_GETNEXT(p%s)) {" % (lv,lv))
        print("  YM_PREPARE(p%s);" %(lv))
        print("  if (lx_is" + y.cmd[1] + ") { res = 1; break; }" )
        print("}")

    return True


def yakuParse(y):
    print("YMINT yak_is" + y.name);
    print("() {")
    if y.han == 'n':
        print("YMINT han = 0;")
    else:
        print("YMINT han = " + y.han + ";")

    print("YMINT res = 1;")
    print("YM p0 = YM_GETFIRST();")
    if subparse(y.cmd, 0) == False:
        return False

    print("if (res == 0) { return 0; }")
    print("return han; }")
    return True

res = True
while res:
    try:
        s = input()
        s = re.sub('#.*$', '', s).split()   # コメント以降を削除してセパレート
        deb_print(s)
        if s==[]: continue
        if s[0] == 'define':
            res = fnDefine(s[1:])
        elif s[0] == 'yaku':
            res = fnYaku(s[1:])

    except EOFError:
        break

if res == False:
    print("define/yaku Error")
    exit()

print("#ifdef YKDEBUG")
print('#include "yakutest.h"')
print("#endif")

for i,j in enumerate(define_table):
    print("// " + j + " " + define_table[j])
    print("LXBOOL lx_is" + j);
    print("() {")
    if pailex(define_table[j]) == False:
        print("define Error")
        exit()
    print("}")

for i in range(len(yaku_table)):
    y = yaku_table[i]
    print("// " + y.name + " " + y.han  + " " + y.cmd[0] + " " + y.cmd[1] + " ...")
    if yakuParse(y) == False:
        print("yaku Error")
        exit()

print('BEGIN_YAKU_TABLE')
for i in range(len(yaku_table)):
    print('YAKU_TABLE(%s, %s)' % (i, 'yak_is'+yaku_table[i].name))
print('END_YAKU_TABLE')
