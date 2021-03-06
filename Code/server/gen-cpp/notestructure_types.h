/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef notestructure_TYPES_H
#define notestructure_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




struct NoteType {
  enum type {
    NORMAL_NOTE = 1,
    MINDMAP = 2,
    SCHEDULE = 3
  };
};

extern const std::map<int, const char*> _NoteType_VALUES_TO_NAMES;

typedef int64_t Timestamp;


class Note {
 public:

  static const char* ascii_fingerprint; // = "C387CCC76D2831588B2463277030137C";
  static const uint8_t binary_fingerprint[16]; // = {0xC3,0x87,0xCC,0xC7,0x6D,0x28,0x31,0x58,0x8B,0x24,0x63,0x27,0x70,0x30,0x13,0x7C};

  Note() : id(), title(), text_abbre(), creater_id(), create_time(0), modify_time(0), share(0), type((NoteType::type)0) {
  }

  virtual ~Note() throw() {}

  std::string id;
  std::string title;
  std::string text_abbre;
  std::string creater_id;
  Timestamp create_time;
  Timestamp modify_time;
  bool share;
  NoteType::type type;

  void __set_id(const std::string& val) {
    id = val;
  }

  void __set_title(const std::string& val) {
    title = val;
  }

  void __set_text_abbre(const std::string& val) {
    text_abbre = val;
  }

  void __set_creater_id(const std::string& val) {
    creater_id = val;
  }

  void __set_create_time(const Timestamp val) {
    create_time = val;
  }

  void __set_modify_time(const Timestamp val) {
    modify_time = val;
  }

  void __set_share(const bool val) {
    share = val;
  }

  void __set_type(const NoteType::type val) {
    type = val;
  }

  bool operator == (const Note & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(title == rhs.title))
      return false;
    if (!(text_abbre == rhs.text_abbre))
      return false;
    if (!(creater_id == rhs.creater_id))
      return false;
    if (!(create_time == rhs.create_time))
      return false;
    if (!(modify_time == rhs.modify_time))
      return false;
    if (!(share == rhs.share))
      return false;
    if (!(type == rhs.type))
      return false;
    return true;
  }
  bool operator != (const Note &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Note & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Note &a, Note &b);


class Notebook {
 public:

  static const char* ascii_fingerprint; // = "E0B4830E36A5B2EC32BDD86D326FDA6B";
  static const uint8_t binary_fingerprint[16]; // = {0xE0,0xB4,0x83,0x0E,0x36,0xA5,0xB2,0xEC,0x32,0xBD,0xD8,0x6D,0x32,0x6F,0xDA,0x6B};

  Notebook() : id(), name(), create_time(0), modify_time(0), creater_id(), share(0) {
  }

  virtual ~Notebook() throw() {}

  std::string id;
  std::vector<Note>  notes;
  std::string name;
  Timestamp create_time;
  Timestamp modify_time;
  std::string creater_id;
  bool share;

  void __set_id(const std::string& val) {
    id = val;
  }

  void __set_notes(const std::vector<Note> & val) {
    notes = val;
  }

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_create_time(const Timestamp val) {
    create_time = val;
  }

  void __set_modify_time(const Timestamp val) {
    modify_time = val;
  }

  void __set_creater_id(const std::string& val) {
    creater_id = val;
  }

  void __set_share(const bool val) {
    share = val;
  }

  bool operator == (const Notebook & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(notes == rhs.notes))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(create_time == rhs.create_time))
      return false;
    if (!(modify_time == rhs.modify_time))
      return false;
    if (!(creater_id == rhs.creater_id))
      return false;
    if (!(share == rhs.share))
      return false;
    return true;
  }
  bool operator != (const Notebook &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Notebook & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Notebook &a, Notebook &b);


class Trash {
 public:

  static const char* ascii_fingerprint; // = "50B6F1640F2F0B6201D7A728922DA7B7";
  static const uint8_t binary_fingerprint[16]; // = {0x50,0xB6,0xF1,0x64,0x0F,0x2F,0x0B,0x62,0x01,0xD7,0xA7,0x28,0x92,0x2D,0xA7,0xB7};

  Trash() : trash_id(), trash_time(0) {
  }

  virtual ~Trash() throw() {}

  std::string trash_id;
  Timestamp trash_time;
  Note note;
  Notebook notebook;

  void __set_trash_id(const std::string& val) {
    trash_id = val;
  }

  void __set_trash_time(const Timestamp val) {
    trash_time = val;
  }

  void __set_note(const Note& val) {
    note = val;
  }

  void __set_notebook(const Notebook& val) {
    notebook = val;
  }

  bool operator == (const Trash & rhs) const
  {
    if (!(trash_id == rhs.trash_id))
      return false;
    if (!(trash_time == rhs.trash_time))
      return false;
    if (!(note == rhs.note))
      return false;
    if (!(notebook == rhs.notebook))
      return false;
    return true;
  }
  bool operator != (const Trash &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Trash & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Trash &a, Trash &b);



#endif
