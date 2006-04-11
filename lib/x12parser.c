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
#include <unistd.h>
/******************************************************************************/
#define X12_PARSER ((X12_Parser)parser->child)
#define EDI_PARSER (parser)

EDI_StateHandler X12_ProcessISA(EDI_Parser);
EDI_StateHandler X12_ProcessMessage(EDI_Parser);
EDI_StateHandler X12_ProcessIEA(EDI_Parser);
void X12_ParserDestroy(EDI_Parser parser);

/******************************************************************************/
struct token {
    enum X12_Delimiter  type;
    char               *token;
};
/******************************************************************************/
#define X12_NEXT_TOKEN(string, delimiters, tok) \
tok.type  = UNKNOWN;\
tok.token = NULL;\
if(string){\
	char *iter;\
	for(iter = string; *iter; iter++){\
        if(*iter == delimiters[ELEMENT]){\
        	tok.type = ELEMENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(*iter == delimiters[SEGMENT]){\
            tok.type = SEGMENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(*iter == delimiters[COMPONENT]){\
            tok.type = COMPONENT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        } else if(*iter == delimiters[REPEAT]){\
        	tok.type = REPEAT;\
            *iter++ = '\0';\
            tok.token = string;\
            string = iter;\
            break;\
        }\
	}\
}\
/******************************************************************************/
EDI_StateHandler X12_ProcessISA(EDI_Parser parser)
{
	unsigned char     ISA_seps[16] = {  3,  6, 17, 20, 31, 34,  50,  53, 
                                       69, 76, 81, 83, 89, 99, 101, 103 };
	unsigned char     i = 0,  j = 0;
	char              elesep       = 0;
	char             *bufIter      = NULL;
	char              componentSep = '\0';
	char              repeatSep    = '\0';
	struct token      tok;

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
	EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "ISA");
	i = 0;
	while(tok.type != SEGMENT){
		X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
		i++;
		switch(tok.type){
			case ELEMENT:
				if(i == 12){
					char *error;
					long  version = strtol(tok.token, &error, 10);
					if(version == 0 && error != '\0'){
						X12_PARSER->error = X12_INVALID_VERSION;
						/* should generate TA1 error info here. */
					}
					if(version < 100 || version > 501){
						X12_PARSER->error = X12_UNSUPPORTED_VERSION;
						/* should generate TA1 error info here. */
					}
					if(version >= 402 && !(isalnum(repeatSep))){
						X12_PARSER->delimiters[REPEAT] = repeatSep;
					}
					X12_PARSER->version = version / 100;
					X12_PARSER->release = version % 100;
				}
				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
				break;
			case SEGMENT:
				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, "ISA");
				break;
			default:
				break;
		}
	}
	X12_PARSER->delimiters[COMPONENT] = componentSep;
	X12_PARSER->previous = SEGMENT;
	while(bufIter < EDI_PARSER->bufEndPtr){
		if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
			bufIter -= 4;
			if(!(isalnum(*bufIter))){
				bufIter++;
			}
			EDI_GAP_SCAN(EDI_PARSER, bufIter);
			if((strncmp(bufIter, "GS", 2) == 0)){
				return (void *)X12_ProcessMessage;
			}
			if((strncmp(bufIter, "IEA", 3) == 0)){
				return (void *)X12_ProcessIEA;
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
	int                        element   = 0;
	int                        component = 0;
	char                      *tag       = NULL;
	char                      *bufIter   = NULL;
	struct token               tok;

	bufIter = EDI_PARSER->bufReadPtr;
	X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	if(X12_PARSER->previous == SEGMENT){
		if(tok.type == ELEMENT && strcmp(tok.token, "IEA") == 0){
			EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "IEA");
			X12_PARSER->previous = ELEMENT;
			EDI_PARSER->bufReadPtr = bufIter;
			return (void *)X12_ProcessIEA;
		}
	} else {
		tag = X12_PARSER->savedTag;
		element = X12_PARSER->savedElementPosition;
		component = X12_PARSER->savedComponentPosition;
	}
	while(tok.type != UNKNOWN){
		EDI_PARSER->bufReadPtr = bufIter;
		switch(tok.type){
			case ELEMENT:
	        	switch(X12_PARSER->previous){
	        		case COMPONENT:
	        			component++;
	        			EDI_PARSER->componentHandler(EDI_PARSER->userData, tok.token);
	        			break;
	        		case SEGMENT:
	        			tag = tok.token;
	        			if(EDI_PARSER->validate){
	        				if(strcmp(tag, "GS") != 0 && strcmp(tag, "GE") != 0){
	        					enum EDI_SegmentValidationError e = 
									EDI_ValidateSegmentPosition(EDI_PARSER->schema, tag);
	        					if(e){
	        						fprintf(stderr, "SEG_ERR: %d\n", e);
	        					}
	        				}
	        			}
	        			EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, tag);
	        			break;
	        		case ELEMENT:
	        			element++;
	        			//fall thru
	        		case REPEAT:
	        			component = 0;
	        			if(EDI_PARSER->validate){
	        				if(strcmp(tag, "GS") != 0 && strcmp(tag, "GE") != 0){
	        					enum EDI_ElementValidationError e = 
									EDI_ValidateElement(EDI_PARSER->schema, element, component, tok.token);
	        					if(e){
	        						fprintf(stderr, "\nELEM_ERR: %d\n", e);
	        					}
	        				}
	        			}
	        			EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
	        			break;
					default: ;
	        	}
	        	break;
			case COMPONENT:
				component++;
				EDI_PARSER->componentHandler(EDI_PARSER->userData, tok.token);
				break;
			case REPEAT:
				component = 0;
				EDI_PARSER->repeatHandler(EDI_PARSER->userData);
				break;
			case SEGMENT:
	        	if(X12_PARSER->previous == COMPONENT){
	        		component++;
        			if(EDI_PARSER->validate){
        				if(strcmp(tag, "GS") != 0 && strcmp(tag, "GE") != 0){
							enum EDI_ElementValidationError e = 
								EDI_ValidateElement(EDI_PARSER->schema, element, component, tok.token);
							if(e){
								fprintf(stderr, "\nELEM_ERR: %d\n", e);
							}
        				}
        			}
	        		EDI_PARSER->componentHandler(EDI_PARSER->userData, tok.token);
	        	} else {
	        		// Only reset the element count if this isn't a repeated element.
	        		if(X12_PARSER->previous == ELEMENT){
	        			element++;
	        		}
	        		component = 0;
        			if(EDI_PARSER->validate){
        				if(strcmp(tag, "GS") != 0 && strcmp(tag, "GE") != 0){
							enum EDI_ElementValidationError e = 
								EDI_ValidateElement(EDI_PARSER->schema, element, component, tok.token);
							if(e){
								fprintf(stderr, "\nELEM_ERR: %d\n", e);
							}
        				}
        			}
	        		EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
	        	}
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, tag);
				tag = NULL;
				element = 0;
    			component = 0;
				while(bufIter < EDI_PARSER->bufEndPtr){
					if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
						bufIter -= 4;
						if(!(isalnum(*bufIter))){
							bufIter++;
						}
						EDI_GAP_SCAN(EDI_PARSER, bufIter);
						if((strncmp(bufIter, "IEA", 3) == 0)){
							X12_PARSER->previous = tok.type;
							return (void *)X12_ProcessIEA;
						}
						break;
					}
				}
				break;
			default: ;
	    }
	    X12_PARSER->previous = tok.type;
	    X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	}
	if(tag){
    	if(X12_PARSER->savedTag){
    		FREE(EDI_PARSER, X12_PARSER->savedTag);
    	}
    	X12_PARSER->savedTag = strdup(tag);
		X12_PARSER->savedElementPosition = element;
		X12_PARSER->savedComponentPosition = component;
	}
	EDI_SetResumeState(EDI_PARSER->machine, (void *)X12_ProcessMessage);
	EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
	return EDI_PARSER->error;
}
/******************************************************************************/
EDI_StateHandler X12_ProcessIEA(EDI_Parser parser)
{
	char           *bufIter = NULL;
	struct token    tok;
	
	X12_PARSER->delimiters[COMPONENT] = '\0';
	X12_PARSER->delimiters[REPEAT]    = '\0';
	
	bufIter = EDI_PARSER->bufReadPtr;
	X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	while(tok.type != UNKNOWN){
		EDI_PARSER->bufReadPtr = bufIter;
		if(tok.type == ELEMENT){
			if(X12_PARSER->previous == SEGMENT){
				EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "IEA");
			} else {
				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
			}
		} else if(tok.type == SEGMENT){	
			EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
			EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, "IEA");
			break;
		}
	    X12_PARSER->previous = tok.type;
		X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	}
	while(bufIter < EDI_PARSER->bufEndPtr){
		if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
			bufIter -= 4;
			if(!(isalnum(*bufIter))){
				bufIter++;
			}
			EDI_GAP_SCAN(EDI_PARSER, bufIter);
			if((strncmp(bufIter, "ISA", 3) == 0)){
				X12_PARSER->delimiters[ELEMENT] = '\0';
				X12_PARSER->delimiters[SEGMENT] = '\0';
				X12_PARSER->version             = 0;
				X12_PARSER->release             = 0;
				return (void *)X12_ProcessISA;
			}
			break;
		}
	}
	if(tok.type == UNKNOWN){
    	EDI_SetResumeState(EDI_PARSER->machine, (void *)X12_ProcessIEA);
		EDI_PARSER->errorCode = EDI_ERROR_BUFFER_END;
		return EDI_PARSER->error;
	} else {
		return EDI_PARSER->seekHeader;
	}
}
/******************************************************************************/
X12_Parser X12_ParserCreate(EDI_Parser parent)
{
	X12_Parser new = NULL;
	new = MALLOC(parent, sizeof(struct X12_ParserStruct));
	if(new){
		new->delimiters[ELEMENT]    = '\0';
		new->delimiters[COMPONENT]  = '\0';
		new->delimiters[REPEAT]     = '\0';
		new->delimiters[SEGMENT]    = '\0';
		new->version                = 0;
		new->release                = 0;
		new->savedTag               = NULL;
		new->savedElementPosition   = 0;
		new->savedComponentPosition = 0;
		new->error                  = X12_ERROR_NONE;
		new->previous               = SEGMENT;
		parent->process             = (void *)X12_ProcessISA;
		parent->freeChild           = (void *)X12_ParserDestroy;
		EDI_AddState(parent->machine, (void *)X12_ProcessISA, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessMessage, 0);
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
	EDI_RemoveState(EDI_PARSER->machine, (void *)X12_ProcessIEA, 0);
	if(X12_PARSER->savedTag){
		FREE(EDI_PARSER, X12_PARSER->savedTag);
	}
	FREE(parser, X12_PARSER);
	EDI_PARSER->child = NULL;
}
