#ifndef __LIST_PANE__H__
#define __LIST_PANE__H__


class NewNoteItem : public QWidget
{
	Q_OBJECT
public:
	NewNoteItem(QWidget* parent = nullptr);
	~NewNoteItem();

protected:
	void initStyleOption(QStyleOptionViewItem* option) const;
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);

signals:
	void LButtonPressed();
	void clicked();

private:
	void setPressed(bool bPressed);
	bool isDown() const { return m_bPressed; }

private:
	bool m_bPressed;
};

enum MOUSE_HINT
{
	MOUSE_IN_EXPAND,
	MOUSE_IN_ADD,
	MOUSE_IN_SEARCH,
	MOUSE_IN_OTHER,
};

class NoteItemTreeView : public QTreeView
{
	Q_OBJECT


public:
	NoteItemTreeView(QWidget* parent = nullptr);
	MOUSE_HINT GetHoverObj() { return m_hoverObj; }

protected:
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;

signals:
	void clickObj(const QModelIndex&, MOUSE_HINT);

private:
	void updateHoverState(QPoint pos);

private:
	MOUSE_HINT m_hoverObj;
	QPoint m_mousepos;
};

class NavigationPanel : public QWidget
					  , public ICoreNotify
{
	Q_OBJECT
	enum MENU_ITEM
	{
		DELETE_NOTEBOOK = 0,
	};

public:
	NavigationPanel(QWidget* parent = nullptr);
	~NavigationPanel();
	QTreeView* treeview() { return m_treeview; }

	HRESULT STDMETHODCALLTYPE onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void initNotebookItem();
	void initModel();

signals:
	void newnote();
	void expand_changed(const QModelIndex&);
	void addnotebook();
	void clicked(const QModelIndex&);

private slots:
	void onObjClick(const QModelIndex&, MOUSE_HINT);
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction* action);

private:
	QStandardItemModel* m_model;
	NoteItemTreeView* m_treeview;
	NewNoteItem* m_newnote;
	QMenu* m_pCustomMenu;
};

#endif
