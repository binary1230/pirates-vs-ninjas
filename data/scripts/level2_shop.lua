-- Level 2 - Jouten's Lair

function DoCutBars()
	world_create_cutbars("Pirates VS Ninjas|Jouten's Tech Shop");
end

function Level_OnLoad() 
	DoCutBars();
end

function OnPickupItem(itemname)
	if itemname == "bomb" then
		msg = "ITEM GET: Ninja bombs.|(Definitely better than plain old regular bombs)|Press D to drop them and break blocks.";

		world_textbox(msg, "bomb-icon.png");
	end
end

function Textbox_Done()
	DoCutBars();
end
