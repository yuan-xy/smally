
File.open("tmp.txt","a") do |f|
 File.open("opcode-32'.txt").each_line do |line|
	ss=line.split("\t")
	begin
		f << ss[0]+"\t"+ss[1].to_i(2).to_s(16)+"\r\n"
	rescue
	end
 end
end
 