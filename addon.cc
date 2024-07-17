#include <napi.h>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

std::string GetPrivilege(const std::string& username) {
    LOCALGROUP_USERS_INFO_0 *groups = NULL;
    DWORD entries = 0, total = 0;
    NET_API_STATUS nStatus;

    nStatus = NetUserGetLocalGroups(NULL, username.c_str(), 0, LG_INCLUDE_INDIRECT, (LPBYTE*)&groups, MAX_PREFERRED_LENGTH, &entries, &total);
    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < entries; ++i) {
            if (wcscmp(groups[i].lgrui0_name, L"Администраторы") == 0) {
                NetApiBufferFree(groups);
                return "Администратор";
            }
            if (wcscmp(groups[i].lgrui0_name, L"Пользователи") == 0) {
                NetApiBufferFree(groups);
                return "Пользователь";
            }
        }
        NetApiBufferFree(groups);
        return "Гость";
    }
    return "";
}

Napi::String GetPrivilegeWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    }
    Napi::String username = info[0].As<Napi::String>();
    std::string privilege = GetPrivilege(username.Utf8Value());
    return Napi::String::New(env, privilege);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getPrivilege"), Napi::Function::New(env, GetPrivilegeWrapped));
    return exports;
}

NODE_API_MODULE(addon, Init)
