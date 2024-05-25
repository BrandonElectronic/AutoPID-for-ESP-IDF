#  组件的模板 


###### 组件的功能函数
1. 功能函数的编辑写入到 template.c中
2. 功能函数的声明放到include文件下 

######  Kconfig
1. 组件的Konfig放到根目录下

######  测试样例
1. 测试样例写入到examples/default_example 
2. 测试的话将examples/default_example单独作为一个工程来进行测试

###### 根目录CMakeLists
1. 要将对应的template.c改成对应的xxxx.c
````c
SRCS "template.c"         # list the source files of
``````

````c
SRCS "xxxx.c"         # list the source files of
``````