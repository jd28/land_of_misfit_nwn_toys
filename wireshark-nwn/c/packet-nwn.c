/* packet-nwn.c
 * Routines for Neverwinter Nights Protocol dissection
 * Copyright 2007, virusman <virusman@virusman.ru>
 *
 * $Id: packet-nwn.c 18197 2006-05-21 05:12:17Z virusman $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * Copied from WHATEVER_FILE_YOU_USED (where "WHATEVER_FILE_YOU_USED"
 * is a dissector file; if you just copied this from README.developer,
 * don't bother with the "Copied from" - you don't even need to put
 * in a "Copied from" if you copied an existing dissector, especially
 * if the bulk of the code in the new dissector is your code)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmodule.h>
#include <epan/packet.h>
#include <epan/prefs.h>

#define PACKET_TYPE_OUT_OF_GAME	0x42
#define PACKET_TYPE_IN_GAME		0x4D
#define PROTOCOL_NWN				0x4E

#define NWN_DEFAULT_PORT 5121
static unsigned int gbl_nwnServerPort=NWN_DEFAULT_PORT;
static dissector_handle_t nwn_handle;

const value_string nwn_packet_type_value[] = {
 	{ PACKET_TYPE_OUT_OF_GAME,			"Out of game packet" },
	{ PACKET_TYPE_IN_GAME,				"In-game packet" },
  	{ 0,                                  	NULL }
};
const value_string nwn_protocol_value[] = {
 	{ PROTOCOL_NWN,						"Neverwinter Nights" },
  	{ 0,                                NULL }
};
const value_string nwn_b_msg_type_value[] = {
  	{ 0,                                NULL }
};
const value_string nwn_m_header_flags_value[] = {
	{ 0x8,						"Continued Message" },
 	{ 0xA,						"Simple Message" },
	{ 0xE,						"Compressed Message (C-S)" },
	{ 0xF,						"Compressed Message (S-C)" },
 	{ 0x10,						"Ping/Acknowledge" },
  	{ 0,                                NULL }
};
const value_string nwn_m_data_sc_msg_type_value[] = {
 	{ 0x1,						"ServerStatus" },
	{ 0x2,						"Login" },
	{ 0x3,						"Module" },
	{ 0x4, 						"Area" },
 	{ 0x5,						"Game Object Update" },
	{ 0x7,						"Store" },
	{ 0x9,						"Chat" },
	{ 0xA,						"PlayerList" },
	{ 0xC,						"Inventory" },
	{ 0xD,						"GuiInventory" },
	{ 0xE,						"Party" },
	{ 0xF,						"Cheat" },
	{ 0x10,						"Camera" },
	{ 0x11, 					"Charlist" },
	{ 0x12,						"ClientSideMessage" },	
	{ 0x13, 					"CombatRound" },
	{ 0x14, 					"Dialog" },
	{ 0x15, 					"GUICharacterSheet" },
	{ 0x16, 					"QuickChat" },
	{ 0x17, 					"Sound" },
	{ 0x18, 					"ItemProperty" },
	{ 0x19, 					"GuiContainer" },
	{ 0x1A, 					"VoiceChat" },
	{ 0x1B, 					"GuiInfoPopup" },
	{ 0x1C, 					"Journal" },
	{ 0x1D, 					"LevelUp" },
	{ 0x1E, 					"GUIQuickBar" },
	{ 0x1F, 					"DungeonMaster" },
	{ 0x20, 					"MapPin" },
	{ 0x21, 					"DebugInfo" },
	{ 0x22, 					"SafeProjectile" },
	{ 0x23, 					"Barter" },
	{ 0x24, 					"PopUpGUIPanel" },	
	{ 0x28, 					"Ambient" },
	{ 0x29, 					"PVP" },
	{ 0x2A, 					"Portal" },
	{ 0x2B, 					"Character_Download" },
	{ 0x2C, 					"LoadBar" },
	{ 0x2D, 					"SaveLoad" },
	{ 0x2E, 					"GuiPartyBar" },
	{ 0x2F, 					"ShutDownServer" },
	{ 0x30, 					"LevelUp" },
	{ 0x31, 					"PlayModuleCharaterList" },
	{ 0x32, 					"CustomToken" },
	{ 0x33, 					"Cutscene" },
  	{ 0,                                NULL }
};
const value_string nwn_m_data_cs_msg_type_value[] = {
	{ 0x7,						"Store" },
  	{ 0,                                NULL }
};





/*
const value_string sm_message_type_value_info[] = {
        { MESSAGE_TYPE_START,                   "Start" },
        { MESSAGE_TYPE_STOP,                    "Stop" },
        { MESSAGE_TYPE_ACTIVE,                  "Active" },
        { MESSAGE_TYPE_STANDBY,                 "Standby" },
        { MESSAGE_TYPE_Q_HOLD_INVOKE,           "Q_HOLD Invoke" },
        { MESSAGE_TYPE_Q_HOLD_RESPONSE,         "Q_HOLD Response" },
        { MESSAGE_TYPE_Q_RESUME_INVOKE,         "Q_RESUME Invoke" },
        { MESSAGE_TYPE_Q_RESUME_RESPONSE,       "Q_RESUME Response" },
        { MESSAGE_TYPE_Q_RESET_INVOKE,          "Q_RESET Invoke" },
        { MESSAGE_TYPE_Q_RESET_RESPONSE,        "Q_RESET Response" },
        { MESSAGE_TYPE_PDU,                     "PDU" },
        { 0,                                    NULL }
};*/


