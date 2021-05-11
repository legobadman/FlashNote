typedef i64 Timestamp

struct Note {
	1:required string id;
	2:required string title;
	3:required string text_abbre; //�ı�����
	4:required string creater_id;
	5:required Timestamp create_time;
	6:required Timestamp modify_time;
	7:required bool share;	//��������
}

struct Notebook {
	1:required string id;
	2:required list<Note> notes;
	3:required string name;
	4:required Timestamp create_time;
	5:required Timestamp modify_time;
	6:required string creater_id;
	7:required bool share;	//��������
}

service UserInfo {
	string GetUserId(1:required string username);
}

service NoteInfo {
	list<Notebook> GetNotebooks(1:required string userid);

	string NewNotebook(1:required string userid, 2:required string name); //����id
	
	/*ɾ���ʼǱ�
	 1. ���ڷǹ���ıʼǱ���ɾ���ʼǱ��Ὣ�ñʼǱ������еıʼ��Ƴ�����ֽ¨��
	 2. ���ڹ���ıʼǱ���ɾ���ʼǱ�ֻ�Ǵ��û��ıʼǱ��б����Ƴ�����������û����Ƴ�����ô�ñʼǱ������루�ݲ�����
	 */
	bool DeleteNotebook(1:required string userid, 2:required string bookid);

	/*�����ʼ�
	 ָ��bookid����������Ϊtitle�ģ��ǹ����ʼǡ�
	 */
	string NewNote(1:required string userid, 2:required string bookid, 3:required string title);		//����note-id
	
	/* ���±ʼ����� */
	bool UpdateNote(1:required string noteid, 2:required string title, 3:required string note);	//���±ʼ����ݡ�

	/* �ӷ���˻�ȡ����note��ȫ������ */
	string GetContent(1:required string noteid);

	/* ���ʼǴ�src_bookidָ����book���ƶ���dest_bookidָ����book��*/
	bool MoveNote(1:required string noteid, 2:required string src_bookid, 3:required string dest_bookid);

	/* ��note��������Ͱ
	  ����userid�û���bookid�ʼǱ��µıʼ�noteid����������userid�ķ�ֽ¨����Ӱ���noteid�������û��µİ��š�
	  Trash���ڲ��������£�
	  [
	    {"_id":<trash-id-by-mongodb>, "note_id":<note-id>, "user_id":<user-id>,
			"oringal_book_id":<book-id>, "trash_time":<date>} 

		...
	  ]
	 
	 */
	bool TrashNote(1:required string userid, 2:required string bookid, 3:required string noteid);

	// ��userid�ķ�ֽ¨�е�noteid�ʼǻָ���
	bool RecoverNote(1:required string userid, 2:required string noteid);

	// ��noteid�ıʼǳ������٣�������Ҫ�����Ƴ�������noteid�ıʼǱ��������Զ������٣�
	bool DeleteNote(1:required string noteid);
}
