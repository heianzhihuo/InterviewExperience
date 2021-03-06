# JVM垃圾回收机制(GC,Garbage Collector)

## JVM要回收哪些区域

在JVM中，程序计数器、JVM栈、本地方法栈是不需要回收的，因为它们的声明周期与线程同步，随着线程的销毁，它们占用的内存会自动释放。所以只有方法区和堆需要GC。一句话**JVM垃圾回收机制针对的是堆中的对象**

## 如何判断对象是否存活

### 1引用计数法

堆中每个对象实例都有一个引用计数器。当要给对象被创建时，就给该对象分配一个变量，该变量设置为1。当任何其它变量赋值为这个对象的引用时，计数器加1。

优点：实现简单，判断效率高
缺点：很难解决对象之间的相互引用，所以Java没用采用这种方法

### 2可达性分析法(根搜索算法)

程序把所有引用关系看作一张图，从一个节点GC ROOT开始，寻找对应的引用节点，找到这个节点后，继续寻找这个节点的的引用节点，当所有引用节点寻找完毕之后，剩余的节点则被认为是没有被引用到的节点，即无用的节点，无用的节点将会被判定为是可回收对象。

可以作为GC Roots的对象
- JVM栈中引用的对象
- 方法中静态属性引用的对象
- 方法中常量引用的对象
- 本地方法栈用JNI引用的对象

## 对象死亡前(被回收)前
不可达的对象并不是立即被销毁，如果要回收一个不可达对象，需要经历两次标记过程，首先是第一次标记，并判断对象是否重写了finalize方法，如果没有重写，则直接进行第二次标记并被回收。如果对象在finalize()方法中重新与引用链接建立了关联关系，则对象并不被回收，继续存活。

## 方法区如何判断是否需要回收
方法区要回收的内容有：废弃常量和无用的类。废弃常量也可以通过引用可达性来判断。
无用的类则需要同时满足下面3个条件：
- 该类的所有实例都已经被回收，Java堆中不存在该类的任何实例
- 加载该类的ClassLoader已经被回收
- 该类对应的java.lang.Class对象没有在任何地方被引用，无法在任何地方通过反射访问该类的方法

## 常用的垃圾回收算法

### 1标记-清除算法

标记-清除算法采用从根集合（GC Roots）进行扫描，堆存活的对象进行标记，标记完毕后，再扫描整个空间中未被标记的对象，进行回收，此算法没有虚拟机采用。

优点：解决了循环引用问题，不需要移动对象，效率较高，不需要额外空间
缺点：扫描了两次，效率较低，收集暂停时间长，产生不连续的内存碎片

### 2 复制算法
将内存分成两块容量大小相等的区域，每次只使用其中一块，当这一块内存用完了，就将所有存活对象复制到另一块内存，然后清除前一块内存，这样就不容易出现内存碎片问题。
缺点：复制的代价很高，适合新生代，因为新生代的对象存活率较低，需要复制的对象较少，需要双倍的内存空间，而且总是有一块内存空闲

### 3标记-整理算法
再标记之后，将存活对象往一端移动，然后清理掉边界以外的内存。不产生碎片，但是移动对象成本大。

### 4 分代收集算法

分代收集算法是目前JVM的主流算法。核心思想是根据对象存活的生命周期将内存划分为若干个不同的区域。一般情况下将堆区划分为老年代（Tenured Generation）和新生代（Young Generation），在堆区之外还有一个代就是永久代（Parmanet Generation）。老年代的特点是每次垃圾收集时只有少量的对象需要被回收，而新生代的特点是每次垃圾回收时都有大量的对象需要被回收，那么就可以根据不同代的特点采取最适合的收集算法。


内存被分成三个区域
- 新生代：Enden，from survivor space，to survivor space，
- 老年代，Tenured
- 永久代：方法区

