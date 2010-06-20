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
    
    puts "Copying \"Empty\"..."
    FileUtils.copy_entry( "Empty", szOutPath )
    
    puts "Removing source control and intermediary build files..."
    
    Dir.glob( "#{ szOutPath }/**/.svn" ) do | szFile |
        if File.directory? szFile
            puts "Removing directory \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end

    Dir.glob( "#{ szOutPath }/**/cvs" ) do | szFile |
        if File.directory? szFile
            puts "Removing directory \"#{ szFile }\"..."
            FileUtils.remove_dir( szFile, true )
        end
    end
    
    if File.directory? "#{ szOutPath }/Debug"
        puts "Removing directory \"#{ szOutPath }/Debug\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Debug", true );
    end
    
    if File.directory? "#{ szOutPath }/Release"
        puts "Removing directory \"#{ szOutPath }/Release\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Release", true );
    end
    
    if File.directory? "#{ szOutPath }/Final"
        puts "Removing directory \"#{ szOutPath }/Final\"..."
        FileUtils.remove_dir( "#{ szOutPath }/Final", true );
    end
    
    puts "Renaming files and contents..."
    
    Dir.glob( "#{ szOutPath }/**/*Empty*" ) do | szFile |
        puts "#{ szFile }..."
        
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
        puts "#{ szFile }..."
        
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
end