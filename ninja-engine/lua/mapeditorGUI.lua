--- Map editor GUI functions
--- Most map editor stuff related to the GUI will be in this file

require("wx")

-- Game engine singleton variables
-- These give us some nice shortcuts without having to type tons of crap
local Game         = engine.Game_GetInstance()
local WORLD             = engine.GameWorld_GetInstance()

-- Global variables
local dialog        = nil -- the wxDialog main toplevel window
local xmlResource   = nil -- the XML resource handle

-- Generate a unique new wxWindowID
local ID_IDCOUNTER = wx.wxID_HIGHEST + 1
function NewID()
    ID_IDCOUNTER = ID_IDCOUNTER + 1
    return ID_IDCOUNTER
end

local ID_FILE_SAVE = NewID()

-- ---------------------------------------------------------------------------
-- Handle the quit button event
function OnQuit(event)
    event:Skip()
    dialog:Show(false)
    dialog:Destroy()
end

function dum()
  wx.wxMessageBox( "DUM", "DUM", wx.wxOK + wx.wxICON_INFORMATION, dialog)
end

function OnBtn(event)
  dum()  
end

function OnLayerUp(event)
  dum()
end

function OnLayerDown(event)
  dum()
end

function mapeditorgui_init()

    xmlResource = wx.wxXmlResource()
    xmlResource:InitAllHandlers()
    local xrcFilename = "data/mapeditor/mapEditorGui.xrc"
	
    if (xmlResource:Load(xrcFilename) == false) then
		engine_print("ERROR: Can't load XRC file: " .. xrcFilename .. "\n")
		return;
	end

    dialog = wx.wxDialog()
		
	local topLevelDlgName = "dlgLayers"
	if	(xmlResource:LoadDialog(dialog, wx.NULL, topLevelDlgName) == false 
		or dialog == nil
		) then
		engine_print("ERROR: Failed loading dialog from XRC: " .. topLevelDlgName .. "\n")
		return;
	end
	
    -- IDLE event calls the game engine Tick(), which is normally
    -- called by the main loop, but can't be called if our GUI is active
    dialog:Connect(wx.wxEVT_IDLE,
      function(event)
      
        if Game:ShouldExit() == true then
          dialog:Close(true)
          return
        end
        
        Game:Tick()
        
        event:Skip()
        event:RequestMore() -- make wx keep firing this event
        
      end
    )
    
    bestSize = dialog:GetBestSize()
    
    ID_BTN1        = xmlResource.GetXRCID("button_1")
    ID_LAYERCHANGE = xmlResource.GetXRCID("btnLayerChange")
    
    dialog:Connect(ID_BTN1, wx.wxEVT_COMMAND_BUTTON_CLICKED, OnBtn)
    --dialog:Connect(ID_LAYERCHANGE, wx.wxEVT_SPIN_UP, OnLayerUp)
    --dialog:Connect(ID_LAYERCHANGE, wx.wxEVT_SPIN_DOWN, OnLayerDown)
    dialog:Connect(wx.wxEVT_CLOSE_WINDOW, OnQuit)

    dialog:Show(true)
    -- dialog:Centre() -- CRASHES UNDER LINUX???
end

function mapeditorgui_update()
  -- do update-ish things here
end

function mapeditorgui_run()
  wx.wxGetApp():MainLoop()
end

function mapeditorgui_shutdown()
  if dialog == nil then
    return
  end
  dialog:Close(true)
  dialog = nil
end
