:<<BATCH
@echo off
setlocal ENABLEDELAYEDEXPANSION


for %%a in (%*) do set "argv[%%a]=1"

SET KAC_DEBUG=
IF defined argv[--debug-rad] (
 set KAC_DEBUG=/DKAC_DEBUG
)

cl ^
-Od ^
!KAC_DEBUG! ^
/EHsc /d2FH4- /MDd /nologo -fp:except -GR- -EHa- -FC -Z7 ^
-W4 -WX -wd4201 -wd4127 -wd4244 ^
%1 ^
/Feexecutable.exe ^
/link /NODEFAULTLIB:MSVCRT

IF EXIST "executable.exe" (
 IF defined argv[--debug-rad] (
  echo [44mdebugging in RADdbg[0m
  start raddbg --auto_run --quit_after_success executable.exe
 ) ELSE (
  echo [33mrunning executable[0m
  @echo on
  start executable.exe
  @echo off
 )
)



@echo off
exit /B
endlocal
BATCH



if [ "$#" -eq 0 ]; then
 echo "\033[93musage: ./build_and_run.sh file.c\033[0m"
else
 rm -f executable
 clear
 clang "$1" \
 -Werror \
 -Wall \
 -Wextra \
 -Wvla \
 -Wshadow \
 -Wuninitialized \
 -Wsometimes-uninitialized \
 -Wimplicit-function-declaration \
 -Wno-missing-braces \
 -Wno-missing-field-initializers \
 -o executable && ./executable
fi
