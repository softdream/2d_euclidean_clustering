#include "cluster.h"
#include "file_read.h"
#include "utils.h"



int main()
{
	std::cout<<"--------------- EUCLIDEAN CLUSTERINT --------------"<<std::endl;
	file::FileRead file_read;

        file_read.openFile( "test_data" );

	cluster::Cluster<float> cluster;

	cv::Mat image = cv::Mat::zeros( 600, 600, CV_8UC3 );

	while( !file_read.endOfFile() ){
		sensor::RecordData data;
	        file_read.readAFrame( data );

		sensor::ScanContainer<float> scan_container;
		Utils::lidarData2DataContainer( data.scan_data, scan_container );		

		Utils::displayAScan( scan_container, image );
	
		std::vector<std::vector<typename sensor::ScanContainer<float>::type>> clusters;
		int num = cluster.extractEuclideanClusters( scan_container, clusters );
		std::cout<<"cluster number = "<< num<<std::endl;

		cv::waitKey(0);
		// draw clusters
		for( auto& cluster : clusters ){
			for( auto& pt : cluster ){
				float img_x = static_cast<float>( image.cols ) / 2 + static_cast<float>( pt[0] ) * 1000;
                		float img_y = static_cast<float>( image.rows ) / 2 - static_cast<float>( pt[1] ) * 1000;

                		cv::circle(image, cv::Point2f(img_x, img_y), 2, cv::Scalar(0, 0, 255), -1);
			}
		}
		imshow("clusters", image);
		cv::waitKey(0);
	
		image = cv::Mat::zeros( 600, 600, CV_8UC3 );	
	}
	
		


	return 0;
}
