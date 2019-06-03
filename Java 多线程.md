# Java线程与多线程

Java中的线程一直不太懂，也基本没用过

## Java中的线程
- java.lang.Thread
- Java中的线程有一个调用栈
- Java总是从main()方法开始执行，main()方法运行在一个线程内，主线程
- 用户线程和守护线程

## Java线程的定义
- 继承Thread类
- 实现Runnable接口

有个run方法
```java
public void run();
```

## 线程状态
- 新状态
- 可运行状态
- 运行状态
- 等待/阻塞/睡眠状态
- 死亡状态
    - 睡眠：调用Thread.sleep(long millis)方法，强制当前线程休眠，放在run()之内
    - 线程优先级和线程让步yield
    
    线程的让步是通过Thread.yield()来实现的。yield()方法的作用是，暂停当前正在执行的对象，并执行其他线程

## 线程的同步与锁
- 