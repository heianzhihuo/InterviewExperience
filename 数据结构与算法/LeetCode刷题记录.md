# LeetCode刷题记录和总结

为了简单表示，不再复制题目，仅简述题目要求

## [191](https://leetcode-cn.com/problems/number-of-1-bits/) 位1的个数

- 题目描述：

编写一个函数，输入是一个无符号整数，返回其二进制表达式中数字位数为 ‘1’ 的个数（也被称为汉明重量）。

- 问题分析：

这题看起来很简单，刚开始想用取余的方法来判断最低位是否是1，然后n右移。结果这种方式不对，因为Java中的移位运算都是有符号移动。为了解决这个问题，用一个mask来表示其中的某一位，让mask不断左移，并且mask和n取与，判断是否为1.

- 示例代码：

```java
public class Solution {
    // you need to treat n as an unsigned value
    public int hammingWeight(int n) {
        int bits = 0;
        int mask = 1;
        for(int i=0;i<32;i++){
            if((n & mask)!=0){
                bits++;
            }
            mask <<= 1;
        }
        return bits;
    }
}
```


## [337](https://leetcode-cn.com/problems/house-robber-iii/) 打家劫舍 III

- 题目描述：

在上次打劫完一条街道之后和一圈房屋后，小偷又发现了一个新的可行窃的地区。这个地区只有一个入口，我们称之为“根”。 除了“根”之外，每栋房子有且只有一个“父“房子与之相连。一番侦察之后，聪明的小偷意识到“这个地方的所有房屋的排列类似于一棵二叉树”。 如果两个直接相连的房子在同一天晚上被打劫，房屋将自动报警。

计算在不触动警报的情况下，小偷一晚能够盗取的最高金额。

- 问题分析：

这题和前面的打家劫舍思路类似，不同的在于这题是在二叉树上打家劫舍。对于二叉树常用的操作是递归，因此，对于一棵二叉树，分成两种情况，根节点被打劫时的最大值，根节点不被打劫最大值。其中，根节点被打劫，则两个子节点的根节点不能被打劫。根节点不被打劫，则有4种情况，左右子节点根节点都被打劫，左右子节点中有一个节点没有被打劫，或者左右两个节点都不被打劫。

- 示例代码：
```java
/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {
    public int rob(TreeNode root) {
        int[] ret = robs(root);
        return Math.max(ret[0], ret[1]);
    }
	
	public int[] robs(TreeNode root) {
		int[] ret = new int[2];
		if(root==null)
			return ret;
		int[] left = robs(root.left);
		int[] right = robs(root.right);
		ret[0] = left[1]+right[1]+root.val;//表示打劫根节点,
		ret[1] = Math.max(left[1]+right[0], left[0]+right[1]);//不打劫根节点
		ret[1] = Math.max(ret[1], left[1]+right[1]);
        ret[1] = Math.max(ret[1], left[0]+right[0]);
		return ret;
	}
}
```

## [1028] 从先序遍历还原二叉树
- 题目描述：

从二叉树的根节点 root 开始进行先序遍历。
在遍历中的每个节点处，我们输出 D 条短划线（其中 D 是该节点的深度），然后输出该节点的值。（如果节点的深度为 D，则其直接子节点的深度为 D + 1。根节点的深度为 0）。
如果节点只有一个子节点，那么保证该子节点为左子节点。
给出遍历输出 S，还原树并返回其根节点 root。

- 问题解析：

这题在LeetCode中是一道hard的题，但是我认为这题其实只有mid难度。熟悉二叉树相关内容的，这题很容易想到用栈来保存二叉树当前遍历的序列，同时栈的深度可以表示每个节点的深度。从而，处理一个未处理节点时，获取其深度后，如果其深度比栈深度小，则出栈，直到栈深度和节点一样，然后让这个节点进栈。

- 示例代码：
```java
/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     int val;
 *     TreeNode left;
 *     TreeNode right;
 *     TreeNode(int x) { val = x; }
 * }
 */
class Solution {
    public TreeNode recoverFromPreorder(String S) {
        if(S.length()==0)
			return null;
        Stack<TreeNode> stack = new Stack<>();
        int i = 0,j;
        while(i<S.length()) {
        	j = i;
        	while(S.charAt(i)=='-') i++;
        	int d = i-j;
        	boolean flag = stack.size()>d;
        	while(stack.size()>d)
        		stack.pop();
        	while(i<S.length() && S.charAt(i)!='-') i++;
        	int val = Integer.valueOf(S.substring(j+d,i));
        	TreeNode tmp = new TreeNode(val);
        	if(flag) {
        		stack.peek().right = tmp;
        	}else if(!stack.isEmpty()) {
        		stack.peek().left = tmp;
        	}
        	stack.push(tmp);
        }
        return stack.get(0);
        
    }
}
```


