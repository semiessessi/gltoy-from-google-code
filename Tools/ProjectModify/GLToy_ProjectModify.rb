#############################################################################################
#
# ęCopyright 2010 Semi Essessi
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

puts "GLToy Project Modify"

# interpret switches
bSync = false
bLibrary = true
szProject = "GLToy"
szCPP = "g++ -traditional -felide-constructors -fenum-int-equiv -fnonnull-objects -fpermissive -O -w -I./Independent -I./Linux"

ARGV.each do | szArgument |
    
    bSwitch = true
    szSwitch = String.new( szArgument )

    if ( szSwitch[ 0, 1 ] == "/" ) or ( szSwitch[ 0, 1 ] == "-" )
        szSwitch.slice!( 0 )
    elsif szSwitch[ 0, 2 ] == "--"
        szSwitch.slice!( 0..1 )
    else
        szProject = String.new( szSwitch )
        bSwitch = false
    end

    if bSwitch
        case szSwitch
            when /"help"|h|\?/
                puts ""
                puts "Modifies projects"
                puts ""
                puts "Usage: GLToy_ProjectModify.rb [switches] [project]"
                puts ""
                puts " project: the project to modify, defaults to \"GLToy\""
                puts " switches: any of /, - or -- followed by:"
                puts "   ?                Displays this message"
                puts "   h"
                puts "   help"
                puts "   e                The project should produce an executable with GLToy"
                puts "   executable"
                puts "   s                Update build scripts from the VS project"
                puts "   sync"

			when /"executable"|e/
                bLibrary = false

            when /"sync"|s/
                bSync = true

        end
    end
end

if not File.directory? szProject
    puts "Fatal error: Project path \"#{ szProject }\" does not exist"
    puts "Make sure Project Modify is run from the GLToy root directory"
    Process.exit
end

if not bLibrary
	szCPP = szCPP + " -o #{ szProject }" 
else
	szCPP = szCPP + " -c" 
end

if bSync
    puts "Reading VS project file #{ szProject }/#{ szProject }.vcxproj..."
    xCurrentFile = File.open( "./#{ szProject }/#{ szProject }.vcxproj", "r" )
    szData = xCurrentFile.read
    xCurrentFile.close	

    szOutput = (
"#############################################################################################
#
# \u00A9Copyright #{ Time.new.year } - Generated by GLToy_ProjectModify.rb
#
#############################################################################################
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the \"Software\"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
#############################################################################################

all:
" )
	
	szOutput += "\t" + szCPP

	# find files in the .vcxproj file
	axFiles = Array.new
	szData.scan( /ClCompile Include=\"([^\"]+)/ ) do | aszMatch |
		if /Independent/.match( aszMatch.first ) or /Linux/.match( aszMatch.first )
			aszMatch.first.gsub!( /\\/, "\/" )
			puts "  Adding file #{ aszMatch.first }..."
			szOutput += " \"" + aszMatch.first + "\""
			axFiles.push( aszMatch.first )
		end
	end

	szOutput += " &> make.output"

	if bLibrary 
		szOutput += "\n\tar rc #{ szProject }.a"
		axFiles.each do | szCPPFile |
			szCPPFile.sub!( /\.cpp/, ".o" )
			szOutput += " \"" + szCPPFile + "\""
		end
	end

	szOutput += "\n"
		
    xCurrentFile = File.open( "./#{ szProject }/makefile", "w" )
    xCurrentFile.write szOutput
    xCurrentFile.close
end
