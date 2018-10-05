## BuildingNavigationMeshes

The NMGen ease-of-use extensions for Unity provide GUI-level access to almost all of the NMGen features. You can build navigation meshes with a single click. Or you can build, tweek, and rebuild selected zones of navigations meshes using advanced GUI features.
你可以通过界面操作来使用NMGen的各种常用和高级功能。


All NMGen assets and support code are 'editor only'. So they have zero impact on scene size and are not included in the Unity project build. (No data or code bloat.) The only impact on the scene will be if you need to create custom scene for things like off-mesh connections and area markers.
所有NMGen的资源和代码都只能在Editor环境下跑。所以它们绝对不影响打包。只有一种情况可能会打包，就是当你想保存一些off-mesh connections关系和一些其他的area markers。


The Unity NMGen build process is pretty much the same as the standard build process describe by An Introduction to NMGen. The Unity extensions take care of gathering scene components and compiling them into input data, and provide ways to adjust the build via GUI editors rather than requiring you to code things.
Unity的NMGen生成流程和An Introduction to NMGen介绍的标准生成流程非常相似。Unity扩展接管了一些场景对象，从它们收集数据并转化为用于编译的input data, 并提供了响应的GUI。


The Main NMGen Build Assets


As noted above, the NMGen build process and all associated assets are implemented as project assets, not scene components. The only time you will need NMGen specific components in a scene is if you need to manually annotate something special that you want to include in the build. (E.g. Off-mesh connections.) The below diagram shows the main assets and how they are related from an input/output perspective.
就像上面提到的，NMGen生成流程和所有相关的资源都被整理为项目资源(prefabs)，而不是某个单独的场景文件。只有一种情况可能会打包，就是当你想保存一些off-mesh connections关系和一些其他的area markers。下面这张图展示了相关资源和生成流程的关系。


[diagram]Navmesh Build-Unity Main Assets


The central asset is the NavmeshBuild asset. It defines the build configuration and holds all intermediate build data created during the build.
核心组件是NavmeshBuild.它管理着生成流程的配置和生成过程中的那些中间数据。


The build uses a scene query asset to determine which scene components will be included in the build. Any ScritableObject that implements the ISceneQuery interface can be used. For example, if you want the build to gather scene components based on tags, then you'll use the TagSceneQuery asset.
生成流程使用了一个负责筛选场景里需要参与生成的对象的组件。任何实现了ISceneQuery接口的ScritableObject组件都可以作为筛选器。举个例子，如果你希望筛选器能按Tag来筛选对象，那你会用到TagSceneQuery组件。


Input processors do the heavy lifting such as triangulating MeshFilter and Terrain components, adding INMGenProcessor objects to the build, etc. Any ScriptableObject that implements the IInputBuildProcessor interface can be used.
Input processors(输入数据处理器)承担了一些繁重的工作，比如从MeshFilter和Terrain组件中提取三角网格，添加INMGenProcessor对象以便执行生成流程等等。任何实现了IInputBuildProcessor接口的ScritableObject对象都可以作为Input Processor.


The bake target is the asset that will get the result of the build. (The navigation mesh.) Any ScritableObject that implements the INavmeshData interface can be used. The normal target will be a CAIBakedNavmesh asset. Note that this is a navigation asset, not an NMGen asset, so it can be distributed as part of the project compile and be referenced by scene components.
Bake Target(烘焙目标)是最终持有Navmesh的结果对象。任何实现了INavmeshData接口的ScritableObject对象都可以作为Bake Target.一般情况下会使用默认的CAIBakedNavmesh组件。Note that this is a navigation asset, not an NMGen asset, so it can be distributed as part of the project compile and be referenced by scene components(1).




The NavmeshBuild Asset
NavmeshBuild组件


At the core of the Unity NMGen build process is the NavmeshBuild asset. It is created using one of the CritterAI->Create NMGenAsset->Navmesh Build menu items, usually the Standard option. Once added to the project you'll be presented with the following inspector.
Unity的NMGen生成流程最核心的组件就是NavmeshBuild。它可以通过CritterAI->Create NMGenAsset->Navmesh Build菜单项进行创建，创建时已经有一些标准配置了。
添加后你可以在U3D的inspector视图中看到如下信息。




[diagram]Navmesh Build Inspector


The upper section of the inspector contains the primary build controls and build target assignment. Since it is possible to reassign the target you can use a single configuration to build any number of navigation meshes. Just reassign the target, open the target's input scene, and build.
查看器的上半部分是控制生成流程和生成对象相关。Since it is possible to reassign the target you can use a single configuration to build any number of navigation meshes. Just reassign the target, open the target's input scene, and build(2).


The center section contains the NMGen configuration. It is defaults to values suitable for generic human-sized agents.
中间部分是和NMGen相关的配置。它的默认参数是根据人类大小进行配置的。


The lower section contains the input configuration. The scene query is optional. If none is provided then the search will be global. E.g. All MeshFilters in the scene. There has to be at least one input processor for the build to produce anything.
下半部分是和输入数据相关的配置。场景筛选器是可选的。如果没有添加任何的场景筛选器，则会在全局范围内进行对象筛选和处理。比如场景里的所有包含MeshFilter组件的对象。不论生成什么，至少要指定一个输入处理器。




(1)完全不懂 navigation asset 和 NMGen Asset有啥区别?
(2)需要实验确定?

