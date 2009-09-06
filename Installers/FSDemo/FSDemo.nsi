Name "FSDemo"

OutFile "FSDemo.exe"

InstallDir $PROGRAMFILES\FSDemo

; Request application privileges for Windows Vista
RequestExecutionLevel user

Page directory
Page instfiles

Section ""

  SetOutPath $INSTDIR
  
  File ..\..\FSDemo\FridgeScript.dll
  File ..\..\FSDemo\FSAssembler.dll
  File ..\..\FSDemo\Script.fs
  File ..\..\Release\FSDemo.exe
  
  CreateDirectory "$SMPROGRAMS\FSDemo"
  CreateShortCut "$SMPROGRAMS\FSDemo\FSDemo.lnk" "$INSTDIR\FSDemo.exe" "" "$INSTDIR\FSDemo.exe" 0
  CreateShortCut "$SMPROGRAMS\FSDemo\Edit Script.lnk" "$INSTDIR\Script.fs" "" "$INSTDIR\Script.fs" 0
  
SectionEnd