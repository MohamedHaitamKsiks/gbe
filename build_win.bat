@echo off
setlocal enabledelayedexpansion

REM Visual Studio IDE is NOT required. This uses MSVC Build Tools + CMake + Ninja (preferred) from VS Code terminal.

REM Locate vcvars64.bat dynamically (VS Build Tools)
set "VSWHERE=C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
set "VCVARS="

if exist "%VSWHERE%" (
  for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VS_INSTALL=%%i"
  )
  if defined VS_INSTALL (
    if exist "%VS_INSTALL%\VC\Auxiliary\Build\vcvars64.bat" set "VCVARS=%VS_INSTALL%\VC\Auxiliary\Build\vcvars64.bat"
  )
)

if not defined VCVARS (
  for /f "delims=" %%i in ('where /R "C:\Program Files (x86)" vcvars64.bat 2^>nul') do (
    if not defined VCVARS set "VCVARS=%%i"
  )
)
if not defined VCVARS (
  for /f "delims=" %%i in ('where /R "C:\Program Files" vcvars64.bat 2^>nul') do (
    if not defined VCVARS set "VCVARS=%%i"
  )
)

if not defined VCVARS (
  echo [ERROR] Could not locate vcvars64.bat. Please install "Visual Studio 2022 Build Tools" with the "Desktop development with C++" toolset.
  exit /b 1
)

call "%VCVARS%"
if errorlevel 1 (
  echo [ERROR] Failed to initialize MSVC build environment.
  exit /b 1
)

REM Prefer Ninja if available; otherwise fall back to NMake
where ninja >nul 2>&1
if %errorlevel%==0 (
  set "GEN=-G Ninja"
) else (
  set "GEN=-G NMake Makefiles"
)

REM Resolve CMake path explicitly if needed
set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"
if not exist "%CMAKE_EXE%" set "CMAKE_EXE=cmake"

echo Using CMake: %CMAKE_EXE%
echo Generator: %GEN%
echo Configuring...

"%CMAKE_EXE%" -S . -B build %GEN% -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
  echo [ERROR] CMake configure failed.
  exit /b 1
)

echo Building...
"%CMAKE_EXE%" --build build --config Release --parallel
if errorlevel 1 (
  echo [ERROR] Build failed.
  exit /b 1
)

echo [OK] Build succeeded. Executable should be in build\gbe.exe (or build\Release\gbe.exe depending on generator).
exit /b 0
