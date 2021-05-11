typedef i64 Timestamp

struct Note {
	1:required string id;
	2:required string title;
	3:required string text_abbre; //文本缩略
	4:required string creater_id;
	5:required Timestamp create_time;
	6:required Timestamp modify_time;
	7:required bool share;	//共享属性
}

struct Notebook {
	1:required string id;
	2:required list<Note> notes;
	3:required string name;
	4:required Timestamp create_time;
	5:required Timestamp modify_time;
	6:required string creater_id;
	7:required bool share;	//共享属性
}

service UserInfo {
	string GetUserId(1:required string username);
}

service NoteInfo {
	list<Notebook> GetNotebooks(1:required string userid);

	string NewNotebook(1:required string userid, 2:required string name); //返回id
	
	/*删除笔记本
	 1. 对于非共享的笔记本，删除笔记本会将该笔记本下所有的笔记移除到废纸篓。
	 2. 对于共享的笔记本，删除笔记本只是从用户的笔记本列表中移除，如果所有用户都移除，那么该笔记本将游离（暂不处理）
	 */
	bool DeleteNotebook(1:required string userid, 2:required string bookid);

	/*创建笔记
	 指定bookid，创建标题为title的（非共享）笔记。
	 */
	string NewNote(1:required string userid, 2:required string bookid, 3:required string title);		//返回note-id
	
	/* 更新笔记内容 */
	bool UpdateNote(1:required string noteid, 2:required string title, 3:required string note);	//更新笔记内容。

	/* 从服务端获取给定note的全部内容 */
	string GetContent(1:required string noteid);

	/* 将笔记从src_bookid指定的book，移动到dest_bookid指定的book。*/
	bool MoveNote(1:required string noteid, 2:required string src_bookid, 3:required string dest_bookid);

	/* 将note移入垃圾桶
	  仅对userid用户下bookid笔记本下的笔记noteid，移入属于userid的废纸篓，不影响该noteid在其他用户下的安放。
	  Trash的内部构造如下：
	  [
	    {"_id":<trash-id-by-mongodb>, "note_id":<note-id>, "user_id":<user-id>,
			"oringal_book_id":<book-id>, "trash_time":<date>} 

		...
	  ]
	 
	 */
	bool TrashNote(1:required string userid, 2:required string bookid, 3:required string noteid);

	// 将userid的废纸篓中的noteid笔记恢复。
	bool RecoverNote(1:required string userid, 2:required string noteid);

	// 将noteid的笔记彻底销毁，但不需要立即移除包含该noteid的笔记本。（可以惰性销毁）
	bool DeleteNote(1:required string noteid);
}
