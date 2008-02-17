--- Map editor GUI functions
--- Most map editor stuff related to the GUI will be in this file

require("wx")

frame = nil

function mapeditorgui_init()

    frame = wx.wxFrame( wx.NULL, wx.wxID_ANY, "PVN Layers",
                        wx.wxDefaultPosition, wx.wxSize(350, 450),
                        wx.wxMINIMIZE_BOX + wx.wxSYSTEM_MENU + wx.wxCAPTION + wx.wxCLIP_CHILDREN )

    -- create a simple file menu
    local fileMenu = wx.wxMenu()
    fileMenu:Append(wx.wxID_EXIT, "E&xit", "Quit the program")

    -- create a simple help menu
    local helpMenu = wx.wxMenu()
    helpMenu:Append(wx.wxID_ABOUT, "&About", "About the wxLua Minimal Application")

    -- create a menu bar and append the file and help menus
    local menuBar = wx.wxMenuBar()
    menuBar:Append(fileMenu, "&File")
    menuBar:Append(helpMenu, "&Help")
    
    -- attach the menu bar into the frame
    frame:SetMenuBar(menuBar)
    
    -- create a simple status bar
    frame:CreateStatusBar(1)
    frame:SetStatusText("Welcome to wxLua.")
    
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
      
        if engine_should_exit_game() == 1 then
          frame:Close(true)
          return
        end
        
        engine_tick()   -- C++ Main Engine Update()
        
        event:Skip()
        event:RequestMore() -- make wx keep firing this event
        
      end
    )
    
    -- connect the selection event of the about menu item
    frame:Connect(wx.wxID_ABOUT, wx.wxEVT_COMMAND_MENU_SELECTED,
       function (event)
            wx.wxMessageBox('This is the "About" dialog of the Minimal wxLua sample.\n'..
                            wxlua.wxLUA_VERSION_STRING.." built with "..wx.wxVERSION_STRING,
                            "About wxLua",
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
  frame:Close(true)
  frame = nil
end