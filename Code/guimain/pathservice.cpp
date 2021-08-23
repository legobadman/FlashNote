#include "stdafx.h"
#include "pathservice.h"
#include <QStandardPaths>


PathService& PathService::instance()
{
	static PathService service;
	return service;
}

PathService::PathService()
{
	_ensurePath();
}

void PathService::_ensurePath()
{
	QStringList locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
	QString appData;

#ifdef Q_OS_WIN
	locations = locations.filter("ProgramData");
	if (locations.empty())
	{
		//TODO
	}
#else
	if (locations.empty())
	{
		//TODO
	}
#endif

	appData = locations[0];

	m_appDataPath.setPath(appData);
	if (!m_appDataPath.exists())
		m_appDataPath.mkdir(".");

	m_dbPath.setPath(appData + "/" + "database");
	if (!m_dbPath.exists())
		m_dbPath.mkdir(".");

	m_assetsPath.setPath(appData + "/" + "assets");
	if (!m_assetsPath.exists())
		m_assetsPath.mkdir(".");
}

QString PathService::GetDbPath()
{
	_ensurePath();
	return m_dbPath.path();
}

QString PathService::GetAssetsPath()
{
	_ensurePath();
	return m_assetsPath.path();
}

bool PathService::checkPathValid()
{
	return m_appDataPath.exists() && m_dbPath.exists() && m_assetsPath.exists();
}