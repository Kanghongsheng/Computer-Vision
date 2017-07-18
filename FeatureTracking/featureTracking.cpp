#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;

void ResizeAndDetect( Mat& _srcImg, vector< KeyPoint >& _kp, Mat& _descriptor, 
		      const Ptr< FeatureDetector >& _detector);

void MatchAndFilter(const Mat& _descriptor1, const Mat& _descriptor2, 
		    vector< DMatch >& _matches_filtered, Ptr< DescriptorMatcher >& _matcher);

void CircleAndText( Mat& _img, KeyPoint& _kp, int& _ID );

bool IsEqual( const Mat& _mat1, const Mat& _mat2 );
int main( int argc,char** argv )
{
	//frame1, 
	Mat frame1, frame2, frame3;
	vector<KeyPoint> kp1, kp2, kp3;
	vector< vector<KeyPoint> > kp(200) ;
	Mat descriptor1, descriptor2, descriptor3, descriptors;
	vector< Mat > descriptorLib;
	int ID;
	vector< int > IDLib;
	Ptr<FeatureDetector> detector = ORB::create(300);
	Ptr< DescriptorMatcher > matcher = DescriptorMatcher::create( "BruteForce-Hamming" );
	//vector< DMatch > matches12_filtered, matches23_filtered;
	vector< vector<DMatch> > matches_filtered( 200 );
	double t_0, t_1;
	VideoCapture frame=VideoCapture(0);
	
	waitKey(1000);
    
	frame >> frame1;
	waitKey( 25 );
	frame >> frame2;
	waitKey( 25 );
	frame >> frame3;
	waitKey( 25 );
	
	ResizeAndDetect( frame1, kp1, descriptor1, detector );
	ResizeAndDetect( frame2, kp2, descriptor2, detector );
	ResizeAndDetect( frame3, kp3, descriptor3, detector );
	
	MatchAndFilter( descriptor1, descriptor2, matches_filtered[1], matcher );
	MatchAndFilter( descriptor2 ,descriptor3, matches_filtered[2], matcher );
	
	ID = 0;
	for( DMatch m_12 : matches_filtered[1] )
	{
		for( DMatch m_23 : matches_filtered[2] )
		{
			if (m_23.queryIdx == m_12.trainIdx )//找到在前后两帧均有匹配的特征点
			{
				  //kp , descriptorLib 是KeyPoint 和descripe 库
				  kp[2].push_back( kp2[ m_23.queryIdx ] );//kp[n]表示第n帧的图像,
				  descriptorLib.push_back( descriptor2.row( m_23.queryIdx ) );//descriptorLib[0]表示第2帧中对应的描述子
				  CircleAndText( frame2, kp2[ m_23.queryIdx ], ID );
				  IDLib.push_back( ID );
				  ID ++;
			}
		}
	}
	
	imshow("frame2", frame2);
	//waitKey(0);

	int frameNum = 3;
	for( ; ;frameNum++ )
	{
		t_0 = getTickCount(); 
		Mat frame4;
		vector< KeyPoint > kp4;
		Mat descriptor4;
		
		frame >> frame4;
		ResizeAndDetect( frame4, kp4, descriptor4, detector );
		MatchAndFilter( descriptor3, descriptor4, matches_filtered[ frameNum ], matcher );
		
		for( DMatch m_34 : matches_filtered[ frameNum ] )
		{
			for( DMatch m_23 : matches_filtered[ frameNum-1 ] )
			{
				if( m_23.trainIdx == m_34.queryIdx  )//认为与前后两帧均能匹配的点为稳定的特征点
				{
					for ( int i : IDLib )//遍历descriptorLib查询新点的特征是否在Lib中
					{
						//如果是之前已经标记的点，更新descriptorLib，记录点在新一帧中的位置，并标记
						if ( IsEqual( descriptor2.row( m_23.queryIdx ), descriptorLib[ i ] ) )
						{
							kp[ frameNum ].push_back( kp3[ m_23.trainIdx ] );
							CircleAndText( frame3, kp3[ m_23.trainIdx ], i );
							descriptorLib[ i ] = descriptor3.row( m_34.queryIdx );
							break;
						}
					}
					//如果点不是descriptorLib中的已知点，更新descriptorLib, IDLib, 标记点及ID
					IDLib.push_back( ID );
					descriptorLib.push_back( descriptor3.row( m_34.queryIdx ) );
					CircleAndText( frame3, kp3[ m_23.trainIdx ], ID );
					ID++;
				}
			}
		}
		
		//显示
		imshow( "frame", frame3 );
		
		//将frame1,2,3,4看作滑动窗口
		frame1 = frame2;
		frame2 = frame3;
		frame3 = frame4;
		frame4.release();
		descriptor2 = descriptor3;
		descriptor3 = descriptor4;
		kp3 = kp4;
		
		char c = (char)waitKey(10);
		if( c == 27 || c == 'q' || c == 'Q' )	{break;}
		
		t_1 = getTickCount();
		cout << "the time used in this loop is: "<< ( t_1 - t_0 )/getTickFrequency() <<endl;
		cout << "The size of IDLib is: "<< IDLib.size() <<endl;
		cout << "The size of descriptorLib is: " << descriptorLib.size() <<endl;
	}
  
    return 0;
}

void DetectAndFilter( Mat& _frame1, Mat& _frame2,  
		     vector< KeyPoint >& _kp1, vector< KeyPoint >& _kp2, 
		     Mat& _descriptor1, Mat& _descriptor2,  
		     vector< DMatch >& _matches_filtered, 
		     Ptr< FeatureDetector >& detector, Ptr< DescriptorMatcher >& matcher )
{
	 vector<DMatch> matches12;
	 
	 detector -> detectAndCompute( _frame1, noArray(), _kp1, _descriptor1 );
	 detector -> detectAndCompute( _frame2, noArray(), _kp2, _descriptor2 );
	 matcher -> match( _descriptor1, _descriptor2, matches12 );
	 
	 for( DMatch m_1 : matches12 )
	{
	    if( m_1.distance < 25)
	    {
	      _matches_filtered.push_back( m_1 );
	    }
	}
}

void ResizeAndDetect(Mat& _srcImg, vector< KeyPoint >& _kp, Mat& _descriptor, 
		     const Ptr<FeatureDetector>& _detector)
{
	resize( _srcImg, _srcImg, Size(400, 300) );
	_detector->detectAndCompute( _srcImg, noArray(), _kp, _descriptor);
}

void MatchAndFilter( const Mat& _descriptor1, const Mat& _descriptor2, 
		     vector<DMatch>& _matches_filtered, Ptr< DescriptorMatcher >& _matcher)
{
	vector<DMatch> _matches;
	_matcher -> match( _descriptor1, _descriptor2, _matches);
	
	for( DMatch dm : _matches )
	{
		 if( dm.distance < 25)
		{
		  _matches_filtered.push_back( dm );
		}
	}
}

void CircleAndText(Mat& _img, KeyPoint& _kp, int& _ID)
{
	char IDstr[1000];
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 0.4;

	circle( _img, _kp.pt, 3, Scalar( 0,0,125 ) );
	sprintf( IDstr, "%d", _ID );
	putText( _img, IDstr, _kp.pt, fontFace, fontScale, Scalar( 255, 0, 0) );
}

bool IsEqual( const Mat& _mat1, const Mat& _mat2 )
{
	int n = _mat1.total() * _mat1.elemSize() ;
	bool ans = memcmp( _mat1.data, _mat2.data, n) == 0;
	return ans;
}