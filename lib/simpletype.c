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

#include "simpletype.h"
/******************************************************************************/
EDI_SchemaNode EDI_CreateElementType(EDI_Schema                  schema,
                                     enum EDI_PrimitiveDataType  type  ,
                                     const char                 *id    , 
                                     long long int               minlen,
                                     long long int               maxlen)
{
	EDI_SimpleType *node = NULL;
	
	node = MALLOC(schema, sizeof(EDI_SimpleType));
	if(!node){
		return NULL;
	}
	node->header.type = EDITYPE_ELEMENT;
	node->header.nodeID = strndup(id, strlen(id), schema->memsuite);
	node->header.refCount = 0;
	node->type = type;
	if(type == EDI_DATA_INTEGER_POS || type == EDI_DATA_DECIMAL_POS){
		node->min = 0;
	} else {
		node->min = minlen;
	}
	node->max = maxlen;
	node->values = NULL;
	if(hashtable_insert(schema->elements, (void *)strdup(id), (void *)node)){
		return (EDI_SchemaNode)node;
	}
	return NULL;
}
/******************************************************************************/
EDI_SchemaNode EDI_GetElementByID(EDI_Schema      schema,
                                  const char     *nodeID)
{
	EDI_SchemaNode    node  = NULL;
	struct hashtable *table = NULL;
	
	table = schema->elements;
	if((node = (EDI_SchemaNode)hashtable_search(table, (void *)nodeID))){
		return node;
	} else {
		return NULL;
	}
}

/******************************************************************************/
enum EDI_ElementValidationError EDI_AddElementValue(EDI_Schema      schema,
                                                    const char     *nodeID,
                                                    const char     *value )
{
	struct hashtable               *table   = NULL;
	EDI_SimpleType                 *element = NULL;
	enum EDI_ElementValidationError error   = VAL_UNKNOWN_ELEMENT;
	
	table = schema->elements;
	if((element = (EDI_SimpleType *)hashtable_search(table, (void *)nodeID))){
		error = EDI_CheckElementConstraints(schema, nodeID, value);
		if(!error || error == VAL_CODE_ERROR){
			if(!element->values){
				element->values = create_hashtable(20);
			}
			if(hashtable_insert(element->values, (void *)strdup(value), (void *)strdup(value))){
				error = VAL_VALID_ELEMENT;
			}
		}
	}
	return error;
}
/******************************************************************************/
enum EDI_ElementValidationError EDI_CheckElementConstraints(EDI_Schema  schema,
                                                            const char *nodeID,
                                                            const char *value)
{
	EDI_SimpleType   *element = NULL;
	struct hashtable *table   = NULL;
	char             *invalid = NULL;
	long long int     lvalue  = 0;
	long long int     i       = 0;
	
	table = schema->elements;
	if((element = (EDI_SimpleType *)hashtable_search(table, (void *)nodeID))){
		switch(element->type){
			case EDI_DATA_STRING:
				lvalue = strlen(value);
				if(lvalue > element->max){
					return VAL_RANGE_HIGH;
				}
				if(lvalue < element->min){
					return VAL_RANGE_LOW;
				}
				for(i = 0; i < lvalue; i++){
					if(!isprint(value[i])){
						return VAL_CHAR_ERROR;
					}
				}
				if(element->values){
					if(! hashtable_search(element->values, (void *)strdup(value))){
						//fprintf(stderr, "%p\n", hashtable_search(element->values, (void *)strdup(value)));
						return VAL_CODE_ERROR;
					}
				}
				break;
			case EDI_DATA_INTEGER:
				lvalue = strtoll(value, &invalid, 10);
				if(value[0] == '\0' || *invalid != '\0'){
					return VAL_CHAR_ERROR;
				}
				if(lvalue > element->max){
					return VAL_RANGE_HIGH;
				}
				if(lvalue < element->min){
					return VAL_RANGE_LOW;
				}
				break;
			case EDI_DATA_INTEGER_POS:
				lvalue = strtol(value, &invalid, 10);
				break;
			default:
				return VAL_UNKNOWN_ELEMENT;
				break;
		}
		return VAL_VALID_ELEMENT;
	} else {
		return VAL_UNKNOWN_ELEMENT;
	}
}
/******************************************************************************/
void EDI_DisposeSimpleType(EDI_Schema     schema,
                           EDI_SchemaNode node  )
{
	EDI_SimpleType   *element = NULL;
	struct hashtable *table   = NULL;

	if(!node){
		return;
	}
	if(node->refCount == 0){
		table = schema->elements;
		if(table){
			element = (EDI_SimpleType*)hashtable_search(table, node->nodeID);
			if(element && element->values){
				hashtable_destroy(element->values, 0);
			}
		}
		FREE(schema, node->nodeID);
		FREE(schema, node);
	}
	return;
}
