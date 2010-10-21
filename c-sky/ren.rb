def s_remainder(s,num)
s[num,s.length-num]
end

Dir.new(".").each do |x|
next if x.index("csky-elf-").nil?
name = s_remainder(x,9)
system("cp",x,name)
end