## [60. Permutation Sequence](https://leetcode.com/problems/permutation-sequence/)
- 集合[1,2,3,...,n]有n!个不同的排列
- 给定n，求集合中按字典序的第k个排列
- 思路：这题有点难想到，我最先想到的办法是用下面的nextPermeation的方法求，但是这种方法效率很低
- 更好的思路是：首先，以任意一个元素开头的排列数都是(n-1)!个
	- 以1开头，K最小为1，最大为(n-1)!-1
	- 以2开头，K最小为(n-1)!，最大为2*(n-1)!-1
	...
	- 同理，以n开头，K最小为(n-1)*(n-1)!，最大为n!-1
- 通过上面的思路，可以先确定第K个排列的开头为(K-1)/(n-1)!+1
- 然后继续再剩下的元素中，找到开头的元素
```java
	public String getPermutation(int n, int k) {
        int f[] = new int[n+1];
        int s = 1;
        f[0] = 1;
        int i;
        for(i=1;i<=n;i++){
            s*=i;
            f[i] = s;
        }
        if(k>s)
            return null;
        List<Integer> list = new ArrayList<>();
        for(i=0;i<n;i++)
            list.add(i+1);
        StringBuilder sb = new StringBuilder();
        for(i=1;i<=n;i++){
            int index = (k-1)/f[n-i];
            sb.append(list.get(index));
            list.remove(index);
            k = k-index*f[n-i];
        }
        return sb.toString();
    }
```

## [31. Next Permutation](https://leetcode.com/problems/next-permutation/)
- 下一个排列
- 找到数组num的下一个字典序比num大的排列，如果num是字典序最大的排列，则返回字典序最小的排列
- 要求必须在内部交换，用常数的内存
- 思路：从数组最右边开始查找，第一个下降的位置，然后找到该位置左侧的所有数中第一个比这个数大的数，交换这两个数，然后再将左侧的数逆序，即可得下一个排列
```java
	public void nextPermutation(int[] nums) {
        int n = nums.length;
        if(n<=1)
            return;
        int i,j;
        for(i=n-1;i>=1;i--)
            if(nums[i]>nums[i-1])
                break;
        if(i!=0){
            for(j=n-1;j>i;j--)
                if(nums[j]>nums[i-1])
                    break;
            int tmp = nums[j];
            nums[j] = nums[i-1];
            nums[i-1] = tmp;
        }
        for(j=0;i+j<n-j-1;j++){
            int tmp = nums[i+j];
            nums[i+j] = nums[n-j-1];
            nums[n-j-1] = tmp;
        }
        return;
    }
```

## [26. Remove Duplicates from Sorted Array](https://leetcode.com/problems/remove-duplicates-from-sorted-array/submissions/)
- 移除有序数组中的重复数字，要求不用额外空间，O(n)时间
- 这是一道很简单的题目，只需要用两个位置标记i,j，j表示正在处理位置，在j之后的还未处理，而i是已经处理的，即到位置i的所有数都是不重复的

```java
	public int removeDuplicates(int[] nums) {
        int n = nums.length;
        if(n==0)
            return n;
        int i = 0,j;
        for(j=1;j<n;j++)
            if(nums[j]!=nums[i]){
                i++;
                nums[i] = nums[j];
            }
        return i+1;
    }
```

## [80. Remove Duplicates from Sorted Array II](https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/)
- 移除有序数组中的重复数字，保证每个数字至多出现两次，要求不用额外空间，O(n)时间
- 这题和上一题很类似，只需要再用一个flag标记变量表示当前数字是否出现过
```java
	public int removeDuplicates(int[] nums) {
        int n = nums.length;
        if(n==0)
            return n;
        int i=0,j,flag = 1;
        for(j=1;j<n;j++)
            if(nums[j]!=nums[i]){
                i++;
                nums[i] = nums[j];
                flag = 1;
            }else if(flag==1){
                i++;
                nums[i] = nums[j];
                flag = 2;
            }
        return i+1;
    }
```


