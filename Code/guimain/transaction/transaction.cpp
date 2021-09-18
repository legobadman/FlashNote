#include "stdafx.h"
#include "transaction.h"


int Transaction::nextId = 0;

Transaction::Transaction()
    : m_state(SS_UNKNOWN)
    , m_id(nextId++)
{
}

bool Transaction::Commit()
{
	if (!m_transform)
		return false;
    if (m_state == SS_START)
    {
        //如果是走文档事务的话不需要Commit执行，只要标记状态即可。
        bool ret = true;//m_transform->forward();
        if (ret)
        {
            m_state = SS_COMMITED;
            return true;
        }
    }
    return false;
}

bool Transaction::Rollback()
{
    if (!m_transform)
        return false;

    bool ret = m_transform->backward();
    m_state = ret ? SS_ROLLBACKED : SS_UNKNOWN;
    return ret;
}

bool Transaction::Undo()
{
	if (!m_transform)
		return false;
    return m_transform->backward();
}

bool Transaction::Redo()
{
	if (!m_transform)
		return false;
    return m_transform->forward();
}


TranRepository::TranRepository()
    : m_curr_idx(0)
{
    m_transactions.push_back(NULL); //方便idx指引。
}

void TranRepository::Undo()
{
    if (TRANSCATION_PTR sptr = m_transactions[m_curr_idx])
    {
        sptr->Undo();
        m_curr_idx--;
    }
}

void TranRepository::Redo()
{
    if (m_curr_idx < m_transactions.size() - 1)
    {
        if (TRANSCATION_PTR sptr = m_transactions[m_curr_idx + 1])
        {
            sptr->Redo();
            m_curr_idx++;
        }
    }
}

bool TranRepository::enableUndo()
{
    return (m_curr_idx > 0);
}

bool TranRepository::enableRedo()
{
    return (m_curr_idx < m_transactions.size() - 1);
}

void TranRepository::Rollback(int id)
{
    //找到ope的start标志的trans
    int end_idx = m_transactions.size() - 1;
    for (int i = end_idx; i >= 0; i--)
    {
        TRANSCATION_PTR spTrans = m_transactions[i];
        if (spTrans->GetId() == id && spTrans->State() == SS_START)
        {
            for (int j = end_idx; j > i; j--)
            {
                m_transactions[j]->Rollback();
            }
            m_transactions[i]->Rollback();
            //删掉这些事务。
            m_transactions.erase(m_transactions.begin() + i, m_transactions.end());
            m_curr_idx = m_transactions.size() - 1;
            return;
        }
    }
}

bool TranRepository::Commit(int id)
{
    int end_idx = m_transactions.size() - 1;
    for (int i = end_idx; i > 0; i--)
    {
        TRANSCATION_PTR spTrans = m_transactions[i];
        if (spTrans->GetId() == id && spTrans->State() == SS_START)
        {
            bool ret = spTrans->Commit();
            if (!ret)
            {
                //之前所有记录都要rollback
                for (int j = end_idx; j > i; j--)
                {
                    m_transactions[j]->Rollback();
                }
                //spTrans->Rollback();
                return false;
            }
            //删掉子事务。
            m_transactions.erase(m_transactions.begin() + i + 1, m_transactions.end());
            m_curr_idx = m_transactions.size() - 1;
            return true;
        }
    }
    return false;
}

bool TranRepository::Cancel(int id)
{
    int end_idx = m_transactions.size() - 1;
    for (int i = end_idx; i > 0; i--)
    {
        TRANSCATION_PTR spTrans = m_transactions[i];
        if (spTrans->GetId() == id)
        {
            m_transactions.erase(m_transactions.begin() + i);
            return true;
        }
    }
    return false;
}

void TranRepository::Add(TRANSCATION_PTR spTrans)
{
    //擦掉m_curr_idx+1之后的内容
    m_transactions.erase(m_transactions.begin() + m_curr_idx + 1, m_transactions.end());
    m_transactions.push_back(spTrans);
    m_curr_idx = m_transactions.size() - 1;
}

int TranRepository::AddExecute(TRANSFORM_PTR pTransform)
{
    TRANSFORM_PTR sptr(pTransform);
    TRANSCATION_PTR spTrans(new Transaction);
    spTrans->SetTransform(sptr);
    spTrans->SetState(SS_START);
    Add(spTrans);
    int id = spTrans->GetId();
    Commit(id);
    return id;
}

int TranRepository::AddBatch(TRANSFORM_PTR pTransform)
{
    TRANSFORM_PTR sptr(pTransform);
    TRANSCATION_PTR spTrans(new Transaction);
    spTrans->SetTransform(sptr);
    spTrans->SetState(SS_START);
    Add(spTrans);
    return spTrans->GetId();
}

void TranRepository::Clear()
{
    m_transactions.clear();
}