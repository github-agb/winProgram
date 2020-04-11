# winProgram

#### 一，Windows进程通信-共享内存空间

##### 共三个模块：用myconsole进程发送指令控制game进程

- game.exe，三个方法，控制台输入指令（'A'，'B'，'R'）分别控制三个方法的调用；
- WGDll.dll，要注入到game进程中的dll文件；
- myconsole.exe，用来注入dll文件的程序；

##### 操作方法：

- 开启game进程，
- 用myconsole把dll注入到game，dll模块和myconsole模块利用共享内存实现进程通信，
- 在myconsole的控制台输入指令，dllmokuai接受指令，调用game模块的方法，实现进程通信，控制game进程