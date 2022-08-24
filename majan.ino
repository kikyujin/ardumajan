#include <Arduboy2.h>

// make an instance of arduboy used for many functions
Arduboy2 arduboy;

// my data type
typedef signed char s8;
typedef signed short s16;

#if 1
void __debug_print (const char* a)
{
  Serial.println(a);
}
#define DEB(a)    do { __debug_print(a); } while (0)
#else
#define DEB(a)
#endif


// 牌イメージ
#include "images.h"

// 牌データ
#define PAI_NONE      0
#define PAI_MANZI_S   1
#define PAI_MANZI_E   9
#define PAI_PINZI_S   10
#define PAI_PINZI_E   18
#define PAI_SOUZI_S   19
#define PAI_SOUZI_E   27
#define PAI_FENG_S    28
#define PAI_FENG_E    31
#define PAI_SANY_S    32
#define PAI_SANY_E    34
#define PAI_FLAG      0x80

#define PAI_MASK      0x3f      // 下位6bitを参照
#define PAI_KIND(p)   ((p)&PAI_MASK)

// それぞれ1~9指定です
#define MAN(x)      ((x)+PAI_MANZI_S-1)
#define PIN(x)      ((x)+PAI_PINZI_S-1)
#define SOU(x)      ((x)+PAI_SOUZI_S-1)

// 1~4
#define FENG(x)     ((x)+PAI_FENG-1)

// 1~3
#define SANY(x)     ((x)+PAI_SANY_S-1)

#define TEHAI_NUM     14
u8 g_tehai[TEHAI_NUM];

// 牌解析
#define PI_NONE   0          // 完全にゼロ
#define PI_SHUNT  0x0        // 上位２ビットがゼロ
#define PI_TOIT   0x40
#define PI_KOT    0x80
#define PI_KANT   0xc0
#define PI_MENT(p)    ((p)&0xc0)
#define PI_PAI(p)   ((p)&PAI_MASK)

u8 g_analyze[TEHAI_NUM];      // 解析中手牌

// is~系のメンツ判定は内部で牌の利用マークをする
// 対子判定
bool isToit (u8 here)
{
  // 対子か？
  if (here <= TEHAI_NUM - 2) {
    // 対子は必ず横並び
    if (PAI_KIND(g_analyze[here]) == PAI_KIND(g_analyze[here + 1])) {
      return true;
    }
  }
  return false;
}

// 刻子判定
bool isKot (u8 here)
{
  if (here <= TEHAI_NUM - 3) {
    u8 a = PAI_KIND(g_analyze[here]);
    if (a == PAI_KIND(g_analyze[here + 1]) && a == PAI_KIND(g_analyze[here + 2])) {
      return true;
    }
  }
  return false;
}

// 順子トップ牌判定
bool isShuntTop(u8 here)
{
  u8 a = PAI_KIND(g_analyze[here]);
  return (a >= PAI_MANZI_S && a <= PAI_MANZI_S + 7)       // PAI_MANZI_Sは0なので記述省略
      || (a >= PAI_PINZI_S && a <= PAI_PINZI_S + 7) 
      || (a >= PAI_SOUZI_S && a <= PAI_SOUZI_S + 7);
}

// 順子牌だったので組み合わせ入れ替え
void swapPaiForShunt(u8 p1, u8 p2, u8 p3)
{
  // 面倒だけど牌を入れ替える
  u8 b = g_analyze[p2];    // フラグ付きの全情報を保存
  u8 c = g_analyze[p3];    // フラグ付きの全情報を保存
  for (s8 i = p3 - 1; i > p2; i--) {
    g_analyze[i + 1] = g_analyze[i];
  }
  for (s8 i = p2 - 1; i > p1; i--) {
    g_analyze[i + 2] = g_analyze[i];
  }
  g_analyze[p1 + 1] = b;
  g_analyze[p1 + 2] = c;
}

void restorePaiForShunt(u8 p1, u8 p2, u8 p3)
{
  u8 b = g_analyze[p1 + 1];    // フラグ付きの全情報を保存
  u8 c = g_analyze[p1 + 2];    // フラグ付きの全情報を保存
  for (s8 i = p1 + 1; i < p2; i++) {
    g_analyze[i] = g_analyze[i + 2];
  }
  g_analyze[p2] = b;
  for (s8 i = p2 + 1; i < p3; i++) {
    g_analyze[i] = g_analyze[i + 1];
  }
  g_analyze[p3] = c;
}

