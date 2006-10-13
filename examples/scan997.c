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

//gcc scan997.c -std=c99 -I../include -L../lib -lm -ledival -o scan997

#include <edival.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 16777216

int group_start = 0;
int counter = 0;
int depth = 0;
char prefix[50];
char curr_seg[4];
int  curr_e = 0;
int  curr_c = 0;
int  is_comp = 0;
int  type_ok = 0;

void handleSegmentError(void *myData, const char *tag, enum EDI_SegmentValidationError err)
{
	if(!(!strcmp(tag, "GS") && err == SEGERR_UNDEFINED) &&
	   !(!strcmp(tag, "GE") && err == SEGERR_UNDEFINED)){
		fprintf(stderr, "*~*~* Segment error %d on %s\n", err, tag);
	}
}

void handleElementError(void *myData, int element, int component, enum EDI_ElementValidationError err)
{
	fprintf(stderr, "\nElement error %d on element %2.2d-%d\n", err, element, component);
}

void handleLoopStart(void *myData, const char *loopID)
{
	fprintf(stderr, "%s<%s>\n", prefix, loopID);
	depth++;
	prefix[0] = '\0';
	for(int i = 0; i < depth; i++){
		strcat(prefix, " ");
	}
	return;
}

void handleLoopEnd(void *myData, const char *loopID)
{
	depth--;
	prefix[0] = '\0';
	for(int i = 0; i < depth; i++){
		strcat(prefix, " ");
	}
	fprintf(stderr, "%s</%s>\n", prefix, loopID);
	return;
}

