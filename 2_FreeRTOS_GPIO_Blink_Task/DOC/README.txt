栈首就是栈开始的地方，栈顶就是栈中下次即将弹出的第一个元素
栈起始地址是内存开辟的首地址 数组第一位的地址

typedef void (*TaskFunction_t)(void*);
定义一个参数为void *类型,返回值为 void的函数指针类型TaskFunction_t.可以用此类型声明指针变量,存储函数的地址,用指针变量跳转到函数执行

float (*h)(); 
表示h是一个指向返回值float类型的函数的指针 
(float(*)()) 
表示一个"指向返回值float类型的函数的指针"的类型转换符 

C在编译时，每一个函数都有一个入口地址，该入口地址就是函数指针所指向的地址。
有了指向函数的指针变量后。可用该指针变量调用函数，就如同用指针变量可引用其他类型变量一样

TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode)   //任务入口（任务的函数名）

指针 地址 指针 地址 指针 地址 指针 地址 指针 地址 指针 地址

数组名
xTaskCreateStatic StackType_t * const puxStackBuffer 参数是任务栈的数组名
(1)数组名作为函数形参时，在函数体内，其失去了本身的内涵，仅仅只是一个指针； 
(2)很遗憾，在失去其内涵的同时，它还失去了其常量特性，可以作自增、自减等操作，可以被修改。 
所以，数据名作为函数形参时，其全面沦落为一个普通指针！它的贵族身份被剥夺，成了一个地地道道的只拥有4个字节的平民。 
调用时指向数组首元素的地址

Thumb状态：执行半字对齐的16位Thumb指令。
假如，第一次取ARM指令1的地址为 0x0000 0000，由于ARM指令占32位（4个字节），
因此地址0x0000 0001、0x0000 0002、0x0000 0003都是指令1的地盘。
那么第二次取ARM指令2的地址为 0x0000 0004，同样的道理，0x0000 0005、0x0000 0006、0x0000 0007也都是指令2的地盘，以此类推：

MSP和PSP 的含义是Main_Stack_Pointer 和Process_Stack_Pointer,在逻辑地址上他们都是R13
主堆栈指针和进程堆栈指针

svc 0
nop
现在的cpu都是多流水的，前一个指令执行的时候，后一条指定已经在译码了，所以svc 0后面必须跟条指令，不让cpu译码的时候出错就行

SVC和PendSV
SVC（系统服务调用，亦简称系统调用）和PendSV（可悬起系统调用）另一个相关的异常是 PendSV（可悬挂的系统调用），它和 SVC 协同使用。
一方面， SVC异常是必须立即得到响应的（若因优先级不比当前正处理的高， 或是其它原因使之无法立即响应， 将上访成硬 fault），应用程序执行 SVC 时都是希望所需的请求立即得到响应。
另一方面， PendSV 则不同，它是可以像普通的中断一样被悬起的（不像 SVC 那样会上访）。 OS 可以利用它“缓期执行”?一个异常——直到其它重要的任务完成后才执行动作。 
悬起 PendSV 的方法是：手工往 NVIC 的 PendSV 悬起寄存器中写 1。 悬起后， 如果优先级不够高，则将缓期等待执行。
PendSV是可悬起异常，如果我们把它配置最低优先级，那么如果同时有多个异常被触发，它会在其他异常执行完毕后再执行，而且任何异常都可以中断它。

当调用 bx r14指令后，硬件会知道要从异常返回，然后出栈，这个时候堆栈指针PSP已经指向了新任务堆栈的正确位置，
当新任务的运行地址被出栈到PC寄存器后，新的任务也会被执行。

configPRIO_BITS       		6 63个中断优先级

主堆栈指针总是指向栈顶位置。一般堆栈的栈底不能动，所以数据入栈前要先修改堆栈指针，使它指向新的空余空间然后再把数据存进去，出栈的时候相反。按"先进后出"的原则存取数据
Cortex-M3有两个堆栈指针，它们是banked，任一时刻只能使用其中一个；对堆栈指针SP（R13）的操作是对当前有效的堆栈指针的操作
- 如果线程模式使用的是MSP，则线程模式和handler使用一个堆栈，即MSP；
- 如果线程模式使用的是PSP，则进入handler时，自动压栈使用的是PSP，进入handler后切换到MSP，退出handler时在切换到PSP；
SP指针 ARM每种模式都有一个自己的R13物理寄存器，应用程序会初始化R13,使其指向该模式专用的栈地址，这就是常说的初始化SP指针
对比Linux开发手册p286和两个服务函数可以看到 sp指针在出栈时（任务栈去PC寄存器运行），先手动r4-r11出栈，然后sp指针指向任务堆栈栈顶，再设置一下链接寄存器就可以将剩下的寄存器自动出栈
（异常发生->进服务函数->服务函数里手动出栈一部分->设置好sp（r13）指针指向栈顶和r14（退出设置）->跳转r14->剩下寄存器自动出栈->PC(R15)即任务入口->进入任务运行
同理对比任务切换
（pend异常发生->进服务函数->根据当前PSP的值自动入栈一部分寄存器，PSP指针也会自动改变->再手动入栈一部分->将栈顶保存进任务控制块的栈顶变量->切换，然后过程和上面一样

MSP 28 主堆栈


