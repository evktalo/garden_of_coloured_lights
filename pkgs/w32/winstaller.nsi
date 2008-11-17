;This is a NSIS (see http://nsis.sourceforge.net) script file.
;You have to have a `makensis` utility (available for free under Windows and Linux) to generate .exe installer files.
;In order to be succesful, you also have to supply required files (look at the uninstaller section where they are mentioned at one place)
;
;
;NSIS Modern User Interface
;Written by Matěj Týč

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Garden of coloured lights"
  OutFile "garden-install.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\garden"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\garden" "InstallLocation"

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE ../../COPYING
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_STARTMENU "garden" $StartMenuFolder
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
    !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section # "Mandatory"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\garden" \
                   "DisplayName" "garden of coloured lights, an extraordinary shoot-em-up"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\garden" \
                   "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\garden" \
                   "InstallLocation" "$INSTDIR"

  CreateDirectory "$INSTDIR"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN garden
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall garden.lnk" "$INSTDIR\uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd


Section "garden" GARDEN_BIN
SetOutPath "$INSTDIR"

file ../../bin/.libs/garden.exe
file mingwm10.dll

!insertmacro MUI_STARTMENU_WRITE_BEGIN garden
createShortCut "$SMPROGRAMS\$StartMenuFolder\garden.lnk" "$INSTDIR\garden.exe" "" "garden.ico"
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section "garden" GARDEN_DATA
SetOutPath "$INSTDIR"
file ../../data/init.txt

SetOutPath "$INSTDIR\beat"
file ../../data/beat/arp.wav
file ../../data/beat/beam.wav
file ../../data/beat/blip.wav
file ../../data/beat/cymbal1.wav
file ../../data/beat/dline.wav
file ../../data/beat/drum1.wav
file ../../data/beat/drum2.wav
file ../../data/beat/drum3.wav
file ../../data/beat/drum4.wav
file ../../data/beat/flute.wav
file ../../data/beat/flute2_l.wav
file ../../data/beat/flute2_s.wav
file ../../data/beat/flute_l.wav
file ../../data/beat/food.wav
file ../../data/beat/funny.wav
file ../../data/beat/light.wav
file ../../data/beat/line1.wav
file ../../data/beat/line2.wav
file ../../data/beat/line3.wav
file ../../data/beat/long.wav
file ../../data/beat/long2.wav
file ../../data/beat/multi.wav
file ../../data/beat/multi2.wav
file ../../data/beat/nice.wav
file ../../data/beat/nice2.wav
file ../../data/beat/pan.wav
file ../../data/beat/piano.wav
file ../../data/beat/saw.wav
file ../../data/beat/spinner.wav
file ../../data/beat/square.wav
file ../../data/beat/stage4.wav
file ../../data/beat/string.wav
file ../../data/beat/sweet.wav
file ../../data/beat/sweet2.wav
file ../../data/beat/synth.wav
file ../../data/beat/thing4.wav
file ../../data/beat/voice.wav
file ../../data/beat/walker.wav
file ../../data/beat/xline.wav

SetOutPath "$INSTDIR\gfx"
file ../../data/gfx/garden.dat
file ../../data/gfx/large.bmp
file ../../data/gfx/multi.bmp
file ../../data/gfx/platform.bmp
file ../../data/gfx/small.bmp
file ../../data/gfx/splash.bmp
file ../../data/gfx/trans.bmp

SetOutPath "$INSTDIR\wavs"
file ../../data/wavs/bang.wav
file ../../data/wavs/bang2.wav
file ../../data/wavs/blop.wav
file ../../data/wavs/cannon.wav
file ../../data/wavs/extra.wav
file ../../data/wavs/fire.wav
file ../../data/wavs/level.wav
file ../../data/wavs/lwbeam.wav
file ../../data/wavs/mgun.wav
file ../../data/wavs/mouth.wav
file ../../data/wavs/p_bang.wav
file ../../data/wavs/rect.wav
file ../../data/wavs/rect2.wav
file ../../data/wavs/scatter.wav
file ../../data/wavs/swbeam.wav
file ../../data/wavs/whoosh.wav
file ../../data/wavs/worms.wav

SectionEnd

; uninstaller section
Section "Uninstall" 


  Delete $INSTDIR\beat\arp.wav
  Delete $INSTDIR\beat\beam.wav
  Delete $INSTDIR\beat\blip.wav
  Delete $INSTDIR\beat\cymbal1.wav
  Delete $INSTDIR\beat\dline.wav
  Delete $INSTDIR\beat\drum1.wav
  Delete $INSTDIR\beat\drum2.wav
  Delete $INSTDIR\beat\drum3.wav
  Delete $INSTDIR\beat\drum4.wav
  Delete $INSTDIR\beat\flute.wav
  Delete $INSTDIR\beat\flute2_l.wav
  Delete $INSTDIR\beat\flute2_s.wav
  Delete $INSTDIR\beat\flute_l.wav
  Delete $INSTDIR\beat\food.wav
  Delete $INSTDIR\beat\funny.wav
  Delete $INSTDIR\beat\light.wav
  Delete $INSTDIR\beat\line1.wav
  Delete $INSTDIR\beat\line2.wav
  Delete $INSTDIR\beat\line3.wav
  Delete $INSTDIR\beat\long.wav
  Delete $INSTDIR\beat\long2.wav
  Delete $INSTDIR\beat\multi.wav
  Delete $INSTDIR\beat\multi2.wav
  Delete $INSTDIR\beat\nice.wav
  Delete $INSTDIR\beat\nice2.wav
  Delete $INSTDIR\beat\pan.wav
  Delete $INSTDIR\beat\piano.wav
  Delete $INSTDIR\beat\saw.wav
  Delete $INSTDIR\beat\spinner.wav
  Delete $INSTDIR\beat\square.wav
  Delete $INSTDIR\beat\stage4.wav
  Delete $INSTDIR\beat\string.wav
  Delete $INSTDIR\beat\sweet.wav
  Delete $INSTDIR\beat\sweet2.wav
  Delete $INSTDIR\beat\synth.wav
  Delete $INSTDIR\beat\thing4.wav
  Delete $INSTDIR\beat\voice.wav
  Delete $INSTDIR\beat\walker.wav
  Delete $INSTDIR\beat\xline.wav
  RMDir  $INSTDIR\beat 
  
  Delete $INSTDIR\gfx\garden.dat
  Delete $INSTDIR\gfx\large.bmp
  Delete $INSTDIR\gfx\multi.bmp
  Delete $INSTDIR\gfx\platform.bmp
  Delete $INSTDIR\gfx\small.bmp
  Delete $INSTDIR\gfx\splash.bmp
  Delete $INSTDIR\gfx\trans.bmp
  RMDir  $INSTDIR\gfx

  Delete $INSTDIR\wavs\bang.wav
  Delete $INSTDIR\wavs\bang2.wav
  Delete $INSTDIR\wavs\blop.wav
  Delete $INSTDIR\wavs\cannon.wav
  Delete $INSTDIR\wavs\extra.wav
  Delete $INSTDIR\wavs\fire.wav
  Delete $INSTDIR\wavs\level.wav
  Delete $INSTDIR\wavs\lwbeam.wav
  Delete $INSTDIR\wavs\mgun.wav
  Delete $INSTDIR\wavs\mouth.wav
  Delete $INSTDIR\wavs\p_bang.wav
  Delete $INSTDIR\wavs\rect.wav
  Delete $INSTDIR\wavs\rect2.wav
  Delete $INSTDIR\wavs\scatter.wav
  Delete $INSTDIR\wavs\swbeam.wav
  Delete $INSTDIR\wavs\whoosh.wav
  Delete $INSTDIR\wavs\worms.wav
  RMDir  $INSTDIR\wavs

  Delete $INSTDIR\garden.exe
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\uninstall.exe ; delete self (see explanation below why this works)
  RMDir  $INSTDIR

  !insertmacro MUI_STARTMENU_GETFOLDER garden $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\garden.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall garden.lnk"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\garden" 
SectionEnd
;--------------------------------
;Descriptions

  ;Language strings
;  LangString DESC_Sec ${LANG_ENGLISH} ""

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

