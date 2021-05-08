/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef UserInfo_H
#define UserInfo_H

#include <thrift/TDispatchProcessor.h>
#include "notestructure_types.h"



class UserInfoIf {
 public:
  virtual ~UserInfoIf() {}
  virtual void GetUserId(std::string& _return, const std::string& username) = 0;
};

class UserInfoIfFactory {
 public:
  typedef UserInfoIf Handler;

  virtual ~UserInfoIfFactory() {}

  virtual UserInfoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(UserInfoIf* /* handler */) = 0;
};

class UserInfoIfSingletonFactory : virtual public UserInfoIfFactory {
 public:
  UserInfoIfSingletonFactory(const boost::shared_ptr<UserInfoIf>& iface) : iface_(iface) {}
  virtual ~UserInfoIfSingletonFactory() {}

  virtual UserInfoIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(UserInfoIf* /* handler */) {}

 protected:
  boost::shared_ptr<UserInfoIf> iface_;
};

class UserInfoNull : virtual public UserInfoIf {
 public:
  virtual ~UserInfoNull() {}
  void GetUserId(std::string& /* _return */, const std::string& /* username */) {
    return;
  }
};


class UserInfo_GetUserId_args {
 public:

  UserInfo_GetUserId_args() : username() {
  }

  virtual ~UserInfo_GetUserId_args() throw() {}

  std::string username;

  void __set_username(const std::string& val) {
    username = val;
  }

  bool operator == (const UserInfo_GetUserId_args & rhs) const
  {
    if (!(username == rhs.username))
      return false;
    return true;
  }
  bool operator != (const UserInfo_GetUserId_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserInfo_GetUserId_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class UserInfo_GetUserId_pargs {
 public:


  virtual ~UserInfo_GetUserId_pargs() throw() {}

  const std::string* username;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _UserInfo_GetUserId_result__isset {
  _UserInfo_GetUserId_result__isset() : success(false) {}
  bool success;
} _UserInfo_GetUserId_result__isset;

class UserInfo_GetUserId_result {
 public:

  UserInfo_GetUserId_result() : success() {
  }

  virtual ~UserInfo_GetUserId_result() throw() {}

  std::string success;

  _UserInfo_GetUserId_result__isset __isset;

  void __set_success(const std::string& val) {
    success = val;
  }

  bool operator == (const UserInfo_GetUserId_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const UserInfo_GetUserId_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const UserInfo_GetUserId_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _UserInfo_GetUserId_presult__isset {
  _UserInfo_GetUserId_presult__isset() : success(false) {}
  bool success;
} _UserInfo_GetUserId_presult__isset;

class UserInfo_GetUserId_presult {
 public:


  virtual ~UserInfo_GetUserId_presult() throw() {}

  std::string* success;

  _UserInfo_GetUserId_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class UserInfoClient : virtual public UserInfoIf {
 public:
  UserInfoClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  UserInfoClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void GetUserId(std::string& _return, const std::string& username);
  void send_GetUserId(const std::string& username);
  void recv_GetUserId(std::string& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class UserInfoProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<UserInfoIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (UserInfoProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_GetUserId(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  UserInfoProcessor(boost::shared_ptr<UserInfoIf> iface) :
    iface_(iface) {
    processMap_["GetUserId"] = &UserInfoProcessor::process_GetUserId;
  }

  virtual ~UserInfoProcessor() {}
};

class UserInfoProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  UserInfoProcessorFactory(const ::boost::shared_ptr< UserInfoIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< UserInfoIfFactory > handlerFactory_;
};

class UserInfoMultiface : virtual public UserInfoIf {
 public:
  UserInfoMultiface(std::vector<boost::shared_ptr<UserInfoIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~UserInfoMultiface() {}
 protected:
  std::vector<boost::shared_ptr<UserInfoIf> > ifaces_;
  UserInfoMultiface() {}
  void add(boost::shared_ptr<UserInfoIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void GetUserId(std::string& _return, const std::string& username) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetUserId(_return, username);
    }
    ifaces_[i]->GetUserId(_return, username);
    return;
  }

};



#endif
