#############################################################################################
#
# ©Copyright 2010 Semi Essessi
#
#############################################################################################
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
#############################################################################################

require "fileutils"

puts "GLToy Project Tool"

# interpret switches
bCreateDirectory = false
bDataStructures = false
bGLToyProject = false
bReplace = false
szOutPath = ""
szProjectName = "Project"

ARGV.each do | szArgument |
    
    bSwitch = true
    szSwitch = String.new( szArgument )

    if ( szSwitch[ 0, 1 ] == "/" ) or ( szSwitch[ 0, 1 ] == "-" )
        szSwitch.slice!( 0 )
    elsif szSwitch[ 0, 2 ] == "--"
        szSwitch.slice!( 0..1 )
    else
        szOutPath = String.new( szSwitch )
        bSwitch = false
    end
    
    if bSwitch
        case szSwitch
            when /"help"|h|\?/
                puts "GLToy Project Tool"
                puts ""
                puts "Creates files based on GLToy"
                puts ""
                puts "Usage: GLToy_ProjectTool.rb [switches] target"
                puts ""
                puts " target: the root directory to output files to"
                puts " switches: any of /, - or -- followed by:"
                puts "   ?                Displays this message"
                puts "   h"
                puts "   help"
                puts ""
                puts "   c                Creates the target directory if it doesn't exist"
                puts "   create-dir"
                puts ""
                puts "   d                Creates data structures and maths functionality"
                puts "   data-structures"
                puts ""
                puts "   g                Specifies that the files are for GLToy project"
                puts "   gltoy-project    Without this flag a general C++ project is created"
                puts ""
                puts "   n:[name]         Specifies the name for the output project"
                puts "   name[name]       If left unspecified, the project will be called \"Project\""
                puts ""
                puts "   r                Clean the target directory, i.e. replace its contents"
                puts "   replace"
                
            when /"create-dir"|c/
                bCreateDirectory = true
            
            when /"data-structures"|d/
                bDataStructures = true
            
            when /"gltoy-project"|g/
                bGLToyProject = true
            
            when /"name"\:.*/
                szProjectName = String.new( szSwitch )
                szProjectName.slice!( 0..4 )
            
            when /n\:.*/
                szProjectName = String.new( szSwitch )
                szProjectName.slice!( 0..1 )
            
            when /"replace"|r/
                bReplace = true
            
        end
    end
end

puts "Generating files in #{ szOutPath } for project \"#{ szProjectName }\"..."
puts ""

# make sure our directory is set up

if bCreateDirectory and not File.directory? szOutPath
    puts "Creating path \"#{ szOutPath }\""
    Dir.mkdir szOutPath
end

if not File.directory? szOutPath
    puts "Fatal error: Output path \"#{ szOutPath }\" does not exist"
    Process.exit
end

if bReplace
    puts "Cleaning target directory..."
    Dir.glob( "#{ szOutPath }/**/*" ) do | szFile |
        if not File.directory? szFile
            File.delete szFile
        end
    end
    
    Dir.glob( "#{ szOutPath }/**/*" ) do | szFile |
        if File.directory? szFile
            FileUtils.remove_dir( szFile, true )
        end
    end
end

# do the work
puts "Project type : #{ bGLToyProject ? "GLToy" : "Independent" }#{ bDataStructures ? " with data structures" : "" }"

