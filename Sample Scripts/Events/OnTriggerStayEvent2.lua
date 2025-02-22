--Copyright (C) 2024 Ehsan Kamrani 
--This file is licensed and distributed under MIT license

--Explanations: https://vanda3d.org/ontriggerstay-event/

function OnTriggerStay(otherActorName)
	if otherActorName == nil then
		PrintConsole("\nMain character is being placed inside the trigger and OnTriggerStay() Event was called")
	else
		prefab_instance_name = GetPrefabInstanceNameFromActor(otherActorName)

		message = string.format("\nOnTriggerStay() Event was called. Prefab instance name is : %s" ,prefab_instance_name)
		PrintConsole(message)
	end
end
