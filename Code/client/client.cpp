#include <stdio.h>
#include <iostream>
#include <string>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>
#include "hello_types.h"
#include "Hello.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char** argv) {

	shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	HelloClient client(protocol);

	transport->open();
	string hello;
	client.getString(hello);
	cout<<hello;
	transport->close();

	return 0;
}