## [514. Freedom Trail](https://leetcode.com/problems/freedom-trail/)
- 在电子游戏中，给定一个字符串ring，另一个字符串key表示需要拼写的字符字符串
- ring的第一个字符在12点方向，字符依次顺时针排列成一个圆
- 每次可以顺时针或者逆时针旋转一次，使得字符移动一个位置
- 求最小的操作次数，使得key中的每个字符能按次序出现在12点的位置
- 动态规划问题
```java
	public int findRotateSteps(String ring, String key) {
        nn = ring.length();
        char chs[] = ring.toCharArray();
        List<Integer> pos[] = new List[26];
        for(int i=0;i<26;i++)
            pos[i] = new ArrayList<>();
        for(int i=0;i<n;i++)
            pos[chs[i]-'a'].add(i);
        HashMap<Integer,Integer> cur = new HashMap<>();
        cur.put(0,0);
        chs = key.toCharArray();
        for(int i=0;i<chs.length;i++){
            HashMap<Integer,Integer> next = new HashMap<>();
            for(int x:pos[chs[i]-'a']){
                next.put(x,Integer.MAX_VALUE);
                for(int y:cur.keySet()){
                    int t = cur.get(y)+distance(x,y);
                    if(t<next.get(x))
                        next.put(x,t);
                }
            }
            cur = next;
        }
        int ret = Integer.MAX_VALUE;
        for(int c:cur.values())
            if(c<ret) ret = c;
        return ret+key.length();
    }
    int nn;
    int distance(int a,int b){
        int z = Math.abs(a-b);
        return Math.min(z,nn-z);
    }
```

## [691. Stickers to Spell Word](https://leetcode.com/problems/stickers-to-spell-word/)

```java
	public int minStickers(String[] stickers, String target) {
        if(stickers.length==0)
            return -1;
        HashMap<Character,Integer> tar = new HashMap<>();
        for(int i=0;i<target.length();i++){
            char ch = target.charAt(i);
            tar.put(ch,tar.getOrDefault(ch,0)+1);
        }
        List<HashMap<Character,Integer>> table = new ArrayList<>();
        for(String str:stickers){
            HashMap<Character,Integer> temp = new HashMap<>();
            for(int i=0;i<str.length();i++){
                char ch = str.charAt(i);
                temp.put(ch,temp.getOrDefault(ch,0)+1);
            }
            table.add(temp);
        }
        HashSet<HashMap<Character,Integer>> cur = new HashSet<>();
        cur.add(tar);
        int ret = 0;
        while(!cur.isEmpty()){
            HashSet<HashMap<Character,Integer>> next = new HashSet<>();
            ret++;
            for(HashMap<Character,Integer> temp:cur){
                for(HashMap<Character,Integer> word:table){
                    HashMap<Character,Integer> sum = sub(temp, word);
                    if(!sum.equals(temp))
                        next.add(sum);
                    if(sum.isEmpty())
                    	return ret;
                }
            }
            cur = next;
        }
        return -1;
    }
    
    HashMap<Character,Integer> sub(HashMap<Character,Integer> a,HashMap<Character,Integer> b){
    	HashMap<Character,Integer> result = new HashMap<>();
    	for(char c:a.keySet()) {
    		int z = a.get(c)-b.getOrDefault(c,0);
    		if(z>0)
    			result.put(c,z);
    	}
    	return result;
    }
```


## [354. Russian Doll Envelopes](https://leetcode.com/problems/russian-doll-envelopes/)
- 俄罗斯套娃信封
- 给定一组信封，每个信封用一组整数(w,h)表示，一个信封能放进另一个里面当且仅当w,h都大于另一个。问最多可以将多少个信封套在一起？
- 这题我的思路是对width和height从小到大排序，一个信封肯定只能装进它后面的信封里，可以用动态规划求解，时间复杂度为O(n^2)，这个思路不会超时，但不是最好的解法
- 解答中的方法是，对width从小到大排序，而height则从大到小排序，寻找高度递增的最大长度，这个方法的时间复杂度为O(nlogn)是最优解法

