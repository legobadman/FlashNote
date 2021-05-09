// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "UserInfo.h"
#include <iostream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::document;


using boost::shared_ptr;

mongocxx::instance inst{};
mongocxx::client conn{mongocxx::uri{}};

class UserInfoHandler : virtual public UserInfoIf
{
 public:
  UserInfoHandler() {
    // Your initialization goes here
  }

  void GetUserId(std::string& _return, const std::string& username)
  {
    auto collection = conn["flashnote"]["user"];
    bsoncxx::stdx::optional<bsoncxx::document::value> maybe_result =
      collection.find_one(document{} << "user" << username << finalize);

    if (maybe_result)
    {
      bsoncxx::document::view view = maybe_result->view();
      bsoncxx::document::element element = view["_id"];
      bsoncxx::type type = element.type(); 
      if (type == bsoncxx::type::k_oid)
      {
        _return = element.get_oid().value.to_string();
      }
      std::string wtf = bsoncxx::to_string(type);
      std::cout << wtf << std::endl;
      std::cout << bsoncxx::to_json(*maybe_result) << std::endl;
    }
    printf("GetUserId\n");
  }

};

int main(int argc, char **argv)
{
  int port = 81;

  shared_ptr<UserInfoHandler> handler(new UserInfoHandler());
  shared_ptr<TProcessor> processor(new UserInfoProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

