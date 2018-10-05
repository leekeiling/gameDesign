## Recast & Detour  Debug Tool代码调用简述

基本的寻路流程

- 点选一个实体进行对应位置的移动操作时，会首先调用PathAgent::setTargetPos，在这个函数里面，会在其对应的寻路服务`pPathFindService`中调用`GetPath`来获取对应的行走路径。
- 获取到路径后，每个`PathAgent`里面有一个用于保存行走路径的变量`m_pVec3StraightPath`,该变量用于保存本`PathAgent`的需要进行行走的路径
- 在`PathCrowd`下面有一个`update`,这个函数在`PathFindService::update`里面不断地调用（实际上就是实时更新`PathAgent`的寻路状态，其实时更新的频率由传入`PathFindService::update`的参数来定）

### Recast Navigation

**RecastNavigation**在特定设置下可以支持动态网格生成的,也就是说可以直接使用**RecastNavigation**来做动态障碍,使用**Tile Cache**的方式实现的。

### RecastDetour

- 在RecastDetour里面有一个**Sample_SoloMesh.cpp**这样一个简单地demo，这个文件实现了如何建立导航网格的全部过程
- 在RecastDetour里面还有一个**NavMeshTesterTool.cpp**，这个文件则演示了如何使用前面建立的导航网格来进行寻路动作

### Recast Demo

该Demo为一个综合性Demo，里面所演示的内容十分全面(其注释十分详尽)，下面简要说明下目录结构和主要文件，以及主要函数的功能,目前项目中所使用的导航以及导航网格生成动态障碍等等，其设计思想以及代码流程都是基于该Demo所提供的方案，所以说完全弄透这套代码基本导航网格寻路以及动态障碍就差不多了

### 目录结构

```
Bin //最终生成的可执行文件
Contrib //一些第三方DLL
English.lproj 
Include 
Source
Icon.icns
Info.plist
premake4.lua //premake4的lua配置脚本，用于生成VS2010工程等
screenshot.png
```

### 主要文件

```
CrowdTool.cpp //管理实体移动寻路以及碰撞
NavMeshTesterTool.cpp // 演示Detour如何来寻找路径，单纯的寻找路径
Sample_SoloMesh.cpp //演示如何来生成导航网格
Sample_TempObstacles.cpp //演示如何添加临时障碍物对网格产生的影响，与动态障碍相关
```

### 关于Demo中的动态障碍

Demo中的动态障碍内容在`Sample_TempObstacles.cpp`,`DetourTileCache.cpp`,过程如下:

> - 首先，当在Demo中按下添加障碍时，会在Demo中调用`Sample_TempObstacles::addTempObstacle`,然后会生成一个`ObstacleRequest`,然后将这个`request`放入请求队列当中去（实际上是个数组）
> - 随后这个增加障碍物的请求依然会在一个`update`（`dtTileCache::update`）中进行，这个`update`更新的频率也是按照设置的帧率来确定的，在这个`update`中，仅仅只做动态障碍相关的更新，比如去除障碍物，添加障碍物等,
> - `update`首先会对前面的`request`进行处理，然后才会进行**导航网格**的`update`
> - 导航网格的更新会在`dtTileCache::buildNavMeshTile`中进行,基本处理过程在代码中有相应注释，至此，动态地添加障碍物的处理过程完毕



