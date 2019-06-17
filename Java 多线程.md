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

# synchronized关键字

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

synchronized关键字的底层原理属于JVM层面。
1. synchronized同步代码块的情况

synchronized同步语句块的实现使用的是monitorenter和monitorexit指令，其中monitorenter指令指向同步代码块的开始位置，monitorexit指令则指明同步代码块的结束位置。当执行monitorenter指令时，线程试图获取锁，也就是monitor（monitor对象存在与每个Java对象的对象头中，这也是为什么Java中任意对象可以作为锁的原因）的持有权。当计数器为0则可以成功获取，获取后将锁计数器设为1也就是加1.相应的在执行monitorexit指令后，将锁计数器设为0，表明锁被释放。如果获取对象锁失败，那么当前线程就要阻塞等待，知道锁被另一个线程释放为止。

2. synchronized修饰方法的情况

synchronized修饰方法使用的是ACC_SYNCHRONIZED标识，该标识指明了该方法是一个同步方法，JVM通过该ACC_SYNCHRONIZED访问标志来辨别一个方法是否声明为同步方法，从而执行相应的同步调用。

## 1.6 谈谈JDK1.6之后的synchronized关键字底层做了哪些优化，可以详细介绍一下这些优化吗？

JDK1.6 对锁的实现引入了大量的优化，比如偏向锁、轻量级锁、自旋锁、适应性自旋锁、锁清楚、锁粗化等技术来减少锁操作的开销。锁主要存在四种状态，依次是：无锁状态、偏向锁状态、轻量级锁状态、重量级锁状态，他们会随着竞争的激烈而逐渐升级。锁可以升级而不可降级，这种策略是为了提高获得锁和释放锁的效率。

1. 偏向锁

引入偏向锁的目的和引入轻量级锁的目的很像，都是为了没有多线程竞争的前提下，减少传统重量级锁使用操作系统互斥量产生的性能消耗。但不同的是：轻量级锁在无竞争的情况下使用CAS操作去代替使用互斥量。而偏向锁在无竞争情况下会把整个同步都消除掉。

偏向锁的意思是会偏向第一个获得它的线程，如果在接下来的执行中，该锁没有被其他线程获取，那么持有偏向锁的线程就不需要进行同步。偏向锁失效后会先升级为轻量级锁。

2. 轻量级锁

倘若偏向锁失败，虚拟机并不会立即升级为重量级锁，它还会尝试使用一种称为轻量级锁的优化手段（1.6之后加入的）。轻量级锁的加锁和解锁都用到了CAS操作。
轻量级锁能提升程序同步性能的依据是“对于绝大部分锁，在整个同步周期内都是不存在竞争的”。在有竞争的情况下，轻量级锁比传统的重量级锁更慢！如果竞争激烈，那么轻量级锁将升级为重量级锁。

3. 自旋锁和自适应自旋
 
轻量级锁失败后，虚拟机为了避免线程真实的在操作系统层面挂起，还会进行一项称为自旋锁的优化手段。互斥同步对性能最大的影响就是阻塞的实现，因为挂起线程/恢复线程的操作都需要转入到内核态中完成。
一般的线程持有锁的时间都不是太长，所以仅仅为了这一点时间去挂起线程/恢复线程是得不尝失的。所以为了让一个线程等待，只需要让线程执行一个忙循环（自旋），这项技术就叫做自旋。
自旋在1.6之前就已经引入了，默认是关闭的。1.6之后默认开启。自旋并不能完全代替阻塞，因为它还要占用处理器时间。自旋等待时间必须有限度，超过了限定次数仍然没有获得锁，就应该挂起线程。自旋次数默认是10.
JDK1.6之中引入了自适应的自旋锁。自适应自旋锁的改进就是：自旋时间不再固定，而是和前一次同一个锁上的自旋时间以及锁的拥有者的状态来决定。

4. 锁消除

锁消除指的是虚拟机即使编译在运行时，如果检测到那些共享数据不可能存在竞争，那么就执行锁消除。锁消除可以节省毫无意义的锁请求锁的时间。

5. 锁粗化

原则上，我们在编写代码的时候，推荐将同步块的作用范围限制的尽量小。大部分情况下，这个原则都是没问题的，但是如果一些列连续的操作对同一个对象反复加锁解锁，会带来很多不必要的性能消耗。

