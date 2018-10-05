## Build Configuration

This topic covers some of the Unity specfic configuration settings found in the NavmeshBuild asset.
本章介绍了Unity扩展里的NavmeshBuild组件所包含的一些配置选项。


Note Note:
See the NMGenParams documentation for information on the standard configuration settings.
可以通过NMGenParams文档了解更多关于标准配置的相关信息。


[diagram]Controls and Target Assignment




The upper portion of the inspector contains the main build controls and build target.
查看器的上半部分是生成控制和build target(存放生成结果的东东).


Navmesh Build Upper


The bake target is the asset that will get the result of the build. (The navigation mesh.) Any ScritableObject that implements the INavmeshData interface can be used. The normal target will be a CAIBakedNavmesh asset. Note that this is a navigation asset, not an NMGen asset, so it can be distributed as part of the project compile and be referenced by scene components.
bake target(烘焙目标)是用来存放生成结果的组件(Navmesh)。任何实现INavmeshData接口的ScriptableObject都可以作为back target。可以直接使用现成的CAIBakedNavmesh组件。注意它是一个navigation asset,不是一个NMGen asset(1),因此它可以参与u3d项目的编译。


There are two types of builds. A standard build is performed directly from the inspector and will run a foreground build of the entire navigation mesh. (Can't do anything else in the Unity editor until the build is complete.) The advanced build provides extra debug options, background build capabilities, and the ability to partially rebuild multi-tile navigation meshes. The advanced build uses a special editor window for the build.
有两种类型的生成。标准生成可以直接通过查看器里的选项发起，然后在后台做navmesh的生成(在UnityEditor环境下，在生成完成之前也没法做别的事情)。高级生成提供了额外的debug选项，更多的后台生成能力，和multi-tile navigation mesh(多Tile的Navmesh)的部分重建。高级生成有一个独立的编辑器面板。


The Log Mesh State button will log the current state of the build target to the console. (Polygon count, bounds, etc.)
Log Mesh State按钮可以通过log显示生成流程当前的进度信息。(多边形数量，边界信息之类的)


The Build & Bake button will run a full build and bake of the navigation mesh in the foreground using the current scene as the input source. (There will be a warning at the top of the inspector if the current scene is not the same as the last input scene.)
Build & Bake按钮发起生成（如果两次生成时input scene发生改变了，会在查看器面板顶部显示一条警告）


If Trace is enabled, then detailed trace messages will be sent to the console. This is useful for debugging the build.
如果开启了Trace,会在console窗口里显示更多的信息。这个对调试非常有用。


NMGen Configuration


Note Note:
This section will not be avaiable if the build type is set to advanced.
注意，如果选择了高级生成，这里的配置是不起效的。


The configuration section contains the standard NMGen confiugration settings plus a some Unity specific options.
configuration区块包含了NMGen的标准流程的配置选项，以及一些针对unity的额外选项。


Navmesh Build Config


The very first NMGen configuration settings you should take a look at are the Agent Settings. Most other settings are derived, at least in part, from these. Unlike the NMGenParams core class, these agent settings are in world units, not cell units. Set the values based on your agent maximums. (Maximum height, maximum radius, etc.)
NMGen配置和Agent配置看着很像。继承了很多选项。不过Agent的配置都是基于世界坐标系的，而NMGenParams的配置是基于Cell坐标系的。


The next settings you'll want to take a look at are the Resolution and Tile Settings. You can derive these yourself with the help of the NMGenParams documentation. But if you've already set up the scene you can use the Derive button to get a good starting point. The values will be derived based on a combination of the agent settings and bounds of the input geometry.
接下来是Resolution和Tile配置。你可以直接使用Derive按钮自动填充，它会根据agent和InputGeometry的bounds自动计算，也可以参考NMGenParams文档里的介绍手工填写。


Note Note:
A Tile Size of zero results in a single tile mesh.
注意：
Tile的Size为0表示只有一个Tile。


There are various toggle options in the Advanced configuration section. These will automatically add NMGen processors to the build as follows:
Ledges Not Walkable -> FilterLedgeSpans
Low Height Not Walkable -> FilterLowHeightSpans
Low Obstacles Walkable -> LowObstaclesWalkable
Apply Poly Flag -> ApplyPolygonFlags (Flag: 0x01)


高级配置支持一些可开关的选项。这些会自动的向生成流程添加相对应的processor：
暗礁不可行走 -> FilterLedgeSpans
低于此高度不可行走 -> FilterLowHeightSpans
低矮的障碍物可以行走 -> LowObstacalesWalkable
给多边形设置Flag -> ApplyPolygonFlags(默认值:1)




If you don't need a lot of height detail in your navigation mesh you can uncheck the Include Detail Mesh option. This will make the final navigation mesh more memory efficient.
如果你不需要高度信息，就别勾选Include Detail Mesh选项。这能明显减少navmesh的内存占用。


The Clean button can be used to round values to more reasonable values. (E.g. 0.2929384839 to 0.29)
Clean按钮能减少数值不必要的精度。(比如将0.2929384839变成0.29)


The Reset button will reset the NMGen configuration to its original default values.
Reset按钮将NMGen配置复位为默认值。


Input Configuration


The input configuration contains the scene query and processor asset assignments.
input配置包括scene query(场景筛选)和processor(输入处理器)的配置。


Navmesh Build Input


The assets assigned in this section can be created using menu items founds under CritterAI -> Create NMGen Assets. Note that some assets are effectively singletons with no configuration. In such cases there is no need to create more than one per project; and the inspector won't let you add them twice to the same build.
添加到这个地方的组件可以通过菜单栏的CritterAI -> Create NMGen Assets来进行创建。注意有一些组件是不可配置的单件(全局独一份)。因为对于生成流程来说它们不需要存在多份，所以查看器直接不让你将这种组件添加多个。


It is perfectly fine to share scene query and input processors between multiple NavmeshBuild assets.
在不同的NavmeshBuild组件之间共享scene query 和 input processor是十分正确的做法。


The most common scene query is TagSceneQuery. The scene query is optional. If none is provided then the search will be global. E.g. All MeshFilters in the scene, all terrains in the scene, etc.
最常用的场景筛选器是TagSceneQuery.场景筛选器是可选的。如果没有指定筛选器，默认就是场景类的全部对象。比如全部的MeshFilter,全部的Terrain等。


There must be at least one input processor for the build. For simple builds this is usually a compiler. The most commonly used compiler is MeshCompiler. You can add as many processors as you wish. The processors will be run in ascending priority. This is especially useful for area assignment since it allows higher priority areas to overwrite lower priority areas.
生成流程至少要有一个input processor(输入处理器)。对于简单的生成，一般是一个编译器。最常用的编译器是MeshCompiler。你可以添加任意数量的处理器。处理器的调用顺序由其优先级来确定。优先级对于重写某些处理结果非常有用。


The inspector won't let you add the the same input processor asset to a build twice.
同一种处理器不能添加两次。


The Auto-Clean option is used to automatically clean up invalid triangles in the input geometry. Sometimes modeling programs can let invalid triangles slip in; triangles that are no problem for the renderer but cause problems for NMGen. For example, triangles that contain the same vertex twice will be removed.
Auto-Clean(自动清理)选项用来清理原始几何体信息中无效的三角形。建模程序有时候会产生一些无实际意义的三角形(1);这些三角形可能不影响渲染，但是会影响Navmesh的生成。举个例子，退化成一个边或一个点的三角形会被移除。