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




typedef int32_t Timestamp;


class Note {
 public:

  static const char* ascii_fingerprint; // = "44D59A597378E5B243F6FA53B3ACA428";
  static const uint8_t binary_fingerprint[16]; // = {0x44,0xD5,0x9A,0x59,0x73,0x78,0xE5,0xB2,0x43,0xF6,0xFA,0x53,0xB3,0xAC,0xA4,0x28};

  Note() : id(), title(), text_abbre(), notebook_id(), creater_id(), create_time(0), modify_time(0) {
  }

  virtual ~Note() throw() {}

  std::string id;
  std::string title;
  std::string text_abbre;
  std::string notebook_id;
  std::string creater_id;
  std::vector<std::string>  owners;
  Timestamp create_time;
  Timestamp modify_time;

  void __set_id(const std::string& val) {
    id = val;
  }

  void __set_title(const std::string& val) {
    title = val;
  }

  void __set_text_abbre(const std::string& val) {
    text_abbre = val;
  }

  void __set_notebook_id(const std::string& val) {
    notebook_id = val;
  }

  void __set_creater_id(const std::string& val) {
    creater_id = val;
  }

  void __set_owners(const std::vector<std::string> & val) {
    owners = val;
  }

  void __set_create_time(const Timestamp val) {
    create_time = val;
  }

  void __set_modify_time(const Timestamp val) {
    modify_time = val;
  }

  bool operator == (const Note & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(title == rhs.title))
      return false;
    if (!(text_abbre == rhs.text_abbre))
      return false;
    if (!(notebook_id == rhs.notebook_id))
      return false;
    if (!(creater_id == rhs.creater_id))
      return false;
    if (!(owners == rhs.owners))
      return false;
    if (!(create_time == rhs.create_time))
      return false;
    if (!(modify_time == rhs.modify_time))
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

  static const char* ascii_fingerprint; // = "6C5D5049A7225822B29295CED8837E24";
  static const uint8_t binary_fingerprint[16]; // = {0x6C,0x5D,0x50,0x49,0xA7,0x22,0x58,0x22,0xB2,0x92,0x95,0xCE,0xD8,0x83,0x7E,0x24};

  Notebook() : id(), name(), create_time(0), modify_time(0), creater() {
  }

  virtual ~Notebook() throw() {}

  std::string id;
  std::vector<Note>  notes;
  std::string name;
  Timestamp create_time;
  Timestamp modify_time;
  std::string creater;

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

  void __set_creater(const std::string& val) {
    creater = val;
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
    if (!(creater == rhs.creater))
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



#endif
