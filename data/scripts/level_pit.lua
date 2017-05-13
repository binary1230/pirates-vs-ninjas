function Level_OnLoad() 
	if jumped_back_from_a_door() == 1 then
		return;
	end
	
	world_create_cutbars("Pirates VS Ninjas|Tectonic Gateway");
end