void load_standard(EDI_Parser p)
{
	EDI_Schema s = NULL;
	EDI_SchemaNode parent = NULL;
	EDI_SchemaNode child  = NULL;
	
	s = EDI_GetSchema(p);
	if(s && strcmp(EDI_GetSchemaId(s), "Functional Acknowledgment: FA/997") != 0){
		s = EDI_RemoveSchema(p);
		EDI_SchemaFree(s);
	}
	if(!s){
		s = EDI_SchemaCreate();
		EDI_SetSchemaId(s, "Functional Acknowledgment: FA/997");
		EDI_SetSchema(p, s);

		EDI_CreateElementType(s, EDI_DATA_INTEGER, "2", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "28", 1, 9);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "96", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "97", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "123", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "143", 3, 3);
		EDI_AddElementValue(s, "143", "100");
		EDI_AddElementValue(s, "143", "101");
		EDI_AddElementValue(s, "143", "102");
		EDI_AddElementValue(s, "143", "103");
		EDI_AddElementValue(s, "143", "104");
		EDI_AddElementValue(s, "143", "105");
		EDI_AddElementValue(s, "143", "106");
		EDI_AddElementValue(s, "143", "107");
		EDI_AddElementValue(s, "143", "108");
		EDI_AddElementValue(s, "143", "109");
		EDI_AddElementValue(s, "143", "110");
		EDI_AddElementValue(s, "143", "111");
		EDI_AddElementValue(s, "143", "112");
		EDI_AddElementValue(s, "143", "113");
		EDI_AddElementValue(s, "143", "120");
		EDI_AddElementValue(s, "143", "121");
		EDI_AddElementValue(s, "143", "124");
		EDI_AddElementValue(s, "143", "125");
		EDI_AddElementValue(s, "143", "126");
		EDI_AddElementValue(s, "143", "127");
		EDI_AddElementValue(s, "143", "128");
		EDI_AddElementValue(s, "143", "129");
		EDI_AddElementValue(s, "143", "130");
		EDI_AddElementValue(s, "143", "131");
		EDI_AddElementValue(s, "143", "132");
		EDI_AddElementValue(s, "143", "133");
		EDI_AddElementValue(s, "143", "135");
		EDI_AddElementValue(s, "143", "138");
		EDI_AddElementValue(s, "143", "139");
		EDI_AddElementValue(s, "143", "140");
		EDI_AddElementValue(s, "143", "141");
		EDI_AddElementValue(s, "143", "142");
		EDI_AddElementValue(s, "143", "143");
		EDI_AddElementValue(s, "143", "144");
		EDI_AddElementValue(s, "143", "146");
		EDI_AddElementValue(s, "143", "147");
		EDI_AddElementValue(s, "143", "148");
		EDI_AddElementValue(s, "143", "149");
		EDI_AddElementValue(s, "143", "150");
		EDI_AddElementValue(s, "143", "151");
		EDI_AddElementValue(s, "143", "152");
		EDI_AddElementValue(s, "143", "153");
		EDI_AddElementValue(s, "143", "154");
		EDI_AddElementValue(s, "143", "155");
		EDI_AddElementValue(s, "143", "157");
		EDI_AddElementValue(s, "143", "158");
		EDI_AddElementValue(s, "143", "159");
		EDI_AddElementValue(s, "143", "160");
		EDI_AddElementValue(s, "143", "161");
		EDI_AddElementValue(s, "143", "163");
		EDI_AddElementValue(s, "143", "170");
		EDI_AddElementValue(s, "143", "175");
		EDI_AddElementValue(s, "143", "176");
		EDI_AddElementValue(s, "143", "179");
		EDI_AddElementValue(s, "143", "180");
		EDI_AddElementValue(s, "143", "185");
		EDI_AddElementValue(s, "143", "186");
		EDI_AddElementValue(s, "143", "187");
		EDI_AddElementValue(s, "143", "188");
		EDI_AddElementValue(s, "143", "189");
		EDI_AddElementValue(s, "143", "190");
		EDI_AddElementValue(s, "143", "191");
		EDI_AddElementValue(s, "143", "194");
		EDI_AddElementValue(s, "143", "195");
		EDI_AddElementValue(s, "143", "196");
		EDI_AddElementValue(s, "143", "197");
		EDI_AddElementValue(s, "143", "198");
		EDI_AddElementValue(s, "143", "199");
		EDI_AddElementValue(s, "143", "200");
		EDI_AddElementValue(s, "143", "201");
		EDI_AddElementValue(s, "143", "202");
		EDI_AddElementValue(s, "143", "203");
		EDI_AddElementValue(s, "143", "204");
		EDI_AddElementValue(s, "143", "205");
		EDI_AddElementValue(s, "143", "206");
		EDI_AddElementValue(s, "143", "210");
		EDI_AddElementValue(s, "143", "211");
		EDI_AddElementValue(s, "143", "212");
		EDI_AddElementValue(s, "143", "213");
		EDI_AddElementValue(s, "143", "214");
		EDI_AddElementValue(s, "143", "215");
		EDI_AddElementValue(s, "143", "216");
		EDI_AddElementValue(s, "143", "217");
		EDI_AddElementValue(s, "143", "219");
		EDI_AddElementValue(s, "143", "220");
		EDI_AddElementValue(s, "143", "222");
		EDI_AddElementValue(s, "143", "223");
		EDI_AddElementValue(s, "143", "224");
		EDI_AddElementValue(s, "143", "225");
		EDI_AddElementValue(s, "143", "227");
		EDI_AddElementValue(s, "143", "228");
		EDI_AddElementValue(s, "143", "240");
		EDI_AddElementValue(s, "143", "242");
		EDI_AddElementValue(s, "143", "244");
		EDI_AddElementValue(s, "143", "245");
		EDI_AddElementValue(s, "143", "248");
		EDI_AddElementValue(s, "143", "249");
		EDI_AddElementValue(s, "143", "250");
		EDI_AddElementValue(s, "143", "251");
		EDI_AddElementValue(s, "143", "252");
		EDI_AddElementValue(s, "143", "255");
		EDI_AddElementValue(s, "143", "256");
		EDI_AddElementValue(s, "143", "259");
		EDI_AddElementValue(s, "143", "260");
		EDI_AddElementValue(s, "143", "261");
		EDI_AddElementValue(s, "143", "262");
		EDI_AddElementValue(s, "143", "263");
		EDI_AddElementValue(s, "143", "264");
		EDI_AddElementValue(s, "143", "265");
		EDI_AddElementValue(s, "143", "266");
		EDI_AddElementValue(s, "143", "267");
		EDI_AddElementValue(s, "143", "268");
		EDI_AddElementValue(s, "143", "269");
		EDI_AddElementValue(s, "143", "270");
		EDI_AddElementValue(s, "143", "271");
		EDI_AddElementValue(s, "143", "272");
		EDI_AddElementValue(s, "143", "273");
		EDI_AddElementValue(s, "143", "274");
		EDI_AddElementValue(s, "143", "275");
		EDI_AddElementValue(s, "143", "276");
		EDI_AddElementValue(s, "143", "277");
		EDI_AddElementValue(s, "143", "278");
		EDI_AddElementValue(s, "143", "280");
		EDI_AddElementValue(s, "143", "283");
		EDI_AddElementValue(s, "143", "284");
		EDI_AddElementValue(s, "143", "285");
		EDI_AddElementValue(s, "143", "286");
		EDI_AddElementValue(s, "143", "288");
		EDI_AddElementValue(s, "143", "290");
		EDI_AddElementValue(s, "143", "300");
		EDI_AddElementValue(s, "143", "301");
		EDI_AddElementValue(s, "143", "303");
		EDI_AddElementValue(s, "143", "304");
		EDI_AddElementValue(s, "143", "309");
		EDI_AddElementValue(s, "143", "310");
		EDI_AddElementValue(s, "143", "311");
		EDI_AddElementValue(s, "143", "312");
		EDI_AddElementValue(s, "143", "313");
		EDI_AddElementValue(s, "143", "315");
		EDI_AddElementValue(s, "143", "317");
		EDI_AddElementValue(s, "143", "319");
		EDI_AddElementValue(s, "143", "322");
		EDI_AddElementValue(s, "143", "323");
		EDI_AddElementValue(s, "143", "324");
		EDI_AddElementValue(s, "143", "325");
		EDI_AddElementValue(s, "143", "326");
		EDI_AddElementValue(s, "143", "350");
		EDI_AddElementValue(s, "143", "352");
		EDI_AddElementValue(s, "143", "353");
		EDI_AddElementValue(s, "143", "354");
		EDI_AddElementValue(s, "143", "355");
		EDI_AddElementValue(s, "143", "356");
		EDI_AddElementValue(s, "143", "357");
		EDI_AddElementValue(s, "143", "358");
		EDI_AddElementValue(s, "143", "361");
		EDI_AddElementValue(s, "143", "362");
		EDI_AddElementValue(s, "143", "404");
		EDI_AddElementValue(s, "143", "410");
		EDI_AddElementValue(s, "143", "412");
		EDI_AddElementValue(s, "143", "414");
		EDI_AddElementValue(s, "143", "417");
		EDI_AddElementValue(s, "143", "418");
		EDI_AddElementValue(s, "143", "419");
		EDI_AddElementValue(s, "143", "420");
		EDI_AddElementValue(s, "143", "421");
		EDI_AddElementValue(s, "143", "422");
		EDI_AddElementValue(s, "143", "423");
		EDI_AddElementValue(s, "143", "424");
		EDI_AddElementValue(s, "143", "425");
		EDI_AddElementValue(s, "143", "426");
		EDI_AddElementValue(s, "143", "429");
		EDI_AddElementValue(s, "143", "431");
		EDI_AddElementValue(s, "143", "432");
		EDI_AddElementValue(s, "143", "433");
		EDI_AddElementValue(s, "143", "434");
		EDI_AddElementValue(s, "143", "435");
		EDI_AddElementValue(s, "143", "436");
		EDI_AddElementValue(s, "143", "437");
		EDI_AddElementValue(s, "143", "440");
		EDI_AddElementValue(s, "143", "451");
		EDI_AddElementValue(s, "143", "452");
		EDI_AddElementValue(s, "143", "453");
		EDI_AddElementValue(s, "143", "455");
		EDI_AddElementValue(s, "143", "456");
		EDI_AddElementValue(s, "143", "460");
		EDI_AddElementValue(s, "143", "463");
		EDI_AddElementValue(s, "143", "466");
		EDI_AddElementValue(s, "143", "468");
		EDI_AddElementValue(s, "143", "470");
		EDI_AddElementValue(s, "143", "475");
		EDI_AddElementValue(s, "143", "485");
		EDI_AddElementValue(s, "143", "486");
		EDI_AddElementValue(s, "143", "490");
		EDI_AddElementValue(s, "143", "492");
		EDI_AddElementValue(s, "143", "494");
		EDI_AddElementValue(s, "143", "500");
		EDI_AddElementValue(s, "143", "501");
		EDI_AddElementValue(s, "143", "503");
		EDI_AddElementValue(s, "143", "504");
		EDI_AddElementValue(s, "143", "511");
		EDI_AddElementValue(s, "143", "517");
		EDI_AddElementValue(s, "143", "521");
		EDI_AddElementValue(s, "143", "527");
		EDI_AddElementValue(s, "143", "536");
		EDI_AddElementValue(s, "143", "540");
		EDI_AddElementValue(s, "143", "561");
		EDI_AddElementValue(s, "143", "567");
		EDI_AddElementValue(s, "143", "568");
		EDI_AddElementValue(s, "143", "601");
		EDI_AddElementValue(s, "143", "602");
		EDI_AddElementValue(s, "143", "620");
		EDI_AddElementValue(s, "143", "625");
		EDI_AddElementValue(s, "143", "650");
		EDI_AddElementValue(s, "143", "715");
		EDI_AddElementValue(s, "143", "753");
		EDI_AddElementValue(s, "143", "754");
		EDI_AddElementValue(s, "143", "805");
		EDI_AddElementValue(s, "143", "806");
		EDI_AddElementValue(s, "143", "810");
		EDI_AddElementValue(s, "143", "811");
		EDI_AddElementValue(s, "143", "812");
		EDI_AddElementValue(s, "143", "813");
		EDI_AddElementValue(s, "143", "814");
		EDI_AddElementValue(s, "143", "815");
		EDI_AddElementValue(s, "143", "816");
		EDI_AddElementValue(s, "143", "818");
		EDI_AddElementValue(s, "143", "819");
		EDI_AddElementValue(s, "143", "820");
		EDI_AddElementValue(s, "143", "821");
		EDI_AddElementValue(s, "143", "822");
		EDI_AddElementValue(s, "143", "823");
		EDI_AddElementValue(s, "143", "824");
		EDI_AddElementValue(s, "143", "826");
		EDI_AddElementValue(s, "143", "827");
		EDI_AddElementValue(s, "143", "828");
		EDI_AddElementValue(s, "143", "829");
		EDI_AddElementValue(s, "143", "830");
		EDI_AddElementValue(s, "143", "831");
		EDI_AddElementValue(s, "143", "832");
		EDI_AddElementValue(s, "143", "833");
		EDI_AddElementValue(s, "143", "834");
		EDI_AddElementValue(s, "143", "835");
		EDI_AddElementValue(s, "143", "836");
		EDI_AddElementValue(s, "143", "837");
		EDI_AddElementValue(s, "143", "838");
		EDI_AddElementValue(s, "143", "839");
		EDI_AddElementValue(s, "143", "840");
		EDI_AddElementValue(s, "143", "841");
		EDI_AddElementValue(s, "143", "842");
		EDI_AddElementValue(s, "143", "843");
		EDI_AddElementValue(s, "143", "844");
		EDI_AddElementValue(s, "143", "845");
		EDI_AddElementValue(s, "143", "846");
		EDI_AddElementValue(s, "143", "847");
		EDI_AddElementValue(s, "143", "848");
		EDI_AddElementValue(s, "143", "849");
		EDI_AddElementValue(s, "143", "850");
		EDI_AddElementValue(s, "143", "851");
		EDI_AddElementValue(s, "143", "852");
		EDI_AddElementValue(s, "143", "853");
		EDI_AddElementValue(s, "143", "854");
		EDI_AddElementValue(s, "143", "855");
		EDI_AddElementValue(s, "143", "856");
		EDI_AddElementValue(s, "143", "857");
		EDI_AddElementValue(s, "143", "858");
		EDI_AddElementValue(s, "143", "859");
		EDI_AddElementValue(s, "143", "860");
		EDI_AddElementValue(s, "143", "861");
		EDI_AddElementValue(s, "143", "862");
		EDI_AddElementValue(s, "143", "863");
		EDI_AddElementValue(s, "143", "864");
		EDI_AddElementValue(s, "143", "865");
		EDI_AddElementValue(s, "143", "866");
		EDI_AddElementValue(s, "143", "867");
		EDI_AddElementValue(s, "143", "868");
		EDI_AddElementValue(s, "143", "869");
		EDI_AddElementValue(s, "143", "870");
		EDI_AddElementValue(s, "143", "871");
		EDI_AddElementValue(s, "143", "872");
		EDI_AddElementValue(s, "143", "873");
		EDI_AddElementValue(s, "143", "874");
		EDI_AddElementValue(s, "143", "875");
		EDI_AddElementValue(s, "143", "876");
		EDI_AddElementValue(s, "143", "877");
		EDI_AddElementValue(s, "143", "878");
		EDI_AddElementValue(s, "143", "879");
		EDI_AddElementValue(s, "143", "880");
		EDI_AddElementValue(s, "143", "881");
		EDI_AddElementValue(s, "143", "882");
		EDI_AddElementValue(s, "143", "883");
		EDI_AddElementValue(s, "143", "884");
		EDI_AddElementValue(s, "143", "885");
		EDI_AddElementValue(s, "143", "886");
		EDI_AddElementValue(s, "143", "887");
		EDI_AddElementValue(s, "143", "888");
		EDI_AddElementValue(s, "143", "889");
		EDI_AddElementValue(s, "143", "891");
		EDI_AddElementValue(s, "143", "893");
		EDI_AddElementValue(s, "143", "894");
		EDI_AddElementValue(s, "143", "895");
		EDI_AddElementValue(s, "143", "896");
		EDI_AddElementValue(s, "143", "920");
		EDI_AddElementValue(s, "143", "924");
		EDI_AddElementValue(s, "143", "925");
		EDI_AddElementValue(s, "143", "926");
		EDI_AddElementValue(s, "143", "928");
		EDI_AddElementValue(s, "143", "940");
		EDI_AddElementValue(s, "143", "943");
		EDI_AddElementValue(s, "143", "944");
		EDI_AddElementValue(s, "143", "945");
		EDI_AddElementValue(s, "143", "947");
		EDI_AddElementValue(s, "143", "980");
		EDI_AddElementValue(s, "143", "990");
		EDI_AddElementValue(s, "143", "993");
		EDI_AddElementValue(s, "143", "996");
		EDI_AddElementValue(s, "143", "997");
		EDI_AddElementValue(s, "143", "998");
		EDI_AddElementValue(s, "143", "999");
		EDI_CreateElementType(s, EDI_DATA_STRING, "329", 4, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "447", 1, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "479", 2, 2);
		EDI_AddElementValue(s, "479", "AA");
		EDI_AddElementValue(s, "479", "AB");
		EDI_AddElementValue(s, "479", "AC");
		EDI_AddElementValue(s, "479", "AD");
		EDI_AddElementValue(s, "479", "AE");
		EDI_AddElementValue(s, "479", "AF");
		EDI_AddElementValue(s, "479", "AG");
		EDI_AddElementValue(s, "479", "AH");
		EDI_AddElementValue(s, "479", "AI");
		EDI_AddElementValue(s, "479", "AK");
		EDI_AddElementValue(s, "479", "AL");
		EDI_AddElementValue(s, "479", "AM");
		EDI_AddElementValue(s, "479", "AN");
		EDI_AddElementValue(s, "479", "AO");
		EDI_AddElementValue(s, "479", "AP");
		EDI_AddElementValue(s, "479", "AQ");
		EDI_AddElementValue(s, "479", "AR");
		EDI_AddElementValue(s, "479", "AS");
		EDI_AddElementValue(s, "479", "AT");
		EDI_AddElementValue(s, "479", "AU");
		EDI_AddElementValue(s, "479", "AV");
		EDI_AddElementValue(s, "479", "AW");
		EDI_AddElementValue(s, "479", "AX");
		EDI_AddElementValue(s, "479", "AY");
		EDI_AddElementValue(s, "479", "AZ");
		EDI_AddElementValue(s, "479", "BA");
		EDI_AddElementValue(s, "479", "BB");
		EDI_AddElementValue(s, "479", "BC");
		EDI_AddElementValue(s, "479", "BD");
		EDI_AddElementValue(s, "479", "BE");
		EDI_AddElementValue(s, "479", "BF");
		EDI_AddElementValue(s, "479", "BL");
		EDI_AddElementValue(s, "479", "BS");
		EDI_AddElementValue(s, "479", "CA");
		EDI_AddElementValue(s, "479", "CB");
		EDI_AddElementValue(s, "479", "CC");
		EDI_AddElementValue(s, "479", "CD");
		EDI_AddElementValue(s, "479", "CE");
		EDI_AddElementValue(s, "479", "CF");
		EDI_AddElementValue(s, "479", "CH");
		EDI_AddElementValue(s, "479", "CI");
		EDI_AddElementValue(s, "479", "CJ");
		EDI_AddElementValue(s, "479", "CK");
		EDI_AddElementValue(s, "479", "CL");
		EDI_AddElementValue(s, "479", "CM");
		EDI_AddElementValue(s, "479", "CN");
		EDI_AddElementValue(s, "479", "CO");
		EDI_AddElementValue(s, "479", "CP");
		EDI_AddElementValue(s, "479", "CQ");
		EDI_AddElementValue(s, "479", "CR");
		EDI_AddElementValue(s, "479", "CS");
		EDI_AddElementValue(s, "479", "CT");
		EDI_AddElementValue(s, "479", "CU");
		EDI_AddElementValue(s, "479", "CV");
		EDI_AddElementValue(s, "479", "CW");
		EDI_AddElementValue(s, "479", "DA");
		EDI_AddElementValue(s, "479", "DD");
		EDI_AddElementValue(s, "479", "DF");
		EDI_AddElementValue(s, "479", "DI");
		EDI_AddElementValue(s, "479", "DM");
		EDI_AddElementValue(s, "479", "DS");
		EDI_AddElementValue(s, "479", "DX");
		EDI_AddElementValue(s, "479", "D3");
		EDI_AddElementValue(s, "479", "D4");
		EDI_AddElementValue(s, "479", "D5");
		EDI_AddElementValue(s, "479", "EC");
		EDI_AddElementValue(s, "479", "ED");
		EDI_AddElementValue(s, "479", "EI");
		EDI_AddElementValue(s, "479", "EN");
		EDI_AddElementValue(s, "479", "EP");
		EDI_AddElementValue(s, "479", "ER");
		EDI_AddElementValue(s, "479", "ES");
		EDI_AddElementValue(s, "479", "EV");
		EDI_AddElementValue(s, "479", "EX");
		EDI_AddElementValue(s, "479", "FA");
		EDI_AddElementValue(s, "479", "FB");
		EDI_AddElementValue(s, "479", "FC");
		EDI_AddElementValue(s, "479", "FG");
		EDI_AddElementValue(s, "479", "FR");
		EDI_AddElementValue(s, "479", "FT");
		EDI_AddElementValue(s, "479", "GC");
		EDI_AddElementValue(s, "479", "GE");
		EDI_AddElementValue(s, "479", "GF");
		EDI_AddElementValue(s, "479", "GL");
		EDI_AddElementValue(s, "479", "GP");
		EDI_AddElementValue(s, "479", "GR");
		EDI_AddElementValue(s, "479", "GT");
		EDI_AddElementValue(s, "479", "HB");
		EDI_AddElementValue(s, "479", "HC");
		EDI_AddElementValue(s, "479", "HI");
		EDI_AddElementValue(s, "479", "HN");
		EDI_AddElementValue(s, "479", "HP");
		EDI_AddElementValue(s, "479", "HR");
		EDI_AddElementValue(s, "479", "HS");
		EDI_AddElementValue(s, "479", "HU");
		EDI_AddElementValue(s, "479", "HV");
		EDI_AddElementValue(s, "479", "IA");
		EDI_AddElementValue(s, "479", "IB");
		EDI_AddElementValue(s, "479", "IC");
		EDI_AddElementValue(s, "479", "ID");
		EDI_AddElementValue(s, "479", "IE");
		EDI_AddElementValue(s, "479", "IF");
		EDI_AddElementValue(s, "479", "IG");
		EDI_AddElementValue(s, "479", "IH");
		EDI_AddElementValue(s, "479", "IJ");
		EDI_AddElementValue(s, "479", "IM");
		EDI_AddElementValue(s, "479", "IN");
		EDI_AddElementValue(s, "479", "IO");
		EDI_AddElementValue(s, "479", "IR");
		EDI_AddElementValue(s, "479", "IS");
		EDI_AddElementValue(s, "479", "JB");
		EDI_AddElementValue(s, "479", "KM");
		EDI_AddElementValue(s, "479", "LA");
		EDI_AddElementValue(s, "479", "LB");
		EDI_AddElementValue(s, "479", "LI");
		EDI_AddElementValue(s, "479", "LN");
		EDI_AddElementValue(s, "479", "LR");
		EDI_AddElementValue(s, "479", "LS");
		EDI_AddElementValue(s, "479", "LT");
		EDI_AddElementValue(s, "479", "MA");
		EDI_AddElementValue(s, "479", "MC");
		EDI_AddElementValue(s, "479", "MD");
		EDI_AddElementValue(s, "479", "ME");
		EDI_AddElementValue(s, "479", "MF");
		EDI_AddElementValue(s, "479", "MG");
		EDI_AddElementValue(s, "479", "MH");
		EDI_AddElementValue(s, "479", "MI");
		EDI_AddElementValue(s, "479", "MJ");
		EDI_AddElementValue(s, "479", "MK");
		EDI_AddElementValue(s, "479", "MM");
		EDI_AddElementValue(s, "479", "MN");
		EDI_AddElementValue(s, "479", "MO");
		EDI_AddElementValue(s, "479", "MP");
		EDI_AddElementValue(s, "479", "MQ");
		EDI_AddElementValue(s, "479", "MR");
		EDI_AddElementValue(s, "479", "MS");
		EDI_AddElementValue(s, "479", "MT");
		EDI_AddElementValue(s, "479", "MV");
		EDI_AddElementValue(s, "479", "MW");
		EDI_AddElementValue(s, "479", "MX");
		EDI_AddElementValue(s, "479", "MY");
		EDI_AddElementValue(s, "479", "MZ");
		EDI_AddElementValue(s, "479", "NC");
		EDI_AddElementValue(s, "479", "NL");
		EDI_AddElementValue(s, "479", "NP");
		EDI_AddElementValue(s, "479", "NR");
		EDI_AddElementValue(s, "479", "NT");
		EDI_AddElementValue(s, "479", "OC");
		EDI_AddElementValue(s, "479", "OG");
		EDI_AddElementValue(s, "479", "OR");
		EDI_AddElementValue(s, "479", "OW");
		EDI_AddElementValue(s, "479", "PA");
		EDI_AddElementValue(s, "479", "PB");
		EDI_AddElementValue(s, "479", "PC");
		EDI_AddElementValue(s, "479", "PD");
		EDI_AddElementValue(s, "479", "PE");
		EDI_AddElementValue(s, "479", "PF");
		EDI_AddElementValue(s, "479", "PG");
		EDI_AddElementValue(s, "479", "PH");
		EDI_AddElementValue(s, "479", "PI");
		EDI_AddElementValue(s, "479", "PJ");
		EDI_AddElementValue(s, "479", "PK");
		EDI_AddElementValue(s, "479", "PL");
		EDI_AddElementValue(s, "479", "PN");
		EDI_AddElementValue(s, "479", "PO");
		EDI_AddElementValue(s, "479", "PQ");
		EDI_AddElementValue(s, "479", "PR");
		EDI_AddElementValue(s, "479", "PS");
		EDI_AddElementValue(s, "479", "PT");
		EDI_AddElementValue(s, "479", "PU");
		EDI_AddElementValue(s, "479", "PV");
		EDI_AddElementValue(s, "479", "PW");
		EDI_AddElementValue(s, "479", "PY");
		EDI_AddElementValue(s, "479", "QG");
		EDI_AddElementValue(s, "479", "QM");
		EDI_AddElementValue(s, "479", "QO");
		EDI_AddElementValue(s, "479", "RA");
		EDI_AddElementValue(s, "479", "RB");
		EDI_AddElementValue(s, "479", "RC");
		EDI_AddElementValue(s, "479", "RD");
		EDI_AddElementValue(s, "479", "RE");
		EDI_AddElementValue(s, "479", "RF");
		EDI_AddElementValue(s, "479", "RG");
		EDI_AddElementValue(s, "479", "RH");
		EDI_AddElementValue(s, "479", "RI");
		EDI_AddElementValue(s, "479", "RJ");
		EDI_AddElementValue(s, "479", "RK");
		EDI_AddElementValue(s, "479", "RL");
		EDI_AddElementValue(s, "479", "RM");
		EDI_AddElementValue(s, "479", "RN");
		EDI_AddElementValue(s, "479", "RO");
		EDI_AddElementValue(s, "479", "RP");
		EDI_AddElementValue(s, "479", "RQ");
		EDI_AddElementValue(s, "479", "RR");
		EDI_AddElementValue(s, "479", "RS");
		EDI_AddElementValue(s, "479", "RT");
		EDI_AddElementValue(s, "479", "RU");
		EDI_AddElementValue(s, "479", "RV");
		EDI_AddElementValue(s, "479", "RW");
		EDI_AddElementValue(s, "479", "RX");
		EDI_AddElementValue(s, "479", "RY");
		EDI_AddElementValue(s, "479", "RZ");
		EDI_AddElementValue(s, "479", "SA");
		EDI_AddElementValue(s, "479", "SB");
		EDI_AddElementValue(s, "479", "SC");
		EDI_AddElementValue(s, "479", "SD");
		EDI_AddElementValue(s, "479", "SE");
		EDI_AddElementValue(s, "479", "SH");
		EDI_AddElementValue(s, "479", "SI");
		EDI_AddElementValue(s, "479", "SJ");
		EDI_AddElementValue(s, "479", "SL");
		EDI_AddElementValue(s, "479", "SM");
		EDI_AddElementValue(s, "479", "SN");
		EDI_AddElementValue(s, "479", "SO");
		EDI_AddElementValue(s, "479", "SP");
		EDI_AddElementValue(s, "479", "SQ");
		EDI_AddElementValue(s, "479", "SR");
		EDI_AddElementValue(s, "479", "SS");
		EDI_AddElementValue(s, "479", "ST");
		EDI_AddElementValue(s, "479", "SU");
		EDI_AddElementValue(s, "479", "SV");
		EDI_AddElementValue(s, "479", "SW");
		EDI_AddElementValue(s, "479", "TA");
		EDI_AddElementValue(s, "479", "TB");
		EDI_AddElementValue(s, "479", "TD");
		EDI_AddElementValue(s, "479", "TE");
		EDI_AddElementValue(s, "479", "TF");
		EDI_AddElementValue(s, "479", "TI");
		EDI_AddElementValue(s, "479", "TJ");
		EDI_AddElementValue(s, "479", "TM");
		EDI_AddElementValue(s, "479", "TN");
		EDI_AddElementValue(s, "479", "TO");
		EDI_AddElementValue(s, "479", "TP");
		EDI_AddElementValue(s, "479", "TR");
		EDI_AddElementValue(s, "479", "TS");
		EDI_AddElementValue(s, "479", "TT");
		EDI_AddElementValue(s, "479", "TU");
		EDI_AddElementValue(s, "479", "TX");
		EDI_AddElementValue(s, "479", "UA");
		EDI_AddElementValue(s, "479", "UB");
		EDI_AddElementValue(s, "479", "UC");
		EDI_AddElementValue(s, "479", "UD");
		EDI_AddElementValue(s, "479", "UI");
		EDI_AddElementValue(s, "479", "UP");
		EDI_AddElementValue(s, "479", "UW");
		EDI_AddElementValue(s, "479", "VA");
		EDI_AddElementValue(s, "479", "VB");
		EDI_AddElementValue(s, "479", "VC");
		EDI_AddElementValue(s, "479", "VD");
		EDI_AddElementValue(s, "479", "VE");
		EDI_AddElementValue(s, "479", "VH");
		EDI_AddElementValue(s, "479", "VI");
		EDI_AddElementValue(s, "479", "VS");
		EDI_AddElementValue(s, "479", "WA");
		EDI_AddElementValue(s, "479", "WB");
		EDI_AddElementValue(s, "479", "WG");
		EDI_AddElementValue(s, "479", "WI");
		EDI_AddElementValue(s, "479", "WL");
		EDI_AddElementValue(s, "479", "WR");
		EDI_AddElementValue(s, "479", "WT");
		EDI_CreateElementType(s, EDI_DATA_STRING, "480", 1, 12);
		EDI_CreateElementType(s, EDI_DATA_STRING, "715", 1, 1);
		EDI_AddElementValue(s, "715", "A");
		EDI_AddElementValue(s, "715", "E");
		EDI_AddElementValue(s, "715", "M");
		EDI_AddElementValue(s, "715", "P");
		EDI_AddElementValue(s, "715", "R");
		EDI_AddElementValue(s, "715", "W");
		EDI_AddElementValue(s, "715", "X");
		EDI_CreateElementType(s, EDI_DATA_STRING, "716", 1, 3);
		EDI_AddElementValue(s, "716", "1");
		EDI_AddElementValue(s, "716", "2");
		EDI_AddElementValue(s, "716", "3");
		EDI_AddElementValue(s, "716", "4");
		EDI_AddElementValue(s, "716", "5");
		EDI_AddElementValue(s, "716", "6");
		EDI_AddElementValue(s, "716", "10");
		EDI_AddElementValue(s, "716", "11");
		EDI_AddElementValue(s, "716", "12");
		EDI_AddElementValue(s, "716", "13");
		EDI_AddElementValue(s, "716", "14");
		EDI_AddElementValue(s, "716", "15");
		EDI_AddElementValue(s, "716", "16");
		EDI_AddElementValue(s, "716", "17");
		EDI_AddElementValue(s, "716", "18");
		EDI_AddElementValue(s, "716", "19");
		EDI_AddElementValue(s, "716", "23");
		EDI_AddElementValue(s, "716", "24");
		EDI_AddElementValue(s, "716", "25");
		EDI_AddElementValue(s, "716", "26");
		EDI_CreateElementType(s, EDI_DATA_STRING, "717", 1, 1);
		EDI_AddElementValue(s, "717", "A");
		EDI_AddElementValue(s, "717", "E");
		EDI_AddElementValue(s, "717", "M");
		EDI_AddElementValue(s, "717", "R");
		EDI_AddElementValue(s, "717", "W");
		EDI_AddElementValue(s, "717", "X");
		EDI_CreateElementType(s, EDI_DATA_STRING, "718", 1, 3);
		EDI_AddElementValue(s, "718", "1");
		EDI_AddElementValue(s, "718", "2");
		EDI_AddElementValue(s, "718", "3");
		EDI_AddElementValue(s, "718", "4");
		EDI_AddElementValue(s, "718", "5");
		EDI_AddElementValue(s, "718", "6");
		EDI_AddElementValue(s, "718", "7");
		EDI_AddElementValue(s, "718", "8");
		EDI_AddElementValue(s, "718", "9");
		EDI_AddElementValue(s, "718", "10");
		EDI_AddElementValue(s, "718", "11");
		EDI_AddElementValue(s, "718", "12");
		EDI_AddElementValue(s, "718", "13");
		EDI_AddElementValue(s, "718", "15");
		EDI_AddElementValue(s, "718", "16");
		EDI_AddElementValue(s, "718", "17");
		EDI_AddElementValue(s, "718", "18");
		EDI_AddElementValue(s, "718", "19");
		EDI_AddElementValue(s, "718", "23");
		EDI_AddElementValue(s, "718", "24");
		EDI_AddElementValue(s, "718", "25");
		EDI_AddElementValue(s, "718", "26");
		EDI_AddElementValue(s, "718", "27");
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "719", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "720", 1, 3);
		EDI_AddElementValue(s, "720", "1");
		EDI_AddElementValue(s, "720", "2");
		EDI_AddElementValue(s, "720", "3");
		EDI_AddElementValue(s, "720", "4");
		EDI_AddElementValue(s, "720", "5");
		EDI_AddElementValue(s, "720", "6");
		EDI_AddElementValue(s, "720", "7");
		EDI_AddElementValue(s, "720", "8");
		EDI_CreateElementType(s, EDI_DATA_STRING, "721", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "722", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "723", 1, 3);
		EDI_AddElementValue(s, "723", "1");
		EDI_AddElementValue(s, "723", "2");
		EDI_AddElementValue(s, "723", "3");
		EDI_AddElementValue(s, "723", "4");
		EDI_AddElementValue(s, "723", "5");
		EDI_AddElementValue(s, "723", "6");
		EDI_AddElementValue(s, "723", "7");
		EDI_AddElementValue(s, "723", "8");
		EDI_AddElementValue(s, "723", "9");
		EDI_AddElementValue(s, "723", "10");
		EDI_AddElementValue(s, "723", "12");
		EDI_AddElementValue(s, "723", "13");
		EDI_CreateElementType(s, EDI_DATA_STRING, "724", 1, 99);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "725", 1, 4);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1528", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1686", 1, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1705", 1, 35);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C030");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "722"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1528"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1686"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "ST");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "143"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "479"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "28"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "480"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK2");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "143"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1705"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK3");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "721"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "719"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "447"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "720"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK4");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C030"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "725"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "723"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "724"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK5");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "717"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK9");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "715"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "97"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "123"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "2"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SE");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "96"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		EDI_SchemaNode root = EDI_CreateComplexType(s, EDITYPE_TRANSACTION, "FA997-005010X230");
		EDI_AppendType(s, root, EDI_GetComplexNodeByID(s, "ST"), 1, 1);
		EDI_AppendType(s, root, EDI_GetComplexNodeByID(s, "AK1"), 1, 1);
		EDI_SchemaNode loop2000 = EDI_CreateComplexType(s, EDITYPE_LOOP, "loop2000");
		EDI_AppendType(s, loop2000, EDI_GetComplexNodeByID(s, "AK2"), 1, 1);
		EDI_SchemaNode loop2100 = EDI_CreateComplexType(s, EDITYPE_LOOP, "loop2100");
		EDI_AppendType(s, loop2100, EDI_GetComplexNodeByID(s, "AK3"), 1, 1);
		EDI_AppendType(s, loop2100, EDI_GetComplexNodeByID(s, "AK4"), 0, 99);
		EDI_StoreComplexNode(s, loop2100);
		EDI_AppendType(s, loop2000, loop2100, 0, 999999);
		EDI_AppendType(s, loop2000, EDI_GetComplexNodeByID(s, "AK5"), 1, 1);
		EDI_StoreComplexNode(s, loop2000);
		EDI_AppendType(s, root, loop2000, 0, 999999);
		EDI_AppendType(s, root, EDI_GetComplexNodeByID(s, "AK9"), 1, 1);
		EDI_AppendType(s, root, EDI_GetComplexNodeByID(s, "SE"), 1, 1);
		
		EDI_SetSegmentErrorHandler(s, &handleSegmentError);
		EDI_SetElementErrorHandler(s, &handleElementError);
		EDI_SetLoopStartHandler(s, &handleLoopStart);
		EDI_SetLoopEndHandler(s, &handleLoopEnd);
	}
	return;
}

