## The Input Build

The Unity NMGen build process is pretty much the same as the standard build process describe by An Introduction to NMGen. The main difference is in the input compile process.
Unity的NMGen生成流程和《An Introduction to NMGen》里介绍的标准生成流程非常相近。主要的不同之处在于对数据数据的编译处理。


The input compile process is manged by an input builder that is modified by the scene query and input processors assigned to the NavmeshBuild asset.
对输入数据进行编译处理是由Input Builder来完成的，使用scene query(场景筛选器)和input processor(数据处理器)可以调整处理过程，这两种东东是在NavmeshBuild组件里进行设置的。


The Input Compile Process
编译输入数据


The input compile process is managed by the input builder. The the only thing the builder does is manage the input processors. At each step it makes sure all the necessary build assets are ready for use and calls the processors in ascending priority. A processor can be active during one step or multiple steps. For example, the MeshCompiler loads MeshFilter components from the scene during the load step, then triangulates them during the compile step.
对输入数据进行编译处理是由InputBuilder(输入数据生成器)来完成的.生成器只干一件事情，那就是管理所有的input processor(输入处理器).它会确保每一个步骤在生成之前可以以正确的优先级调用不同的input processor(输入处理器).一个处理器可以在不止一个步骤里被调用。举个例子，MeshCompiler会在load step(数据加载阶段)将所有的MeshFilter(u3d组件)找到，然后在之后的compile step(编译步骤)里提取它们的三角形信息。


Unity Input Build Process
Unity里的生成input对象流程


Load Components
加载组件


The scene query asset is used to determine which components in the scene will be included in the input compile. Any ScritableObject that implements the ISceneQuery interface can be used. For example, if you want the build to gather scene components based on tags, then you'll use the TagSceneQuery asset. If no scene query is provided, then the the query will be global. (E.g. All MeshFitlers in the scene, all Terrains in the scene, etc.)
scene query asset(场景过滤组件)用来筛选需要参与编译流程的场景对象。任何实现了ISceneQuery接口的ScritableObject组件都可以作为筛选器。举个例子，如果你希望筛选器能按Tag来筛选对象，那你会用到TagSceneQuery组件。如果没有添加任何的场景筛选器，则会在全局范围内进行对象筛选和处理(例如，所有在场景里包含MeshFilter组件的对象，所有包含Terrain组件的对象之类的事情)。




During the load component step, each input processor uses the scene query to load the components that it cares about. For example, the MeshCompiler will load all MeshFilters and the TerrainCompiler will load the Terrain component.
在加载组件阶段，每一个使用scene query(场景筛选器)的input processor(输入处理器)都会在阶段做一些事情。举个例子，MeshCompiler会筛选所有的MeshFilter组件，而TerrainCompilter则会筛选出所有的Terrain组件。


Filter Components
过滤组件


During the filter component step the input processors have a chance to remove components from the input compile. The component loaders are generally dumb. They load everything the scene query let's them load. In this step, other processors can refine the result. For example, a MeshInputFilter can be used to remove MeshFilter's that reference all 'door' meshes. Or the StaticFilter can be used to remove all components not marked as static.
在过滤组件阶段，每一个input processor都可以移除不需要的组件对象。组件的加载和筛选一般都不是很灵活。它们会加载任何符合场景筛选器规则的对象。在这个阶段，一些有需要的processor可以重新处理下收集起来的结果。举个例子，MeshInputFilter可以将所有属于'door'对象的MeshFilter从结果中移除。而StaticFilter可以将所有标记为static的对象的组件移除掉。


Apply Area Modifiers
应用AreaModifier(区域修改器)


At this point all components that will be compiled have been gathered. During this step input processors can assign custom areas to each component. The assignments will be used during the compile step. For example, the MeshAreaDef can assign the area 21 to all MeshFilters that reference swamp and marsh meshes. During the compile step the MeshCompiler will then assign 21 to all triangles generated from the meshes.
在这个时候，所有需要参与编译的组件都已经收集完毕了。在这个步骤里， input processor可以为每一个组件对象赋上一个自定义的区域类型。这些区域类型会在编译过程中使用到。举个例子，MeshAreaDef对象可以将所有认定为是沼泽和湿地的MeshFilter标记为一个值为21的数。在之后的编译环节里，MeshCompiler会将这些网格产生的三角形的标志位设置为21。




Note Note:
MaxArea will be assigned by default. You can use the DefaultAreaDef asset to assign a different area as the default.
默认值是MaxArea的值。你可以使用DefaultAreaDef对象来重新定义一个默认值。


Compile Input
编译输入数据


During the compile input step the input processors will compile everything needed to create the objects required by the rest of the NMGen build process. (I.e. The InputGeometry, ProcessorSet, and ConnectionSet objects.) MeshFilter and Terrain components will be triangulated, and off-mesh connections and NMGen processors created.
在编译环节，各种input processor会生成创建Navmesh所需的各种中间数据(比如若干个InputGeometry对象，ProcessorSet对象和ConnectionSet对象，MeshFilter和Terrain组件会被三角化，off-mesh connections和NMGen processors会被创建之类的。


Post-Process
后处理


At this point all data needed to create the final input objects has been compiled. During the post-process step the input processors are given the oportunity to make final adjustments to the data before it is locked down in the immutable InputGeometry, ProcessorSet, and ConnectionSet objects.
在这个时候，所有为创建最终navmesh的资源和对象都应该被编译好了。后处理阶段为input processor提供了修改InputGeometry, ProcessorSet, ConnectionSet对象的最后机会，在这之后，它们就会变成只读对象了。