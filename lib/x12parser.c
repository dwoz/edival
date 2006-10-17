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
static EDI_Schema loadStandards(EDI_Schema);
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
		length             \
	);                    \
}
/******************************************************************************/
void handleX12SegmentError(void *myData, const char *tag, enum EDI_SegmentValidationError err)
{
	fprintf(stderr, "*~*~* Segment error %d on %s\n", err, tag);
}
/******************************************************************************/
void handleX12ElementError(void *myData, int element, int component, enum EDI_ElementValidationError err)
{
	fprintf(stderr, "\nElement error %d on element %2.2d-%d\n", err, element, component);
}
/******************************************************************************/
EDI_StateHandler X12_ProcessISA(EDI_Parser parser)
{
	const unsigned char ISA_seps[16] = {  3,  6, 17, 20, 31, 34,  50,  53,
                                        69, 76, 81, 83, 89, 99, 101, 103 };
	unsigned int      i = 0,  j = 0;
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
	X12_PARSER->segmentError = EDI_ValidateSegmentPosition(X12_PARSER->x12Schema, "ISA");
	if(X12_PARSER->segmentError){
		fprintf(stderr, "SEG_ERR: %d ***", X12_PARSER->segmentError);
	}
	EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "ISA");
	i = j = 0;
	while(tok.type != SEGMENT){
		X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
		i++;
		switch(tok.type){
			case ELEMENT:
			{
			  	if(!EDI_ValidateElement(X12_PARSER->x12Schema, i, &j, tok.token, strlen(tok.token))
			  		&& i == 12){
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
				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
				break;
			}
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
	char                      *IEA_tag   = "IEA";

	bufIter = EDI_PARSER->bufReadPtr;
	X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	if(X12_PARSER->previous == SEGMENT){
		if(tok.type == ELEMENT){
			while(!(isalnum(tok.token[0]))){
				tok.token++;
			}
			EDI_GAP_SCAN(EDI_PARSER, tok.token);
		 	if(string_eq(tok.token, IEA_tag)){
				EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, IEA_tag);
				X12_PARSER->previous = ELEMENT;
				EDI_PARSER->bufReadPtr = bufIter;
				return (void *)X12_ProcessIEA;
			}
		}
	} else {
		tag = X12_PARSER->savedTag;
		element = X12_PARSER->savedElementPosition;
		component = X12_PARSER->savedComponentPosition;
	}
	while(tok.type != UNKNOWN){
		EDI_PARSER->bufReadPtr = bufIter;
		switch(tok.type){
			case REPEAT:
				if(EDI_PARSER->repeatHandler){
					EDI_PARSER->repeatHandler(EDI_PARSER->userData);
				} else {
					fprintf(stderr, "FATAL (edival): A repeated element was found in the data stream but no \n\
					callback function has been registered for the event using EDI_SetRepeatHandler()\n");
					exit(70);
				}
			case ELEMENT:
	        	switch(X12_PARSER->previous){
	        		case COMPONENT:
	        			component++;
	        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	        			EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
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
	        				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
			  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
								EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
			  				}
	        				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	        			} else {
	        				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
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
     			EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
				break;
			case SEGMENT:
	        	if(X12_PARSER->previous == COMPONENT){
	        		component++;
        			ELEMENT_VALIDATE(EDI_PARSER, X12_PARSER, element, &component, tok.token, tok.length);
	        		EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
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
	     				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
		  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
							EDI_ValidateSyntax(EDI_PARSER->schema, element, component);
		  				}
	     				EDI_PARSER->compositeEndHandler(EDI_PARSER->userData);
	     			} else {
	     				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
	     			}
	        	}
  				if((EDI_PARSER->validate ^ X12_PARSER->segmentError) == EDI_TRUE){
					EDI_ValidateSyntax(EDI_PARSER->schema, 0, element);
  				}
				EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, tag);
				tag = NULL;
				element = 0;
    			component = 0;
				while(bufIter < EDI_PARSER->bufEndPtr){
					if(*bufIter++ == X12_PARSER->delimiters[ELEMENT]){
						bufIter -= 4;
						while(!(isalnum(*bufIter))){
							bufIter++;
						}
						EDI_GAP_SCAN(EDI_PARSER, bufIter);
						if(bufIter[0] == 'I' && bufIter[1] == 'E' && bufIter[2] == 'A'){
							X12_PARSER->previous = tok.type;
							return (void *)X12_ProcessIEA;
						}
						break;
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
	char *bufIter           = EDI_PARSER->bufReadPtr;
	long long int size      = EDI_PARSER->binaryElementSize;
	long long int finished  = EDI_PARSER->bytesHandled;
	enum X12_Delimiter type = UNKNOWN;
	
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
							exit(70);
	        			} else {
	        				EDI_PARSER->binBufferHandler(EDI_PARSER->userData, EDI_PARSER->binBuffer, EDI_PARSER->binaryElementSize);
	        			}
	        			break;
					default: 
						fprintf(stderr, "FATAL (edival): Invalid Binary data element type!  Terminating process.");
						exit(70);
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
						exit(70);
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
				exit(70);
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
	int             dummy   = 0;
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
				X12_PARSER->segmentError = EDI_ValidateSegmentPosition(X12_PARSER->x12Schema, "IEA");
				EDI_PARSER->segmentStartHandler(EDI_PARSER->userData, "IEA");
			} else {
				EDI_ValidateElement(X12_PARSER->x12Schema, 1, &dummy, tok.token, strlen(tok.token));
				EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
			}
		} else if(tok.type == SEGMENT){	
			EDI_ValidateElement(X12_PARSER->x12Schema, 2, &dummy, tok.token, strlen(tok.token));
			EDI_PARSER->elementHandler(EDI_PARSER->userData, tok.token);
			EDI_PARSER->segmentEndHandler(EDI_PARSER->userData, "IEA");
			break;
		}
		X12_PARSER->previous = tok.type;
		X12_NEXT_TOKEN(bufIter, X12_PARSER->delimiters, tok);
	}
	while(bufIter < EDI_PARSER->bufEndPtr){
		if(!(*bufIter++ ^ X12_PARSER->delimiters[ELEMENT])){
			bufIter -= 4;
			if(!(isalnum(*bufIter))){
				bufIter++;
			}
			EDI_GAP_SCAN(EDI_PARSER, bufIter);
			if((strncmp(bufIter, "ISA", 3) == 0)){
				EDI_PARSER->bufReadPtr = bufIter;
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
		new->x12Schema              = loadStandards(EDI_SchemaCreate_MM(parent->memsuite));
		new->error                  = X12_ERROR_NONE;
		new->previous               = SEGMENT;
		parent->process             = (void *)X12_ProcessISA;
		parent->freeChild           = (void *)X12_ParserDestroy;
		EDI_AddState(parent->machine, (void *)X12_ProcessISA, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessMessage, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessBinaryElement, 0);
		EDI_AddState(parent->machine, (void *)X12_ProcessIEA, 0);
		new->parent = parent;
		new->x12Schema->parser = parent;
	}
	return new;
}
/******************************************************************************/
static EDI_Schema loadStandards(EDI_Schema s)
{
	EDI_SchemaNode hold;

	if(s){
		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I01", 2, 2);
		EDI_AddElementValue(hold, "00");
		EDI_AddElementValue(hold, "01");
		EDI_AddElementValue(hold, "02");
		EDI_AddElementValue(hold, "03");
		EDI_AddElementValue(hold, "04");
		EDI_AddElementValue(hold, "05");
		EDI_AddElementValue(hold, "06");
		
		EDI_CreateElementType(s, EDI_DATA_STRING, "I02", 10, 10);

		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I03", 2, 2);
		EDI_AddElementValue(hold, "00");
		EDI_AddElementValue(hold, "01");

		EDI_CreateElementType(s, EDI_DATA_STRING, "I04", 10, 10);

		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I05", 2, 2);
		EDI_AddElementValue(hold, "01");
		EDI_AddElementValue(hold, "02");
		EDI_AddElementValue(hold, "03");
		EDI_AddElementValue(hold, "04");
		EDI_AddElementValue(hold, "07");
		EDI_AddElementValue(hold, "08");
		EDI_AddElementValue(hold, "09");
		EDI_AddElementValue(hold, "10");
		EDI_AddElementValue(hold, "11");
		EDI_AddElementValue(hold, "12");
		EDI_AddElementValue(hold, "13");
		EDI_AddElementValue(hold, "14");
		EDI_AddElementValue(hold, "15");
		EDI_AddElementValue(hold, "16");
		EDI_AddElementValue(hold, "17");
		EDI_AddElementValue(hold, "18");
		EDI_AddElementValue(hold, "19");
		EDI_AddElementValue(hold, "20");
		EDI_AddElementValue(hold, "21");
		EDI_AddElementValue(hold, "22");
		EDI_AddElementValue(hold, "23");
		EDI_AddElementValue(hold, "24");
		EDI_AddElementValue(hold, "25");
		EDI_AddElementValue(hold, "26");
		EDI_AddElementValue(hold, "27");
		EDI_AddElementValue(hold, "28");
		EDI_AddElementValue(hold, "29");
		EDI_AddElementValue(hold, "30");
		EDI_AddElementValue(hold, "31");
		EDI_AddElementValue(hold, "32");
		EDI_AddElementValue(hold, "33");
		EDI_AddElementValue(hold, "34");
		EDI_AddElementValue(hold, "35");
		EDI_AddElementValue(hold, "36");
		EDI_AddElementValue(hold, "37");
		EDI_AddElementValue(hold, "38");
		EDI_AddElementValue(hold, "AM");
		EDI_AddElementValue(hold, "NR");
		EDI_AddElementValue(hold, "SA");
		EDI_AddElementValue(hold, "SN");
		EDI_AddElementValue(hold, "ZZ");

		EDI_CreateElementType(s, EDI_DATA_STRING, "I06", 15, 15);
		EDI_CreateElementType(s, EDI_DATA_STRING, "I07", 15, 15);
		EDI_CreateElementType(s, EDI_DATA_DATE, "I08", 6, 6);
		EDI_CreateElementType(s, EDI_DATA_TIME, "I09", 4, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "I65", 1, 1);

		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I11", 5, 5);
		EDI_AddElementValue(hold, "00200");
		EDI_AddElementValue(hold, "00201");
		EDI_AddElementValue(hold, "00204");
		EDI_AddElementValue(hold, "00300");
		EDI_AddElementValue(hold, "00301");
		EDI_AddElementValue(hold, "00302");
		EDI_AddElementValue(hold, "00303");
		EDI_AddElementValue(hold, "00304");
		EDI_AddElementValue(hold, "00305");
		EDI_AddElementValue(hold, "00306");
		EDI_AddElementValue(hold, "00307");
		EDI_AddElementValue(hold, "00400");
		EDI_AddElementValue(hold, "00401");
		EDI_AddElementValue(hold, "00402");
		EDI_AddElementValue(hold, "00403");
		EDI_AddElementValue(hold, "00404");
		EDI_AddElementValue(hold, "00405");
		EDI_AddElementValue(hold, "00406");
		EDI_AddElementValue(hold, "00500");
		EDI_AddElementValue(hold, "00501");
		EDI_AddElementValue(hold, "00502");
		EDI_AddElementValue(hold, "00503");

		EDI_CreateElementType(s, EDI_DATA_INTEGER, "I12", 9, 9);

		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I13", 1, 1);
		EDI_AddElementValue(hold, "0");
		EDI_AddElementValue(hold, "1");

		hold = EDI_CreateElementType(s, EDI_DATA_STRING, "I14", 1, 1);
		EDI_AddElementValue(hold, "I");
		EDI_AddElementValue(hold, "P");
		EDI_AddElementValue(hold, "T");

		EDI_CreateElementType(s, EDI_DATA_STRING, "I15", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "I16", 1, 5);
	
		hold = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "ISA");
		EDI_AppendType(hold, EDI_GetElementByID(s, "I01"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I02"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I03"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I04"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I05"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I06"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I05"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I07"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I08"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I09"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I65"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I11"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I12"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I13"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I14"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I15"), 1, 1);
		EDI_StoreComplexNode(s, hold);
		
		hold = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "IEA");
		EDI_AppendType(hold, EDI_GetElementByID(s, "I16"), 1, 1);
		EDI_AppendType(hold, EDI_GetElementByID(s, "I12"), 1, 1);
		EDI_StoreComplexNode(s, hold);

		hold = EDI_CreateComplexType(s, EDITYPE_TRANSACTION, "ANSI X12 Interchange");
		EDI_AppendType(hold, EDI_GetComplexNodeByID(s, "ISA"), 1, 1);
		EDI_AppendType(hold, EDI_GetComplexNodeByID(s, "IEA"), 1, 1);
		EDI_StoreComplexNode(s, hold);
		
		EDI_SetSegmentErrorHandler(s, &handleX12SegmentError);
		EDI_SetElementErrorHandler(s, &handleX12ElementError);

	}
	return s;
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
	EDI_SchemaFree(X12_PARSER->x12Schema);
	FREE(parser, X12_PARSER);
	EDI_PARSER->child = NULL;
}
