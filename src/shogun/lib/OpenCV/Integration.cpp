#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <shogun/base/init.h>
#include <shogun/lib/common.h>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/DenseFeatures.h>

#include <map>
#include <iostream>
#include <string>

using namespace std;
using namespace shogun;
using namespace cv;

enum Options {CV2SG_CONSTRUCTOR, CV2SG_MANUAL, CV2SG_MEMCPY,SG2CV_CONSTRUCTOR,
SG2CV_MANUAL, SG2CV_MEMCPY};

template <typename T>
struct TypeName;

template<>
struct TypeName<uint8_t>
{
	static const int get()
	{
		return 0;
	}
};

template<>
struct TypeName<int8_t>
{
	static const int get()
	{
		return 1;
	}
};

template<>
struct TypeName<uint16_t>
{
	static const int get()
	{
		return 2;
	}
};

template<>
struct TypeName<int16_t>
{
	static const int get()
	{
		return 3;
	}
};

template<>
struct TypeName<int32_t>
{
static const int get()
	{
		return 4;
	}
};

template<>
struct TypeName<float32_t>
{
static const int get()
	{
		return 5;
	}
};

template<>
struct TypeName<float64_t>
{
	static const int get()
	{
		return 6;
	}
};

class SGMatrixFactory
{
	public:
	template <typename T> static SGMatrix<T> getMatrix(Mat, Options=CV2SG_CONSTRUCTOR);
};

template< typename T> SGMatrix<T> SGMatrixFactory::getMatrix(Mat cvMat, Options option)
{
	int nRows=cvMat.rows;
	int nCols=cvMat.cols;
	const int myType = TypeName<T>::get();
	cvMat.convertTo(cvMat,myType);
	switch (option)
	{
		case CV2SG_CONSTRUCTOR:
		{
			SGMatrix<T> sgMat((T*)cvMat.data, nRows, nCols, false);
			return sgMat;
		}

		case CV2SG_MANUAL:
		{
			SGMatrix<T> sgMat(nRows, nCols);

			for(int i=0; i < nRows; i++)
				for(int j=0; j < nCols; j++)
					sgMat(i,j) = cvMat.at<T>(i,j);
			return sgMat;
		}

		case CV2SG_MEMCPY:
		{
			SGMatrix<T> sgMat(nRows, nCols);
			memcpy(sgMat.matrix, cvMat.data, nRows*nCols*sizeof(T));
			return sgMat;
		}
	}
}

class CDenseSGMatrixFactory
{
	public:
	template <typename T> static CDenseFeatures<T>* getDenseFeatures(Mat, Options=CV2SG_CONSTRUCTOR);
};

template<typename T> CDenseFeatures<T>* CDenseSGMatrixFactory::getDenseFeatures(Mat cvMat, Options option)
{
	SGMatrix<T> sgMat = SGMatrixFactory::getMatrix<T>(cvMat, option);
	CDenseFeatures<T>* features = new CDenseFeatures<T>(sgMat);
	return features;
}

class CVMatrixFactory
{
public:
	template <typename T> static Mat getMatrix(CDenseFeatures<float64_t>* A,
			Options=SG2CV_CONSTRUCTOR);
};

template <typename T> Mat CVMatrixFactory::getMatrix(CDenseFeatures<float64_t>* A,
		Options option)
{
	SGMatrix<float64_t> sgMat=((CDenseFeatures<float64_t>*)A)->get_feature_matrix();
	int nRows=sgMat.num_rows;
	int nCols=sgMat.num_cols;
	const int myType = TypeName<T>::get();

	switch(option)
	{
		case SG2CV_CONSTRUCTOR:
		{
			Mat cvMat(nRows, nCols, CV_64FC1, (void*)sgMat.matrix);
			cvMat.convertTo(cvMat,myType);
			return cvMat;
		}
		case SG2CV_MANUAL:
		{
			Mat cvMat(nRows, nCols, CV_64FC1);
			for(int i=0; i<nRows; i++)
				for(int j=0; j<nCols;j++)
					cvMat.at<double>(i,j) = sgMat(i,j);
			cvMat.convertTo(cvMat,myType);
			return cvMat;
		}

		case SG2CV_MEMCPY:
		{
			Mat cvMat(nRows, nCols, CV_64FC1);
			memcpy((double*)cvMat.data, sgMat.matrix, nRows*nCols*sizeof(double));
			cvMat.convertTo(cvMat,myType);
			return cvMat;
		}
	}
}

int main()
{
	init_shogun_with_defaults();
	Mat cvMatiii = Mat::eye(3,4,CV_8U);
	cvMatiii.at<unsigned char>(0,1) = 3;
//-----------------------------------------------------------------------------
//Implementation part.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Convert any cv::Mat object into the specified type of CDenseFeatures object
//-----------------------------------------------------------------------------
	CDenseFeatures<float64_t>* A = CDenseSGMatrixFactory::getDenseFeatures
			<float64_t>(cvMatiii, CV2SG_MANUAL);
//-----------------------------------------------------------------------------
// Convert any float64_t CDenseFeature object into specified cv::Mat object
//-----------------------------------------------------------------------------
	Mat cvMat1 = CVMatrixFactory::getMatrix<uint16_t>(A, SG2CV_MEMCPY);
	cout<<cvMat1<<endl;
	cout<<cvMat1.type()<<endl;
	return 0;
}

