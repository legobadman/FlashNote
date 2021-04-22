#ifndef MTEXTEDIT_H_
#define MTEXTEDIT_H_

#include <QTextEdit>
#include <QMimeData>
#include <QtGui/QImage>

class MTextEdit : public QTextEdit {
    Q_OBJECT
  public:
    MTextEdit(QWidget *parent);

    void        dropImage(const QImage& image, const QString& format);

  protected:
    bool        canInsertFromMimeData(const QMimeData *source) const;
    void        insertFromMimeData(const QMimeData *source);
    QMimeData  *createMimeDataFromSelection() const;

};

#endif