void handleSegmentStart(void *myData, const char *tag)
{
	fprintf(stderr, "%sSeg: %3s ->", prefix, tag);
	if(strcmp("GS", tag) == 0){
		group_start = 1;
		counter = 0;
		type_ok = 0;
	}
	strcpy(curr_seg, tag);
	curr_e = 0;
	return;
}

void handleSegmentEnd(void *myData, const char *tag)
{
    fprintf(stderr, " <- End %s\n", tag);
    return;
}

void handleCompositeStart(void *myData)
{
	curr_e++;
	curr_c = 0;
	is_comp = 1;
	fprintf(stderr, "{");
	return;
}

void handleCompositeEnd(void *myData)
{
    fprintf(stderr, "}");
    is_comp = 0;
    return;
}

void handleElement(void *myData, const char *val)
{
	fprintf(stderr, "[%s]", val);
	if(is_comp){
		curr_c++;
		/*fprintf(stderr, "<%s%2.2d-%d>%s</%s%2.2d-%d>", curr_seg, curr_e, curr_c, val, curr_seg, curr_e, curr_c);*/
	} else {
		curr_e++;
		/*fprintf(stderr, "<%s%2.2d>%s</%s%2.2d>", curr_seg, curr_e, val, curr_seg, curr_e);*/
	}
	if(group_start){
		counter++;
		if(counter == 1){
			if(strcmp("FA", val) == 0){
				type_ok = 1;
			} else {
				fprintf(stderr, "*** Type not 'FA': No transaction-level validation will be performed on this functional group.\n");
			}
		} else if(counter == 8){
			if(type_ok){
				if((strcmp("005010X230", val) == 0)){
				load_standard((EDI_Parser)myData);
				} else {
					fprintf(stderr, "*** Version not '005010X230': No transaction-level validation will be performed on this functional group.\n");
				}
			}
			group_start = 0;
		}
	}
	return;
}

void handleJunk(void *myData, const char *val)
{
    fprintf(stderr, "Junk: >>{%s}<<\n", val);
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
	
	gettimeofday(&tv, NULL);
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
	
	EDI_SetSegmentStartHandler(p, handleSegmentStart);
	EDI_SetSegmentEndHandler(p, handleSegmentEnd);
	EDI_SetCompositeStartHandler(p, handleCompositeStart);
	EDI_SetCompositeEndHandler(p, handleCompositeEnd);
	EDI_SetElementHandler(p, handleElement);
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

	gettimeofday(&tv, NULL);
	end_time = tv.tv_sec + (tv.tv_usec * 0.000001);
	file_size = statbuf.st_size;
	fprintf(stdout, "Processed %d Bytes of data\n", (int)file_size);
	run_time = end_time - start_time;
	fprintf(stdout, "Runtime: %f seconds\n", run_time);
	speed = (file_size/1048576)/run_time;
	fprintf(stdout, "Average processing speed was %f MB/sec\n", speed);
	return 0;
}
