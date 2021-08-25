#include "stdafx.h"
#include "selectnotebookdlg.h"
#include "guihelper.h"
#include "ui_selectnotebookdlg.h"
#include "LeftSideItemDelegate.h"


SelectNotebookDlg::SelectNotebookDlg(QDialog* parent)
	: QDialog(parent)
{
	m_ui = new Ui::SelectNotebookDlg();

	m_ui->setupUi(this);

	connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QStandardItemModel* model = new QStandardItemModel;

	com_sptr<INoteApplication> spApp = AppHelper::coreApp();
	com_sptr<INotebooks> spNotebooks;
	spApp->GetNotebooks(&spNotebooks);

	int count = 0;
	spNotebooks->GetCount(&count);

	for (int i = 0; i < count; i++)
	{
		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(i, &spNotebook);

		QString bookName = AppHelper::GetNotebookName(spNotebook);
		QString bookId = AppHelper::GetNotebookId(spNotebook);

		QStandardItem* pItem = new QStandardItem(bookName);
		pItem->setEditable(false);
		pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
			ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM), ItemContentTypeRole);
		pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
			ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
		pItem->setData(QVariant(bookId), ItemCoreObjIdRole);

		model->appendRow(pItem);
	}
	m_ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ui->listView->setModel(model);
}

QString SelectNotebookDlg::getBookId()
{
	QModelIndex idx = m_ui->listView->currentIndex();
	return idx.data(ItemCoreObjIdRole).toString();
}