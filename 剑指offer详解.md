这里是所有剑指offer的题目总结，对各类算法题的思路进行总结

# 一 斐波那契数列
- 题目描述：输入一个整数n，求出斐波那契数列的第n项
- 问题分析：斐波那契数列的递推公式为f(n) = f(n-1)+f(n-2)。通过这个公式很容易想到两种方法，一种是递归，另一种是迭代方法。
- 显然递归方法的时间复杂度是O(2^n)，而迭代法的时间复杂度是O(n).
- 实例代码
- 迭代法
```java
	int Fibonacci(int n){
		if(n==0){
			return 0;
		}
		if(n==1 || n==2){
			return 1;
		}
		int a = 1,b = 1,c = 2;
		for(int i = 3;i<=n;i++){
			c = a + b;
			a = b;
			b = c;
		}
		return c;
	}
```
- 递归法
```java
	int Fibonacci(int n){
		if(n==0){
			return 0;
		}
		if(n==1 || n==2){
			return 1;
		}
		return Fibonacci(n-1)+Fibonacci(n-2);
	}
```

# 二 跳台阶问题
- 题目描述：一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级台阶总共有多少种跳法。
- 问题分析：
- 正向分析：有两种跳法，1阶或者5阶。那么假定第一次跳的是1阶，那么剩下n-1个台阶，跳法是f(n-1);假定第一次跳的是2阶，那么剩下n-2个台阶，跳法是f(n-2)。从而由上面可以总结跳法为:f(n)=f(n-1)+f(n-2)。
- 所以跳台阶问题实际是斐波那契数列问题。其中f(1)=1,f(2)=2,f(3)=3...
- 实例代码：
```java
	int jumpFloor(int n){
		if(n<=0){
			return 0;
		}
		if(n==1){
			return 1;
		}
		if(n==2){
			return 2;
		}
		int a = 1,b = 2,c = 0;
		for(int i=3;i<=n;i++){
			c = a+b;
			a = b;
			b = c;
		}
		return c;
	}
```

# 三 变态跳台阶问题
- 题目描述：一只青蛙一次可以跳上1级台阶，也可以跳上2级...它也可以跳上n级。求该青蛙跳上一个n级台阶总共有多少种跳法？
- 问题分析：
这题的思路和前两题是相同的。假设n>=2，第一步有n种跳法：跳1级、跳2级...跳n级；跳1级，则剩下n-1级，剩下的跳法是f(n-1)；跳2级，则剩下n-2级，剩下的跳法是f(n-2);...跳n-1级，剩下1级，则剩下的跳法是f(n-1);跳n级，剩下0级，则剩下的跳法是f(0)。所以在n>=2时，f(n)=f(n-1)+f(n-2)+...+f(1)。而f(n-1) = f(n-2)+f(n-3)+...+f(1)。所以f(n)=2*f(n-1)，又f(1)=1，所以得f(n)=2^(n-1)

- 示例代码：
```java
	int JumpFloorII(int n){
		return 1<<(n-1);
	}
```

# 四 二维数组中的查找
- 题目描述：在一个二维数组种，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。请完成一个函数，输入这样的一个二维数组和一个整数，判断数组中是否含有该整数。

- 问题分析：
这题还是比较简单的。矩阵时有序的，从左下角看，向上数字是递减的，向右数字是递增的，因此从左下角开始查找，当要找的数字比左下角数字大时，右移。当要查找的数字比左下角的数字小时，上移。这样查找的速度最快。（当然，从右上角开始是同理的，因为右上角向下数字是递增的，向左数字是递减的。）

- 代码示例（右上角）

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

# 五 替换空格
- 题目描述：题目描述：请实现一个函数，将一个字符串中的每个空格替换成“%20”。例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。
- 问题分析：这题很简单，可以通过循环判断字符串是否为空格，是则用append()方法追加"20%"，否则追加原字符.
或者用replaceAll(String regex,String replacement)方法

