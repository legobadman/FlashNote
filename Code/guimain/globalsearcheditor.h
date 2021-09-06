#ifndef __GLOBAL_SEARCH_EDITOR_H__
#define __GLOBAL_SEARCH_EDITOR_H__

class GlobalSearchEditor : public QWidget
{
	Q_OBJECT
public:
	GlobalSearchEditor(QWidget* parent = NULL);
	~GlobalSearchEditor();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

private:
	QPoint m_origin_pos;
};

#endif
