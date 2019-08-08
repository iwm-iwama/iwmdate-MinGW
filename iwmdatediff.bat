:: Ini ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	@echo off
	cls

	:: �t�@�C�����̓\�[�X�Ɠ���
	set fn=%~n0
	set exec=%fn%.exe
	set op_link=-O2 -lgdi32 -luser32 -lshlwapi
	set src=%fn%.c
	set lib=lib_iwmutil.a

:: Make ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	echo --- Compile -S --------------------------------------
	for %%s in (%src%) do (
		gcc %%s -S
		echo %%~ns.s
	)
	echo.

	echo --- Make -----------------------------------------
	for %%s in (%src%) do (
		gcc %%s -c -Wall %op_link%
	)
	gcc *.o %lib% -o %exec% %op_link%
	echo %exec%

	:: �㏈��
	strip %exec%
	rm *.o

	:: ���s
	if not exist "%exec%" goto end

	:: ����
	echo.
	pause

:: Test ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	cls

	set s1="1970/12/10"
	set s2=now
	echo %s1% - %s2%

	%exec%
	%exec% %s2% %s1%
	%exec% %s2% %s1% -format "%%g%%y�N%%m��%%d�� %%D�ʎZ�� %%W�T%%w��"
	%exec% %s1% %s2%
	%exec% %s1% %s2% -format "%%g%%y�N%%m��%%d�� %%D�ʎZ�� %%W�T%%w��"
	%exec% %s1% %s2% -f "%%M�ʎZ�� %%H�ʎZ�� %%N�ʎZ�� %%S�ʎZ�b"

:: Quit ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:end
	echo.
	pause
	exit
