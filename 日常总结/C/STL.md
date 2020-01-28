STL
---
* 容器
1.	顺序容器：vector, deque, list 
```
容器元素并没有排序，插入位置和元素值无关
```

2.	关联容器：set, multiset, map, multimap 
```
元素是排序的，插入任何元素都能按照相应的排序规则来确定其位置，在查找时具有非常好的性能，通常以平衡二叉树方式实现插入和检索的时间是 logN
```

3.	容器适配器：stack, queue, priority_queue 
```		
stack 是项的优先序列，满足序列中被删除、检索和修改的项只能是最近插入序列的项（栈顶的项）。后进先出。
queue 队列。插入只可以在尾部进行，删除、检索和修改只允许从头部进行。先进先出。
priority_queue 优先级队列，维持内部有序，最高优先级元素总数第一个出列。
```

* 迭代器

* 算法
1. 不变序列算法：
 
```
1、该算法不会修改算法所作用的容器或对象
2、适用于顺序容器和关联容器
3、时间复杂度是 n
min, max, min_element, max_element, for_each, count, count_if, find, find_if, find_end, find_first_of, adjacent_find, search, search_n, equal, mismatch, lexicographical_compare
```
2. 变值算法
```
1、此类算法会修改源区间或者目标区间元素的值
2、值被修改的那个区间，不可以是属于关联容器的
for_each, copy, copy_backward, transform, swap_ranges, fill, fill_n, generate, generate, replace, replace_if, replace_copy, replace_copy_if	
```

3. 删除算法：不应作用于关联容器
```
删除一个容器里的某些元素
删除--不会使得容器里的元素减少
- 将所有应该被删除的元素看成空位
- 用留下的元素从后往前移，依次去填空位
- 元素从前往后移，它原来的位置也就算是空位
- 也应该由后面留下的元素填上
- 没有被填上的空位，维持其原来的值不变
remove, remove_if, remove_copy, remove_copy_if, unique, unique_copy 复杂度均为 n
```

4. 变序算法：不适用于关联容器
```
变序算法改变容器中元素的顺序，但是不改变元素的值，复杂度均为 n
reverse, resverse_copy, rotate, rotate_copy, next_permutation, prev_permutation, random_shuffle, partition, stable_partition
```

5. 排序算法：不适用于关联容器和 list
```
比变序算法复杂度更高，一般是 nlogN
sort(快速排序), stable_sort(归并排序) partital_sort, partial_sort_copy, nth_element, make_heap, push_heap, pop_heap, sort_heap
```

6. 有序区间算法：不适用于 关联容器 和 list
```
要求所操作的区间是已经从小到大排好序的
binary_search, includes, lower_bound, upper_bound, equal_range, merge, set_union, set_intersection, set_difference, set_symmetric_difference, inplace_merge
```

