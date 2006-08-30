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

#ifndef EDIVAL_INCLUDED
#define EDIVAL_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EDI_MAJOR_VERSION 0
#define EDI_MINOR_VERSION 1
#define EDI_MICRO_VERSION 2

/******************************************************************************/
/********** PARSING/TOKENIZATION API; SEE BELOW FOR VALIDATION API ************/
/******************************************************************************/

struct EDI_ParserStruct;
typedef struct EDI_ParserStruct *EDI_Parser;

typedef unsigned char EDI_Bool;
#define EDI_TRUE   ((EDI_Bool) 1)
#define EDI_FALSE  ((EDI_Bool) 0)

enum EDI_Error {
	EDI_ERROR_NONE              = 0,
	EDI_ERROR_NO_MEM            = 1,
	EDI_ERROR_UNKNOWN           = 2,
	EDI_ERROR_INVALID_STATE     = 3,
	EDI_ERROR_SUSPENDED         = 4,
	EDI_ERROR_ABORTED           = 5,
	EDI_ERROR_BUFFER_END        = 6,
	EDI_ERROR_FINISHED          = 7
};

enum EDI_Status {
    EDI_STATUS_OK               = 0,
    EDI_STATUS_SUSPENDED        = 1,
    EDI_STATUS_ERROR            = 2
};

enum EDI_Parsing {
    EDI_INITIALIZED,
    EDI_PARSING,
    EDI_FINISHED,
    EDI_SUSPENDED
};

typedef struct {
    enum EDI_Parsing parsing;
    EDI_Bool finalBuffer;
} EDI_ParsingStatus;

typedef struct {
  void  *(*malloc_fcn) (size_t size);
  void  *(*realloc_fcn)(void *ptr, size_t size);
  void   (*free_fcn)   (void *ptr);
} EDI_Memory_Handling_Suite;


/*******************************************************************************
    Constructs a new parser.
*******************************************************************************/
EDI_Parser EDI_ParserCreate(void);


/*******************************************************************************
    Constructs a new parser using the memory management suite referred to
    by memsuite. If memsuite is NULL, then use the standard library memory
    suite. 
   
    All further memory operations used for the created parser will come from
    the given suite.
*******************************************************************************/
EDI_Parser EDI_ParserCreate_MM(EDI_Memory_Handling_Suite *memsuite);


/*******************************************************************************
    Sets the userdata structure within the parser.  This layout and usage of
    this structure is to be determined by the calling program.
*******************************************************************************/
void EDI_SetUserData(EDI_Parser parser, void *p);


/*******************************************************************************
    This is called when a segment start tag is found in the data stream.  The
    arguments are the userData structure and a tag indicating the segment.
*******************************************************************************/
typedef void (*EDI_SegmentStartHandler)(void *, const char *, int);

void EDI_SetSegmentStartHandler(EDI_Parser, EDI_SegmentStartHandler);


/*******************************************************************************
    This is called when a segment terminator is found in the data stream. The
    arguments are the userData structure and a tag indicating the segment.
*******************************************************************************/
typedef void (*EDI_SegmentEndHandler)(void *, const char *);

void EDI_SetSegmentEndHandler(EDI_Parser, EDI_SegmentEndHandler);


/*******************************************************************************
    This is called when an element is found in the data stream.
    N.B. - this function will also be called for zero length elements, i.e.,
    dat == '\0'
*******************************************************************************/
typedef void (*EDI_ElementHandler)(void *, const char *, int);

void EDI_SetElementHandler(EDI_Parser, EDI_ElementHandler);


/*******************************************************************************
    This is called when a component of a composite element is found in the data 
    stream.
    N.B. - this function will also be called for zero length components, i.e.,
    dat == '\0'.  If a segment contains composite element for which there is no
    data, the EDI_ElementHandler callback will be called in instead of using
    this callback multiple times.
*******************************************************************************/
typedef void (*EDI_ComponentHandler)(void *, const char *, int, int); 

void EDI_SetComponentHandler(EDI_Parser, EDI_ComponentHandler);


/*******************************************************************************
    This is called when an element is repeated with the repetition character to
    indicate that the following element is a repetition of the previous.  No
    parsed data is passed with this function.
*******************************************************************************/
typedef void (*EDI_RepeatHandler)(void *);

void EDI_SetRepeatHandler(EDI_Parser, EDI_RepeatHandler);


/*******************************************************************************
    The Non EDI Handler will be called whenever the parser encounters data
    in the stream that is not valid.  E.g., "\r\n" in addition to segment
    terminators, junk data before start of interchange or after end of
    interchange, etc.
*******************************************************************************/
typedef void (*EDI_NonEDIDataHandler)(void *, const char *);

void EDI_SetNonEDIDataHandler(EDI_Parser, EDI_NonEDIDataHandler);


EDI_Bool            EDI_ParserReset(EDI_Parser);
void               *EDI_GetBuffer(EDI_Parser, int);
enum EDI_Status     EDI_Parse(EDI_Parser, const char *, int);
enum EDI_Status     EDI_ParseBuffer(EDI_Parser, int);

