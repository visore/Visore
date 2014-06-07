#ifndef VIEIGEN_H
#define VIEIGEN_H

#include <vieigenfloat.h>
#include <vieigendouble.h>
#include <vieigenmpreal.h>
#include <QHash>

typedef ViEigenFloat ViEigen24;
typedef ViEigenDouble ViEigen53;
typedef ViEigenMpreal<64> ViEigen64;
typedef ViEigenMpreal<72> ViEigen72;
typedef ViEigenMpreal<80> ViEigen80;
typedef ViEigenMpreal<88> ViEigen88;
typedef ViEigenMpreal<96> ViEigen96;
typedef ViEigenMpreal<104> ViEigen104;
typedef ViEigenMpreal<112> ViEigen112;
typedef ViEigenMpreal<120> ViEigen120;
typedef ViEigenMpreal<128> ViEigen128;
typedef ViEigenMpreal<144> ViEigen144;
typedef ViEigenMpreal<160> ViEigen160;
typedef ViEigenMpreal<176> ViEigen176;
typedef ViEigenMpreal<192> ViEigen192;
typedef ViEigenMpreal<208> ViEigen208;
typedef ViEigenMpreal<224> ViEigen224;
typedef ViEigenMpreal<240> ViEigen240;
typedef ViEigenMpreal<256> ViEigen256;
typedef ViEigenMpreal<288> ViEigen288;
typedef ViEigenMpreal<320> ViEigen320;
typedef ViEigenMpreal<352> ViEigen352;
typedef ViEigenMpreal<384> ViEigen384;
typedef ViEigenMpreal<416> ViEigen416;
typedef ViEigenMpreal<448> ViEigen448;
typedef ViEigenMpreal<480> ViEigen480;
typedef ViEigenMpreal<512> ViEigen512;
typedef ViEigenMpreal<640> ViEigen640;
typedef ViEigenMpreal<768> ViEigen768;
typedef ViEigenMpreal<896> ViEigen896;
typedef ViEigenMpreal<1024> ViEigen1024;

class ViEigenManager
{

	public:

		static int bitPrecision(const int &decimalPrecision);
		static bool isSupported(const int &decimalPrecision);
		static ViEigenBase* getByBits(const int &bitPrecision);
		static ViEigenBase* getByDecimals(const int &decimalPrecision);

	private:

		static void initialize();

	private:

		static bool mInitialized;
		static QHash<int, ViEigenBase*> mEigens;

};

#endif
