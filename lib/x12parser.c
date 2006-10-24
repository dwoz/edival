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

#include "x12parser.h"
/******************************************************************************/
#define X12_PARSER ((X12_Parser)parser->child)
#define EDI_PARSER (parser)

EDI_StateHandler X12_ProcessISA(EDI_Parser);
EDI_StateHandler X12_ProcessMessage(EDI_Parser);
EDI_StateHandler X12_ProcessBinaryElement(EDI_Parser);
EDI_StateHandler X12_ProcessIEA(EDI_Parser);
void X12_ParserDestroy(EDI_Parser);

/******************************************************************************/
struct token {
    enum X12_Delimiter  type;
    char               *token;
    int                 length;
};
/******************************************************************************/
#define X12_NEXT_TOKEN(string, delimiters, tok) \
tok.type  = UNKNOWN;\
tok.token = NULL;\
tok.length = 0;\
if(string){\
	char *iter;\
	for(iter = string; *iter; iter++){\
        if(!(*iter ^ delimiters[ELEMENT])){\
				tok.type = ELEMENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(!(*iter ^ delimiters[SEGMENT])){\
            tok.type = SEGMENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(!(*iter ^ delimiters[COMPONENT])){\
            tok.type = COMPONENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(!(*iter ^ delimiters[REPEAT])){\
				tok.type = REPEAT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        }\
        tok.length++;\
	}\
}
/******************************************************************************/
#define ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, component, value, length) \
if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){\
	EDI_ValidateElement(  \
		EDI_PARSER->schema,\
		element           ,\
		component         ,\
		value             ,\
		length            ,\
		X12_PARSER->data   \
	);                    \
}
/******************************************************************************/
EDI_StateHandler X12_ProcessISA(EDI_Parser parser)
{
	const unsigned char ISA_seps[16] = {  3,  6, 17, 20, 31, 34,  50,  53,
                                        69, 76, 81, 83, 89, 99, 101, 103 };
	unsigned int    i = 0,  j = 0;
	char            elesep       = 0;
	char           *bufIter      = NULL;
	char            componentSep = '\0';
	char            repeatSep    = '\0';
	struct token    tok;

	if((EDI_PARSER->bufEndPtr - EDI_PARSER->bufReadPtr) < 106){
		EDI_SetResumeState(EDI_PARSER->machine, EDI_PARSER->process);
		EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
		return EDI_PARSER->error;
	}
	bufIter = EDI_PARSER->bufReadPtr;
	elesep = bufIter[3];
	for(i = 0; i < 16; i++){
		for(j = 0; j < 106; j++){
			if(j == ISA_seps[i]){
				if(bufIter[j] == elesep){
					j=ISA_seps[i]+1;
					i++;
				} else {
					EDI_PARSER->bufReadPtr += 3;
					return EDI_PARSER->seekHeader;
				}
			} else if(bufIter[j] == elesep){
				EDI_PARSER->bufReadPtr += 3;
				return EDI_PARSER->seekHeader;
			}
		}
	}
	EDI_PARSER->bufReadPtr += 106;
	X12_PARSER->delimiters[ELEMENT] = bufIter[3];
	repeatSep                       = bufIter[82];
	componentSep                    = bufIter[104];
	X12_PARSER->delimiters[SEGMENT] = bufIter[105];
	X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	if(EDI_PARSER->schema->documentType == EDI_ANSI_X12){
		EDI_PARSER->validate = EDI_TRUE;
		X12_PARSER->segmentError = EDI_ValidateSegmentPosition(EDI_PARSER->schema, "ISA");
	} else {
		EDI_PARSER->validate = EDI_FALSE;
	}
	EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "ISA");
	i = j = 0;
	while(tok.type != SEGMENT){
		X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
		i++;
		/* Setup the defaults: string data and the unconverted input string */
		X12_PARSER->data->type = EDI_DATA_STRING;
		X12_PARSER->data->data.string = tok.token;
		switch(tok.type){
			case ELEMENT:
			{
				ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, i, &j, tok.token, tok.length);
				if(i == 12){
					char *error;
					long  version = strtol(tok.token, &error, 10);
					if(version == 0 && error != '\0'){
						X12_PARSER->error = X12_INVALID_VERSION;
						/* should generate TA1 error info here. */
					}
					if(version >= 402 && !(isalnum(repeatSep))){
						X12_PARSER->delimiters[REPEAT] = repeatSep;
					}
					X12_PARSER->version = version / 100;
					X12_PARSER->release = version % 100;
				}
				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
				break;
			}
			case SEGMENT:
				ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, i, &j, tok.token, tok.length);
				//EDI_ValidateElement(X12_PARSER->x12Schema, i, &j, tok.token, strlen(tok.token), X12_PARSER->data);
				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, "ISA");
				break;
			default:
				break;
		}
	}
	X12_PARSER->delimiters[COMPONENT] = componentSep;
	X12_PARSER->previous = SEGMENT;
	X12_PARSER->savedElementPosition = 0;
	X12_PARSER->savedComponentPosition = 0;
	while(bufIter < EDI_PARSER->bufEndPtr){
		if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
			bufIter -= 4;
			if(!(isalnum(*bufIter))){
				bufIter++;
			}
			EDI_GAP_SCAN(EDI_PARSER, bufIter);
			if((strncmp(bufIter, "IEA", 3) == 0)){
				return (void *)X12_ProcessIEA;
			} else {
				return (void *)X12_ProcessMessage;
			}
		}
	}
	EDI_SetResumeState(EDI_PARSER->machine, (void *)X12_ProcessMessage);
	EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
	return EDI_PARSER->error;
}
/******************************************************************************/
EDI_StateHandler X12_ProcessMessage(EDI_Parser parser)
{
	int             element   = 0;
	int             component = 0;
	char           *tag       = NULL;
	char           *bufIter   = NULL;
	struct token    tok;

	bufIter = EDI_PARSER->bufReadPtr;
	X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	if(X12_PARSER->previous == SEGMENT){
		if(tok.type == ELEMENT){
			while(!(isalnum(tok.token[0]))){
				tok.token++;
			}
			EDI_GAP_SCAN(EDI_PARSER, tok.token);
		 	/*if(string_eq(tok.token, "IEA")){
		 		EDI_ValidateSegmentPosition(X12_PARSER->x12Schema, "IEA");
				EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "IEA");
				X12_PARSER->previous = ELEMENT;
				EDI_PARSER->bufReadPtr = bufIter;
				return (void *)X12_ProcessIEA;
			} else*/ 
			if(string_eq(tok.token, "ISA")){
				/* If this happens, we had an incomplete interchange (missing IEA) 
				 * and we were lucky enough that the next interchange used the same
				 * delimiter! */
				*--bufIter = X12_PARSER->delimiters[ELEMENT];
				bufIter -= 3;
				X12_PARSER->delimiters[COMPONENT] = '\0';
				X12_PARSER->delimiters[ELEMENT]   = '\0';
				X12_PARSER->delimiters[REPEAT]    = '\0';
				X12_PARSER->delimiters[SEGMENT]   = '\0';
				X12_PARSER->version             = 0;
				X12_PARSER->release             = 0;
				EDI_PARSER->bufReadPtr = bufIter;
				return (void *)X12_ProcessISA;
			}
		}
	} else {
		tag = X12_PARSER->savedTag;
		element = X12_PARSER->savedElementPosition;
		component = X12_PARSER->savedComponentPosition;
	}
	while(tok.type != UNKNOWN){
		EDI_PARSER->bufReadPtr = bufIter;
		/* Setup the defaults: string data and the unconverted input string */
		X12_PARSER->data->type = EDI_DATA_STRING;
		X12_PARSER->data->data.string = tok.token;
		switch(tok.type){
			case REPEAT:
				if(EDI_PARSER->repeatHandler){
					EDI_PARSER->repeatHandler(EDI_PARSER->userData);
				} else {
					fprintf(stderr, "FATAL (edival): A repeated element was found in the data stream but no \n\
					callback function has been registered for the event using EDI_SetRepeatHandler()\n");
					EDI_PARSER->errorCode = EDI_ERROR_INVALID_STATE;
					return EDI_PARSER->error;
				}
			case ELEMENT:
	        	switch(X12_PARSER->previous){
	        		case COMPONENT:
	        			component++;
	        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	        			EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
		  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
							EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
		  				}
		  				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        			break;
	        		case SEGMENT:
	        			tag = tok.token;
	        			X12_PARSER->segmentError = SEGERR_NONE;
	        			if(EDI_PARSER->validate){
	        				X12_PARSER->segmentError = EDI_ValidateSegmentPosition(EDI_PARSER->schema, tag);
	        			} else {
	        				X12_PARSER->segmentError = SEGERR_UNDEFINED;
	        			}
	        			EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, tag);
	        			break;
	        		case ELEMENT:
	        			element++;
	        		case REPEAT:
	        			component = 0;
	        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	        			if(component == 1){
	        				EDI_PARSER->compositeStartHandler(EDI_PARSER->userData);
	        				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
			  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
								EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
			  				}
	        				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        			} else {
	        				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
	        			}
	        			break;
					default: ;
	        	}
	        	break;
			case COMPONENT:
				if(X12_PARSER->previous == ELEMENT){
					element++;
					component = 0;
				} else if(X12_PARSER->previous == REPEAT){
					component = 0;
				}
				component++;
				ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
     			if(component == 1){
     				EDI_PARSER->compositeStartHandler(EDI_PARSER->userData);
     			}
     			EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
				break;
			case SEGMENT:
	        	if(X12_PARSER->previous == COMPONENT){
	        		component++;
        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	        		EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
	  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
						EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
	  				}
	        		EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        	} else {
	        		if(X12_PARSER->previous == ELEMENT){
	        			/* Only increment the element count if this isn't a repeated element. */
	        			element++;
	        		}
	        		component = 0;
        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	     			if(component == 1){
	     				EDI_PARSER->compositeStartHandler(EDI_PARSER->userData);
	     				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
		  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
							EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
		  				}
	     				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	     			} else {
	     				EDI_PARSER->elementHandler(EDI_PARSER->userData, X12_PARSER->data);
	     			}
	        	}
  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
					EDI_ValidateSyntax(EDI_PARSER->schema, 0, element);
  				}
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, tag);
				element = 0;
    			component = 0;
				if(tag[0] == 'I' && tag[1] == 'E' && tag[2] == 'A'){
					X12_PARSER->previous = tok.type;
					return (void *)X12_ProcessIEA;
				} else {
					tag = NULL;
					while(bufIter < EDI_PARSER->bufEndPtr){
						if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
							bufIter -= 4;
							while(!(isalnum(*bufIter))){
								bufIter++;
							}
							EDI_GAP_SCAN(EDI_PARSER, bufIter);
							if(bufIter[0] == 'I' && bufIter[1] == 'S' && bufIter[2] == 'A'){
								/* If this happens, we probably have a bad interchange. (missing IEA)
								 * Same scenario as above. */
								X12_PARSER->delimiters[COMPONENT] = '\0';
								X12_PARSER->delimiters[ELEMENT]   = '\0';
								X12_PARSER->delimiters[REPEAT]    = '\0';
								X12_PARSER->delimiters[SEGMENT]   = '\0';
								X12_PARSER->version             = 0;
								X12_PARSER->release             = 0;
								return EDI_PARSER->seekHeader;
							}
							break;
						}
					}
				}
				break;
			default: ;
		}
		if(EDI_PARSER->binaryElementSize){
			if(EDI_PARSER->binBuffer){
				FREE(parser, EDI_PARSER->binBuffer);
			}
			EDI_PARSER->binBuffer = MALLOC(parser, EDI_PARSER->binaryElementSize * sizeof(char));
	    	if(X12_PARSER->savedTag){
	    		FREE(EDI_PARSER, X12_PARSER->savedTag);
	    	}
	    	X12_PARSER->savedTag = EDI_strdup(tag);
			X12_PARSER->savedElementPosition = element;
			X12_PARSER->savedComponentPosition = component;
	    	return (void *)X12_ProcessBinaryElement;
		} else {
			X12_PARSER->previous = tok.type;
			X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
		}
	}
	if(tag){
    	if(X12_PARSER->savedTag){
    		FREE(EDI_PARSER, X12_PARSER->savedTag);
    	}
    	X12_PARSER->savedTag = EDI_strdup(tag);
		X12_PARSER->savedElementPosition = element;
		X12_PARSER->savedComponentPosition = component;
	}
	EDI_SetResumeState(EDI_PARSER->machine, (void *)X12_ProcessMessage);
	EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
	return EDI_PARSER->error;
}
/******************************************************************************/
EDI_StateHandler X12_ProcessBinaryElement(EDI_Parser parser)
{
	char              *bufIter  = EDI_PARSER->bufReadPtr;
	long long int      size     = EDI_PARSER->binaryElementSize;
	long long int      finished = EDI_PARSER->bytesHandled;
	enum X12_Delimiter type     = UNKNOWN;
	
	/*  
	 *  If the current buffer does not hold the entire binary element, we
	 *  will fall back to the caller who has the responsibilty to load more
	 *  data to the buffer.  This will repeat until the entire binary element
	 *  is present in memory. 
	 */
	if((EDI_PARSER->bufEndPtr - bufIter) > (size - finished)){
		memcpy(
			&(EDI_PARSER->binBuffer[finished]), 
			bufIter, 
			(size - finished)
		);
		finished = size;
		EDI_PARSER->bufReadPtr += size;
	} else {
		memcpy(
			&(EDI_PARSER->binBuffer[finished]), 
			(void *)bufIter,
			(EDI_PARSER->bufEndPtr - bufIter)
		);
		finished += (EDI_PARSER->bufEndPtr - bufIter);
		EDI_PARSER->bufReadPtr = EDI_PARSER->bufEndPtr;
	}
	if(finished == size){
		if(!(*EDI_PARSER->bufReadPtr ^ X12_PARSER->delimiters[REPEAT])){
			type = REPEAT;
		} else if(!(*EDI_PARSER->bufReadPtr ^ X12_PARSER->delimiters[ELEMENT])){
			type = ELEMENT;
		} else if(!(*EDI_PARSER->bufReadPtr ^ X12_PARSER->delimiters[COMPONENT])){
			type = COMPONENT;
		} else if(!(*EDI_PARSER->bufReadPtr ^ X12_PARSER->delimiters[SEGMENT])){
			type = SEGMENT;
		}
		int element = X12_PARSER->savedElementPosition;
		int component = X12_PARSER->savedComponentPosition;			
		switch(type){
			case ELEMENT:
			case COMPONENT:
	        	switch(X12_PARSER->previous){
	        		case COMPONENT:
	        			component++;
	        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, NULL, 1);
	        			EDI_PARSER->binBufferHandler(EDI_PARSER->userData, EDI_PARSER->binBuffer, EDI_PARSER->binaryElementSize);
		  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
							EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
		  				}
		  				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        			break;
	        		case ELEMENT:
	        			element++;
	        			component = 0;
	        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, NULL, 1);
	        			if(component == 1){
							fprintf(stderr, "FATAL (edival): Invalid Binary data element type!  Terminating process.");
							EDI_PARSER->errorCode = EDI_ERROR_INVALID_STATE;
							return EDI_PARSER->error;
	        			} else {
	        				EDI_PARSER->binBufferHandler(EDI_PARSER->userData, EDI_PARSER->binBuffer, EDI_PARSER->binaryElementSize);
	        			}
	        			break;
					default: 
						fprintf(stderr, "FATAL (edival): Invalid Binary data element type!  Terminating process.");
						EDI_PARSER->errorCode = EDI_ERROR_INVALID_STATE;
						return EDI_PARSER->error;
	        	}
	        	break;
			case SEGMENT:
	        	if(X12_PARSER->previous == COMPONENT){
	        		component++;
        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, NULL, 1);
	        		EDI_PARSER->binBufferHandler(EDI_PARSER->userData, EDI_PARSER->binBuffer, EDI_PARSER->binaryElementSize);
	  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
						EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
	  				}
	        		EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        	} else {
	        		if(X12_PARSER->previous == ELEMENT){
	        			/* Only increment the element count if this isn't a repeated element. */
	        			element++;
	        		}
	        		component = 0;
        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, NULL, 1);
	     			if(component == 1){
						fprintf(stderr, "FATAL (edival): Invalid Binary data element type!  Terminating process.");
						EDI_PARSER->errorCode = EDI_ERROR_INVALID_STATE;
						return EDI_PARSER->error;
	     			} else {
	     				EDI_PARSER->binBufferHandler(EDI_PARSER->userData, EDI_PARSER->binBuffer, EDI_PARSER->binaryElementSize);
	     			}
	        	}
  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
					EDI_ValidateSyntax(EDI_PARSER->schema, 0, element);
  				}
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, X12_PARSER->savedTag);
				break;
			default:
				fprintf(stderr, "FATAL (edival): Invalid Binary data element type!  Terminating process.");
				EDI_PARSER->errorCode = EDI_ERROR_INVALID_STATE;
				return EDI_PARSER->error;
		}
		EDI_PARSER->bufReadPtr++;
		X12_PARSER->previous = type;
		EDI_PARSER->binaryElementSize = 0;
		return (void *)X12_ProcessMessage;
	}
	EDI_SetResumeState(EDI_PARSER->machine, (void *)X12_ProcessBinaryElement);
	EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
	return EDI_PARSER->error;
}
/******************************************************************************/
EDI_StateHandler X12_ProcessIEA(EDI_Parser parser)
{
	/* If this is the end of the buffer and there are no more ISA segments
	 * possible, change the state to finished. */
	if(parser->final && ((EDI_PARSER->bufEndPtr - EDI_PARSER->bufReadPtr) < 106)){
		if(parser->schema && 
			parser->schema->loopEndHandler &&
			parser->schema->root){
			parser->schema->loopEndHandler(
				parser->userData,
				(EDI_GetDocumentRoot(parser->schema))->nodeID
			);
		}
		EDI_PARSER->errorCode = EDI_ERROR_FINISHED;
		return EDI_PARSER->error;
	} else {	
	/* Otherwise, initialize the parser and go back to the generic EDI parser
	 * and look for more (not necessarily X12) data. */
		X12_PARSER->delimiters[COMPONENT] = '\0';
		X12_PARSER->delimiters[ELEMENT]   = '\0';
		X12_PARSER->delimiters[REPEAT]    = '\0';
		X12_PARSER->delimiters[SEGMENT]   = '\0';
		X12_PARSER->version               = 0;
		X12_PARSER->release               = 0;
		if(X12_PARSER->savedTag){
			free(X12_PARSER->savedTag);
		}
		X12_PARSER->savedElementPosition   = 0;
		X12_PARSER->savedComponentPosition = 0;
		X12_PARSER->error                  = X12_ERROR_NONE;
		X12_PARSER->previous               = SEGMENT;
		return EDI_PARSER->seekHeader;
	}
}
/******************************************************************************/
X12_Parser X12_ParserCreate(EDI_Parser parent)
{
	X12_Parser new = NULL;
	new = MALLOC(parent, sizeof(struct X12_ParserStruct));
	if(new){
		memset(new, 0, sizeof(struct X12_ParserStruct));
		new->data                   = MALLOC(parent, sizeof(struct EDI_DataElementStruct));
		parent->process             = (void *)X12_ProcessISA;
		parent->freeChild           = (void *)X12_ParserDestroy;
		EDI_AddState(parent->machine, (void *)X12_ProcessISA, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessMessage, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessBinaryElement, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessIEA, 0);
		new->parent = parent;
	}
	return new;
}
/******************************************************************************/
void X12_ParserDestroy(EDI_Parser parser)
{
	EDI_PARSER->process = NULL;
	EDI_RemoveState(EDI_PARSER->machine, (void *)X12_ProcessISA, 0);
	EDI_RemoveState(EDI_PARSER->machine, (void *)X12_ProcessMessage, 0);
	EDI_RemoveState(EDI_PARSER->machine, (void *)X12_ProcessBinaryElement, 0);
	EDI_RemoveState(EDI_PARSER->machine, (void *)X12_ProcessIEA, 0);
	if(X12_PARSER->savedTag){
		free(X12_PARSER->savedTag);
	}
	FREE(parser, X12_PARSER);
	EDI_PARSER->child = NULL;
}
