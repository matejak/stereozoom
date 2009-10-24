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
  Name "stereozoom2"
  OutFile "stereozoom2-install.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\stereozoom2"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2" "InstallLocation"

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
  !insertmacro MUI_PAGE_LICENSE ../COPYING
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_STARTMENU "stereozoom2" $StartMenuFolder
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

InstType "Common"
InstType "Full"

; RO => read only section
Section # "Mandatory"
  SectionIN 1 2 RO
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2" \
                   "DisplayName" "stereozoom2 - more than a cool stereo pairs viewer"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2" \
                   "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2" \
                   "InstallLocation" "$INSTDIR"

  CreateDirectory "$INSTDIR"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN stereozoom2
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall stereozoom2.lnk" "$INSTDIR\uninstall.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

SubSection "programs" SecBIN

  Section "gstereozoom2" SecBIN_GUI
  SectionIN 1 2
  SetOutPath "$INSTDIR"
  
  file ../bin/gstereozoom2.exe

  file mingwm10.dll
  file DevIL.dll
  file alleg42.dll
 
  !insertmacro MUI_STARTMENU_WRITE_BEGIN stereozoom2
  createShortCut "$SMPROGRAMS\$StartMenuFolder\gstereozoom2.lnk" "$INSTDIR\gstereozoom2.exe" "" "gstereozoom2.ico"
  !insertmacro MUI_STARTMENU_WRITE_END

  SectionEnd

  Section "stereozoom2" SecBIN_CLI
  SectionIN 2
  SetOutPath "$INSTDIR"

  file ../bin/stereozoom2.exe
  SectionEnd

SubSectionEnd

SubSection "data files" SecDAT

  Section "documentation" SecDAT_HELP
  SectionIN 2
  SetOutPath "$INSTDIR\docs"
;  file ../docs/stereozoom2.pdf

  !insertmacro MUI_STARTMENU_WRITE_BEGIN stereozoom2
  createShortCut "$SMPROGRAMS\$StartMenuFolder\stereozoom2 guide.lnk" "$INSTDIR\docs\stereozoom2.pdf"
  !insertmacro MUI_STARTMENU_WRITE_END
  SectionEnd

  Section "sample images" SecDAT_IMG
  SectionIN 1 2
  SetOutPath "$INSTDIR\images"
;  file bla qua hleq
  SectionEnd
SubSectionEnd

; uninstaller section
Section "Uninstall" 
  Delete $INSTDIR\docs\stereozoom2.pdf
  RMDir  $INSTDIR\docs

  Delete $INSTDIR\images
  Delete $INSTDIR\images
  Delete $INSTDIR\images
  Delete $INSTDIR\images
  Delete $INSTDIR\images
  Delete $INSTDIR\images
  RMDir  $INSTDIR\images

  Delete $INSTDIR\uninstall.exe ; delete self (see explanation below why this works)
  Delete $INSTDIR\gstereozoom2.exe
  Delete $INSTDIR\stereozoom2.exe
  Delete $INSTDIR\mingwm10.dll
  Delete $INSTDIR\DevIL.dll
  Delete $INSTDIR\alleg42.dll
  RMDir  $INSTDIR

  !insertmacro MUI_STARTMENU_GETFOLDER stereozoom2 $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\gstereozoom2.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\stereozoom2 guide.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall stereozoom2.lnk"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2" 
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\stereozoom2"
SectionEnd
;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecBIN ${LANG_ENGLISH} "The stereozoom2 based executables, you probably want this."
  LangString DESC_SecBIN_GUI ${LANG_ENGLISH} "gstereozoom2 has a nice graphical interface, Windows users definitelly want this."
  LangString DESC_SecBIN_CLI ${LANG_ENGLISH} "stereozoom2 has powerful command line interface, but is probably more useful with some decent shell or in scripts"

  LangString DESC_SecLIB ${LANG_ENGLISH} "stereozoom2 library (licensed under LGPL) is useful if you want to develop applications or run applications based on it. You are probably not interested in it unless you are a programmer."
  LangString DESC_SecLIB_H ${LANG_ENGLISH} "header files, you need them if you want to use either of the libraries"
  LangString DESC_SecLIB_DLL ${LANG_ENGLISH} "shared library (DLL). Useful if you need one"
  LangString DESC_SecLIB_A ${LANG_ENGLISH} "static library. Useful if you want to use stereozoom2 library without forcing people to install the DLL"

  LangString DESC_SecDAT ${LANG_ENGLISH} "The data files (documentation, sample images)"
  LangString DESC_SecDAT_HELP ${LANG_ENGLISH} "If you want documentation for stereozoom2 interface and the library API"
  LangString DESC_SecDAT_IMG ${LANG_ENGLISH} "You will loose the last doubts after checking stereozoom2 on these sample images!"
;  LangString DESC_Sec ${LANG_ENGLISH} ""

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecBIN} $(DESC_SecBIN)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecBIN_GUI} $(DESC_SecBIN_GUI)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecBIN_CLI} $(DESC_SecBIN_CLI)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecLIB} $(DESC_SecLIB)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecLIB_H} $(DESC_SecLIB_H)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecLIB_DLL} $(DESC_SecLIB_DLL)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecLIB_A} $(DESC_SecLIB_A)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDAT} $(DESC_SecDAT)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDAT_HELP} $(DESC_SecDAT_HELP)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDAT_IMG} $(DESC_SecDAT_IMG)
;    !insertmacro MUI_DESCRIPTION_TEXT ${Sec} $(DESC_Sec)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

