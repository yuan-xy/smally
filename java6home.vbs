set sysenv=CreateObject("WScript.Shell").Environment("system") '系统环境变量的数组对象
'sysenv.Remove("ztest2")                        '删除变量
sysenv("JAVA_HOME")="D:\jdk1.6.0_10"                '添加变量