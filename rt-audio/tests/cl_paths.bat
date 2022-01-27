@echo off

set VS_VER=14.21.27702
set VS_VER=14.25.28610
SET VS_BASE=C:\Program Files (x86)\Microsoft Visual Studio\2019
SET VS_TOOLS_HOME=%VS_BASE%\Community\VC\Tools\MSVC\%VS_VER%

set WIN_SDK_VER=10.0.17763.0
set WIN_SDK_PATH=C:\Program Files (x86)\Windows Kits\10
set WIN_SDK_INCLUDE=%WIN_SDK_PATH%\include\%WIN_SDK_VER%
set WIN_SDK_LIB=%WIN_SDK_PATH%\lib\%WIN_SDK_VER%

SET INCLUDE=%VS_TOOLS_HOME%\include;^
%WIN_SDK_INCLUDE%\ucrt;^
%WIN_SDK_INCLUDE%\shared;^
%WIN_SDK_INCLUDE%\um;^
%WIN_SDK_INCLUDE%\winrt;^
%WIN_SDK_INCLUDE%\cppwinrt

SET LIB=%VS_TOOLS_HOME%\lib\x64;^
%WIN_SDK_LIB%\ucrt\x64;^
%WIN_SDK_LIB%\um\x64

SET Path=%VS_TOOLS_HOME%\bin\HostX64\x64
