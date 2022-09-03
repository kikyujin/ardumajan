#コメント
define ANY ([!t!k!s]..)
define KYH (KFx)|(KH.)
define CHU ([TK][MPZ][2-8])|(S.[2-6])

# 1. メンゼンチンツモ (門前清自摸和)
yaku TSUMO	1	'all ANY		

# 2. ヤクハイ (役牌)
yaku YAKUHAI	n	'count KYH

# 3. タンヤオ (断么九)
yaku TANYAO		1	'all CHU

# 4. ピンフ (平和)
yaku PINHU		1	'all (T[MPZ].)|(TFy)|(S..)	#ただし両面待ち

# 5. イーペーコー (一盃口)
yaku IPEIKO 1	'mrep (!s..) 'eq (rkrprn)

# 6. サンショクドウジュン (三色同順)
yaku SANDOU 2   'mrep (SM.) 'mrep (SPrn) 'eq (SZrn)

