#ifndef __SQL_CONST_H__
#define __SQL_CONST_H__

#define TABLE_NAME_NOTE		"NOTE"
#define TABLE_NAME_NOTEBOOK	"NOTEBOOK"
#define TABLE_NAME_TRASH	"TRASH"
#define TABLE_NAME_SCHEDULE	"SCHEDULES"

const char* SQL_CREATE_NOTE = 
	"CREATE TABLE NOTE("
	"	ID				TEXT,"
	"	create_time		TEXT,"
	"	modify_time		TEXT,"
	"	title			TEXT	DEFAULT '',"
	"	content			TEXT	DEFAULT '',"
	"	type			INT		DEFAULT 0"	//0:normal		1:mindmap		2:chedule
	");";

const char* SQL_REMOVE_NOTE = "DROP TABLE NOTE;";

const char* SQL_CREATE_NOTEBOOK = 
	"CREATE TABLE NOTEBOOK("
	"	ID				TEXT,"
	"	create_time		TEXT,"
	"	modify_time		TEXT,"
	"	name			TEXT,"
	"	notes			TEXT DEFAULT ''"	//split by |
	");";

const char* SQL_REMOVE_NOTEBOOK = "DROP TABLE NOTEBOOK;";

const char* SQL_CREATE_TRASH =
	"CREATE TABLE TRASH("
	"	ID				TEXT,"
	"	srcbook_id		TEXT,"
	"	note_id			TEXT,"
	"	trash_time		TEXT"
	");";

const char* SQL_REMOVE_TRASH = "DROP TABLE TRASH;";

const char* SQL_CREATE_SCHEDULES =
	"CREATE TABLE SCHEDULES("
	"	note_id			TEXT"
	");";

const char* SQL_REMOVE_SCHEDULES = "DROP TABLE SCHEDULES;";

#endif