- 示例代码：

- 常规做法
```java
	public String replaceSpace(StringBuffer str){
		StringBuffer ret = new StringBuffer();
		for(int i = 0; i < str.toString().length();i++){
			char ch = str.charAt(i);
			if(String.valueOf(ch).equals(" ")){
				ret.append("%20");
			}else{
				ret.append(ch);
			}
		}
		return ret.toString();
	}
```

- 一行代码解决
```java
	public String replaceSpace(StringBuffer str){
		return str.toString().replace(" ","%20");
	}
```

# 六 数值的整数次方
- 题目描述：
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
- 问题解析：
这题的考点在于快速幂算法，次要考点在于double的比较。1. 当base=0且exponent<0时，会出现对0求倒数的情况，需要进行错误处理；2. 当base==0的判断，不能直接用==判断 3. 优化求幂函数(二分幂)。当n为偶数，a^n = (a^n/2)\*(a^n/2); 当n为奇数时，a^n=(a^n/2)\*(a^n/2)\*a。时间复杂度O(logn)。

- 代码示例
```java
	public class Solution{
		boolean invalidInput = false;
		public double Power(double base, int exponent){
			if(equal(base,0.0) && exponent<=0){
				invalidInput = true;
				return 0.0;
			}
			int n = exponent;
			if(exponent<0){
				n = -exponent;
			}
			double ret = getPower(base,n);
			if(exponent<0){
				ret = 1.0/ret;
			}
			return ret;
		}
		boolean equal(double n1, double n2){
			return n1-n2>-0.000001 && n1-n2<0.000001;
		}
		public double getPower(double a,int n){
			if(n==0){
				return 1.0;
			}
			if(n==1){
				return a;
			}
			double ret = getPower(a,n>>1);
			ret *= ret;
			if(n%1==1){
				ret *= a;
			}
			return ret;
		}
	}
```

# 七 调整数组顺序使奇数位于偶数前面
- 题目描述：
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变。

- 问题分析：
这道题解法很多种。思想很简单，新建一个长度为n的数组，先把奇数复制到新数组中，然后从该最后一个位置，开始复制偶数

- 示例代码：
```java
	public class Solution{
		public void reOrderArray(int[] array) {
			int n =  array.length;
			if(n==0 || n==1){
				return;
			}
			int[] tmp = new int[n];
			int i = 0;
			for(int j=0;j<n;j++){
				if(array[j]%2==1){
					tmp[i] = array[j];
					i++;
				}
			}
			for(int j=0;j<n;j++){
				if(array[j]%2==0){
					tmp[i] = array[j];
					i++;
				}
			}
			for(int j=0;j<n;j++){
				array[i] = tmp[i];
			}
		}
	}
```	

# 八 链表中倒数第k个节点
- 题目描述：
输入一个链表，输出该链表的倒数第k个节点

- 问题分析：
这题第一次看到的时候没有思路，想着先遍历一遍，计算节点数，再找倒数第k个节点。
思路总结：两个指针，一个指针p1先走k-1个节点后，另一个指针p2开始走，当p1到达最后时，p2指针所在的位置就是倒数第k个节点。

简单的理解和思路发散：这是一个典型的双指针问题，很多关于链表的题都可以用双指针解决，比如：找到链表中最中间的节点，判断一个链表是否有环等。

示例代码：
```java
/*
//链表节点类
public class ListNode {
	int val;
	ListNode next = null;
	
	ListNode(int val) {
		this.val = val;
	}
}
*/
//时间复杂度O(n)，一次遍历即可
public class Solution {
	public ListNode FindKthToFail(ListNode head,int k){
		ListNode pre = head, p = head;
		for(int i=0;i<k;i++){
			if(p==null)
				return null;
			p = p.next;
		}
		while(p!=null){
			p = p.next;
			pre = pre.next;
		}
		return pre;
	}
}
```

# 九 反转链表

