# For use with the NSIS installer compiler script

## Setup executable and default build locations
OutFile "NABC Windows Installer (x86).exe"
InstallDir $PROGRAMFILES\BlueOkiris\nabc

# Default installer section
Section

## Add uninstaller and compiler
SetOutPath $INSTDIR
File ..\..\build\nabc.exe
WriteUninstaller $INSTDIR\uninstaller.exe

## Add standard library
SetOutPath $INSTDIR
File ..\..\lib\include\std.hpp

## Add start menu shortcut
CreateShortcut $SMPROGRAMS\nabc.lnk $INSTDIR\nabc.exe

SectionEnd

# Section to define Uninstall action
Section "Uninstall"

## Delete everything
Delete $INSTDIR\uninstaller.exe
Delete $INSTDIR\nabc.exe
Delete $INSTDIR\std.hpp
Delete $SMPROGRAMS\nabc.lnk
RMDir $INSTDIR

SectionEnd
