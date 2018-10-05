## Naviagation Mesh地图基本概念

### Navmesh

核心类。

### Tiles

存放了大多数结构信息和状态信息。Navmesh有点向Tile Manager。Navmesh可由单个Tile构成，也可以向网格一样平铺开来，以便在运行期动态的替换其中的某个Tile。

### Tiles包含的数据结构（data structural）

1. ### Polygons

   多边形集合

2. ### off-mesh connection

   分离的网格之间的连接关系集合。想象一下，一个角色跳过栏杆-栏杆可以阻碍普通的前进。

### Tiles包括的状态数据 （state data）

1. ### Area

   一般用来定义移动代价。代价影响寻路。举个例子，穿越被标记为沼泽的区域时的代价花费可能比穿越标记为草地的区域的代价要高。

2. ### Flags

   可以用来控制一个节点的可行走属性。举个例子，一个作为'门'的多边形，在门关闭的时候，可以给这个多边形设置一个特殊的标志位，以表示它是不可通过的。或者设置一个特殊的标志位，只允许特定类型的角色通过。

Areas和Flags的意义不是由Navmesh定义的，这非常重要。这些特别指定的数字都是由导航系认的用户系统自定义的NavmeshQueryFilter来解释的。举个例子：一个过滤器认为ID为5的区域具有10点路径代价，而另一个过滤器则认为只有2的路径代价。当一个关闭的门的标志位被设置后(1)，一种过滤器在寻路的时候就会考虑这种情况而将这个门视为不可通过，另外一种过滤器-可能是用于鬼魂的寻路-就会完全忽略这个标志位而将此节点认为是可通过的。

### Tile 结构 VS 状态

节点元素和状态的分离。

首先，这样它们可以独立的进行保存和加载。一个通常情况是，加载一个预制的Navmesh数据后，其中的状态可能都是默认值，而利用一个上一次运行时保存的串行化的状态缓存，可以覆盖原始的状态信息。

不过这里有一个限制，状态信息和节点自身是精确匹配的。你不能在更改完一个节点的信息后，又将一个老的状态赋给它。

### Tile和Polygon的引用方式

要完全理解Navmesh，最后一点信息是关于Tile和Polygon的引用这个概念。这些引用是Navmesh为数不多的数据结构之一，而不是由Tile定义的。理解引用是什么很重要，因为通过它可以实现当导航网格发生变化的时候，一些类似寻路功能的模块能自动的将引用的多边形置为无效。

本质上来说，Tile和Polygon的引用是一个uint32(无符号整型)。Tile的引用很少被客户代码直接使用，因为使用场合基本上都可以被Polygon代替。Navmesh里的每个Polygon的引用都是唯一的，因此它们能够非常方便的定位到指定的Polygon，而不需要知道这个Polygon属于哪个Tile。

虽然名字差别比较大，Polygon的引用概念同时也涵盖了off-mesh connections对象。
引用可以变成无效的。如果在无效之后还使用引用，函数会返回一个失败信息。Polygon的引用有效性是基于Tile的引用的。因此当Tile的引用无效了，所有属于Tile的Polygon的引用也会无效。
构造的变化会影响引用的生命周期。改变Navmesh的配置或者Tile的内部结构会引起相关的引用全部失效。

引用会在Tile的状态发生变化的时候进行保存，比如指定Area或Flag值的时候。在运行期动态的加载或卸载Tile也会使引用自动保存，还有当正常串行化/反串行化一个Navmesh的时候。

### 创建Navigation Mesh流程

1、生成打包好的Tile数据。

2、使用NMGen生成PolyMesh和PolyMeshDetail

3、创建个ConnectionSet(Off-mesh connection的容器)，这是可选的。

4、将这些数据(tile,PolyMesh,PolyMeshDetail,ConnectionSet)加载到NavmeshTileBuildData对象里。

5、创建一个NavmeshTileDatad数据。

现在Tile的构成关系和默认状态已经都封装到NavmeshTileData里了，并且为生成Navmesh而做了数据结构的改变

注意：
直接使用NavmeshTileBuildData类可能有点难搞。GetBuildData这个工具函数提供了一种标准流程。

创建一个Navmesh
如果创建只有个Tile的Navmesh,只需要调用一次Create(【NavmeshTileBuildData】, Navmesh)函数。
创建多Tile的Navmesh需要多次调用函数。使用Create(【NavmeshParams】, Navmesh)函数创建一个空的navmesh,然后使用AddTile函数将Tile添加进去(1)。



Navmesh的串行化
Namvesh类支持二进制串行化，使用GetSerializedMesh来完成。这会将整个mesh转成二进制数据。
它可以每Tile的、状态和拓扑结构分离的进行串行化。从mesh里取Tile使用GetTile(Int32)函数，然后使用GetState(Byte[])函数和SetState(Byte[])函数进行状态的赋值。



(1)Tile数据有自己的生成流程，可以参考An Introduction to NMGen