if bGLToyProject

    # copy Empty into the directory
    if not File.directory? "Empty"
        puts "Fatal error: To create a GLToy project you must run the script from the root directory of the GLToy source"
        Process.exit
    end
	
	puts "Creating GLToy project..."
    
    puts "Copying from \"Empty\"..."
    FileUtils.copy_entry( "Empty", szOutPath )
    
    puts "Removing source control and intermediary build files..."
    
    Dir.glob( "#{ szOutPath }/**/.svn" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end

    Dir.glob( "#{ szOutPath }/**/cvs" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    Dir.glob( "#{ szOutPath }/**/.cvs" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    Dir.glob( "#{ szOutPath }/**/.p4" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    if File.directory? "#{ szOutPath }/Debug"
        puts "  Removing \"#{ szOutPath }/Debug\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Debug", true );
    end
    
    if File.directory? "#{ szOutPath }/Release"
        puts "  Removing \"#{ szOutPath }/Release\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Release", true );
    end
    
    if File.directory? "#{ szOutPath }/Final"
        puts "  Removing \"#{ szOutPath }/Final\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Final", true );
    end
    
    puts "Renaming files and contents..."
    
    Dir.glob( "#{ szOutPath }/**/*Empty*" ) do | szFile |
        puts "  #{ szFile.gsub( "Empty", szProjectName ) }..."
        
        xCurrentFile = File.open( szFile, "r" )
        szOutput = xCurrentFile.read
        xCurrentFile.close
        
        szOutput.gsub! "Empty", szProjectName
        szOutput.gsub! "EMPTY", szProjectName.upcase()
        
        xCurrentFile = File.open( szFile, "w" )
        xCurrentFile.write( szOutput )
        xCurrentFile.close
        
        FileUtils.move szFile, szFile.gsub( "Empty", szProjectName )
    end
    
    Dir.glob( "#{ szOutPath }/**/*GLToy*" ) do | szFile |
        puts "  #{ szFile }..."
        
        xCurrentFile = File.open( szFile, "r" )
        szOutput = xCurrentFile.read
        xCurrentFile.close
        
        szOutput.gsub! "Empty", szProjectName
        szOutput.gsub! "EMPTY", szProjectName.upcase()
        
        xCurrentFile = File.open( szFile, "w" )
        xCurrentFile.write( szOutput )
        xCurrentFile.close
    end
    
else

	puts "Creating C++ project..."

    puts "Creating directory structure..."
    
    Dir.mkdir "#{ szOutPath }/Independent"
    puts( "  Created #{ szOutPath }/Independent" ) if File.directory? "#{ szOutPath }/Independent"
    Dir.mkdir "#{ szOutPath }/Independent/Core"
    puts( "  Created #{ szOutPath }/Independent/Core" ) if File.directory? "#{ szOutPath }/Independent/Core"
    Dir.mkdir "#{ szOutPath }/Win32"
    puts( "  Created #{ szOutPath }/Win32" ) if File.directory? "#{ szOutPath }/Win32"
    Dir.mkdir "#{ szOutPath }/Win32/Core"
    puts( "  Created #{ szOutPath }/Win32/Core" ) if File.directory? "#{ szOutPath }/Win32/Core"
    
    puts "Creating code files..."
    
    szCurrentFile = "#{ szOutPath }/Win32/Core/Platform_#{ szProjectName }.h"
    xCurrentFile = File.open( szCurrentFile, "w" )
    xCurrentFile.write(
"#ifndef __PLATFORM_#{ szProjectName.upcase }_H_
#define __PLATFORM_#{ szProjectName.upcase }_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// L I B R A R I E S
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#define #{ szProjectName.upcase }_PLATFORM_WIN32

#define #{ szProjectName }_API __stdcall
#define #{ szProjectName }_Export __declspec( dllexport )
#define #{ szProjectName }_Import __declspec( dllimport )
#define #{ szProjectName }_Inline __forceinline

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

#endif
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
    
    szCurrentFile = "#{ szOutPath }/Independent/Core/#{ szProjectName }.h"
    xCurrentFile = File.open( szCurrentFile, "w" )
    xCurrentFile.write(
"#ifndef __#{ szProjectName.upcase }_H_
#define __#{ szProjectName.upcase }_H_

/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/Platform_#{ szProjectName }.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// P R A G M A S
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4250 ) // inheritance by dominance - which I think is pretty desirable behaviour...

/////////////////////////////////////////////////////////////////////////////////////////////
// M A C R O S
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL ( 0 )
#endif

// prevent const_cast and dynamic cast
// TODO: work out a better way, this fails spectacularly with VC 2010 which feels compelled to #include <exception> somewhere
// presumably for the standard library bits that I use, despite the fact that exceptions are disabled
//#define const_cast CONST_CAST_IS_FORBIDDEN_if_you_really_must_then_undef_const_cast
//#define dynamic_cast DYNAMIC_CAST_IS_FORBIDDEN_dont_undef_dynamic_cast

// is this a debug build?
// also set all debug related defines here
#ifdef _DEBUG
    #define #{ szProjectName.upcase }_DEBUG
    #define #{ szProjectName }_IsDebugBuild() ( true )
    #define #{ szProjectName }_DebugVar static
	#define #{ szProjectName }_DebugOutput( format, ... ) #{ szProjectName }::DebugOutput( format, __VA_ARGS__ )
#else
    #define #{ szProjectName.upcase }_RELEASE
    #define #{ szProjectName }_IsDebugBuild() ( false )
    #define #{ szProjectName }_DebugVar static const
	#define #{ szProjectName }_DebugOutput( format, ... )
#endif

// TODO: Asserts in project tool
#define #{ szProjectName }_Assert( ... )

/////////////////////////////////////////////////////////////////////////////////////////////
// T Y P E D E F S
/////////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

typedef unsigned int #{ szProjectName }_Hash;

/////////////////////////////////////////////////////////////////////////////////////////////
// C O N S T A N T S
/////////////////////////////////////////////////////////////////////////////////////////////

static const #{ szProjectName }_Hash u#{ szProjectName.upcase }_BAD_HASH = 0;
static const u_int uDEBUGOUTPUT_MAX_LENGTH = 512;

/////////////////////////////////////////////////////////////////////////////////////////////
// C L A S S E S
/////////////////////////////////////////////////////////////////////////////////////////////

class #{ szProjectName }
{

public:

	static void DebugOutput( const char* szFormatString, ... );
	
};

#endif
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
    
    szCurrentFile = "#{ szOutPath }/Independent/Core/#{ szProjectName }.cpp"
    xCurrentFile = File.open( szCurrentFile, "w" )
    xCurrentFile.write(
"/////////////////////////////////////////////////////////////////////////////////////////////
// I N C L U D E S
/////////////////////////////////////////////////////////////////////////////////////////////

#include <Core/#{ szProjectName }.h>

// C/C++ headers
#include <stdarg.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////
// D A T A
/////////////////////////////////////////////////////////////////////////////////////////////

static char szDebugMessageBuffer[ uDEBUGOUTPUT_MAX_LENGTH ];

/////////////////////////////////////////////////////////////////////////////////////////////
// F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////////////////////

void #{ szProjectName }::DebugOutput( const char* szFormatString, ... )
{
    va_list xArgumentList;

    va_start( xArgumentList, szFormatString );
    int iMessageLength = _vscprintf( szFormatString, xArgumentList ) + 1;

    vsprintf( szDebugMessageBuffer, /* iMessageLength, */ szFormatString, xArgumentList );

    // we no longer need xArgumentList
    va_end( xArgumentList );

	// TODO in GLToy_ProjectTool...
    //Platform_DebugOutput( szDebugMessageBuffer );
}
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
	
	puts "Creating project files..."
    
    szCurrentFile = "#{ szOutPath }/#{ szProjectName }.vcxproj"
    xCurrentFile = File.open( szCurrentFile, "w" )
    xCurrentFile.write(
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
  <ItemGroup Label=\"ProjectConfigurations\">
    <ProjectConfiguration Include=\"Debug|Win32\">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include=\"Release|Win32\">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include=\"Independent\\Compression\\#{ szProjectName }_Compression.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Array.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BinaryTree.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BitStream.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BSPTree.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_DataStructure.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_FlatDataStructure.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_HashTree.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_List.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Pair.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Stack.h\" />
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Tree.h\" />
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }.h\" />
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Functor.h\" />
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Serialisable.h\" />
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Timer.h\" />
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Maths.h\" />
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Matrix.h\" />
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Plane.h\" />
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Quaternion.h\" />
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Vector.h\" />
    <ClInclude Include=\"Win32\\Core\\Platform_#{ szProjectName }.h\" />
    <ClInclude Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Maths.h\" />
    <ClInclude Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Vector.h\" />
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include=\"Independent\\Compression\\#{ szProjectName }_Compression.cpp\" />
    <ClCompile Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BitStream.cpp\" />
    <ClCompile Include=\"Independent\\Core\\#{ szProjectName }.cpp\" />
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Maths.cpp\" />
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Matrix.cpp\" />
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Plane.cpp\" />
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Quaternion.cpp\" />
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Vector.cpp\" />
    <ClCompile Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Maths.cpp\" />
    <ClCompile Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Vector.cpp\" />
  </ItemGroup>
  <PropertyGroup Label=\"Globals\">
    <ProjectGuid>{2688314A-C516-445E-8471-156DD9450762}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <RootNamespace>#{ szProjectName }</RootNamespace>
  </PropertyGroup>
  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />
  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">
    <ConfigurationType>StaticLibrary</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">
    <ConfigurationType>StaticLibrary</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />
  <ImportGroup Label=\"ExtensionSettings\">
  </ImportGroup>
  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">
    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />
  </ImportGroup>
  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">
    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />
  </ImportGroup>
  <PropertyGroup Label=\"UserMacros\" />
  <PropertyGroup />
  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">
    <ClCompile>
      <PrecompiledHeader>
      </PrecompiledHeader>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <PreprocessorDefinitions>WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>./Independent/;./Win32/</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <PrecompiledHeader>
      </PrecompiledHeader>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <PreprocessorDefinitions>WIN32;NDEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>./Independent/;./Win32/</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <SubSystem>Windows</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />
  <ImportGroup Label=\"ExtensionTargets\">
  </ImportGroup>
</Project>
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
	
    szCurrentFile = "#{ szOutPath }/#{ szProjectName }.vcxproj.filters"
    xCurrentFile = File.open( szCurrentFile, "w" )
	xCurrentFile.write(
"<?xml version=\"1.0\" encoding=\"utf-8\"?>
<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
  <ItemGroup>
    <Filter Include=\"Win32\">
      <UniqueIdentifier>{49e9dedd-150d-4b03-b6cd-004bd6a4bfa2}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Win32\\Core\">
      <UniqueIdentifier>{b7a0d07c-fa7c-4bc2-b217-7455681c786f}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Independent\">
      <UniqueIdentifier>{b03bf959-6011-4a87-88e1-397dc1e642b9}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Independent\\Core\">
      <UniqueIdentifier>{d01b899b-db70-4837-8bc0-eeeba9fee9db}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Independent\\Core\\Data Structures\">
      <UniqueIdentifier>{bcba01d9-7fa1-43a1-aac0-a92f0da5cdd9}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Independent\\Maths\">
      <UniqueIdentifier>{b3a3e99f-1baa-4f6b-a72e-66febbd5536a}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Win32\\Maths\">
      <UniqueIdentifier>{50353087-1b0e-4f1f-8fee-5e05b6bff54f}</UniqueIdentifier>
    </Filter>
    <Filter Include=\"Independent\\Compression\">
      <UniqueIdentifier>{4dd84edb-2f21-4688-8176-9b76b99f97a9}</UniqueIdentifier>
    </Filter>
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include=\"Win32\\Core\\Platform_#{ szProjectName }.h\">
      <Filter>Win32\\Core</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }.h\">
      <Filter>Independent\\Core</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Tree.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Array.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BinaryTree.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BitStream.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BSPTree.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_DataStructure.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_FlatDataStructure.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_HashTree.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_List.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Pair.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_Stack.h\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Vector.h\">
      <Filter>Independent\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Maths.h\">
      <Filter>Win32\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Vector.h\">
      <Filter>Win32\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Maths.h\">
      <Filter>Independent\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Timer.h\">
      <Filter>Independent\\Core</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Functor.h\">
      <Filter>Independent\\Core</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Core\\#{ szProjectName }_Serialisable.h\">
      <Filter>Independent\\Core</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Matrix.h\">
      <Filter>Independent\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Plane.h\">
      <Filter>Independent\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Maths\\#{ szProjectName }_Quaternion.h\">
      <Filter>Independent\\Maths</Filter>
    </ClInclude>
    <ClInclude Include=\"Independent\\Compression\\#{ szProjectName }_Compression.h\">
      <Filter>Independent\\Compression</Filter>
    </ClInclude>
  </ItemGroup>
  <ItemGroup>
    <ClCompile Include=\"Independent\\Core\\Data Structures\\#{ szProjectName }_BitStream.cpp\">
      <Filter>Independent\\Core\\Data Structures</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Vector.cpp\">
      <Filter>Independent\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Vector.cpp\">
      <Filter>Win32\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Win32\\Maths\\Platform_#{ szProjectName }_Maths.cpp\">
      <Filter>Win32\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Maths.cpp\">
      <Filter>Independent\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Core\\#{ szProjectName }.cpp\">
      <Filter>Independent\\Core</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Matrix.cpp\">
      <Filter>Independent\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Plane.cpp\">
      <Filter>Independent\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Maths\\#{ szProjectName }_Quaternion.cpp\">
      <Filter>Independent\\Maths</Filter>
    </ClCompile>
    <ClCompile Include=\"Independent\\Compression\\#{ szProjectName }_Compression.cpp\">
      <Filter>Independent\\Compression</Filter>
    </ClCompile>
  </ItemGroup>
</Project>
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
	
    szCurrentFile = "#{ szProjectName }.sln"
    xCurrentFile = File.open( szCurrentFile, "w" )
	xCurrentFile.write(	
"Microsoft Visual Studio Solution File, Format Version 11.00
# Visual C++ Express 2010
Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"#{ szProjectName }\", \"#{ szProjectName }\\#{ szProjectName }.vcxproj\", \"{2688314A-C516-445E-8471-156DD9450762}\"
EndProject
Global
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|Win32 = Debug|Win32
		Release|Win32 = Release|Win32
	EndGlobalSection
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{2688314A-C516-445E-8471-156DD9450762}.Debug|Win32.ActiveCfg = Debug|Win32
		{2688314A-C516-445E-8471-156DD9450762}.Debug|Win32.Build.0 = Debug|Win32
		{2688314A-C516-445E-8471-156DD9450762}.Release|Win32.ActiveCfg = Release|Win32
		{2688314A-C516-445E-8471-156DD9450762}.Release|Win32.Build.0 = Release|Win32
	EndGlobalSection
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
EndGlobal
" )
    xCurrentFile.close
    puts "  #{ szCurrentFile }"
	
end

if bDataStructures
	puts "Creating data structures..."    
    
	Dir.mkdir "#{ szOutPath }/Independent/Core/Data Structures"
	Dir.mkdir "#{ szOutPath }/Independent/Maths"
	Dir.mkdir "#{ szOutPath }/Win32/Maths"
	
	puts "Copying from \"GLToy\"..."
	FileUtils.copy_entry( "GLToy/Independent/Compression", "#{ szOutPath }/Independent/Compression" ) 
	FileUtils.copy_entry( "GLToy/Independent/Core/Data Structures", "#{ szOutPath }/Independent/Core/Data Structures" ) 
	FileUtils.copy_entry( "GLToy/Win32/Maths", "#{ szOutPath }/Win32/Maths" )
	
	puts "Removing source control files..."
    
    Dir.glob( "#{ szOutPath }/**/.svn" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end

    Dir.glob( "#{ szOutPath }/**/cvs" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    Dir.glob( "#{ szOutPath }/**/.cvs" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    Dir.glob( "#{ szOutPath }/**/.p4" ) do | szFile |
        if File.directory? szFile
            puts "  Removing \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
	
	FileUtils.cp "GLToy/Independent/Core/GLToy_Functor.h", "#{ szOutPath }/Independent/Core/"
	FileUtils.cp "GLToy/Independent/Core/GLToy_Serialisable.h", "#{ szOutPath }/Independent/Core/"
	FileUtils.cp "GLToy/Independent/Core/GLToy_Timer.h", "#{ szOutPath }/Independent/Core/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Maths.h", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Maths.cpp", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Vector.h", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Vector.cpp", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Plane.h", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Plane.cpp", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Quaternion.h", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Quaternion.cpp", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Matrix.h", "#{ szOutPath }/Independent/Maths/"
	FileUtils.cp "GLToy/Independent/Maths/GLToy_Matrix.cpp", "#{ szOutPath }/Independent/Maths/"
	
	puts "Renaming files and contents..."
    
    Dir.glob( "#{ szOutPath }/**/*GLToy*" ) do | szFile |
        puts "  #{ szFile.gsub( "GLToy", szProjectName ) }..."
        
        xCurrentFile = File.open( szFile, "r" )
        szOutput = xCurrentFile.read
        xCurrentFile.close
        
        szOutput.gsub! "GLToy", szProjectName
        szOutput.gsub! "GLTOY", szProjectName.upcase()
		szOutput.gsub! "\#include \<Maths/#{ szProjectName }_Ray\.h\>\n", ""
        
        xCurrentFile = File.open( szFile, "w" )
        xCurrentFile.write( szOutput )
        xCurrentFile.close
        
        FileUtils.move szFile, szFile.gsub( "GLToy", szProjectName )
    end
end