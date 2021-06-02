#include "thriftclient.h"

ThriftClient::ThriftClient()
	: m_pClient(NULL)
{
	m_socket.reset(new TSocket("120.78.150.174", 82));
	m_transport.reset(new TBufferedTransport(m_socket));
	m_protocol.reset(new TBinaryProtocol(m_transport));
	m_pClient = new NoteInfoClient(m_protocol);
	m_transport->open();
}

ThriftClient::~ThriftClient()
{
	delete m_pClient;
	m_pClient = NULL;
}
