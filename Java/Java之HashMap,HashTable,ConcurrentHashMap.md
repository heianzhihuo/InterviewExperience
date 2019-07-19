# HashMap,HashTable和ConcurrentHashMap分析
## HashTable
- 数组+链表实现，keyhevalue都**不能为null**，线程安全，实现线程安全的方式是锁住整个表，效率较低，在ConcurrentHashMap中有改进
- 初始size为11，扩容：newsize = oldsize*2+1，size始终是奇数
- index计算方法：index = (hash & 0x7FFFFFFF)%tab.length，由于tab.length始终是奇数，所以简单取模哈希结果更加均匀
- hash = hashCode()
- 迭代器是enumerator，不是fail-fast迭代器的
- 继承自Dictionary类

## HashMap
- 数组+链表实现，key和value**可以为null**，最多只能有一个为null的key，可以有多个value为null，**线程不安全**
- 初始size为16，扩容：newsize = oldsize*2，size始终为2的幂次
- 扩容针对整个Map，每次扩容时，原来元素依次重新计算存放位置，并重新插入
- 插入元素后才判断是否需要扩容，可能产生无效扩容，（即扩容后，可能不再有元素插入）
- 当Map中的总元素超过Entry数组的75%时，触发扩容操作，目的是减小链表长度，使元素分布更均匀
- index计算方法：index = hash%(tab.length-1)，这里的hash计算如下，取得位置更加分散
```java
static int hash(Object x){
	int h = x.hashCode();
	h += ~(h<<9);
	h ^= (h>>>14);
	h += (h<<4);
	h ^= (h>>>10);
	return h;
}
```
- 加载因子(Load Factor)，默认为75%
- 只有一个线程访问的时候HashMap比HashTable效率高
- 迭代器(Iterator)是fail-fast迭代器，当有其它线程修改HashMap结构（增加或移除元素），将会抛出ConcurrenModificationException
- Map interface的一个实现
- 在Java 8中，当链表大小超过阈值8时，链表会被改造成红黑树


## ConcurrentHashMap(Java 5)
- 分段数组+链表，**线程安全**
- 通过把Map分成N个Segment，可以提供相同的线程安全，效率替身了N倍，N默认为16
- 不同段的允许多个修改操作同时进行，其关键在于使用了**锁分离技术**
- 扩容：段内扩容（段内元素超过该段对应Entry数组长度的75%触发扩容，不会对整个Map进行扩容），插入前检测需不需扩容，避免无效扩容
- **锁分段技术**：首先将数据分成一段一段的存储，然后给每一段数据一把锁
- HashTable的替代

## ConcurrentHashMap

[参考文章](https://blog.csdn.net/bill_xiang_/article/details/81122044)

这里对ConcurrentHashMap再做个总结，因为前面写的不够全面，而且在新的Java版本中，分段锁的实现方式已经被摒弃了。

ConcurrentHashMap从JDK1.5开始随java.util.concurrent包一起引入JDK中，解决了HashMap线程不安全和HashTable效率不高的问题。

在JDK1.7之前，ConcurrentHashMap使用分段锁机制实现，JDK1.8则使用数组+链表+红黑树和CAS原子操作实现。

1. ConcurrentHashMap的实现——JDK7版本
	
	分段锁机制
	
	HashTable效率低下的主要原因是因为其实现使用了synchronized关键字对put等操作进行加锁，synchronized关键字是对整个对象进行加锁，也就是说在进行put等修改Hash表操作时，锁住了整个Hash表，从而使其表现的效率低下。因此，在JDK1.5~1.7版本，Java使用了分段锁机制实现ConcurrentHashMap。
	
	ConcurrentHashMap在对象中保存了一个Segment数组，即将整个Hash表划分为多个段；每个Segment元素，即每个分类则像是一个HashTable；这样，执行put操作时，首先根据hash算法定位元素属于哪个Segment，然后对这个Segment加锁即可。因此，ConcurrentHashMap可以实现多线程put操作。
	
	get操作不加锁
	
2. ConcurrentHashMap的实现——JDK8版本

	JDK1.7中ConcurrentHashMap是通过分段锁实现的，其最大并发度受到Segment的个数限制。因此在JDK1.8中，采用数组+链表+红黑树实现，加锁则采用CAS和synchronized实现。
	
	CAS原理
	
	锁一般分为悲观锁和乐观锁，Java中，悲观锁的实现方式是各种锁；而乐观锁则是通过CAS实现的。
	
	CAS操作存在一些缺点：1.存在ABA问题，其解决思路是使用版本号；2.循环时间长，开销大；3.只能保证一个共享变量的原子操作。
	
	
	