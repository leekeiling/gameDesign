## Building Multi-Tile Meshes

A multi-tile navigation mesh consists of a group of square tiles layed out in a grid on the xz-plane. Each tile in the mesh consists of its own polygon mesh, detail mesh, and off-mesh connection data. The tiles are linked together when added to the navigation mesh to form the full navigation mesh.
一个多Tile的Navmesh包括了一组矩形的Tile，它们网格化的平铺在xz平面。每个Tile都有自己的PolyMesh,PolyMeshDetail,off-mesh connection数据。这些Tile的Navmesh被连接在一起形成一个完整的Navmesh.


This topic covers the extra steps necessary to create a multi-tile navigation mesh.
本文包含了创建一个多Tile的Navmesh时所必须包括的步骤。




The Multi-Tile Build Process
多Tile生成流程


The process for building multi-tile navigation meshes is very similar to building a single-tile mesh. The main difference is that you need to repeat the IncrementalBuilder and tile build steps for each tile, then combine the result into the final navigation mesh. You also have a new input data class: TileSetDefinition. This class combines the InputGeometry and some configuration settings to define the tile set.
多Tile的Navmesh的生成流程和单Tile的Navmesh生成流程非常相近。两者最大的不同只不过是你需要重复的将IncrementalBuilder(增量生成器)和tile生成流程应用到每一个Tile的生成，然后将这些对象合并从而生成最终的Navmesh.为此你需要使用到一个新的输入数据类：TileSetDefinition.
这个类集成了InputGeometry和一些用来指导Tile生成的配置数据集。

```c++
CopyC#

// Example: A simple multi-tile navigation mesh build.

// 例子：生成一个简单的多Tile的Navmesh;

// Where 'geom' is an InputGeometry object and 'pset' is a ProcessorSet object.

// 'geom'是一个InputGeometry对象，'pset'是一个ProcessorSet对象;

NMGenParams config = new NMGenParams();

// Load the various standard config settings based on your needs.

// E.g. cell size and agent settings.

// 装载你需要的标准配置数据

// 比如Tile的大小和Agent(运动体代理对象)相关的配置.

// ...

// Then set the tile settings.

// The border size must be greater than zero, otherwise the tiles won't connect

// properly when added to the navigation mesh.  You should almost always use

// the standard border size.

// 然后设置Tile配置

// Tile的边界尺寸必须大于0，否则将此Tile添加到Navmesh时，会无法和其他Tile连起来。

// 大部分情况下直接使用默认标准尺寸就好。

config.TileSize = 512;

config.BorderSize = NMGenParams.DeriveBorderSize(config);  // Standard border size. // 边界的默认标准大小;

// Create the tile set definition.

// 创建TileSetDefinition

TileSetDefinition tdef = 

        TileSetDefinition.Create(geom.BoundsMin, geom.BoundsMax, config, geom);

NMGenAssetFlag resultFlags = NMGenAssetFlag.PolyMesh | NMGenAssetFlag.DetailMesh;

List<TileBuildAssets> tiles = new List<TileBuildAssets>();

int maxPolys = 0;  // Need to keep track of maximum polys in a tile. // 需要找出一个Tile包含多边形的数量的最大值;

// Interate over all tiles and build.

// 迭代生成全部的Tile;

for (int tx = 0; tx < tdef.Width; tx++)

{

        for (int tz = 0; tz < tdef.Depth; tz++)

        {

                IncrementalBuilder builder = 

                        IncrementalBuilder.Create(tx, tz, resultFlags, tdef, pset);

                // Perform the build and check the result.

                // 执行生成和检查结果

                builder.BuildAll();

                switch (builder.State)

                {

                        case NMGenState.Aborted:

                                // Handle the error.

                                // Check builder messages for details.

                        case NMGenState.NoResult:

                                // No result was produced.  This is not uncommon for multi-tile builds.

                                // E.g. There may be no geometry at the location.

                                // 没有生成任何东西.

                                // 比如，在指定的位置没有任何几何体信息.

                                continue;

                }

                // Completed with a result.  Build the tile data.

                // 获取完成结果，生成Tile数据.

                NMGenAssets assets = builder.Result;

                // You can build the tile manually, but the tile build task will take

                // care of standard error handling.

                // 你可以手工的生成Tile，不过Tile生成任务对象会帮你处理好一些错误流程.

                TileBuildTask task = TileBuildTask.Create(tx, tz

                        , assets.PolyMesh.GetData(false), assets.DetailMesh.GetData(false)

                        , ConnectionSet.CreateEmpty()  // Or use your connection set. // 或者使用你自定义的ConnectionSet.

                        , false, 0);

                task.Run();

                if (task.TaskState == BuildTaskState.Aborted)

                {

                        // Perform error handling.

                        // Check task messages for details.

                }

                TileBuildAssets tassets = task.Result;

                // Store for later.

// 存起来，之后要用到.

                tiles.Add(tassets);

                maxPolys = System.Math.Max(maxPolys, tassets.PolyCount);

        }

        // Create an empty navigation mesh large enough to hold all tiles

        // that produced a result.

        // 创建一个空的、足够大的Navmesh来装下全部的数据.

        NavmeshParams nconfig = new NavmeshParams(tdef.BoundsMin

                , tdef.Width, tdef.Depth

                , tiles.Count, maxPolys);

        Navmesh navmesh;

        if ((Navmesh.Create(nconfig, out navmesh) & NavStatus.Sucess) == 0)

        {

                // Perform error handling.

        }

        // Add the tiles to the navigation mesh.

        // 将Tile添加到Navmesh.

        foreach (TileBuildAssets tile in tiles)

        {

                // Note: Allowing the navigation mesh to assign the tile reference.

       // 注意: Navmesh会持有Tile对象的引用，因此确保在此前提下不会发生问题。

                uint trash;

                navmesh.AddTile(tile.Tile, Navmesh.NullTile, out trash);

        }

        // The navigation mesh is ready to use.

        // You will usually serialize it for later use.

        // 现在Navmesh已经可以使用了.

        // 一般你会将它串行化，为了以后用.

}



```


Notes


The tile size should usually be between 500 and 1000. A tile size that is too small can result in extra, unnecessary polygons and less than optimal pathfinding. A value that is too large can be result in memory and performance issues during the build process. In general, pick the largest size that also results in a good tile layout along the x and z axes. (You want to avoid creation of thin tiles along the upper bounds of the navigation mesh.)
You can use a single ConnectionSet for the entire build. The build process will add the appropriate connections to each tile.
Off-mesh connections can only be formed between a tile and its immediate neighbors. For example, an off-mesh connection can't start in tile (5, 8) and end in (5, 10) since it would skip over tile (5, 9). So keep the length of off-mesh connections less than (TileSize * XZCellSize).


注意：
Tile本身的大小一般在500-1000之间。一个太小的Tile尺寸会导致寻路时额外不必要的性能开销。一个太大的值则会影响生成的执行效率。In general, pick the largest size that also results in a good tile layout along the x and z axes. (You want to avoid creation of thin tiles along the upper bounds of the navigation mesh.)(1);
你可以只使用一个ConnectionSet来用于多Tile的Navmesh的生成。生成流程会自动的为相邻的Tile添加连接数据。
连接信息只能应用于一个Tile内部、以及与它相邻的Tile。比如，不可能出现一个将(5,8)和(5,10)两个Tile相连的连接信息，因为跳过了(5,9)。所以off-mesh connections的长度肯定小于 TileSize * XZCellSize。(2)








(1)为什么?
(2)这些size都是谁的size.