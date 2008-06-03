--- Map editor GUI functions
--- Most map editor stuff related to the GUI will be in this file

require("wx")

frame = nil

-- Generate a unique new wxWindowID
local ID_IDCOUNTER = wx.wxID_HIGHEST + 1
function NewID()
    ID_IDCOUNTER = ID_IDCOUNTER + 1
    return ID_IDCOUNTER
end

local ID_FILE_SAVE = NewID()

function mapeditorgui_init()

    --engine_print("SUP FOOLZ")
    --return

    frame = wx.wxFrame( wx.NULL, wx.wxID_ANY, "PVN Map Editor",
                        wx.wxDefaultPosition, wx.wxSize(350, 450),
                        wx.wxMINIMIZE_BOX + wx.wxSYSTEM_MENU + wx.wxCAPTION + wx.wxCLIP_CHILDREN +
                        wx.wxFRAME_TOOL_WINDOW )

    local fileMenu = wx.wxMenu()
    fileMenu:Append(wx.wxID_EXIT, "E&xit", "Quit Map Editor")
    fileMenu:Append(ID_FILE_SAVE, "S&ave", "Save Current Map")
    
    local helpMenu = wx.wxMenu()
    helpMenu:Append(wx.wxID_ABOUT, "&About", "About Pirates VS Ninjas")

    -- create a menu bar and append the file and help menus
    local menuBar = wx.wxMenuBar()
    menuBar:Append(fileMenu, "&File")
    menuBar:Append(helpMenu, "&Help")
    
    -- attach the menu bar into the frame
    frame:SetMenuBar(menuBar)
    
    -- create a simple status bar
    frame:CreateStatusBar(1)
    frame:SetStatusText("PVN Mapeditor")



    -- wx.wxMessageBox('DUMSHIT\n', "About PVN", wx.wxOK, frame)


    
    -- connect the selection event of the exit menu item to an
    -- event handler that closes the window
    frame:Connect(wx.wxID_EXIT, wx.wxEVT_COMMAND_MENU_SELECTED,
      function (event) 
        frame:Close(true) 
      end 
    )
    
    -- IDLE event calls the game engine Tick(), which is normally
    -- called by the main loop, but can't be called if our GUI is active
    frame:Connect(wx.wxEVT_IDLE,
      function(event)
      
        if engine.GameState_GetInstance():ShouldExit() == true then
          frame:Close(true)
          return
        end
        
		engine.GameState_GetInstance():Tick()
        
        event:Skip()
        event:RequestMore() -- make wx keep firing this event
        
      end
    )
    
    -- connect the selection event of the about menu item
    frame:Connect(wx.wxID_ABOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
       function (event)
            wx.wxMessageBox('Pirates VS Ninjas Level Editor (early version)\n'..
                            wxlua.wxLUA_VERSION_STRING.." built with "..wx.wxVERSION_STRING,
                            "About PVN",
                            wx.wxOK + wx.wxICON_INFORMATION,
                            frame)
       end 
    )
    
        -- connect the selection event of the about menu item
    frame:Connect(ID_FILE_SAVE, wx.wxEVT_COMMAND_MENU_SELECTED,
       function (event)
            wx.wxMessageBox('DUMSHIT\n'..
                            wxlua.wxLUA_VERSION_STRING.." built with "..wx.wxVERSION_STRING,
                            "About PVN",
                            wx.wxOK + wx.wxICON_INFORMATION,
                            frame)
       end 
    )

    frame:Show(true)
end

function mapeditorgui_update()
  -- do update-ish things here
end

function mapeditorgui_run()
  wx.wxGetApp():MainLoop()
end

function mapeditorgui_shutdown()
	if frame == nil then
		return
	end
  frame:Close(true)
  frame = nil
end
