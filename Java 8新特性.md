#Java 8新特性详解

以前从来没有关注过Java各个版本的特性，之前面试的时候被问到过Java版本的问题，现在，今天忽然发现了这个问题就在这里随便写一点，只是当个搬运工

## 关于Java版本
刚才在看HashMap和HashTable的时候，发现一个小小的问题，有的博客说的版本是Java 5，有的版本是Java 1.8，所以到底是1.8的版本是高还是5的版本高？
- 其实上面是有问题的，准确的说不是Java 1.8，而应该是JDK 1.8，又称为Java 8。也就是说Java 5实际上是JDK 1.5，因此Java 8的版本比Java 5的版本更高
- JDK全称是Java Development Kit，中文是Java开发工具包，是整个Java开发的核心，集成了jre和一些Java基础的类库
- JRE全称是Java Runtime Environment，中文是Java运行时环境。主要包含两部分jvm的标准实现和Java的一些基本类库
- JVM全称时Java Virtual Machine，Java虚拟机，可以识别class文件中的字节码指令，并调用操作系统向上的API完成相关的功能，是Java能够跨平台的核心。针对不同的操作系统有不同的jvm实现，从而实现Java的跨平台运行
- JDK>JRE>JVM

上面这些是初学Java时需要了解的，很多内容都忘记了

## 关于Java 8
Java 8是Java语言开发的一个主要版本，由Oracle公司于2014年3月18日发布。是Java自Java 5(发布于2004年)之后的最重要的一个版本。这个版本包含语言、编译器、库和JVM等方面的十多个特性。虽然这个版本已经发布很久了，有必要自己总结一下Java 8的各种新特性。

上面的都是废话，下面的才重点。

## Java 8的新特性
1. **Lambda表达式**
- Lambda表达式，也称为闭包，**允许把函数作为一个方法的参数**(函数作为参数传递进方法中)，使用Lambda表达式可以使代码更简洁紧凑
- Lambda表达式语法格式如下：
```
(parameters) -> expression
或
(parameters) -> {statements;}
```
- Lambda表达式的重要特征：
	- **可选声明类型**：不需要声明参数类型，编译器可以统一识别参数
	- **可选的参数圆括号**：一个参数无需定义圆括号，但多个参数需要定义圆括号
	- **可选的大括号**：如果主体包含了一个语句，就不需要使用大括号
	- **可选的返回关键字**：如果主体只有一个表达式返回值则编译器会自动返回值，大括号需要指定返回了一个数值
- Lambda表达式实例：
```java
// 1. 不需要参数，返回值为5
() -> 5
// 2. 接受一个参数(数字类型)，
x -> 2*x
// 3. 接受两个参数
(x,y) -> x-y
// 4. 接受两个int参数
(int x,int y) -> x+y
// 5. 接受一个参数，并在控制台打印
(String s)->System.out.println(s)
```
- 测试
```java
public class Test{
	public static viod main(String args[]){
		Test test = new Test();
		MathOperation addtion = (a,b)->a+b;
		//等价于，上面的定义相比于下面更为简洁
		MathOperation add = new MathOperation() {
			@Override
			public int operation(int a, int b) {
				// TODO Auto-generated method stub
				return a+b;
			}
		};
		
		MathOperation subtraction = (int a,int b)->a-b;
		MathOperation multiplication = (a,b)->{return a*b;};
		MathOperation division = (a,b)->a/b;
		System.out.println("10+5="+test.operate(10,5,addtion));
		System.out.println("10+5="+test.operate(10,5,add));
		//下面这个和上面的可以等价
		System.out.println("10+5="+test.operate(10,5,(a,b)->a+b));
		System.out.println("10-5="+test.operate(10,5,subtraction));
		System.out.println("10*5="+test.operate(10,5,multiplication));
		System.out.println("10/5="+test.operate(10,5,division));
		
		GreetingService greeetService1 = message->System.out.println("Hello"+message);
		
		greeetService1.sayMessage("Google");
	}
	interface MathOperation{
		int operation(int a,int b);
	}
	interface GreetingService{
		void sayMessage(String message);
	}
	int operate(int a,int b,MathOperation mathOperation){
		return mathOperation.operation(a,b);
	}
}
```
- Lambda表达式只能引用标记了final的外层局部变量，不能在Lambda表达式内部修改定义在域外的局部变量
- Lambda表达式的局部变量可以不用声明为final，但必须不可被后面的代码修改
- Lambda表达式中不允许声明一个与局部变量同名的参数或局部变量

