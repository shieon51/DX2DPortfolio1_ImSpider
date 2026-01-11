#pragma once

class Utility
{
public:
    static vector<string> SplitString(string origin, const string& tok);

    static string ToString(wstring str);
    static wstring ToWString(string str);
};