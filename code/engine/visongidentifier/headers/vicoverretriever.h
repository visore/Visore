#ifndef VICOVERRETRIEVER_H
#define VICOVERRETRIEVER_H

#include <viwebservicer.h>
#include <vimetadata.h>
#include <vicommon.h>

class ViCoverRetriever : public QObject
{

	Q_OBJECT

	signals:

		void retrieved(bool success);

	private slots:

		void testImage(bool success);

	public:

		ViCoverRetriever();
		virtual ~ViCoverRetriever();
		void retrieve(ViMetadata metadata);
		ViMetadata metadata();

	protected:

		void processUrls(QStringQueue urls);
		virtual void retrieveUrls() = 0;

	private:

		void processNext(QString url);
		bool saveImage(QByteArray &data);

	protected:

		ViWebServicer mServicer;
		ViMetadata mMetadata;

	private:

		ViWebServicer mImageServicer;
		QStringQueue mUrls;

};

#endif
