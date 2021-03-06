//------------------------------------------------------------------------------
#define   IWM_VERSION         "iwmdateadd_20210603"
#define   IWM_COPYRIGHT       "Copyright (C)2008-2021 iwm-iwama"
//------------------------------------------------------------------------------
#include "lib_iwmutil.h"

INT  main();
VOID print_version();
VOID print_help();

// [文字色] + ([背景色] * 16)
//  0 = Black    1 = Navy     2 = Green    3 = Teal
//  4 = Maroon   5 = Purple   6 = Olive    7 = Silver
//  8 = Gray     9 = Blue    10 = Lime    11 = Aqua
// 12 = Red     13 = Fuchsia 14 = Yellow  15 = White

// タイトル
#define   COLOR01             (15 + ( 9 * 16))
// 入力例／注
#define   COLOR11             (15 + (12 * 16))
#define   COLOR12             (13 + ( 0 * 16))
#define   COLOR13             (12 + ( 0 * 16))
// 引数
#define   COLOR21             (14 + ( 0 * 16))
#define   COLOR22             (11 + ( 0 * 16))
// 説明
#define   COLOR91             (15 + ( 0 * 16))
#define   COLOR92             ( 7 + ( 0 * 16))

#define   DATE_FORMAT         "%g%y-%m-%d" // (注)%g付けないと全て正数表示

/*
	出力フォーマット
	-f=STR | -format=STR
*/
MBS  *_Format = DATE_FORMAT;
/*
	改行するとき TRUE
	-N
*/
BOOL _NL = TRUE;

INT
main()
{
	// lib_iwmutil 初期化
	iCLI_getCMD();       //=> $IWM_CMD
	iCLI_getARGS();      //=> $IWM_ARGV, $IWM_ARGC
	iConsole_getColor(); //=> $IWM_ColorDefault, $IWM_StdoutHandle
	iExecSec_init();     //=> $IWM_ExecSecBgn

	// -h | -help
	if(! $IWM_ARGC || imb_cmpp($IWM_ARGV[0], "-h") || imb_cmpp($IWM_ARGV[0], "-help"))
	{
		print_help();
		imain_end();
	}

	// -v | -version
	if(imb_cmpp($IWM_ARGV[0], "-v") || imb_cmpp($IWM_ARGV[0], "-version"))
	{
		print_version();
		imain_end();
	}

	INT *iAryDt    = icalloc_INT(6); // y, m, d, h, n, s
	INT *iAryDtAdd = icalloc_INT(6); // ±y, ±m, ±d, ±h, ±n, ±s

	// [0]
	/*
		"." "now" => 現在時
	*/
	if(imb_cmpp($IWM_ARGV[0], ".") || imb_cmpp($IWM_ARGV[0], "now"))
	{
		iAryDt = idate_now_to_iAryYmdhns_localtime();
	}
	else
	{
		iAryDt = idate_MBS_to_iAryYmdhns($IWM_ARGV[0]);
	}

	// [1..]
	for(INT _i1 = 1; _i1 < $IWM_ARGC; _i1++)
	{
		MBS **_as1 = ija_split($IWM_ARGV[_i1], "=", "\"\"\'\'", FALSE);
		MBS **_as2 = ija_split(_as1[1], ",", "\"\"\'\'", TRUE);

		// -y
		if(imb_cmpp(_as1[0], "-y"))
		{
			iAryDtAdd[0] += inum_atoi(_as2[0]);
		}

		// -m
		if(imb_cmpp(_as1[0], "-m"))
		{
			iAryDtAdd[1] += inum_atoi(_as2[0]);
		}

		// -d
		if(imb_cmpp(_as1[0], "-d"))
		{
			iAryDtAdd[2] += inum_atoi(_as2[0]);
		}

		// -w
		if(imb_cmpp(_as1[0], "-w"))
		{
			iAryDtAdd[2] += inum_atoi(_as2[0]) * 7;
		}

		// -h
		if(imb_cmpp(_as1[0], "-h"))
		{
			iAryDtAdd[3] += inum_atoi(_as2[0]);
		}

		// -n
		if(imb_cmpp(_as1[0], "-n"))
		{
			iAryDtAdd[4] += inum_atoi(_as2[0]);
		}

		// -s
		if(imb_cmpp(_as1[0], "-s"))
		{
			iAryDtAdd[5] += inum_atoi(_as2[0]);
		}

		// -f | -format
		if(imb_cmpp(_as1[0], "-f") || imb_cmpp(_as1[0], "-format"))
		{
			_Format = ims_clone(_as2[0]);
		}

		// -N
		if(imb_cmpp(_as1[0], "-N"))
		{
			_NL = FALSE;
		}

		ifree(_as2);
		ifree(_as1);
	}

	iAryDt = idate_add(
		iAryDt[0], iAryDt[1], iAryDt[2], iAryDt[3], iAryDt[4], iAryDt[5],
		iAryDtAdd[0], iAryDtAdd[1], iAryDtAdd[2], iAryDtAdd[3], iAryDtAdd[4], iAryDtAdd[5]
	);

	P(
		idate_format_ymdhns(
			_Format,
			iAryDt[0], iAryDt[1], iAryDt[2], iAryDt[3], iAryDt[4], iAryDt[5]
		)
	);

	if(_NL)
	{
		NL();
	}

	// Debug
	/// icalloc_mapPrint(); ifree_all(); icalloc_mapPrint();

	imain_end();
}

