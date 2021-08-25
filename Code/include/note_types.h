#ifndef __NOTE_TYPES_H__
#define __NOTE_TYPES_H__

#include <string>
#include <QString>
#include <QDateTime>
#include <QVector>
#include <QMetaType>

struct NOTE
{
	QString title;
	QString text_abbre;
	QString content;
	QString creater;
	QString id;
	QDateTime create_time;
	QDateTime modify_time;
	NOTE_TYPE type;
	bool share;
};

Q_DECLARE_METATYPE(NOTE)

struct NOTEBOOK
{
	QVector<NOTE> notes;
	QString name;
	QString creater;	//name
	QString id;
	QDateTime create_time;
	QDateTime modify_time;
	bool share;
};

Q_DECLARE_METATYPE(NOTEBOOK)

#endif
