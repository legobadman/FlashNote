#ifndef __PATH_SERVICE_H__
#define __PATH_SERVICE_H__

#include <QDir>

class PathService
{
public:
	static PathService& instance();
	QString GetDbPath();
	QString GetAssetsPath();
	bool checkPathValid();

private:
	PathService();
	void _ensurePath();

private:
	QDir m_appDataPath;
	QDir m_dbPath;
	QDir m_assetsPath;
};


#endif