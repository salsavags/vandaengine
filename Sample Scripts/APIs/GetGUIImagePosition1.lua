--Copyright (C) 2024 Ehsan Kamrani 
--This file is licensed and distributed under MIT license

--Explanations: https://vanda3d.org/getguiimageposition/

x = 0
y = 0
function OnSelectMouseLButtonDown()
		x,y = GetGUIImagePosition("gui_test_test", "image1")

		message = string.format("\nGUI image position is > %d, %d" , x,y)
		PrintConsole(message)
end

function OnSelectMouseRButtonDown()

end

function OnSelectMouseEnter()

end
