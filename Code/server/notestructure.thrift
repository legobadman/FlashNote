struct Note {
	1:required string id;
	2:required string title;
	3:required string text_abbre; //ÎÄ±¾ËõÂÔ
}

struct Notebook {
	1:required list<Note> notes;
	2:required string name;
}

service UserServer {
	i32 GetUserId(1:required string username);
}

service NoteServer {
	list<Notebook> GetNotebooks(1:required string userid);
	string GetContent(1:required string noteid);
}