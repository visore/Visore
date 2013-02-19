#ifndef VIMUSICBRAINZCOVERRETRIEVER_H
#define VIMUSICBRAINZCOVERRETRIEVER_H

#include <vicoverretriever.h>
#include <QDate>

class ViMusicBrainzCoverRetriever : public ViCoverRetriever
{

	Q_OBJECT

	private slots:

		void processReply(bool success);

	public:

		ViMusicBrainzCoverRetriever();

	protected:

		void retrieveUrls();
		QDate getDate(QString string);

};

#endif
