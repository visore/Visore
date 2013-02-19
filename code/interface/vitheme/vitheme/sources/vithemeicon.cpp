#include <vithemeicon.h>

ViThemeIcon::ViThemeIcon(QString name)
	: ViThemeResource()
{
	setDirectory(":/icons/" + name);

	mNormalPath = generatePath("normal");
	mSelectedPath = generatePath("selected");
	mHoveredPath = generatePath("hovered");
	mDisabledPath = generatePath("disbaled");
}

ViThemeIcon::ViThemeIcon(const ViThemeIcon &other)
	: ViThemeResource(other)
{
	mNormalPath = other.mNormalPath;
	mSelectedPath = other.mSelectedPath;
	mHoveredPath = other.mHoveredPath;
	mDisabledPath = other.mDisabledPath;
}

QIcon ViThemeIcon::icon(ViThemeIcon::Mode mode, int size)
{
	return QIcon(pixmap(mode, size));
}

QImage ViThemeIcon::image(ViThemeIcon::Mode mode, int size)
{
	if(size < 0)
	{
		return transform(mode);
	}
	return transform(mode).scaledToWidth(size, Qt::SmoothTransformation);
}

QPixmap ViThemeIcon::pixmap(ViThemeIcon::Mode mode, int size)
{
	return QPixmap::fromImage(image(mode, size));
}

QString ViThemeIcon::path(ViThemeIcon::Mode mode) const
{
	if(mode == ViThemeIcon::Normal)
	{
		return mNormalPath;
	}
	else if(mode == ViThemeIcon::Selected)
	{
		return mSelectedPath;
	}
	else if(mode == ViThemeIcon::Hovered)
	{
		return mHoveredPath;
	}
	else if(mode == ViThemeIcon::Disabled)
	{
		return mDisabledPath;
	}
	return "";
}

void ViThemeIcon::set(const ViThemeIcon::Mode &mode, const ViThemeIcon &icon, const ViThemeIcon::Mode &iconMode)
{
	set(mode, icon.path(iconMode));
}

void ViThemeIcon::set(const ViThemeIcon::Mode &mode, const QString &path)
{
	if(mode == ViThemeIcon::Normal)
	{
		mNormalPath = path;
	}
	else if(mode == ViThemeIcon::Selected)
	{
		mSelectedPath = path;
	}
	else if(mode == ViThemeIcon::Hovered)
	{
		mHoveredPath = path;
	}
	else if(mode == ViThemeIcon::Disabled)
	{
		mDisabledPath = path;
	}
}

QImage ViThemeIcon::transform(ViThemeIcon::Mode mode)
{
	QImage image = QImage(path());
	if(mode == ViThemeIcon::Normal)
	{
		return image;
	}
	if(path(mode) == "")
	{
		if(mode == ViThemeIcon::Selected && path(ViThemeIcon::Hovered) != "")
		{
			image = QImage(path(ViThemeIcon::Hovered));
		}

		QRgb (*convert)(QRgb);
		if(mode == ViThemeIcon::Selected)
		{
			convert = &ViThemeIcon::toSelectedColor;
		}
		else if(mode == ViThemeIcon::Hovered)
		{
			convert = &ViThemeIcon::toHoveredColor;
		}
		else if(mode == ViThemeIcon::Disabled)
		{
			convert = &ViThemeIcon::toDisabledColor;
		}

		QRgb oldColor;
		for(int x = 0; x < image.width(); ++x)
		{
			for(int y = 0; y < image.height(); ++y)
			{
				oldColor = image.pixel(x, y);
				if(qAlpha(oldColor) > 222)
				{
					image.setPixel(x, y, convert(oldColor));
				}
			}
		}
	}
	else
	{
		image = QImage(path(mode));
	}
	return image;
}

QRgb ViThemeIcon::toSelectedColor(QRgb rgb)
{
	QColor color(rgb);
	int value;

	value = color.red() - 20;
	if(value < 0) color.setRed(0);
	else color.setRed(value);

	value = color.green() - 20;
	if(value < 0) color.setGreen(0);
	else color.setGreen(value);

	value = color.blue() - 20;
	if(value < 0) color.setBlue(0);
	else color.setBlue(value);

	return color.rgba();
}

QRgb ViThemeIcon::toHoveredColor(QRgb rgb)
{
	QColor color(rgb);
	int value;

	value = color.red() + 20;
	if(value > 255) color.setRed(255);
	else color.setRed(value);

	value = color.green() + 20;
	if(value > 255) color.setGreen(255);
	else color.setGreen(value);

	value = color.blue() + 20;
	if(value > 255) color.setBlue(255);
	else color.setBlue(value);

	return color.rgba();
}

QRgb ViThemeIcon::toDisabledColor(QRgb rgb)
{
	QColor color(rgb);
	color.setRed(color.green());
	color.setBlue(color.green());
	return color.rgba();
}

QRgb ViThemeIcon::transformColor(QRgb rgb, const int &red, const int &green, const int &blue)
{
	QColor color(rgb);

	int value = color.red() + red;
	if(value > 255)
	{
		color.setRed(255);
	}
	else
	{
		color.setRed(value);
	}

	value = color.green() + green;
	if(value > 255)
	{
		color.setGreen(255);
	}
	else
	{
		color.setGreen(value);
	}

	value = color.blue() + blue;
	if(value > 255)
	{
		color.setBlue(255);
	}
	else
	{
		color.setBlue(value);
	}

	return color.rgba();
}