/*const value_string sm_pdu_type_value[] = {
	{ PDU_MTP3_TO_SLT,			"mtp3 to SLT"},
	{ PDU_MTP3_FROM_SLT, 			"mtp3 from SLT"},
	{ PDU_SET_STATE, 			"set session state"},
	{ PDU_RETURN_STATE,			"return session state"},
        { 0,                                    NULL }
};*/





/* Initialize the protocol and registered fields */
static int proto_nwn = -1;

static int hf_nwn_msg_type = -1;
static int hf_nwn_protocol = -1;
static int hf_nwn_b_msg_type = -1;
static int hf_nwn_m_checksum = -1;
static int hf_nwn_m_query = -1;
static int hf_nwn_m_session = -1;
static int hf_nwn_m_header_flags = -1;
static int hf_nwn_m_header_parts = -1;
static int hf_nwn_m_header_size = -1;
static int hf_nwn_m_data_direction = -1;
static int hf_nwn_m_data_sc_msg_type = -1;
static int hf_nwn_m_data_sc_msg_subtype = -1;
static int hf_nwn_m_data_cs_msg_type = -1;

static int hf_nwn_m_data_msg_cs_store_4_obj = -1;
static int hf_nwn_m_data_msg_cs_store_4_1 = -1;
static int hf_nwn_m_data_msg_cs_store_4_markup = -1;
static int hf_nwn_m_data_msg_cs_store_4_markdown = -1;
static int hf_nwn_m_data_msg_cs_store_4_bmmarkdown = -1;
static int hf_nwn_m_data_msg_cs_store_4_identifyprice = -1;
static int hf_nwn_m_data_msg_cs_store_4_maxbuyprice = -1;
static int hf_nwn_m_data_msg_cs_store_4_storegold = -1;
static int hf_nwn_m_data_msg_cs_store_4_blackmarket = -1;

/* Initialize the subtree pointers */
static gint ett_nwn = -1;
static gint ett_nwn_M = -1;
static gint ett_nwn_B = -1;
static gint ett_nwn_message = -1;
static gint ett_nwn_message_data = -1;

static gint recursive = 0;



static void
dissect_nwn_B(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
	if (check_col(pinfo->cinfo, COL_INFO))
	{
		col_set_str(pinfo->cinfo, COL_INFO, "Out of game");
	}
	proto_tree_add_item(tree, hf_nwn_protocol, tvb, 1, 1, FALSE);
	proto_tree_add_item(tree, hf_nwn_b_msg_type, tvb, 2, 2, FALSE);
}

static void
dissect_nwn_message_data_CS_Store(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, guint8 msg_subtype)
{
	gint offset = 0;
	switch(msg_subtype)
	{
		case 0x4:
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_obj, tvb, offset, 4, TRUE); offset+=4;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_1, tvb, offset, 1, TRUE); offset+=1;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_markup, tvb, offset, 2, TRUE); offset+=2;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_markdown, tvb, offset, 1, TRUE); offset+=1;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_bmmarkdown, tvb, offset, 1, TRUE); offset+=1;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_identifyprice, tvb, offset, 4, TRUE); offset+=4;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_maxbuyprice, tvb, offset, 4, TRUE); offset+=4;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_storegold, tvb, offset, 4, TRUE); offset+=4;
			proto_tree_add_item(tree, hf_nwn_m_data_msg_cs_store_4_blackmarket, tvb, offset, 1, TRUE); offset+=1;
			
			break;
	}
}

