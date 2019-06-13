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


## 1.1说一说自己对于synchronized关键字的理解

synchronized关键字解决的是多线程之间访问资源的同步性，synchronized关键字可以保证被它修饰的方法或者代码块在任意时刻只能有一个线程在执行

在Java的早期版本中，synchronized属于重量级的锁，效率低下，因为监视器锁（monitor）是依赖于底层操作系统的Mutex Lock实现的，Java的线程是映射到操作系统的原生线程之上的。如果要挂起或者唤醒一个线程，都需要操作系统帮忙完成，而操作系统实现线程需要从用户态转换到内核态，这个状态转换过程需要相对比较长的时间，时间成本相对较高。在Java 6之后，从JVM层面对synchronized较大优化，所以现在的synchronized锁效率也有较高的提升。

## 1.2 说说自己是怎么使用synchronized关键字的，在项目中用到了吗
- 修饰实例方法，作用于当前对象实例加锁，进入同步代码前，要获得当前对象实例的锁
- 修饰静态方法，作用于当前类对象加锁，进入同步代码前要获得当前类对象的锁。也就是给当前类加锁，会作用于所有对象实例。线程A调用一个实例对象的非静态synchronized方法，而线程B需要调用这个实例对象所属类的静态synchronized方法，是允许的。
- 修饰代码块，指定加锁对象，对给定对象加锁，进入代码块前，要获得给定对象的锁。

双重校验锁实现单例模式
```java
public class Singleton {
	private volatile static Singleton uniqueInstance;
	private Singleton(){
	}
	
	public static Singleton getUniqueInstance(){
		//先判断对象是否已经实例过，没有实例化过才进入加锁代码
		if(uniqueInstance==null){
			//类对象加锁
			synchronized(Singleton.class){
				if(uniqueInstance==null){
					uniqueInstance = new Singleton();
				}
			}
		}
	}
}
```
这里要注意，uniqueInstance采用volatile关键字修饰，这是很有必要的，uniqueInstance = new Singleton();这段代码其实分为三步执行：
1. 为uniqueInstance分配内存空间
2. 初始化uniqueInstance
3. 将uniqueInstance指向分配的内存地址

但是由于JVM执行重排的特性，执行顺序有可能变成1->3->2.指令重排在单线程环境下不会出现问题，但是在多线程环境下会导致一个线程获得还没初始化的实例。例如，线程T1执行力1和3，此时T2调用了getUniqueInstance()后发现uniqueInstance不为空，因此返回uniqueInstance，但此时uniqueInstance还未被初始化。
使用volatile可以禁止JVM的指令重排，保证在多线程环境下也能正常运行。

## 1.3讲一讲synchronized关键字的底层原理