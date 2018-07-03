## The Advanced Build Type

The NavmeshBuild asset's advanced build type serves three main purposes:
Provides extra debug information to help you figure out problems, or better set the configuration.
Allows a partial rebuilds of multi-tiled navigation meshes.
Performs background processing for long builds.
NavmeshBuild组件的高级生成流程有三个目的：
提供更多的debug信息,或更多的配置控制。
为multi-tiled navigation mesh(多Tile的Navmesh)提供局部重建功能(每Tile重建)。
后台生成。


The advanced build uses the Navmesh Build Manager editor window that can be opened via the CritterAI menu.
可以通过菜单栏里的CritterAI打开Navmesh Build Manager窗口。


The main drawback of the advanced build is that things can get a bit complex. With lots of extra options you need to know what you are doing.
这个东东看着比较复杂。有很多额外的选项等着你去研究。


The advanced build breaks the build down into four steps:
Compile the input.
Set and accept the NMGen Configuration
Build and preview the result.
Bake the result to the target.


高级生成流程可以分解成以下4步：
编译输入数据
应用NMGen配置数据
生成预览结果。
烘焙至bake target。


While the standard build handles all these steps in one click, they are all separate steps in the advanced build.
标准生成里一个点击就完成了所有的步骤，而高级生成里这些步骤是分离的。


Caution note Caution:
The advanced build features are currently in an experimental state. The Unity editor can be finicky when it comes to running background processes. Also, exception handling is not fully tested.
Make sure you save your scenes frequency when using the advanced build features.
注意：
高级生成功能目前还处于实验阶段。当开始执行生成时，UnityEditor可能会变得很诡异。而且异常处理也没有完备的测试过。
在使用此功能的时候一定要多保存场景(1)。


Limitations & Gotchas
限制和陷阱


Because the advanced build lets you go back and forth between the build and the scene while the build is in a partially complete state, there are some things you need to be careful of.
因为高级生成流程会让你在场景和生成器之间来回的倒腾4次，因此你需要特别小心一些方面。


The input is compiled manually. If you make a change to the scene in the middle of a build, you will need to recompile the input before the build will pick up the changes.
原始数据的编译是人工进行的。如果你的场景数据变了，生成器不会自动重建，而是需要你自己重新编译。


Certain configuration settings are locked in once you reach the builder stage. If you want to change these settings after you've started the build you'll need to re-initialize the builder, throwing away any unbaked results.
If you chose to base a new build on the current target's navigation mesh then you will be automatically locked into certain configutration settings from the target. This includes the target's origin. Any new scene geometry below the origin will not be included in the new build.
(2)


The input data cannot survive a Unity Editor reset. A Unity Editor reset will occur, for example, when scripts are recompiled. If an editor reset occurs during a build the build will enter a recovery state requiring manual intervention.
input data(InputData对象)在UnityEditor重置的时候会丢失。UnityEditor会由很多原因引起，比如脚本改变导致重新编译，项目添加新文件导致等等。如果在生成过程中发生了这种情况，生成流程会进入一种等待恢复的状态，等待人工进行恢复。


Be careful when switching scenes while performing an advanced build. If you re-compile the input, it will gather the input from the currently open scene.
在使用高级生成的时候，要注意切换unity场景这个操作。如果你想要重新编译输入数据，它会从当前打开的场景上进行对象收集。


The advanced builder stores intermediate build data in the NavmeshBuild asset. This data is not cleared when you bake the mesh. You must exit the build using the Exit Build button. Otherwise, the next time you interact with the build you'll start with the cached data. The primary reason for this behavior is to prevent the loss of build data in case of an accidental Unity Editor reset. But it is also a useful behavior if you happen to want to return to a partially complete build at a later time.
高级生成NavmeshBuild组件里存放中间数据。这些中间数据在完成navmesh生成后不会被清理。你需要使用Exit Build(退出生成)按钮来结束生成流程。否则，当你下次发起生成的时候，会从一个有历史数据的状态开始进行。一般利用上述特性的情况是当一个意外的编辑器重置发生了，你可以利用这个特点恢复数据。你也可以在希望回退到上一个完整的重建结果时利用它。


