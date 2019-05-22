# Java常见面试题总结

1. HashTable,HashMap,ConcurrentHashMap
- HashTable
	- 数组+链表实现，key和value都**不能为null**，线程安全，实现线程安全的方式是锁住整个HashTable，效率低，ConcurrentHashMap做了相关优化
	- 初始化size为11，扩容：newsize = oldsize*2+1，size始终为奇数，所以简单取模哈希的结果更加均匀
	- 计算index的方法：index = (hash & 0x7FFFFFFF) % tab.length，位运算的目的是得到一个正数
- HashMap
	- 数组+链表实现，key和value都**可以为null**，**线程不安全**
	- size初始化为16，扩容newsize = oldsize*2，size为2的n次幂
	- 扩容针对整个Map，每次扩容时，原来数组中的元素依次重新计算存放位置，并重新插入
	- 插入元素后才判断该不该扩容，有可能有无效扩容（插入后，如果扩容，但没有再次插入，可能产生无效扩容）
	- 当Map中元素总数超过Entry数组的75%，触发扩容操作，为了减小链表长度，元素分配更均匀
	- 计算index的方法：index = hash & (tab.length-1)
	- 为null的键只能有一个，可以有一个或多个键对应的值为null
	- 当链表大小超过阈值8时，链表就会被改造为树形结构（红黑树，在Java1.8中引入）
- HashMap的初始值还要考虑加载因子
	- 哈希冲突：若干key的哈希值按数组大小取模后，如果落在同一个数组下表下，将组成一条Entry链，对key的查找需要遍历真个链表的元素执行equals（）比较
	- 加载因子：为了降低哈希冲突概率，默认当HashMap中的键值对数达到数组大小的75%时，会触发扩容。即当预估容量是100时，需要设定100/0.75=134的数组大小
	- 空间换时间：如果希望加快Key查找的时间，还可以进一步降低加载因子，加大初始大小，以降低哈希冲突概率
- HashMap和HashTable包含如下属性
	- 容量(capacity)
	- 初始化容量(initial capacity)
	- 尺寸(size)：当前在hash表中记录的数量
	- 负载因子(load factor)
- ConcurrentHashMap
	- 底层采用分段数组+链表实现，线程安全
	- 通过把Map分为N个Segment，可以提供相同的线程安全，但是效率提升了N倍，默认提升16倍
	- ConcurrentHashMap允许多个修改操作并发进行，其关键在于使用了锁分离技术
	- 有些方法需要跨段，比如size()和containsValue()
	- 扩容：段内扩容（段内元素超过该段对应Entry数组长度的75%触发扩容，不会对整个Map进行扩容），插入前检测需不需扩容，避免无效扩容
- **锁分段技术**：首先将数据分成一段一段的存储，然后给每一段数据一把锁

2. 