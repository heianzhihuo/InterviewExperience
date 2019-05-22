# LeetCode刷题记录和总结

为了简单表示，不再复制题目，我简述题目要求即可

## 1046. Last Stone Weight
> We have a collection of rocks, each rock has a positive integer weight.
> Each turn, we choose the two heaviest rocks and smash them together.  Suppose the stones have weights x and y with x <= y.  The result of this smash is:

> - If x == y, both stones are totally destroyed;

> - If x != y, the stone of weight x is totally destroyed, and the stone of weight y has new weight y-x.

> At the end, there is at most 1 stone left.  Return the weight of this stone (or 0 if there are no stones left.)

题目的意思是给定一个正整数组stones[]，每次从数组中选择两个最大的数x和y：如果x和y相等，则这两个数同时从数组中删除；如果x和y不等，则删除这两个数后，数组中留下y-x。

这题考察的数据结构中的**最大堆**，说来惭愧的是，手撸一个最大堆还要半个小时，还有各种bug，对基本的数据结构的实现不够熟练，还是一个菜鸡。

最大堆的本质是一棵完全二叉树，堆顶(即根节点)元素值比左右儿子的元素值大，最小堆则是根节点元素比左右儿子的元素值小。

最大堆和最小堆常用数组实现。A[i]的左右儿子分别是A[(i+1)*2-1)]和A[(i+1)*2]，其父节点是A[(i-1)/2)]。

最大堆的插入元素x：实际插入元素在数组末尾，即完全二叉树的最后一个节点n，自下向上比较，x与其父节点比较，如果比父节点大，则父节点元素下移，直到其父节点值比x大为止，则将元素x赋值给当前节点。

最大堆删除堆顶元素：实际删除的元素仍然在数组末尾，也是完全二叉树的最后一个节点n。交换最后一个节点和根节点的值，设最后一个节点值位x，自上而下比较，如果x小于其儿子中较大的那个，则将其较大的儿子上移，直到x大于其两个子节点为止。

最小堆的插入和删除和最大堆同理，总结就是插入和删除操作都是在数组尾部进行的，不同的是，插入是自下而上调整堆，删除是自上而下调整堆。

```java
/*手撸Java最大堆*/
class MaxHeap{
	List<Integer> heap = new ArrayList<>();
	
	void insert(int v){
		int i = heap.size();
		heap.add(v);
		while(i>0){
			int j = (i-1)/2;
			if(heap.get(j)>=v)
				break;
			heap.set(i,heap.get(j));
			i = j;
		}
		heap.set(i,v);
	}
	
	int pop(){
		int ret = heap.get(0);
		int n = heap.size()-1;
		int v = heap.get(n);
		heap.remove(n);
		int i = 0;
		int j = (i+1)*2-1;
		while(j<n){
			if(heap.get(j+1)>heap.get(j))
				j++;
			if(heap.get(j)<=v)
				break;
			heap.set(i,heap.get(j));
			i = j;
		}
		heap.set(i,v);
		return ret;
	}
}
```

## [378. Kth Smallest Element in a Sorted Matrix](https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix/)
- 找出有序NxN矩阵中第k小的数，其中矩阵中的数从上往下，从左到右是递增的
- 思想：**分治**,**二分查找**
- 在有序数组中找第k小的数的方法是按数组的中位数将数组划分成两部分
- 这里的分治则是按照极大值和极小值的中间的数将数组划分成两部分
```java
	/*378. Kth Smallest Element in a Sorted Matrix*/
    public int kthSmallest(int[][] matrix, int k) {
        int n = matrix.length;
        int low = matrix[0][0],high = matrix[n-1][n-1];
        while(low<=high){
            int mid  = low+(high-low)/2;//注意：这里不能用(low+high)/2，因为溢出了
            int count =  getLessEqual(matrix,mid);
            if(count<k) low = mid +1;
            else high = mid -1;
        }
        return low;
    }
    int getLessEqual(int[][] matrix,int val){
        int n = matrix.length;
        int i = n-1,j = 0,res = 0;
        while(i>=0 && j<n){
            if(matrix[i][j]>val) i--;
            else{
                res += i+1;//第j列的matrix[0][j]...matrix[i][j]都比val小
                j++;
            }
            
        }
        return res;
    }
```

## [218. The Skyline Problem](https://leetcode.com/problems/the-skyline-problem/)
- 三元组[Li,Ri,Hi]，Li和Ri为横坐标，Hi为高度，即x轴和Li到Ri范围内围成了一个矩形
- 给了多个这样的三元组，求这些三元组围成的矩形的轮廓，给定的三元组按Li从小到大排列
- ![图片展示](https://leetcode.com/static/images/problemset/skyline1.jpg "矩形")
- ![图片展示](https://leetcode.com/static/images/problemset/skyline2.jpg "矩形轮廓的点")

提示是栈、分治、分段树，个人想法是和凸包问题相似
一个思路，先把所有矩形的四个顶点找出来，以x和y排序，还是没有思路，未完成

