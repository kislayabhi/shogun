/*
 * Copyright (c) The Shogun Machine Learning Toolbox
 * Written (w) 2014 Abhijeet Kislay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the Shogun Development Team.
 */

#include <shogun/lib/config.h>
#ifdef HAVE_OPENCV

#ifndef CV2_SGMATRIX_FACTORY_H_
#define CV2_SGMATRIX_FACTORY_H_

#include <opencv2/highgui/highgui.hpp>
#include <shogun/lib/SGMatrix.h>
#include <shogun/features/DenseFeatures.h>

namespace shogun{

enum CV2SGOptions {CV2SG_CONSTRUCTOR, CV2SG_MANUAL, CV2SG_MEMCPY};

template <typename T> struct CV2SGTypeName;

template<> struct CV2SGTypeName<uint8_t>
{
	static const int get()
	{
		return 0;
	}
};

template<> struct CV2SGTypeName<int8_t>
{
	static const int get()
	{
		return 1;
	}
};

template<> struct CV2SGTypeName<uint16_t>
{
	static const int get()
	{
		return 2;
	}
};

template<> struct CV2SGTypeName<int16_t>
{
	static const int get()
	{
		return 3;
	}
};

template<> struct CV2SGTypeName<int32_t>
{
	static const int get()
	{
		return 4;
	}
};

template<> struct CV2SGTypeName<float32_t>
{
	static const int get()
	{
		return 5;
	}
};

template<> struct CV2SGTypeName<float64_t>
{
    static const int get()
    {
    	return 6;
    }
};

class CV2SGMatrixFactory
{
	public:
	
	CV2SGMatrixFactory();
	
	~CV2SGMatrixFactory();
	
	template <typename T> static SGMatrix<T> getMatrix(cv::Mat, CV2SGOptions=CV2SG_CONSTRUCTOR);
};

template<typename T> SGMatrix<T> CV2SGMatrixFactory::getMatrix(cv::Mat cvMat, CV2SGOptions option)
{
	int num_rows=cvMat.rows;
	int num_cols=cvMat.cols;
	const int myType=CV2SGTypeName<T>::get();
	cvMat.convertTo(cvMat,myType);
	switch (option)
	{

		case CV2SG_CONSTRUCTOR:
		{
			SGMatrix<T> sgMat((T*)cvMat.data, num_rows, num_cols, false);
			return sgMat;
		}

		case CV2SG_MANUAL:
		{
			SGMatrix<T> sgMat(num_rows, num_cols);

			for(int i=0; i<num_rows; i++)
			{
				for(int j=0; j<num_cols; j++)
				{
					sgMat(i,j)=cvMat.at<T>(i, j);
				}
			}
			return sgMat;
		}

		case CV2SG_MEMCPY:
		{
			SGMatrix<T> sgMat(num_rows, num_cols);
			memcpy(sgMat.matrix, cvMat.data, num_rows*num_cols*sizeof(T));
			SGMatrix<T>::transpose_matrix(sgMat.matrix,num_rows,num_cols);
			return sgMat;
		}
		
		default:
		{
			SGMatrix<T> sgMat(num_rows, num_cols);
			memcpy(sgMat.matrix, cvMat.data, num_rows*num_cols*sizeof(T));
			SGMatrix<T>::transpose_matrix(sgMat.matrix,num_rows,num_cols);
			return sgMat;
		}
	}
}

}
#endif /*CV2_SGMATRIX_FACTORY_H_*/
#endif /* HAVE_OPENCV */
