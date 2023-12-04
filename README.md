# SimpleTankInUE  

# 自主实现坦克的运动过程  

UE4中pawn类型有自带的载具组件，可以完成大部分车辆的移动。但是坦克这类载具有很大的不同，一般车辆是前轮保持一致，后轮保持一致；履带类载具是左边和左边各自保持一致，左右独立，这使得履带可以利用左右速度差完成原地转弯的行为，且悬挂系统的上下起伏也更大，以适应更复杂的地形。所以每一个轮轴都应该独立计算受力情况个悬挂起伏状态。
  
![trace](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-04_13-55-18.png)  

每个轮子的力独立计算，但是同一边的轮子，力的方向应该是一致的，大小根据其局部速度进行调整，加速阶段力先增加后减少，达到标准速度后进行匀速行驶，所以力的大小应该也相差无几。  
前进时左右同时向前推进，能够顺利前进，爬坡；转弯时只给外圈一侧加速即可，如果力度不够，也可以左右相反施加力来完成转动效果。

![](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-04_13-52-55.png)  

悬挂系统，轮子悬挂位置每帧都需要向垂直方向做射线检测，以确定接触点的位置，悬挂位置根据高度适当调整。
![](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-04_13-50-46.png)  
![](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-04_13-45-49.png)  
![](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-04_13-48-46.png)


最后是射击系统，和一般枪战类似，从炮口开始计算射击路径。每一次攻击都需要经过装弹换弹，才能再次攻击。  
![](https://github.com/fallwindy/SimpleTankInUE/blob/main/image/Snipaste_2023-12-02_09-55-14.png)