```java
	public int maxEnvelopes(int[][] envelopes) {
        Comparator<int[]> comparator = (a,b)->a[0]==b[0]?b[1]-a[1]:a[0]-b[0];//这里使用了Lambda表达式和三元运算符
        int n = envelopes.length;
        if(n<=1) return n;
        Arrays.sort(envelopes,comparator);
        int dp[] = new int[n];
        int len = 0;
        for(int[] envelope:envelopes){
            int index = Arrays.binarySearch(dp,0,len,envelope[1]);
            if(index<0)
                index = -(index+1);
            dp[index] = envelope[1];
            if(index==len)
                len++;
        }
        return len;
    }
```

## [304. Range Sum Query 2D - Immutable](https://leetcode.com/problems/range-sum-query-2d-immutable/)
- 2维数组范围和查询
- 给定一个二维数组矩阵，求两个给定点所围成的矩形范围内所有元素的和
	- 1 假定矩阵不会改变
	- 2 有很多个sumRegion查询请求
	- 3 row1<row2 and col1<col2
- 我的思路是直接保存矩阵，然后每次请求都计算两个点范围内的元素之和。这个方法的效率低，因为每次查询都是O(n)的
- 这题和一维数组范围和查询类似，可以用dp[i][j]表示(0,0)到(i,j)的矩形内元素之和，从而sum(r1,c1,r2,c2) = dp[r2][c2]+dp[r1-1][c1-1]-dp[r2][c1-1]-dp[r1-1][c2]
```java
class NumMatrix {
    int dp[][];
    public NumMatrix(int[][] matrix) {
        int m = matrix.length;
        if(m==0) return;
        int n = matrix[0].length;
        dp = new int[m+1][n+1];
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                dp[i+1][j+1] = dp[i][j+1]+dp[i+1][j]-dp[i][j]+matrix[i][j];
    }
    
    public int sumRegion(int row1, int col1, int row2, int col2) {
        return dp[row2+1][col2+1]+dp[row1][col1]-dp[row2+1][col1]-dp[row1][col2+1];
    }
}
```


## [1092. Shortest Common Supersequence](https://leetcode.com/problems/shortest-common-supersequence/)
- 最短公共超串
- 给定字符串str1和字符串str2，返回一个最短的字符串，使得str1和str2都是这个字符串的子串
- 最先想到的是最长公共子串，可以通过最长公共子串的问题来求解这个问题，先求出str1和str2的最长公共子串，并将子串在str1和str2中的位置标识出来，然后将str1和str2中非公共子串的字符和公共子串字符按顺序连接，即可得到最短公共超串
- 类比最大公约数和最小公倍数

```java
	public String shortestCommonSupersequence(String str1, String str2) {
        int n1 = str1.length();
        int n2 = str2.length();
        if(n1==0 || n2==0)
            return str1+str2;
        char char1[] = str1.toCharArray();
        char char2[] = str2.toCharArray();
        int dp[][] = new int[n1+1][n2+1];
        int i,j;
        for(i=0;i<n1;i++)
            for(j=0;j<n2;j++){
                dp[i+1][j+1] = Math.max(dp[i][j+1],dp[i+1][j]);
                if(char1[i]==char2[j])
                dp[i+1][j+1] = Math.max(dp[i+1][j+1],dp[i][j]+1);  
            }
        StringBuilder sb = new StringBuilder();
        i = n1;j = n2;
        char ch;
        while(i>0 || j>0){
            if(i==0 || (j>0 && dp[i][j]==dp[i][j-1])){
                j--;
                ch = char2[j];
            }else if(j==0 || (i>0 && dp[i][j]==dp[i-1][j])){
                i--;
                ch = char1[i];
            }else{
                i--;
                j--;
                ch = char1[i];
            }
            sb.append(ch);
        }
        return sb.reverse().toString();   
    }
```


