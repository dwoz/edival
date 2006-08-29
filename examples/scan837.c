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

//gcc scan837.c -I../include -L../lib -ledival -o scan837

#include <edival.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFF_SIZE 16777216

int group_start = 0;
int counter = 0;

void load_standard(EDI_Parser p)
{
	EDI_Schema s = NULL;
	EDI_SchemaNode parent = NULL;
	EDI_SchemaNode child  = NULL;
	
	s = EDI_GetSchema(p);
	if(s && strcmp(EDI_GetSchemaId(s), "Health Care Claim: HC/837") != 0){
		s = EDI_RemoveSchema(p);
		EDI_SchemaFree(s);
	}
	if(!s){
		s = EDI_SchemaCreate();
		EDI_SetSchemaId(s, "Health Care Claim: HC/837");
		EDI_SetSchema(p, s);

		EDI_CreateElementType(s, EDI_DATA_STRING, "143", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "329", 4, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1005", 4, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "353", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "127", 1, 30);
		EDI_CreateElementType(s, EDI_DATA_DATE, "373", 6, 8);
		EDI_CreateElementType(s, EDI_DATA_TIME, "337", 4, 8);
		EDI_CreateElementType(s, EDI_DATA_STRING, "640", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "128", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "352", 1, 80);
		EDI_CreateElementType(s, EDI_DATA_STRING, "98", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1065", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1035", 1, 35);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1036", 1, 25);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1037", 1, 25);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1038", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1039", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "66", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "67", 2, 80);
		EDI_CreateElementType(s, EDI_DATA_STRING, "706", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "93", 1, 60);
		EDI_CreateElementType(s, EDI_DATA_STRING, "166", 1, 55);
		EDI_CreateElementType(s, EDI_DATA_STRING, "19", 2, 30);
		EDI_CreateElementType(s, EDI_DATA_STRING, "156", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "116", 3, 15);
		EDI_CreateElementType(s, EDI_DATA_STRING, "26", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "309", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "310", 1, 30);
		EDI_CreateElementType(s, EDI_DATA_STRING, "366", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "365", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "364", 1, 80);
		EDI_CreateElementType(s, EDI_DATA_STRING, "443", 1, 20);
		EDI_CreateElementType(s, EDI_DATA_STRING, "628", 1, 12);
		EDI_CreateElementType(s, EDI_DATA_STRING, "734", 1, 12);
		EDI_CreateElementType(s, EDI_DATA_STRING, "735", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "736", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1221", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1222", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "559", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1073", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1223", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1138", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1069", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1336", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1143", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "584", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1032", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1384", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1220", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1250", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1251", 1, 35);
		EDI_CreateElementType(s, EDI_DATA_STRING, "355", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "81", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "374", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "100", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "280", 4, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "669", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1068", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1067", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1109", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1066", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "659", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "380", 1, 15);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1028", 1, 38);
		EDI_CreateElementType(s, EDI_DATA_DECIMAL, "782", 1, 18);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1343", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1331", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1332", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1325", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1359", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1363", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1351", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1362", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1366", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1338", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1360", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1029", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1383", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1514", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1315", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1314", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1352", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1345", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1368", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "755", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "756", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "757", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "704", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1525", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1166", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "332", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "338", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "799", 1, 30);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1146", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1149", 4, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1154", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1161", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "235", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1137", 1, 15);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1318", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "522", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "478", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "449", 1, 80);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1333", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1018", 1, 15);
		EDI_CreateElementType(s, EDI_DATA_STRING, "649", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "363", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1316", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1317", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "609", 1, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1367", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1342", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1322", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1335", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1344", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "65", 1, 8);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1346", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1347", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "954", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1348", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1349", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1350", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1382", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "923", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1403", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1404", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1136", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1321", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1270", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1271", 1, 30);
		EDI_CreateElementType(s, EDI_DATA_STRING, "673", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "61", 1, 30);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1473", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "118", 1, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "234", 1, 48);
		EDI_CreateElementType(s, EDI_DATA_STRING, "901", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1526", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1527", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "921", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1470", 1, 9);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1167", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "615", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "616", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "678", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "679", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1033", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1034", 1, 5);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "554", 1, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1339", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1365", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "1328", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1340", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1364", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1341", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1327", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1334", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1337", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1371", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1361", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1358", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1369", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1329", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1356", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1370", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1319", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1320", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1330", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1357", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "594", 1, 1);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1355", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1207", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "750", 2, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "737", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "738", 1, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "739", 1, 20);
		EDI_CreateElementType(s, EDI_DATA_STRING, "740", 1, 20);
		EDI_CreateElementType(s, EDI_DATA_STRING, "741", 1, 20);
		EDI_CreateElementType(s, EDI_DATA_STRING, "935", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "936", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "752", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1373", 2, 4);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1253", 3, 6);
		EDI_CreateElementType(s, EDI_DATA_STRING, "1254", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "350", 1, 20);
		EDI_CreateElementType(s, EDI_DATA_STRING, "687", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "236", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "212", 1, 17);
		EDI_CreateElementType(s, EDI_DATA_STRING, "648", 3, 3);
		EDI_CreateElementType(s, EDI_DATA_STRING, "639", 2, 2);
		EDI_CreateElementType(s, EDI_DATA_STRING, "499", 1, 10);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "289", 1, 2);
		EDI_CreateElementType(s, EDI_DATA_INTEGER, "96", 1, 10);
		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C040");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "128"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "128"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "128"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C035");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1222"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "559"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C023");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1331"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1332"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1325"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C024");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1362"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1362"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1362"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "156"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "26"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C002");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "704"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "704"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "704"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "704"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "704"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C001");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1018"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1018"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1018"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1018"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1018"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C022");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1270"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1271"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "799"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C003");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1339"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1339"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1339"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1339"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C004");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1328"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1328"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1328"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1328"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C006");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1361"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1361"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1361"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1361"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1361"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_COMPOSITE, "C005");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1369"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1369"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1369"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1369"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1369"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "ST");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "143"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "BHT");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1005"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "353"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "640"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "REF");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "128"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C040"), 0, 1);
		EDI_AddSyntaxNote(s, parent, EDI_SYNTAX_REQUIRED, 2, (int[]){2, 3});
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "NM1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "98"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1065"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1035"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1036"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1037"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1038"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1039"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "66"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "67"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "706"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "98"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "N2");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "93"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "93"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "N3");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "166"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "166"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "N4");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "19"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "156"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "116"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "26"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "309"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "310"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "PER");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "366"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "93"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "365"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "364"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "365"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "364"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "365"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "364"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "443"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "HL");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "628"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "734"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "735"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "736"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "PRV");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1221"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "128"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "156"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C035"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1223"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SBR");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1138"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1069"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "93"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1336"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1143"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "584"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1032"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "PAT");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1069"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1384"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "584"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1220"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "81"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "DTP");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CUR");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "98"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "100"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "280"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "98"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "100"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "669"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "374"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "337"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "DMG");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1068"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1067"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1109"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1066"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "26"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "659"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CLM");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1028"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1032"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1343"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C023"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1359"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1363"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1351"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C024"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1366"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1338"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1360"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1029"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1383"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1514"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CL1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1315"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1314"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1345"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "DN1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "DN2");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1368"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "PWK");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "755"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "756"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "757"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "98"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "66"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "67"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C002"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1525"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CN1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1166"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "332"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "338"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "799"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "DSB");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1146"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1149"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1154"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1161"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1137"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "UR");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1318"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "AMT");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "522"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "478"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "K3");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "449"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1333"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C001"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "NTE");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "363"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "81"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1316"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1317"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "166"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "166"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR2");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "609"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1367"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1367"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1342"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR3");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1322"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1335"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR4");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1322"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1344"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "65"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "81"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1346"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1347"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "954"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "954"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "954"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR5");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1322"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1348"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1348"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1349"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1350"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1350"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1350"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1382"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1348"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR6");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "923"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1322"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1137"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1384"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR8");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1403"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1404"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "373"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CRC");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1136"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1321"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1321"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1321"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1321"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1321"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "HI");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C022"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "QTY");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "673"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C001"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "61"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "HCP");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1473"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "118"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "901"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1526"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1527"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CR7");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "921"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1470"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1470"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "HSD");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "673"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1167"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "615"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "616"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "678"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "679"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CAS");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1033"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1034"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "OI");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1032"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1383"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1351"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1360"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1363"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "MIA");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "MOA");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "954"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "LX");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "554"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV1");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1331"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1365"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C004"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1340"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1364"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1341"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1327"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1334"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "116"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1360"), 0, 1);
		EDI_AddSyntaxNote(s, parent, EDI_SYNTAX_PAIRED, 2, (int[]){3,4});
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV2");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1371"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1345"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1337"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV3");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1331"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C006"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1358"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1327"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1360"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C004"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "TOO");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1270"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1271"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C005"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV4");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1329"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1338"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1356"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "352"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1370"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1319"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1320"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1330"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1327"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1384"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1337"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1357"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV5");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "355"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "594"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "923"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV6");
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1332"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1331"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C004"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SV7");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1355"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1207"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "750"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1073"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "MEA");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "737"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "738"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "739"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C001"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "740"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "741"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "935"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "936"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "752"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1373"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "PS1");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "127"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "156"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "IMM");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1253"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1250"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1251"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "1254"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "755"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "LIN");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "350"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "235"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "CTP");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "687"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "236"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "212"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C001"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "648"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "649"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "639"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "499"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "289"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SVD");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "67"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "782"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetComplexNodeByID(s, "C003"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "234"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "380"), 0, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "554"), 0, 1);
		EDI_StoreComplexNode(s, parent);

		parent = EDI_CreateComplexType(s, EDITYPE_SEGMENT, "SE");
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "96"), 1, 1);
		EDI_AppendType(s, parent, EDI_GetElementByID(s, "329"), 1, 1);
		EDI_StoreComplexNode(s, parent);

		EDI_SchemaNode loop1 = EDI_CreateComplexType(s, EDITYPE_TRANSACTION, "ANSI837");
		EDI_AppendType(s, loop1, EDI_GetComplexNodeByID(s, "ST"), 1, 1);
		EDI_AppendType(s, loop1, EDI_GetComplexNodeByID(s, "BHT"), 1, 1);
		EDI_AppendType(s, loop1, EDI_GetComplexNodeByID(s, "REF"), 0, 3);
		EDI_SchemaNode loop410 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop410");
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "NM1"), 1, 1);
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "N2"), 0, 2);
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "N3"), 0, 2);
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "N4"), 0, 1);
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "REF"), 0, 2);
		EDI_AppendType(s, loop410, EDI_GetComplexNodeByID(s, "PER"), 0, 2);
		EDI_StoreComplexNode(s, loop410);
		EDI_AppendType(s, loop1, loop410, 0, 10);
		EDI_SchemaNode loop1010 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop1010");
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "HL"), 1, 1);
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "PRV"), 0, 1);
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "SBR"), 0, 1);
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "PAT"), 0, 1);
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "DTP"), 0, 5);
		EDI_AppendType(s, loop1010, EDI_GetComplexNodeByID(s, "CUR"), 0, 1);
		EDI_SchemaNode loop1610 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop1610");
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "NM1"), 1, 1);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "N2"), 0, 2);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "N3"), 0, 2);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "N4"), 0, 1);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "DMG"), 0, 1);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "REF"), 0, 20);
		EDI_AppendType(s, loop1610, EDI_GetComplexNodeByID(s, "PER"), 0, 2);
		EDI_StoreComplexNode(s, loop1610);
		EDI_AppendType(s, loop1010, loop1610, 0, 10);
		EDI_SchemaNode loop2310 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop2310");
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CLM"), 1, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "DTP"), 0, 150);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CL1"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "DN1"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "DN2"), 0, 35);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "PWK"), 0, 10);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CN1"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "DSB"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "UR"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "AMT"), 0, 40);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "REF"), 0, 30);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "K3"), 0, 10);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "NTE"), 0, 20);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR1"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR2"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR3"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR4"), 0, 3);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR5"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR6"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CR8"), 0, 1);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "CRC"), 0, 100);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "HI"), 0, 25);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "QTY"), 0, 10);
		EDI_AppendType(s, loop2310, EDI_GetComplexNodeByID(s, "HCP"), 0, 1);
		EDI_SchemaNode loop4710 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop4710");
		EDI_AppendType(s, loop4710, EDI_GetComplexNodeByID(s, "CR7"), 1, 1);
		EDI_AppendType(s, loop4710, EDI_GetComplexNodeByID(s, "HSD"), 0, 12);
		EDI_StoreComplexNode(s, loop4710);
		EDI_AppendType(s, loop2310, loop4710, 0, 6);
		EDI_SchemaNode loop4910 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop4910");
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "NM1"), 1, 1);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "PRV"), 0, 1);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "N2"), 0, 2);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "N3"), 0, 2);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "N4"), 0, 1);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "REF"), 0, 20);
		EDI_AppendType(s, loop4910, EDI_GetComplexNodeByID(s, "PER"), 0, 2);
		EDI_StoreComplexNode(s, loop4910);
		EDI_AppendType(s, loop2310, loop4910, 0, 9);
		EDI_SchemaNode loop5610 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop5610");
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "SBR"), 1, 1);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "CAS"), 0, 99);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "AMT"), 0, 15);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "DMG"), 0, 1);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "OI"), 0, 1);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "MIA"), 0, 1);
		EDI_AppendType(s, loop5610, EDI_GetComplexNodeByID(s, "MOA"), 0, 1);
		EDI_SchemaNode loop6310 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop6310");
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "NM1"), 1, 1);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "N2"), 0, 2);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "N3"), 0, 2);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "N4"), 0, 1);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "PER"), 0, 2);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "DTP"), 0, 9);
		EDI_AppendType(s, loop6310, EDI_GetComplexNodeByID(s, "REF"), 0, 3);
		EDI_StoreComplexNode(s, loop6310);
		EDI_AppendType(s, loop5610, loop6310, 0, 10);
		EDI_StoreComplexNode(s, loop5610);
		EDI_AppendType(s, loop2310, loop5610, 0, 10);
		EDI_SchemaNode loop7010 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop7010");
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "LX"), 1, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV1"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV2"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV3"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "TOO"), 0, 32);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV4"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV5"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV6"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "SV7"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "HI"), 0, 25);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "PWK"), 0, 10);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CR1"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CR2"), 0, 5);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CR3"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CR4"), 0, 3);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CR5"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CRC"), 0, 3);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "DTP"), 0, 15);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "QTY"), 0, 5);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "MEA"), 0, 20);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "CN1"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "REF"), 0, 30);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "AMT"), 0, 15);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "K3"), 0, 10);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "NTE"), 0, 10);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "PS1"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "IMM"), 0, 999999);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "HSD"), 0, 1);
		EDI_AppendType(s, loop7010, EDI_GetComplexNodeByID(s, "HCP"), 0, 1);
		EDI_SchemaNode loop9910 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop9910");
		EDI_AppendType(s, loop9910, EDI_GetComplexNodeByID(s, "LIN"), 1, 1);
		EDI_AppendType(s, loop9910, EDI_GetComplexNodeByID(s, "CTP"), 0, 1);
		EDI_AppendType(s, loop9910, EDI_GetComplexNodeByID(s, "REF"), 0, 1);
		EDI_StoreComplexNode(s, loop9910);
		EDI_AppendType(s, loop7010, loop9910, 0, 999999);
		EDI_SchemaNode loop10210 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop10210");
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "NM1"), 1, 1);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "PRV"), 0, 1);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "N2"), 0, 2);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "N3"), 0, 2);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "N4"), 0, 1);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "REF"), 0, 20);
		EDI_AppendType(s, loop10210, EDI_GetComplexNodeByID(s, "PER"), 0, 2);
		EDI_StoreComplexNode(s, loop10210);
		EDI_AppendType(s, loop7010, loop10210, 0, 10);
		EDI_SchemaNode loop10910 = EDI_CreateComplexType(s, EDITYPE_LOOP, "Loop10910");
		EDI_AppendType(s, loop10910, EDI_GetComplexNodeByID(s, "SVD"), 1, 1);
		EDI_AppendType(s, loop10910, EDI_GetComplexNodeByID(s, "CAS"), 0, 99);
		EDI_AppendType(s, loop10910, EDI_GetComplexNodeByID(s, "DTP"), 0, 9);
		EDI_StoreComplexNode(s, loop10910);
		EDI_AppendType(s, loop7010, loop10910, 0, 999999);
		EDI_StoreComplexNode(s, loop7010);
		EDI_AppendType(s, loop2310, loop7010, 0, 999999);
		EDI_StoreComplexNode(s, loop2310);
		EDI_AppendType(s, loop1010, loop2310, 0, 100);
		EDI_StoreComplexNode(s, loop1010);
		EDI_AppendType(s, loop1, loop1010, 1, 999999);
		EDI_AppendType(s, loop1, EDI_GetComplexNodeByID(s, "SE"), 1, 1);
	}
	return;
}

void handleSegmentStart(void *myData, const char *tag, int offset)
{
	//fprintf(stderr, "Seg: %3s ->", tag);
	if(strcmp("GS", tag) == 0){
		group_start = 1;
		counter = 0;
	}
	return;
}

void handleSegmentEnd(void *myData, const char *tag)
{
    //fprintf(stderr, " <- End: %s\n", tag);
    return;
}

void handleElement(void *myData, const char *val, int position)
{
	//fprintf(stderr, "[%s]", val);
	if(group_start){
		counter++;
		if(counter == 1){
			if(strcmp("HC", val) == 0){
				load_standard((EDI_Parser)myData);
			} else {
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
    //fprintf(stderr, "{%s}", val);
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
