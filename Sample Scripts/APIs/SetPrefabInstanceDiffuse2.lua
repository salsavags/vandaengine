--Copyright (C) 2024 Ehsan Kamrani 
--This file is licensed and distributed under MIT license

--Explanations: https://vanda3d.org/setprefabinstancediffuse/

--Script name is SetPrefabInstanceDiffuse2.lua

function Init()
	EnablePrefabInstanceMaterial("this")
	SetPrefabInstanceDiffuse("this", 0.75, 0.5, 0.25)
end

function Update()

end
