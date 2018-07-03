## The Path Corridor

The path corridor provides a way of moving along a navigation path without worrying about the minutiae of managing the path.
廊道提供了一种沿指定导航路径进行移动，而不必担心管理使用路径数据时的细节问题。


Core Class: PathCorridor
核心类：PathCorridor


In the simplest scenario, a path is a list of waypoints from a starting point to an end point. The waypoints form a connected set of line segments along which a navigation client moves. But 


navigation is much more complex in the real world. There are locomotion limitations, local steering restrictions, dynamic obstacle avoidance, and good old floating point errors. All of these 


issues can make management of a path difficult.
在最简单的情景里，一个路径是一组从开始点到结束点的路点列表。如果能精确的按照使用此导航数据运动，角色的轨迹会是一组相互连接的线段。但是在真实环境下进行导航比这个要复杂的多。你需要考虑角色的运


动能力的局限性(1)、局部转向限制、避开动态障碍物、以及一些常见的浮点数错误。所有这些情况都会让管理一个路径的使用变得困难。


Navigation based on a polygon mesh provides a different abstraction than just a list of waypoints. The path returned by the NavmeshQuery class is a list of polygons. Technically, a client can 


move freely anywhere within this corridor of polygons. If a client strays outside the corridor, then the corridor can be patched up using local searches rather than full path replanning.
在一个多边形网格上进行导航比在一堆路点上进行导航要更加抽象一些。NavmeshQuery返回的Path是一组多边形。技术上讲，一个角色可以在这个廊道上的任意多边形内自由的移动。如果一个角色跑出了这个廊道的范


围，廊道的范围可以使用局部搜索进行扩大，而不是重新规划整条路径。


The path corridor is loaded with a path, a start point, and a target point. It is then used to move the navigation client through the corridor. It handles path straightening, constraining the 


client to the navigation mesh, and adjusting the corridor due to inaccurate client locomotion.
廊道需要加载一个Path、一个起始点、一个目标点。然后用于角色在这个廊道内移动。它处理路径的矫正、确保角色不会跑出Navmesh、以及当角色移动超出预期时修正廊道信息。


The PathCorridor API is well documented. The only thing worth repeating here is a warning: You can't just initialize a corridor with a path and forget long distance path planning. Since the 


path corridor class uses local searches to maintain the corridor, attempting to apply large delta movement to the corridor's position or target can result in a malformed corridor. Details are 


in the API documentation.
PathCorridor API已经有详细的文档说明了。在这里只有一点值得提一下：你不能只创建一个廊道然后一切都交给它去做，而不进行长路径规划。虽然廊道使用了局部搜索来维持廊道完整性，不过如果在廊道里产生一


个大的运动距离，或者目标点离得很远，会产生一个畸形的廊道(1)。更多的细节可以参考相关API文档。




(1)加速、减速、惯性之类的运动参数.
(2)啥意思?