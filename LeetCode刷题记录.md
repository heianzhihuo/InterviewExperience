# LeetCode刷题记录和总结

为了简单表示，不再复制题目，我简述题目要求即可

## [Longest Word in Dictionary](https://leetcode.com/problems/longest-word-in-dictionary/)
- 字典中最长单词
- 给定一个字符串列表words，找到words中最长（如果长度相同，则返回字典序小）的字符串，该字符串每次能通过words中的某个字符串增加一个字符得到
- 刚开始我的思路是按字典序排序，然后判断某个位置的字符串的前一个位置是否是它的前缀，但是这样的结果不对
- 然后看解析发现用的是字典树，即Trie树，于是写了一个用HashMap作为索引的字典树，查找用的BFS，结果只超过37%的submit
- 然后看最优解，用长度为26的数组来保存子节点，查找用的是DFS

```java
	/*720. Longest Word in Dictionary*/
    public String longestWord(String[] words) {
        Trie trie = new Trie();
        for(String w:words)
            trie.insert(w);
        trie.dfs(trie.root,0);
        return trie.res;
    }
    class TrieNode{
        String word = "";
        boolean isWord = false;
        TrieNode children[] = new TrieNode[26];
    }
    class Trie{
        TrieNode root = new TrieNode();
        public void insert(String word){
            TrieNode cur = root;
            for(char c:word.toCharArray()){
                int i = c-'a';
                if(cur.children[i]==null){
                    TrieNode newNode = new TrieNode();
                    cur.children[i] = newNode;
                    cur = newNode;
                }else{
                    cur = cur.children[i];  
                }
            }
            cur.isWord = true;
            cur.word = word;
        }
        int max = -1;
        String res = "";
        public void dfs(TrieNode node,int length){
            for(int i=0;i<26;i++){
                if(node.children[i]!=null && node.children[i].isWord)
                    dfs(node.children[i],length+1);
            }
            if(length>max){
                max = length;
                res = node.word;
            }
        }
    }
```

## [300. Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence/)
- 最长递增子串

```java
	public int lengthOfLIS(int[] nums) {
        int n = nums.length;
        if(n==0)
            return 0;
        int result[] = new int[n];
        // result[0] = 1;
        int i,j;
        for(i=0;i<n;i++){
            result[i] = 1;
            for(j=0;j<i;j++)
                if(nums[i]>nums[j] && result[j]+1>result[i])
                    result[i] = result[j]+1;
        }
        int len = 0;
        for(i=0;i<n;i++)
            if(result[i]>len)
                len = result[i];
        return len;
    }
```
 
## [128. Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/submissions/)
- 最长连续元素序列
- 给定一个无序数组，求这个数组中元素能够组成的最长的连续整数的长度
- 示例：
> Input:[100,4,200,1,3,2,1]\
Output:4\
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.

- 这使LeetCode中一道Hard的题，但是思路很简单，只需要对数组进行排序，然后按照思路处理即可，时间复杂度是O(nlogn)的，用排序的结果超过100%的提交
- 唯一的疑问在于题目要求复杂度是O(n)
- 看题解有三种思路
	- 方法一：暴力搜索，时间复杂度O(n^3)
	- 方法二：排序，时间复杂度O(nlogn)
	- 方法三：HashSet，这里说时间复杂度是O(n)的，在方法3的代码中就是把数组中元素扔到HashSet中，然后遍历HashSet中的元素，方法和排序后判断方式基本相同。显然这里有一个前提假设是HashSet的插入是O(1)的，因此这里说时间复杂度为O(1)。然后我在看结果中的代码，实际上排序方法更快。
	
```java
	/*128. Longest Consecutive Sequence*/
    public int longestConsecutive(int[] nums) {
        if(nums.length==0)
            return 0;
        Arrays.sort(nums);
        int cur,pre;
        int max = 1;
        int i;
        pre = nums[0];
        cur = pre+1;
        for(i=1;i<nums.length;i++){
            if(nums[i]==cur)
                cur++;
            else if(nums[i]>cur){
                if(cur-pre>max)
                    max = cur-pre;
                pre = nums[i];
                cur = pre+1;
            }
        }
        if(cur-pre>max)
            max = cur-pre;
        return max;
    }
```

