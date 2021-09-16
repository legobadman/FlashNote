#include "stdafx.h"
#include "transaction.h"


ITransaction::ITransaction()
{

}


bool ITransaction::Commit()
{
    if (State() == SS_START)
        return CommitImpl();
    return false;
}

bool ITransaction::Rollback()
{
    if (State() == SS_COMMITED)
        return RollbackImpl();
    return false;
}

bool ITransaction::Undo()
{
    return false;
}

bool ITransaction::Redo()
{
    return false;
}


TranRepository::TranRepository()
    : m_curr_idx(0)
{

}

void TranRepository::Undo()
{

}

void TranRepository::Redo()
{

}

void TranRepository::Rollback(TRAN_OPERATOR ope)
{
    //找到ope的start标志的trans
    int end_idx = m_transactions.size() - 1;
    for (int i = end_idx; i >= 0; i--)
    {
        TRANSCATION_PTR spTrans = m_transactions[i];
        if (spTrans->GetOp() == ope && spTrans->State() == SS_START)
        {
            for (int j = end_idx; j > i; j--)
            {
                m_transactions[j]->Rollback();
            }
            //删掉这些事务。
            m_transactions.erase(m_transactions.begin() + i, m_transactions.end());
        }
    }
}

bool TranRepository::Commit(TRAN_OPERATOR ope)
{
    int end_idx = m_transactions.size() - 1;
    for (int i = end_idx; i >= 0; i--)
    {
        TRANSCATION_PTR spTrans = m_transactions[i];
        if (spTrans->GetOp() == ope && spTrans->State() == SS_START)
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
            //删掉这些事务。
            m_transactions.erase(m_transactions.begin() + i + 1, m_transactions.end());
            return true;
        }
    }
    return false;
}

void TranRepository::Add(TRANSCATION_PTR spTrans)
{
    m_transactions.push_back(spTrans);
}