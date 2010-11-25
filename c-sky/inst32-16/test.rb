File.open("32.txt","a") do |f|
 flag=false
 File.open("inst.txt").each_line do |line|
  if flag
   f << line
   flag = false
  end
  flag = true if line[0,5] == "yufa:"
 end
end
 
