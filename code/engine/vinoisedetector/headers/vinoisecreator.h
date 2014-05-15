#ifndef VINOISECREATOR_H
#define VINOISECREATOR_H

#include <vichunk.h>

class ViBuffer;

class ViNoiseCreator
{
	//
	//		1.0		....				....					   /\							  /\		 /\
	//				|	|				|	|					  /	 \							 |	|		|  |
	//				|	|				|	|					 /	  \							 |	|		|  |
	//				|	|				|	|					/	   \						/	 \	   /	\
	//		0.5	....	....		....	|	....		....		\		....		....	 |	   |	|	  ....
	//										|	|						 \		/					 \	  /		\	 /
	//										|	|						  \	   /					  |	 |		 |	|
	//										|	|						   \  /						  |  |		 |	|
	//		-1.0							....							\/						   \/		  \/
	//
	//
	//		1.0								....							/\
	//										|	|						   /  \
	//										|	|						  /	   \
	//										|	|						 /		\
	//		0.5	....	....		....	|	....		....		/		 ....			and the negative one. I'm not going to draw it
	//				|	|				|	|					\	   /
	//				|	|				|	|					 \	  /
	//				|	|				|	|					  \  /
	//		-1.0	....				....					   \/
	//
	//
	//			Hill					Double Hill					Spikes								Zig Zag

	public:

		ViNoiseCreator();
		~ViNoiseCreator();

		void createNoise(ViBuffer *input, ViBuffer *output, ViBuffer *mask);

	protected:

		enum Type
		{
			Hill = 0,
			DoubleHill = 1,
			Spikes = 2,
			ZigZag = 3
		};

		void addNoise(ViSampleChunk &samples1, ViSampleChunk &samples2, ViSampleChunk &mask1, ViSampleChunk &mask2);

		void createNoiseKind();
		void nextNoiseKind();

		void createNoise1();
		void createNoise2();
		void createNoise3();
		void createNoise4();

	private:

		qreal *mRandom;
		ViSampleChunk mNoise;

		int mNoiseLength;
		Type mNoiseType;
		int mNoiseDirection;

};

#endif
