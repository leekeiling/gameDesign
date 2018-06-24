# Long and short term goals

## Behavior flags or stacks

## Behavior flags or stacks

Your units may have more than one goal. For example, you may have a general goal like “spying” but also a more immediate goal like “go to the enemy headquarters”. In addition, there may be temporary goals like “avoid that patrol guard”. Here are some ideas for goals:

- **Stop**: Stay in the current location
- **Stay**: Stay in one area
- **Flee**: Move to a safe area
- **Retreat**: Move to a safe area, while fighting off enemy units
- **Explore**: Find and learn about areas for which little information is known
- **Wander**: Move around aimlessly
- **Search**: Look for a particular object
- **Spy**: Go near an object or unit to learn more about it, without being seen
- **Patrol**: Repeatedly walk through an area to make sure no enemy units go through it
- **Defend**: Stay near some object or unit to keep enemy units away
- **Guard**: Stay near the entrance to some area to keep enemy units out
- **Attack**: Move to some object or unit to capture or destroy it
- **Surround**: With other units, try to surround an enemy unit or object
- **Shun**: Move away from some object or unit
- **Avoid**: Stay away from any other units
- **Follow**: Stay near some unit as it moves around
- **Group**: Seek and form groups of units
- **Work**: Perform some task like mining, farming, or collecting

For each unit you can have a **flag indicating which behavior it is to perform. To have multiple levels, keep a *behavior stack*.** The top of the stack will be the most immediate goal and the bottom of the stack will be the overall goal. When you need to do something new but later want to go back to what you were doing, push a new behavior on the stack. If you instead need to do something new but *don’t* want to go back to the old behavior, clear the stack. Once you are done with some goal, pop it from the stack and start performing the next behavior on the stack.

## Waiting for movement(碰撞检测)

It is inevitable that the movement algorithm will run into **obstacles** that were not there during the pathfinding process. An easily implemented technique is based on the **assumption that the *other* obstacle will move first**. This is particularly useful when the obstacle is a friendly unit. When an obstacle is in the way, wait some amount of time for it to move. **If it still hasn’t moved after that period of time, recalculate a path around it or to the destination. If the obstacle is detected ahead of time, your unit can simply walk slower to give the other unit more time to get out of the way.**

It is possible that **two units will bump into each other, and each will wait for the other to proceed**. In this case, a priority scheme can be used: **assign each unit a unique number, and then make the lower numbered unit wait for the higher numbered unit**. This will force one of the units to proceed if both are waiting. When obstacles are detected ahead of time, the lower numbered unit should slow down before reaching the expected point of collision.

## Coordinated movement

The technique described above does not work when units are trying to move through a narrow corridor. If one unit stands still while the other tries to go around, the corridor can’t be used by both units. One unit will block it while the other one will take a long path around.

It should be possible for the second unit to communicate with the first one, and ask it to back up. Once the corridor is clear, the second unit can pass through, and then the first unit can go through. This may be complicated to implement unless you can identify the corridors beforehand. For randomly generated maps, it could be very difficult to determine where the corridor is and how far the first unit needs to back up.