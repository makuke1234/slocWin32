#include "common.h"

#include <stdio.h>

int main(int argc, char * argv[])
{
	char * otherArgv[] = { argv[0], "." };
	if (argc == 1)
	{
		argc = 2;
		argv = otherArgv;
	}

	for (int i = 1; i < argc; ++i)
	{
		const char * path = argv[i];
		if (!sloc_sfs_addFilesFromDir(&sloc_sourceFiles, path))
			return -1;
		
		for (size_t i = 0; i < sloc_sourceFiles.n_files; ++i)
		{
			sloc_sourcefile_t * file = &sloc_sourceFiles.files[i];
			size_t i = strnlen(file->path, MAX_PATH);
			for (; i > 0; --i)
			{
				if (file->path[i - 1] == '\\')
					break;
			}
			file->lang = sloc_matchLang(file->path + i, 0);
			// Load one file contents
			
			HANDLE hFile = CreateFileA(
				file->path,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				file->sloc = 0;
				continue;
			}

			// The file has been opened successfully
			DWORD fileSize = 0;
			LARGE_INTEGER fSz;
			if (GetFileSizeEx(hFile, &fSz))
			{
				fileSize = fSz.LowPart;
			}
			char * fileContents = malloc(fileSize);
			if (fileContents != NULL)
			{
				DWORD bytesRead;
				if (ReadFile(hFile, fileContents, fileSize, &bytesRead, NULL))
				{
					file->sloc = sloc_countSloc(fileContents, bytesRead, file->lang);
				}
				free(fileContents);
			}

			// Close file
			CloseHandle(hFile);
		}
		sloc_sfs_qsort(&sloc_sourceFiles);

		printf("Files from %s\n", path);
		for (size_t i = 0; i < sloc_sourceFiles.n_files; ++i)
		{
			sloc_sourcefile_t * file = &sloc_sourceFiles.files[i];
			printf("%s [%s]: %zu sloc\n", file->path, file->lang->name, file->sloc);
		}
		putchar('\n');

		// Clear contents
		sloc_sfs_clear(&sloc_sourceFiles);
	}

	return 0;
}
