--- Map editor GUI functions
--- Most map editor stuff related to the GUI will be in this file

require("wx")

frame = nil

function init()

    frame = wx.wxFrame( wx.NULL, wx.wxID_ANY, "wxLua Very Minimal Demo",
                        wx.wxDefaultPosition, wx.wxSize(450, 450),
                        wx.wxDEFAULT_FRAME_STYLE )

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

function update()

  wx.wxGetApp():MainLoop()

  --engine_print("update().\n")
  
  --while wx.wxGetApp():Pending() do
    --wx.wxGetApp():Dispatch()
    --engine_print("dispatch().\n")
  --end
  
  -- local idleEvent = wx.wxIdleEvent()
  -- wx.wxGetApp():SendIdleEvents(frame, idleEvent)
  --frame:ProcessEvent(idleEvent)
  -- wx.wxGetApp():ProcessIdle()
end

function shutdown()
  
end