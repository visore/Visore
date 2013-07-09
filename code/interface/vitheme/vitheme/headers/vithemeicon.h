#ifndef VITHEMEICON_H
#define VITHEMEICON_H

#include <vithemeresource.h>

class ViThemeIcon : public ViThemeResource
{

	public:

		enum Mode
		{
			Normal = 0,
			Selected = 1,
			Hovered = 2,
			Disabled = 3,
			Enabled = Normal
		};

	public:

		ViThemeIcon(QString name = "");
		ViThemeIcon(const ViThemeIcon &other);
		
		QIcon icon(ViThemeIcon::Mode mode = ViThemeIcon::Normal, int size = -1) const;
		QImage image(ViThemeIcon::Mode mode = ViThemeIcon::Normal, int size = -1) const;
		QPixmap pixmap(ViThemeIcon::Mode mode = ViThemeIcon::Normal, int size = -1) const;

		QString path(ViThemeIcon::Mode mode = ViThemeIcon::Normal) const;

		void set(const ViThemeIcon::Mode &mode, const ViThemeIcon &icon, const ViThemeIcon::Mode &iconMode = ViThemeIcon::Normal); // Changes the icon from "mode" to the icon's "iconMode" image
		void set(const ViThemeIcon::Mode &mode, const QString &path);

	protected:

		QImage transform(ViThemeIcon::Mode mode) const;
		QRgb transformColor(QRgb rgb, const int &red, const int &green, const int &blue);

		static QRgb toSelectedColor(QRgb rgb);
		static QRgb toHoveredColor(QRgb rgb);
		static QRgb toDisabledColor(QRgb rgb);

	private:

		QString mNormalPath;
		QString mSelectedPath;
		QString mHoveredPath;
		QString mDisabledPath;

};

#endif
