#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__




enum TRAN_STATE
{
    SS_UNKNOWN,
    SS_START,
    SS_COMMITED,
    SS_ROLLBACKED,
};

class ITransaction
{
public:
    ITransaction();
    bool Commit();
    bool Rollback();    //ֻ�д���COMMITED״̬�ļ�¼����rollback
    bool Undo();
    bool Redo();
    TRAN_STATE State() const { return m_state; }
    void SetState(TRAN_STATE state) { m_state = state; }
    int GetId() const { return m_id; };

protected:
    static int nextId;

private:
    virtual bool Forward() = 0;
    virtual bool Backward() = 0;

    TRAN_STATE m_state;
    int m_id;
};

typedef shared_ptr<ITransaction> TRANSCATION_PTR;

class TranRepository
{
public:
    TranRepository();
    void Undo();
    void Redo();
    void Rollback(int id);
    bool Commit(int id);
    void Add(TRANSCATION_PTR spTrans);      //�ͻ�beginTransactionʱ���д���trans�����롣
    void Clear();

private:
    vector<TRANSCATION_PTR> m_transactions;
    int m_curr_idx;
};


#endif