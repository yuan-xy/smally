#!/usr/bin/ruby
require 'tempfile'
dir = ARGV.shift
t=Tempfile.new("ext-cd.gdb")
t.puts <<"End"
attach #{Process.ppid}
call chdir(#{dir.dump})
End
t.close
STDIN.reopen("/dev/null")
STDOUT.reopen("/dev/null")
STDERR.reopen("/dev/null")

system("gdb","-batch","-n","-x",t.path)

