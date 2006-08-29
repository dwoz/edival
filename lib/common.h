/*
 *  Copyright (C) 2006 Michael Edgar
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef EDICommon_INCLUDED
#define EDICommon_INCLUDED

#include "edival.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
/******************************************************************************/
typedef void* (*EDI_StateHandler)(void *parser);
/******************************************************************************/
#define MALLOC(obj, s) (obj->memsuite->malloc_fcn((s)))
#define REALLOC(obj, p,s) (obj->memsuite->realloc_fcn((p),(s)))
#define FREE(obj, p) (obj->memsuite->free_fcn((p)))
/******************************************************************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
/******************************************************************************/
size_t strnlen (const char *, size_t);
/******************************************************************************/
char *strndup(const char *, size_t, const EDI_Memory_Handling_Suite *);
inline int string_eq(const char*, const char*);
#endif /* _GNU_SOURCE */

#define EDI_GAP_SCAN(parser, pointer) \
	do { \
		int prefix = pointer - parser->bufReadPtr;\
		if(prefix && !isspace(*(parser->bufReadPtr))){\
			char *garbage = strndup(parser->bufReadPtr, prefix, parser->memsuite);\
			if(!garbage){\
				parser->errorCode = EDI_ERROR_NO_MEM;\
				return parser->error;\
			}\
			parser->nonEDIDataHandler(parser->userData, garbage);\
			FREE(parser, garbage);\
        	parser->bufReadPtr = pointer;\
		}\
	} while(NULL)

#endif /* EDICommon_INCLUDED */
