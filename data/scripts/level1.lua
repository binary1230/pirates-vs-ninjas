-- Level 1 - Treasure Island

introState = 1;          -- to skip set to 9
done_intro = false;

function UpdateIntroState() 
	if done_intro then
		return;
	end

	if introState == 1 then

		music_play("bassy1.ogg");
		world_play_input_script("input/level1.1.input");

	elseif introState == 2 then

		world_textbox("Sup. It's a good day to be a sweet, sweet ninja.| |(Press C to continue)~I should meet up with my friend~I'll just head up from here.", "amaya-icon.png");
		
	elseif introState == 3 then

		world_create_cutbars("Pirates VS Ninjas|Level 1 - Treasure Island");

		-- Now they can't go offscreen
		world_allow_player_offscreen(0);

		-- start the game for real.
		done_intro = true;
		music_play("level1.ogg");
	end
	
	introState = introState+1;
end

function Level_OnLoad() 
	
	if jumped_back_from_a_door() == 1 then
		done_intro = true;
		return
	end

	window_fadein(6);

	-- until the cinematic is over, they can go offscreen
	world_allow_player_offscreen(1);

	UpdateIntroState();

end

function Input_DonePlaying() 
	UpdateIntroState();	
end

function Textbox_Done()
	UpdateIntroState();
end