static void
dissect_nwn_message_data(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
	guint offset = 0;
	guint direction, msg_type, msg_subtype;
	proto_item *message_data;
	proto_tree *message_data_tree;	
	guint rest_length;
	tvbuff_t *next_tvb;

	direction = tvb_get_guint8(tvb,offset);
	proto_tree_add_item(tree, hf_nwn_m_data_direction, tvb, offset, 1, FALSE); offset+=1;
	msg_type = tvb_get_guint8(tvb,offset);
	if(!recursive)
	{
		if (check_col(pinfo->cinfo, COL_INFO))
			col_append_fstr(pinfo->cinfo, COL_INFO, ", Type: %x", msg_type);
	}
	if (direction == 0x50)
		{ proto_tree_add_item(tree, hf_nwn_m_data_sc_msg_type, tvb, offset, 1, FALSE); offset+=1; }
	else
		{ proto_tree_add_item(tree, hf_nwn_m_data_cs_msg_type, tvb, offset, 1, FALSE); offset+=1; }

	msg_subtype = tvb_get_guint8(tvb,offset);
	proto_tree_add_item(tree, hf_nwn_m_data_sc_msg_subtype, tvb, offset, 1, FALSE); offset+=1;
	if(tvb_reported_length(tvb) > offset)
	{
		message_data = proto_tree_add_text(tree, tvb, offset, -1, "Raw Data");
		message_data_tree = proto_item_add_subtree(message_data, ett_nwn_message_data);
		rest_length = tvb_reported_length(tvb) - offset;
		next_tvb = tvb_new_subset(tvb, offset, rest_length , rest_length);
		if(direction == 0x50)
		{
			switch(msg_type)
			{
				case 0x7:
					dissect_nwn_message_data_CS_Store(next_tvb, pinfo, message_data_tree, msg_subtype);
					break;
			}
		}
	}
}

static void
dissect_nwn_message_header(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
	tvbuff_t *next_tvb = NULL;
	guint rest_length, size;
	guint16 offset = 0;
	guint8 flags;
	proto_item *message;
	proto_tree *message_tree;

	/*
	01000 - continued
	01010 - std
	01110 - compressed
	10000 - ping
	*/
	size = tvb_get_ntohs(tvb,3);
	message = proto_tree_add_text(tree, tvb, 0, size+5, "Message");
	message_tree = proto_item_add_subtree(message, ett_nwn_message);
	flags = tvb_get_guint8(tvb, offset);

	if(!recursive)
	{
		if (flags == 0x10 && check_col(pinfo->cinfo, COL_INFO))
			col_append_str(pinfo->cinfo, COL_INFO, ", Ping");
		else if (!(flags & 0x2) && check_col(pinfo->cinfo, COL_INFO))
			col_append_str(pinfo->cinfo, COL_INFO, ", Continued");
		else if ((flags & 0x4 || flags == 0xE) && check_col(pinfo->cinfo, COL_INFO))
			col_append_str(pinfo->cinfo, COL_INFO, ", Compressed");
		/*else if (flags & 0x40 && check_col(pinfo->cinfo, COL_INFO))
			col_append_str(pinfo->cinfo, COL_INFO, ", Resend");*/
	}

	proto_tree_add_item(message_tree, hf_nwn_m_header_flags, tvb, offset, 1, FALSE); offset+=1;
	proto_tree_add_item(message_tree, hf_nwn_m_header_parts, tvb, offset, 2, FALSE); offset+=2;
	proto_tree_add_item(message_tree, hf_nwn_m_header_size, tvb, offset, 2, FALSE); offset+=2;
	size = tvb_get_ntohs(tvb,offset-2);
	if(size>0)
	{
		rest_length = size;
		if(flags == 0xE)
		{
		    guint16 orig_size = size*2;
		    guchar *decompressed_buffer = (guchar*)g_malloc(orig_size);
		    next_tvb = tvb_uncompress(tvb, offset, size);
		    if(next_tvb) {
			tvb_set_child_real_data_tvbuff(tvb, next_tvb);
			add_new_data_source(pinfo, next_tvb, "Decompressed Data");
		    }
		    proto_tree_add_text(message_tree, tvb, offset, -1, "Raw Data");
		    return;
		}
		else
			next_tvb = tvb_new_subset(tvb, offset, rest_length , rest_length);
		if(!next_tvb) next_tvb = tvb_new_subset(tvb, offset, rest_length , rest_length);
		dissect_nwn_message_data(next_tvb, pinfo, message_tree);
	}
	if(size && tvb_reported_length(tvb) > offset + size)
	{
		guint tvb_offset = offset + size + 7;
		rest_length = tvb_reported_length(tvb) - tvb_offset;
		next_tvb = tvb_new_subset(tvb, tvb_offset, rest_length , rest_length);
		if(recursive == 0 && check_col(pinfo->cinfo, COL_INFO))
			col_append_str(pinfo->cinfo, COL_INFO, ", Compound");
		recursive = 1;
		dissect_nwn_message_header(next_tvb, pinfo, tree);
		recursive = 0;
	}
}


