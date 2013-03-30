// References:
// http://forum.notebookreview.com/7401649-post2249.html
//		http://goo.gl/info/QVrhu#week
//		http://goo.gl/info/1ZRWW#week
// http://msdn.microsoft.com/en-us/library/gg537732(v=VS.85).aspx
// http://stackoverflow.com/questions/118547/creating-a-zip-file-on-windows-xp-2003-in-c-c
// http://stackoverflow.com/questions/597959/sending-the-message-ctrlaltdel-from-my-application

#include <Windows.h>
#include <ShlDisp.h>
#include <cstdio>

const char * ParameterFilename = "OrganizeWindows_ShouldUndoNextTime";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HRESULT hResult;

	hResult = CoInitialize(NULL);

	if (SUCCEEDED(hResult))
	{
		IShellDispatch * pISD;

		hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pISD);

		if (SUCCEEDED(hResult) && NULL != pISD)
		{
			FILE * File = fopen(ParameterFilename, "rb");

			if (NULL == File)
			{
				// Perform the Taskbar operation
				pISD->TileVertically();

				// Create file
				File = fopen(ParameterFilename, "wb");
				if (NULL != File) fclose(File);
			}
			else
			{
				// Undo the last Taskbar operation
				pISD->UndoMinimizeALL();

				// Make sure the file is empty before removing it (as to not accidentally delete a user's valuable file)
				fseek(File, 0, SEEK_END); long FileSize = ftell(File); if (0 != FileSize) MessageBox(NULL, "Warning: The file 'OrganizeWindows_ShouldUndoNextTime' was not empty. When this program creates it, it should be empty.\n\nIt will not be deleted in case you have valuable information there. Please delete it yourself, or move this program into another folder that doesn't have a file with such name.", "OrganizeWindows", MB_ICONWARNING);
				fclose(File);
				if (0 == FileSize) remove(ParameterFilename);
			}

			pISD->Release();
		}

		CoUninitialize();
	}

	return EXIT_SUCCESS;
}
