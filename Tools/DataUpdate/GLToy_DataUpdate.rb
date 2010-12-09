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

$LOAD_PATH << './Tools/Include'

# Tools
require "GLToy_ProjectList.rb"

# Ruby
require "fileutils"

puts "GLToy Data Update"

puts ""

Dir.glob( "Data/**/.svn" ) do | szFile |
	if File.directory? szFile
		FileUtils.copy_entry( szFile, szFile.gsub( /\.svn/, "&svn" ) )
		FileUtils.remove_dir( szFile )
	end
end

$g_xProjectListNoDemos.each do | szProject |
	puts "#{ szProject }"
		
	FileUtils.copy_entry( "Data", "#{ szProject }\\Data" )
	
	Dir.glob( "#{ szProject }/Data/**/&svn" ) do | szFile |
        if File.directory? szFile
			FileUtils.remove_dir( szFile )
        end
    end
	
	FileUtils.remove_file( "#{ szProject }\\Data\\TextureTool.exe", true )
	FileUtils.remove_file( "#{ szProject }\\Data\\TextureTool.chm", true )
	FileUtils.remove_file( "#{ szProject }\\Data\\UserImages.bmp", true )
end

Dir.glob( "Data/**/&svn" ) do | szFile |
	puts "fixing #{ szFile }..."
	if File.directory? szFile
		FileUtils.copy_entry( szFile, szFile.gsub( /&svn/, ".svn" ) )
		FileUtils.remove_dir( szFile )
	end
end

puts "Done!"
