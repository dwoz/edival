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

#include "complextype.h"
#include "schema.h"
#include "simpletype.h"
/******************************************************************************/
#define SCHEMA_PUSH(segment) (schema->stack[++(schema->depth)] = segment)
#define SCHEMA_SAVE(segment) (schema->stack[schema->depth] = segment)
#define SCHEMA_POP()         (schema->stack[--(schema->depth)])
#define CHECK_USAGE(segment)\
if(segment->count < segment->max_occurs){\
	segment->count++;\
	SCHEMA_SAVE(segment);\
	if(error){\
		return error;\
	} else {\
		return SEGERR_NONE;\
	}\
} else {\
	if(segment->previousSibling){\
		return SEGERR_EXCEED_REPEAT;\
	}\
}
/******************************************************************************/
static EDI_Schema schemaCreate(EDI_Memory_Handling_Suite *); 
static void schemaInit(EDI_Schema);
/******************************************************************************/
EDI_Schema EDI_SchemaCreate(void)
{
	return schemaCreate(NULL);	
}
/******************************************************************************/
EDI_Schema EDI_SchemaCreate_MM(EDI_Memory_Handling_Suite *memsuite)
{
	return schemaCreate(memsuite);	
}
/******************************************************************************/
static EDI_Schema schemaCreate(EDI_Memory_Handling_Suite *memsuite)
{
	EDI_Schema schema = NULL;
	
	if(memsuite){
        EDI_Memory_Handling_Suite *mtemp;
        schema = (EDI_Schema)\
            memsuite->malloc_fcn(sizeof(struct EDI_SchemaStruct));
        if (schema != NULL) {
				schema->memsuite = (EDI_Memory_Handling_Suite *)\
        			memsuite->malloc_fcn(sizeof(EDI_Memory_Handling_Suite));
            mtemp = (EDI_Memory_Handling_Suite *)schema->memsuite;
            mtemp->malloc_fcn = memsuite->malloc_fcn;
            mtemp->realloc_fcn = memsuite->realloc_fcn;
            mtemp->free_fcn = memsuite->free_fcn;
        }
	} else {
        EDI_Memory_Handling_Suite *mtemp;
        schema = (EDI_Schema)malloc(sizeof(struct EDI_SchemaStruct));
        if (schema != NULL) {
				schema->memsuite = (EDI_Memory_Handling_Suite *)\
        			malloc(sizeof(EDI_Memory_Handling_Suite));
            mtemp = (EDI_Memory_Handling_Suite *)schema->memsuite;
            mtemp->malloc_fcn = malloc;
            mtemp->realloc_fcn = realloc;
            mtemp->free_fcn = free;
        }
	}
	if(schema){
		schemaInit(schema);
	}
	return schema;
}
/******************************************************************************/
static void schemaInit(EDI_Schema schema)
{
	schema->identifier   = "";
	schema->complexNodes = create_hashtable(20);
	schema->elements     = create_hashtable(20);
	schema->root         = NULL;
	schema->depth        = 0;
}
/******************************************************************************/
char *EDI_GetSchemaId(EDI_Schema schema)
{
	char *id = NULL;
	if(schema){
		id = schema->identifier;
	}
	return id;
}
/******************************************************************************/
void EDI_SetSchemaId(EDI_Schema schema, char *id)
{
	if(schema){
		schema->identifier = id;
	}
}
/******************************************************************************/
void EDI_SetSegmentErrorHandler(EDI_Schema schema, EDI_SegmentErrorHandler h)
{
    schema->handleSegmentError = h;
}
/******************************************************************************/
void EDI_SetElementErrorHandler(EDI_Schema schema, EDI_ElementErrorHandler h)
{
    schema->handleElementError = h;
}
/******************************************************************************/
enum EDI_SegmentValidationError EDI_ValidateSegmentPosition(EDI_Schema  schema,
                                                            const char *nodeID)
{
	unsigned char                   startDepth = schema->depth;
	unsigned int                    startCount = 0;
	EDI_ChildNode                   startNode  = NULL;
	EDI_ChildNode                   current    = NULL;
	enum EDI_SegmentValidationError error      = SEGERR_NONE;
	
	startNode = schema->stack[startDepth];
	if(!startNode){
		return SEGERR_UNDEFINED;
	}
	startCount = startNode->count;
	current = startNode;
	while(current){
		if(current->node->type == EDITYPE_LOOP){
			EDI_LoopNode loop = (EDI_LoopNode)(current->node);
			if((strcmp(nodeID, loop->startID) == 0)){
				SCHEMA_SAVE(current);
				if(loop->position == 0){
					if(current->count >=  current->max_occurs){
						error = SEGERR_LOOP_EXCEEDED;
					}
					schema->depth++;
					current->count++;
					CHECK_USAGE(loop->node.firstChild);
				} else {
					//Can't determine the loop until a later element.
					return SEGERR_LOOP_SEEK;
				}
			}
		} else {
			if((strcmp(nodeID, current->node->nodeID) == 0)){
				CHECK_USAGE(current);
			}
		}
		if(current->count < current->min_occurs){
			fprintf(stderr, "\nMissing Mandatory %s (used %d) looking for %s\n", current->node->nodeID, current->count, nodeID);
			error = SEGERR_MANDATORY;
		}
		if(current->nextSibling){
			current = current->nextSibling;
		} else {
			EDI_ChildNode clear = NULL;
			if(schema->depth > 0){
				current = SCHEMA_POP();
				clear = ((EDI_ComplexType)current->node)->firstChild;
			} else {
				current  = schema->root->firstChild;
				if((strcmp(nodeID, current->node->nodeID) != 0)){
					//Unexpected segment... must reset our position!
					schema->depth = startDepth;
					startNode->count = startCount;
					return SEGERR_UNEXPECTED;
				} else {
					clear = current;
				}
			}
			//Reset the usage of child nodes to 0 since we are moving up a level
			while(clear){
				clear->count = 0;
				clear = clear->nextSibling;
			}
		}
	}
	return error;
}
/******************************************************************************/
enum EDI_ElementValidationError EDI_ValidateElement(EDI_Schema schema        ,
                                                    int        elementIndex  , 
                                                    int        componentIndex, 
                                                    const char *value        )
{
	int                             index   = 0;
	char                           *name    = NULL;
	EDI_ChildNode                   segment = NULL;
	EDI_ChildNode                   element = NULL;
	enum EDI_ElementValidationError error   = VAL_VALID_ELEMENT;
	
	segment = schema->stack[schema->depth];
	if(segment){
		element = ((EDI_ComplexType)segment->node)->firstChild;
		for(index = 1; index < elementIndex; index++){
			if(element->nextSibling){
				element = element->nextSibling;
			} else {
				return VAL_TOO_MANY_ELEMENTS;
			}
		}
		if(element->node->type == EDITYPE_COMPOSITE){
			if(componentIndex == 0 && strlen(value) > 0){
				componentIndex = 1;
			}
		}
		if(componentIndex > 0){
			if(element->node->type != EDITYPE_COMPOSITE){
				return VAL_TOO_MANY_COMPONENTS;
			} else {
				EDI_ChildNode c = ((EDI_ComplexType)element->node)->firstChild;
				for(index = 1; index < componentIndex; index++){
					if(c->nextSibling){
						c = c->nextSibling;
					} else {
						return VAL_TOO_MANY_COMPONENTS;
					}
				}
				element = c;
			}
		}
		if(strlen(value) > 0){
			element->count++;
			/* FIXME: enable repeating elements. Must clear usages at end of segment. */
			//if(element->count <= element->max_occurs){
				name = element->node->nodeID;
				error = EDI_CheckElementConstraints(schema, name, value);
			//} else {
			//	error = VAL_REPETITION_EXCEEDED;
			//}
		} else if(element->min_occurs > 0){
			error = VAL_MANDATORY_ELEMENT;
		}	
	} else {
		error = VAL_INVALID_SEGMENT;
	}
	return error;
}
/******************************************************************************/
enum EDI_ElementValidationError EDI_ValidateSyntax(EDI_Schema schema,
                                                   int        element)
{
	enum EDI_ElementValidationError error   = VAL_VALID_ELEMENT;

	return error;
}
/******************************************************************************/
void EDI_SchemaFree(EDI_Schema schema)
{
	void (*free_fcn)(void *ptr);

	if(strcmp(EDI_GetSchemaId(schema->parser->schema), schema->identifier) == 0){
		schema->parser->schema = NULL;
		schema->parser->validate = EDI_FALSE;
	}
	EDI_DisposeNode(schema, (EDI_SchemaNode)schema->root);
	hashtable_destroy(schema->complexNodes, 0);
	hashtable_destroy(schema->elements, 0);
	free_fcn = schema->memsuite->free_fcn;
	free_fcn((void *)schema->memsuite);
	free_fcn(schema);
	schema = NULL;
	return;
}
/******************************************************************************/
void EDI_DisposeNode(EDI_Schema     schema  ,
                     EDI_SchemaNode node    )
{
	if(node->type == EDITYPE_ELEMENT){
		EDI_DisposeSimpleType(schema, node);
	} else {
		EDI_DisposeComplexType(schema, (EDI_ComplexType)node);
	}
}
