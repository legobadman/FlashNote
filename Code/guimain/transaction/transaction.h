#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__


enum TRAN_STATE
{
    SS_UNKNOWN,
    SS_START,
    SS_COMMITED,
    SS_ROLLBACKED,
};

interface ITransform
{
    virtual bool prepare() = 0;
    virtual bool forward() = 0;
    virtual bool backward() = 0;
};

typedef shared_ptr<ITransform> TRANSFORM_PTR;

class Transaction
{
public:
    Transaction();
    void SetTransform(TRANSFORM_PTR sptr) { m_transform = sptr; }
    bool Commit();
    bool Rollback();    //只有处在COMMITED状态的记录才能rollback
    bool Undo();
    bool Redo();
    TRAN_STATE State() const { return m_state; }
    void SetState(TRAN_STATE state) { m_state = state; }
    int GetId() const { return m_id; };

protected:
    static int nextId;

private:
    TRAN_STATE m_state;
    TRANSFORM_PTR m_transform;
    int m_id;
};

typedef shared_ptr<Transaction> TRANSCATION_PTR;

class TranRepository
{
public:
    TranRepository();
    void Undo();
    void Redo();
    void Rollback(int id);
    bool Commit(int id);
    bool Cancel(int id);
    void Add(TRANSCATION_PTR spTrans);      //客户beginTransaction时自行创建trans并插入。
    int AddExecute(TRANSFORM_PTR pTransform);
    int AddBatch(TRANSFORM_PTR pTransform);
    void Clear();
    bool enableUndo();
    bool enableRedo();

private:
    vector<TRANSCATION_PTR> m_transactions;
    int m_curr_idx;
};


#endif