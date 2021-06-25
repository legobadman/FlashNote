#include "stdafx.h"
#include "notebookmodel.h"
#include "guihelper.h"
#include "LeftSideItemDelegate.h"


NotebookModel::NotebookModel(QObject* parent)
	: QStandardItemModel(parent)
{
	com_sptr<INoteApplication> spApp = coreApp;
	com_sptr<INotebooks> spNotebooks;
	spApp->GetNotebooks(&spNotebooks);

	int count = 0;
	spNotebooks->GetCount(&count);

	for (int i = 0; i < count; i++)
	{
		VARIANT index;
		V_VT(&index) = VT_I4;
		V_I4(&index) = i;

		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(index, &spNotebook);

		QString bookName = AppHelper::GetNotebookName(spNotebook);
		QString bookId = AppHelper::GetNotebookId(spNotebook);

		QStandardItem* pItem = new QStandardItem(bookName);
		pItem->setEditable(false);
		pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
			ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM), ItemContentTypeRole);
		pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
			ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
		pItem->setData(QVariant(bookId), ItemCoreObjIdRole);
		pItem->setIcon(QIcon(":/icons/16x16/notebookmenu.png"));

		appendRow(pItem);
	}
}

NotebookModel::~NotebookModel()
{

}