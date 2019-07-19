这里是所有剑指offer的题目总结

# 1 二进制中1的个数
- 题目描述：输入一个整数，输出该数二进制表示中1的个数。其中负数用补码表示。
- 思路：最简单的思路，整数n每次无符号右移一位，同1做&运算，判断最后是否为1
- 剑指offer的思路，n&(n-1)操作可以把最右边的1变成0，可以判断进行了多少次这样的操作即可
- C++的SSE4指令——popcnt，_mm_popcnt_u32，用于统一32位的整数中1的位数
```java
	public int NumberOf1(int n) {
		int count = 0;
		//方法一
		/*
		 * while(n!=0) { count+=n%2; n/=2;//这里可以不用位移 }
		 */
		// 方法二,注意方法2在本质上和方法一的时间效率是一样的
		while (n != 0) {
			n = n & (n - 1);// 作用，把n最右边的1变0
			count++;
		}
		return count;
	}
```

# 2 二进制中0的个数
- 题目描述：
- 思路：每次右移一位，判断是否是0即可
```java
	public int findZero(int n) {
		int count = 0;
		while (n != 0) {
			count += (1 - n % 2);// 注意，用取余没有问题
			n /= 2;
		}
		return count;
	}
```

# 3 二进制

# 二维数组中的查找
- 题目描述：再一个二维数组中（每一维数组的长度相同），每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。
- 思路：从右上角开始，若小，则向下走，删除一行；若大，则向左走，删除一行

```java
	public boolean Find(int target, int [][] array) {
        int row = 0;
        int col = array[0].length-1;
        while(row<array.length && col>=0){
            if(target>array[row][col])
                row++;
            else if(target<array[row][col])
                col--;
            else return true;
        }
		
		return false;
    }
```

# 替换空格
- 题目描述：请实现一个函数，将一个字符串中的每个空格替换成“%20”。例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。


