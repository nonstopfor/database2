## 实验报告

## 1.功能说明

* 基础需求

* 拓展需求(注：测试数据以及测试结果在Sample文件夹中给出，这里就不再展示)

  1.多表`whereclause`子句

  实现了多表（大于等于2）的`whereclause`子句，实现大的思路与单表类似，通过枚举所有行的组合（每个表中取一行），对每种组合判断是否符合`where`后的条件，如果是，就加入结果集。

  接口设计如下：

  ```
  vector<vector<int>>v=now->where_multiple(tablename,condition)
  enumerate(num_table,1,temp,tablename,result,condition)
  if(where_multiple_work(tablename,v,condition,NULL_flag)){
  			result.push_back(v);
  }
  ```

  通过`Database::where_multiple`函数返回所有行号的组合，`Database::where_multiple`中调用`Database::enumerate`函数对行的组合进行递归枚举，在枚举完最后一个表的行后调用`Database::where_multiple_work`函数判断该行的组合是否满足`where`后的条件，是就加入结果集。

  

  2.支持`UNION`操作符

  实现了多个`UNION`的操作，支持大于等于2个`UNION`以及`UNION ALL`的混合使用。由于`UNION`的操作满足左结合性质，所以只需从左往右进行循环计算即可（可以证明只需从右往左找到第一个`UNION`，左边的都要去重，右边的不去重）。

  接口设计如下：

  ```
  for(int i=0;i<=pos_start;++i){
  	result=(*now)[0]->combine(columnname,result,results[i+1],1);
  }//1代表去重
  for(int i=pos_start+1;i<unions.size();++i){
  	result=(*now)[0]->combine(columnname,result,results[i+1],0);
  }//0代表不去重
  ```

  第一个for循环是去重的union，而第二个for循环是不去重的union，通过调用`Table::combine`函数进行结果集的union操作。如果不需要去重，直接将一个结果集的结果拼接在另外一个结果集之后即可。如果需要去重，在`Table::combine`函数中通过`forsort`辅助排序结构体的使用对两个数据集的和进行排序，去重（使用std::unique函数）后再返回。

  

  3.实现SQL数字函数

  

  4.拓展支持的数据类型

  

  5.实现SQL算术运算符

  

  6.实现SQL逻辑运算符

  

  7.支持多表的`JOIN`

  实现了多表的`JOIN`，支持`(INNER) JOIN`,`LEFT JOIN`,`RIGHT JOIN`的混用（支持大于等于两个表的join操作）。join的大致思路与union类似，从左往右不断地通过循环拓展结果集。join相比于union的麻烦之处不仅在于left join和right join比union的去重更难实现一下，还在于要记录需要展示的数据中哪些是要用NULL来填充的。我利用`std::pair<int,bool>`较简洁地实现了这一个要求，`int`记录了表中对应的行号，而`bool`代表这个数据是否要用NULL填充。

  接口设计如下：

  ```
  for(int i=0;i<joins.size();++i){
  	nowtables.push_back(needs[i+1][0]);
  	r=now->join_it(r,nowtables,needs[i+1],joins[i]);
  }
  if(join_ok(tablename,r[i],need,NULL_flag)){
  	result.push_back(r[i]);
  	...
  }
  ```

  for循环就是不断从左往后合并结果集的过程，通过`Database::join_it`函数返回合并之后的结果集（存储了在各表中对应的行号）。在`Database::join_it`中又调用`Database::join_ok`函数判断是否满足`on`之后的条件（处理方式与`where`子句相同），如果满足，就加入结果集。之后再对`LEFT JOIN`和`RIGHT JOIN`进行判断，如果是两者中的一个，再向结果集中加入额外的结果。

  

  8.数据库存档

  实现了数据库的存档功能（退出后启动仍可以看到原先的数据），并且做到了每一次操作都修改对应的数据文件。存档功能要实现，自然是要讲Database中的数据以合适的方式储存到数据文件（数据存储在`data_keep.txt`）中，以便下次重新启动时能够方便地读取数据。我设计了一个便于读取的数据存储形式，某次存储结果展示如下：

  ```
  database: OOP
  table: fop stu_id//主键
  column: stu_id int(11) 0 stu_score int(11) 1 exam_year int(11) 1 gender char(1) 1 //数据是否可以为空,1代表可以为空
  data: 2018011246 91 2008 m 
  data: 2018011344 92 2009 w 
  data: 2018011445 95 2011 m 
  table: oop_info stu_id
  column: stu_id int(11) 0 stu_name char(1) 1 join_year int(11) 1 gender char(1) 1 
  data: 2018011247 a 2008 m 
  data: 2018011344 b 2009 w 
  data: 2018011445 c 2010 m 
  ```

  相信以上的存储结构一目了然吧！有了这个结构，就可以方便地在重新启动的时候读取数据并建立新的数据库、新的表、新的列。至于在每一次操作后都修改数据文件，只需要再main函数的最后将最新数据写入保存数据的文件即可。

  

  9.支持`LIKE`子句进行模糊搜索

  支持了在`where`或者`on`子句中使用`LIKE`，所有使用`where`或者`on`的句子都支持`LIKE`。要实现这个功能，先看一个例子。

  ```
  select ... where x like "%a%b%c"//a,b,c均为字符串
  ```

  我们只需要在x中找到`abc`这个子串即可（不过需要注意的是首尾的%，如果首没有%，意味着x必须以a开头而不是找到a就行，尾部同理）。不难证明每次寻找都应该取从上一个位置往右第一次找到的位置，然后再从这个位置往后继续找下一个字符串。

  接口设计如下：

  ```
  bool compare(string data1,string data2,string type1,string type2,string opt)
  ```

  这是一个统一的比较函数（作为基础功能函数声明在`column.h`中），如果opt是“like"，就忽略`data1`和`data2`的类型`type1`和`type2`直接进行"like"的判断，实现方法就是上面所叙述的办法。否则做与类型有关的大小比较。

  

  10.支持数据库的远程访问

  

## 2.实验反思