VOID
print_version()
{
	PZ(COLOR92, NULL);
	LN();
	P(" %s\n", IWM_COPYRIGHT);
	P("   Ver.%s+%s\n", IWM_VERSION, LIB_IWMUTIL_VERSION);
	LN();
	PZ(-1, NULL);
}

VOID
print_help()
{
	print_version();
	PZ(COLOR01, " 日時の前後を計算 \n\n");
	PZ(COLOR11, " %s [日付] [オプション] \n\n", $IWM_CMD);
	PZ(COLOR12, " (使用例)\n");
	PZ(COLOR91, "   %s \"2000/1/1\" -y=8 -m=11 -d=9 -f=\"%%g%%y-%%m-%%d(%%a) %%h:%%n:%%s\"\n\n", $IWM_CMD);
	PZ(COLOR21, " [日付]\n");
	PZ(COLOR91, NULL);
	P2("   \"now\" \".\" (現在日時)");
	P2("   \"+2000/01/01\" \"+2000-01-01\"");
	P2("   \"+2000/01/01 00:00:00\" \"+2000-01-01 00:00:00\"\n");
	PZ(COLOR21, " [オプション]\n");
	PZ(COLOR22, NULL);
	P2("   -y=[±年] -m=[±月] -d=[±日] -w=[±週] -h=[±時] -n=[±分] -s=[±秒]\n");
	P2("   -format=STR | -f=STR");
	PZ(COLOR91, NULL);
	P("       ※STRが無指定のとき \"%s\"\n", DATE_FORMAT);
	P2("       %g : +/-表\示");
	P2("       %y : 年(0000)  %m : 月(00)  %d : 日(00)");
	P2("       %h : 時(00)  %n : 分(00)  %s : 秒(00)");
	P2("       %a : 曜日  %A : 曜日数");
	P2("       %c : 年通算日  %C : 修正ユリウス通算日  %J : ユリウス通算日");
	P2("       %e : 年通算週");
	P2("       \\t : タブ  \\n : 改行");
	PZ(COLOR22, "   -N\n");
	PZ(COLOR91, "       改行しない\n\n");
	PZ(COLOR12, " (備考)\n");
	PZ(COLOR91, NULL);
	P2("   ・ユリウス暦 （-4712/01/01〜1582/10/04）");
	P2("   ・グレゴリオ暦（1582/10/15〜9999/12/31）");
	PZ(COLOR13, "    (注１) ");
	PZ(COLOR91, "空白暦 1582/10/5〜1582/10/14 は、\"1582/10/4\" として取扱う。\n");
	PZ(COLOR13, "    (注２) ");
	PZ(COLOR91, "BC暦は、\"-1/1/1\" を \"0/1/1\" として取扱う。\n");
	PZ(COLOR13, "    (注３) ");
	PZ(COLOR91, "プログラム上は、修正ユリウス暦を使用。\n\n");
	PZ(COLOR92, NULL);
	LN();
	PZ(-1, NULL);
}
