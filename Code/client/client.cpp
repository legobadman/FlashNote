#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>
#include "hello_types.h"
#include "Hello.h"
#include "UserInfo.h"
#include "NoteInfo.h"

//using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using boost::shared_ptr;

//#define HELLO
#define USER_INFO
#define NOTE_INFO

int main(int argc, char** argv)
{
	shared_ptr<TTransport> socket(new TSocket("120.78.150.174", 81));
	shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	
	std::string hello;
#ifdef HELLO
	HelloClient client(protocol);
	transport->open();
	client.getString(hello);
	std::cout << hello;
#endif

#ifdef USER_INFO
	UserInfoClient client(protocol);
	transport->open();
	std::string userid;
	client.GetUserId(userid, "luzh");
	std::cout << userid;
#endif

#ifdef NOTE_INFO
	shared_ptr<TTransport> socket2(new TSocket("120.78.150.174", 82));
	shared_ptr<TTransport> transport2(new TBufferedTransport(socket2));
	shared_ptr<TProtocol> protocol2(new TBinaryProtocol(transport2));

	NoteInfoClient client2(protocol2);
	transport2->open();
	std::vector<Notebook> notebooks;
	client2.GetNotebooks(notebooks, userid);
	transport2->close();
#endif
	
	transport->close();

	return 0;
}
