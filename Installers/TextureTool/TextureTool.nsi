!define UninstallKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\GLToy Texture Tool"

Name "GLToy Texture Tool"

Icon "..\..\Resources\Windows Icon Files\Generic Tool.ico"
LicenseData "..\..\Data\mit.license.txt"

OutFile "GLToy_Texture_Tool_Installer.exe"

InstallDir "$PROGRAMFILES\GLToy\Texture Tool"

; Request application privileges for Windows Vista - we need admin for the vc redist
RequestExecutionLevel admin

Page directory
Page instfiles

Section ""

    SetOutPath $INSTDIR
    File ..\..\Data\TextureTool.exe
    File ..\..\Data\mit.license.txt

    ; VC 2010 run-time installer (necessary for MFC)
    File vcredist_x86.exe

    ExecWait "$INSTDIR\vcredist_x86.exe /q"

    Delete $INSTDIR\vcredist_x86.exe

    CreateDirectory "$SMPROGRAMS\GLToy\Texture Tool"
    CreateShortCut "$SMPROGRAMS\GLToy\Texture Tool\GLToy Texture Tool.lnk" "$INSTDIR\TextureTool.exe" "" "$INSTDIR\TextureTool.exe" 0
    CreateShortCut "$SMPROGRAMS\GLToy\Texture Tool\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
    
    WriteUninstaller $INSTDIR\uninstall.exe
    
    WriteRegStr HKLM "${UninstallKey}" "DisplayName" "GLToy Texture Tool"
    WriteRegStr HKLM "${UninstallKey}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "${UninstallKey}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM "${UninstallKey}" "Install Location" $INSTDIR
    WriteRegDWORD HKLM "${UninstallKey}" "EstimatedSize" 300000 ;about 300Kb - the vc redist is pretty much the whole size of this installer :/
    
    ExecWait "$INSTDIR\TextureTool.exe /register"

SectionEnd

Section "Uninstall"

    Delete "$SMPROGRAMS\GLToy\Texture Tool\GLToy Texture Tool.lnk"
    Delete "$SMPROGRAMS\GLToy\Texture Tool\Uninstall GLToy Texture Tool.lnk"
    RMDir "$SMPROGRAMS\GLToy\Texture Tool"

    Delete $INSTDIR\TextureTool.exe
    Delete $INSTDIR\mit.license.txt
    Delete $INSTDIR\uninstall.exe
    RMDir $INSTDIR
    
    DeleteRegKey HKLM "${UninstallKey}"

SectionEnd