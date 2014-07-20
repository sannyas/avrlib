# NRF24L01+ Library

## 功能
+ 接收或发送模式.
+ 多通道通信.
+ 可配置使用或关闭Auto ACK.
+ 可配置使用或关闭动态包长度( Dynamic payload len ).

## 项目文件
+ nrf24l01p.h 头文件,声明了可用的API, 宏和平台依赖的外部函数
+ nrf24l01p.c 芯片相关的最核心的功能实现, 该文件的内容是可移植的, 移植时不需要修改.
+ nrf24l01p\_platform.h 平台相关的头文件, 如果需要将延时函数定义成宏, 可以在此文件中定义, 也可以定义其他一些平台相关的内容.
+ nrf24l01p\_platform.c 为了使代码更容易移植, 需要实现几个平台相关的函数, 主要是SPI通信的 nrf24l01p\_doCommand(), nrf24l01p\_chipEnable(), nrf24l01p\_paltformInit(), nrf24l01p\_delay\_ms() 和 nrf24l01p\_delay\_us(). 详细的函数原型见 nrf24l01p.h 头文件, 移植方法后面详细说明. 
+ README.md 此文件.
+ tx AVR的发送测试
+ rx AVR的接收测试
+ rasp\_test 移植到树莓派, 主要用于测试. 

## 函数库使用说明.

主要的接口如下:

初始化:

	void nrf24l01p_init( void );


配置:

	void nrf24l01p_setDynamicPayloadLen( uint8_t enable );
	void nrf24l01p_setChannel( uint8_t ch );

设置地址:

	void nrf24l01p_setTxAddr( uint8_t *addr, uint8_t len );
	void nrf24l01p_setRxAddr( uint8_t *addr, uint8_t len, uint8_t pipe, uint8_t payload_len );

模式设置:

	void nrf24l01p_enterTxMode( void );
	void nrf24l01p_enterRxMode( void );
	void nrf24l01p_standby( void );
	void nrf24l01p_powerDown( void );

接收或发送数据:

	uint8_t	nrf24l01p_transmit( uint8_t *data, uint8_t len );
	uint8_t nrf24l01p_dataPending( void );
	uint8_t nrf24l01p_receive( uint8_t *data, uint8_t maxlen );


流程:

	+------------+                +-----------+       +----------+ 
	| initialize | -------------> | Configure | ----> | Set addr | --*
	+------------+   ^            +-----------+       +----------+   |
	                 |                                               |
	                 |                                               |
	                 |     *-------*                                 |
	                 |     |       |                                 |
	                 |     |       V                                 |
	                 |     |   +----------+      +---------+         |
	                 |     *<--| transmit | <--- | Tx Mode | <------/|
	      *----------*         +----------+      +---------+         |
	      |                                         |                |
	      |                                         |                |
	      |                  +----------------+     | +---------+    |
	      |               *--| Data pending ? | <---|-| Rx Mode | <--/
	      |           Yes |  +----------------+   ^ | +---------+         
	      |               |     +---------+       | |      |    
	      |               *---> | receive | ------* |      |  
	      |                     +---------+         |      |   
	      |                                         |      |    
	      |   +----------------------+              |      |
	      *-<-| Standby or powerdown | <------------/------/
	          +----------------------+
	

### 初始化和配置

调用 `nrf24l01p_init()` 完成初始化. 此函数会调用平台相关的 `nrf24l01p_platformInit()`, 然后将芯片重置为初始配置. 初始配置为: 使能Auto ACK,关闭所有接收通道, 设置地址宽度为5, Channel为2, 其他为芯片默认值, 可以通过定义的宏常量进行修改, 因为这些在具体的应用中一般不需要在代码中修改.

根据需要调用函数配置, 设置使能动态包长度, 设置Channel.

### 设置地址

如果接下来需要进入发送模式, 需要设置发送地址, 调用 `nrf24l01p_setTxAddr()`设置发送地址, 此函数需要两个参数, 地址和地址长度, `nrf24l01p_setDynamicPayloadLen()`必须在此函数之前调用. 

如果接下来需要进入接收模式, 需要设置接收地址, 调用 `nrf24l01p_setRxAddr()`, 该函数参数较多, 包括 addr, len, pipe, payload\_len, 如果addr为0, 则忽略除pipe之外的所有参数, 将该通道禁止; 如果addr不为0, 则设置指定通道的地址并使能该通道, 要注意的是, 通道2,3,4,5只需要一个字节的地址, 其MSByte与通道1相同. 如果没有使能动态包长度功能, 最后一个参数payload\_len必须被正确设置, 表示指定的通道的数据包长度.

### 进入发送或接受模式

调用 nrf24l01p\_enterTxMode() 或 nrf24l01p\_enterRxMode() 函数将将使芯片进入发送或接受模式

### 发送数据

进入发送模式后, 调用nrf24l01p\_transmit 函数发送数据, 此函数需要两个参数, 数据和数据长度, 如果发送成功, 函数返回1, 如果失败, 返回0.

### 接收数据

进入接收模式后, 芯片工作并接受其他芯片发来的数据, 函数nrf24l01p\_dataPending() 用于检测是否芯片接收到数据并数据可以读取, 如果可读, 函数返回1, 否则返回0. 调用nrf24l01p\_dataPending()返回1后, 可以使用函数nrf24l01p\_receive()读取数据, 调用该函数返回实际读取的数据大小. 

### 模式切换

nrf24l01+ 只能工作与半双工模式, 要实现双向通信或其他复杂操作, 需要根据需要改变模式, 调用 nrf24l01p\_standby 函数使芯片处于待机模式, 调用 nrf24l01p\_powerdown 函数是芯片处于掉电模式, 当芯片处于待机模式或掉电模式后, 可以从头开始按以上步骤重新配置芯片并进入指定的模式.

## 移植
