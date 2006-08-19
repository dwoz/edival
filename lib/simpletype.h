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
 
#ifndef EDISimpleType_INCLUDED
#define EDISimpleType_INCLUDED

#include "schema.h"
/******************************************************************************/

/******************************************************************************/
struct EDI_SimpleTypeStruct {
	struct EDI_SchemaNodeStruct header;
	enum EDI_PrimitiveDataType  type;
	unsigned int                min;
	unsigned int                max;
	struct hashtable           *values;
};
/******************************************************************************/
void EDI_DisposeSimpleType(EDI_Schema, EDI_SchemaNode);

enum EDI_ElementValidationError 
EDI_CheckElementConstraints(EDI_Schema  ,
                            const char *,   /* Name of element      */
                            const char *);  /* String/element value */

#endif /* EDISimpleType_INCLUDED */
