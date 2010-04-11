Name "FPSToy"

OutFile "FPSToy.exe"

InstallDir "$PROGRAMFILES\FPSToy"

; Request application privileges for Windows Vista
RequestExecutionLevel user

Page directory
Page instfiles

Section ""

  SetOutPath $INSTDIR
  File ..\..\Data\FPSToy.exe
  File ..\..\Data\licensing.txt
  File ..\..\Data\lgpl.license.txt
  File ..\..\Data\cc.license.txt
  File ..\..\Data\FridgeScript.dll
  File ..\..\Data\FSAssembler.dll
  
  SetOutPath $INSTDIR\Fonts
  File ..\..\Data\Fonts\Console.font
  
  SetOutPath $INSTDIR\Shaders
  File ..\..\Data\Shaders\Raytrace_Plane.shader
  File ..\..\Data\Shaders\Raytrace_Plane.fs
  File ..\..\Data\Shaders\Raytrace_Fullscreen.vs
  
  SetOutPath $INSTDIR\Textures\Fonts
  File ..\..\Data\Textures\Fonts\Console.png
  
  SetOutPath $INSTDIR\Textures\Generic
  File ..\..\Data\Textures\Generic\Grid1.png
  File ..\..\Data\Textures\Generic\Grid2.png
  
  SetOutPath $INSTDIR\Textures\Widgets
  File ..\..\Data\Textures\Widgets\SplashBanner.png
  File ..\..\Data\Textures\Widgets\SplashBanner.license.txt
  
  SetOutPath $INSTDIR
  
  CreateDirectory "$SMPROGRAMS\FPSToy"
  CreateShortCut "$SMPROGRAMS\FPSToy\FPSToy.lnk" "$INSTDIR\FPSToy.exe" "" "$INSTDIR\FPSToy.exe" 0
  
SectionEnd