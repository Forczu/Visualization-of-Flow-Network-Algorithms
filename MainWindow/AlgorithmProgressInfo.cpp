#include "AlgorithmProgressInfo.h"

void AlgorithmProgressInfo::setInformation(int key, QString const & value)
{
	_infoMap[key] = "> " + value + '\n';
}

QString AlgorithmProgressInfo::operator[](int const key) const
{
	return _infoMap[key];
}