void showTehai()
{
  for (s8 i = 0; i < TEHAI_NUM; i++) {
    char buff[4];
    sprintf(buff, "%02x,", g_analyze[i]);
    Serial.print(buff);
  }
  Serial.println();  
}

// １メンツ判定
#define PI_ANALYZE_MAX  64      // 最大バッファ(最大126=0x7e)
#define PI_LEAF   0x80          // 末端マーク
#define PI_ROOT   0xff          // ルートマーク

u8 g_pindex[PI_ANALYZE_MAX];    // 解析結果
u8 g_piRoot[PI_ANALYZE_MAX];    // 親ノード保持
u8 g_bToitChk;   // 対子チェック済みマーク
u8 g_piLast;    // g_pindexの末尾

bool recIndex (u8 root, u8 index)
{
  // 解析バッファオーバーフローチェック
  if (g_piLast == PI_ANALYZE_MAX) {
    DEB("a-b o/f");
    return false;
  }
  g_pindex[g_piLast] = index;
  g_piRoot[g_piLast] = root;
  g_piLast++;
  return true;
}

void initAnalyzeMent(const u8* pai = NULL)
{
  if (pai != NULL) {
    // コピー
    memcpy(g_analyze, pai, sizeof(g_analyze));
  }

  // 解析フラグクリア
  g_bToitChk = false;
  g_piLast = 0;
}


// 最大５段程度の再帰にしかならないので深度優先検索を採用します
// root g_pindexに対するインデックス(初期値PI_ROOT)
// now  g_analyzeに対する牌解析位置(初期値0)
bool analyzeOneMent (u8 root, u8 now)
{
  if (now >= TEHAI_NUM) {
    // １ルート探索終了
    // 末端マーク
    g_piRoot[root] |= PI_LEAF;
    return true;
  }

  // 残り１枚か？
  if (now == TEHAI_NUM - 1) {
    // もはやメンツは作れない
    return false;
  }

  // 対子か？（対子は１解析セットに雀頭１つしかない）
  bool res = false;
  if (!g_bToitChk && isToit(now)) {
    // インデックス記録
    if (!recIndex(root, PI_TOIT | PAI_KIND(g_analyze[now]))) {
      return false;
    }

    // 深度優先検索
    g_bToitChk = true;
    if (!analyzeOneMent(g_piLast - 1, now + 2)) {
      // このルートは無い
      g_piLast--;   // 記録を消す
    } else {
    // このルートで最後まで解析できた
      res = true;
    }
    // 探索を続ける
    g_bToitChk = false;
  }

  // 刻子か？
  if (isKot(now)) {
    // インデックス記録
    if (!recIndex(root, PI_KOT | PAI_KIND(g_analyze[now]))) {
      return false;
    }

    // 深度優先検索
    if (!analyzeOneMent(g_piLast - 1, now + 3)) {
      // このルートは無い
      g_piLast--;   // 記録を消す
    } else {
    // このルートで最後まで解析できた
      res = true;
    }
    // 探索を続ける
  }

  // 順子か？
  if (now >= TEHAI_NUM - 2) {
    // 対子でも刻子でもない２枚組
    return res;
  }


  // そもそもMIN/PIN/SOUの1～7でないと駄目
  if (!isShuntTop(now)) {
    return res;
  }

  // スタック節約のために分かりにくいループにしてあります
  // ２枚目を探す
  // 同じ数字は省く
  u8 p2, p3;
  for (p2 = now + 1;;) {
    u8 a = PAI_KIND(g_analyze[now]);
    u8 p = PAI_KIND(g_analyze[p2]);
    if (a == p) {
      if (++p2 >= TEHAI_NUM - 1) {
        // 残り牌が無い
        return res;
      }
      // else continue;
    } else {
      // a != p
      if (a + 1 == p) {
        break;    // 三枚目を探す
      } else {
        // 順子ではない
        return res;
      }
    }
  }

  // ３枚目を探す
  // 同じ数字は省く
  for (p3 = p2 + 1;;) {
    u8 a = PAI_KIND(g_analyze[p2]);
    u8 p = PAI_KIND(g_analyze[p3]);
    if (a == p) {
      if (++p3 >= TEHAI_NUM) {
        // 残り牌が無い
        return res;
      }
      // else continue;
    } else {
      // a != p
      if (a + 1 == p) {
        break;    // 順子だ
      } else {
        // 順子ではない
        return res;
      }
    }
  }

  // 順子だ
  // インデックス記録
  if (!recIndex(root, PI_SHUNT | PAI_KIND(g_analyze[now]))) {
    return false;
  }

  // 面倒だけど牌を入れ替える
 #if 0
  {
    char buff[16];
    sprintf(buff, "p1p2p3 %d,%d,%d", now, p2, p3);
    Serial.println(buff);
  }
  Serial.print("bf ");
  showTehai();
#endif
  swapPaiForShunt(now, p2, p3);
#if 0
  Serial.print("af ");
  showTehai();
#endif

  // 深度優先検索
  if (!analyzeOneMent(g_piLast - 1, now + 3)) {
    // このルートは無い
    g_piLast--;   // 記録を消す
  } else {
  // このルートで最後まで解析できた
    res = true;
  }

  // 牌を戻す
  restorePaiForShunt(now, p2, p3);
#if 0
  Serial.print("rs ");
  showTehai();
#endif

  return res;
}


