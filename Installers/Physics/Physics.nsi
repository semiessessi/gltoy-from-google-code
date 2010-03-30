Name "Physics"

OutFile "Physics.exe"

InstallDir "$PROGRAMFILES\GLToy Physics Demo"

; Request application privileges for Windows Vista
RequestExecutionLevel user

Page directory
Page instfiles

Section ""

  SetOutPath $INSTDIR
  File ..\..\Data\Physics.exe
  
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
  
  SetOutPath $INSTDIR
  
  CreateDirectory "$SMPROGRAMS\GLToy Physics Demo"
  CreateShortCut "$SMPROGRAMS\GLToy Physics Demo\GLToy Physics Demo.lnk" "$INSTDIR\Physics.exe" "" "$INSTDIR\Physics.exe" 0
  
SectionEnd