#ifndef __UTILS_H
#define __UTILS_H

#include <Eigen/Dense>

#include <opencv2/opencv.hpp>

#include "data_type.h"
#include "data_container.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200

#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 500
#define CENTER_X 200
#define CENTER_Y 250

class Utils
{
public:
	template<typename T>
	static void displayVertexPoses( const std::vector<Eigen::Matrix<T, 3, 1>> &vertex_poses, const std::string &window_name = "vertex", const T scale = 400 );

	static void displayAScan(const sensor::LaserScan& scan);

	template<typename T>
	static void displayAScan(sensor::ScanContainer<T>& scan_container, cv::Mat& image);

	template<typename T>
	static void lidarData2DataContainer( const sensor::LaserScan& scan, sensor::ScanContainer<T>& scan_container );
};

template<typename T>
void Utils::displayVertexPoses( const std::vector<Eigen::Matrix<T, 3, 1>> &vertex_poses, const std::string &window_name, const T scale )
{
	cv::Mat image = cv::Mat::zeros( WINDOW_WIDTH, WINDOW_HEIGHT, CV_8UC3 );
        cv::line( image, cv::Point( WINDOW_WIDTH / 2, 0 ), cv::Point( WINDOW_WIDTH / 2, WINDOW_HEIGHT ), cv::Scalar( 67, 128, 94 ), 1 );
        cv::line( image, cv::Point( 0, WINDOW_HEIGHT / 2 ), cv::Point( WINDOW_WIDTH, WINDOW_HEIGHT / 2 ), cv::Scalar( 67, 128, 94 ), 1 );

        if( vertex_poses.empty() ){
        	return;
        }

        for( auto it : vertex_poses ){
        	cv::Point2d point( it[0] * scale + WINDOW_WIDTH / 2, it[1] * scale + WINDOW_HEIGHT / 2 );
                cv::circle(image, point, 1, cv::Scalar(0, 0, 255), -1);   
        }

        cv::imshow( window_name, image );
        cv::waitKey( 0 );
}

void Utils::displayAScan(const sensor::LaserScan& scan)
{
	cv::Mat image = cv::Mat::zeros(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);
	cv::line(image, cv::Point2f(CENTER_X, CENTER_Y), cv::Point2f(CENTER_X + 40, CENTER_Y), cv::Scalar(255, 255, 0), 4);
	cv::line(image, cv::Point2f(CENTER_X, CENTER_Y), cv::Point2f(CENTER_X, CENTER_Y - 40), cv::Scalar(0, 0, 255), 4);
	cv::line(image, cv::Point2f(0, CENTER_Y), cv::Point2f(IMAGE_WIDTH, CENTER_Y), cv::Scalar(0, 255, 0), 1);
	cv::line(image, cv::Point2f(CENTER_X, 0), cv::Point2f(CENTER_X, IMAGE_HEIGHT), cv::Scalar(0, 255, 0), 1);

	for (int i = 0; i < 160; i++) {
		float dist = static_cast<float>(scan.dists[i]);
		float theta = scan.angles[i];
		float lidar_x = dist * cos((M_PI / 180) * theta);
		float lidar_y = dist * sin((M_PI / 180) * theta);

		//std::cout << "dist : ( " << lidar_x << ", " << lidar_y << " )" << std::endl;

		float img_x = CENTER_X + lidar_x;
		float img_y = CENTER_Y - lidar_y;

		cv::circle(image, cv::Point2f(img_x, img_y), 2, cv::Scalar(255, 0, 0), -1);
	}

	cv::imshow("scan", image);
	cv::waitKey(10);
}

template<typename T>
void Utils::lidarData2DataContainer( const sensor::LaserScan& scan, sensor::ScanContainer<T>& scan_container )
{
	for(size_t i = 0; i < 160; i ++){
        	T dist = static_cast<T>( scan.dists[i] ) * 0.001; // mm -> m
                // judgement 
                if( dist <= 0.005 || dist >= 0.3 ) {
                }else{

                	T theta = scan.angles[i];
                        T lidar_x = dist * cos( (M_PI / 180) * theta );
                        T lidar_y = -dist * sin( (M_PI / 180) * theta );
                        std::cout << "dist : ( " << lidar_x << ", " << lidar_y << " )" << std::endl;
                        scan_container.addData( Eigen::Matrix<T, 2, 1>( lidar_x, lidar_y ));
               }
	}

}

template<typename T>
void Utils::displayAScan(sensor::ScanContainer<T>& scan_container, cv::Mat& image)
{
	int width = image.cols;
	int height = image.rows;

	for( int i = 0; i < scan_container.getSize(); i ++ ){
		typename sensor::ScanContainer<T>::type pt = scan_container.getIndexData( i );
		float img_x = static_cast<float>( width ) / 2 + static_cast<float>( pt[0] ) * 1000;
                float img_y = static_cast<float>( height ) / 2 - static_cast<float>( pt[1] ) * 1000;

                cv::circle(image, cv::Point2f(img_x, img_y), 2, cv::Scalar(255, 0, 0), -1);	
	}
	
	cv::imshow("scan container", image);
}


#endif
