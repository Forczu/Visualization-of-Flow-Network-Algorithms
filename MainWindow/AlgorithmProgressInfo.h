#pragma once
#include <QString>
#include <QMap>
class AlgorithmProgressInfo
{
	QMap<int, QString> _infoMap;
public:
	inline QString getInformation(int key) const { return _infoMap[key]; }
	void setInformation(int key, QString const & value);
	QString operator[](int const key) const;
};

