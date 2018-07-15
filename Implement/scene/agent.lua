local core = require("PF.core")

Agent = Agent or {}

--[[
to do set agent's member
]]--


--to do
function Agent.Init( ... )
	-- to do
end


function Agent.SetPosition( idx, x, y, z )
	core.SetAIPosition(idx, x, y, z)
end

function Agent.GetPosition( idx)
	core.GetPosition(idx)
end

function Agent.SetTarget( idx, x, y, z )
	core.SetTarget(idx, x, y, z)
end

function Agent.GetTarget( idx )
	core.GetTarget(idx)
end

function Agent.SetVelocity( idx, vx, vy, vz )
	core.SetVeloctiy(idx, vx, vy, vz)
end

function Agent.GetVelocity( idx )
	core.GetVelocity(idx)
end

function Agent.GetCurrentPosition( idx )
	--to do
end

--to do
function Agent.SetIncludeFilter( ... )
	core.SetIncludeFilter(...)
end


--to do
function Agent.SetExcludeFilter( ... )
	core.SetExcludeFilter(...)
end

--to do
function Agent.SetAreaCost(...)
	core.SetAreaCost(...)
end

--to do to update the agent's current position
function Agent.Update( ... )
	-- body
end