## 1.7 Synchronized和ReenTrantLock的对比

# Java线程池

## 2.1 说一说synchronized关键字和volatile关键字的区别
- volatile关键字是线程同步的轻量级实现，所以volatile性能肯定比synchronized关键字要好。但是volatile关键字只能作用于变量，而synchronized关键字可以修饰方法及代码块。在JDK1.6后对synchronized关键字的优化，效率有了显著提升，实际开发中使用synchronized关键字的场景更多一些
- 多线程访问volatile关键字不会发生阻塞，而synchronized关键字可能会发生阻塞
- volatile关键字能保证数据的可见性，但不能保证数据的原子性。synchronized关键字两者都能保证
- volatile关键字主要用于解决变量在多个线程之间的可见性，而synchronized关键字解决的是多个线程之间访问资源的同步性

## 3.1 为什么要使用线程池
- 降低资源消耗。通过重复利用已经创建的线程降低线程创建和销毁造成的消耗
- 提高响应速度。当任务到达时，任务不需要等待线程创建就能立即执行
- 提高线程的可管理性。线程是稀缺资源，如果无限制创建，不仅会消耗资源，还会降低系统的稳定性，使用线程池可以统一进行分配，调优和监控

## 3.2 实现Runnable接口和Callable接口的区别

如果想让线程池执行任务需要实现Runnable接口或Callable接口。区别在于Runnable几口不会返回结果，但是Callable接口可以返回结果

## 3.3 执行execute()方法和submit()方法的区别是什么呢？

1. execute()方法适用于提交不需要返回值的任务，无法判断任务是否被线程池执行成功与否；
2. submit()方法用于提交需要返回值的任务。线程池会返回一个future类型的对象，通过这个future对象可以判断任务是否执行成功，并且可以通过future的get()方法来获取返回值，get()方法会阻塞当前线程直到任务完成，而使用get(long timeout,TimeUnit unit)方法则会阻塞当前线程一段时间后立即返回，这时有可能任务没有执行完。

## 3.4 如何创建线程池

......

# 信号量Semaphore

Semaphore是一个计数信号量，它的本质是一个“共享锁”。信号量维护了一个信号量许可集合。线程通过调用acquire()来获取信号量的许可；当信号量中有可用的许可时，线程获取该许可；否则线程必须等待，直到有可用的许可位置。线程通过release()来释放它所持信号量的许可。

通常把一个非负的整数称为Semaphore，表示为S。S可以理解为可用资源的数量，假定S>=0。
S实现同步机制表示为PV原语句操作
P(S):若S=0，线程进入等待队列；否则--S;
V(S):++S，唤醒处于等待中的线程。

```java
public abstract class Semaphore{
	private int value = 0;
	
	public Semaphore(){
	}
	
	public Semaphore(int initial){
		if(initial>=0)
			value = initial;
		else
			throw new IllegalArgumentException("intial<0");
	}
	
	public final synchronized void P() throws InterruptedException{
		while(value==0)
			wait();
		value--;
	}
	
	protected final synchronized void Vc(){
		value++;
		notifyAll();
	}
	
	protected final synchronized void Vb(){
		value++;
		notifyAll();
		if(value>1)
			value = 1;
	}
	
	public abstract void V();
	
	public String toString(){
		return ".value="+value;
	}

}
```


# Atomic原子类

## 5.1 原子类

Atomic是指一个操作是不可中断的。

## 5.3 AtomicInteger的使用

AtomicInteger类主要利用CAS(compare and swap)+volatile和native方法来保证原子操作，从而避免synchronized的高开销，提高执行效率。

CAS的原理是拿期望值和原本的值作比较，如果相同则更新成新的值。

# AQS原理分析

## AQS介绍

AQS的全称为AbstractQueuedSynchronizer

## 6.2 AQS原理分析

AQS的核心思想是，如果被请求的共享资源空闲，则将当前请求资源的线程设置为有效的工作线程，并将共享资源设置为锁定状态。如果被请求的资源被占用，那么将暂时获取不到锁的线程加入到队列中。

## 6.3 AQS资源共享的方式
- Exclusive(独占):只有一个线程能执行
	- 公平锁：按照线程在队列中的排队顺序，先到者先拿到锁
	- 非公平锁：当线程要获取锁时，无视队列顺序直接去抢锁，谁抢到就是谁的
- Share(共享)：多个线程可以同时执行。