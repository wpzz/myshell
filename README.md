# myshell
tiny command shell imitate the linux shell<br>
简易Linux终端命令解析器（2016.09-2016.10）vim+gcc<br>
项目描述：模仿linux shell实现的命令行解释器，用户能通过输入命令与系统进行交互。<br>
项目要点：
由fork（）及exec（）函数搭出基本框架，再运用linux相关知识及相应api实现了cd、ls、mkdir、rmdir、rm、pwd等linux常用命令。<br>

myshell.c 为主体代码<br>
mybin 文件夹下为具体命令的实现代码<br>