/*******************************************************************************
    These functions are not yet implemented.
*******************************************************************************/
enum EDI_Status     EDI_StopParser(EDI_Parser, EDI_Bool);
enum EDI_Status     EDI_ResumeParser(EDI_Parser);
void                EDI_GetParsingStatus(EDI_Parser, EDI_ParsingStatus *);

/*******************************************************************************
    Get the currently set error code from the parser object.
*******************************************************************************/
enum EDI_Error      EDI_GetErrorCode(EDI_Parser);

/*******************************************************************************
    Use the currently set memory handling functions.
*******************************************************************************/
void               *EDI_MemMalloc(EDI_Parser, size_t);
void               *EDI_MemRealloc(EDI_Parser, void *, size_t);
void                EDI_MemFree(EDI_Parser, void *);
void                EDI_ParserFree(EDI_Parser);

/*******************************************************************************
        ********************** VALIDATION API **********************
*******************************************************************************/

struct EDI_SchemaStruct;
typedef struct EDI_SchemaStruct *EDI_Schema;

struct EDI_SchemaNodeStruct;
typedef struct EDI_SchemaNodeStruct *EDI_SchemaNode;

/*******************************************************************************
 *  Enumeration of validation types which can be added to the validation schema.
 ******************************************************************************/
enum EDI_NodeType {
    EDITYPE_TRANSACTION = 1,
    EDITYPE_LOOP        = 2,
    EDITYPE_SEGMENT     = 3,
    EDITYPE_COMPOSITE   = 4,
    EDITYPE_ELEMENT     = 5
};

/*******************************************************************************
 *  Enumeration of all supported based datatypes.  Binary data allows any data
 *  stream to validate, assuming size constraints are met (X12 BIN02 element).
 ******************************************************************************/
enum EDI_PrimitiveDataType {
    EDI_DATA_STRING      = 1, /* Alphanumeric string & predefined identifiers */
    EDI_DATA_INTEGER     = 2,
    EDI_DATA_DECIMAL     = 3,
    EDI_DATA_DATE        = 4, /* CCYYMMDD */
    EDI_DATA_TIME        = 5, /* HHMMSSdd */
    EDI_DATA_BINARY      = 6  /* Binary data */
};

/*******************************************************************************
 * Enumeration of errors that can signal an invalid string for a given element
 * in the data stream.
 ******************************************************************************/
enum EDI_ElementValidationError {
    VAL_VALID_ELEMENT       = 0,
    VAL_RANGE_HIGH          = 1,
    VAL_RANGE_LOW           = 2,
    VAL_CHAR_ERROR          = 3,
    VAL_CODE_ERROR          = 4,
    VAL_DATE_ERROR          = 5,
    VAL_TIME_ERROR          = 6,
    VAL_UNKNOWN_ELEMENT     = 7,
    VAL_MANDATORY_ELEMENT   = 8,
    VAL_TOO_MANY_ELEMENTS   = 9,
    VAL_MISSING_CONDITIONAL = 10,
    VAL_EXCLUSION_VIOLATED  = 11,
    VAL_TOO_MANY_COMPONENTS = 12,
    VAL_REPETITION_EXCEEDED = 13,
    VAL_INVALID_SEGMENT     = 14
};

/*******************************************************************************
 * Enumeration of errors that can signal an invalid occurance for a segment
 * in the data stream.
 ******************************************************************************/
enum EDI_SegmentValidationError {
    SEGERR_NONE          = 0,
    SEGERR_UNEXPECTED    = 1,
    SEGERR_MANDATORY     = 2,
    SEGERR_LOOP_EXCEEDED = 3,
    SEGERR_EXCEED_REPEAT = 4,
    SEGERR_UNDEFINED     = 5,
    SEGERR_SEQUENCE      = 6,
    SEGERR_UNKNOWN       = 7,
    SEGERR_LOOP_SEEK     = 8
};

/*******************************************************************************
 * Enumeration of element syntax restrictions that can be enforced
 * These are taken from the X12 standards.  Other syntax types could be added
 * as required.
 ******************************************************************************/
enum EDI_SyntaxType {
    EDI_SYNTAX_PAIRED    = 1,
    EDI_SYNTAX_REQUIRED  = 2,
    EDI_SYNTAX_EXCLUSION = 3,
    EDI_SYNTAX_CONDITION = 4,
    EDI_SYNTAX_LIST      = 5
};

/*******************************************************************************
    Constructs a new schema for validation.
    Returns NULL if failure.
*******************************************************************************/
EDI_Schema EDI_SchemaCreate(void);

/*******************************************************************************
    Constructs a new schema using the memory management suite referred to
    by memsuite. If memsuite is NULL, then use the standard library memory
    suite. 
   
    All further memory operations used for the created schema will come from
    the given suite.
*******************************************************************************/
EDI_Schema EDI_SchemaCreate_MM(EDI_Memory_Handling_Suite *memsuite);


void EDI_SchemaFree(EDI_Schema);

/*******************************************************************************
    Get or Set the schema object currently being used by the parser.
*******************************************************************************/
EDI_Schema EDI_GetSchema(EDI_Parser);
void       EDI_SetSchema(EDI_Parser, EDI_Schema);

