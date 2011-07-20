// pipelist.cpp (Windows NT/2000)
//
// This example will show how you can enumerate all named pipes
// active on a system.
//
// (c)2000 Ashot Oganesyan K, SmartLine, Inc
// mailto:ashot@aha.ru, http://www.protect-me.com, http://www.codepile.com

#include <windows.h>
#include <stdio.h>

#define FileDirectoryInformation 1
#define STATUS_NO_MORE_FILES 0x80000006L

typedef struct
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct
{
    LONG Status;
    ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    union {
        struct {
            WCHAR FileName[1];
        } FileDirectoryInformationClass;

        struct {
            DWORD dwUknown1;
            WCHAR FileName[1];
        } FileFullDirectoryInformationClass;

        struct {
            DWORD dwUknown2;
            USHORT AltFileNameLen;
            WCHAR AltFileName[12];
            WCHAR FileName[1];
	} FileBothDirectoryInformationClass;
    };
} FILE_QUERY_DIRECTORY, *PFILE_QUERY_DIRECTORY;


// ntdll!NtQueryDirectoryFile (NT specific!)
//
// The function searches a directory for a file whose name and attributes
// match those specified in the function call.
//
// NTSYSAPI
// NTSTATUS
// NTAPI
// NtQueryDirectoryFile(
//    IN HANDLE FileHandle,                      // handle to the file
//    IN HANDLE EventHandle OPTIONAL,
//    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
//    IN PVOID ApcContext OPTIONAL,
//    OUT PIO_STATUS_BLOCK IoStatusBlock,
//    OUT PVOID Buffer,                          // pointer to the buffer to receive the result
//    IN ULONG BufferLength,                     // length of Buffer
//    IN FILE_INFORMATION_CLASS InformationClass,// information type
//    IN BOOLEAN ReturnByOne,                    // each call returns info for only one file
//    IN PUNICODE_STRING FileTemplate OPTIONAL,  // template for search
//    IN BOOLEAN Reset                           // restart search
// );
typedef LONG (WINAPI *PROCNTQDF)( HANDLE,HANDLE,PVOID,PVOID,PIO_STATUS_BLOCK,PVOID,ULONG,
                                  UINT,BOOL,PUNICODE_STRING,BOOL );

PROCNTQDF NtQueryDirectoryFile;

void main(void)
{
    LONG ntStatus;
    IO_STATUS_BLOCK IoStatus;
    HANDLE hPipe;
    BOOL bReset = TRUE;
    PFILE_QUERY_DIRECTORY DirInfo,
                          TmpInfo;


    NtQueryDirectoryFile = (PROCNTQDF)GetProcAddress(
                                      GetModuleHandle("ntdll"),
                                      "NtQueryDirectoryFile"
                                      );

    if (!NtQueryDirectoryFile)
       return;

    hPipe = CreateFile("\\\\.\\Pipe\\",GENERIC_READ,
                       FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                       NULL,OPEN_EXISTING,0,NULL);

   if(hPipe == INVALID_HANDLE_VALUE)
     return;

   DirInfo = (PFILE_QUERY_DIRECTORY) new BYTE[1024];

   printf("Pipe name (Number of instances, Maximum instances)\n\n");
   while(1)
   {
       ntStatus = NtQueryDirectoryFile(hPipe,NULL,NULL,NULL,&IoStatus,DirInfo,1024,
                                       FileDirectoryInformation,FALSE,NULL,bReset);

       if (ntStatus!=NO_ERROR)
       {
          if (ntStatus == STATUS_NO_MORE_FILES)
             break;

          return;
       }

       TmpInfo = DirInfo;
       while(1)
       {
          if(TmpInfo->NextEntryOffset==0)
            break;

          TmpInfo->FileDirectoryInformationClass.FileName[TmpInfo->FileNameLength/sizeof(WCHAR)] = NULL;

          wprintf(L"%s (%d, %d)\n",TmpInfo->FileDirectoryInformationClass.FileName,
                                   TmpInfo->EndOfFile.LowPart,
                                   TmpInfo->AllocationSize.LowPart );

          TmpInfo = (PFILE_QUERY_DIRECTORY)((DWORD)TmpInfo+TmpInfo->NextEntryOffset);
       }

       bReset = FALSE;
   }

   delete DirInfo;
   CloseHandle(hPipe);
}
