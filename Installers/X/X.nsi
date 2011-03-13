!define UninstallKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\X"

Name "X"

LicenseData "X.license.txt"

OutFile "X_Installer.exe"

InstallDir "$PROGRAMFILES\GLToy\X"

; Request application privileges for Windows Vista - we need admin for the vc redist
RequestExecutionLevel admin

Page license
Page directory
Page instfiles

Section ""

	SetOutPath "$INSTDIR"
	File ..\..\X\Data\X.exe
	File ..\..\X\Data\licensing.txt
	File ..\..\X\Data\mit.license.txt
	File ..\..\X\Data\cc.license.txt
	;File ..\..\X\Data\autoexec.console
	
    ; VC 2010 run-time installer - sadly necessary
    File ..\TextureTool\vcredist_x86.exe

    ExecWait "$INSTDIR\vcredist_x86.exe /q"

    Delete $INSTDIR\vcredist_x86.exe

	;SetOutPath "$INSTDIR\Environments\"
	;File /r /x .svn ..\..\X\Data\Environments\*.bsp

	SetOutPath "$INSTDIR\Fonts"
	File ..\..\X\Data\Fonts\*.font

	SetOutPath "$INSTDIR\Materials\"
	File /r /x .svn ..\..\X\Data\Materials\*.*

	SetOutPath "$INSTDIR\PFX"
	File /r /x .svn ..\..\X\Data\PFX\*.*

	SetOutPath "$INSTDIR\Shaders"
	File /r /x .svn ..\..\X\Data\Shaders\*.*

	SetOutPath "$INSTDIR\Sounds\"
	File /r /x .svn ..\..\X\Data\Sounds\*.*

	SetOutPath "$INSTDIR\Textures\"
	File /r /x .svn ..\..\X\Data\Textures\*.*

	;SetOutPath "$INSTDIR\Weapons\"
	;File /r /x .svn ..\..\X\Data\Weapons\*.*

	SetOutPath "$INSTDIR"
	CreateDirectory "$SMPROGRAMS\GLToy\X"
	CreateShortCut "$SMPROGRAMS\GLToy\X\X.lnk" "$INSTDIR\X.exe" "" "$INSTDIR\X.exe" 0
	CreateShortCut "$SMPROGRAMS\GLToy\X\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
    
	WriteUninstaller $INSTDIR\uninstall.exe
	  
	WriteRegStr HKLM "${UninstallKey}" "DisplayName" "X"
    WriteRegStr HKLM "${UninstallKey}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "${UninstallKey}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM "${UninstallKey}" "Install Location" $INSTDIR
    WriteRegDWORD HKLM "${UninstallKey}" "EstimatedSize" 15000000 ;about 15MB?
  
SectionEnd

Section "Uninstall"

    RMDir /r "$SMPROGRAMS\GLToy\X"

    RMDir /r $INSTDIR
    
    DeleteRegKey HKLM "${UninstallKey}"

SectionEnd
