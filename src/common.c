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
	return sloc_countLine(code, n_code);
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
					ff += sloc_countLine(code + ff, n_code - ff);
				else if (code[ff + 1] == '*')
				{
					// search "*/" part
					for (; ff < (n_code - 1); ++ff)
					{
						if (code[ff] == '*' && code[ff + 1] == '/')
							break;
					}
				}
				else
					return ff + sloc_countLine(code + ff, n_code - ff);
			}
			
			break;
		default:
			return ff + sloc_countLine(code + ff, n_code - ff);
		}
	}
	
	return ff;
}
static inline size_t sloc_countLine_Cpp(const char * code, size_t n_code)
{

	return sloc_countLine(code, n_code);
}

const sloc_language_t sloc_langExtensions[sloc_num_of_languages] = {
	[sloc_Assembly]     = {
		.name = "Assembly",
		.ext  = "*.asm;*.S",
		.fid  = &sloc_countLine_Asm
	},
	[sloc_B]            = {
		.name = "B",
		.ext  = "*.b"
	},
	[sloc_C]            = {
		.name = "C",
		.ext  = "*.c;*.h",
		.fid  = &sloc_countLine_C
	},
	[sloc_Csharp]       = {
		.name = "C#",
		.ext  = "*.cs;*.csx"
	},
	[sloc_Cpp]          = {
		.name = "C++",
		.ext  = "*.C;*.cc;*.cpp;*.cxx;*.c++;*.H;*.hh;*.hpp;*.hxx;*.h++",
		.fid  = &sloc_countLine_Cpp
	},
	[sloc_CMake]        = {
		.name = "CMake",
		.ext  = "CMakeLists.txt;*.cmake"
	},
	[sloc_CSS]          = {
		.name = "CSS",
		.ext  = "*.css"
	},
	[sloc_D]            = {
		.name = "D",
		.ext  = "*.d"
	},
	[sloc_Go]           = {
		.name = "Go",
		.ext  = "*.go"
	},
	[sloc_HTML]         = {
		.name = "HTML",
		.ext  = "*.html;*.htm"
	},
	[sloc_Java]         = {
		.name = "Java",
		.ext  = "*.java"
	},
	[sloc_JavaScript]   = {
		.name = "JavaScript",
		.ext  = "*.js;*.cjs;*.mjs"
	},
	[sloc_JSON]         = {
		.name = "JSON",
		.ext  = "*.json"
	},
	[sloc_Lua]          = {
		.name = "Lua",
		.ext  = "*.lua"
	},
	[sloc_Makefile]     = {
		.name = "Makefile",
		.ext  = "makefile;GNUmakefile;Makefile"
	},
	[sloc_Nim]          = {
		.name = "Nim",
		.ext  = "*.nim;*.nims;*.nimble"
	},
	[sloc_PHP]          = {
		.name = "PHP",
		.ext  = "*.php;*.phar;*.phtml;*.pht;*.phps"
	},
	[sloc_PowerShell]   = {
		.name = "PowerShell",
		.ext  = "*.ps1;*.ps1xml;*.psc1;*.psd1;*.psm1;*.pssc;*.psrc;*.cdxml"
	},
	[sloc_Python]       = {
		.name = "Python",
		.ext  = "*.py;*.py3;*.pyw;*.pyx"
	},
	[sloc_R]            = {
		.name = "R",
		.ext  = "*.r;*.rdata;*.rds;*.rda"
	},
	[sloc_ResourceFile] = {
		.name = "Windows Resource File",
		.ext  = "*.rc;*.RC"
	},
	[sloc_Ruby]         = {
		.name = "Ruby",
		.ext  = "*.rb"
	},
	[sloc_Rust]         = {
		.name = "Rust",
		.ext  = "*.rs;*.rlib"
	},
	[sloc_Shell]        = {
		.name = "Shell",
		.ext  = "*.sh;*.cmd;*.bat"
	},
	[sloc_VBScript]     = {
		.name = "VBScript",
		.ext  = "*.vbs"
	},
	[sloc_VisualBasic]  = {
		.name = "Visual Basic",
		.ext  = "*.vb"
	},
	[sloc_XAML]         = {
		.name = "XAML",
		.ext  = "*.xaml"
	},
	[sloc_XML]          = {
		.name = "XML",
		.ext  = "*.xml"
	},
	[sloc_YAML]         = {
		.name = "YAML",
		.ext  = "*.yaml;*.yml"
	},
	[sloc_Zig]          = {
		.name = "Zig",
		.ext  = "*.zig;*.zir"
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
	return ((const sloc_sourcefile_t *)a)->sloc > ((const sloc_sourcefile_t *)b)->sloc;
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
void sloc_sfs_qsort(sloc_sfs_t * files)
{
	qsort(files->files, files->n_files, sizeof(sloc_sourcefile_t), &sloc_sourcefile_comp);
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
