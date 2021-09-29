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

typedef struct SlocLang
{
	const char * name;
	const char * ext;
	const sloc_countLineFunc_t fid;
} SlocLang_t;

extern const SlocLang_t sloc_c_langExtensions[sloc_num_of_languages];
extern const SlocLang_t sloc_c_langExtOther;


const SlocLang_t * SlocLang_match(const char * fName, size_t n_fName);

size_t sloc_countSLOC(const char * fileContents, size_t n_fileContents, const SlocLang_t * lang);

typedef struct SlocSourceFileStat
{
	const SlocLang_t * lang;
	char * path;
	size_t sloc;
} SlocSourceFileStat_t;

int SlocSourceFileStat_comp(const void * a, const void * b);


typedef struct SlocLangStat
{
	const SlocLang_t * lang;
	size_t sloc;
} SlocLangStat_t;

int SlocLangStat_comp(const void * a, const void * b);


typedef struct SlocStatistics
{
	SlocSourceFileStat_t * files;
	size_t n_files, cap;

	SlocLangStat_t langStats[sloc_num_of_languages];

	size_t slocTotal;
} SlocStat_t;

bool SlocStat_addFilesFromDir(SlocStat_t * files, const char * path);
bool SlocStat_add(SlocStat_t * files, SlocSourceFileStat_t sourceFile);
void SlocStat_makeLangStats(SlocStat_t * files);
void SlocStat_qsortFiles(SlocStat_t * files);
void SlocStat_qsortLangStats(SlocStat_t * files);
void SlocStat_clear(SlocStat_t * files);

#endif
