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


	SlocStat_t langStatistics = { 0 };

	for (int i = 1; i < argc; ++i)
	{
		const char * path = argv[i];
		if (!SlocStat_addFilesFromDir(&langStatistics, path))
			return -1;
		
		for (size_t i = 0; i < langStatistics.n_files; ++i)
		{
			SlocSourceFileStat_t * file = &langStatistics.files[i];
			size_t i = strnlen(file->path, MAX_PATH);
			for (; i > 0; --i)
			{
				if (file->path[i - 1] == '\\')
					break;
			}
			file->lang = SlocLang_match(file->path + i, 0);
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
					file->sloc = sloc_countSLOC(fileContents, bytesRead, file->lang);
				}
				free(fileContents);
			}

			// Close file
			CloseHandle(hFile);
		}
		SlocStat_qsortFiles(&langStatistics);

		SlocStat_makeLangStats(&langStatistics);
		SlocStat_qsortLangStats(&langStatistics);

		printf("Files from %s\n", path);
		for (size_t i = 0; i < langStatistics.n_files; ++i)
		{
			SlocSourceFileStat_t * file = &langStatistics.files[i];
			printf("%s [%s]: %zu sloc\n", file->path, file->lang->name, file->sloc);
		}

		
		printf("By language:\n");
		for (size_t i = 0; i < sloc_num_of_languages; ++i)
		{
			SlocLangStat_t * langStat = &langStatistics.langStats[i];
			if (langStat->lang == NULL)
				continue;
			
			int percent = (int)(((size_t)10000 * langStat->sloc) / langStatistics.slocTotal);
			printf("[%s]: %zu sloc -> %d.%d%%\n", langStat->lang->name, langStat->sloc, percent / 100, percent % 100);
		}

		putchar('\n');

		// Clear contents
		SlocStat_clear(&langStatistics);
	}

	return 0;
}
