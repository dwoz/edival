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

#ifndef EDISchema_INCLUDED
#define EDISchema_INCLUDED

#include "ediparse.h"
#include "edival.h"
#include "hashtable.h"
/******************************************************************************/
typedef struct EDI_ComplexTypeStruct *EDI_ComplexType;
typedef struct EDI_ChildNodeStruct   *EDI_ChildNode;
typedef struct EDI_SimpleTypeStruct   EDI_SimpleType;
typedef struct EDI_LoopNodeStruct    *EDI_LoopNode;
typedef struct EDI_SyntaxNoteStruct  *EDI_SyntaxNote;
/******************************************************************************/
struct EDI_SchemaStruct {
	char                            *identifier;
	struct hashtable                *elements;
	struct hashtable                *complexNodes;
	EDI_ComplexType                  root;
	EDI_ChildNode                    stack[50];
	unsigned int                     depth;
	EDI_ChildNode                    prevElementNode;
	unsigned int                     prevElementIndex;
	const EDI_Memory_Handling_Suite *memsuite;
	EDI_Parser                       parser;
	EDI_SegmentErrorHandler          segmentErrorHandler;
	EDI_ElementErrorHandler          elementErrorHandler;
};
/******************************************************************************/
struct EDI_SchemaNodeStruct {
    char              *nodeID;
    enum EDI_NodeType  type;
    unsigned int       refCount;
};
/******************************************************************************/

/**
 *  Validate the position of a segment
 */
enum EDI_SegmentValidationError 
EDI_ValidateSegmentPosition(EDI_Schema  ,
                            const char *);  /* Segment Tag */

/**
 *  Validate an element in the current segment based on previously defined 
 *  restrictions.
 */
enum EDI_ElementValidationError 
EDI_ValidateElement(EDI_Schema  ,
                    int         ,   /* Element position      */
                    int         ,   /* Composite position    */
                    const char *,   /* String/element value  */
                    int         );  /* String/element length */
                    
/**
 *  Validate the relational syntax of a segment/composite element node
 */
enum EDI_ElementValidationError 
EDI_ValidateSyntax(EDI_Schema,
                   int       );  /* Element position; for composites ONLY */

void EDI_DisposeNode(EDI_Schema, EDI_SchemaNode);
#endif /* EDISchema_INCLUDED */
