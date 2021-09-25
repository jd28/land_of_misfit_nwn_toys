int NWNXCallback_Register(object obj, string script, float delay, float step = 0.0f, float min = 0.0f, int retain = FALSE);

int NWNXCallback_Register(object obj, string script, float delay, float step = 0.0f, float min = 0.0f, int retain = FALSE)
{
    string params = FloatToString(delay) + "~" + FloatToString(step) + "~" + FloatToString(min) + "~" + IntToString(retain) + "~" + script;
    SetLocalString(obj, "NWNX!CALLBACK!REGISTER", params);
    return StringToInt(GetLocalString(obj, "NWNX!CALLBACK!REGISTER"));
}
