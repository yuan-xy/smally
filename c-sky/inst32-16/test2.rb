lines = []
File.open("16.txt").each_line do |line|
 lines << line
end

(1..100).each {|x| lines << "" }
puts lines.size

count=0
File.open("32.txt").each_line do |line|
 puts "#{count}: #{line.split[0]} ~ #{lines[count].split[0]}"
 lines.insert(count," \r\n") if line.split[0] != lines[count].split[0]
 count+=1
end

puts lines.size

File.open("16-2.txt","w") do |f|
 lines.each{|x| f << x}
end
