#ifndef __MIND_TRANSACTION_H__
#define __MIND_TRANSACTION_H__

#include "../transaction/transaction.h"

class MindNode;

struct MindNodeInfo
{
    MindNode* parnetNode;
    MindNode* childNode;
    QString editText;
    bool bToRight;

    MindNodeInfo() : parnetNode(NULL), childNode(NULL), bToRight(false) {}
};

//�ͻ���Ҫ������Ǽ�ID�������ɢ��ȥ�����ص���
enum TRAN_OPERATOR
{
    OP_NONE,
    OP_ADD,
    OP_MOVE,
    OP_REMOVE,
    OP_EDITTEXT,
};

class MindTransaction : public ITransaction
{
public:
    MindTransaction(TRAN_OPERATOR ope, const MindNodeInfo& info);
    ~MindTransaction();

private:
    virtual bool Forward() override;
    virtual bool Backward() override;

    TRAN_OPERATOR m_ope;
    MindNodeInfo m_info;
};


#endif