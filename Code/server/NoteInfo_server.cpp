// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/NoteInfo.h"
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using boost::shared_ptr;

mongocxx::instance inst{};
mongocxx::client conn{ mongocxx::uri{} };

typedef mongocxx::stdx::optional<mongocxx::result::insert_one> RESULT_INSERTONE;
typedef mongocxx::result::bulk_write RESULT_BULKWRITE;

#define TO_BSON_OID(strid) bsoncxx::oid{ bsoncxx::stdx::string_view{strid} }
#define BSON_NOW() bsoncxx::types::b_date{ std::chrono::system_clock::now() }

bsoncxx::oid get_bsonid(std::string strid)
{
	bsoncxx::oid oid;
	try
	{
		oid = bsoncxx::oid{ bsoncxx::stdx::string_view{strid} };
	}
	catch (...)
	{
	}
	return oid;
}

static int api_log_level = 1;

class ApiLogger
{
public:
	ApiLogger(std::string apiname)
	{
		apiname = apiname;
		if (api_log_level == 1)
		{
			std::cout << std::endl << std::endl << std::endl;
		}

		std::string indent;
		switch (api_log_level)
		{
		case 2: indent = "\t"; break;
		case 3: indent = "\t\t"; break;
		case 4: indent = "\t\t\t"; break;
		}
		std::cout << indent << "[begin]" << " " << apiname << std::endl;
	}
	~ApiLogger()
	{
		std::string indent;
		switch (api_log_level)
		{
		case 2: indent = "\t"; break;
		case 3: indent = "\t\t"; break;
		case 4: indent = "\t\t\t"; break;
		}
		std::cout << indent << "[end]" << " " << apiname << std::endl;
	}
	void setname(std::string apiname) { apiname = apiname; }
	void append_err(std::string err)
	{
		std::string indent;
		switch (api_log_level)
		{
		case 1: indent = "\t"; break;
		case 2: indent = "\t\t"; break;
		case 3: indent = "\t\t\t"; break;
		}
		std::cout << indent << "[error]" << err << std::endl;
	}
	void append_info(std::string info)
	{
		std::string indent;
		switch (api_log_level)
		{
		case 1: indent = "\t"; break;
		case 2: indent = "\t\t"; break;
		case 3: indent = "\t\t\t"; break;
		}
		std::cout << indent << "[info]" << info << std::endl;
	}
	void add_level() { api_log_level++; }
	void down_level() { api_log_level--; }

private:
	std::string apiname;
};


class NoteInfoHandler : virtual public NoteInfoIf
{
public:
	NoteInfoHandler() {
		// Your initialization goes here
	}

	void GetNotebooks(std::vector<Notebook>& _return, const std::string& userid)
	{
		ApiLogger apilogger("GetNotebooks");

		mongocxx::collection users = conn["flashnote"]["user"];
		auto notebooks = conn["flashnote"]["notebooks"];
		auto note_coll = conn["flashnote"]["notes"];
		
		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return;
		}

		bsoncxx::document::view userview = user_result->view();
		auto booksview = userview.find("notebooks");
		if (booksview == userview.end())
		{
			apilogger.append_err("user document has no key notebooks.");
			return;
		}

