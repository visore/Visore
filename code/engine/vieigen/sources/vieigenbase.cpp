#include <vieigenbase.h>

QString eigenToString(const double &value, const int &precision)
{
	return QString::number(value, 'g', precision);
}

QString eigenToString(const mpfr::mpreal &value, const int &precision)
{
	QString temp = QString::fromStdString(value.toString()).replace(",", ".");
	return QString::number(temp.toDouble(), 'g', precision);
}

void ViEigenBase::intialize(float &value)
{
}

void ViEigenBase::intialize(double &value)
{
}
