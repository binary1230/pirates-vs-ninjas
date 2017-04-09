-- Level 2 - Jouten's Lair

function DoCutBars()
	world_create_cutbars("Pirates VS Ninjas|Level 2 - Jouten's Lair");
end

function Level_OnLoad() 

	-- Don't do anything if we're warping back from a door
	if jumped_back_from_a_door() == 1 then
		DoCutBars();
		return;
	end

	msg = "You found the|'cosmic door'.| |Huzzah.~As a reward, I'll|tell you a little|secret.  If you press|the \"D\" key,|you will drop little|airballs.~Don't ask me why|Jouten can do it.|He's just a|tech guy.~I mean, Toneriko|is understandable,|since she's...| |Oops.|No plot for you yet!~Anyway, press \"C\"|to start.  Try and|find that 'cosmic|door' again.";

	world_textbox(msg, "amaya-icon.png");
end

function Textbox_Done()
	DoCutBars();
end
