## The Navigation Query

The navigation mesh query is the most important class to understand since most navigation clients will interact with the query rather than the navigation mesh itself. The mesh contains the data, but the query provides almost all of the features necessary for pathfinding.

### Core Class: NavmeshQuery

### Query features fall into two general categories: Pathfinding and local search.

Pathfinding involves standard A* and Dijkstra searches that find the best path(s) between two points. **Paths are made up of a list polygon references that represent a polygon corridor from the start to the end position. Path straightening is used to convert the path into a list of waypoints. (I.e. String pulling.)**

The local search features offer various methods for locating polygons and points on polygons, and for querying the local environment. I.e. Raycasting, finding the distance to the nearest wall, etc.

**Many of the query methods require a NavmeshQueryFilter.** Filters define area traversal costs as well as flags used for including/excluding polygons and off-mesh connections from results.

The best way to understand the query class is to play around with it. The Sample Pack includes the Query Explorer demo that permits experimentation with all of the main query features.

### Common Operations

This section contains some simple examples of common query operations.
Finding a Point in the Navigation Mesh
You can't do much without first getting a valid point on the navigation mesh. So the first step is to find one.
GetNearestPoint(Vector3, Vector3, NavmeshQueryFilter, NavmeshPoint)

```c++
CopyC#
// Where 'query' is a NavmeshQuery object and 'filter' is a NavmeshQueryFilter object.
// 'position' is a Vector3 indicating the world position of the client.
// 'query'是一个 NavmeshQuery对象，'filter'是一个NavmeshQueryFilter对象。
// 'position' 是一个Vector3对象，值为角色的世界坐标。

```

```c++
NavmeshPoint result;
Vector3 extents = new Vector3(1, 1, 1);  // Keep this to the minimum extents practical. // 范围越小越好。


NavStatus status = query.GetNearestPoly(position, extents, filter
        , out result);


if (result.polyRef == Navmesh.NullPoly)
{
        // Handle error.  Could not find a result.
        // The status can be checked to see if there was an error.  If not, then
        // the cause is that the search extents did not overlap any polygons.
        // 错误处理。找不到结果。
        // 可以检查状态看看是什么问题。如果没有问题，说明指定范围里不包含多边形。
}


// Use the result point, which includes a vector point and the reference of 
// the polygon that contains the point.
//使用结果点。包括一个Vector3的点和包含这个点的Polygon的引用。

```

Basic Pathfinding
Even if you are planning to use PathCorridor or CrowdManager, you'll always need to do long distance planning using the basic NavmeshQuery features. First, get a path, then optionally straighten it.

```c++
CopyC#
// Where 'query' is a NavmeshQuery object and 'filter' is a NavmeshQueryFilter object.
// 'start' and 'end' are NavmeshPoints known to be on the navigation mesh.
// 'query'是一个 NavmeshQuery对象，'filter'是一个NavmeshQueryFilter对象;
// 'start' 和 'end' 是已知的在Navmesh上的点;


int pathCount;
// The path will be a list of polygon references.
// path是一组polygon的引用;
uint[] path = new uint[100];  // Size for maximum allowed path length. // 路径的最大长度;
NavStatus status;


if (start.polyRef == end.polyRef)
{
        // No need to do any planning.
        // 开始点和结束点在同一个多边形内，不需要进行路径规划;
        pathCount = 1;
        path[0] = start.polyRef;
}
else
{
  		//need to make the filter
        status = query.FindPath(start, end, filter, path
                , out pathCount);

		
        if (NavUtil.Failed(status) || path.pathCount == 0)
        {
                // Handle pathfinding failure.
                // 处理寻路失败;
        }
        else if (end.polyRef != path[pathCount - 1])
        {
            // Handle a partial path.
            // The query either could not reach the end point,
            // or the path buffer was too small to hold the
            // entire path.  (A check of 'status' will reveal if
            // the buffer was too small.)
            // 处理只有一部分路径的情况;
            // 可能是结束点不可达;
            // 或者路径的buffer长度太小，装不下整条路径;
            // 如果是buffer太小，可以检查state;
        }


}


// If you need to straighten the path...
// 如果你需要拉直路径;


const int MaxStraightPath = 4;  // Just getting the first 4 waypoints. // 只处理前4个路点;
int wpCount;


// The waypoints.
// 路点列表;
Vector3[] wpPoints = new Vecotr3[MaxStraightPath];


// A list of polygon references.  (The polygon being entered at each waypoint.)
// 一个多边形引用列表;(路点是多边形的入口点)
uint[] wpPath = new uint[MaxStraightPath];


// The type of each waypoint. (Start, end, off-mesh connection.)
// 每一个路点的类型信息.(开始点，结束点, 连接);
WaypointFlag[] wpFlags = new WaypointFlag[MaxStraightPath];


status = query.GetStraightPath(start.point
        , goal.point
        , path
        , 0                // The index of the start of the path. // 路径的开始点;
        , pathCount        // The length of the path.             // 路径的长度;
        , wpPoints
        , wpFlags
        , wpPath
        , out wpCount);


if (NavUtil.Failed(status) || wpCount == 0)
{
        // Handle the failure.  There should always be at least one waypoint 
        // (the goal) for a valid point/path combination,
        // 处理失败。应该总是存在一个点(目标点)用于路径合并.
}


// Use the path and waypoints.
// 使用路径和路点;
```

(1)可以理解为句柄，或索引。
(2)Path上每个多边形中心之间的连线一般不会是直线，路径如果有拐角也会造成一些美观上的问题，这个时候需要使用特定的方法将路径变得尽量笔直，就好像将一根绳子拉直。



## 综述

1. 实例化query对象
2. 开始结点寻找最近多边形
3. 实例化queryfilter对象
4. 初始化用于保存多边形引用路径的数组
5. findpath
6. 初始化waypoints数组
7. 初始化多边形引用列表
8. GetStraigntPath