## [522. Longest Uncommon Subsequence II](https://leetcode.com/problems/longest-uncommon-subsequence-ii/submissions/)
- 最长非公共子串
- 给定一个字符串列表，求其中最长的非共子串。最长的非公共子串的定义是，这些字符串中某个字符串的最长子串，该子串不是其它任何字符串的子串
- 这题的思路很简单，由于有上次后面的题的教训，看清题目要求后比较简单
- 这题限定字符串列表中字符串长度不超过10，因此把所有具有相同长度的字符串放在同一个list中，然后从长度为10的字符串开始，对于字符串word，word显然不可能是长度比它短的字符串的子串，因此只需要遍历长度比它长的字符串，word是否是它们的子串。
- 判断一个字符串是否是另一个字符串的子串的方式很简单，即判断一个字符串能否由另一个字符串删除若干字符串得到
```java
	public int findLUSlength(String[] strs) {
        int n = strs.length;
        int ret = -1,i,j,k,t;
        List<String> []cache = new List[10];
        for(i=0;i<10;i++)
            cache[i] = new ArrayList<>();
        for(i=0;i<n;i++)
            cache[strs[i].length()-1].add(strs[i]);
        for(i=9;i>=0;i--){
            for(j=0;j<cache[i].size();j++){
                boolean flag = true;
                for(k=i;k<10;k++)
                    for(t=0;flag && t<cache[k].size();t++)
                        if(!(k==i && j==t) && isSubString(cache[i].get(j),cache[k].get(t)))
                            flag = false;
                if(flag)
                    return i+1;
            }
        }
        return -1;
    }
    boolean isSubString(String p,String t){
        if(p.length()>t.length())
            return false;
        int i,j=0;
        for(i=0;i<p.length();i++){
            for(;j<t.length() && p.charAt(i)!=t.charAt(j);j++);
            if(j==t.length())
                return false;
            j++;
        }
        return true;
    }
```

## [1048. Longest String Chain](https://leetcode.com/problems/longest-string-chain/)
- 最长字符串链
- word1是word2的predecessor，当且仅当在word1的某个位置添加一个字符就和word2相等
- 字符串链是字符串序列，[word_1,word_2,...word_k] k>=1, word_1是word_2的predecessor,word_2是word_3的predecessor.
- 求给定字符串数组的最长字符串链
- 本题是动态规划问题
- 设dp[word_i]是以word结尾的最长字符串链，则dp[word_i] = max{dp[word_i-1]}+1, word_i-1是word_i的predecessor，i表示字符串长度
- 刚开始我的思路是先对words数组按字符串长度排序，用HashMap记录每个字符串的最优解，按字符串长度从小到大依次计算，计算word的最优解时，遍历word的所有predecessor（方法是用substring，遍历删除各个位置），判断这个predecessor是不是在HashMap中。这个思路没有超时，但是只超过20%的submit
- 看最优解的思路后，发现了一个新的问题，字符串的长度是有限的，所有字符串长度都在1-16之间。
- 下一个思路，将相同长度的字符串放在同一个HashMap中，查找字符串word的predecessor（predecessor求解依然没变）时只需要在比word长度小1的HashMap中查找，这个方法超过70%的submit
- 最终的思路，依然是将相同长度的字符串放在同一个list中，list[i]表示长度为i+1的字符串的列表，对于list[i]中的每个word，判断list[i-1]中的每个word1是否是word的predecessor

```java
	public int longestStrChain(String[] words) {
        int n = words.length;
        List<String> []cache = new List[16];
        List<Integer> []longest = new List[16];
        int i,j,k,ret = 1;
        for(i=0;i<16;i++){
            cache[i] = new ArrayList<>();
            longest[i] = new ArrayList<>();
        }
        for(i=0;i<n;i++){
            j = words[i].length();
            cache[j-1].add(words[i]);
            longest[j-1].add(1);
        }
        for(i=1;i<16;i++){
            for(j=0;j<cache[i].size();j++){
                int t = longest[i].get(j);
                for(k=0;k<cache[i-1].size();k++){
                    if(isMatch(cache[i].get(j),cache[i-1].get(k))){
                        t = Math.max(t,longest[i-1].get(k)+1);
                    }
                }
                longest[i].set(j,t);
                ret = Math.max(ret,t);
            }
        }
        return ret;
    }
    boolean isMatch(String p,String t){
        boolean flag = true;
        for(int i=0;i<t.length();i++)
            if(flag){
                if(p.charAt(i)!=t.charAt(i)){
                    if(p.charAt(i+1)!=t.charAt(i))
                        return false;
                    flag = false;
                }  
            }else{
                if(p.charAt(i+1)!=t.charAt(i))
                    return false;
            }
        return true;
    }
```


