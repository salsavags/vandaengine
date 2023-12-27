--Copyright (C) 2024 Ehsan Kamrani 
--This file is licensed and distributed under MIT license

--Explanations: https://vanda3d.org/getdirectionalshadowweightofsplits/

value = 0.0

function Init()
	value = GetDirectionalShadowWeightOfSplits()

	message = string.format("\nDirectional shadow weight of splits is : %.2f" ,value)
	PrintConsole(message)
end

function Update()

end
