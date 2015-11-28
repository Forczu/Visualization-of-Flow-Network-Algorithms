#include "CheckInfo.h"


CheckInfo& CheckInfo::operator+=(QString const& info)
{
	_info.append("* " + info + '\n');
	return *this;
}

CheckInfo::CheckInfo(bool succeeded)
{
	setSucceeded(succeeded);
}
