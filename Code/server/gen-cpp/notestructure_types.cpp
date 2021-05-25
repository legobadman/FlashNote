/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "notestructure_types.h"

#include <algorithm>



const char* Note::ascii_fingerprint = "0FE07E98F8DCAA5C4CAAD9CDCED5F067";
const uint8_t Note::binary_fingerprint[16] = {0x0F,0xE0,0x7E,0x98,0xF8,0xDC,0xAA,0x5C,0x4C,0xAA,0xD9,0xCD,0xCE,0xD5,0xF0,0x67};

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
}

const char* Notebook::ascii_fingerprint = "422405715F4FD702966E81371B63B490";
const uint8_t Notebook::binary_fingerprint[16] = {0x42,0x24,0x05,0x71,0x5F,0x4F,0xD7,0x02,0x96,0x6E,0x81,0x37,0x1B,0x63,0xB4,0x90};

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
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->notes.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += this->notes[_i4].read(iprot);
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
    std::vector<Note> ::const_iterator _iter5;
    for (_iter5 = this->notes.begin(); _iter5 != this->notes.end(); ++_iter5)
    {
      xfer += (*_iter5).write(oprot);
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

const char* Trash::ascii_fingerprint = "49F40AF7F6EF2525B146C3AFC11BA377";
const uint8_t Trash::binary_fingerprint[16] = {0x49,0xF4,0x0A,0xF7,0xF6,0xEF,0x25,0x25,0xB1,0x46,0xC3,0xAF,0xC1,0x1B,0xA3,0x77};

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


