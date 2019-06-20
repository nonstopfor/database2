# 实验报告

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

  支持了五个聚类函数，包括 `min`, `max`, `ave`, `sum`, `var_samp`，分别求最小值、最大值、平均值、总和、方差。传入欲求值的列名即可，与 `count` 类似。

  4.拓展支持的数据类型

  新增支持 `time`、`date` 数据类型。

  `time` 支持的数据范围为 -99:59:59 ~ 99:59:59；

  `date` 支持的数据范围为 0000-01-01 ~ 9999-12-31。

  输入时若按带分隔符(: , -) 的格式输入时，需要双引号或单引号包裹。也可直接不加分隔符与引号，以数字串的形式输入。

  两类型均支持了比较操作符运算。

  `addtime(s, n)` 与 `adddate(s, n)` 函数第一个参数为 `time` 、`date` 类型，第二个为整型，将分别以秒和天为单位相加后返回结果。

  5.实现SQL算术运算符

  新增算符：`+`,`-`,`*`,`/`,`%`，通过在头文件`math_cal.h`中定义的函数实现运算的处理，函数中接收来自主进程的变量`NULL_flag`，用来监测是否出现返回`NULL`的情形（如除数为0,对浮点类型取余数）。

  6.实现SQL逻辑运算符

  新增逻辑运算符`NOT`,`AND`,`OR`,`XOR`的支持，具体的功能由递归实现，关于`NULL`的处理依然依靠变量`NULL_flag`。

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

  

## 2.实验反思

### 亮点

* 整体架构清晰，可拓展性较好。
* `forsort`结构体的设计为`GROUP`和`ORDER`以及`UNION`语句（以及其他排序操作）提供了很大的便利，尤其是可以通过参数指定哪些列是排序的指标，比较强大。
* 在处理`where/on`子句时做得较好，将运算，比较，逻辑判断三个部分分离，十分清晰，也便于调试修改。值得一提的是，这样的分离也为实现`LIKE`子句提供了很好的便利，因为`LIKE`本质上是一种比较操作，只要在比较函数中加入对于`LIKE`的处理即可。
* `select`返回的是结果集，为后续操作提供了很大便利，比如`GROUP`和`ORDER`语句就只需要对返回的结果集进行操作即可。也很好地实现了功能的分离。
* 在实现`JOIN`语句时巧妙地引入了`std::pair<int,bool>`，再结合`std::map`的使用（计数，判断某一行是否出现在结果集中），很好地解决了显示为NULL的问题。
* 实现数据库的存档功能时，设计了便于读取的存储形式，避免了在读取时进行复杂的判断。
* 聚类函数可以方便地增加，很好的实现了解耦合。



### 不足

* 对输入语句的语法判断存在一定的纰漏，对于一些非标准格式的输入可能处理上存在一定的问题。

* 没有过多考虑效率因素，没有写高效的数据结构（如B树等），所以复杂度可能偏高。

* 基于第一阶段挑选的代码继续开发，在增加对 `time`、`date` 两个类型的支持的过程中，发现设计类型的代码凌乱的分布在多个函数中，只能逐个修改。反思自己在实现第一阶段代码的过程中也没有很好的为后续增加类型支持提供条件，可扩展性较差。如果采用行为型模式中的模板方法，将类型抽象出一个抽象类，在分别继承编写各个具体类型，就能遵循开放封闭原则，很好地保证拓展性。

  

## 3.附加说明

我们组第二阶段使用的是其他组的代码，并未改动该组第一阶段的`main1.cpp`，也并未修改该组给出的接口，只新加了接口。

