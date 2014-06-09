/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2014 Abhijeet Kislay
 */


#include <shogun/lib/config.h>
#ifdef HAVE_OPENCV

#include <gtest/gtest.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <shogun/lib/SGMatrix.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/lib/OpenCV/CV2FeaturesFactory.h>
#include <shogun/lib/OpenCV/SG2CVMatFactory.h>

using namespace cv;
using namespace shogun;

TEST(SGMatrix2CVMatConstructor, SGMatrix_to_CVMat_conversion_using_Constructor)
{

	index_t data[] = {1, 3, 0,
					  0, 1, 0,
					  0, 0, 1};

	Mat cvMat = Mat::eye(3,3,CV_8U);
	cvMat.at<unsigned char>(0,1) =3;

	CDenseFeatures<float64_t>* A = CV2FeaturesFactory::getDenseFeatures<float64_t>(cvMat,CV2SG_MANUAL);

	Mat cvMat1 = SG2CVMatFactory::getMatrix<uint8_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat2 = SG2CVMatFactory::getMatrix<int8_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat3 = SG2CVMatFactory::getMatrix<uint16_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat4 = SG2CVMatFactory::getMatrix<int16_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat5 = SG2CVMatFactory::getMatrix<int32_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat6 = SG2CVMatFactory::getMatrix<float32_t>(A, SG2CV_CONSTRUCTOR);
	Mat cvMat7 = SG2CVMatFactory::getMatrix<float64_t>(A, SG2CV_CONSTRUCTOR);

	index_t k=0;
	for (index_t i=0; i<3; ++i)
	{
		for (index_t j=0; j<3; ++j)
		{
			EXPECT_EQ(cvMat1.at<uint8_t>(i,j), data[k]);
			EXPECT_EQ(cvMat2.at<int8_t>(i,j), data[k]);
			EXPECT_EQ(cvMat3.at<uint16_t>(i,j), data[k]);
			EXPECT_EQ(cvMat4.at<int16_t>(i,j), data[k]);
			EXPECT_EQ(cvMat5.at<int32_t>(i,j), data[k]);
			EXPECT_EQ(cvMat6.at<float32_t>(i,j), data[k]);
			EXPECT_EQ(cvMat7.at<float64_t>(i,j), data[k]);
			++k;
		}

	}
}
#endif //HAVE_OPENCV

