## The Incremental Builder

The IncrementalBuilder implements a flexible build process that can be used at design time to create PolyMesh and PolyMeshDetail objects. INMGenProcessor's are used to customize the process for almost any need.

Note Note:（注意不适合运行期使用）
This builder is not suitable for use at run-time since it is designed for flexibility rather than high performance.

There are a lot of configuration settings to deal with during the build process. See the NMGenParams documentation for details, including recommendations.

**Only one builder is needed if the navigation mesh will contain only a single tile. Multi-tile meshes require one builder for each tile. (Builders are single use.)**

```c++

CopyC#
// Example: A simple NMGen build.
// 例子：一个简单的NMGen生成.


// Where 'mesh' is a TriangleMesh object containing all of the input triangles
// and 'config' is a NMGenParams object containing the configuration.
// 'mesh'对象是指一个作为原始输入的三角形网格对象.
// 'config'是一个包含配置数据的NMGenParams对象.


// Create an input geometry object.
// 创建一个InputGeometry对象;
byte[] areas = NMGen.CreateDefaultAreaBuffer(mesh.triCount);
InputGeometryBuilder gbuilder = InputGeometryBuilder.Create(mesh, areas, 45.5f);
gbuilder.BuildAll();
InputGeometry geom = gbuilder.Result;


// Create the incremental builder.
// 创建一个增量生成器.
NMGenAssetFlag resultFlags = NMGenAssetFlag.PolyMesh | NMGenAssetFlag.DetailMesh;
IncrementalBuilder builder = IncrementalBuilder.Create(config
        , resultFlags
        , geom
        , ProcessorSet.CreateStandard(ProcessorSet.StandardOptions));


// Perform the build and check the result.
// 执行生成操作并检查结果.
builder.BuildAll();


switch (builder.State)
{
        case NMGenState.Aborted:


                // Handle the error.
                // Check builder messages for details.
// 处理错误.
// 检查buidler的messages字段以便获得错误的细节信息.


        case NMGenState.Complete:


                NMGenAssets result = builder.Result;


                // Use the poly and detail mesh data.
// 使用多边形网格和细节数据.


        case NMGenState.NoResult:


                // No result was produced.  
                // Unexpected for single-tile builds.  Not uncommon for multi-tile builds.
                // Check builder messages for details.
// 没有生成结果.
// (2)Unexpected for single-tile builds.  Not uncommon for multi-tile builds.
// 检查buidler的messages字段以便获得错误的细节信息.
}
```

The IncrementalBuilder's main output is a PolyMesh object. The polygon mesh is made up of convex polygons with between three and MaxAllowedVertsPerPoly vertices. Each polygon includes edge link, area, flag data.（输出是多边形网格对象。多边形网格有多边形列表构成）

Polygons may overlap, based on the value of WalkableHeight used during the build.
At the end of the build process all polygons should have at least one flag set. Usually DefaultFlag.
All polygons will have a non-zero area id. (<= MaxArea)（多边形之间可以重叠，取决于可行走的高度）

### **Poly Mesh-Basic**

### **The Detail Mesh**

An common optional output of the IncrementalBuilder is a PolyMeshDetail object.(optional 附加的)

Pathfinding only uses the PolyMesh. For storage efficiency and speed the polygon mesh doesn't have a lot of height accuracy. But some secondary navigation features require more accurate height data. The detail mesh provides the extra detail.（次级导航需对高度信息有着更高的精度需求。PolyMeshDetail提供这些额外的细节信息。）

The detail mesh consists of sub-meshes, one for each polygon in the polygon mesh. Each sub-mesh is a triangle mesh that contains all the vertices of its associated polygon, plus additional vertices that provide the extra height detail.（PolyMeshDetail包含了若干个子网格(sub-meshes),多边形网格里的每一个多边形都具有此类信息。一个子网格是一个三角网格，包含了所有属于某个多边形的原始三角形顶点信息，以及为了高度信息而额外添加的顶点(4)。）

### The NMGen Processors

The NMGen processors provided by the ProcessorSet are what give the incremental builder its flexibility and power. Without any processors, only a very simple polygon mesh can be created.

**The NMGen Processors**

Processors are called during the post-processor step of each stage, in ascending priority. A processor may perform processing during a single stage or multiple stages.生成流程中每一个步骤都有后处理阶段(post-processor)，处理单元(processor)在这个时机得到执行。一个处理单元(processor)可以在其中一个或多个阶段得到执行。(6)

Any oject that implements INMGenProcessor can act as a processor. The **NMGenProcessor abstract class** provides a good base to start from.

Two common uses for processors include **polygon area** and **flag assignment.**有两个非常常用的processor，它们是【分配多边形的区域ID】和【分配多边形的标志位】。

### Area Assignment

### Area Marker

Area assignment can happen via the InputGeometry object. But sometimes that isn't convenient or possible. Another choice is to mark areas into the compact heightfield. You can use the AreaBoxMarker, AreaCylinderMarker, and AreaConvexMarker processors to do this.分配区域可能在InputGeometry对象生成的时候就做过一次了。但是有时候只在那个阶段做不是很方便(7)。另外一种情况是当你想将一些区域压缩为一个特定地形(8)。你可以使用**AreaBoxMarker,** AreaCylinderMarker, and AreaConvexMarker processors来做到区域压缩。

Because processors are prioritized, area markers can overwrite each other in useful ways. For example, lets say you have a swamp area with a dry island contained within it. The swamp area needs to be assigned one area while the island needs to be a assigned a different area. 
If you mark the swamp, the island will be included. This can't be helped since all markers are convex. But by marking the island with a higher **priority marker**, it will be assigned the proper area.因为processor之间是有优先级的，所以区域标记可以被任意覆写是非常有用的特点。举个例子，你可以在一块沼泽区域内部标记出一块陆地。只需要为相应的多边形分配不同的区域类型就行了。
如果你标记沼泽，内陆岛也会被一并标记。因为所有多边形区域是凸包，所以这是没办法的事情。但是如果使用一个优先级更高的Marker(9)，就可以给内陆岛分配一个合适的区域类型。

Another powerful use of area markers is applying NullArea. Sometimes the build process will create polygons in areas you don't like. For example, there may be clutter on the ground that can technically be walked across, but looks bad when it is. If you mark the area with 
NullArea, then the area will be excluded from the final polygon mesh.另外一种很酷的AreaMarker可以将区域标记为NullArea.有时候会生成一些你根本不想要的多边形区域。举个例子，地上有一块实际上可以通过，但是通过路径会非常奇葩的区域。虽然可以为这块区域正确的生成多边形信息，但是你不想要这个奇葩位。如果你将区域标记为NullArea，这个区域就会在生成最终Navmesh时被排除在外。

### Polygon Flag Assignment

Polygon flags can be managed entirely in the Navmesh object. But sometimes the flags are best assigned to the PolyMesh during the build process. For example, you may want to assign the 'swim' flag to all 'water' area polygons.
Navmesh对象可以完全控制任意多边形的标志位。但是有些时候在PolyMesh生成的中途来分配标志位会更方便。比如你可能想为所有标记成'water'区域的多边形赋上一个'swim'的标志位(10).

The **ApplyPolygonFlags processo**r can be used to add flags to all polygons.
The **AreaFlagMapper processor** can be used to add flags to polygons based on the area id of each polygon.
ApplyPolygonFlags这个processor可以用来给所有的多边形指定标志位。
AreaFlagMapper这个processor可以按区域给多边形指定标志位。







