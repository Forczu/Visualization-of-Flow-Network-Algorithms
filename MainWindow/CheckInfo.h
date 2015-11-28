#pragma once
#include <QString>
class CheckInfo
{
	bool _succeeded;
	QString _info;
public:
	CheckInfo(bool succeeded);

	inline bool didSucceeded() const { return _succeeded; }
	void setSucceeded(bool succeeded) { _succeeded = succeeded; }
	inline QString getInformation() const { return _info; }

	CheckInfo& operator+=(QString const& info);
};

