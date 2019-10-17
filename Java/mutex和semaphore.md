从Java的多线程，到Java的synchronized，再到Java Monitor，最终到mutex和semaphore，其核心在为同步与互斥

# 同步与互斥

在多任务系统中，同时运行的多个任务，必然存在下面的需求：
- 都需要访问/使用同一种资源
- 多个任务之间有依赖关系，某个任务的运行依赖于另一个任务。

## 同步

是指在不同任务的任务之间的若干程序片段，它们的运行必须按照某种规定的先后次序运行。比如A任务的运行依赖于B任务产生的数据，因此，在B任务产生数据前，A任务必须等待。

## 互斥

不同的任务之间的若干程序片段，当在运行某个任务中的一个程序片段的时候，其它任何一个任务都不能运行，只有等这个任务运行完成才可以运行。比如：一个公共资源只能被一个进程或者线程使用，多个进程和线程不能同时使用公共资源。

#互斥锁(同步)

互斥锁(mutex)，mutual exclusive，也称为排它锁，是一种简单的加锁方法来控制对共享资源的访问，互斥锁只有两种状态，上锁(lock)和解锁(unlock)

- 互斥锁的特点
	1. **原子性**：把一个互斥量锁定为一个原子操作，保证了如果一个线程锁定了一个互斥量，没有其他线程在同一时间可以成功锁定这个互斥量；
	2. **唯一性**：如果一个线程锁定了一个互斥量，在它解除锁定之前，没有其它线程可以锁定这个互斥量；
	3. **非繁忙等待**：如果一个线程已经锁定了一个互斥量，第二个线程又去试图锁定这个互斥量，则第二个线程将被挂起（不占用任何CPU资源），直到第一个线程解除对这个互斥量的锁定为止，第二个线程则被唤醒并继续执行，同时锁定这个互斥量。
- 互斥锁的操作流程
	1. 在临界区前，对互斥锁进行加锁
	2. 在临界区后，对互斥锁进行解锁
	3. 对互斥锁进行加锁后，任何其他试图再次对互斥锁加锁的线程都会被阻塞，直到锁被释放。对互斥锁进行加锁后，任何其他试图再次对互斥锁加锁的线程都会被阻塞，直到锁被释放。
- 临界区，在Java中，用synchronized修饰的代码段或者方法

例如，最简单的并发冲突就是一个变量自增1
```C
balance = balance + 1;
```
这条语句实际编译后对扩展为两条语句：
```C
int tmp = balance + 1;
balance = tmp;
```
这时给这条语句加一个互斥锁，使其达到任何一个线程，要么全部执行上述代码，要么不执行这段代码的效果
```C
lock_t mutex;
...
lock(&mutex);
	balance = balance + 1;
unlock(&mutex);
```
那么如何实现lock()函数呢？
```C
typedef struct __lock_t {int flag;} lock_t;
void init(lock_t *mutex){
	//0:lock is available
	//1:lock is held
	mutex->flag = 0;
}
void lock(lock_t *mutex){
	while(mutex->flag == 1) {
		; //Wait the lock
	}
	mutex->flag = 1;// Set the lock, i.e. start to hold lock
}
void unlock(lock_t *mutex){
	mutex->flag;
}
```

上述代码虽然简单，但是lock()有一个问题。假如thread A是第一个运行到此的线程，它得到的mutex->flag的值为0，于是它跳出循环，继续往下运行，接下来要通过mutex->flag = 1来持有锁，使其它线程在检测while循环为真，进而进入循环等待状态。如果在A执行到这个赋值为1的语句之前，又有另一个thread B运行到了while循环部分，由于mutex->flag还未被赋值为1，B同样可以跳出while，从而跟A一样拿到这把锁！这就出现了冲突。

问题在于:
- 对mutex->flag的检测
- 对mutex->flag的赋值

这两个操作必须是不被干扰的，也就是它必须是atomic的，要么这两段代码不被执行，要么这两段代码被不中断地完整执行。这就需要借助CPU指令集的帮助，来保证上述两条语句的atomic操作。也就是著名的TestAndSet()操作。
```C
int TestAndSet(int *ptr,int new){
	int old = *ptr;
	*ptr = new;
	return old;
}
```
CPU的指令集，并不需要支持繁复的各种atomic操作。仅仅支持上面这个函数，各种互斥加锁的情形都能被覆盖。
此时lock()的实现，可以改造为：
```
typedef struct __lock_t{ int flag;} lock_t;
void init(lock_t *mutex){
	//0:lock is available
	//1:lock is held;
	mutex->flag = 0;
}
void lock(lock_t *mutex){
	while(TestAndSet(&mutex->flag,1)==1){
		;
	}
}
void unlock(lock_t *mutex){
	mutex->flag = 0;
}
```
这样的支持TestAndSet()的实现，是最简单的spin lock，弹簧锁，是最初被工业使用的最简单的实现技术。

其实这个其实和CAS相似

# 条件变量(同步)

与互斥锁不同，条件变量用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊情况发生为止。通常条件变量和互斥锁同时使用。
条件变量是利用线程共享的全局变量进行的一种同步机制，主要包括两个动作：
- 一个线程等待“条件变量的成立条件”而挂起
- 另一个线程使条件变量成立（给出条件成立信号）

## 原理

条件的检测是在互斥锁的保护下进行的。线程在改变条件状态之前，必须首先锁住互斥量。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，重新评价条件。

## 条件变量的操作流程如下：
	1. 初始化：init()或者pthread_cond_tcond=PTHREAD_COND_INITIALIER;属性置为NULL;
	2. 等待条件成立：pthread_wait,pthread_timewait.wait()释放锁，并阻塞等待条件变量为真timewait()设置等待时间，仍未signal，返回ETIMEOUT(加锁保证只有要给线程wait)
	3. 激活条件变量
	4. 清除条件变量

# 读写锁(同步)

# 自旋锁(同步)

自旋锁与互斥量功能一样，唯一不同的就是互斥量阻塞后休眠让出CPU，而自旋锁阻塞后不会让出CPU，会一直忙等待，直到得到锁。
自旋锁在用户态使用的比较少，在内核态使用的比较多，自旋锁使用的场景：锁的持有时间比较短，或者说少于2次上下文切换的时间。

# 信号量(同步与互斥)

Semaphore，信号量广泛应用于进程的或者线程的同步和互斥，信号量的本质是一个非负的整数计数器，它用来控制对公共资源的访问。
PV原语是对信号量的操作，一次P操作使信号量减一，一次V操作使信号量加一。P操作对应于wait()，V操作对应于signal()


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