
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


