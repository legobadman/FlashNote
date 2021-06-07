#ifndef __CPP_SYNTAX_HIGHLIGHTER_H__
#define __CPP_SYNTAX_HIGHLIGHTER_H__

#include <QtGui/QSyntaxHighlighter>

class CppSyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	CppSyntaxHighlighter(QTextDocument* parent = 0);

protected:
	void highlightBlock(const QString& text) override;

private:
	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegularExpression commentStartExpression;
	QRegularExpression commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
};



#endif
