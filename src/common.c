#include "common.h"

static const bool sloc_whitespaceCharacters[256] = {
	['\t'] = true,
	['\n'] = true,
	['\v'] = true,
	['\f'] = true,
	['\r'] = true,
	[' ']  = true,
};
bool sloc_isWhitespace(const char * code, size_t n_code)
{
	for (size_t i = 0; i < n_code; ++i)
	{
		if (!sloc_whitespaceCharacters[(size_t)code[i]])
			return false;
	}
	return true;
}


size_t sloc_countLine(const char * code, size_t n_code)
{
	// Checks if the code is whitespace
	for (size_t i = 0; i < n_code; ++i)
	{
		if (code[i] == '\n')
			return i;
	}
	return n_code;
}
size_t sloc_countLineUntilNotWhitespace(const char * code, size_t n_code)
{
	for (size_t i = 0; i < n_code; ++i)
	{
		if (!sloc_whitespaceCharacters[(size_t)code[i]])
			return i;
	}
	return n_code;
}

static inline size_t sloc_countLine_Asm(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case ';':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	return ff;
}
static inline size_t sloc_countLine_C(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '/':
			if (ff < (n_code - 1))
			{
				if (code[ff + 1] == '/')
				{
					ff += sloc_countLine(code + ff, n_code - ff);
				}
				else if (code[ff + 1] == '*')
				{
					// search "*/" part
					for (; ff < (n_code - 1); ++ff)
					{
						if (code[ff] == '*' && code[ff + 1] == '/')
							break;
					}
				}
			}
			
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_CMake(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '#':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_CSS(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '/':
			if (ff < (n_code - 1))
			{
				if (code[ff + 1] == '*')
				{
					// search "*/" part
					for (; ff < (n_code - 1); ++ff)
					{
						if (code[ff] == '*' && code[ff + 1] == '/')
							break;
					}
					break;
				}
			}
			
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_HTML(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '<':
			if (ff < (n_code - 3))
			{
				if (code[ff + 1] == '!' && code[ff + 2] == '-' && code[ff + 3] == '-')
				{
					// search "*/" part
					for (; ff < (n_code - 2); ++ff)
					{
						if (code[ff] == '-' && code[ff + 1] == '-' && code[ff + 2] == '>')
							break;
					}
					break;
				}
			}
			
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_Lua(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '-':
			if (ff < (n_code - 1) && code[ff + 1] == '-')
			{
				if (ff < (n_code - 3) && code[ff + 2] == '[' && code[ff + 3] == '[')
				{
					for (; ff < (n_code - 1); ++ff)
					{
						if (code[ff] == ']' && code[ff + 1] == ']')
							break;
					}
				}
				else
					ff += sloc_countLine(code + ff, n_code - ff);
			}
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_Nim(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '#':
			if (ff < (n_code - 1) && code[ff + 1] == '[')
			{
				for (; ff < (n_code - 1); ++ff)
				{
					if (code[ff] == ']' && code[ff + 1] == '#')
						break;
				}
			}
			else
				ff += sloc_countLine(code + ff, n_code - ff);
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_PHP(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '/':
			if (ff < (n_code - 1))
			{
				if (code[ff + 1] == '/')
				{
					ff += sloc_countLine(code + ff, n_code - ff);
					break;
				}
				else if (code[ff + 1] == '*')
				{
					// search "*/" part
					for (; ff < (n_code - 1); ++ff)
					{
						if (code[ff] == '*' && code[ff + 1] == '/')
							break;
					}
					break;
				}
			}
			
			break;
		case '#':
			for (; ff < n_code; ++ff)
			{
				if (code[ff] == ';' || code[ff] == '\n')
					break;
			}
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_PS(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '#':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		case '<':
			if (ff < (n_code - 1) && code[ff + 1] == '#')
			{
				++ff;
				// Search for "#>" part
				for (; ff < (n_code - 1); ++ff)
				{
					if (code[ff] == '#' && code[ff + 1] == '>')
						break;
				}
			}
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_Python(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '#':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		case '"':
			if (ff < (n_code - 2))
			{
				if (code[ff + 1] == '"' && code[ff + 2] == '"')
				{
					// Search for """ part
					for (; ff < (n_code - 2); ++ff)
					{
						if (code[ff] == '"' && code[ff + 1] == '"' && code[ff + 2] == '"')
							break;
					}
				}
			}
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_Ruby(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '#':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		case '=':
			if (ff < (n_code - 5))
			{
				if (code[ff + 1] == 'b' && code[ff + 2] == 'e' && code[ff + 3] == 'g' && code[ff + 4] == 'i' && code[ff + 5] == 'n')
				{
					// Search for "=end" part
					for (; ff < (n_code - 5); ++ff)
					{
						if (code[ff] == '=' && code[ff + 1] == 'e' && code[ff + 2] == 'n' && code[ff + 3] == 'd')
							break;
					}
					break;
				}
			}
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_VB(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case '\'':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_VBScript(const char * code, size_t n_code)
{
	size_t ff = 0;
	for (; ff < n_code;)
	{
		ff += sloc_countLineUntilNotWhitespace(code + ff, n_code - ff);
		switch (code[ff])
		{
		case 'R':
			if (ff < (n_code - 3) && code[ff + 1] == 'e' && code[ff + 2] == 'm' && code[ff + 3] == ' ')
			{
				[[fallthrough]];
			}
			else
				break;
		case '\'':
			ff += sloc_countLine(code + ff, n_code - ff);
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}


const sloc_language_t sloc_langExtensions[sloc_num_of_languages] = {
	[sloc_Assembly]     = {
		.name = "Assembly",
		.ext  = "*.asm;*.S",
		.fid  = &sloc_countLine_Asm
	},
	[sloc_B]            = {
		.name = "B",
		.ext  = "*.b",
		.fid  = &sloc_countLine_C
	},
	[sloc_C]            = {
		.name = "C",
		.ext  = "*.c;*.h",
		.fid  = &sloc_countLine_C
	},
	[sloc_Csharp]       = {
		.name = "C#",
		.ext  = "*.cs;*.csx",
		.fid  = &sloc_countLine_C
	},
	[sloc_Cpp]          = {
		.name = "C++",
		.ext  = "*.C;*.cc;*.cpp;*.cxx;*.c++;*.H;*.hh;*.hpp;*.hxx;*.h++",
		.fid  = &sloc_countLine_C
	},
	[sloc_CMake]        = {
		.name = "CMake",
		.ext  = "CMakeLists.txt;*.cmake",
		.fid  = &sloc_countLine_CMake
	},
	[sloc_CSS]          = {
		.name = "CSS",
		.ext  = "*.css",
		.fid  = &sloc_countLine_CSS
	},
	[sloc_D]            = {
		.name = "D",
		.ext  = "*.d",
		.fid  = &sloc_countLine_C
	},
	[sloc_Go]           = {
		.name = "Go",
		.ext  = "*.go",
		.fid  = &sloc_countLine_C
	},
	[sloc_HTML]         = {
		.name = "HTML",
		.ext  = "*.html;*.htm",
		.fid  = &sloc_countLine_HTML
	},
	[sloc_Java]         = {
		.name = "Java",
		.ext  = "*.java",
		.fid  = &sloc_countLine_C
	},
	[sloc_JavaScript]   = {
		.name = "JavaScript",
		.ext  = "*.js;*.cjs;*.mjs",
		.fid  = &sloc_countLine_C
	},
	[sloc_JSON]         = {
		.name = "JSON",
		.ext  = "*.json"
	},
	[sloc_JSONC]       = {
		.name = "JSON with comments",
		.ext  = "*.jsonc",
		.fid  = &sloc_countLine_C
	},
	[sloc_Lua]          = {
		.name = "Lua",
		.ext  = "*.lua",
		.fid  = &sloc_countLine_Lua
	},
	[sloc_Makefile]     = {
		.name = "Makefile",
		.ext  = "makefile;GNUmakefile;Makefile",
		.fid  = &sloc_countLine_CMake
	},
	[sloc_Nim]          = {
		.name = "Nim",
		.ext  = "*.nim;*.nims;*.nimble",
		.fid  = &sloc_countLine_Nim
	},
	[sloc_PHP]          = {
		.name = "PHP",
		.ext  = "*.php;*.phar;*.phtml;*.pht;*.phps",
		.fid  = &sloc_countLine_PHP
	},
	[sloc_PowerShell]   = {
		.name = "PowerShell",
		.ext  = "*.ps1;*.ps1xml;*.psc1;*.psd1;*.psm1;*.pssc;*.psrc;*.cdxml",
		.fid  = &sloc_countLine_PS
	},
	[sloc_Python]       = {
		.name = "Python",
		.ext  = "*.py;*.py3;*.pyw;*.pyx",
		.fid  = &sloc_countLine_Python
	},
	[sloc_R]            = {
		.name = "R",
		.ext  = "*.r;*.rdata;*.rds;*.rda",
		.fid  = &sloc_countLine_CMake
	},
	[sloc_ResourceFile] = {
		.name = "Windows Resource File",
		.ext  = "*.rc;*.RC",
		.fid  = &sloc_countLine_C
	},
	[sloc_Ruby]         = {
		.name = "Ruby",
		.ext  = "*.rb",
		.fid  = &sloc_countLine_Ruby
	},
	[sloc_Rust]         = {
		.name = "Rust",
		.ext  = "*.rs;*.rlib",
		.fid  = &sloc_countLine_C
	},
	[sloc_Shell]        = {
		.name = "Shell",
		.ext  = "*.sh;*.cmd;*.bat"
	},
	[sloc_VBScript]     = {
		.name = "VBScript",
		.ext  = "*.vbs",
		.fid  = &sloc_countLine_VBScript
	},
	[sloc_VisualBasic]  = {
		.name = "Visual Basic",
		.ext  = "*.vb",
		.fid  = &sloc_countLine_VB
	},
	[sloc_XAML]         = {
		.name = "XAML",
		.ext  = "*.xaml",
		.fid  = &sloc_countLine_HTML
	},
	[sloc_XML]          = {
		.name = "XML",
		.ext  = "*.xml",
		.fid  = &sloc_countLine_HTML
	},
	[sloc_YAML]         = {
		.name = "YAML",
		.ext  = "*.yaml;*.yml",
		.fid  = &sloc_countLine_CMake
	},
	[sloc_Zig]          = {
		.name = "Zig",
		.ext  = "*.zig;*.zir",
		.fid  = &sloc_countLine_C
	}
};

const sloc_language_t sloc_langExtOther = {
	.name = "Other",
	.ext = "*",
	.fid = &sloc_countLine
};


static inline bool sloc_matchLangIsMatchPattern(const char * fName, size_t n_fName, const char * pattern, size_t n_pattern)
{
	intptr_t i = (intptr_t)n_pattern - 1, j = (intptr_t)n_fName - 1;
	for (; i >= 0 && j >= 0; --i)
	{
		switch (pattern[i])
		{
		case '*':
			if (i > 0)
			{
				--i;
				while (j >= 0 && fName[j] != pattern[i]) --j;
				if (j == -1)
					return false;
			}
			else
				return true;
			break;
		default:
			if (pattern[i] != fName[j])
				return false;
			--j;
		}
	}

	return true;
}
static inline bool sloc_matchLangIsMatch(const char * fName, size_t n_fName, const sloc_language_t * lang)
{
	// Check pattern matching
	const char * pattern = lang->ext;
	for (size_t i = 0; pattern[i] != '\0';)
	{
		// Found pattern terminator
		if (pattern[i] == ';')
		{
			if (sloc_matchLangIsMatchPattern(fName, n_fName, pattern, i))
				return true;
			else
			{
				pattern += i + 1;
				i = 0;
			}
		}
		else
			++i;
	}
	if (sloc_matchLangIsMatchPattern(fName, n_fName, pattern, strlen(pattern)))
		return true;

	return false;
}

const sloc_language_t * sloc_matchLang(const char * fName, size_t n_fName)
{
	if (n_fName == 0)
		n_fName = strlen(fName);
	for (size_t i = 0; i < sloc_num_of_languages; ++i)
	{
		if (sloc_matchLangIsMatch(fName, n_fName, &sloc_langExtensions[i]))
			return &sloc_langExtensions[i];
	}
	return &sloc_langExtOther;
}

size_t sloc_countSloc(const char * fileContents, size_t n_fileContents, const sloc_language_t * lang)
{
	sloc_countLineFunc_t fid = lang->fid;
	if (fid == NULL)
		fid = &sloc_countLine;
	size_t counter = 0;
	for (size_t i = 0; i < n_fileContents; ++i)
	{
		i += fid(fileContents + i, n_fileContents - i);
		++counter;
	}
	return counter;
}

int sloc_sourcefile_comp(const void * a, const void * b)
{
	const size_t va = ((const sloc_sourcefile_t *)a)->sloc, vb = ((const sloc_sourcefile_t *)b)->sloc;
	return -1 * (vb < va) + (va < vb);
}
int sloc_langStat_comp(const void * a, const void * b)
{
	const size_t va = ((const sloc_langStat_t *)a)->sloc, vb = ((const sloc_langStat_t *)b)->sloc;
	return -1 * (vb < va) + (va < vb);
}

sloc_sfs_t sloc_sourceFiles;

bool sloc_sfs_addFilesFromDir(sloc_sfs_t * files, const char * path)
{
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	{
		char buffer[MAX_PATH];
		strcpy_s(buffer, MAX_PATH, path);
		strcat_s(buffer, MAX_PATH, "\\*");
		hFind = FindFirstFileA(buffer, &ffd);
	}

	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	
	do
	{
		// If it's "not" a directory
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char fullpath[MAX_PATH];
			strcpy_s(fullpath, MAX_PATH, path);
			strcat_s(fullpath, MAX_PATH, "\\");
			strcat_s(fullpath, MAX_PATH, ffd.cFileName);

			if (!sloc_sfs_add(files, (sloc_sourcefile_t){ .path = strdup(fullpath) }))
				return false;
		}
	} while (FindNextFileA(hFind, &ffd));
	FindClose(hFind);

	return true;
}
bool sloc_sfs_add(sloc_sfs_t * files, sloc_sourcefile_t sourceFile)
{
	if (files->cap == 0)
	{
		files->files = malloc(1 * sizeof(sloc_sourcefile_t));
		if (files->files == NULL)
			return false;
		
		files->cap = 1;
	}
	else if (files->n_files >= files->cap)
	{
		size_t tempcap = files->n_files * (size_t)2;
		void * mem = malloc(tempcap * sizeof(sloc_sourcefile_t));
		if (mem == NULL)
			return false;

		memcpy(mem, files->files, files->n_files * sizeof(sloc_sourcefile_t));
		free(files->files);
		files->files = mem;
		files->cap = tempcap;
	}
	files->files[files->n_files] = sourceFile;
	++files->n_files;

	return true;
}
void sloc_sfs_makeLangStats(sloc_sfs_t * files)
{
	memset(files->langStats, 0, sloc_num_of_languages * sizeof(sloc_langStat_t));

	files->slocTotal = 0;
	for (size_t i = 0; i < files->n_files; ++i)
	{
		sloc_sourcefile_t * file = &files->files[i];
		if (file->lang != &sloc_langExtOther)
		{
			intptr_t index = file->lang - sloc_langExtensions;
			if (files->langStats[index].lang == NULL)
				files->langStats[index].lang = &sloc_langExtensions[index];

			files->langStats[index].sloc += file->sloc;
			files->slocTotal += file->sloc;
		}
	}
}
void sloc_sfs_qsortFiles(sloc_sfs_t * files)
{
	qsort(files->files, files->n_files, sizeof(sloc_sourcefile_t), &sloc_sourcefile_comp);
}
void sloc_sfs_qsortLangStats(sloc_sfs_t * files)
{
	qsort(files->langStats, sloc_num_of_languages, sizeof(sloc_langStat_t), &sloc_langStat_comp);
}
void sloc_sfs_clear(sloc_sfs_t * files)
{
	for (size_t i = 0; i < files->n_files; ++i)
	{
		if (files->files[i].path != NULL)
		{
			free(files->files[i].path);
			files->files[i].path = NULL;
		}
	}
	files->n_files = 0;
}