		bsoncxx::array::view av = booksview->get_array().value;
		for (auto iter = av.begin(); iter != av.end(); iter++)
		{
			bsoncxx::array::element elem(*iter);
			std::string bookid = elem.get_oid().value.to_string();

			bsoncxx::stdx::optional<bsoncxx::document::value> book_result =
				notebooks.find_one(document{} << "_id" << TO_BSON_OID(bookid) << finalize); 
			if (book_result)
			{
				bsoncxx::document::view bookview = book_result->view();
					
				Notebook notebook;
				notebook.id = bookview["_id"].get_oid().value.to_string();
				notebook.name = bookview["name"].get_utf8().value.to_string();
				notebook.create_time = bookview["create_time"].get_date().to_int64();
				notebook.modify_time = bookview["modify_time"].get_date().to_int64();
				notebook.creater_id = bookview["creater"].get_oid().value.to_string();

				bsoncxx::array::view av = bookview["notes"].get_array().value;
				for (auto iter = av.begin(); iter != av.end(); iter++)
				{
					bsoncxx::array::element elem(*iter);
					std::string noteid = elem.get_oid().value.to_string();

					auto note_result = note_coll.find_one(document{}
							<< "_id"
							<< get_bsonid(noteid)
							<< finalize);
					if (!note_result)
					{
						apilogger.append_err("no note with id = " + noteid);
					}

					bsoncxx::document::view note_view = note_result->view();

					Note note;
					note.title = note_view["title"].get_utf8().value.to_string();
					note.text_abbre = note_view["content"].get_utf8().value.to_string();
					note.id = noteid;
					note.create_time = note_view["create_time"].get_date().to_int64();
					note.modify_time = note_view["modify_time"].get_date().to_int64();
					note.creater_id = note_view["creater"].get_oid().value.to_string();

					notebook.notes.push_back(note);
				}
				_return.push_back(notebook);
			}
			else
			{
				//bookid已经失效了。
				std::cout << "bookid = " << bookid << "is invalid" << std::endl;
			}
		}
	}

	void GetFreeNotes(std::vector<Note>& freenotes, const std::string& userid)
	{
		ApiLogger apilogger("GetFreeNotes");
		
		auto note_coll = conn["flashnote"]["notes"];
		auto users = conn["flashnote"]["user"];
		auto freenotes_coll = conn["flashnote"]["freenotes"];

		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return;
		}

		freenotes.clear();

		mongocxx::cursor cursor = freenotes_coll.find({ document{} << "userid"
			<< TO_BSON_OID(userid) << finalize });
		for (auto doc : cursor)
		{
			std::string noteid = doc["noteid"].get_oid().value.to_string();
			
			auto note_result = note_coll.find_one(document{}
				<< "_id"
				<< get_bsonid(noteid)
				<< finalize);
			if (!note_result)
			{
				apilogger.append_err("no note with id = " + noteid);
			}

			bsoncxx::document::view note_view = note_result->view();

			Note note;
			note.title = note_view["title"].get_utf8().value.to_string();
			note.text_abbre = note_view["content"].get_utf8().value.to_string();
			note.id = noteid;
			note.create_time = note_view["create_time"].get_date().to_int64();
			note.modify_time = note_view["modify_time"].get_date().to_int64();
			note.creater_id = note_view["creater"].get_oid().value.to_string();

			freenotes.push_back(note);
		}
	}

	void NewNotebook(std::string& newbookid, const std::string& userid, const std::string& name)
	{
		ApiLogger apilogger("NewNotebook");

		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection users = conn["flashnote"]["user"];

		// 要先检查userid的合法性
		bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result = 
			users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);

		if (!maybe_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return;
		}

		//名字不能过长或空。
		if (name.empty() || name.length() > 20)
		{
			apilogger.append_err("the name of notebook should not be empty or too long.");
			return;
		}

		// 检查用户是否已有同名的notebook。
		std::vector<Notebook> books;
		apilogger.add_level();
		GetNotebooks(books, userid);
		apilogger.down_level();

		for (int i = 0; i < books.size(); i++)
		{
			if (books[i].name == name)
			{
				apilogger.append_err("name " + name + " has been used by current user.");
				return;
			}
		}

		// 2.先创建一个notebook
		const bsoncxx::document::value& notebook = bsoncxx::builder::basic::make_document(
			kvp("create_time", BSON_NOW()),
			kvp("modify_time", BSON_NOW()),
			kvp("name", name),
			kvp("creater", TO_BSON_OID(userid)),
			kvp("owners", [=](bsoncxx::builder::basic::sub_array subarr){
					subarr.append(TO_BSON_OID(userid));
				}),
			kvp("notes", bsoncxx::types::b_array())
		);				

		auto retVal = notebooks.insert_one(notebook.view());
		if (!retVal)
		{
			apilogger.append_err("the notebook has failed inserted into collection notebooks.");
			return;
		}
		const RESULT_BULKWRITE& writeRes = retVal->result();
		if (writeRes.inserted_count() != 1)
		{
			apilogger.append_err("insert count is not equal to 1.");
			return;
		}

		apilogger.append_info("notebook has been inserted into the db");

		// 3.将notebook加入该用户的笔记本列表。
		bsoncxx::oid oid = retVal->inserted_id().get_oid().value;
		newbookid = oid.to_string();
		auto update_result = users.update_one(document{} << "_id" << TO_BSON_OID(userid) << finalize,
				document{} << "$addToSet" << open_document <<
				"notebooks" << TO_BSON_OID(newbookid) << close_document << finalize);
		if (!update_result)
		{
			apilogger.append_err("users collection failed to add notebook to notebooks set.");
			return;
		}
		const RESULT_BULKWRITE& updateRes = update_result->result();
		if (updateRes.modified_count() == 1)
		{
			apilogger.append_info("users collection has added notebook to notebooks set.");
		}
		else
		{
			apilogger.append_err("update result error");
		}
	}

	bool DeleteNotebook(const std::string& userid, const std::string& bookid)
	{
		ApiLogger apilogger("DeleteNotebook");

		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection users = conn["flashnote"]["user"];

		//合法性检查
		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("no user with user-id = " + userid);
			return false;
		}

		auto book_result = notebooks.find_one(document{} << "_id" << get_bsonid(bookid) << finalize);
		if (!book_result)
		{
			apilogger.append_err("no book with bookid = " + bookid);
			return false;
		}

		//notebook删除userid的owner
		auto update_result = notebooks.update_one(
			document{} << "_id" << TO_BSON_OID(bookid) << finalize,
			document{} << "$pull" << open_document <<
			"owners" << TO_BSON_OID(userid) << close_document << finalize);
		if (!update_result || update_result->result().modified_count() != 1)
		{
			apilogger.append_err("update notebooks' owners failed.");
			return false;
		}

		//user移除该notebook
		update_result = users.update_one(
			document{} << "_id" << TO_BSON_OID(userid) << finalize,
			document{} << "$pull" << open_document <<
			"notebooks" << TO_BSON_OID(bookid) << close_document << finalize);
		if (!update_result || update_result->result().modified_count() != 1)
		{
			apilogger.append_err("user remove notebook failed.");
			return false;
		}

		//再取一次notebook，观察是否还有其他owner
		book_result = notebooks.find_one(document{} << "_id" << TO_BSON_OID(bookid) << finalize);
		bsoncxx::document::view bookview = book_result->view();
		auto owners_view = bookview.find("owners");
		if (owners_view == bookview.end())
		{
			apilogger.append_err("no owners key in notebook(id = " + bookid);
			return false;
		}

		bsoncxx::array::view av = bookview["owners"].get_array().value;
		int sz = std::distance(av.begin(), av.end());
		if (sz == 0)
		{
			//只有一个owner且被删除的笔记本，就不视为共享笔记本
			//此时需要trash掉所有的note。
			auto notes_view = bookview.find("notes");
			if (notes_view == bookview.end())
			{
				apilogger.append_err("book don't has a notes key.");
				return false;
			}
			bsoncxx::array::view av = notes_view->get_array().value;
			for (auto iter = av.begin(); iter != av.end(); iter++)
			{
				bsoncxx::array::element elem(*iter);
				std::string noteid = elem.get_oid().value.to_string();
				apilogger.add_level();
				bool bRet = TrashNote(userid, bookid, noteid);
				apilogger.down_level();
				if (!bRet)
				{
					apilogger.append_err("note (id=" + noteid + ") trash failed");
					return false;
				}
			}
		}

		//最后delete notebook
		auto delete_result = notebooks.delete_one(
			document{} << "_id" << TO_BSON_OID(bookid) << finalize);
		if (!delete_result || delete_result->result().deleted_count() != 1)
		{
			apilogger.append_err("delete notebook from collection notebooks failed.");
			return false;
		}
		apilogger.append_info("notebook record has been removed from collection notebooks.");
		return true;
	}

	void NewNote(std::string& newnoteid, const std::string& userid, const std::string& bookid, const std::string& title)
	{
		ApiLogger apilogger("NewNote");

		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection users = conn["flashnote"]["user"];
		mongocxx::collection freenotes = conn["flashnote"]["freenotes"];

		//合法性检查
		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return;
		}

		//空bookid表示游离的note
		bool bFreeNote = bookid.empty();
		
		if (!bFreeNote)
		{
			auto book_result = notebooks.find_one(document{} << "_id" << get_bsonid(bookid) << finalize);
			if (!book_result)
			{
				apilogger.append_err("no book with bookid = " + bookid);
				return;
			}
		}

		const bsoncxx::document::value& note = bsoncxx::builder::basic::make_document(
			kvp("creater", TO_BSON_OID(userid)),
			kvp("owners", [=](bsoncxx::builder::basic::sub_array subarr) {
				subarr.append(TO_BSON_OID(userid));
				}),
			kvp("create_time", BSON_NOW()),
			kvp("modify_time", BSON_NOW()),
			kvp("title", title),
			kvp("content", ""));

		auto retVal = notes.insert_one(note.view());
		if (!retVal || retVal->result().inserted_count() != 1)
		{
			apilogger.append_err("NewNote insert_one failed");
		}
		else
		{
			bsoncxx::oid oid = retVal->inserted_id().get_oid().value;
			newnoteid = oid.to_string();

			apilogger.append_info("new note(id=" + newnoteid +
				") has been inserted into notes");

			if (!bFreeNote)
			{
				auto update_result = notebooks.update_one(
					document{} << "_id" << TO_BSON_OID(bookid) << finalize,
					document{} << "$addToSet" << open_document <<
					"notes" << TO_BSON_OID(newnoteid) << close_document << finalize
				);
				if (update_result && update_result->result().modified_count() == 1)
				{
					apilogger.append_info("new note(id=" + newnoteid +
						") has been inserted into notebook.");
				}
				else
				{
					apilogger.append_err("note cannot insert into the notebook");
					newnoteid = "";
				}
			}
			else
			{
				const bsoncxx::document::value& freenote = bsoncxx::builder::basic::make_document(
					kvp("noteid", TO_BSON_OID(newnoteid)),
					kvp("userid", TO_BSON_OID(userid))
					);
				retVal = freenotes.insert_one(freenote.view());
				if (!retVal || retVal->result().inserted_count() != 1)
				{
					apilogger.append_err("freenotes insert_one failed");
				}
				else
				{
					apilogger.append_info("new free note(id=" + newnoteid +
						") has been inserted into freenote.");
				}
			}
		}
	}

	bool UpdateNote(const std::string& noteid, const std::string& title, const std::string& content)
	{
		ApiLogger apilogger("UpdateNote");

		mongocxx::collection notes = conn["flashnote"]["notes"];
		auto note_result = notes.find_one(document{} << "_id" << get_bsonid(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with noteid = " + noteid);
			return false;
		}

		//TODO: title和content的限制

		auto update_result = notes.update_one(
			document{} << "_id" << TO_BSON_OID(noteid) << finalize,
			document{} << "$set" << open_document <<
			"title" << title << 
			"content" << content <<
			close_document << finalize);
		if (update_result && update_result->result().modified_count() == 1)
		{
			apilogger.append_info("note id = " + noteid + " has been updated");
			return true;
		}
		else
		{
			apilogger.append_err("note id = " + noteid + " updated failed");
			return false;
		}
	}

	void GetContent(std::string& content, const std::string& noteid)
	{
		ApiLogger apilogger("GetContent");

		mongocxx::collection notes = conn["flashnote"]["notes"];
		auto note_result = notes.find_one(document{} << "_id" << get_bsonid(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with note-id = " + noteid);
			return;
		}
		bsoncxx::document::view noteview = note_result->view();
		if (noteview.find("content") == noteview.end())
		{
			apilogger.append_err("there is no content field in note(id=" + noteid + ")");
			return;
		}
		content = noteview["content"].get_utf8().value.to_string();
	}

	bool MoveNote(const std::string& noteid, const std::string& src_bookid, const std::string& dest_bookid)
	{
		// TODO
	}

	bool TrashNote(const std::string& userid, const std::string& bookid, const std::string& noteid)
	{
		ApiLogger apilogger("TrashNote");

		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection users = conn["flashnote"]["user"];
		mongocxx::collection trash_conn = conn["flashnote"]["trash"];

		//合法性检查
		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return false;
		}

		auto book_result = notebooks.find_one(document{} << "_id" << get_bsonid(bookid) << finalize);
		if (!book_result)
		{
			apilogger.append_err("no book with bookid = " + bookid);
			return false;
		}

		auto note_result = notes.find_one(document{} << "_id" << get_bsonid(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with noteid = " + noteid);
			return false;
		}

		//1. notebooks先去掉noteid
		auto update_result = notebooks.update_one(
			document{} << "_id" << TO_BSON_OID(bookid) << finalize,
			document{} << "$pull" << open_document <<
			"notes" << TO_BSON_OID(noteid) << close_document << finalize);
		if (!update_result || update_result->result().modified_count() != 1)
		{
			apilogger.append_err("Notebook (id=" + bookid + ") don't have note (id=" + noteid + ")");
			return false;
		}

		apilogger.append_info(bookid + "has been removed by notebook collection");

		//2. 生成Trash记录
		const bsoncxx::document::value& trash = bsoncxx::builder::basic::make_document(
			kvp("trash_time", bsoncxx::types::b_date{ std::chrono::system_clock::now() }),
			kvp("user_id", TO_BSON_OID(userid)),
			kvp("srcbook_id", TO_BSON_OID(bookid)),
			kvp("note_id", TO_BSON_OID(noteid))
			);

		auto retVal = trash_conn.insert_one(trash.view());
		if (retVal && retVal->result().inserted_count() == 1)
		{
			apilogger.append_info("note(id=" + noteid + ") has been inserted into trash");
			return true;
		}
		else
		{
			apilogger.append_err("trash record insert failed");
			return false;
		}
	}

	void GetTrashes(std::vector<Trash> & trashes, const std::string & userid)
	{
		// Your implementation goes here
		ApiLogger apilogger("GetTrashes");
		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection users = conn["flashnote"]["user"];
		mongocxx::collection trash_conn = conn["flashnote"]["trash"];

		trashes.clear();

		mongocxx::cursor cursor = trash_conn.find({ document{} << "user_id" 
			<< TO_BSON_OID(userid) << finalize });
		for (auto doc : cursor)
		{
			Trash trash;
			trash.trash_id = doc["_id"].get_oid().value.to_string();
			trash.trash_time = doc["trash_time"].get_date().to_int64();
			trash.notebook.id = doc["srcbook_id"].get_oid().value.to_string();	//只返回id，然后客户端从内核中取。

			//检索note
			std::string noteid = doc["note_id"].get_oid().value.to_string();
			auto note_result = notes.find_one(document{}
				<< "_id"
				<< get_bsonid(noteid)
				<< finalize);
			if (!note_result)
			{
				apilogger.append_err("no note with id = " + noteid);
			}

			bsoncxx::document::view note_view = note_result->view();

			Note note;
			note.title = note_view["title"].get_utf8().value.to_string();
			note.text_abbre = note_view["content"].get_utf8().value.to_string();
			note.id = noteid;
			note.create_time = note_view["create_time"].get_date().to_int64();
			note.modify_time = note_view["modify_time"].get_date().to_int64();
			note.creater_id = note_view["creater"].get_oid().value.to_string();

			trash.note = note;
			trashes.push_back(trash);
		}
	}

	bool RecoverNote(const std::string& userid, const std::string& noteid)
	{
		ApiLogger apilogger("RecoverNote");

		mongocxx::collection users = conn["flashnote"]["user"];
		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection trash_conn = conn["flashnote"]["trash"];
		mongocxx::collection freenotes = conn["flashnote"]["freenotes"];

		auto note_result = notes.find_one(document{} << "_id" << get_bsonid(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with noteid = " + noteid);
			return false;
		}

		auto user_result = users.find_one(document{} << "_id" << get_bsonid(userid) << finalize);
		if (!user_result)
		{
			apilogger.append_err("User (id = " + userid + ") is invalid");
			return false;
		}

		auto trash_record = trash_conn.find_one(
			document{}
			<< "user_id" << TO_BSON_OID(userid)
			<< "note_id" << TO_BSON_OID(noteid)
			<< finalize
		);
		if (!trash_record)
		{
			apilogger.append_err("error: There is no trash record");
			return false;
		}

		bsoncxx::document::view trashview = trash_record->view();
		if (trashview.find("_id") == trashview.end() ||
			trashview.find("srcbook_id") == trashview.end())
		{
			apilogger.append_err("no _id or srcbook_id in trash record.");
			return false;
		}

		bsoncxx::types::b_oid trashid = trashview["_id"].get_oid();
		bsoncxx::types::b_oid bookOid = trashview["srcbook_id"].get_oid();
		//检查bookOid的合法性

		//从trash中移除该记录。
		auto delete_result = trash_conn.delete_one(document{} << "_id" << trashid.value << finalize);
		if (!delete_result || delete_result->result().deleted_count() != 1)
		{
			apilogger.append_err("delete trash-record from collection trash failed.");
			return false;
		}
		apilogger.append_info("Trash record has been removed.");

		//将该note重新添加到notebook
		auto book_result = notebooks.find_one(document{} << "_id" << bookOid.value << finalize);
		bool bRet = true;
		if (book_result)
		{
			apilogger.add_level();
			bRet = AddNoteToBook(bookOid.value.to_string(), noteid);
			apilogger.down_level();
		}
		else
		{
			const bsoncxx::document::value& freenote = bsoncxx::builder::basic::make_document(
				kvp("noteid", TO_BSON_OID(noteid)),
				kvp("userid", TO_BSON_OID(userid))
				);
			auto retVal = freenotes.insert_one(freenote.view());
			if (!retVal || retVal->result().inserted_count() != 1)
			{
				bRet = false;
				apilogger.append_err("freenotes insert_one failed");
			}
			else
			{
				bRet = true;
				apilogger.append_info("new free note(id=" + noteid +
					") has been inserted into freenote.");
			}
		}
		return bRet;
	}

	bool DeleteNote(const std::string& userid, const std::string& noteid)
	{
		ApiLogger apilogger("DeleteNote");

		// 从废纸篓彻底删除此笔记，类似删除笔记本，需要观察持有者的个数。
		mongocxx::collection notes = conn["flashnote"]["notes"];
		mongocxx::collection trash_conn = conn["flashnote"]["trash"];

		auto note_result = notes.find_one(document{} << "_id" << get_bsonid(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with noteid = " + noteid);
			return false;
		}

		//note owners移除当前用户
		auto update_result = notes.update_one(
			document{} << "_id" << TO_BSON_OID(noteid) << finalize,
			document{} << "$pull" << open_document <<
			"owners" << TO_BSON_OID(userid) << close_document << finalize);
		if (!update_result || update_result->result().modified_count() != 1)
		{
			apilogger.append_err("note (id="+ noteid + ") remove owner failed");
			return false;
		}

		//先从用户trash删除该记录。
		auto delete_result = trash_conn.delete_one(
			document{} << "user_id" << TO_BSON_OID(userid) <<
				"note_id" << TO_BSON_OID(noteid) << finalize);
		if (!delete_result || delete_result->result().deleted_count() != 1)
		{
			apilogger.append_err("delete trash with note id=" + noteid + " failed");
			return false;
		}

		//观察是否还有其他owners
		note_result = notes.find_one(document{} << "_id" << TO_BSON_OID(noteid) << finalize);
		bsoncxx::document::view note_view = note_result->view();

		auto owners_view = note_view.find("owners");
		if (owners_view == note_view.end())
		{
			apilogger.append_err("no owners key in note(id = " + noteid);
			return false;
		}

		bsoncxx::array::view av = note_view["owners"].get_array().value;
		int sz = std::distance(av.begin(), av.end());
		if (sz == 0)
		{
			//从notes删除该笔记
			delete_result = notes.delete_one(document{} << "_id"
				<< TO_BSON_OID(noteid) << finalize);
			if (!delete_result || delete_result->result().deleted_count() != 1)
			{
				apilogger.append_err("notes.deleteOne failed. (noteid=" + noteid + ")");
				return false;
			}
			else
			{
				apilogger.append_info("note was deleted from notes collection");
				return true;
			}
		}
		return true;
	}

private:
	bool AddNoteToBook(const std::string& bookid, const std::string& noteid)
	{
		ApiLogger apilogger("AddNoteToBook");

		//不能外部调用，因为笔记不能在同一用户不同笔记本共享。
		mongocxx::collection notebooks = conn["flashnote"]["notebooks"];
		mongocxx::collection notes = conn["flashnote"]["notes"];

		auto note_result = notes.find_one(document{} << "_id" << TO_BSON_OID(noteid) << finalize);
		if (!note_result)
		{
			apilogger.append_err("no note with noteid = " + noteid);
			return false;
		}

		auto book_result = notebooks.find_one(document{} << "_id" << TO_BSON_OID(bookid) << finalize);
		if (!book_result)
		{
			apilogger.append_err("no notebookid = " + bookid);
			return false;
		}

		auto update_result = notebooks.update_one(
			document{} << "_id" << TO_BSON_OID(bookid) << finalize,
			document{} << "$addToSet" << open_document <<
			"notes" << TO_BSON_OID(noteid) << close_document << finalize
		);
		if (update_result && update_result->result().modified_count() == 1)
		{
			apilogger.append_info(noteid + " has been inserted into notebook.");
			return true;
		}
		else
		{
			apilogger.append_err(noteid + " has not been inserted into notebook.");
			return false;
		}
	}
};

int main(int argc, char **argv)
{
	int port = 82;
	shared_ptr<NoteInfoHandler> handler(new NoteInfoHandler());
	shared_ptr<TProcessor> processor(new NoteInfoProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
	server.serve();
	return 0;
}

