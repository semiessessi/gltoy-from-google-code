!define UninstallKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\FPSToy"

Name "FPSToy"

LicenseData "FPSToy.license.txt"

OutFile "FPSToy_Installer.exe"

InstallDir "$PROGRAMFILES\GLToy\FPSToy"

; Request application privileges for Windows Vista - we need admin for the vc redist
RequestExecutionLevel admin

Page license
Page directory
Page instfiles

Section ""

	SetOutPath "$INSTDIR"
	File ..\..\FPSToy\Data\FPSToy.exe
	File ..\..\FPSToy\Data\licensing.txt
	File ..\..\FPSToy\Data\mit.license.txt
	File ..\..\FPSToy\Data\cc.license.txt
	File ..\..\FPSToy\Data\autoexec.console
	
    ; VC 2010 run-time installer - sadly necessary
    File ..\TextureTool\vcredist_x86.exe

    ExecWait "$INSTDIR\vcredist_x86.exe /q"

    Delete $INSTDIR\vcredist_x86.exe

	SetOutPath "$INSTDIR\Environments\"
	File /r /x .svn ..\..\FPSToy\Data\Environments\*.bsp

	SetOutPath "$INSTDIR\Fonts"
	File ..\..\FPSToy\Data\Fonts\Console.font

	SetOutPath "$INSTDIR\Materials\"
	File /r /x .svn ..\..\FPSToy\Data\Materials\*.*

	SetOutPath "$INSTDIR\PFX"
	File /r /x .svn ..\..\FPSToy\Data\PFX\*.*

	SetOutPath "$INSTDIR\Shaders"
	File /r /x .svn ..\..\FPSToy\Data\Shaders\*.*

	SetOutPath "$INSTDIR\Sounds"
	File /r /x .svn ..\..\FPSToy\Data\Sounds\*.*

	SetOutPath "$INSTDIR\Textures\"
	File /r /x .svn ..\..\FPSToy\Data\Textures\*.*

	SetOutPath "$INSTDIR\Weapons\"
	File /r /x .svn ..\..\FPSToy\Data\Weapons\*.*

	SetOutPath "$INSTDIR"
	CreateDirectory "$SMPROGRAMS\GLToy\FPSToy"
	CreateShortCut "$SMPROGRAMS\GLToy\FPSToy\FPSToy.lnk" "$INSTDIR\FPSToy.exe" "" "$INSTDIR\FPSToy.exe" 0
	CreateShortCut "$SMPROGRAMS\GLToy\FPSToy\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
    
	WriteUninstaller $INSTDIR\uninstall.exe
	  
	WriteRegStr HKLM "${UninstallKey}" "DisplayName" "FPSToy"
    WriteRegStr HKLM "${UninstallKey}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "${UninstallKey}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM "${UninstallKey}" "Install Location" $INSTDIR
    WriteRegDWORD HKLM "${UninstallKey}" "EstimatedSize" 15000000 ;about 15MB?
  
SectionEnd

Section "Uninstall"

    RMDir /r "$SMPROGRAMS\GLToy\FPSToy"

    RMDir /r $INSTDIR
    
    DeleteRegKey HKLM "${UninstallKey}"

SectionEnd
