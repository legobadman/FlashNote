#include "stdafx.h"
#include "transaction.h"


int ITransaction::nextId = 0;

ITransaction::ITransaction()
    : m_state(SS_UNKNOWN)
    , m_id(nextId++)
{
}

bool ITransaction::Commit()
{
    if (m_state == SS_START)
    {
        /*bool ret = Forward();*/
        m_state = SS_COMMITED;
        return true;
    }
    return false;
}

bool ITransaction::Rollback()
{
    if (m_state == SS_COMMITED)
    {
        bool ret = Backward();
        m_state = ret ? SS_ROLLBACKED : SS_UNKNOWN;
        return ret;
    }
    return false;
}

bool ITransaction::Undo()
{
    return Backward();
}

bool ITransaction::Redo()
{
    return Forward();
}


TranRepository::TranRepository()
    : m_curr_idx(0)
{
    m_transactions.push_back(NULL); //����idxָ����
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

void TranRepository::Rollback(int id)
{
    //�ҵ�ope��start��־��trans
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
            //ɾ����Щ����
            m_transactions.erase(m_transactions.begin() + i, m_transactions.end());
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
                //֮ǰ���м�¼��Ҫrollback
                for (int j = end_idx; j > i; j--)
                {
                    m_transactions[j]->Rollback();
                }
                //spTrans->Rollback();
                return false;
            }
            //ɾ����Щ����
            m_transactions.erase(m_transactions.begin() + i + 1, m_transactions.end());
            return true;
        }
    }
    return false;
}

void TranRepository::Add(TRANSCATION_PTR spTrans)
{
    m_transactions.push_back(spTrans);
    m_curr_idx++;
}

void TranRepository::Clear()
{
    m_transactions.clear();
}