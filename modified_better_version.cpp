#include <windows.h>
#include <stdio.h>

typedef VOID ( _stdcall *RtlSetProcessIsCritical ) (
               IN BOOLEAN        NewValue,
               OUT PBOOLEAN OldValue, 
               IN BOOLEAN     IsWinlogon );

BOOL ProcessIsCritical() 
{
   HANDLE hDLL;
   RtlSetProcessIsCritical fSetCritical;

   hDLL = LoadLibraryA("ntdll.dll");
   if ( hDLL != NULL )
   {
    fSetCritical = (RtlSetProcessIsCritical) GetProcAddress( (HINSTANCE)hDLL, "RtlSetProcessIsCritical" );
    if(fSetCritical) 
    {
      fSetCritical(1,0,0);
      FreeLibrary(hDLL);
      return 1;
    } 
    else
    {
      FreeLibrary(hDLL);
      return 0;
    }
   } else
          return 0;
}

int main (void)
{
   if(ProcessIsCritical())
   {
       MessageBoxA(NULL, "THIS PROCESS IS CRITICAL DO NOT TRY TO KILL IT", "WARNING!", MB_ICONHAND | MB_OK);
   }
   else
   {
       MessageBoxA(NULL, "Unable to set the process as critical", "ERROR!", MB_ICONHAND | MB_OK);
   }
   return 0;
}
//rtselprocessiscritical(0,0,0);
