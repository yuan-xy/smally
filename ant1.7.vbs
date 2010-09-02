set sysenv=CreateObject("WScript.Shell").Environment("system") '系统环境变量的数组对象
'sysenv.Remove("ztest2")                        '删除变量
sysenv("ANT_HOME")="D:\apache-ant-1.7.0"                '添加变量