## [395. Longest Substring with At Least K Repeating Characters](https://leetcode.com/problems/longest-substring-with-at-least-k-repeating-characters/)
- 求最长子串，在该子串中每个字符都至少出现K次
- 我的思路找到该串中出现次数少于K次的字符，将这些字符移除后，形成了若干子串，在子串中递归求解子问题
- 第一次实现时用的是HashMap，得到的结果是只超过60%的submits，然后改成了长度为26的数组，同时先判断是否所有字符出现的次数都大于等于k
```java
	/*395. Longest Substring with At Least K Repeating Characters*/
	public int longestSubstring(String s, int k) {
        int n = s.length();
        if(k==0)
            return n;//如果k<=1，显然任意字符串都成立
        if(n<k)
            return 0;//如果字符串长度小于K，没有这样的子串
        int count[] = new int[26];
        int i;
        for(i=0;i<n;i++)
            count[s.charAt(i)-'a']++;
        boolean flag = true;
        for(i=0;i<26;i++)
            if(count[i]>0 && count[i]<k){
                flag = false;
                break;
            }
        if(flag) return n;
        int pre=0,cur = 0,ret = 0;
        while(cur<n){
            if(count[s.charAt(cur)-'a']<k){
                ret = Math.max(ret,longestSubstring(s.substring(pre,cur),k));
                pre = cur+1;
            }
                
            cur++;
        }
        ret = Math.max(ret,longestSubstring(s.substring(pre,cur),k));
        return ret;
    }
```

## [524. Longest Word in Dictionary through Deleting](https://leetcode.com/problems/longest-word-in-dictionary-through-deleting/)
- 给定字符串s，和字符串集合d，求d一个字符串，该字符串能通过s删除若干字符得到，并且该字符串长度最长，且如果有相同长度的符号条件的字符串，则返回其中字典序最小的那个字符串
- 刚开始的错误的以为直接统计字符串中字符出现次数，比较字符串的字符出现次数
- 然后马上想到，其实就是找集合d中是字符串s的子串的字符串，并且使得该字符串长度最长字典序最小
- 判断字符串t是否是字符串s的子串的方法：从字符串t的第0个位置的字符ch，找字符ch在s中最早出现的位置，然后从该位置开始，找t[1]最早出现的位置
- 所以依次遍历d中每个字符串t，判断t是否是s的子串，再判断t是否长度大于最优解ret的长度，或者t的长度等于ret长度，且t的字典序比ret的字典序小。其中后面的条件可以放在前面。

```java
	/*524. Longest Word in Dictionary through Deleting*/
	public String findLongestWord(String s, List<String> d) {
        String ret = "";
        for(String t:d){
            if((t.length()>ret.length() || (t.length()==ret.length() && t.compareTo(ret)<0)) && isSubSequence(s,t))
                ret = t;
        }
        return ret;
    }
    public boolean isSubSequence(String s,String t){
        int i,j;
        j = 0;
        for(i=0;i<t.length();i++,j++){
            for(;j<s.length() && s.charAt(j)!=t.charAt(i);j++);
            if(j==s.length())
                return false;
        }
        return true;
    }
```

## [409. Longest Palindrome](https://leetcode.com/problems/longest-palindrome/)
- 最长回文串，给定一个字符串s，求用s中字母能组成的最长回文串的长度
- 思路：统计字符串s中出现次数为奇数次的字母的个数
```java
	/*409. Longest Palindrome*/
    public int longestPalindrome(String s) {
        int count[] = new int[128];
        int i;
        for(i=0;i<s.length();i++)
            count[s.charAt(i)]++;
        int result = 0;
        for(i=0;i<128;i++)
            if(count[i]%2==1)
                result++;
        if(result!=0)
            return s.length()-result+1;
        return s.length();
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


## [1046. Last Stone Weight](https://leetcode.com/problems/last-stone-weight/)
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


## [218. The Skyline Problem](https://leetcode.com/problems/the-skyline-problem/)
- 三元组[Li,Ri,Hi]，Li和Ri为横坐标，Hi为高度，即x轴和Li到Ri范围内围成了一个矩形
- 给了多个这样的三元组，求这些三元组围成的矩形的轮廓，给定的三元组按Li从小到大排列
- ![图片展示](https://leetcode.com/static/images/problemset/skyline1.jpg "矩形")
- ![图片展示](https://leetcode.com/static/images/problemset/skyline2.jpg "矩形轮廓的点")

提示是栈、分治、分段树，个人想法是和凸包问题相似
一个思路，先把所有矩形的四个顶点找出来，以x和y排序，还是没有思路，未完成

