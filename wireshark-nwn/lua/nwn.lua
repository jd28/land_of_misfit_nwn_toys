local nwn_proto = Proto("nwn", "NWN Network Protocol")

local f = nwn_proto.fields

f.packet_type = ProtoField.uint8("nwn.packet_type", "Packet Type", base.HEX, 
                                 { [0x4d] = "In game packet",
                                   [0x42]   = "Out of game packet" })

f.protocol  = ProtoField.uint8("nwn.protocol", "Protocol", base.HEX,
                               { [0x4e] = "Neverwinter Nights" })

f.msg_type  = ProtoField.string("nwn.msg_type", "Message Type")
f.checksum  = ProtoField.uint16("nwn.checksum", "Checksum", base.HEX)
f.packet_id = ProtoField.uint16("nwn.packet_id", "Packet ID", base.HEX)
f.session   = ProtoField.uint16("nwn.session", "Last Received Packet", base.HEX)
f.flags     = ProtoField.uint8("nwn.flags", "Header Flags", base.HEX,
                               { [0x8]  = "Continued Message",
                                 [0xA]  = "Simple Message",
                                 [0xE]  = "Compressed Message (C-S)",
                                 [0xF]  = "Compressed Message (S-C)",
                                 [0x10] = "Ping/Acknowledge", })

f.parts     = ProtoField.uint16("nwn.parts", "Parts")
f.size      = ProtoField.uint16("nwn.size", "Size", base.HEX)
f.direction = ProtoField.uint8("nwn.direction", "Direction", base.HEX)

f.sc_msg_type = ProtoField.uint8("nwn.sc_msg_type", "Server-Client Message Type", base.HEX,
                                 { [0x1]  = "ServerStatus",
                                   [0x2]  = "Login",
                                   [0x3]  = "Module",
                                   [0x4]  = "Area",
                                   [0x5]  = "Game Object Update",
                                   [0x7]  = "Store",
                                   [0x9]  = "Chat",
                                   [0xA]  = "PlayerList",
                                   [0xC]  = "Inventory",
                                   [0xD]  = "GuiInventory",
                                   [0xE]  = "Party",
                                   [0xF]  = "Cheat",
                                   [0x10] = "Camera",
                                   [0x11] = "Charlist",
                                   [0x12] = "ClientSideMessage",  
                                   [0x13] = "CombatRound",
                                   [0x14] = "Dialog",
                                   [0x15] = "GUICharacterSheet",
                                   [0x16] = "QuickChat",
                                   [0x17] = "Sound",
                                   [0x18] = "ItemProperty",
                                   [0x19] = "GuiContainer",
                                   [0x1A] = "VoiceChat",
                                   [0x1B] = "GuiInfoPopup",
                                   [0x1C] = "Journal",
                                   [0x1D] = "LevelUp",
                                   [0x1E] = "GUIQuickBar",
                                   [0x1F] = "DungeonMaster",
                                   [0x20] = "MapPin",
                                   [0x21] = "DebugInfo",
                                   [0x22] = "SafeProjectile",
                                   [0x23] = "Barter",
                                   [0x24] = "PopUpGUIPanel",
                                   [0x28] = "Ambient",
                                   [0x29] = "PVP",
                                   [0x2A] = "Portal",
                                   [0x2B] = "Character_Download",
                                   [0x2C] = "LoadBar",
                                   [0x2D] = "SaveLoad",
                                   [0x2E] = "GuiPartyBar",
                                   [0x2F] = "ShutDownServer",
                                   [0x30] = "LevelUp",
                                   [0x31] = "PlayModuleCharaterList",
                                   [0x32] = "CustomToken",
                                   [0x33] = "Cutscene", })

f.sc_msg_subtype = ProtoField.uint8("nwn.sc_msg_subtype", "Server-Client Message SubType", base.HEX)

f.raw_data = ProtoField.bytes("nwn.raw_data", "Raw Data")

local nwn_sc_messege_dissectors = {}
local function register_sc_dissector(type, f)
   nwn_sc_messege_dissectors[type] = f
end

register_sc_dissector(
   0x12,
   function(buffer, pinfo, tree, offset)
      tree:add(f.raw_data, buffer(offset, buffer:len() - offset))
   end)	      

local function dissect_message_data(buffer, pinfo, tree, offset)
   local direct = buffer(offset, 1):uint()
   tree:add(f.direction, buffer(offset, 1))
   offset = offset + 1
   if direct == 0x50 then
      local t = buffer(offset, 2):uint()
      tree:add(f.sc_msg_type, buffer(offset, 1))
      offset = offset + 1
      tree:add(f.sc_msg_subtype, buffer(offset, 1))
      offset = offset + 1

      local f = nwn_sc_messege_dissectors[t]
      if f and type(f) == "function" then
         f(buffer, pinfo, tree, offset)
      end
   end
end

local function disset_message_header(buffer, pinfo, tree, offset)
   tree:add(f.flags, buffer(offset, 1))
   offset = offset + 1
   tree:add(f.parts, buffer(offset, 2))
   offset = offset + 2
   tree:add(f.size, buffer(offset, 2))
   offset = offset + 2

   if offset >= buffer:len() then return end

   dissect_message_data(buffer, pinfo, tree, offset)
end

local function dissect_in_game(buffer, pinfo, tree)
   local offset = 1
   pinfo.cols.info:set("In-Game")
   local subtree = tree:add("In-Game Packet")
   subtree:add(f.checksum, buffer(offset, 2))
   offset = offset + 2

   pinfo.cols.info:append(string.format(", PacketID: %x", buffer(offset, 2):uint()))
   subtree:add(f.packet_id, buffer(offset, 2))
   offset = offset + 2

   pinfo.cols.info:append(string.format(", LastPacket: %x", buffer(offset, 2):uint()))
   subtree:add(f.session, buffer(offset, 2))
   offset = offset + 2
   
   disset_message_header(buffer, pinfo, subtree, offset)
end

function nwn_proto.dissector(buffer, pinfo, tree)
   pinfo.cols.protocol = "NWN"
   local tree = tree:add(nwn_proto, buffer(0, 1), "NWN Protocol Data")
   local packet_type = buffer(0, 1):uint()

   tree = tree:add(f.packet_type, buffer(0, 1))
   if packet_type == 0x4d then
      dissect_in_game(buffer, pinfo, tree)
   end
end

-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
-- register our protocol to handle udp port 7777
udp_table:add(5126, nwn_proto)