## [710. Random Pick with Blacklist](https://leetcode.com/problems/random-pick-with-blacklist/)
- 给定一个黑名单B，数组B中包含区间\[0,N)的整数，B中的数没有重复
- 写一个随机函数，等概率的返回区间\[0,N)中除去B中数后的任意一个数
- 思路1：初始化的时候生成一个数组，把除数组B中的数都放到数组A中，pick的时候随机生成一个下标，从A中取数，这种思路的结果是pick时间复杂度为O(1)，效率高，但是初始化的效率很低，当N很大时，而数组B的长度很小时
- 思路2：对数组B进行排序，从而将区间\[0,N)的区间分成若干个小区间，随机生成一个数x，x落在某个区间则x的值加上相应的值，在查找区间的时候可以通过二分查找，因此初始化的时间复杂度是O(MlogM),M是数组B的长度，pick的时间复杂度是O(logM)

```java
	/*710. Random Pick with Blacklist*/
	class Solution {
	    int[] black;
	    int N;
	    public Solution(int N, int[] blacklist) {
	        black = blacklist;
	        Arrays.sort(black);
	        for(int i=1;i<black.length;i++)
	            black[i] -= i;
	        this.N = N-black.length;
	    }
	    public int pick() {
	        int x = (int)(Math.random()*N);
	        int i=0,j = black.length-1;
	        while(i<j){
	            int mid = i+(j-i)/2;
	            if(black[mid]>x)
	                j = mid -1;
	            else
	                i = mid +1;
	        }
	        if(i<black.length && black[i]<=x)
	            i++;
	        return x+i;
	    }
	}
```


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

## [873. Length of Longest Fibonacci Subsequence](https://leetcode.com/problems/length-of-longest-fibonacci-subsequence/)
- 最长斐波那契子序列的长度
- 给定一个有序数组，求该数组的一个最长子序列，且该子序列是斐波那契数列
- **动态规划**
- 动态规划的问题在于找出最优子结构，这题想了挺久才想出来，刚开始思路不对
- 刚开始的思路是用dp[i]表示以A[i]结尾的最长斐波那契数列的长度，但是，无论怎么样都没办法通过子问题求解原问题，即dp[i]不能用dp[0]-dp[i-1]表示，然后又在dp[i]上添加其他信息，dp[i]中存储以dp[i][0]表示长度，dp[i][1]表示前一个位置，即倒数第二个数。这个思路始终没有办法用子问题的解来表示原问题的解
- 新的思路是dp[i][j]表示以A[i]、A[j]结尾的最长斐波那契数列的长度，于是dp[i][j] = dp[k][i]+1 if A[k] = A[j]-A[i]。通过这个方式可以用子问题求解原问题，而这种方法需要在数组A中查找A[j]-A[i]，对于每个(i,j)对二分查找的时间复杂度是O(logn)的，因此这种思路的时间复杂度是O(n^2logn)的。
- 按照这种思路，写出来的代码超过20%的submit，显然还有更优的方法
- 在所有submit中，最优的方法是，用HashMap来存储每个数的索引，同时dp[i][j]
- 刚开始的思路不对的原因在于，只用结尾的数无法确定一个斐波那契数列。而新思路的核心在于，用了结尾的两个数来表示一个斐波那契数列。在我想到这个思路之前，我想到的是斐波那契数列数列只要确定了第1和第2个数，整个数列的结果都是确定的，那么只要求出以数组任意两个数开始的斐波那契数列的长度，这个方法的时间复杂度是O(n^3)的。
- 所以问题的核心在于斐波那契数列有两个初值
```java
    public int lenLongestFibSubseq(int[] A) {
        int n = A.length;
        HashMap<Integer,Integer> index = new HashMap<>();
        HashMap<Integer,Integer> longest = new HashMap<>();
        int i,j;
        int ret = 0;
        for(i=0;i<n;i++)
            index.put(A[i],i);
        for(i=0;i<n-1;i++)
            for(j=i+1;j<n;j++)
                if(A[j]-A[i]<A[i] && index.containsKey(A[j]-A[i])){
                    int k = index.get(A[j]-A[i]);
                    int l = longest.getOrDefault(k*n+i,2)+1;
                    longest.put(i*n+j,l);
                    ret = Math.max(ret,l);
                }
        return ret;
    }
```

## [845. Longest Mountain in Array](https://leetcode.com/problems/longest-mountain-in-array/)
- 数组中最长山峰
- 给定一个数组A，B是A的连续子数组，如果B满足下列条件按则称B是mountain
    - B.length>=3
    - 存在0<i<B.length-1，使得B[0]<B[1]<...<B[i-1]>B[i+1]>B[i+1]>...>B[B.length-1]
- 这题的思路很简单，就是记录之前的趋势是增长、减小还是不变，记录由增长到减小的区间长度
- 这题写的代码思路不是很清晰，有点乱，细节有问题

