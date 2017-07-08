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
//#define SURF_SWITCH
#define _ORB

 
 int main(int argc,char** argv)
 {
   Mat src1,src2,img;
   src1=imread(argv[1],1);
   src2=imread(argv[2],1);
   double t0;
   
#ifdef FAST
   //FAST algorithm only detect the keypoints.
   Ptr<FastFeatureDetector> fast=FastFeatureDetector::create();//threshold = 10;nonmaxSurpression=true;TYPE_9_16
   vector<KeyPoint> kp1,kp2;
   t0=getTickCount();
   double t_total;
   fast->detect(src1,kp1);
   fast->detect(src2,kp2);
   t_total=((double)getTickCount()-t0)/getTickFrequency();
   cout<<"Total time: "<<t_total<<" s"<<endl;//t_FAST is 0.0092s
   cout<<"The number of KeyPoints in src1: "<<kp1.size()<<"\n in src2: "<<kp2.size()<<endl;//3476 in src1;3219 in src2;
   drawKeypoints(src1,kp1,img);
   imshow("FAST",img);
   waitKey(0);
#endif
   
#ifdef SIFT
   //this part needs both of OpenCV3.2 and OpenCV_contrib
   //SIFT is defined in opencv2/xfeatures2d/nonfree
   //SIFT algorithm consists of keypoints and descriptors
   Ptr<cv::xfeatures2d::SiftFeatureDetector> detector=cv::xfeatures2d::SiftFeatureDetector::create();
   vector<KeyPoint> kp1;
   vector<KeyPoint> kp2;
   t0=getTickCount();
   double t_detect,t_compute,t_match,t_total;
   detector->detect(src1,kp1);
   detector->detect(src2,kp2);
   t_detect=getTickCount();
   //descriptors
   Mat descriptors1;
   Mat descriptors2;
   detector->compute(src1,kp1,descriptors1);
   detector->compute(src2,kp2,descriptors2);
   t_compute=getTickCount();
   
   //match
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create( "FlannBased" );
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   t_match=getTickCount();
   t_total=(t_match-t0)/getTickFrequency();
   cout<<"Detect time: "<<(t_detect-t0)/getTickFrequency()<<" s"<<endl;//0.509s
   cout<<"Describe time: "<<(t_compute-t_detect)/getTickFrequency()<<" s"<<endl;//0.681s
   cout<<"Match time: "<<(t_match-t_compute)/getTickFrequency()<<" s"<<endl;//0.071s
   cout<<"Total time: "<<t_total<<" s"<<endl;//1.26224s
   drawMatches(src1,kp1,src2,kp2,matches,img);
   //drawKeypoints(src1,kp1,img);
   imshow("SIFT matches",img);
   imwrite("SIFT_matches.png",img);
   waitKey(0);
#endif

#ifdef SURF_SWITCH
   Ptr<SURF> detecor=SURF::create(1000);
   vector<KeyPoint> kp1,kp2;
   Mat descriptors1,descriptors2;
   double t_detect,t_compute,t_match,t_total;
   t0=getTickCount();
   detecor->detect(src1,kp1);
   detecor->detect(src2,kp2);
   t_detect=getTickCount();
   detecor->compute(src1,kp1,descriptors1);
   detecor->compute(src2,kp2,descriptors2);
   t_compute=getTickCount();
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create("FlannBased");
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   t_match=getTickCount();
   t_total=(t_match-t0)/getTickFrequency();
   cout<<"Detect time: "<<(t_detect-t0)/getTickFrequency()<<" s"<<endl;//0.2459s
   cout<<"Describe time: "<<(t_compute-t_detect)/getTickFrequency()<<" s"<<endl;//0.61s
   cout<<"Match time: "<<(t_match-t_compute)/getTickFrequency()<<" s"<<endl;//0.1222s
   cout<<"Total time: "<<t_total<<" s"<<endl;//0.9829S
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
   Ptr<FeatureDetector> detecor=ORB::create(500,1.2f,8,31,0,4);
   vector<KeyPoint> kp1,kp2;
   Mat descriptors1,descriptors2;
   t0=getTickCount();
   double t_detect,t_compute,t_match,t_total;
   detecor->detect(src1,kp1);
   detecor->detect(src2,kp2);
   t_detect=getTickCount();
   detecor->compute(src1,kp1,descriptors1);
   detecor->compute(src2,kp2,descriptors2);
   t_compute=getTickCount();
   Ptr<DescriptorMatcher> matcher=DescriptorMatcher::create("BruteForce-Hamming");
   vector<DMatch> matches;
   matcher->match(descriptors1,descriptors2,matches);
   t_match=getTickCount();
   t_total=(t_match-t0)/getTickFrequency();
   cout<<"Detect time: "<<(t_detect-t0)/getTickFrequency()<<" s"<<endl;//0.033s
   cout<<"Describe time: "<<(t_compute-t_detect)/getTickFrequency()<<" s"<<endl;//0.0257s
   cout<<"Match time: "<<(t_match-t_compute)/getTickFrequency()<<" s"<<endl;//0.0063s
   cout<<"Total time: "<<t_total<<" s"<<endl;//0.0674s
   drawMatches(src1,kp1,src2,kp2,matches,img);
   imwrite("ORB_WTA_K_4_matches.png",img);
   imshow("ORB matches",img);
   waitKey(0);
   //ORB keypoints
   Mat ORB_kp;
   //Match filter
   int minDist=20000,maxDist=0;
   for(int i=0;i<matches.size();i++)
   {
     if(matches[i].distance<minDist)
     {
       minDist=matches[i].distance;
    }
    if(matches[i].distance>maxDist)
    {maxDist=matches[i].distance;}
  }
  
  vector<DMatch> matches_filtered;
  for(int i=0;i<matches.size();i++)
  {
    if(matches[i].distance<min(3*minDist,30))
    {
      matches_filtered.push_back(matches[i]);
    }
  }
  Mat img_filtered;
  drawMatches(src1,kp1,src2,kp2,matches_filtered,img_filtered);
  imshow("ORB_matches_filtered",img_filtered); 
  
  waitKey(0);
#endif
   
   return 0;
}