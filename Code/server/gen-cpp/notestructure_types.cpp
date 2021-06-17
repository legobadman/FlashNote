/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "notestructure_types.h"

#include <algorithm>



int _kNoteTypeValues[] = {
  NoteType::NORMAL_NOTE,
  NoteType::MINDMAP,
  NoteType::SCHEDULE
};
const char* _kNoteTypeNames[] = {
  "NORMAL_NOTE",
  "MINDMAP",
  "SCHEDULE"
};
const std::map<int, const char*> _NoteType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kNoteTypeValues, _kNoteTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* Note::ascii_fingerprint = "C387CCC76D2831588B2463277030137C";
const uint8_t Note::binary_fingerprint[16] = {0xC3,0x87,0xCC,0xC7,0x6D,0x28,0x31,0x58,0x8B,0x24,0x63,0x27,0x70,0x30,0x13,0x7C};

uint32_t Note::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_id = false;
  bool isset_title = false;
  bool isset_text_abbre = false;
  bool isset_creater_id = false;
  bool isset_create_time = false;
  bool isset_modify_time = false;
  bool isset_share = false;
  bool isset_type = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->id);
          isset_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->title);
          isset_title = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text_abbre);
          isset_text_abbre = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->creater_id);
          isset_creater_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->create_time);
          isset_create_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->modify_time);
          isset_modify_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->share);
          isset_share = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->type = (NoteType::type)ecast0;
          isset_type = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_title)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_text_abbre)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_creater_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_create_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_modify_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_share)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_type)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Note::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Note");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("title", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->title);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text_abbre", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->text_abbre);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("creater_id", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->creater_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("create_time", ::apache::thrift::protocol::T_I64, 5);
  xfer += oprot->writeI64(this->create_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("modify_time", ::apache::thrift::protocol::T_I64, 6);
  xfer += oprot->writeI64(this->modify_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("share", ::apache::thrift::protocol::T_BOOL, 7);
  xfer += oprot->writeBool(this->share);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("type", ::apache::thrift::protocol::T_I32, 8);
  xfer += oprot->writeI32((int32_t)this->type);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Note &a, Note &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.title, b.title);
  swap(a.text_abbre, b.text_abbre);
  swap(a.creater_id, b.creater_id);
  swap(a.create_time, b.create_time);
  swap(a.modify_time, b.modify_time);
  swap(a.share, b.share);
  swap(a.type, b.type);
}

const char* Notebook::ascii_fingerprint = "E0B4830E36A5B2EC32BDD86D326FDA6B";
const uint8_t Notebook::binary_fingerprint[16] = {0xE0,0xB4,0x83,0x0E,0x36,0xA5,0xB2,0xEC,0x32,0xBD,0xD8,0x6D,0x32,0x6F,0xDA,0x6B};

uint32_t Notebook::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_id = false;
  bool isset_notes = false;
  bool isset_name = false;
  bool isset_create_time = false;
  bool isset_modify_time = false;
  bool isset_creater_id = false;
  bool isset_share = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->id);
          isset_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->notes.clear();
            uint32_t _size1;
            ::apache::thrift::protocol::TType _etype4;
            xfer += iprot->readListBegin(_etype4, _size1);
            this->notes.resize(_size1);
            uint32_t _i5;
            for (_i5 = 0; _i5 < _size1; ++_i5)
            {
              xfer += this->notes[_i5].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          isset_notes = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->name);
          isset_name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->create_time);
          isset_create_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->modify_time);
          isset_modify_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->creater_id);
          isset_creater_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->share);
          isset_share = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_notes)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_name)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_create_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_modify_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_creater_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_share)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Notebook::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Notebook");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("notes", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->notes.size()));
    std::vector<Note> ::const_iterator _iter6;
    for (_iter6 = this->notes.begin(); _iter6 != this->notes.end(); ++_iter6)
    {
      xfer += (*_iter6).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("name", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("create_time", ::apache::thrift::protocol::T_I64, 4);
  xfer += oprot->writeI64(this->create_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("modify_time", ::apache::thrift::protocol::T_I64, 5);
  xfer += oprot->writeI64(this->modify_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("creater_id", ::apache::thrift::protocol::T_STRING, 6);
  xfer += oprot->writeString(this->creater_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("share", ::apache::thrift::protocol::T_BOOL, 7);
  xfer += oprot->writeBool(this->share);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Notebook &a, Notebook &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.notes, b.notes);
  swap(a.name, b.name);
  swap(a.create_time, b.create_time);
  swap(a.modify_time, b.modify_time);
  swap(a.creater_id, b.creater_id);
  swap(a.share, b.share);
}

const char* Trash::ascii_fingerprint = "50B6F1640F2F0B6201D7A728922DA7B7";
const uint8_t Trash::binary_fingerprint[16] = {0x50,0xB6,0xF1,0x64,0x0F,0x2F,0x0B,0x62,0x01,0xD7,0xA7,0x28,0x92,0x2D,0xA7,0xB7};

uint32_t Trash::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_trash_id = false;
  bool isset_trash_time = false;
  bool isset_note = false;
  bool isset_notebook = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->trash_id);
          isset_trash_id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->trash_time);
          isset_trash_time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->note.read(iprot);
          isset_note = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->notebook.read(iprot);
          isset_notebook = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_trash_id)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_trash_time)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_note)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_notebook)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t Trash::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Trash");

  xfer += oprot->writeFieldBegin("trash_id", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->trash_id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("trash_time", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->trash_time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("note", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->note.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("notebook", ::apache::thrift::protocol::T_STRUCT, 4);
  xfer += this->notebook.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Trash &a, Trash &b) {
  using ::std::swap;
  swap(a.trash_id, b.trash_id);
  swap(a.trash_time, b.trash_time);
  swap(a.note, b.note);
  swap(a.notebook, b.notebook);
}