static void
dissect_nwn_M(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
	tvbuff_t *next_tvb;
	guint rest_length;
	gint offset = 1;
	proto_item *packet;
	proto_tree *packet_tree;
	guint16 query, session;

	if (check_col(pinfo->cinfo, COL_INFO))
	{
		col_set_str(pinfo->cinfo, COL_INFO, "In-game");
	}
	
	packet = proto_tree_add_text(tree, tvb, offset, -1, "In-Game Packet");
	packet_tree = proto_item_add_subtree(packet, ett_nwn_M);
	proto_tree_add_item(packet_tree, hf_nwn_m_checksum, tvb, offset, 2, FALSE); offset+=2;
	query = tvb_get_ntohs(tvb, offset);
	if (check_col(pinfo->cinfo, COL_INFO))
		col_append_fstr(pinfo->cinfo, COL_INFO, ", PacketID: %x", query);
	proto_tree_add_item(packet_tree, hf_nwn_m_query, tvb, offset, 2, FALSE); offset+=2;
	session = tvb_get_ntohs(tvb, offset);
	if (check_col(pinfo->cinfo, COL_INFO))
		col_append_fstr(pinfo->cinfo, COL_INFO, ", LastPacket: %x", session);
	proto_tree_add_item(packet_tree, hf_nwn_m_session, tvb, offset, 2, FALSE); offset+=2;
	rest_length = tvb_reported_length(tvb) - offset;
	next_tvb = tvb_new_subset(tvb, offset, rest_length , rest_length);
	dissect_nwn_message_header(next_tvb, pinfo, packet_tree);
}



/* Code to actually dissect the packets */
static void
dissect_nwn(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
	proto_item *ti;
	proto_tree *nwn_tree;
	/*tvbuff_t *next_tvb = NULL;*/
	guint8 packet_type;
	/*guint16 msg_type;*/

	packet_type = tvb_get_guint8(tvb,0);  /*this can be B or M*/

	if (check_col(pinfo->cinfo, COL_PROTOCOL)) 
		col_set_str(pinfo->cinfo, COL_PROTOCOL, "NWN");

	if (check_col(pinfo->cinfo, COL_INFO)) 
		col_clear(pinfo->cinfo, COL_INFO);

	if (tree) {
		ti = proto_tree_add_item(tree, proto_nwn, tvb, 0, -1, FALSE);
		nwn_tree = proto_item_add_subtree(ti, ett_nwn);

		proto_tree_add_item(nwn_tree, hf_nwn_msg_type, tvb, 0, 1, FALSE);

		if(packet_type == PACKET_TYPE_OUT_OF_GAME)
			dissect_nwn_B(tvb, pinfo, nwn_tree);
		else if (packet_type == PACKET_TYPE_IN_GAME)
			dissect_nwn_M(tvb, pinfo, nwn_tree);
	}
}

void
proto_reg_handoff_nwn(void)
{
	static int Initialized=FALSE;
	static int ServerPort=0;

	if (Initialized) {
		dissector_delete_uint("udp.port", ServerPort, nwn_handle);
	} else {
		nwn_handle = create_dissector_handle(dissect_nwn, proto_nwn);
		Initialized=TRUE;
	}

	/* set port for future deletes */
	ServerPort=gbl_nwnServerPort;

	dissector_add_uint("udp.port", gbl_nwnServerPort, nwn_handle);
	dissector_add_uint("udp.port", 5125, nwn_handle);
	/*data_handle = find_dissector("data");*/
}


