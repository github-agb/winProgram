# winProgram
windows program demo

Windows进程通信-共享内存空间
1，game.exe，三个方法，控制台输入指令（'A'，'B'，'R'）分别控制三个方法的调用；
2，WGDll.dll，要注入到game进程中的dll文件；
3，myconsole.exe，用来注入dll文件的程序；
先开启game进程，然后用myconsole把dll注入到game，dll模块和myconsole模块利用共享内存实现进程通信，在myconsole的控制台输入指令，dllmokuai接受指令，调用game模块的方法，达到控制game的目的