### 新生代的回收算法
新生代包含三个区：Eden，from survivor space, to survivor space。绝大多数最新被创建的对象都被分配到这里，大部分对象在创建后会变得很快不可达。
1. 所有新生对象首先都是放在新生代的，新生代的目标就是尽可能快速的收集掉那些声明周期短的对象。
2. 新生代内存按照8:1:1的比例分为一个Eden和两个survivor区（s0，s1）区。大部分对象在Eden区生成，回收时先将Eden区存活的对象复制到s0区，然后清空eden区，当s0区存放满了时，将eden区存活对象复制到s1区，然后情况Eden和s0区，然后s0区和s1区交换，保持s1区为空
3. 当s1区不足以存放eden和s0区存活对象时，将存活对象直接存放到老年代。若是老年代也满了就会触发一次Full GC，也就是新生代、老年代都回收。
4. 新生代发生的GC叫做Minor GC，Minor GC发生的频率比较高，不一定等Eden区满了才触发

### 老年代回收算法
1. 在年轻代经历了N次垃圾回收后仍然存活的对象，就会被放到老年代中，因此，老年代中存活的对象是声明周期较长的对象
2. 内存比新生代大，（大概比例是1：2），当老年代内存满时触发Major GC也就是Full GC，Full GC的发生频率比较低。


# 1 JVM内存分配与回收

GC堆被分成三个区域：新生代、老年代、永久代
新生代又分为：Eden、from survivor、to survivor
划分的目的是更好的回收内存和更快的分配内存

## 1.1 对象在eden区分配
大部分对象在新生代的eden区分配，当eden区满时，JVM触发一次Minor GC
- Minor GC：发生在新生代的垃圾回收动作，Minor GC非常频繁，回收速度一般比较快
- Major GC/Full GC：发生带老年代的GC，出现了Full GC通常伴随至少一次Minor GC，Full GC速度比Minor GC慢，且频率低

## 1.2 大对象直接进入老年代
大对象就是需要大量连续内存空间的对象，比如字符串和数组。
目的时避免为大对象分配内存时由于分配担保机制带来的复制而降低效率。

## 1.3 长期存活的对象将进入老年代
虚拟机给每个对象一个年龄(Age)计数器。如果对象在Eden中出生，经过一次Minor GC后仍然能够存活，并且能够被survivor容纳，将被移动到survivor空间，并且将对象年龄设为1.对象在survivor空间中，每经过一次Minor GC年龄就加1，当年龄增加到一定程度（默认为15岁），就会晋升到老年代，这个阈值可以通过参数-XX:MaxTenuringThreshold来设置

## 1.4 动态对象的年龄判定

如果Survivor空间中相同年龄的对象大小综合大于survivor空间的一般，年龄大于或等于该年龄的对象就可以直接进入老年代，无需达到需求的年龄。

# 2 判定对象死亡的算法

## 2.1引用计数法

## 2.2可达性分析法

## 2.3四种引用方式

1. 强引用

创建一个对象，并把这个对象赋值给一个引用变量。强引用的有引用变量指向时，永远不会被垃圾回收，JVM抛出OutOfMemory错误也不会回收这种对象。

2. 软引用

如果一个对象具有软引用，内存空间足够，垃圾回收器就不会回收它。如果内存空间不足了，则这些对象就会被回收。虚拟机会尽快回收长时间闲置不用的软引用对象。软引用可以用来构建敏感数据的缓存。

3. 弱引用

当JVM进行垃圾回收时，无论空间是否充足，都会回收被若引用关联的对象。弱引用可以和一个引用队列联合使用。

4. 虚引用

虚引用在任何时候都可能被垃圾回收器回收。虚引用必须和引用队列关联使用。

## 2.4不可达对象

即使在可达性分析过程中不可达的对象，并非时立即被回收的。一个对象死亡，至少要经过两次标记过程。



# 4垃圾收集器

垃圾收集器是垃圾回收算法（标记-清除算法、复制算法、标记-整理算法、火车算法）的具体实现。下面介绍的hotSpot虚拟机中的垃圾收集器

## 4.1 垃圾收集器

JDK7/8后，HotSpot虚拟收集器及组合(连线)，如下图：
![图片展示](/HotSpot虚拟机收集器及组合.jpg)

1. 图中展示了7种不同的分代收集器
2. 所处区域表明其是属于新生代还是老年代收集器
3. 两个收集器之间有连线则表明它们可以搭配使用

## 4.2 并发收集器和并行收集器的区别
- 并行：多条垃圾收集线程并行工作，但此时用户线程仍然处于等待状态
- 并发：用户线程和垃圾收集线程同时执行，如CMS和G1

