#ifndef COMMON_H
#define COMMON_H

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define NOMINMAX

#include <windows.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// namespace sloc

enum sloc_languages
{
	sloc_Assembly,
	sloc_B,
	sloc_C,
	sloc_Csharp,
	sloc_Cpp,
	sloc_CMake,
	sloc_CSS,
	sloc_D,
	sloc_Go,
	sloc_HTML,
	sloc_Java,
	sloc_JavaScript,
	sloc_JSON,
	sloc_JSONC,
	sloc_Lua,
	sloc_Makefile,
	sloc_Nim,
	sloc_PHP,
	sloc_PowerShell,
	sloc_Python,
	sloc_R,
	sloc_ResourceFile,
	sloc_Ruby,
	sloc_Rust,
	sloc_Shell,
	sloc_VBScript,
	sloc_VisualBasic,
	sloc_XAML,
	sloc_XML,
	sloc_YAML,
	sloc_Zig,

	sloc_num_of_languages
};

typedef size_t (*sloc_countLineFunc_t)(const char * code, size_t n_code);

bool sloc_isWhitespace(const char * code, size_t n_code);
size_t sloc_countLine(const char * code, size_t n_code);

typedef struct sloc_language
{
	const char * name;
	const char * ext;
	const sloc_countLineFunc_t fid;
} sloc_language_t;

extern const sloc_language_t sloc_langExtensions[sloc_num_of_languages];
extern const sloc_language_t sloc_langExtOther;


const sloc_language_t * sloc_matchLang(const char * fName, size_t n_fName);

size_t sloc_countSloc(const char * fileContents, size_t n_fileContents, const sloc_language_t * lang);

typedef struct sloc_sourcefile
{
	const sloc_language_t * lang;
	char * path;
	size_t sloc;
} sloc_sourcefile_t;

int sloc_sourcefile_comp(const void * a, const void * b);

typedef struct sloc_sfs
{
	sloc_sourcefile_t * files;
	size_t n_files, cap;
} sloc_sfs_t;

extern sloc_sfs_t sloc_sourceFiles;

bool sloc_sfs_addFilesFromDir(sloc_sfs_t * files, const char * path);
bool sloc_sfs_add(sloc_sfs_t * files, sloc_sourcefile_t sourceFile);
void sloc_sfs_qsort(sloc_sfs_t * files);
void sloc_sfs_clear(sloc_sfs_t * files);

#endif
