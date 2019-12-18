@echo off

call cl_paths.bat

del /q Release\*

set OPTS=/GS /TP /W3 /Zc:wchar_t /I"." /I"asio_sdk" /Gm- /O2 /Ob2 /Zc:inline /fp:precise ^
 /D "_CRT_SECURE_NO_WARNINGS" ^
 /D "WIN32" /D "_WINDOWS" /D "NDEBUG" ^
 /D "__WINDOWS_ASIO__" ^
 /D "__WINDOWS_WASAPI__" ^
 /D "__WINDOWS_DS__" ^
 /D "RTAUDIO_EXPORT" ^
 /D "_WINDLL" /D "_MBCS" /WX- /Zc:forScope /GR /Gd /MD ^
 /EHsc /nologo  /wd4275 /wd4251

cl.exe -c %OPTS% asio_sdk\asio.cpp         /Fo"Release\asio.obj"
cl.exe -c %OPTS% asio_sdk\asiodrivers.cpp  /Fo"Release\asiodrivers.obj"
cl.exe -c %OPTS% asio_sdk\asiolist.cpp     /Fo"Release\asiolist.obj"

cl.exe -c %OPTS%  RtApi.cpp           /Fo"Release\RtApi.obj"
cl.exe -c %OPTS%  RtAudio.cpp         /Fo"Release\RtAudio.obj"
cl.exe -c %OPTS%  RtAudio_ASIO.cpp    /Fo"Release\RtAudio_ASIO.obj"
cl.exe -c %OPTS%  RtAudio_DS.cpp      /Fo"Release\RtAudio_DS.obj"
cl.exe -c %OPTS%  RtAudio_WASAPI.cpp  /Fo"Release\RtAudio_WASAPI.obj"

set LOPTS=/OUT:"Release\rtaudio.dll" ^
 /DYNAMICBASE ^
 "dsound.lib" "ksuser.lib" "mfplat.lib" "mfuuid.lib" "wmcodecdspuuid.lib" ^
 "winmm.lib" "ole32.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" ^
 "shell32.lib" "oleaut32.lib" "uuid.lib" "comdlg32.lib" "advapi32.lib" ^
 /IMPLIB:"Release\rtaudio.lib" /DLL /MACHINE:X64 ^
 /SUBSYSTEM:CONSOLE /NOLOGO

link.exe Release\*.obj %LOPTS%

