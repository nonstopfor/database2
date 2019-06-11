# 数据库项目说明

## 1.整体架构

三个主要的类：

* `Database`:单个数据库，包含`vector<Table*> dvalue`，存储多个表
* `Table`：单个表，包含`vector<Column*> tvalue`，存储多个列
* `Column`：单个属性，包含`vector<string> cvalue`，存储每一行的数据

`main.cpp`中包含有`Database* now`,指向当前正在使用的数据库，以及`vector<string>all_database`，存储所有的数据库的名字，以及`map<string,Database*> database`,从名字到数据库指针的映射。

## 2.运行逻辑

在`main.cpp`中通过标准输入`cin`读入数据（逐行），根据对输入语句的语法分类，