```java
    public int longestMountain(int[] A) {
        int n = A.length;
        if(n<=2)
            return 0;
        int i;
        int start = 0,d = 0;//d表示增长还是减小，还是不变
        int ret = 0;
        boolean flag = false;//表示从start到i是否出现了山顶
        for(i=1;i<n;i++){
            if(A[i]>A[i-1]){
                if(d<=0){
                    if(flag && i-start>=3 && i-start>ret) ret = i-start;
                    start = i-1;
                    d = 1;
                    flag = false;
                }
            }else if(A[i]<A[i-1]){
                if(d==1)
                    flag = true;
                d = -1;
            }else{
                if(d<0){
                    if(flag && i-start>=3 && i-start>ret) ret = i-start;
                    start = i-1;
                    d = 1;
                    flag = false;
                }
                d = 0;
            }
        }
        if(flag && i-start>=3 && i-start>ret) ret = i-start;
        return ret;
    }
```

## [646. Maximum Length of Pair Chain](https://leetcode.com/problems/maximum-length-of-pair-chain/)
- 最长整数对链
- 给定n个数对，每个数对的第一个数都小于第二个数，定义(c,d) can follow (a,b) 当且仅当 b<c
- 找到数对集合中可以组成的最长链的长度
- 贪心算法，任务调度问题，时间复杂度O(nlogn)

```java
    public int findLongestChain(int[][] pairs) {
        int n = pairs.length;
        if(n<=1)
            return n;
        Arrays.sort(pairs,(a,b)->a[0]-b[0]);//函数引用和Lambda表达式
        int ret = 1;
        int end = pairs[0][1];
        for(int i=1;i<n;i++)
            if(pairs[i][0]>end){
                ret++;
                end = pairs[i][1];
            }else end = Math.min(end,pairs[i][1]);
        return ret;
    }
```


## [925. Long Pressed Name](https://leetcode.com/problems/long-pressed-name/)
- 在打字的时候可能会有时可能会因为长按导致某些字母重复
- 判断字符串typed是否是是name long pressed-name
- 思路很简单，一次进行字符计数，判断typed中字符数是否大于等于name中的
```java
    public boolean isLongPressedName(String name, String typed) {
        int n1 = name.length(),n2 = typed.length();
        int i = 0,j = 0;
        while(true){
            char ch = name.charAt(i);
            int a = 0,b = 0;
            for(;i<n1 && name.charAt(i)==ch;i++) a++;
            for(;j<n2 && typed.charAt(j)==ch;j++) b++;
            if(i==n1 && j==n2)
                break;
            if(i==n1 || j==n2)
                return false;
            if(b<a)
                return false;
        }
        return true;
    }
```

## [388. Longest Absolute File Path](https://leetcode.com/problems/longest-absolute-file-path/)
- 用字符串来抽象文件系统
- 字符串"dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext"表示
```
dir
    subdir1
    subdir2
        file.ext
```
- 字符串"dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext"表示
```
dir
    subdir1
        file1.ext
        subsubdir1
    subdir2
        subsubdir2
            file2.ext
```
- 文件名至少包含一个.和扩展名，目录或子目录不包含.
- 这题的思路很简单，考察的是栈和字符串处理，难度不大，但是细节繁琐

```java
    public int lengthLongestPath(String input) {
        int n = input.length();
        if(n==0)
            return 0;
        Stack<Integer> stack = new Stack<>();
        int max_length = 1;
        int cur_length = 0;
        int i = 0,j = n,k,a;
        while(i<n){
            j = input.indexOf('\n',i);
            if(j==-1)
                j = n;
            for(k=0;k<j-i && input.charAt(i+k)=='\t';k++);
            if(k<stack.size()){
                for(a=i-2;a>=i-stack.peek() && input.charAt(a)!='.';a--);
                if(a>=i-stack.peek() && cur_length>max_length) max_length = cur_length;
                while(k<stack.size()) cur_length -= stack.pop()+1;
            }
            int t = j-i-k;
            stack.add(t);
            cur_length += t+1;
            i = j+1;
        }
        for(a=i-2;a>=i-stack.peek() && input.charAt(a)!='.';a--);
        if(a>=i-stack.peek() && cur_length>max_length) max_length = cur_length;
		return max_length-1;
    }
```

