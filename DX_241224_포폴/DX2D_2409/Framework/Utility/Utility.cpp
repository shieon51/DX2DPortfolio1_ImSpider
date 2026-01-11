#include "Framework.h"

vector<string> Utility::SplitString(string origin, const string& tok)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    if (origin.size() > 0)
        result.push_back(origin);

    return result;
}

string Utility::ToString(wstring str)
{
    string result;
    result.assign(str.begin(), str.end());

    return result;
}

wstring Utility::ToWString(string str)
{
    wstring result;
    result.assign(str.begin(), str.end());

    return result;
}
