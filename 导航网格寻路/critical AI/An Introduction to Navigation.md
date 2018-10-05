## An Introduction to Navigation

The navigation side of CAINav consists of features used to create and use navigation meshes for pathfinding and local steering.（CAINav提供创建Navmesh、基于Navmesh的寻路和局部转向避障。）

There are five main classes you'll be dealing with:

Note Note:
CAINav uses the OpenGL coordinate system.

The **Navmesh class** defines the structure and state of the navigation mesh. The only user component that deals directly with this class on a regular basis is the component responsible for creating the mesh and managing its state. Normal navigation clients, the one's needing to perform pathfinding and such, rarely if ever interact directly with the navigation mesh.

The most important class to understand and become comfortable with is the **NavmeshQuery class**. It provides almost all of the client navigation features, including A* and Dijkstra path searches, string pulling, point and polygon searches, raycasting, etc.

The **NavmeshQueryFilter** is a seemingly minor class, but it has a large impact on navigation. Filters are used to define the costs of traversing the mesh and flags that control which parts of the navigation mesh are included in operations. Multiple filters can be used to customize the navigation behavior on a per client basis.

The **PathCorridor class** provides a way of moving along a navigation path without worrying about the minutiae of managing the path.

The **CrowdManager** is the big beast of the navigation classes. It not only handles a lot of the path management for you, but also local steering and dynamic avoidance between navigation clients. I.e. It can keep your agents from running into each other.

### 综述

需要注意的5个类：

1.  Navmesh class
2.  NavmeshQuery class
3. NavmeshQueryFilter class
4. PathCorridor class
5.  CrowdManager