Don't forget to bake your build result. Baking to the target is never automatic.
别忘了bake(烘焙)。bake操作并不是一个自动执行。


Input Compile


The first step in the advanced build is the input compile. Once you run the compile you'll get useful debug information along with visualization options.
高级生成流程的第一部就是编译输入数据。当你完成编译步骤之后，你会得到一些有用的调试信息，它们会以可视化的形式直接显示在场景上，通过相应的选项开关进行显隐控制。


Advanced Build-Input


You can come back and re-compile the input at any point in the build process. This is especially helpful if you are tweaking the scene and partially rebuilding a multi-tile navigation mesh.
你可以在生成流程的任意阶段回来重新编译输入数据。这在你调整场景和进行局部生成时非常有用。


Configuration


During the configuration step you set the NMGen configuration that same as you would during a standard build.
配置NMGen的过程和标准生成流程一致。


Advanced Build-Config


Once the configuration is set, you can start a new build from scratch using the From Scratch button. If the target contains a supported multi-tile navigation mesh, then you can initialize the build from the target using the Based on Target button. This will load the existing mesh's configuration and tile data into the builder so you can perform a partial rebuild.
当你配置好了你可以使用From Scratch按钮从头开始生成。如果bake target对象包含了一个完整的multi-tile navigation mesh(多Tile的Navmesh)数据，你可以通过Based on Target按钮从中逆向填充生成相关配置数据，以便方便的进行局部重新生成。


The Builder


Note Note:
There are two types of builders, one for single-tile and one for multi-tile meshes. This section covers the multi-tile version. The single-tile version is simple, so it is not covered here.
注意，有两种生成器，一种是单Tile的，另外一种是多Tile的。本章节介绍的是多Tile的。单Tile很简单，就不在这里介绍了。


The builder allows you to build, tweak, and rebuild your navigation mesh as needed. Once satisfied you can bake the result to the build target. Various debug visualizations are available to help you see what is going on with the build. Some visualizations are only available in zone mode.
生成器能让你生成、修改、和重新生成navmesh。你可以不断尝试，生成的结果会存放在build target对象里。许多可视化的调试信息能让你看到当前的进展情况。有些信息只在zone mode(区域模式)里才会有(3)。


Advanced Build-Builder


Caution note Caution:
Be careful. Some of the visualizations, especially the Input Geometry visualization, are framerate killers.
注意：
小心。一些可视化的调试信息是FPS杀手，比如Input Geometry。


It is possible to operate on only part of a navigation mesh by entering zone mode. You can do this by clicking a tile and adjusting the Zone Size to get the coverage you desire. You can set the zone size to zero if you only want to select a single tile. Exit zone mode by clicking on the selected tile.
在zone mode(区域模式)下你可以操作部分navmesh.你可以点击单个Tile，调整其Zone Size(区域大小)。如果你只想有一个Tile，可以将ZoneSize设置为0。退出ZoneMode只需要点击先前选中的Tile。


Builds are performed in the background with the progress shown in the tile grid. Be aware that the background builder is shared by all build operations in a priority queued manner. It also limits the number of concurrent operations. So if you are building multiple meshes at once, or perform an input compile in the middle of an in-progress build, don't be surprise when some operations have to wait for others to complete. Input compiles have the highest priority, followed by zone builds. 'Build All' builds have the lowest priority.
生成流程会在后台执行，并显示一个进度条。后台的生成器(4)被所有生成操作所共享，不同操作之间存在优先级设置。它同时执行的任务的数量也有限制。所以当你进行一个多mesh生成，或者在某个生成过程中执行编译输入数据，你可能需要等待其他操作的完成。输入数据的编译操作具有最高优先级，然后是分区生成。'Build All'操作(5)优先级最低。