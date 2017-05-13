-- Level 2 - Jouten's Lair

function DoCutBars()
	world_create_cutbars("Pirates VS Ninjas|Level 2 - Treasure Cave");
end

function Level_OnLoad() 

	-- Don't do anything if we're warping back from a door
	if jumped_back_from_a_door() == 1 then
		DoCutBars();
		return;
	end

	msg = "My friend's shop is up ahead|and should have more useful NINJA SCIENCE~I'll head over there";

	world_textbox(msg, "amaya-icon.png");
end

function Textbox_Done()
	DoCutBars();
end
