#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <cv.hpp>
#include <iostream>

#include "std_msgs/Int16.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termio.h>
#include <unistd.h>

#include <time.h>
//#include "camera_test/where.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char**argv){
  //for cam----------------------------------------------
  VideoCapture cap(0);
  Mat frame, grayframe; 

  if(!cap.isOpened()){
    cout<<"no camera"<<endl;
    return -1;
  }

  int const Xcenter = 160;//X center
  int const Yvalue = 100;//Y center
  int const width =639;
  int const range =40;
  Point p1(1,Yvalue), p2(width/2,Yvalue);//from right to middle
  Point p3(Xcenter,Yvalue-20), p4(Xcenter,Yvalue+20);// make center mark
  int Lvalue=130;
  int Lpixel=0, Rpixel=0;
  int Ldistance=275, Rdistance=320, pastL;
  int differ;
  //--------------------------------------------------
  //for msg pub --------------------------------------
  ros::init(argc, argv, "cam_msg_publisher");
  ros::NodeHandle nh;
  std_msgs::Int16 cam_msg;
  ros::Publisher pub = nh.advertise<std_msgs::Int16>("cam_msg",100);

  int init_past=1;
  int pub_value;
  //--------------------------------------------------
  ros::Rate loop_rate(50);
  cout<<"start"<<endl;
 
  //clock_t tStart = clock();
  for(;;){
	
    cap>>frame;
	if(waitKey(30) >=0 )break; 
   	resize(frame,frame,Size(320,200)); 
	imshow("frame",frame);
    cvtColor(frame,grayframe,COLOR_BGR2GRAY) ; 
	imshow("gray",grayframe); 
/*
	GaussianBlur(grayframe,grayframe, Size(9,9),0);
	imshow("GGGG", grayframe);
	*/
	Mat thresframe;
	threshold(grayframe,thresframe,100,255,THRESH_BINARY);
	imshow("threshold", thresframe);
	
	/* canny
	Mat cannyframe;
	Canny(grayframe, cannyframe, 30,90,3);
	imshow("canny",cannyframe); 
	*/
	//polylines

	Mat mask = Mat::zeros(grayframe.size(), grayframe.type());
	Mat output;
	cv::Point pts[5] = {
	cv::Point(0,200),
	cv::Point(0,180),
	cv::Point(70,120),
	cv::Point(220,120),
	cv::Point(320,200)
	};
	
	Scalar yy(255,255,255);
	//polylines(grayframe, &pts, &npts, 1,false, Scalar(0,255,0),2);
	fillConvexPoly(mask, pts, 5, yy);
	//imshow("asdfsadf",mask);
	Mat bitwiseframe;
	bitwise_and(thresframe, mask,bitwiseframe); 
	//addWeighted(grayframe, 1, img_edges, 0.5,0, grayframe);
	imshow("poly", bitwiseframe);

	
	// lane detection
	
	vector<cv::Vec4i> line;
	HoughLinesP(bitwiseframe,line,1,CV_PI/180,20,20,30);

	for(int i=0; i< line.size();i++){
	
	cv::Vec4i l = line[i];
	cv::line(frame, cv::Point(l[0], l[1]), cv::Point(l[2],l[3]), Scalar(0,0,255),2);

	};

	Mat norm;
	normalize(frame, norm, 0,255, NORM_MINMAX,CV_8U);
	imshow("houghline",norm);

    loop_rate.sleep();
	//printf("time::: %.fs\n",(double)(clock()-tStart)/CLOCKS_PER_SEC);   


  }



  cout<<"Camera off"<<endl;
  return 0;
}