## [674. Longest Continuous Increasing Subsequence](https://leetcode.com/problems/longest-continuous-increasing-subsequence/)
- 最长连续递增子序列
- 这题思路很简单，直接遍历，记录到当前位置时递增序列的长度，如果A[i]>A[i-1],则cur加一，否则cur置为1
```java
    public int findLengthOfLCIS(int[] nums) {
        int n = nums.length;
        if(n==0)
            return 0;
        int ret = 1;
        int cur = 1,i;
        for(i=1;i<n;i++)
            if(nums[i]>nums[i-1])
                cur++;
            else{
                ret = Math.max(ret,cur);
                cur = 1;
            }
        ret = Math.max(ret,cur);
        return ret;     
    }
```

## [1027. Longest Arithmetic Sequence](https://leetcode.com/problems/longest-arithmetic-sequence/)
- 最长等差子序列
- 给定一个数组A，求A的最长子数组，该子数组是一个等差数列
- 动态规划，用dp[i][j]表示以A[i]结尾的，公差为j的最长等差序列的长度，dp[i][j] = dp[k][j]+1，其中A[k]+j=A[i]

```java
    public int longestArithSeqLength(int[] A) {
        int n = A.length;
        if(n==0)
            return 0;
        int max = A[0],min = A[0];
        int i,j,k;
        for(i=1;i<n;i++){
            max = Math.max(max,A[i]);
            min = Math.min(min,A[i]);
        }
        int dp[][] = new int[n][2*(max-min)+1];
        int bias = max-min;
        int ret = 1;
        for(i=0;i<n;i++)
            for(j=0;j<i;j++){
                int d = A[i]-A[j]+bias;
                if(dp[j][d]==0)
                    dp[i][d] = 2;
                else dp[i][d] = dp[j][d]+1;
                ret = Math.max(ret,dp[i][d]);
            }
        return ret;
    }
```
## [673. Number of Longest Increasing Subsequence](https://leetcode.com/problems/number-of-longest-increasing-subsequence/)
- 最长递增子序列的数目
- 求一个数组的最长递增子序列的数目
- 动态规划

```java
    public int findNumberOfLIS(int[] nums) {
        int n = nums.length;
        if(n==0)
            return 0;
        int cnt[] = new int[n];
        int len[] = new int[n];
        int max = 1,ret = 1;
        int i,j;
        cnt[0] = 1;
        len[0] = 1;
        for(i=1;i<n;i++){
            len[i] = 1;
            cnt[i] = 1;
            for(j=0;j<i;j++){
                if(nums[i]>nums[j]){
                    if(len[j]+1>len[i]){
                        len[i] = len[j]+1;
                        cnt[i] = cnt[j];
                    }else if(len[j]+1==len[i])
                        cnt[i] += cnt[j];
                    
                }
            }
            if(len[i]>max){
                max = len[i];
                ret = cnt[i];
            }else if(len[i]==max)
                ret += cnt[i];
        }
        return ret;
    }
```

## [594. Longest Harmonious Subsequence](https://leetcode.com/problems/longest-harmonious-subsequence/)
- 最长和谐子序列
- 如果一个数组中的最大值和最小值的差为1，那么这个数组称为和谐数组
- 求一个数组的最长和谐子数组
- 这道题刚开始题意理解错误，误以为是最长连续子数组。然后，看到是子数组后，思路也不对，我的想法是统计每个数字出现的个数，然后排序，找相邻的数。
- 更好的思路是直接对数组排序，然后统计相邻的数出现的个数，但是这里我理解错了，把和谐数组当成最大值和最小值差值小于等于1了。

```java
    public int findLHS(int[] nums) {
        int n = nums.length;
        if(n==0)
            return 0;
        Arrays.sort(nums);
        int pre = -1,preCount = 0;
        int cur,curCount;
        int ret = 0,i;
        cur = nums[0];
        curCount = 1;
        for(i=1;i<n;i++){
            if(nums[i]==cur)
                curCount++;
            else{
                if(cur-pre==1)
                    ret = Math.max(ret,curCount+preCount);
                pre = cur;
                preCount = curCount;
                cur = nums[i];
                curCount = 1;
            }
        }
        if(cur-pre==1)
            ret = Math.max(ret,curCount+preCount);
        return ret;
    }
```

## [300. Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence/)
- 最长递增子串
- 求一个数组的最长递增子序列的长度
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

