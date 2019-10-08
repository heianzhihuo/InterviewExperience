# Java中的Monitor机制

# Monitor的概念

管程，英文是Monitor，也常被翻译为监视器。操作系统在面对进程/线程间的同步互斥的时候，所支持的一些同步原语，其中semaphore信号量和mutex互斥量是最重要的同步原语。

在使用基本的mutex进行并发控制时，需要程序员非常小心的控制mutex的down和up操作，否则很容易引起死锁等问题。为了更容易的编写出正确并发程序，所以在mutex和semaphore的基础上，提出了更高层次的同步原语monitor。操作系统本身并不支持monitor，monitor是属于编程语言的范畴，C语言不支持monitor，Java语言支持monitor。monitor机制的实现，属于编译器的工作。

monitor的重要特点是，同一个时刻，只有一个进程/线程能进入monitor中定义的临界区，这使得monitor能够达到互斥效果。无法进入monitor临界区的进程/线程，它们应该被阻塞，并且在必要的时候会被唤醒。mutex和semaphore在变成上容易出错，因为我们要亲自去操作变量以及对进程/线程进行阻塞和唤醒。monitor这个机制称为更高级的原语，因为它可以对外屏蔽掉这些机制，并在内部实现这些机制，使得使用monitor的人看到的是一个更简洁易用的接口。

# monitor的基本元素

monitor机制需要几个元素来配合，分别是：
1. 临界区
2. monitor对象及锁
3. 条件变量以及定义在monitor上的wait，signal操作

使用monitor机制的目的是为了互斥进入临界区，为了阻塞无法进入临界区的进程/线程，还需要一个monitor object来协助，这个monitor object内部会有相应的数据结构，如列表，来保存被阻塞的线程；同时由于monitor机制本质上是基于mutex这种基本原语的，所以monitor object还必须维护一个基于mutex的锁。此外，为了在适当的时候能够阻塞和唤醒进程/线程，还需要引入一个条件变量，这个条件变量在用来决定什么时候是”适当的时候“，这个条件可以来自程序代码的逻辑，也可以是在monitor object的内部，总而言之，程序员对条件变量的定义有很大的自主性。不过，由于monitor object内部采用了数据结构来保存被阻塞的队列，因此它也必须对外提供两个API来让线程进入阻塞状态以及之后被唤醒，分别是wait和notify。

# Java语言对monitor的支持

monitor是操作系统提出来的一种高级原语，但具体实现模式可能不一样。

## 临界区的圈定

在Java中，可以采用synchronized关键字来修饰实例方法、类方法以及代码块，被synchronized关键字修饰的方法、代码块，就是monitor的临界区。

## monitor object

在使用synchronized关键字的时候，往往需要执行一个对象与之关联，如synchronized(this)，或者synchronized(ANOTHER_LOCK)，synchronized如果修饰的是实例方法，那么其关联对象实际上是this，如果修饰的是类方法，那么其关联对象是this.class。总之，synchronized需要关联一个对象，而这个对象就是monitor object。

monitor机制中，monitor object充当着维护mutex以及定义wait/signal API来管理线程的阻塞和唤醒的角色。

Java语言中的java.lang.Object类，便是满足这个要求的对象，任何一个Java对象都可以作为monitor机制的monitor object。
Java对象存储在内存中，分别分为三个部分，即对象头、实例数据和对齐填充，而在其对象头中保存了锁标识；同时，java.lang.Object类定义了wait(),notify(),notifyAll()方法，这些方法的具体实现依赖于一个叫ObjectMonitor模式的实现，这是JVM内部基于C++实现的一套机制。