// test
const unsigned char test_tehai[] = {
  MAN(1), MAN(1), MAN(1),
  MAN(2), MAN(2), MAN(2),
  MAN(3), MAN(3), MAN(3),
  MAN(4), MAN(4),
  MAN(5), MAN(5), MAN(5),
};

void printIndex (u8 k)
{
  char ch, n;

  switch (PI_MENT(k)) {
  case PI_TOIT:
    ch = 'T';
    break;
  case PI_KOT:
    ch = 'K';
    break;
  case PI_SHUNT:
    ch = 'S';
    break;
  default:
    ch = 'E';   // error
  }
  n = PI_PAI(k);

  char buff[6];
  sprintf(buff, "%c%d", ch, n);
  Serial.print(buff);
}

void traceTree (s8 i)
{
  u8 root = g_piRoot[i];
  if (root != PI_ROOT) {
    traceTree(root & 0x7f);
  }
  printIndex(g_pindex[i]);
}

void paiTest ()
{
  initAnalyzeMent(test_tehai);

  bool b = analyzeOneMent(PI_ROOT, 0);
  for (s8 i = 0; i < g_piLast; i++) {
    printIndex(g_pindex[i]);
  }  
  Serial.println();
  
  if (b) {
    s8 n = 0;
    for (s8 i = g_piLast - 1; i >= 0; i--) {
      if (g_piRoot[i] != PI_ROOT && g_piRoot[i] & PI_LEAF) {
        // 末端
        traceTree(i);
        Serial.println();
        n++;
      }
    }

    char buff[6];
    sprintf(buff, "%d", n);
    Serial.print(buff);
    Serial.println(F("pattern"));
  } else {
    Serial.println(F("no ten"));
  }
}






static int gcount = 0;

void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(10);

  // debug serial
  Serial.begin(57600);
  DEB("Good morning!");

  //	arduboy.clear();
  arduboy.fillRect(0, 0, 128, 64);
  for (int i = 0; i < 9; i++) {
  	arduboy.drawBitmap(i * 9, 0, img_back, 10, 15, BLACK);
  	arduboy.drawBitmap(i * 9 + 1, 2, &img_manzi[i * 16], 8, 11, BLACK);
  }
  for (int i = 0; i < 9; i++) {
  	arduboy.drawBitmap(i * 9, 14, img_back, 10, 15, BLACK);
  	arduboy.drawBitmap(i * 9 + 1, 16, &img_manzi[(i+9) * 16], 8, 11, BLACK);
  }
  for (int i = 0; i < 9; i++) {
  	arduboy.drawBitmap(i * 9, 28, img_back, 10, 15, BLACK);
  	arduboy.drawBitmap(i * 9 + 1, 30, &img_manzi[(i+18) * 16], 8, 11, BLACK);
  }

  for (int i = 0; i < 4; i++) {
  	arduboy.drawBitmap(128 - 36 + i * 9, 0, img_back, 10, 15, BLACK);
  	arduboy.drawBitmap(128 - 35 + i * 9, 2, &img_manzi[(i+27) * 16], 8, 11, BLACK);
  }
  for (int i = 0; i < 3; i++) {
  	arduboy.drawBitmap(128 - 36 + i * 9, 14, img_back, 10, 15, BLACK);
  	arduboy.drawBitmap(128 - 35 + i * 9, 16, &img_manzi[(i+31) * 16], 8, 11, BLACK);
  }



	arduboy.display();

  paiTest();
}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  gcount++;
  arduboy.setTextColor(BLACK);
  arduboy.setTextBackground(WHITE);
  arduboy.setCursor(0, 64-8);

  // then we print to screen what is in the Quotation marks ""
  arduboy.print(gcount);

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