Java8的Lambda表达式其实和python的Lambda表达式是类似的，其本质是匿名函数。在实际中，我写的代码几乎没有涉及到Lambda表达式，同时在Java中也很少用到interface、final、abstract等关键字，对接口、抽象类的理解不是很深刻。

2. 方法引用
- 方法引用通过用方法的名字来指向一个方法。方法引用可以使语言的构造更紧凑简洁，减少冗余代码。下面是Car类定义了4个方法，来区分4种不同方法的引用。
```java
@FunctionalInterface
interface Supplier<T>{
	T get();
}
class Car{
	//Supplier是Java8中的函数式接口，这里和lambda一起使用了
	public static Car create(final Supplier<Car> supplier){
		return supplier.get();
	}
}
```
- 方法引用使用一对冒号::
	- 构造器引用：Class::new，或者更一般的Class<T>::new，实例如下：
	```java
	final Car car = Car.create(Car::new);
	final List<Car> cars = Arrays.asList(car);
	```
	- 静态方法引用：语法是Class::static_method
	```java
	cars.forEach(Car::collide);
	```
	- 特定类的任意对象的方法引用，语法是Class::method
	```java
	cars.forEach(Car::repair);
	```
	- 特定对象的方法引用，语法是instance::method
	```java
	final Car police = Car.create(Car::new);
	cars.forEach(police::follow);
	```
- 方法引用实例
```java
import java.util.List;
import java.util.ArrayList;
public class Test{
	public static void main(String args[]){
		List names = new ArrayList();
		
		names.add("Google");
		names.add("Runoob");
		names.add("Taobao");
		names.add("Baidu");
		names.add("Sina");
		
		names.forEach(System.out::println);
	}
}
```

3. 函数式接口
- 函数式接口就是仅有一个抽象方法，但是可以有多个非抽象方法的接口，函数式接口可以转换为Lambda表达式。可以用Lambda表达式来实现这个接口
- Java8提供了一个特殊的注解@FunctionalInterface，Java库中所有相关接口都已经有该注解了
- 函数式接口中允许定义默认方法，静态方法，java.lang.Object里的public方法
```java
@FunctionalInterface
interface GreetingService{
	void sayMessage(String message);
	@Override
	boolean equals(Object obj);
	default void sayHello() {
		System.out.println("Hello");
	}
	static void printHello() {
		System.out.println("Hello");
	}
}

GreetingService greeetService1 = message->System.out.println("Hello "+message);
```
- Java已有的函数式接口（部分）
	- java.lang.Runnable
	- java.util.Comparator
	- java.io.FileFilter
	- java.nio.file.PathMatcher
	- java.util.concurrent.Callable
	- java.awt.event.ActionListener
	- java.lang.reflect.InvocationHandler
	- java.util.function（JDK1.8新增的）

由上面这个列表中可知，实现比较器的代码更简单了
```
Class Person{
	String name;
	int age;
}
Comparator ageComparator<Person> = (a,b)->(a.age-b.age);
//原来的实现方式如下，这是我最常用的方法
//另一种方法是在Person类中实现Comparable接口
Comparator<Person> ageComparator1 = new Comparator<Person>() {
	@Override
	public int compare(Person o1, Person o2) {
		// TODO Auto-generated method stub
		return o1.age-o2.age;
	}

};
```

4. 接口的默认方法和静态方法
- 接口可以有已经实现的方法，不需要实现类去实现其方法。当需要修改接口的时候，需要修改全部实现了该接口的类。默认方法可以使得开发者在不破坏二进制兼容性的前提下，往接口中添加新的方法。
- 默认接口和静态方法的实例在上文中见interface GreetingService

5. 重复注解
- Java5引入了注解特性，但是Java5中的注解在同一个地方只能使用一次。Java 8打破了这个限制
- Java5提供了3个内置注解
	- @Override 意思是重写、覆盖，最长用到的注解
	- @Deprecated 意思是废弃的、过时的
	- @SuppressWarning 意思是压缩警告，偶尔会看到

除了@Override之外，其它的注解几乎没有用到，而且@Override也是自动生成的，因此注解特性以及重复注解不怎么关注

6. Stream

7. Optional类
8. Date/Time API
9. Base64
