//------------------------------------------------------------------------------
#define   IWM_VERSION         "iwmdatediff_20200531"
#define   IWM_COPYRIGHT       "Copyright (C)2008-2020 iwm-iwama"
//------------------------------------------------------------------------------
#include "lib_iwmutil.h"

INT  main();
VOID print_version();
VOID print_help();

// [�����F] + ([�w�i�F] * 16)
//  0 = Black    1 = Navy     2 = Green    3 = Teal
//  4 = Maroon   5 = Purple   6 = Olive    7 = Silver
//  8 = Gray     9 = Blue    10 = Lime    11 = Aqua
// 12 = Red     13 = Fuchsia 14 = Yellow  15 = White
#define   ColorHeaderFooter   ( 7 + ( 0 * 16))
#define   ColorBgText1        (15 + (12 * 16))
#define   ColorExp1           (13 + ( 0 * 16))
#define   ColorExp2           (14 + ( 0 * 16))
#define   ColorExp3           (11 + ( 0 * 16))
#define   ColorText1          (15 + ( 0 * 16))

#define   DATE_FORMAT         "%g%y-%m-%d" // (��)%g�t���Ȃ��ƑS�Đ����\��

/*
	�o�̓t�H�[�}�b�g
	-f=STR | -format=STR
*/
MBS  *_Format = DATE_FORMAT;
/*
	���s����Ƃ� TRUE
	-N
*/
BOOL _NL = TRUE;
/*
	���s�֌W
*/
MBS  *$program     = "";
MBS  **$args       = {0};
UINT $argsSize     = 0;
UINT $colorDefault = 0;

INT
main()
{
	$program      = iCmdline_getCmd();
	$args         = iCmdline_getArgs();
	$argsSize     = iary_size($args);
	$colorDefault = iConsole_getBgcolor(); // ���݂̕����F�^�w�i�F

	// -help "-h"��hour
	if($argsSize == 0 || imb_cmpp($args[0], "-help"))
	{
		print_help();
		imain_end();
	}

	// -v | -version
	if(imb_cmpp($args[0], "-v") || imb_cmpp($args[0], "-version"))
	{
		print_version();
		LN();
		imain_end();
	}

	INT  *iAryDtBgn = icalloc_INT(6); // y, m, d, h, n, s
	INT  *iAryDtEnd = icalloc_INT(6); // y, m, d, h, n, s

	// [0] . | now
	iAryDtBgn = (imb_cmppi($args[0], ".") || imb_cmpp($args[0], "now") ?
		idate_now_to_iAryYmdhns_localtime() :
		idate_MBS_to_iAryYmdhns($args[0])
	);

	// [1] . | now
	iAryDtEnd = (imb_cmppi($args[1], ".") || imb_cmpp($args[1], "now") ?
		idate_now_to_iAryYmdhns_localtime() :
		idate_MBS_to_iAryYmdhns($args[1])
	);

	// [2..]
	for(INT _i1 = 2; _i1 < $argsSize; _i1++)
	{
		MBS **_as1 = ija_split($args[_i1], "=", "\"\"\'\'", FALSE);
		MBS **_as2 = ija_split(_as1[1], ",", "\"\"\'\'", TRUE);

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

	// diff[8]
	INT *iAryDiff=idate_diff(
		iAryDtBgn[0], iAryDtBgn[1], iAryDtBgn[2], iAryDtBgn[3], iAryDtBgn[4], iAryDtBgn[5],
		iAryDtEnd[0], iAryDtEnd[1], iAryDtEnd[2], iAryDtEnd[3], iAryDtEnd[4], iAryDtEnd[5]
	);

	P(
		idate_format_diff(
			_Format,
			iAryDiff[0], iAryDiff[1], iAryDiff[2], iAryDiff[3], iAryDiff[4], iAryDiff[5], iAryDiff[6], iAryDiff[7]
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
	LN();
	P (" %s\n",
		IWM_COPYRIGHT
	);
	P ("   Ver.%s+%s\n",
		IWM_VERSION,
		LIB_IWMUTIL_VERSION
	);
}

VOID
print_help()
{
	iConsole_setTextColor(ColorHeaderFooter);
		print_version();
		LN();
	iConsole_setTextColor(ColorBgText1);
		P (" %s [���t1] [���t2] [�I�v�V����] \n\n", $program);
	iConsole_setTextColor(ColorExp1);
		P2(" (�g�p��)");
	iConsole_setTextColor(ColorText1);
		P ("   %s . 2000/01/01 -f=\"%%g%%y-%%m-%%d %%h:%%n:%%s\"\n\n", $program);
	iConsole_setTextColor(ColorExp2);
		P2(" [���t1] [���t2]");
	iConsole_setTextColor(ColorText1);
		P2("   \".\" \"now\" (���ݓ���)");
		P2("   \"+2000/01/01\" \"+2000-01-01\"");
		P2("   \"+2000/01/01 00:00:00\" \"+2000-01-01 00:00:00\"");
		NL();
	iConsole_setTextColor(ColorExp2);
		P2(" [�I�v�V����]");
	iConsole_setTextColor(ColorExp3);
		P2("   -format=STR | -f=STR");
	iConsole_setTextColor(ColorText1);
		P ("       ��STR�����w��̂Ƃ� \"%s\"\n", DATE_FORMAT);
		P2("       %g:+/-�\\��");
		P2("       %y:�N  %m:��  %d:��  %h:��  %n:��  %s:�b");
		P2("       �ʎZ(��)  %M:��  %D:��  %H:��  %N:��  %S:�b  %W:�T  %w:�T�]��");
		P2("       \\t:�^�u  \\n:���s");
	iConsole_setTextColor(ColorExp3);
		P2("   -N");
	iConsole_setTextColor(ColorText1);
		P2("       ���s���Ȃ�");
		NL();
	iConsole_setTextColor(ColorExp1);
		P2(" (���l)");
	iConsole_setTextColor(ColorText1);
		P2("   �E�����E�X�� �i-4712/01/01�`1582/10/04�j");
		P2("   �E�O���S���I��i1582/10/15�`9999/12/31�j");
		P2("    (��1) �󔒗� 1582/10/5�`1582/10/14 �́A\"1582/10/4\" �Ƃ��Ď戵���B");
		P2("    (��2) BC��́A\"-1/1/1\" �� \"0/1/1\" �Ƃ��Ď戵���B");
		P2("    (��3) �v���O������́A\"JD�ʓ��łȂ�CJD�ʓ�\" ���g�p�B");
	iConsole_setTextColor(ColorHeaderFooter);
		LN();
	iConsole_setTextColor($colorDefault); // ���̕����F�^�w�i�F
}
