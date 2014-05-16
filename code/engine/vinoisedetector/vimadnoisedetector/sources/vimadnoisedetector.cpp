#include <vimadnoisedetector.h>
#include <qmath.h>

#define MAD 0.6745
#define MAD_THRESHOLD 3.5
#define WINDOW_SIZE 128

ViMadNoiseDetector::ViMadNoiseDetector()
	: ViNoiseDetector()
{
	mHalfWindow = WINDOW_SIZE / 2;
	setOffset(mHalfWindow);
}


typedef double elem_type ;
elem_type torben(QQueue<qreal> &m)
{
	int n = m.size();
	int         i, less, greater, equal;
	elem_type  min, max, guess, maxltguess, mingtguess;

	min = max = m[0] ;
	for (i=1 ; i<n ; i++) {
		if (m[i]<min) min=m[i];
		if (m[i]>max) max=m[i];
	}

	while (1) {
		guess = (min+max)/2;
		less = 0; greater = 0; equal = 0;
		maxltguess = min ;
		mingtguess = max ;
		for (i=0; i<n; i++) {
			if (m[i]<guess) {
				less++;
				if (m[i]>maxltguess) maxltguess = m[i] ;
			} else if (m[i]>guess) {
				greater++;
				if (m[i]<mingtguess) mingtguess = m[i] ;
			} else equal++;
		}
		if (less <= (n+1)/2 && greater <= (n+1)/2) break ;
		else if (less>greater) max = maxltguess ;
		else min = mingtguess;
	}
	if (less >= (n+1)/2) return maxltguess;
	else if (less+equal >= (n+1)/2) return guess;
	else return mingtguess;
}


/*
 * This routine was found at
 * http://stackoverflow.com/questions/810657/fastest-code-c-c-to-select-the-median-in-a-set-of-27-floating-point-values
 * written by user chmike
 *
 * Note that the routine only gives correct answers if the number of input pixels is odd
 */
double heapMedian3( double *a, int n_full )
{

	int n_half = (int) ((float)n_full / 2.0 + 0.5);


   double left[n_half], right[n_half], median, *p;
   int nLeft, nRight;

   int nVal;
   //printf("nhalf=%d, n_full=%d\n", n_half, n_full);

   // pick first value as median candidate
   p = a;
   median = *p++;
   nLeft = nRight = 1;

   for(;;) {
	   // get next value
	   double val = *p++;

	   // if value is smaller than median, append to left heap
	   if( val < median ) {
		   // move biggest value to the heap top
		   int child = nLeft++, parent = (child - 1) / 2;
		   while( parent && val > left[parent] ) {
			   left[child] = left[parent];
			   child = parent;
			   parent = (parent - 1) / 2;
		   }
		   left[child] = val;

		   // if left heap is full
		   if( nLeft == n_half ) {
			   // for each remaining value
			   for( nVal = n_full - (p - a); nVal; --nVal ) {
				   // get next value
				   val = *p++;

				   // if value is to be inserted in the left heap
				   if( val < median ) {
					   child = left[2] > left[1] ? 2 : 1;
					   if( val >= left[child] )
						   median = val;
					   else {
						   median = left[child];
						   parent = child;
						   child = parent*2 + 1;
						   while( child < n_half ) {
							   if( child < n_half-1 && left[child+1] > left[child] )
								   ++child;
							   if( val >= left[child] )
								   break;
							   left[parent] = left[child];
							   parent = child;
							   child = parent*2 + 1;
						   }
						   left[parent] = val;
					   }
				   }
			   }
			   return median;
		   }
	   }

	   // else append to right heap
	   else  {
		   // move smallest value to the heap top
		   int child = nRight++, parent = (child - 1) / 2;
		   while( parent && val < right[parent] ) {
			   right[child] = right[parent];
			   child = parent;
			   parent = (parent - 1) / 2;
		   }
		   right[child] = val;

		   // if right heap is full
		   if( nRight == n_half ) {
			   // for each remaining value
			   for( nVal = n_full - (p - a); nVal; --nVal ) {
				   // get next value
				   val = *p++;

				   // if value is to be inserted in the right heap
				   if( val > median ) {
					   child = right[2] < right[1] ? 2 : 1;
					   if( val <= right[child] )
						   median = val;
					   else {
						   median = right[child];
						   parent = child;
						   child = parent*2 + 1;
						   while( child < n_half ) {
							   if( child < n_half-1 && right[child+1] < right[child] )
								   ++child;
							   if( val <= right[child] )
								   break;
							   right[parent] = right[child];
							   parent = child;
							   child = parent*2 + 1;
						   }
						   right[parent] = val;
					   }
				   }
			   }
			   return median;
		   }
	   }
   }
}

void ViMadNoiseDetector::calculateNoise(QQueue<qreal> &samples)
{
	static int i;
	static qreal median, mad, value;

	while(samples.size() >= WINDOW_SIZE)
	{
		/*samples.clear();
		qreal f[] = {1, 5, 4, 3, 5, 8, 7, 9, 4, 5, 6, 2, 1, 4, 5, 6, 2, 1, 4, 5, 96, 8, 7, 2, 1, 4, 5, 5, 4, 5, 8, 15, 25, 2, 36, 69, 5, 56, 2, 58};
		for(i=0;i<40;++i) samples.append(f[i]);
		mHalfWindow = 20;*/

		/*for(i=0;i<samples.size(); ++i)
		{
			cout<<samples[i]<<", ";
		}
		cout<<endl;*/


		// Calculate the median
		//QQueue<qreal> ordered = samples;
		//qSort(ordered.begin(), ordered.end());
		//median = ordered[mHalfWindow];

		qreal median2 =  torben(samples);

		/*QQueue<qreal> o2 = samples;
		qreal *d=new qreal[o2.size()];
		for(i=0;i<o2.size();++i)
		{
			d[i]=o2[i];
		}
		qreal m3 = heapMedian3(d, samples.size());*/

		//cout<<median<<"\t"<<median2<<"\t"<<m3<<"\t\t\t"<<ordered[mHalfWindow-1]<<"\t"<<ordered[mHalfWindow+1]<<endl;
//exit(0);


		// Calculate MAD
		QQueue<qreal> deviations;
		for(i = 0; i < WINDOW_SIZE; ++i)
		{
			deviations.enqueue(qAbs(samples[i] - median));
		}
		qSort(deviations);
		mad = deviations[mHalfWindow];

		// Calculate the z-score
		value = qAbs((MAD * (samples[mHalfWindow] - median)) / (1 + mad));
		setNoise(value /*/ MAD_THRESHOLD*/);
		//cout<<value<<"\t"<<(MAD * (samples[mHalfWindow] - median))<<"\t"<<value<<endl;

		samples.removeFirst();
	}
}

ViMadNoiseDetector* ViMadNoiseDetector::clone()
{
	return new ViMadNoiseDetector(*this);
}

#ifdef __cplusplus
extern "C"
{
#endif

ViMadNoiseDetector* create()
{
	return new ViMadNoiseDetector();
}

#ifdef __cplusplus
}
#endif
