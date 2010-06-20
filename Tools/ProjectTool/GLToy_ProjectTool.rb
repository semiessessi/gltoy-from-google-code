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
                puts "   d                Creates a fork of the GLToy data structures with the project"
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
        puts "  #{ szFile }..."
        
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
#define NULL (0)
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
#else
    #define #{ szProjectName.upcase }_RELEASE
    #define #{ szProjectName }_IsDebugBuild() ( false )
    #define #{ szProjectName }_DebugVar static const
#endif

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

#endif
" )
    xCurrentFile.close    
    puts "  #{ szCurrentFile }"
end