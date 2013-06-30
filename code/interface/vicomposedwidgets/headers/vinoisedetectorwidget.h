#ifndef VINOISEDETECTORWIDGET_H
#define VINOISEDETECTORWIDGET_H

#include <viwidget.h>
#include <vinoisedetector.h>

namespace Ui
{
    class ViNoiseDetectorWidget;
}

class ViNoiseDetectorWidget : public ViWidget
{

    Q_OBJECT

    private slots:

        void toggleDetector();

    public:

        ViNoiseDetectorWidget(QWidget *parent = 0);
        ~ViNoiseDetectorWidget();

        ViProcessor::ProcessMode processMode();
        ViModifyProcessor::ModifyMode modifyMode();

        ViNoiseDetector* detector(); //Ownership transfered to requestor

    private:

        Ui::ViNoiseDetectorWidget *mUi;

};

#endif
