puts "GLToy CodeChecker"

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
puts "Successfully cleaned whitespace from #{ uExistCount } files"
puts "Unable to open #{ uTotalCount - uExistCount } files"

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
