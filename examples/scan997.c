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
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "143", 3, 3);
		EDI_AddElementValue(parent, "100");
		EDI_AddElementValue(parent, "101");
		EDI_AddElementValue(parent, "102");
		EDI_AddElementValue(parent, "103");
		EDI_AddElementValue(parent, "104");
		EDI_AddElementValue(parent, "105");
		EDI_AddElementValue(parent, "106");
		EDI_AddElementValue(parent, "107");
		EDI_AddElementValue(parent, "108");
		EDI_AddElementValue(parent, "109");
		EDI_AddElementValue(parent, "110");
		EDI_AddElementValue(parent, "111");
		EDI_AddElementValue(parent, "112");
		EDI_AddElementValue(parent, "113");
		EDI_AddElementValue(parent, "120");
		EDI_AddElementValue(parent, "121");
		EDI_AddElementValue(parent, "124");
		EDI_AddElementValue(parent, "125");
		EDI_AddElementValue(parent, "126");
		EDI_AddElementValue(parent, "127");
		EDI_AddElementValue(parent, "128");
		EDI_AddElementValue(parent, "129");
		EDI_AddElementValue(parent, "130");
		EDI_AddElementValue(parent, "131");
		EDI_AddElementValue(parent, "132");
		EDI_AddElementValue(parent, "133");
		EDI_AddElementValue(parent, "135");
		EDI_AddElementValue(parent, "138");
		EDI_AddElementValue(parent, "139");
		EDI_AddElementValue(parent, "140");
		EDI_AddElementValue(parent, "141");
		EDI_AddElementValue(parent, "142");
		EDI_AddElementValue(parent, "143");
		EDI_AddElementValue(parent, "144");
		EDI_AddElementValue(parent, "146");
		EDI_AddElementValue(parent, "147");
		EDI_AddElementValue(parent, "148");
		EDI_AddElementValue(parent, "149");
		EDI_AddElementValue(parent, "150");
		EDI_AddElementValue(parent, "151");
		EDI_AddElementValue(parent, "152");
		EDI_AddElementValue(parent, "153");
		EDI_AddElementValue(parent, "154");
		EDI_AddElementValue(parent, "155");
		EDI_AddElementValue(parent, "157");
		EDI_AddElementValue(parent, "158");
		EDI_AddElementValue(parent, "159");
		EDI_AddElementValue(parent, "160");
		EDI_AddElementValue(parent, "161");
		EDI_AddElementValue(parent, "163");
		EDI_AddElementValue(parent, "170");
		EDI_AddElementValue(parent, "175");
		EDI_AddElementValue(parent, "176");
		EDI_AddElementValue(parent, "179");
		EDI_AddElementValue(parent, "180");
		EDI_AddElementValue(parent, "185");
		EDI_AddElementValue(parent, "186");
		EDI_AddElementValue(parent, "187");
		EDI_AddElementValue(parent, "188");
		EDI_AddElementValue(parent, "189");
		EDI_AddElementValue(parent, "190");
		EDI_AddElementValue(parent, "191");
		EDI_AddElementValue(parent, "194");
		EDI_AddElementValue(parent, "195");
		EDI_AddElementValue(parent, "196");
		EDI_AddElementValue(parent, "197");
		EDI_AddElementValue(parent, "198");
		EDI_AddElementValue(parent, "199");
		EDI_AddElementValue(parent, "200");
		EDI_AddElementValue(parent, "201");
		EDI_AddElementValue(parent, "202");
		EDI_AddElementValue(parent, "203");
		EDI_AddElementValue(parent, "204");
		EDI_AddElementValue(parent, "205");
		EDI_AddElementValue(parent, "206");
		EDI_AddElementValue(parent, "210");
		EDI_AddElementValue(parent, "211");
		EDI_AddElementValue(parent, "212");
		EDI_AddElementValue(parent, "213");
		EDI_AddElementValue(parent, "214");
		EDI_AddElementValue(parent, "215");
		EDI_AddElementValue(parent, "216");
		EDI_AddElementValue(parent, "217");
		EDI_AddElementValue(parent, "219");
		EDI_AddElementValue(parent, "220");
		EDI_AddElementValue(parent, "222");
		EDI_AddElementValue(parent, "223");
		EDI_AddElementValue(parent, "224");
		EDI_AddElementValue(parent, "225");
		EDI_AddElementValue(parent, "227");
		EDI_AddElementValue(parent, "228");
		EDI_AddElementValue(parent, "240");
		EDI_AddElementValue(parent, "242");
		EDI_AddElementValue(parent, "244");
		EDI_AddElementValue(parent, "245");
		EDI_AddElementValue(parent, "248");
		EDI_AddElementValue(parent, "249");
		EDI_AddElementValue(parent, "250");
		EDI_AddElementValue(parent, "251");
		EDI_AddElementValue(parent, "252");
		EDI_AddElementValue(parent, "255");
		EDI_AddElementValue(parent, "256");
		EDI_AddElementValue(parent, "259");
		EDI_AddElementValue(parent, "260");
		EDI_AddElementValue(parent, "261");
		EDI_AddElementValue(parent, "262");
		EDI_AddElementValue(parent, "263");
		EDI_AddElementValue(parent, "264");
		EDI_AddElementValue(parent, "265");
		EDI_AddElementValue(parent, "266");
		EDI_AddElementValue(parent, "267");
		EDI_AddElementValue(parent, "268");
		EDI_AddElementValue(parent, "269");
		EDI_AddElementValue(parent, "270");
		EDI_AddElementValue(parent, "271");
		EDI_AddElementValue(parent, "272");
		EDI_AddElementValue(parent, "273");
		EDI_AddElementValue(parent, "274");
		EDI_AddElementValue(parent, "275");
		EDI_AddElementValue(parent, "276");
		EDI_AddElementValue(parent, "277");
		EDI_AddElementValue(parent, "278");
		EDI_AddElementValue(parent, "280");
		EDI_AddElementValue(parent, "283");
		EDI_AddElementValue(parent, "284");
		EDI_AddElementValue(parent, "285");
		EDI_AddElementValue(parent, "286");
		EDI_AddElementValue(parent, "288");
		EDI_AddElementValue(parent, "290");
		EDI_AddElementValue(parent, "300");
		EDI_AddElementValue(parent, "301");
		EDI_AddElementValue(parent, "303");
		EDI_AddElementValue(parent, "304");
		EDI_AddElementValue(parent, "309");
		EDI_AddElementValue(parent, "310");
		EDI_AddElementValue(parent, "311");
		EDI_AddElementValue(parent, "312");
		EDI_AddElementValue(parent, "313");
		EDI_AddElementValue(parent, "315");
		EDI_AddElementValue(parent, "317");
		EDI_AddElementValue(parent, "319");
		EDI_AddElementValue(parent, "322");
		EDI_AddElementValue(parent, "323");
		EDI_AddElementValue(parent, "324");
		EDI_AddElementValue(parent, "325");
		EDI_AddElementValue(parent, "326");
		EDI_AddElementValue(parent, "350");
		EDI_AddElementValue(parent, "352");
		EDI_AddElementValue(parent, "353");
		EDI_AddElementValue(parent, "354");
		EDI_AddElementValue(parent, "355");
		EDI_AddElementValue(parent, "356");
		EDI_AddElementValue(parent, "357");
		EDI_AddElementValue(parent, "358");
		EDI_AddElementValue(parent, "361");
		EDI_AddElementValue(parent, "362");
		EDI_AddElementValue(parent, "404");
		EDI_AddElementValue(parent, "410");
		EDI_AddElementValue(parent, "412");
		EDI_AddElementValue(parent, "414");
		EDI_AddElementValue(parent, "417");
		EDI_AddElementValue(parent, "418");
		EDI_AddElementValue(parent, "419");
		EDI_AddElementValue(parent, "420");
		EDI_AddElementValue(parent, "421");
		EDI_AddElementValue(parent, "422");
		EDI_AddElementValue(parent, "423");
		EDI_AddElementValue(parent, "424");
		EDI_AddElementValue(parent, "425");
		EDI_AddElementValue(parent, "426");
		EDI_AddElementValue(parent, "429");
		EDI_AddElementValue(parent, "431");
		EDI_AddElementValue(parent, "432");
		EDI_AddElementValue(parent, "433");
		EDI_AddElementValue(parent, "434");
		EDI_AddElementValue(parent, "435");
		EDI_AddElementValue(parent, "436");
		EDI_AddElementValue(parent, "437");
		EDI_AddElementValue(parent, "440");
		EDI_AddElementValue(parent, "451");
		EDI_AddElementValue(parent, "452");
		EDI_AddElementValue(parent, "453");
		EDI_AddElementValue(parent, "455");
		EDI_AddElementValue(parent, "456");
		EDI_AddElementValue(parent, "460");
		EDI_AddElementValue(parent, "463");
		EDI_AddElementValue(parent, "466");
		EDI_AddElementValue(parent, "468");
		EDI_AddElementValue(parent, "470");
		EDI_AddElementValue(parent, "475");
		EDI_AddElementValue(parent, "485");
		EDI_AddElementValue(parent, "486");
		EDI_AddElementValue(parent, "490");
		EDI_AddElementValue(parent, "492");
		EDI_AddElementValue(parent, "494");
		EDI_AddElementValue(parent, "500");
		EDI_AddElementValue(parent, "501");
		EDI_AddElementValue(parent, "503");
		EDI_AddElementValue(parent, "504");
		EDI_AddElementValue(parent, "511");
		EDI_AddElementValue(parent, "517");
		EDI_AddElementValue(parent, "521");
		EDI_AddElementValue(parent, "527");
		EDI_AddElementValue(parent, "536");
		EDI_AddElementValue(parent, "540");
		EDI_AddElementValue(parent, "561");
		EDI_AddElementValue(parent, "567");
		EDI_AddElementValue(parent, "568");
		EDI_AddElementValue(parent, "601");
		EDI_AddElementValue(parent, "602");
		EDI_AddElementValue(parent, "620");
		EDI_AddElementValue(parent, "625");
		EDI_AddElementValue(parent, "650");
		EDI_AddElementValue(parent, "715");
		EDI_AddElementValue(parent, "753");
		EDI_AddElementValue(parent, "754");
		EDI_AddElementValue(parent, "805");
		EDI_AddElementValue(parent, "806");
		EDI_AddElementValue(parent, "810");
		EDI_AddElementValue(parent, "811");
		EDI_AddElementValue(parent, "812");
		EDI_AddElementValue(parent, "813");
		EDI_AddElementValue(parent, "814");
		EDI_AddElementValue(parent, "815");
		EDI_AddElementValue(parent, "816");
		EDI_AddElementValue(parent, "818");
		EDI_AddElementValue(parent, "819");
		EDI_AddElementValue(parent, "820");
		EDI_AddElementValue(parent, "821");
		EDI_AddElementValue(parent, "822");
		EDI_AddElementValue(parent, "823");
		EDI_AddElementValue(parent, "824");
		EDI_AddElementValue(parent, "826");
		EDI_AddElementValue(parent, "827");
		EDI_AddElementValue(parent, "828");
		EDI_AddElementValue(parent, "829");
		EDI_AddElementValue(parent, "830");
		EDI_AddElementValue(parent, "831");
		EDI_AddElementValue(parent, "832");
		EDI_AddElementValue(parent, "833");
		EDI_AddElementValue(parent, "834");
		EDI_AddElementValue(parent, "835");
		EDI_AddElementValue(parent, "836");
		EDI_AddElementValue(parent, "837");
		EDI_AddElementValue(parent, "838");
		EDI_AddElementValue(parent, "839");
		EDI_AddElementValue(parent, "840");
		EDI_AddElementValue(parent, "841");
		EDI_AddElementValue(parent, "842");
		EDI_AddElementValue(parent, "843");
		EDI_AddElementValue(parent, "844");
		EDI_AddElementValue(parent, "845");
		EDI_AddElementValue(parent, "846");
		EDI_AddElementValue(parent, "847");
		EDI_AddElementValue(parent, "848");
		EDI_AddElementValue(parent, "849");
		EDI_AddElementValue(parent, "850");
		EDI_AddElementValue(parent, "851");
		EDI_AddElementValue(parent, "852");
		EDI_AddElementValue(parent, "853");
		EDI_AddElementValue(parent, "854");
		EDI_AddElementValue(parent, "855");
		EDI_AddElementValue(parent, "856");
		EDI_AddElementValue(parent, "857");
		EDI_AddElementValue(parent, "858");
		EDI_AddElementValue(parent, "859");
		EDI_AddElementValue(parent, "860");
		EDI_AddElementValue(parent, "861");
		EDI_AddElementValue(parent, "862");
		EDI_AddElementValue(parent, "863");
		EDI_AddElementValue(parent, "864");
		EDI_AddElementValue(parent, "865");
		EDI_AddElementValue(parent, "866");
		EDI_AddElementValue(parent, "867");
		EDI_AddElementValue(parent, "868");
		EDI_AddElementValue(parent, "869");
		EDI_AddElementValue(parent, "870");
		EDI_AddElementValue(parent, "871");
		EDI_AddElementValue(parent, "872");
		EDI_AddElementValue(parent, "873");
		EDI_AddElementValue(parent, "874");
		EDI_AddElementValue(parent, "875");
		EDI_AddElementValue(parent, "876");
		EDI_AddElementValue(parent, "877");
		EDI_AddElementValue(parent, "878");
		EDI_AddElementValue(parent, "879");
		EDI_AddElementValue(parent, "880");
		EDI_AddElementValue(parent, "881");
		EDI_AddElementValue(parent, "882");
		EDI_AddElementValue(parent, "883");
		EDI_AddElementValue(parent, "884");
		EDI_AddElementValue(parent, "885");
		EDI_AddElementValue(parent, "886");
		EDI_AddElementValue(parent, "887");
		EDI_AddElementValue(parent, "888");
		EDI_AddElementValue(parent, "889");
		EDI_AddElementValue(parent, "891");
		EDI_AddElementValue(parent, "893");
		EDI_AddElementValue(parent, "894");
		EDI_AddElementValue(parent, "895");
		EDI_AddElementValue(parent, "896");
		EDI_AddElementValue(parent, "920");
		EDI_AddElementValue(parent, "924");
		EDI_AddElementValue(parent, "925");
		EDI_AddElementValue(parent, "926");
		EDI_AddElementValue(parent, "928");
		EDI_AddElementValue(parent, "940");
		EDI_AddElementValue(parent, "943");
		EDI_AddElementValue(parent, "944");
		EDI_AddElementValue(parent, "945");
		EDI_AddElementValue(parent, "947");
		EDI_AddElementValue(parent, "980");
		EDI_AddElementValue(parent, "990");
		EDI_AddElementValue(parent, "993");
		EDI_AddElementValue(parent, "996");
		EDI_AddElementValue(parent, "997");
		EDI_AddElementValue(parent, "998");
		EDI_AddElementValue(parent, "999");
		EDI_CreateElementType(s, EDI_DATA_STRING, "329", 4, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "447", 1, 4);
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "479", 2, 2);
		EDI_AddElementValue(parent, "AA");
		EDI_AddElementValue(parent, "AB");
		EDI_AddElementValue(parent, "AC");
		EDI_AddElementValue(parent, "AD");
		EDI_AddElementValue(parent, "AE");
		EDI_AddElementValue(parent, "AF");
		EDI_AddElementValue(parent, "AG");
		EDI_AddElementValue(parent, "AH");
		EDI_AddElementValue(parent, "AI");
		EDI_AddElementValue(parent, "AK");
		EDI_AddElementValue(parent, "AL");
		EDI_AddElementValue(parent, "AM");
		EDI_AddElementValue(parent, "AN");
		EDI_AddElementValue(parent, "AO");
		EDI_AddElementValue(parent, "AP");
		EDI_AddElementValue(parent, "AQ");
		EDI_AddElementValue(parent, "AR");
		EDI_AddElementValue(parent, "AS");
		EDI_AddElementValue(parent, "AT");
		EDI_AddElementValue(parent, "AU");
		EDI_AddElementValue(parent, "AV");
		EDI_AddElementValue(parent, "AW");
		EDI_AddElementValue(parent, "AX");
		EDI_AddElementValue(parent, "AY");
		EDI_AddElementValue(parent, "AZ");
		EDI_AddElementValue(parent, "BA");
		EDI_AddElementValue(parent, "BB");
		EDI_AddElementValue(parent, "BC");
		EDI_AddElementValue(parent, "BD");
		EDI_AddElementValue(parent, "BE");
		EDI_AddElementValue(parent, "BF");
		EDI_AddElementValue(parent, "BL");
		EDI_AddElementValue(parent, "BS");
		EDI_AddElementValue(parent, "CA");
		EDI_AddElementValue(parent, "CB");
		EDI_AddElementValue(parent, "CC");
		EDI_AddElementValue(parent, "CD");
		EDI_AddElementValue(parent, "CE");
		EDI_AddElementValue(parent, "CF");
		EDI_AddElementValue(parent, "CH");
		EDI_AddElementValue(parent, "CI");
		EDI_AddElementValue(parent, "CJ");
		EDI_AddElementValue(parent, "CK");
		EDI_AddElementValue(parent, "CL");
		EDI_AddElementValue(parent, "CM");
		EDI_AddElementValue(parent, "CN");
		EDI_AddElementValue(parent, "CO");
		EDI_AddElementValue(parent, "CP");
		EDI_AddElementValue(parent, "CQ");
		EDI_AddElementValue(parent, "CR");
		EDI_AddElementValue(parent, "CS");
		EDI_AddElementValue(parent, "CT");
		EDI_AddElementValue(parent, "CU");
		EDI_AddElementValue(parent, "CV");
		EDI_AddElementValue(parent, "CW");
		EDI_AddElementValue(parent, "DA");
		EDI_AddElementValue(parent, "DD");
		EDI_AddElementValue(parent, "DF");
		EDI_AddElementValue(parent, "DI");
		EDI_AddElementValue(parent, "DM");
		EDI_AddElementValue(parent, "DS");
		EDI_AddElementValue(parent, "DX");
		EDI_AddElementValue(parent, "D3");
		EDI_AddElementValue(parent, "D4");
		EDI_AddElementValue(parent, "D5");
		EDI_AddElementValue(parent, "EC");
		EDI_AddElementValue(parent, "ED");
		EDI_AddElementValue(parent, "EI");
		EDI_AddElementValue(parent, "EN");
		EDI_AddElementValue(parent, "EP");
		EDI_AddElementValue(parent, "ER");
		EDI_AddElementValue(parent, "ES");
		EDI_AddElementValue(parent, "EV");
		EDI_AddElementValue(parent, "EX");
		EDI_AddElementValue(parent, "FA");
		EDI_AddElementValue(parent, "FB");
		EDI_AddElementValue(parent, "FC");
		EDI_AddElementValue(parent, "FG");
		EDI_AddElementValue(parent, "FR");
		EDI_AddElementValue(parent, "FT");
		EDI_AddElementValue(parent, "GC");
		EDI_AddElementValue(parent, "GE");
		EDI_AddElementValue(parent, "GF");
		EDI_AddElementValue(parent, "GL");
		EDI_AddElementValue(parent, "GP");
		EDI_AddElementValue(parent, "GR");
		EDI_AddElementValue(parent, "GT");
		EDI_AddElementValue(parent, "HB");
		EDI_AddElementValue(parent, "HC");
		EDI_AddElementValue(parent, "HI");
		EDI_AddElementValue(parent, "HN");
		EDI_AddElementValue(parent, "HP");
		EDI_AddElementValue(parent, "HR");
		EDI_AddElementValue(parent, "HS");
		EDI_AddElementValue(parent, "HU");
		EDI_AddElementValue(parent, "HV");
		EDI_AddElementValue(parent, "IA");
		EDI_AddElementValue(parent, "IB");
		EDI_AddElementValue(parent, "IC");
		EDI_AddElementValue(parent, "ID");
		EDI_AddElementValue(parent, "IE");
		EDI_AddElementValue(parent, "IF");
		EDI_AddElementValue(parent, "IG");
		EDI_AddElementValue(parent, "IH");
		EDI_AddElementValue(parent, "IJ");
		EDI_AddElementValue(parent, "IM");
		EDI_AddElementValue(parent, "IN");
		EDI_AddElementValue(parent, "IO");
		EDI_AddElementValue(parent, "IR");
		EDI_AddElementValue(parent, "IS");
		EDI_AddElementValue(parent, "JB");
		EDI_AddElementValue(parent, "KM");
		EDI_AddElementValue(parent, "LA");
		EDI_AddElementValue(parent, "LB");
		EDI_AddElementValue(parent, "LI");
		EDI_AddElementValue(parent, "LN");
		EDI_AddElementValue(parent, "LR");
		EDI_AddElementValue(parent, "LS");
		EDI_AddElementValue(parent, "LT");
		EDI_AddElementValue(parent, "MA");
		EDI_AddElementValue(parent, "MC");
		EDI_AddElementValue(parent, "MD");
		EDI_AddElementValue(parent, "ME");
		EDI_AddElementValue(parent, "MF");
		EDI_AddElementValue(parent, "MG");
		EDI_AddElementValue(parent, "MH");
		EDI_AddElementValue(parent, "MI");
		EDI_AddElementValue(parent, "MJ");
		EDI_AddElementValue(parent, "MK");
		EDI_AddElementValue(parent, "MM");
		EDI_AddElementValue(parent, "MN");
		EDI_AddElementValue(parent, "MO");
		EDI_AddElementValue(parent, "MP");
		EDI_AddElementValue(parent, "MQ");
		EDI_AddElementValue(parent, "MR");
		EDI_AddElementValue(parent, "MS");
		EDI_AddElementValue(parent, "MT");
		EDI_AddElementValue(parent, "MV");
		EDI_AddElementValue(parent, "MW");
		EDI_AddElementValue(parent, "MX");
		EDI_AddElementValue(parent, "MY");
		EDI_AddElementValue(parent, "MZ");
		EDI_AddElementValue(parent, "NC");
		EDI_AddElementValue(parent, "NL");
		EDI_AddElementValue(parent, "NP");
		EDI_AddElementValue(parent, "NR");
		EDI_AddElementValue(parent, "NT");
		EDI_AddElementValue(parent, "OC");
		EDI_AddElementValue(parent, "OG");
		EDI_AddElementValue(parent, "OR");
		EDI_AddElementValue(parent, "OW");
		EDI_AddElementValue(parent, "PA");
		EDI_AddElementValue(parent, "PB");
		EDI_AddElementValue(parent, "PC");
		EDI_AddElementValue(parent, "PD");
		EDI_AddElementValue(parent, "PE");
		EDI_AddElementValue(parent, "PF");
		EDI_AddElementValue(parent, "PG");
		EDI_AddElementValue(parent, "PH");
		EDI_AddElementValue(parent, "PI");
		EDI_AddElementValue(parent, "PJ");
		EDI_AddElementValue(parent, "PK");
		EDI_AddElementValue(parent, "PL");
		EDI_AddElementValue(parent, "PN");
		EDI_AddElementValue(parent, "PO");
		EDI_AddElementValue(parent, "PQ");
		EDI_AddElementValue(parent, "PR");
		EDI_AddElementValue(parent, "PS");
		EDI_AddElementValue(parent, "PT");
		EDI_AddElementValue(parent, "PU");
		EDI_AddElementValue(parent, "PV");
		EDI_AddElementValue(parent, "PW");
		EDI_AddElementValue(parent, "PY");
		EDI_AddElementValue(parent, "QG");
		EDI_AddElementValue(parent, "QM");
		EDI_AddElementValue(parent, "QO");
		EDI_AddElementValue(parent, "RA");
		EDI_AddElementValue(parent, "RB");
		EDI_AddElementValue(parent, "RC");
		EDI_AddElementValue(parent, "RD");
		EDI_AddElementValue(parent, "RE");
		EDI_AddElementValue(parent, "RF");
		EDI_AddElementValue(parent, "RG");
		EDI_AddElementValue(parent, "RH");
		EDI_AddElementValue(parent, "RI");
		EDI_AddElementValue(parent, "RJ");
		EDI_AddElementValue(parent, "RK");
		EDI_AddElementValue(parent, "RL");
		EDI_AddElementValue(parent, "RM");
		EDI_AddElementValue(parent, "RN");
		EDI_AddElementValue(parent, "RO");
		EDI_AddElementValue(parent, "RP");
		EDI_AddElementValue(parent, "RQ");
		EDI_AddElementValue(parent, "RR");
		EDI_AddElementValue(parent, "RS");
		EDI_AddElementValue(parent, "RT");
		EDI_AddElementValue(parent, "RU");
		EDI_AddElementValue(parent, "RV");
		EDI_AddElementValue(parent, "RW");
		EDI_AddElementValue(parent, "RX");
		EDI_AddElementValue(parent, "RY");
		EDI_AddElementValue(parent, "RZ");
		EDI_AddElementValue(parent, "SA");
		EDI_AddElementValue(parent, "SB");
		EDI_AddElementValue(parent, "SC");
		EDI_AddElementValue(parent, "SD");
		EDI_AddElementValue(parent, "SE");
		EDI_AddElementValue(parent, "SH");
		EDI_AddElementValue(parent, "SI");
		EDI_AddElementValue(parent, "SJ");
		EDI_AddElementValue(parent, "SL");
		EDI_AddElementValue(parent, "SM");
		EDI_AddElementValue(parent, "SN");
		EDI_AddElementValue(parent, "SO");
		EDI_AddElementValue(parent, "SP");
		EDI_AddElementValue(parent, "SQ");
		EDI_AddElementValue(parent, "SR");
		EDI_AddElementValue(parent, "SS");
		EDI_AddElementValue(parent, "ST");
		EDI_AddElementValue(parent, "SU");
		EDI_AddElementValue(parent, "SV");
		EDI_AddElementValue(parent, "SW");
		EDI_AddElementValue(parent, "TA");
		EDI_AddElementValue(parent, "TB");
		EDI_AddElementValue(parent, "TD");
		EDI_AddElementValue(parent, "TE");
		EDI_AddElementValue(parent, "TF");
		EDI_AddElementValue(parent, "TI");
		EDI_AddElementValue(parent, "TJ");
		EDI_AddElementValue(parent, "TM");
		EDI_AddElementValue(parent, "TN");
		EDI_AddElementValue(parent, "TO");
		EDI_AddElementValue(parent, "TP");
		EDI_AddElementValue(parent, "TR");
		EDI_AddElementValue(parent, "TS");
		EDI_AddElementValue(parent, "TT");
		EDI_AddElementValue(parent, "TU");
		EDI_AddElementValue(parent, "TX");
		EDI_AddElementValue(parent, "UA");
		EDI_AddElementValue(parent, "UB");
		EDI_AddElementValue(parent, "UC");
		EDI_AddElementValue(parent, "UD");
		EDI_AddElementValue(parent, "UI");
		EDI_AddElementValue(parent, "UP");
		EDI_AddElementValue(parent, "UW");
		EDI_AddElementValue(parent, "VA");
		EDI_AddElementValue(parent, "VB");
		EDI_AddElementValue(parent, "VC");
		EDI_AddElementValue(parent, "VD");
		EDI_AddElementValue(parent, "VE");
		EDI_AddElementValue(parent, "VH");
		EDI_AddElementValue(parent, "VI");
		EDI_AddElementValue(parent, "VS");
		EDI_AddElementValue(parent, "WA");
		EDI_AddElementValue(parent, "WB");
		EDI_AddElementValue(parent, "WG");
		EDI_AddElementValue(parent, "WI");
		EDI_AddElementValue(parent, "WL");
		EDI_AddElementValue(parent, "WR");
		EDI_AddElementValue(parent, "WT");
		EDI_CreateElementType(s, EDI_DATA_STRING, "480", 1, 12);
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "715", 1, 1);
		EDI_AddElementValue(parent, "A");
		EDI_AddElementValue(parent, "E");
		EDI_AddElementValue(parent, "M");
		EDI_AddElementValue(parent, "P");
		EDI_AddElementValue(parent, "R");
		EDI_AddElementValue(parent, "W");
		EDI_AddElementValue(parent, "X");
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "716", 1, 3);
		EDI_AddElementValue(parent, "1");
		EDI_AddElementValue(parent, "2");
		EDI_AddElementValue(parent, "3");
		EDI_AddElementValue(parent, "4");
		EDI_AddElementValue(parent, "5");
		EDI_AddElementValue(parent, "6");
		EDI_AddElementValue(parent, "10");
		EDI_AddElementValue(parent, "11");
		EDI_AddElementValue(parent, "12");
		EDI_AddElementValue(parent, "13");
		EDI_AddElementValue(parent, "14");
		EDI_AddElementValue(parent, "15");
		EDI_AddElementValue(parent, "16");
		EDI_AddElementValue(parent, "17");
		EDI_AddElementValue(parent, "18");
		EDI_AddElementValue(parent, "19");
		EDI_AddElementValue(parent, "23");
		EDI_AddElementValue(parent, "24");
		EDI_AddElementValue(parent, "25");
		EDI_AddElementValue(parent, "26");
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "717", 1, 1);
		EDI_AddElementValue(parent, "A");
		EDI_AddElementValue(parent, "E");
		EDI_AddElementValue(parent, "M");
		EDI_AddElementValue(parent, "R");
		EDI_AddElementValue(parent, "W");
		EDI_AddElementValue(parent, "X");
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "718", 1, 3);
		EDI_AddElementValue(parent, "1");
		EDI_AddElementValue(parent, "2");
		EDI_AddElementValue(parent, "3");
		EDI_AddElementValue(parent, "4");
		EDI_AddElementValue(parent, "5");
		EDI_AddElementValue(parent, "6");
		EDI_AddElementValue(parent, "7");
		EDI_AddElementValue(parent, "8");
		EDI_AddElementValue(parent, "9");
		EDI_AddElementValue(parent, "10");
		EDI_AddElementValue(parent, "11");
		EDI_AddElementValue(parent, "12");
		EDI_AddElementValue(parent, "13");
		EDI_AddElementValue(parent, "15");
		EDI_AddElementValue(parent, "16");
		EDI_AddElementValue(parent, "17");
		EDI_AddElementValue(parent, "18");
		EDI_AddElementValue(parent, "19");
		EDI_AddElementValue(parent, "23");
		EDI_AddElementValue(parent, "24");
		EDI_AddElementValue(parent, "25");
		EDI_AddElementValue(parent, "26");
		EDI_AddElementValue(parent, "27");
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "719", 1, 10);
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "720", 1, 3);
		EDI_AddElementValue(parent, "1");
		EDI_AddElementValue(parent, "2");
		EDI_AddElementValue(parent, "3");
		EDI_AddElementValue(parent, "4");
		EDI_AddElementValue(parent, "5");
		EDI_AddElementValue(parent, "6");
		EDI_AddElementValue(parent, "7");
		EDI_AddElementValue(parent, "8");
		EDI_CreateElementType(s, EDI_DATA_STRING, "721", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "722", 1, 2);
		parent = EDI_CreateElementType(s, EDI_DATA_STRING, "723", 1, 3);
		EDI_AddElementValue(parent, "1");
		EDI_AddElementValue(parent, "2");
		EDI_AddElementValue(parent, "3");
		EDI_AddElementValue(parent, "4");
		EDI_AddElementValue(parent, "5");
		EDI_AddElementValue(parent, "6");
		EDI_AddElementValue(parent, "7");
		EDI_AddElementValue(parent, "8");
		EDI_AddElementValue(parent, "9");
		EDI_AddElementValue(parent, "10");
		EDI_AddElementValue(parent, "12");
		EDI_AddElementValue(parent, "13");
		EDI_CreateElementType(s, EDI_DATA_STRING, "724", 1, 99);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "725", 1, 4);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1528", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1686", 1, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1705", 1, 35);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C030");
		EDI_AppendType(parent, EDI_GetElementByID(s, "722"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "1528"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "1686"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "ST");
		EDI_AppendType(parent, EDI_GetElementByID(s, "143"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK1");
		EDI_AppendType(parent, EDI_GetElementByID(s, "479"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "28"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "480"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK2");
		EDI_AppendType(parent, EDI_GetElementByID(s, "143"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "1705"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK3");
		EDI_AppendType(parent, EDI_GetElementByID(s, "721"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "719"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "447"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "720"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK4");
		EDI_AppendType(parent, EDI_GetComplexNodeByID(s, "C030"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "725"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "723"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "724"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK5");
		EDI_AppendType(parent, EDI_GetElementByID(s, "717"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "718"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AK9");
		EDI_AppendType(parent, EDI_GetElementByID(s, "715"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "97"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "123"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "2"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "716"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SE");
		EDI_AppendType(parent, EDI_GetElementByID(s, "96"), 1, 1);
		EDI_AppendType(parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		EDI_SchemaNode root = EDI_CreateComplexType(s, EDITYPE_TRANSACTION, "FA997-005010X230");
		EDI_AppendType(root, EDI_GetComplexNodeByID(s, "ST"), 1, 1);
		EDI_AppendType(root, EDI_GetComplexNodeByID(s, "AK1"), 1, 1);
		EDI_SchemaNode loop2000 = EDI_CreateComplexType(s, EDITYPE_LOOP, "loop2000");
		EDI_AppendType(loop2000, EDI_GetComplexNodeByID(s, "AK2"), 1, 1);
		EDI_SchemaNode loop2100 = EDI_CreateComplexType(s, EDITYPE_LOOP, "loop2100");
		EDI_AppendType(loop2100, EDI_GetComplexNodeByID(s, "AK3"), 1, 1);
		EDI_AppendType(loop2100, EDI_GetComplexNodeByID(s, "AK4"), 0, 99);
		EDI_StoreComplexNode(s, loop2100);
		EDI_AppendType(loop2000, loop2100, 0, 999999);
		EDI_AppendType(loop2000, EDI_GetComplexNodeByID(s, "AK5"), 1, 1);
		EDI_StoreComplexNode(s, loop2000);
		EDI_AppendType(root, loop2000, 0, 999999);
		EDI_AppendType(root, EDI_GetComplexNodeByID(s, "AK9"), 1, 1);
		EDI_AppendType(root, EDI_GetComplexNodeByID(s, "SE"), 1, 1);
		
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
