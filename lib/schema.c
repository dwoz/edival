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
	unsigned int                    startDepth = schema->depth;
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
			if(string_eq(nodeID, loop->startID)){
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
			if(string_eq(nodeID, current->node->nodeID)){
				CHECK_USAGE(current);
			}
		}
		if(current->count < current->min_occurs){
			fprintf(stderr, "\nMissing Mandatory %s (used %d) looking for '%s'\n", current->node->nodeID, current->count, nodeID);
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
				if(!string_eq(nodeID, current->node->nodeID)){
					//Unexpected segment... must reset our position!
					schema->depth = startDepth;
					startNode->count = startCount;
					fprintf(stderr, "undexpected segment %s\n", nodeID);
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
                                                    const char *value        ,
                                                    int        length        )
{
	int                             index   = 0;
	char                           *name    = NULL;
	EDI_ChildNode                   clear   = NULL;
	EDI_ChildNode                   segment = NULL;
	EDI_ChildNode                   element = NULL;
	enum EDI_ElementValidationError error   = VAL_VALID_ELEMENT;
	
	if(elementIndex > 1){
		element = schema->prevElementNode;
	} else {
		segment = schema->stack[schema->depth];
		if(segment){
			clear = element = ((EDI_ComplexType)segment->node)->firstChild;
			while(clear){
				clear->count = 0;
				clear = clear->nextSibling;
			}			
			schema->prevElementIndex = 1;		
		} else {
			error = VAL_INVALID_SEGMENT;
		}
	}
	if(!error){
		for(index = schema->prevElementIndex; index < elementIndex; index++){
			if(element->nextSibling){
				element = element->nextSibling;
			} else {
				return VAL_TOO_MANY_ELEMENTS;
			}
		}
		schema->prevElementNode = element;
		schema->prevElementIndex = index;
		if(element->node->type == EDITYPE_COMPOSITE){
			if(componentIndex == 0 && length > 0){
				componentIndex = 1;
			}
		}
		if(componentIndex){
			if(element->node->type != EDITYPE_COMPOSITE){
				return VAL_TOO_MANY_COMPONENTS;
			} else {
				EDI_ChildNode c = ((EDI_ComplexType)element->node)->firstChild;
				if(componentIndex == 1){
					clear = c;
					while(clear){
						clear->count = 0;
						clear = clear->nextSibling;
					}
				}
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
		if(length){
			element->count++;
			if(element->count <= element->max_occurs){
				name = element->node->nodeID;
				error = EDI_CheckElementConstraints(schema, name, value, length);
			} else {
				error = VAL_REPETITION_EXCEEDED;
			}
		} else if(element->min_occurs > 0){
			error = VAL_MANDATORY_ELEMENT;
		}
	}
	return error;
}
/******************************************************************************/
enum EDI_ElementValidationError EDI_ValidateSyntax(EDI_Schema schema,
                                                   int        element)
{
	int                             i, j   = 0;
	int                             found  = 0;
	EDI_Bool                        anchor = EDI_FALSE;
	EDI_ComplexType                 parent = NULL;
	EDI_SyntaxNote                  note   = NULL;
	EDI_ChildNode                   child  = NULL;
	enum EDI_ElementValidationError error  = VAL_VALID_ELEMENT;

	parent = (EDI_ComplexType)(schema->stack[schema->depth])->node;
	if(parent && parent->firstNote){
		if(element-- > 0){
			child = parent->firstChild;
			while(element--){
				child = child->nextSibling;
			}
			parent = (EDI_ComplexType)child->node;
		}
		note = parent->firstNote;
		child = parent->firstChild;
		while(note && !error){
			found = 0;
			i = j = 0;
			anchor = EDI_FALSE;
			while(child){
				i++;
				if(i == note->positions[j]){
					j++;
					if(child->count > 0){
						found++;
						if(j == 1){
							anchor = EDI_TRUE;
						}
					}
				}
				child = child->nextSibling;
			}
			switch(note->type){
			case EDI_SYNTAX_PAIRED:
				if(found && found != note->count){
					error = VAL_MISSING_CONDITIONAL;
				}
				break;
			case EDI_SYNTAX_REQUIRED:
				if(!found){
					error = VAL_MISSING_CONDITIONAL;
				}
				break;
			case EDI_SYNTAX_EXCLUSION:
				if(found > 1){
					error = VAL_EXCLUSION_VIOLATED;
				}
				break;
			case EDI_SYNTAX_CONDITION:
				if(anchor && found < note->count){
					error = VAL_MISSING_CONDITIONAL;
				}
				break;
			case EDI_SYNTAX_LIST:
				if(anchor && found == 1){
					error = VAL_MISSING_CONDITIONAL;
				}
				break;
			}
			note = note->next;
		}
	} else {
		error = VAL_INVALID_SEGMENT;
	}
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
