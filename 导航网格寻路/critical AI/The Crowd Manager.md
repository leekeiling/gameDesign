## The Crowd Manager

The crowd manager is the big beast of the navigation components. It not only handles a lot of the **path management** for you, but also **local steering** and **dynamic avoidance** between navigation clients. I.e. It can keep your agents from running into each other.

The **NavmeshQuery and PathCorridor classes** provide perfectly good, easy to use path planning features. But in the end they only **give you points that your navigation client should be moving toward**. When it comes to deciding things like **client velocity and steering to avoid other clients,** that is up to you to implement. Unless, of course, you decide to use the crowd manager.

Basically, you add an navigation client (agent) to the crowd manager, providing various configuration settings such as **maximum speed** and **acceleration**. You also provide a **local target** to more toward. The crowd manager then provides, with every update, the new agent **position** and **velocity** for the frame. The movement will be constrained to the navigation mesh, and steering will be applied to ensure agents managed by the crowd manager do not collide with each other.

This is very powerful feature set. But it comes with limitations.
The biggest limitation is that you must give control of the agent's position and velocity completely over to the crowd manager. You can update things like maximum speed and acceleration. But in order for the crowd manager to do its thing, it can't allow you to constantly be giving it overrides to position and velocity. So you give up direct control of the agent's movement. It belongs to the crowd manager.（必须将角色的位置和速度完全交给CrowdManager）

The second biggest limitation revolves around the fact that the crowd manager deals with local planning. The agent's target should never be more than 256 polygons aways from its current position. If it is, you risk your agent failing to reach its target. So you may still need to do long distance planning and provide the crowd manager with intermediate targets.

Other significant limitations:（性能开销比较大！！！）
All agents using the crowd manager use the same NavmeshQueryFilter.
Crowd management is relatively expensive. The maximum agents under crowd management at any one time is around 20.

The Sample Pack includes a Crowd Manager Demo that lets you see the manager in action.

### 综述

The Crowd Manger管理AI 角色群体，是角色根据预定义的路由在保证不碰撞的情况下达到目的地。

