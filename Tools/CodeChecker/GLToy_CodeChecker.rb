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

puts "GLToy Code Checker"

# strip tabs and trim trailing whitespace, leaving just one newline behind

puts "Cleaning whitespace..."
uTabCount = 0
uTrailCount = 0
uExistCount = 0
uTotalCount = 0
ARGV.each do | szPath |
    uTotalCount += 1
    if ( File::exists?( szPath ) )
        uExistCount += 1
        
        puts "#{szPath}..."
        
        xCurrentFile = File.open( szPath, "r" )
        szOutput = ""
        until xCurrentFile.eof
            cChar = xCurrentFile.getc
            if( cChar == "\t" )
                uTabCount += 1
                szOutput += "    "
            else
                szOutput += cChar
            end
        end
        xCurrentFile.close
        
        szOutput.chomp!
        uTrailCount += 1 unless ( szOutput.rstrip! == nil )
        szOutput += "\n"
        
        xCurrentFile = File.open( szPath, "w" )
        xCurrentFile.write( szOutput )
        xCurrentFile.close
    else
        puts "Error: Unable to open file #{ szPath }"
    end
end

puts "Warning: Replaced #{ uTabCount } tabs with spaces" if ( uTabCount > 0 )
puts "Warning: Removed trailing whitespace from #{ uTrailCount } files" if ( uTrailCount > 0 )
puts "Successfully cleaned whitespace from #{ uExistCount } files" if( uExistCount > 0 )
puts "Error: Unable to open #{ uTotalCount - uExistCount } files" if ( uTotalCount != uExistCount )

# need to implement something like in jEngine's scripts
# iterating over everything not in a string or comment

# puts "Tidying brackets..."
# ARGV.each do | szPath |
    # uTotalCount += 1
    # if ( File::exists?( szPath ) )
        # uExistCount += 1
        
        # puts "#{szPath}..."
        
        # xCurrentFile = File.open( szPath, "r" )
        # szOutput = xCurrentFile.read
        # xCurrentFile.close
        
        # szOutput.gsub!( /\(([^ \)])/, '\( \1' )
        # szOutput.gsub!( /([^ \(])\)/, '\1 \)' )
        # szOutput.gsub!( /\[([^ \]])/, '\[ \1' )
        # szOutput.gsub!( /([^ \[])\]/, '\1 \]' )
        
        # xCurrentFile = File.open( szPath, "w" )
        # xCurrentFile.write( szOutput )
        # xCurrentFile.close
    # else
        # puts "Error: Unable to open file #{ szPath }"
    # end
# end

# check for copyright/license comments and seperator comments
# puts "Verifying layout..."
