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
#include "simpletype.h"
/******************************************************************************/
EDI_SchemaNode EDI_CreateComplexType(EDI_Schema         schema,
                                     enum EDI_NodeType  type  ,
                                     const char        *id    )
{
	EDI_ComplexType node = NULL;
	
	if(type == EDITYPE_LOOP){
		node = MALLOC(schema, sizeof(struct EDI_LoopNodeStruct));
	} else {
		node = MALLOC(schema, sizeof(struct EDI_ComplexTypeStruct));
	}
	if(!node){
		return NULL;
	}
	node->header.type = type;
	node->header.nodeID = strndup(id, strlen(id), schema->memsuite);
	node->header.refCount = 0;

	node->firstChild = NULL;
	node->lastChild  = NULL;
	node->childCount = 0;
	if(type == EDITYPE_LOOP){
		((struct EDI_LoopNodeStruct *)node)->position = 0;
		((struct EDI_LoopNodeStruct *)node)->values   = NULL;
	}
	if(type == EDITYPE_TRANSACTION){
		schema->root = node;
		schema->stack[0] = schema->root->firstChild;
	}
	return (EDI_SchemaNode)node;
}
/******************************************************************************/
void EDI_StoreComplexNode(EDI_Schema     schema,
                          EDI_SchemaNode node  )
{
	if(node && node->type != EDITYPE_ELEMENT){
		hashtable_insert(schema->complexNodes, 
		                 (void *)strdup(node->nodeID), 
		                 (void *)node);
	}
	return;
}
/******************************************************************************/
EDI_SchemaNode EDI_GetComplexNodeByID(EDI_Schema      schema,
                                      const char     *nodeID)
{
	EDI_SchemaNode    node  = NULL;
	struct hashtable *table = NULL;
	
	table = schema->complexNodes;
	if((node = (EDI_SchemaNode)hashtable_search(table, (void *)nodeID))){
		return node;
	} else {
		return NULL;
	}
}

/******************************************************************************/
enum EDI_ElementValidationError EDI_SetLoopID(EDI_Schema     schema  ,
                                              EDI_SchemaNode node    ,
                                              unsigned int   position, 
                                              unsigned int   count   ,
                                              ...)
{
	int                             i        = 0;
	char                           *name     = NULL;
	EDI_ComplexType                 segment  = NULL;
	EDI_ChildNode                   element  = NULL;
	struct EDI_LoopNodeStruct      *loopNode = NULL;
	enum EDI_ElementValidationError error    = VAL_VALID_ELEMENT;
	va_list                         argp;
	
	va_start(argp, count);
	loopNode = (struct EDI_LoopNodeStruct *)node;
	segment = (EDI_ComplexType)((EDI_ComplexType)node)->firstChild->node;
	element = segment->firstChild;
	while(++i < position) {
		element = element->nextSibling;
	}
	name = element->node->nodeID;
	for(i = 0; i < count; i++){
		char *value = va_arg(argp, char *);
		error = EDI_CheckElementConstraints(schema, name, value);
		if(!error){
			if(!loopNode->values){
				loopNode->values = create_hashtable(5);
			}
			hashtable_insert(loopNode->values, (void *)value, (void *)value);
		}
	}
	va_end(argp);
	return error;
}
/******************************************************************************/
void EDI_AddSyntaxNote(EDI_Schema          schema,
                       EDI_SchemaNode      node  ,
                       enum EDI_SyntaxType type  , 
                       unsigned int        count , 
                       ...)
{
	fprintf(stderr, "FIXME: EDI_AddSyntaxNode not yet implemented.");
}
/******************************************************************************/
static EDI_SchemaNode EDI_RemoveChild(EDI_Schema      schema,
                                      EDI_ComplexType parent,
                                      EDI_ChildNode   child )
{
	EDI_SchemaNode detached;

    if(!child){
        return NULL;
    }
    detached = child->node;
    if(parent->firstChild == child){
        parent->firstChild = child->nextSibling;
        if(parent->firstChild){
            parent->firstChild->previousSibling = NULL;
        }
    } else if(child->nextSibling){
        child->nextSibling->previousSibling = child->previousSibling;
    }
    if(parent->lastChild == child){
        parent->lastChild = child->previousSibling;
        if(parent->lastChild){
            parent->lastChild->nextSibling = NULL;
        }
    } else if(child->previousSibling){
        child->previousSibling->nextSibling = child->nextSibling;
    }
    FREE(schema, child);
    parent->childCount--;
    detached->refCount--;
    return detached;
}
/******************************************************************************/
EDI_SchemaNode EDI_AppendType(EDI_Schema     schema    ,
                              EDI_SchemaNode parent    ,
                              EDI_SchemaNode new       ,
                              unsigned int   min_occurs,
                              unsigned int   max_occurs)
{
    EDI_ChildNode   childMeta  = NULL;
    EDI_ComplexType cParent    = (EDI_ComplexType)parent;
    
    if(!parent || !new){
        return NULL;
    }
    switch(parent->type){
    	case EDITYPE_TRANSACTION:
    		if(new->type != EDITYPE_LOOP && 
    		   new->type != EDITYPE_SEGMENT){
    			return NULL;
    		}
    	case EDITYPE_LOOP:
    		if(new->type != EDITYPE_LOOP && 
    		   new->type != EDITYPE_SEGMENT){
    			return NULL;
    		}
    		if(cParent->childCount == 0){
    			((struct EDI_LoopNodeStruct*)(parent))->startID = new->nodeID;
    		}
    		break;
    	case EDITYPE_SEGMENT:
    		if(new->type != EDITYPE_COMPOSITE && 
    		   new->type != EDITYPE_ELEMENT){
    			return NULL;
    		}
    		break;
    	case EDITYPE_COMPOSITE:
    		if(new->type != EDITYPE_ELEMENT){
    			return NULL;
    		}
    		break;
    	default:
    		return NULL;
    }
    childMeta = MALLOC(schema, sizeof(struct EDI_ChildNodeStruct));
    if(!childMeta){
    	schema->parser->errorCode = EDI_ERROR_NO_MEM;
    	return NULL;
    }
    childMeta->min_occurs = min_occurs;
    childMeta->max_occurs = max_occurs;
    childMeta->node       = new;
    childMeta->count      = 0;
    
    if(cParent->lastChild){
        cParent->lastChild->nextSibling = childMeta;
    } else {
        cParent->firstChild = childMeta;
    }
	childMeta->previousSibling = cParent->lastChild;
	cParent->lastChild = childMeta;
	childMeta->nextSibling = NULL;
	cParent->childCount++;
	new->refCount++;

	if(!schema->stack[0] && schema->root){
		schema->stack[0] = schema->root->firstChild;
	}
	return new;
}
/******************************************************************************/
void EDI_DisposeComplexType(EDI_Schema      schema,
                            EDI_ComplexType node  )
{
    EDI_SchemaNode child;

    if(!node){
        return;
    }
	if(node->header.refCount == 0){
		while(node->childCount > 0){
			child = EDI_RemoveChild(schema, node, node->firstChild);
			EDI_DisposeNode(schema, child);
		}
	    if(node->header.nodeID){
	        FREE(schema, node->header.nodeID);
	    }
		FREE(schema, node);
	}
	return;
}
