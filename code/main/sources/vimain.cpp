#include "vimainwindow.h"
#include "viaudioengine.h"
#include "vicoder.h"
#include <QApplication>
#include <viautoregresser.h>
#include <vifixedmodelorder.h>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	QIcon logo("logo.png");
	application.setWindowIcon(logo);
	application.setOrganizationName(ViManager::name());
	application.setOrganizationDomain(ViManager::url().toString());
	application.setApplicationName(ViManager::name());
	application.setApplicationVersion(ViManager::version().toString());

	//Register SIGNAL/SLOT parameter types
	qRegisterMetaType<ViAudioPosition>("ViAudioPosition");
    qRegisterMetaType<ViRealSpectrum>("ViRealSpectrum");




	/*ViSampleChunk c(16);
			c[0] = 0.1;
			c[1] = 0.15;
			c[2] = 	0.12;
			c[3] = 0.11;
			c[4] = 0.16;
			c[5] = 0.6;
			c[6] = 0.5;
			c[7] = 0.15;
			c[8] = 0.12;
			c[9] = 0.1;
			c[10] = 0.11;
			c[11] = 0.13;
			c[12] = 0.12;
			c[13] = 0.11;
			c[14] = 0.1;
			c[15] = 0.11;*/

	ViSampleChunk c(4);
				c[0] = 1;
				c[1] = 2;
				c[2] = 	3;
				c[3] = 4;

			ViFixedModelOrder *o = new ViFixedModelOrder();
			o->setOrder(2);

				ViAutoRegresser r;

			r.setModelOrder(o);

			cout<<"***: "<<r.calculate(c)<<" "<<r.coefficients().size()<<endl;

	ViVector v = r.coefficients();
	for(int i = 0; i < v.size(); ++i)
	{
		cout<<v[i]<<endl;
	}

	int x = 0;
	int y = 10/x;

    ViMainWindow *window = ViMainWindow::instance();
	window->setWindowIcon(logo);
    window->show();
	
    application.exec();

	return 0;
}

