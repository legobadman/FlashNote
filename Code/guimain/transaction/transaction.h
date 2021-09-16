#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__


//�ͻ���Ҫ������Ǽ�ID�������ɢ��ȥ�����ص���
enum TRAN_OPERATOR
{
    TOP_ADD,
    TOP_MOVE,
    TOP_REMOVE,
    TOP_EDITTEXT,
};

enum TRAN_STATE
{
    SS_START,
    SS_COMMITED
};

class ITransaction
{
public:
    ITransaction();
    bool Commit();
    bool Rollback();    //ֻ�д���COMMITED״̬�ļ�¼����rollback
    bool Undo();
    bool Redo();
    virtual TRAN_STATE State() = 0;
    virtual TRAN_OPERATOR GetOp() = 0;

private:
    virtual bool CommitImpl() = 0;
    virtual bool RollbackImpl() = 0;

};

typedef shared_ptr<ITransaction> TRANSCATION_PTR;

class TranRepository
{
public:
    TranRepository();
    void Undo();
    void Redo();
    void Rollback(TRAN_OPERATOR ope);
    bool Commit(TRAN_OPERATOR ope);
    void Add(TRANSCATION_PTR spTrans);      //�ͻ�beginTransactionʱ���д���trans�����롣

private:
    vector<TRANSCATION_PTR> m_transactions;
    int m_curr_idx;
};


#endif