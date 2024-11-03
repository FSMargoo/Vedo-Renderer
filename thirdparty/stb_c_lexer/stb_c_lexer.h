// stb_c_lexer.h - v0.12 - public domain Sean Barrett 2013
// lexer for making little C-like languages with recursive-descent parsers
//
// This file provides both the interface and the implementation.
// To instantiate the implementation,
//      #define STB_C_LEXER_IMPLEMENTATION
// in *ONE* source file, before #including this file.
//
// The default configuration is fairly close to a C lexer, although
// suffixes on integer constants are not handled (you can override this).
//
// History:
//     0.12 fix compilation bug for NUL support; better support separate inclusion
//     0.11 fix clang static analysis warning
//     0.10 fix warnings
//     0.09 hex floats, no-stdlib fixes
//     0.08 fix bad pointer comparison
//     0.07 fix mishandling of hexadecimal constants parsed by strtol
//     0.06 fix missing next character after ending quote mark (Andreas Fredriksson)
//     0.05 refixed get_location because github version had lost the fix
//     0.04 fix octal parsing bug
//     0.03 added STB_C_LEX_DISCARD_PREPROCESSOR option
//          refactor API to simplify (only one struct instead of two)
//          change literal enum names to have 'lit' at the end
//     0.02 first public release
//
// Status:
//     - haven't tested compiling as C++
//     - haven't tested the float parsing path
//     - haven't tested the non-default-config paths (e.g. non-stdlib)
//     - only tested default-config paths by eyeballing output of self-parse
//
//     - haven't implemented multiline strings
//     - haven't implemented octal/hex character constants
//     - haven't implemented support for unicode CLEX_char
//     - need to expand error reporting so you don't just get "CLEX_parse_error"
//
// Contributors:
//   Arpad Goretity (bugfix)
//   Alan Hickman (hex floats)
//   github:mundusnine (bugfix)
//
// LICENSE
//
//   See end of file for license information.

#ifndef INCLUDE_STB_C_LEXER_H
#define INCLUDE_STB_C_LEXER_H

#include <stdio.h>

typedef struct
{
	// lexer variables
	char *input_stream;
	char *eof;
	char *parse_point;
	char *string_storage;
	int   string_storage_len;

	// lexer parse location for error messages
	char *where_firstchar;
	char *where_lastchar;

	// lexer token variables
	long token;
	double real_number;
	long   int_number;
	char *string;
	int string_len;
} stb_lexer;

typedef struct
{
	int line_number;
	int line_offset;
} stb_lex_location;

#ifdef __cplusplus
extern "C" {
#endif

extern void stb_c_lexer_print_token(stb_lexer *lexer);

extern void stb_c_lexer_init(stb_lexer *lexer, const char *input_stream, const char *input_stream_end, char *string_store, int store_length);
// this function initialize the 'lexer' structure
//   Input:
//   - input_stream points to the file to parse, loaded into memory
//   - input_stream_end points to the end of the file, or NULL if you use 0-for-EOF
//   - string_store is storage the lexer can use for storing parsed strings and identifiers
//   - store_length is the length of that storage

extern int stb_c_lexer_get_token(stb_lexer *lexer);
// this function returns non-zero if a token is parsed, or 0 if at EOF
//   Output:
//   - lexer->token is the token ID, which is unicode code point for a single-char token, < 0 for a multichar or eof or error
//   - lexer->real_number is a double constant value for CLEX_floatlit, or CLEX_intlit if STB_C_LEX_INTEGERS_AS_DOUBLES
//   - lexer->int_number is an integer constant for CLEX_intlit if !STB_C_LEX_INTEGERS_AS_DOUBLES, or character for CLEX_charlit
//   - lexer->string is a 0-terminated string for CLEX_dqstring or CLEX_sqstring or CLEX_identifier
//   - lexer->string_len is the byte length of lexer->string

extern void stb_c_lexer_get_location(const stb_lexer *lexer, const char *where, stb_lex_location *loc);
// this inefficient function returns the line number and character offset of a
// given location in the file as returned by stb_lex_token. Because it's inefficient,
// you should only call it for errors, not for every token.
// For error messages of invalid tokens, you typically want the location of the start
// of the token (which caused the token to be invalid). For bugs involving legit
// tokens, you can report the first or the range.
//    Output:
//    - loc->line_number is the line number in the file, counting from 1, of the location
//    - loc->line_offset is the char-offset in the line, counting from 0, of the location


#ifdef __cplusplus
}
#endif

enum
{
	CLEX_eof = 256,
	CLEX_parse_error,
	CLEX_intlit        ,
	CLEX_floatlit      ,
	CLEX_id            ,
	CLEX_dqstring      ,
	CLEX_sqstring      ,
	CLEX_charlit       ,
	CLEX_eq            ,
	CLEX_noteq         ,
	CLEX_lesseq        ,
	CLEX_greatereq     ,
	CLEX_andand        ,
	CLEX_oror          ,
	CLEX_shl           ,
	CLEX_shr           ,
	CLEX_plusplus      ,
	CLEX_minusminus    ,
	CLEX_pluseq        ,
	CLEX_minuseq       ,
	CLEX_muleq         ,
	CLEX_diveq         ,
	CLEX_modeq         ,
	CLEX_andeq         ,
	CLEX_oreq          ,
	CLEX_xoreq         ,
	CLEX_arrow         ,
	CLEX_eqarrow       ,
	CLEX_shleq, CLEX_shreq,

	CLEX_first_unused_token

};
#endif // INCLUDE_STB_C_LEXER_H


/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
