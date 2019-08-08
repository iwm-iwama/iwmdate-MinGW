//------------------------------------------------------
#define IWMDATEDIFF_VERSION "iwmdatediff_20170109"
#define IWMDATEDIFF_COPYRIGHT "(C)2008-2017 iwm-iwama"
//------------------------------------------------------
#include "lib_iwmutil.h"

INT *getAryTime(MBS *ymd,INT *localtime);
VOID version();
VOID help();

MBS *_program=0;

#define DATE_FORMAT "%g%y-%m-%d"

INT
main(){
	_program=iCmdline_getCmd();
	MBS **args=iCmdline_getArgs();
	MBS **ap1={0};

	// help
	//  �I�v�V�����Ȃ��̂Ƃ��\��
	//  "-h" <= hour �Ȃ̂Ŏg��Ȃ�
	if(!**(args+0)){
		help();
		imain_end();
	}

	// version
	ap1=iargs_option(args,"-version","-v");
		if($IWM_bSuccess){
			version();
			LN();
			imain_end();
		}
	ifree(ap1);

	INT *localtime=idate_now_to_iAryYmdhns_localtime();

	// ymd1
	MBS *_ymd1=*(args+0);
	INT *ai1=getAryTime(_ymd1,localtime);

	// ymd2
	MBS *_ymd2=*(args+1);
	INT *ai2=getAryTime(_ymd2,localtime);

	// diff[8]
	INT *iAryDiff=idate_diff(*(ai1+0),*(ai1+1),*(ai1+2),*(ai1+3),*(ai1+4),*(ai1+5),*(ai2+0),*(ai2+1),*(ai2+2),*(ai2+3),*(ai2+4),*(ai2+5));

	// -format
	MBS *_format=0;
	ap1=iargs_option(args,"-format","-f");
		_format=($IWM_bSuccess ?
			ims_clone(*ap1) :
			DATE_FORMAT
		);
	ifree(ap1);

	P("%s",idate_format_diff(_format,iAryDiff[0],iAryDiff[1],iAryDiff[2],iAryDiff[3],iAryDiff[4],iAryDiff[5],iAryDiff[6],iAryDiff[7]));

	// -N ���s���Ȃ�
	ap1=iargs_option(args,"-N",NULL);
		if(!$IWM_bSuccess){
			NL();
		}
	ifree(ap1);

	imain_end();
}

INT
*getAryTime(
	MBS *ymd,
	INT *localtime
){
	INT *rtn=0;
	if(imb_cmppi(ymd,"now") || *ymd=='.'){
		rtn=icalloc_INT(6);
		INT i1=0;
		for(i1=0;i1<6;i1++) *(rtn+i1)=*(localtime+i1);
	}
	else{
		rtn=idate_MBS_to_iAryYmdhns(ymd);
	}
	return rtn;
}

VOID
version(){
	LN();
	P2(IWMDATEDIFF_COPYRIGHT);
	P ("  Ver.%s+%s\n",IWMDATEDIFF_VERSION,LIB_IWMUTIL_VERSION);
}

VOID
help(){
	version();
	LN();
	P2("���g�p�@��");
	P ("  %s [���t1] [���t2] [�I�v�V����] \n",_program);
	NL();
	P2("�����t��");
	P2("  \".\" \"now\" => ���ݓ���");
	P2("  \"2008/12/10\" \"2008-12-10\"");
	NL();
	P2("���I�v�V������");
	P2("  -format | -f STR");
	P ("      ��STR�����w��̂Ƃ��A\"%s\"�Ɠ��`\n",DATE_FORMAT);
	P2("      %g:+/-�\\��");
	P2("      %y:�N  %m:��  %d:��  %h:��  %n:��  %s:�b");
	P2("      �ʎZ(��)  %M:��  %D:��  %H:��  %N:��  %S:�b  %W:�T  %w:�T�]��");
	P2("      \\t:�^�u  \\n:���s");
	P2("  -N");
	P2("      ���s���Ȃ�.");
	NL();
	P2("���g�p�၄");
	P ("  %s 2000/1/1 2008/12/10 -f \"%%D�ʎZ�� %%W�ʎZ�T%%w��\"\n",_program);
	NL();
	P2("�����l��");
	P2("  �����E�X��i-4712/01/01�`1582/10/04�j�A");
	P2("  �O���S���I��i1582/10/15�`9999/12/31�j�ɑΉ��B");
	P2("  (��1) �󔒗�i1582/10/5�`1582/10/14�j�́A\"1582/10/4\" �Ƃ��Ď戵���B");
	P2("  (��2) BC��́A\"-1/1/1\" �� \"0/1/1\" �Ƃ��Ď戵���B");
	P2("  (��3) �v���O������́A\"JD�ʓ��łȂ�CJD�ʓ�\" ���g�p�B");
	LN();
}
