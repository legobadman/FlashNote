#include "stdafx.h"
#include "mindtransaction.h"
#include "mindnode.h"


MindTransaction::MindTransaction(TRAN_OPERATOR ope, const MindNodeInfo& info)
    : m_ope(ope)
    , m_info(info)
{
}

MindTransaction::~MindTransaction()
{

}

bool MindTransaction::Forward()
{
    switch (m_ope)
    {
        case OP_ADD:
        {
            MindNode* pChild = m_info.parnetNode->NewChildImpl(m_info.bToRight);
            m_info.childNode = pChild;
            break;
        }
    }
    return true;
}

bool MindTransaction::Backward()
{
    switch (m_ope)
    {
        case OP_ADD:
        {
            //m_info.parnetNode->removeChild(m_info.childNode);
            m_info.childNode->onDeleteNode();
            break;
        }
    }
    return false;
}