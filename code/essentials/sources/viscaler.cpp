#include <viscaler.h>

double ViScaler::scale(const double &value, const double &fromMinimum, const double &fromMaximum, const double &toMinimum, const double &toMaximum)
{
	return (value - fromMinimum) * (toMaximum - toMinimum) / (fromMaximum - fromMinimum) + toMinimum;
}
