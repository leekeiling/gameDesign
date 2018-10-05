core = require("FP.core")


Scene = Scene or {}


--to do
function Scene.Load(path)
	core.Load(path)
end

function Scene.RandomInit(path)
	core.RandomInit(path)
end

function Scene.Init( ... )
	-- to do
end

--to do
--need extra arguments to set agent's data
function Scene.AddAgent( id, ... )
	-- body
end


function Scene.RemoveAgent( id )
	core.RemoveAgent(id)
end

