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

#include <edival.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 1024

int group_start = 0;
int counter = 0;

void load_standard(EDI_Parser p)
{
	EDI_Schema s = NULL;
	EDI_SchemaNode parent = NULL;
	EDI_SchemaNode child  = NULL;
	
	s = EDI_GetSchema(p);
	if(s && strcmp(EDI_GetSchemaId(s), "Functional Acknowledgement: FA/997") != 0){
		s = EDI_RemoveSchema(p);
		EDI_SchemaFree(s);
	}
	if(!s){
		s = EDI_SchemaCreate();
		EDI_SetSchemaId(s, "Functional Acknowledgement: FA/997");
		EDI_SetSchema(p, s);
	} 
	/* 997 Elements */
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "2", 0, 999999);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "28", 0, 999999999);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "96", 0, 999999999);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "97", 0, 999999);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "123", 0, 999999);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "143", 3, 3);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "ST01_143", 3, 3);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "329", 4, 9);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "447", 1, 6);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "479", 2, 2);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "715", 1, 1);
	EDI_AddElementValue(s, "715", "E");
	EDI_AddElementValue(s, "715", "A");
	EDI_AddElementValue(s, "715", "R");
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "716", 1, 3);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "717", 1, 1);
	EDI_AddElementValue(s, "717", "E");
	EDI_AddElementValue(s, "717", "A");
	EDI_AddElementValue(s, "717", "R");
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "718", 1, 3);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "719", 0, 999999);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "720", 1, 3);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "721", 2, 3);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "722", 0, 99);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "723", 1, 3);
	child = EDI_CreateElementType(s, EDI_DATA_STRING, "724", 1, 99);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "725", 0, 9999);
	child = EDI_CreateElementType(s, EDI_DATA_INTEGER, "1528", 0, 99);
	//fprintf(stderr, "Elements built\n");

	/* 997 Composites */
	parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C030");
	child = EDI_GetElementByID(s, "722");
	fprintf(stderr, "Append C030-1: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "1528");
	fprintf(stderr, "Append C030-2: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 ST Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "ST");
	fprintf(stderr, "Add ST01 value: %d\n", EDI_AddElementValue(s, "ST01_143", "997"));
	child = EDI_GetElementByID(s, "ST01_143");
	fprintf(stderr, "Append ST01: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "329");
	fprintf(stderr, "Append ST02: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	EDI_StoreComplexNode(s, parent);
	
	/* 997 AK1 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK1");
	child = EDI_GetElementByID(s, "479");
	fprintf(stderr, "Append AK101: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "28");
	fprintf(stderr, "Append AK102: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	EDI_StoreComplexNode(s, parent);
	
	/* 997 AK2 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK2");
	child = EDI_GetElementByID(s, "143");
	fprintf(stderr, "Append AK201: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "329");
	fprintf(stderr, "Append AK202: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	EDI_StoreComplexNode(s, parent);
	
	/* 997 AK3 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK3");
	child = EDI_GetElementByID(s, "721");
	fprintf(stderr, "Append AK301: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "719");
	fprintf(stderr, "Append AK302: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "447");
	fprintf(stderr, "Append AK303: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	child = EDI_GetElementByID(s, "720");
	fprintf(stderr, "Append AK304: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 AK4 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK4");
	child = EDI_GetComplexNodeByID(s, "C030");
	fprintf(stderr, "Append AK401: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "725");
	fprintf(stderr, "Append AK402: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	child = EDI_GetElementByID(s, "723");
	fprintf(stderr, "Append AK403: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "724");
	fprintf(stderr, "Append AK404: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 AK5 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK5");
	child = EDI_GetElementByID(s, "717");
	fprintf(stderr, "Append AK501: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "718");
	fprintf(stderr, "Append AK502: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK503: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK504: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK505: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK506: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 AK9 Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK9");
	child = EDI_GetElementByID(s, "715");
	fprintf(stderr, "Append AK901: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "97");
	fprintf(stderr, "Append AK902: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "123");
	fprintf(stderr, "Append AK903: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "2");
	fprintf(stderr, "Append AK904: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "716");
	fprintf(stderr, "Append AK905: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK906: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK907: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK908: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	fprintf(stderr, "Append AK909: %p\n", EDI_AppendType(s, parent, child, 0, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 SE Segment */
	parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SE");
	child = EDI_GetElementByID(s, "96");
	fprintf(stderr, "Append SE01: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetElementByID(s, "329");
	fprintf(stderr, "Append SE02: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 AK2/AK3 Loop */
	parent = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop-AK2/AK3");
	child = EDI_GetComplexNodeByID(s, "AK3");
	fprintf(stderr, "Append AK3 to AK2/AK3 Loop: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetComplexNodeByID(s, "AK4");
	fprintf(stderr, "Append AK4 to AK2/AK3 Loop: %p\n", EDI_AppendType(s, parent, child, 0, 99));
	EDI_StoreComplexNode(s, parent);

	/* 997 AK2 Loop */
	parent = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop-AK2");
	child = EDI_GetComplexNodeByID(s, "AK2");
	fprintf(stderr, "Append AK2 to AK2 Loop: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetComplexNodeByID(s, "Loop-AK2/AK3");
	fprintf(stderr, "Append Loop-AK2/AK3 to AK2 Loop: %p\n", EDI_AppendType(s, parent, child, 0, 999999));
	EDI_StoreComplexNode(s, parent);
	child = EDI_GetComplexNodeByID(s, "AK5");
	fprintf(stderr, "Append AK5 to AK2 Loop: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	EDI_StoreComplexNode(s, parent);

	/* 997 Transaction */
	parent = EDI_CreateComplexType(s, EDITYPE_TRANSACTION, "TS997");
	child = EDI_GetComplexNodeByID(s, "ST");
	fprintf(stderr, "Append ST to 997: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetComplexNodeByID(s, "AK1");
	fprintf(stderr, "Append AK1 to 997: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetComplexNodeByID(s, "Loop-AK2");
	fprintf(stderr, "Append Loop-AK2 to 997 Loop: %p\n", EDI_AppendType(s, parent, child, 0, 999999));
	EDI_StoreComplexNode(s, parent);
	child = EDI_GetComplexNodeByID(s, "AK9");
	fprintf(stderr, "Append AK9 to 997: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	child = EDI_GetComplexNodeByID(s, "SE");
	fprintf(stderr, "Append SE to 997: %p\n", EDI_AppendType(s, parent, child, 1, 1));
	return;
}

void handleSegmentStart(void *myData, const char *tag, int offset)
{
	fprintf(stderr, "Seg: %3s ->", tag);
	if(strcmp("GS", tag) == 0){
		group_start = 1;
		counter = 0;
	}
	return;
}

void handleSegmentEnd(void *myData, const char *tag)
{
    fprintf(stderr, " <- End: %s\n", tag);
    return;
}

void handleElement(void *myData, const char *val, int position)
{
	fprintf(stderr, "[%s]", val);
	if(group_start){
		counter++;
		if(counter == 1){
			if(strcmp("FA", val) == 0){
				load_standard((EDI_Parser)myData);
			} else {
				fprintf(stderr, "\n*** This program only supports the validation of Functional Acknowledgements (GS02 = 'FA'/ST01 = '997')\n");
				fprintf(stderr, "*** No transaction-level validation will be performed on this file.\n");
				//exit(1);
			}
		} else {
			group_start = 0;
		}
	}
	return;
}

void handleComponent(void *myData, const char *val, int elementPosition, int position)
{
    fprintf(stderr, "{%s}", val);
    return;
}

void handleJunk(void *myData, const char *val)
{
    //fprintf(stderr, "Junk: >>{%s}<<\n", val);
    return;

}

int main(int argc, char **argv)
{
	int bytes_read, file_stat_size;
	double file_size = 0;
	double speed = 0;
	int input, i, length;
	void *buff = NULL;
	unsigned long j = 0;
	unsigned int size;
	struct timezone tz;
	struct timeval tv;
	double start_time, end_time, run_time;
	struct stat statbuf;
	EDI_Parser p = NULL;
	enum EDI_Status status;
	FILE *fin;

	if(argc < 2){
		fprintf(stderr, "Not enough arguments, exiting.\n");
		exit(1);
	}
	
	gettimeofday(&tv, &tz);
	start_time = tv.tv_sec + (tv.tv_usec * 0.000001);

	if((input = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "Couldn't open\n");
		exit(1);
	}
	if (fstat (input, &statbuf) < 0){
		fprintf(stderr, "Couldn't stat\n");
		exit(1);
	}
        
	p = EDI_ParserCreate();
	fprintf(stderr, "Parser built\n");
	
	EDI_SetSegmentStartHandler(p, handleSegmentStart);
	EDI_SetSegmentEndHandler(p, handleSegmentEnd);
	EDI_SetElementHandler(p, handleElement);
	EDI_SetComponentHandler(p, handleComponent);
	EDI_SetNonEDIDataHandler(p, handleJunk);
	EDI_SetUserData(p, p);
        
	buff = EDI_GetBuffer(p, BUFF_SIZE);
	length = read(input, buff, BUFF_SIZE);
	while(length > 0){
		status = EDI_ParseBuffer(p, length);
		if(status != EDI_STATUS_OK){
			fprintf(stdout, "Error: %d\n", EDI_GetErrorCode(p));
		}
		buff = EDI_GetBuffer(p, BUFF_SIZE);
		length = read(input, buff, BUFF_SIZE);
	}
	EDI_Schema s = EDI_GetSchema(p);	
	if(s){
		EDI_SchemaFree(s);
	}
	EDI_ParserFree(p);
	close(input);

	gettimeofday(&tv, &tz);
	end_time = tv.tv_sec + (tv.tv_usec * 0.000001);
	file_size = statbuf.st_size;
	fprintf(stdout, "Processed %d Bytes of data\n", (int)file_size);
	run_time = end_time - start_time;
	fprintf(stdout, "Runtime: %f seconds\n", run_time);
	speed = (file_size/1048576)/run_time;
	fprintf(stdout, "Average processing speed was %f MB/sec\n", speed);
	return 0;
}
