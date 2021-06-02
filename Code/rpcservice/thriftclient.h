#ifndef __THRIFT_CLIENT_H__
#define __THRIFT_CLIENT_H__

#include <boost/shared_ptr.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TTransportUtils.h>

#include "UserInfo.h"
#include "NoteInfo.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using boost::shared_ptr;


class ThriftClient
{
public:
	ThriftClient();
	~ThriftClient();
	NoteInfoClient* client() { return m_pClient; }

private:
	NoteInfoClient* m_pClient;
	boost::shared_ptr<TSocket> m_socket;
	boost::shared_ptr<TTransport> m_transport;
	boost::shared_ptr<TProtocol> m_protocol;
};

#endif
