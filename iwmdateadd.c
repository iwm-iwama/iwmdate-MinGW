//-----------------------------------------------------
#define IWMDATEADD_VERSION   "iwmdateadd_20170109"
#define IWMDATEADD_COPYRIGHT "(C)2008-2017 iwm-iwama"
//-----------------------------------------------------
#include "lib_iwmutil.h"

VOID version();
VOID help();

MBS *_program=0;
MBS *_option[6]={"-y","-m","-d","-h","-n","-s"};

#define DATE_FORMAT "%g%y-%m-%d" // (��)%g�t���Ȃ��ƑS�Đ����\��

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

	INT i1=0;

	// _ymd
	INT *iAryAdd=0;// y,m,d,h,n,s
	MBS *_ymd=*(args);
		iAryAdd=(imb_cmppi(_ymd,"now") || *_ymd=='.' ?
			idate_now_to_iAryYmdhns_localtime() :
			idate_MBS_to_iAryYmdhns(_ymd)
		);

	// �}����
	INT *iAryAddCnt=icalloc_INT(6);// �}y,�}m,�}d,�}h,�}n,�}s
	for(i1=0;i1<6;i1++){
		ap1=iargs_option(args,*(_option+i1),NULL);
			if($IWM_bSuccess){
				*(iAryAddCnt+i1)=atoi(*ap1);
			}
		ifree(ap1);
	}
	ap1=iargs_option(args,"-w",NULL);
		if($IWM_bSuccess){
			*(iAryAddCnt+2)+=(atoi(*ap1)*7);
		}
	ifree(ap1);

	// add
	iAryAdd=idate_add(
		*(iAryAdd+0),*(iAryAdd+1),*(iAryAdd+2),*(iAryAdd+3),*(iAryAdd+4),*(iAryAdd+5),
		*(iAryAddCnt+0),*(iAryAddCnt+1),*(iAryAddCnt+2),*(iAryAddCnt+3),*(iAryAddCnt+4),*(iAryAddCnt+5)
	);

	// -format
	MBS *_format=0;
	ap1=iargs_option(args,"-format","-f");
		_format=($IWM_bSuccess ?
			ims_clone(*ap1) :
			DATE_FORMAT
		);
	ifree(ap1);

	// �\��
	P(
		idate_format_ymdhns(
			_format,
			*(iAryAdd+0),*(iAryAdd+1),*(iAryAdd+2),*(iAryAdd+3),*(iAryAdd+4),*(iAryAdd+5)
		)
	);

	// -N ���s���Ȃ�
	ap1=iargs_option(args,"-N",NULL);
		if(!$IWM_bSuccess){
			NL();
		}
	ifree(ap1);

	imain_end();
}

VOID
version(){
	LN();
	P2(IWMDATEADD_COPYRIGHT);
	P ("  Ver.%s+%s\n",IWMDATEADD_VERSION,LIB_IWMUTIL_VERSION);
}

VOID
help(){
	version();
	LN();
	P2("���g�p�@��");
	P ("  %s [���t] [�I�v�V����]\n",_program);
	NL();
	P2("�����t��");
	P2("  \".\" \"now\" => ���ݓ���");
	P2("  \"+2008/12/10\" \"+2008-12-10\"");
	P2("  \"+2008/12/10 00:00:00\" \"+2008-12-10 00:00:00\"");
	NL();
	P2("���I�v�V������");
	P2("  -y [�}�N] -m [�}��] -d [�}��] -w [�}�T] -h [�}��] -n [�}��] -s [�}�b]");
	NL();
	P2("  -format | -f STR");
	P ("      ��STR�����w��̂Ƃ��A\"%s\"�Ɠ��`\n",DATE_FORMAT);
	P2("      %g:+/-�\\��");
	P2("      %y:�N(0000)  %Y:�N(��2��)  %m:��(00)  %d:��(00)");
	P2("      %h:��(00)  %n:��(00)  %s:�b(00)");
	P2("      %a:�j��  %A:�j����");
	P2("      %c:�N�ʎZ��  %C:CJD�ʎZ��");
	P2("      %e:�N�ʎZ�T  %E:CJD�ʎZ�T");
	P2("      \\t:�^�u  \\n:���s");
	P2("  -N");
	P2("      ���s���Ȃ�.");
	NL();
	P2("���g�p�၄");
	P ("  %s 2000/1/1 -y 8 -m 11 -d 9 -f \"%%g%%y-%%m-%%d(%%a) %%h:%%n:%%s\"\n",_program);
	NL();
	P2("�����l��");
	P2("  �����E�X��i-4712/01/01�`1582/10/04�j�A");
	P2("  �O���S���I��i1582/10/15�`9999/12/31�j�ɑΉ��B");
	P2("  (��1) �󔒗�i1582/10/5�`1582/10/14�j�́A\"1582/10/4\" �Ƃ��Ď戵���B");
	P2("  (��2) BC��́A\"-1/1/1\" �� \"0/1/1\" �Ƃ��Ď戵���B");
	P2("  (��3) �v���O������́A\"JD�ʓ��łȂ�CJD�ʓ�\" ���g�p�B");
	LN();
}
