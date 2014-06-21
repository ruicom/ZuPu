#include <stdio.h>
#include <windows.h> 
void find(char * lpPath)
{
char szFind[MAX_PATH],szFile[MAX_PATH];
WIN32_FIND_DATA FindFileData;
strcpy(szFind,lpPath);
strcat(szFind,"\\*.*");
HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
if(INVALID_HANDLE_VALUE == hFind)
return;
while(TRUE)
{
if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
{
   if(FindFileData.cFileName[0]!='.')
   {
    strcpy(szFile,lpPath);
    strcat(szFile,"\\");
    strcat(szFile,FindFileData.cFileName);
    find(szFile);
   }
}
else
{
   printf("%s\n",FindFileData.cFileName);
}
if(!FindNextFile(hFind,&FindFileData))
   break;
}
}
void main()
{
char filepath[MAX_PATH]="E:\\我的文件夹\\测试文件夹";
find(filepath);
}