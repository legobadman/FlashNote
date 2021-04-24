#ifndef __ALIGNMENT_COMBOBOX_H__
#define __ALIGNMENT_COMBOBOX_H__

//#define USE_QCOMBOBOX
class PopupWidget;

#ifdef USE_QCOMBOBOX
class AlignmentComboBox : public QComboBox
{
    Q_OBJECT
public:
    AlignmentComboBox(QWidget* parent = nullptr);
    ~AlignmentComboBox();

protected:
    void paintEvent(QPaintEvent* e) override;
};
#else

class AlignmentComboBox : public MenuButton
{
    Q_OBJECT
public:
    AlignmentComboBox(QWidget* parent = nullptr);
    ~AlignmentComboBox();

signals:
    void alignmentChanged(int);

public slots:
    void popup();

private:
    PopupWidget* m_popup;
    QListView* m_pView;
    QStandardItemModel* m_pModel;
};

#endif


#endif
