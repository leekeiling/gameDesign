## An Introduction to NMGen

The purpose of NMGen is to generate navigation meshes from arbitrary source geometry. Features fall into two categories: Core and 'standard build'. NMGen用于将任意网格转化为一个或多个NavMesh。 其所有功能都包含于两个目录： Core(核心) 和 'standard build'(普通);

The core features are located in the org.critterai.nmgen namespace and involve creating simplified polygon mesh data from detailed triangle-based source geometry. You can code your own build process from scratch using these features, but it will require a significant amount of work and detailed knowlege of all aspects of the process.核心功能位于nmgen命名空间下，与将复杂的三角形mesh转化为简化的多边形mesh有关。你可以通过代码彻底重新规划生成过程(1)，不过这里有很多事情要做，而且你需要了解生成过程中各个方面的实现细节(2)。

The standard build process implements a flexible, extendable build process suitable for use at design time. It takes care of many of the mundane details for you. Its features are implemented in the following namespaces:
Common extentions: org.critterai.nmbuild
Unity extensions: org.critterai.nmbuild.u3d.editor
普通生成流程为了满足编辑期的需要，被设计成可变的、可扩展的结构。它已经为你处理好了很多细枝末节的事情(3)。普通功能位于以下的命名空间中：
通用扩展(3): nmbuild
Unity扩展: nmbuild.u3d.editor

The standard build process has three main steps which will be covered in detail throughout the rest of this section.
Compile the input.
The NMGen Build. (Produces the polygon meshes.)
Create the navigation mesh.
**普通生成流程有如下三个主要步骤，稍后的章节会对这三个步骤进行详细的介绍。**
**编译数据源.**
**调用NMGen的生成功能(生成多边形网格).**
**生成 Navigation Mesh.(导航网格)**

Note Note:
CAINav uses the OpenGL coordinate system.

快速扫盲
1、A* Path Finding是一个图最短路径搜索算法，只要数据源支持"遍历邻居"的操作，就可以应用A*;
2、Ploygen Mesh 是凸多边形网格，因为凸多边形内任意两点可以直达--在满足指定的倾斜角(slope limit)和高度差(step height)的情况下。
3、如果有地形的Triangle Mesh,其实已经可以基于它进行A*了，因为三角形也是凸多边形。大费周章的使用Nav Mesh是为了1、大幅简化图的复杂度，将不必要的信息进行合并。2、记录一些额外的信息，比如 xxx xxx。