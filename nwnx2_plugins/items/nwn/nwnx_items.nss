const int NWNX_ITEMS_EVENT_ALL               = 0;
const int NWNX_ITEMS_EVENT_CAN_EQUIP         = 1;
const int NWNX_ITEMS_EVENT_CAN_UNEQUIP       = 2;
const int NWNX_ITEMS_EVENT_MIN_LEVEL         = 3;
const int NWNX_ITEMS_EVENT_CAN_USE           = 4;
const int NWNX_ITEMS_EVENT_CALC_BASE_COST    = 5;
const int NWNX_ITEMS_EVENT_NUM               = 6;

int NWNXItems_GetDefaultILR(object item);
int NWNXItems_GetEventType();
object NWNXItems_GetEventObject();
object NWNXItems_GetEventItem();
int NWNXItems_SetEventHandler(string script);
void NWNXEqiups_SetHelmetHidden(object pc, int val = TRUE);
void NWNXItems_SetResult(int result);

int NWNXItems_GetDefaultILR(object item) {
    SetLocalString(item, "NWNX!ITEMS!GET_DEFAULT_ILR", "        ");
    return StringToInt(GetLocalString(item, "NWNX!ITEMS!GET_DEFAULT_ILR"));
}
!
int NWNXItems_GetEventType() {
    SetLocalString(GetModule(), "NWNX!ITEMS!GET_TYPE", "       ");
    return StringToInt(GetLocalString(GetModule(), "NWNX!ITEMS!GET_TYPE"));
}

object NWNXItems_GetEventObject() {
    return GetLocalObject(GetModule(), "NWNX!ITEMS!OBJECT");
}

object NWNXItems_GetEventItem() {
    return GetLocalObject(GetModule(), "NWNX!ITEMS!ITEM");
}

int NWNXItems_SetEventHandler(int type, string script) {
    string s = "SET_EQUIP_HANDLER_" + IntToString(type);
    SetLocalString(GetModule(), s, script);
    return GetLocalString(GetModule(), s);
}

void NWNXItems_SetHelmetHidden(object pc, int val = TRUE) {
    SetLocalInt(pc, "NWNX_HELM_HIDDEN", val);
}

void NWNXItems_SetResult(int result) {
    SetLocalString(GetModule(), "NWNX!ITEMS!SET_RESULT", IntToString(result));
}
