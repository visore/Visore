#ifndef VIPROJECTINFO_H
#define VIPROJECTINFO_H

class ViProjectInfo
{

	public:

		ViProjectInfo();
		bool isNull();

	protected:

		void setNull(bool null);

	private:

		bool mIsNull;

};

#endif
