typedef i32 Timestamp

struct Note {
	1:required string id;
	2:required string title;
	3:required string text_abbre; //ÎÄ±¾ËõÂÔ
	4:required string notebook_id;
	5:required string creater_id;
	6:required list<string> owners;
	7:required Timestamp create_time;
	8:required Timestamp modify_time;
}

struct Notebook {
	1:required string id;
	2:required list<Note> notes;
	3:required string name;
	4:required Timestamp create_time;
	5:required Timestamp modify_time;
	6:required string creater;
}

service UserInfo {
	string GetUserId(1:required string username);
}

service NoteInfo {
	list<Notebook> GetNotebooks(1:required string userid);
	string GetContent(1:required string noteid);
}
