
#include "watershed.h"

#include <opencv/cv.hpp>

using namespace std;

void watershed(QString path,int color, int color_2, int& amount, std::vector<double>& length, std::vector<double>& width, std::vector<double>& area)
{
    cv::Mat source_img;
        source_img = cv::imread(path.toStdString().c_str());
        cv::Mat hsv;
        cvtColor(source_img, hsv, CV_BGR2HSV);
        vector<cv::Mat> channels;
        split(hsv, channels);


    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                 1,  1, 1,
                 1, -8, 1,
                 1,  1, 1);
        cv::Mat imgLaplacian;
        cv::Mat sharpen = channels[1];
        cv::filter2D(sharpen, imgLaplacian, CV_32F, kernel);
        channels[1].convertTo(sharpen, CV_32F);
        cv::Mat imgResult = sharpen - imgLaplacian;
        imgResult.convertTo(imgResult, CV_8UC3);
        imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

        cv::inRange(imgResult, cv::Scalar(color, color, color),
                           cv::Scalar(color_2, color_2, color_2),
                           imgResult);


    cv::blur(imgResult, imgResult, cv::Size(3, 3));

    int erode_sz = 4;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                           cv::Size(2*erode_sz + 1, 2*erode_sz+1),
                                           cv::Point(erode_sz, erode_sz) );

     cv::erode(imgResult, imgResult, element);
     cv::dilate(imgResult, imgResult, element);
     cv::bitwise_not(imgResult, imgResult);

     cv::threshold(imgResult, imgResult, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

     cv::Mat opening;
     cv::Mat kernel_1(3, 3, CV_8U, cv::Scalar(1));
     cv::morphologyEx(imgResult, opening, cv::MORPH_OPEN, kernel_1, cv::Point(-1,-1), 1);

     cv::Mat sure_bg;
     cv::dilate(opening, sure_bg, kernel_1, cv::Point(-1,-1), 3);

     cv::Mat dist_transform;
     cv::distanceTransform(opening, dist_transform, CV_DIST_L2, 5);

     cv::Mat sure_fg;
     double minVal, maxVal;
     cv::Point minLoc, maxLoc;
     cv::minMaxLoc(dist_transform, &minVal, &maxVal, &minLoc, &maxLoc);
     cv::threshold(dist_transform, sure_fg, 0, 255, 0);

     dist_transform = dist_transform/maxVal;

     cv::Mat unknown, sure_fg_uc1;
            sure_fg.convertTo(sure_fg_uc1, CV_8UC1);
            cv::subtract(sure_bg, sure_fg_uc1, unknown);

            int compCount = 0;
            vector<vector<cv::Point> > contours;
            vector<cv::Vec4i> hierarchy;
            sure_fg.convertTo(sure_fg, CV_32SC1, 1.0);
            cv::findContours(sure_fg, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
            if( contours.empty() ) return;
            cv::Mat markers = cv::Mat::zeros(sure_fg.rows, sure_fg.cols, CV_32SC1);
            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
                cv::drawContours(markers, contours, idx, cv::Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);
            markers++;

            for(int i=0; i<markers.rows; i++){
                    for(int j=0; j<markers.cols; j++){
                        unsigned char &v = unknown.at<unsigned char>(i, j);
                        if(v==255){
                            markers.at<int>(i, j) = 0;

                        }
                    }
                }

            watershed( source_img, markers );

            for(int i = 0; i < markers.rows; i++ ){
                    for(int j = 0; j < markers.cols; j++ )
                    {
                        int index = markers.at<int>(i,j);
                        if( index == -1 )
                            source_img.at<cv::Vec3b>(i,j) = cv::Vec3b(0,255,0);

                    }
                }
            amount = 0;
            for (unsigned int i = 0; i < contours.size(); i = i + 2){
                cv::RotatedRect rect = cv::minAreaRect(contours[i]);
                cv::putText(source_img, to_string(amount+1), rect.center, cv::FONT_ITALIC, 2, cv::Scalar(0,0,255), 1);
                length.push_back(rect.size.height*PIXEL_SIZE);
                width.push_back(rect.size.width*PIXEL_SIZE);
                area.push_back(cv::contourArea(contours[i])*PIXEL_SIZE*PIXEL_SIZE);
                amount++;
            }

            cv::imwrite("result.png", source_img);

            return;

}
