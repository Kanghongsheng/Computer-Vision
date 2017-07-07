#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
 
 
//#define FAST
//#define SIFT
#define SURF_SWITCH
//#define _ORB

 
 int main(int argc,char** argv)
 {
   Mat src1,src2,img;
   src1=imread(argv[1],1);
   src2=imread(argv[2],1);
   double t;
   
#ifdef FAST
   //FAST algorithm only detect the keypoints.
   Ptr<FastFeatureDetector> fast=FastFeatureDetector::create();
   t=getTickCount();
   fast->detect(src1,kp);
   t=((double)getTickCount()-t)/getTickFrequency();
   cout<<"Total time: "<<t<<" s"<<endl;
   drawKeypoints(src1,kp,img);
   imshow("FAST",img);
   waitKey(0);
#endif
   
#ifdef SIFT
   //this part needs both of OpenCV3.2 and OpenCV_contrib
   //SIFT is defined in opencv2/xfeatures2d/nonfree
   //SIFT algorithm consists of keypoints and descriptors
   Ptr<cv::xfeatures2d::SiftFeatureDetector> detector=SiftFeatureDetector::create();//Ptr<>中的类型要写全
   vector<KeyPoint> kp1;
   vector<KeyPoint> kp2;
   t=getTickCount();
   detector->detect(src1,kp1);
   detector->detect(src2,kp2);
   
   //descriptors
   Mat descriptors1;
   Mat descriptors2;
   detector->compute(src1,kp1,descriptors1);
   detector->compute(src2,kp2,descriptors2);
   
   //match
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create( "FlannBased" );
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   drawMatches(src1,kp1,src2,kp2,matches,img);
   //t is only the time taken to detect feature Points
   t=((double)getTickCount()-t)/getTickFrequency();
   cout<<"Total time: "<<t<<" s"<<endl;
   //drawKeypoints(src1,kp1,img);
   imshow("SIFT matches",img);
   waitKey(0);
#endif

#ifdef SURF_SWITCH
   Ptr<SURF> detecor=SURF::create();
   vector<KeyPoint> kp1,kp2;
   Mat descriptors1,descriptors2;
   t=getTickCount();
   detecor->detect(src1,kp1);
   detecor->detect(src2,kp2);
   detecor->compute(src1,kp1,descriptors1);
   detecor->compute(src2,kp2,descriptors2);
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create("BruteForce-SL2");
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   t=((double)getTickCount()-t)/getTickFrequency();
   cout<<"Total time: "<<t<<" s"<<endl;
   drawMatches(src1,kp1,src2,kp2,matches,img);
   imshow("SURF matches",img);
   waitKey(0);
   //SURF keypoints
   Mat SURF_kp;
   drawKeypoints(src1,kp1,SURF_kp);
   imshow("SURF Keypoints",SURF_kp);
   waitKey(0);
#endif
   
#ifdef _ORB
   Ptr<FeatureDetector> detecor=ORB::create();
   vector<KeyPoint> kp1,kp2;
   Mat descriptors1,descriptors2;
   t=getTickCount();
   detecor->detect(src1,kp1);
   detecor->detect(src2,kp2);
   detecor->compute(src1,kp1,descriptors1);
   detecor->compute(src2,kp2,descriptors2);
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create("BruteForce-Hamming");
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   t=((double)getTickCount()-t)/getTickFrequency();
   cout<<"Total time: "<<t<<" s"<<endl;
   drawMatches(src1,kp1,src2,kp2,matches,img);
   imshow("ORB matches",img);
   waitKey(0);
   //ORB keypoints
   Mat ORB_kp;
   drawKeypoints(src1,kp1,ORB_kp);
   imshow("ORB Keypoints",ORB_kp);
   waitKey(0);
#endif
   
   return 0;
}