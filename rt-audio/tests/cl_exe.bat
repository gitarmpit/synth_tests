@echo off
call cl_paths.bat

set OPTS=/GS /TP /W3 /Zc:wchar_t /I".." ^
  /Gm- /O2 /Ob2  /Zc:inline /fp:precise /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "_MBCS" ^
  /errorReport:prompt /WX- /Zc:forScope /GR /Gd /MD /EHsc /nologo /diagnostics:column 

cl.exe -c %OPTS% %1.cpp


set LOPTS=/OUT:"%1.exe"^
 "dsound.lib" "ksuser.lib" "mfplat.lib" "mfuuid.lib" "wmcodecdspuuid.lib" "winmm.lib" "ole32.lib" ^
 "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "shell32.lib" "oleaut32.lib" "uuid.lib" "comdlg32.lib" "advapi32.lib" ^
 /MACHINE:X64 /INCREMENTAL:NO /SUBSYSTEM:CONSOLE 
 
:: /NOLOGO  suppreses linker's copyright message 
::  /NXCOMPAT   data execution prevention

link.exe %LOPTS% %1.obj ..\Release\rtaudio.lib