- 题目描述：
输入一个链表，反转这个链表

- 问题分析：
对于熟悉指针的人来说，这是很简单的题。这道题考察的是头插法。

- 示例代码：
```java
/*
//链表节点类
public class ListNode {
	int val;
	ListNode next = null;
	
	ListNode(int val) {
		this.val = val;
	}
}
*/
//时间复杂度O(n)，一次遍历即可
public class Solution {
	public ListNode ReverseList(ListNode head){
		ListNode p = null;
		ListNode ret = null;
		while(head!=null){
			p = head.next;//保存当前未反转的链表的下个节点
			head.next = ret;//当前未反转链表的头向已反转链表的头部
			ret = head;
			head = p;
		}
		return ret;
	}
}
```

# 十 合并两个排序链表
- 题目描述：
输入两个递增的链表，输出两个链表合并和的链表，要求合并后的链表也是递增的。
- 问题分析：
这是典型的归并排序，二路归并

- 代码示例：

```java
/*
//链表节点类
public class ListNode {
	int val;
	ListNode next = null;
	
	ListNode(int val) {
		this.val = val;
	}
}
*/
public class Solution{
	public ListNode Merge(ListNode list1,ListNode list2){
		ListNode ret = new ListNode(-1);//增减额外的空节点
		ListNode p = ret;
		while(list1!=null || list2!=null){
			if(list1==null || (list2!=null && list2.val<=list1.val)){
				p.next = list2;
				list2 = list2.next;
				p = p.next;
			} else {
				p.next = list1;
				list1 = list1.next;
				p = p.next;
			}
		}
		return ret.next;
	}
}
```

# 十一 用两个栈实现队列
- 题目描述：
用两个栈来实现队列，完成队列的push和pop操作。队列中的元素为int类型。

- 问题分析：
这题考察的是数据结构的基础。在数据结构中，用两个栈来实现队列，这是很常规的操作。栈的特点是：先进后出，队列的特点是，先进先出。用两个栈实现队列的核心在于，在一个栈中入，而在另一个栈中出。

- 示例代码：

```java
import java.util.Stack;
public class Solution {
	Stack<Integer> stack1 = new Stack<>();
	Stack<Integer> stack2 = new Stack<>();
	//
	public void push(int v){
		stack1.push(v);
	}
	
	public int pop() {
		if(stack1.isEmpty() && stack2.isEmpty()) {
			throw new RuntimeException("Queue is empty!");
		}
		if(stack2.isEmpty()){
			while(!stack1.isEmpty()){
				stack2.push(stack1.pop());
			}
		}
		return stack2.pop();
	}
	
}
```

# 十二 栈的压入弹出序列

- 题目描述：
输入两个整数序列，第一个序列表示栈的入栈顺序，请判断第二个序列是否为该栈的出栈顺序。假设入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的入栈顺序，序列4,5,3,2,1是该栈的一个出栈序列，但4,3,5,1,2就不可能是该栈的弹出序列。

- 题目分析：
这题的思路比较难想到，如果之前没遇到过或者不熟悉可能没法解决。
思路：借用一个辅助的栈，遍历压栈顺序，判断栈顶元素是不是出栈顺序的第一个元素，是则出栈，不是则入栈，直到相等后开始出栈，这样循环完入栈序列后，如果辅助栈不为空，说明出栈序列不是该栈的弹出顺序。

```java
import java.util.ArrayList;
import java.util.Stack;
public class Solution {
	public boolean isPopOrder(int[] pushA,int[] popA){
		if(pushA.length==0 || popA.length==0)
			return false;
		Stack<Integer> stack = new Stack<>();
		int j = 0;
		for(int i=0;i<pushA.length;i++){
			stack.push(pushA[i]);
			while(!stack.isEmpty() && stack.peek()==popA[j]){
				stack.pop();
				j++;
			}
		}
		return stack.isEmpty();
	}
}

```


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