/*******************************************************************************
    Remove the schema object currently being used by the parser.  This will
    stop future validation of the data parsed until a new schema is set on the
    parser.
*******************************************************************************/
EDI_Schema EDI_RemoveSchema(EDI_Parser);

/*******************************************************************************
    Get or set the identifier on the schema object.
*******************************************************************************/
char *EDI_GetSchemaId(EDI_Schema);
void  EDI_SetSchemaId(EDI_Schema, char*);

/*******************************************************************************
 * Returns a new element type Schema node.  NULL if unsuccessful.  The element
 * type is also added to the message schema.  When building a numeric type,
 * the min/max arguments are also numeric.  When building a non-numeric type,
 * the min/max arguments refer to the length in bytes of the element.  Binary
 * elements are parsed as qualified by their preceeding numeric element.
 ******************************************************************************/
EDI_SchemaNode 
EDI_CreateElementType(EDI_Schema                ,
                      enum EDI_PrimitiveDataType,  /* Data type               */
                      const char               *,  /* Element type identifier */
                      unsigned int              ,  /* Minimum length    */
                      unsigned int              ); /* Maximum length    */

/*******************************************************************************
 * Retrieves a pointer to the element node identified by the second argument.
 ******************************************************************************/
EDI_SchemaNode EDI_GetElementByID(EDI_Schema  ,
                                  const char *);

/*******************************************************************************
 * Adds an explicit value to an element's value table.  If values are added to 
 * this table, no other values for that element will validate.  This function
 * only applies to elements of type EDI_DATA_STRING that represent ID values.
 ******************************************************************************/
enum EDI_ElementValidationError 
EDI_AddElementValue(EDI_Schema      ,
                    const char     *,  /* Element type identifier */
                    const char     *); /* Explicit value   */

/*******************************************************************************
 * Returns a new composite/segment/loop/message type Schema node (complex types)
 * Requesting a type of EDITYPE_TRANSACTION will cause the new node to be set
 * as the schema's root node.
 ******************************************************************************/
EDI_SchemaNode 
EDI_CreateComplexType(EDI_Schema        ,
                      enum EDI_NodeType ,  /* Node type               */
                      const char       *); /* Complex type identifier */


/*******************************************************************************
 * Saves the complex node identified by the second argument inside the schema.
 ******************************************************************************/
void EDI_StoreComplexNode(EDI_Schema     ,
                          EDI_SchemaNode);

/*******************************************************************************
 * Retrieves a pointer to the complex node identified by the second argument.
 ******************************************************************************/
EDI_SchemaNode EDI_GetComplexNodeByID(EDI_Schema  ,
                                      const char *);

/*******************************************************************************
 *  Add a syntax restriction to a segment or composite for the elements it
 *  contains.  
 ******************************************************************************/
void
EDI_AddSyntaxNote(EDI_Schema         ,
                  EDI_SchemaNode     ,  /* Segment/Composite with restriction */
                  enum EDI_SyntaxType,  /* Type of syntax restriction         */
                  unsigned int       ,  /* Number of elements included in note*/
                  unsigned int *     ); /* Integer list of element positions  */

/*******************************************************************************
 *  Appends any of the following:
 *    1.  Element to a segment or composite.
 *    2.  Segment to a loop or transaction type.
 *    3.  Loop to a parent loop or parent transaction type.
 *    4.  Composite to a segment.
 *  Arguments:
 *    1.  Child schema node to append
 *    2.  Parent schema node being appended to
 *  Returns:
 *    Schema node which was appended or NULL if failed.
 ******************************************************************************/
EDI_SchemaNode
EDI_AppendType   (EDI_Schema    ,
                  EDI_SchemaNode,  /* Parent Node  */
                  EDI_SchemaNode,  /* Child Node */
                  unsigned int  ,  /* Minimum occurances of the child */
                  unsigned int  ); /* Maximum occurances of the child */

/*******************************************************************************
 The SegmentErrorHandler function will be called at any point that a segment
 validation error has occurred.  This will generally happen any time an unknown
 segment is found, any time a segment is out of order, or if a particular
 segment has been repeated beyond the specified maximum allowed occurrences.
*******************************************************************************/
typedef void (*EDI_SegmentErrorHandler)(void *, enum EDI_SegmentValidationError);

void EDI_SetSegmentErrorHandler(EDI_Schema, EDI_SegmentErrorHandler);

/*******************************************************************************
 The ElementErrorHandler function will be called at any point that an element
 error has been detected in the data stream and validation is activated.  Some
 possible errors include invalid string in a numeric element, invalid length of
 a data element, etc.  See "enum EDI_ElementValidationError" for more details.
*******************************************************************************/
typedef void (*EDI_ElementErrorHandler)(void *, int, int, enum EDI_ElementValidationError);

void EDI_SetElementErrorHandler(EDI_Schema, EDI_ElementErrorHandler);



#ifdef __cplusplus
}
#endif

#endif /* EDIVAL_INCLUDED */