## 4.3 Serial收集器
Serial(串行)收集器时最基本的例是最悠久的垃圾收集器

1. 特点
	
	- 针对新生代
	- 采用复制算法
	- 单线程收集
	- 进行垃圾收集时，必须暂停所有工作线程，直到完成

2. 应用场景

HotSpot在Client模式下默认的新生代收集器
优点:简单高效，没有线程切换开销

## 4.4 ParNew收集器
ParNew收集器是Serial收集器的多线程版，

1. 特点
	- 除了使用多线程外，其余行为、特点和Serial收集器一样
	- 两个收集器公用了不少代码

2. 应用场景

Server模式下，是虚拟机的重要选择，除了Serial收集器外，只有它能与CMS收集器配合工作。	

3. 为什么只有ParNew能与CMS收集器配合

CMS是在JDK1.5推出的第一个真正意义上的并发收集器，第一次实现了垃圾收集线程与用户线程同时工作。CMS无法与Parallel Scavenge配合工作，因为Parallel Scavenge以及G1都没有用传统的GC收集器代码框架，而另外独立实现；而其余几种收集器则公用了部分框架代码

## 4.5 Parallel Scavenge收集器
Parallel Scavenge收集器与吞吐量密切相关，也成为吞吐量收集器。

1. 特点
	- 有些特点与ParNew相似：新生代收集器、复制算法、多线程收集
	- 关注点与其它收集器不同，其目标是提高吞吐量。而CMS等则关注点是尽可能缩短垃圾收集时用户线程的停顿时间。

2. 应用场景

高吞吐量为目标，应用程序运行在具有多个CPU上，堆暂停时间没有特别高的要求时，程序主要在后台进行计算，而不需要与用户进行太多交互。常用于那些执行批处理、订单处理、工资支付、科学计算的应用程序。

## 4.6 Serial Old收集器

Serial Old是Serial收集器的老年版本

1. 特点
	- 针对老年代
	- 标记-整理算法
	- 单线程收集

2. 应用场景

主要用于Client模式。在Server模式有两大用途：与Parallel Scavenge收集器搭配使用，作为CMS的后备预案
	

## 4.7 Parallel Old收集器

Parallel Old垃圾收集器是Parallel Scavenge收集器的老年代版本

1. 特点
	- 针对老年代
	- 采用标记-整理算法
	- 多线程收集
	
2. 应用场景

在JDK1.6之后代替Serial Old。在Server模式，多CPU情况

## 4.8 CMS收集器
标记并发清理收集器也称为并发低停顿收集器或低延迟收集器

1. 特点
	- 针对老年代
	- 基于标记-清除算法
	- 以获取最短回收停顿时间为目标
	- 并发收集、低停顿
	- 需要更多内存

2. 应用场景

与用户交互较多，注重服务的相应速度，给用户带来较好的体验，常见的web、B/S系统的服务器上

3. CMS收集器运行过程

	- 初始标记：仅标记GC Roots能直接关联到的对象，速度很快，但需要Stop the world
	- 并发标记：进行GC Roots Tracing的过程，刚才产生的集合中标记出存活的对象，应用程序也在运行，不能保证标记出所有存活对象
	- 重新标记：修正并发标记期间因用户程序继续运作而导致标记变动的那一部分对象的标记记录。需要Stop the world，停顿时间比初始标记长，但比并发标记短，采用多线程并行执行来提高效率
	- 并发清除

4. 缺点：虽然不会暂停用户线程，但是因为占用一部分CPU资源，使应用程序变慢，总吞吐量降低

## 4.9 G1收集器

G1（Garbage-First）使JDK7后推出的商用收集器

1. 特点
	- 并行与并发：充分利用多CPU、多核环境下的硬件优势，可以缩短Stop the world的时间，也可以让并发让垃圾收集和用户程序同时执行
	- 分代收集，收集范围包括新生代和老年代，能独立管理整个GC堆，不需要其它收集器搭配
	- 结合多种垃圾收集算法，空间整合，不产生碎片
	- 可预测的停顿：低停顿的同时实现高吞吐量


