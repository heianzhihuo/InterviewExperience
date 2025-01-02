# Python基础知识
1. 上下文管理器
上下文管理器（with语句）用于管理文件或数据库连接等资源，确保在使用或异常后进行关闭

```python
# 在退出代码块后，文件句柄自动关闭
with open("file.txt", "w") as file:
    file.write("Hello, World!")
```

2. Metaclasses控制类的产生以及行为，类的类，决定了类本身的行为方式


```python
class Meta(type):
    def __new__(cls, name, bases, dct):
        print(f"Creating class {name}")
        return super().__new__(cls, name, bases, dct)

class MyClass(metaclass=Meta):
    pass
```

3. 协程
协程通过使用 async 和 await 关键字暂停和恢复执行，从而实现异步编程。
```python
import asyncio

async def async_task():
    print("Task started")
    await asyncio.sleep(1)
    print("Task completed")

asyncio.run(async_task())
```

4. 抽象基类 
抽象基类（ABCs）来定义一个类的蓝图，并确保派生子类实现特定的方法。
```python
from abc import ABC, abstractmethod

class Animal(ABC):
    @abstractmethod
    def sound(self):
        pass

class Dog(Animal):
    def sound(self):
        return "Woof!"

dog = Dog()
```

5. Descriptors
Descriptors 管理属性的行为，并提供对获取、设置和删除属性值的细粒度控制。
```python
class Descriptor:
    def __get__(self, instance, owner):
        return instance._value

    def __set__(self, instance, value):
        instance._value = value * 2

class MyClass:
    attribute = Descriptor()

obj = MyClass()
obj.attribute = 10
print(obj.attribute)  # Output: 20
```
6.
7.
8.
9.
10. 自定义迭代器
自定义迭代器允许控制对象的迭代方式。
```python
class MyIterator:
    def __init__(self, max):
        self.max = max
        self.current = 0

    def __iter__(self):
        return self

    def __next__(self):
        if self.current < self.max:
            self.current += 1
            return self.current
        else:
            raise StopIteration

iterator = MyIterator(5)
for value in iterator:
    print(value)
```
