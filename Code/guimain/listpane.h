#ifndef __LIST_PANE__H__
#define __LIST_PANE__H__

enum MOUSE_HINT
{
	MOUSE_IN_EXPAND,
	MOUSE_IN_ADD,
	MOUSE_IN_SEARCH,

	MOUSE_IN_TEXT,
	MOUSE_IN_RIGHTMENU,

	MOUSE_IN_OTHER,
};

#define NEW_NOTE_WIDGET_WIDTH 255
#define NEW_NOTE_MENU_ITEM_HEIGHT 40
#define NEW_NOTE_MENU_ITEM_WIDTH 182

class NewNoteItem : public QWidget
{
	Q_OBJECT

	enum MENU_ITEM
	{
		MENU_NEWNOTE,
		MENU_MINDMAP,
		MENU_SCHEDULE,
	};

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
	void mouseMoveEvent(QMouseEvent* e);

signals:
	void LButtonPressed();
	void newnote(NOTE_TYPE);
	void rightmenu();

private slots:
	void MenuActionSlot(QAction* action);

private:
	void setPressed(bool bPressed);
	bool isDown() const { return m_bPressed; }
	void updateHoverState(QPoint pos);

private:
	MOUSE_HINT m_hoverObj;

	const int text_hover_start = 15;
	const int text_hover_end = 126;
	const int menu_hover_end = NEW_NOTE_WIDGET_WIDTH - 20;
	const int menu_hover_start = menu_hover_end - 16;

	QMenu* m_pCustomMenu;

	bool m_bPressed;
};

class NewNoteMenu : public QListView
{
	Q_OBJECT
public:
	NewNoteMenu(QWidget* parent = nullptr);
	~NewNoteMenu();

public slots:
	void onIndexClicked(const QModelIndex& index);

signals:
	void newnote(NOTE_TYPE);
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
{
	Q_OBJECT
	enum MENU_ITEM
	{
		DELETE_NOTEBOOK = 0,
		DELETE_SCHEDULE,
	};

	struct NavigationPanelNotifier : public ICoreNotify
	{
		NavigationPanelNotifier(NavigationPanel* pWidget) : m_pWidget(pWidget) {}
		HRESULT onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg) {
			if (m_pWidget)
				return m_pWidget->onCoreNotify(pCoreObj, arg);
			else
				return E_NOTIMPL;
		}
		NavigationPanel* m_pWidget;
	};

public:
	NavigationPanel(QWidget* parent = nullptr);
	~NavigationPanel();
	QTreeView* treeview() { return m_treeview; }

	HRESULT onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg);

public:
	HRESULT QueryInterface(QUuid, void **) { return E_NOTIMPL; }
	long AddRef(void) { return 1; }
	long Release(void) { return 1; }

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void initNotebookItem();
	void initSchedule();
	void initModel();

signals:
	void newnote(NOTE_TYPE);
	void expand_changed(const QModelIndex&);
	void addnotebook();
	void addschedule();
	void clicked(const QModelIndex&);
	void currentChanged(const QModelIndex&);

private slots:
	void onObjClick(const QModelIndex&, MOUSE_HINT);
	void onTreeItemSelected(const QModelIndex& curr, const QModelIndex& prev);
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction* action);

private:
	QStandardItemModel* m_model;
	NoteItemTreeView* m_treeview;
	NewNoteItem* m_newnote;
	QMenu* m_pCustomMenu;
	shared_ptr<NavigationPanelNotifier> m_spNotifier;
};

#endif