void
proto_register_nwn(void)
{                 
	static hf_register_info hf[] = {
		{ &hf_nwn_msg_type,
			{ "Packet Type",           "nwn.pck_type",
			FT_UINT8, BASE_HEX, VALS(nwn_packet_type_value), 0x0,          
			"", HFILL }
		},
		{ &hf_nwn_protocol,
			{ "Protocol",				"nwn.b.protocol",
			FT_UINT8, BASE_HEX, VALS(nwn_protocol_value), 0x0,          
			"", HFILL }
		},
		{ &hf_nwn_b_msg_type,
			{ "Message Type",			"nwn.b.msg_type",
			FT_STRING, BASE_NONE, NULL, 0x0,			
			"", HFILL }
		},
		{ &hf_nwn_m_checksum,
			{ "Checksum",			"nwn.m.checksum",
			FT_UINT16, BASE_HEX, NULL, 0x0,			
			"", HFILL }
		},
		{ &hf_nwn_m_query,
			{ "Packet ID",			"nwn.m.query",
			FT_UINT16, BASE_HEX, NULL, 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_session,
			{ "Last Recieved Packet",			"nwn.m.session",
			FT_UINT16, BASE_HEX, NULL, 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_header_flags,
			{ "Flags",			"nwn.m.header.flags",
			FT_UINT8, BASE_HEX, VALS(nwn_m_header_flags_value), 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_header_parts,
			{ "Parts",			"nwn.m.header.parts",
			FT_UINT16, BASE_HEX, NULL, 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_header_size,
			{ "Size",			"nwn.m.header.size",
			FT_UINT16, BASE_HEX, NULL, 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_data_direction,
			{ "Direction",			"nwn.m.data.direction",
			FT_UINT8, BASE_HEX, NULL, 0x0, 		
			"", HFILL }
		},
		{ &hf_nwn_m_data_sc_msg_type,
			{ "Message Type",			"nwn.m.data.sc.msg_type",
			FT_UINT8, BASE_HEX, VALS(nwn_m_data_sc_msg_type_value), 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_cs_msg_type,
			{ "Message Type",			"nwn.m.data.cs.msg_type",
			FT_UINT8, BASE_HEX, VALS(nwn_m_data_cs_msg_type_value), 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_sc_msg_subtype,
			{ "Message Subtype",			"nwn.m.data.sc.msg_subtype",
			FT_UINT8, BASE_HEX, NULL, 0x0,		
			"", HFILL }
		},

		{ &hf_nwn_m_data_msg_cs_store_4_obj,
			{ "ObjectID",			"nwn.m.data.cs.store.4.ObjID",
			FT_UINT32, BASE_HEX, NULL, 0x0,		
			"", HFILL }
		},	
		{ &hf_nwn_m_data_msg_cs_store_4_1,
			{ "Unknown",			"nwn.m.data.cs.store.4.arg",
			FT_UINT8, BASE_HEX, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_markup,
			{ "MarkUp",			"nwn.m.data.cs.store.4.MarkUp",
			FT_INT16, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_markdown,
			{ "MarkDown",			"nwn.m.data.cs.store.4.MarkDown",
			FT_INT8, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_bmmarkdown,
			{ "BMMarkDown",			"nwn.m.data.cs.store.4.BMMarkDown",
			FT_INT8, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_identifyprice,
			{ "IdentifyPrice",			"nwn.m.data.cs.store.4.IdentifyPrice",
			FT_INT32, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_maxbuyprice,
			{ "MaxBuyPrice",			"nwn.m.data.cs.store.4.MaxBuyPrice",
			FT_INT32, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_storegold,
			{ "StoreGold",			"nwn.m.data.cs.store.4.StoreGold",
			FT_INT32, BASE_DEC, NULL, 0x0,		
			"", HFILL }
		},
		{ &hf_nwn_m_data_msg_cs_store_4_blackmarket,
			{ "BlackMarket",			"nwn.m.data.cs.store.4.BlackMarket",
			FT_INT8, BASE_DEC, NULL, 0x0, 	
			"", HFILL }
		},
	};

/* Setup protocol subtree array */
	static gint *ett[] = {
		&ett_nwn,
		&ett_nwn_M,
		&ett_nwn_B,
		&ett_nwn_message,
		&ett_nwn_message_data,
	};


/* Register the protocol name and description */
	module_t *nwn_module;
	proto_nwn = proto_register_protocol("NWN Network Protocol",
	    "NWN", "nwn");

	register_dissector("nwn", dissect_nwn, proto_nwn);
/*
	nwn_module = prefs_register_protocol(proto_nwn,
		proto_reg_handoff_nwn);
	prefs_register_uint_preference(nwn_module, "udp.port",
					"NWN Server UDP Port",
					"Set the UDP port for the NWN Server",
					5121, &gbl_nwnServerPort);
*/


/* Required function calls to register the header fields and subtrees used */
	proto_register_field_array(proto_nwn, hf, array_length(hf));
	proto_register_subtree_array(ett, array_length(ett));
}
