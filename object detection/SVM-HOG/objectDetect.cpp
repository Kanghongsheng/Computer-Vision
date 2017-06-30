#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::ml;

//#define TRAIN//开关控制训练还是直接载入训练好的模型
#define DETECTOR//detector开关，直到将训练好的detector写入文件
#define DRAW//识别开关

//------------------------DetectAndDraw--------------------
static void DetectAndDraw(Mat& _src, HOGDescriptor& hog);

//---------------------------main()------------------------------
int main(int argc,char** argv)
{
     //样本自己拍摄保存，这块样本还是太少...
     int PosTrainNums=34;//正样本数量
     int NegTrainNums=200;//背景数量
     
     char img_path[200];
     Mat img;
     HOGDescriptor hog(Size(128,128),Size(16,16),Size(8,8),Size(8,8),9);
     vector<float> descriptors;
     int descriptorDim;
     Mat train_mat, res_mat;
     Ptr<SVM> svm=SVM::create();
     vector<float> detector;
     
#ifdef TRAIN 
     for(int Num=0;Num<PosTrainNums;Num++)
     {
     //image path
     sprintf(img_path,"/home/khs/PickingChallenge/camera_test/src/posImg/%d.png",Num+1);   
     img=imread(img_path,1);
     if(img.empty()){cout<<"fail to load img"<<endl;}
     resize(img,img,Size(128,128),0,0);
     hog.compute(img,descriptors,Size(8,8));
     
     //第一次计算hog时初始化train_mat  res_mat
      if(Num==0)
      {
	//初始化
	descriptorDim=hog.getDescriptorSize();
	train_mat=Mat::zeros(PosTrainNums+NegTrainNums,descriptorDim,CV_32FC1);
	//res_mat=Mat::zeros(PosTrainNums+NegTrainNums,1,CV_32FC1);//float 类型res_mat会报错， 还是int类型更合适
	res_mat=Mat_<int>(PosTrainNums+NegTrainNums,1);
      }
     
     //将descriptors中数据写入train_mat中，将对应的label写入res_mat中
      for(int i=0;i<descriptorDim;i++)
      {
      train_mat.at<float>(Num,i)=descriptors[i];
      }
      res_mat.at<int>(Num,0)=1;
    
    }
    
    //负样本
    for(int No=0;No<NegTrainNums;No++)
    {
     //负样本路径
     sprintf(img_path,"/home/khs/PickingChallenge/camera_test/src/negImg/%3d.png",No+1);
     img=imread(img_path,1);
     resize(img,img,Size(128,128),0,0);
 
     hog.compute(img,descriptors,Size(8,8));
     
     //将descriptors中数据写入train_mat中，将对应的label写入res_mat中
      for(int i=0;i<descriptorDim;i++)
      {
      train_mat.at<float>(PosTrainNums+No,i)=descriptors[i];
      }
     res_mat.at<int>(PosTrainNums+No,0)=0;
    
    }
    
    cout<<"train_mat.rows(): "<<train_mat.rows<<endl;
    cout<<"train_mat.cols(): "<<train_mat.cols<<endl;
    cout<<"res_mat.rows(): "<<res_mat.rows<<endl;
    
    //-------------------------SVMtraining--------------------------------
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setC(0.01);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER,2000,1e-6));
    cout<<"start training svm"<<endl;
    svm->train(train_mat,ROW_SAMPLE,res_mat);
    cout<<"training success"<<endl;
    svm->save("cupSVM.xml");
    //svm->getSupportVectors();
    
#endif
    
#ifndef TRAIN
    svm=SVM::load("cupSVM.xml");
#endif

#ifdef DETECTOR   
    //------------------------------------构建detector------------------------------------------
    Mat sv;
    //svm->getUncompressedSupportVectors();UnpressedSV有啥用？
    //获取support vector
    sv=svm->getSupportVectors();
    sv.convertTo(sv,CV_64F);//转换为alpha的类型，便于后面与alpha相乘
    cout<<"supportVector.type: "<<sv.type()<<endl;
    
    //特征向量位数
    int svDim=sv.rows;//OpenCV 3.2 版本对support vector进行了压缩，这里仅能得到一个sv
    cout<<"svDim: "<<svDim<<endl;
    
    Mat alpha(1,svDim,CV_64F);
    Mat svidx(1,svDim,CV_32S);
    
    double rho;
    rho=svm->getDecisionFunction(0,alpha,svidx);
    
    Mat result_mat=-1*alpha * sv;    //网上有说这里需要前面乘-1，但是实测好像没啥影响...可能效果太差了看不出来
    cout<<"result_mat.type(): "<<result_mat.type()<<"\n"<<"size: "<<result_mat.rows<<" * "<<result_mat.cols<<endl;
    
    for(int i=0;i<result_mat.cols;i++)
    {
      detector.push_back(result_mat.at<float>(0,i));
    }
    detector.push_back(rho);
    cout<<"detector.size(): "<<detector.size()<<endl;
    
    ofstream fout("detector.txt");
    for(int i=0;i<detector.size();i++)
    {
      fout<<detector[i]<<"\n";
    }
    fout.close();
#endif

    hog.setSVMDetector(detector);
    //hog.getDefaultPeopleDetector();//官方的detector性能原来也不好。。。
    
    //----------------------------------------------对图片进行识别--------------------------------------
#define PICTURE
#ifdef PICTURE  //检测图片还是video  
    Mat testImg;
    char test_path[200];
    for(int i=0;i<10;i++)
    {
      sprintf(test_path,"/home/khs/PickingChallenge/camera_test/src/testImg/%3d.png",i+1);
      testImg=imread(test_path,1);
      if(testImg.empty())
      {
	cout<<"testImg failed"<<endl;
      }
      DetectAndDraw(testImg,hog);
    }
#endif    
    
    //---------------------------------------------------对video识别跟踪----------------------------------------
#ifndef PICTURE
    
    Mat testImg;
    VideoCapture frame(1);
    for(int i=0;i<1000;i++)
      {
      frame>>testImg;
      DetectAndDraw(testImg,hog);
      waitKey(10);
      //if(waitKey(20000))break;
      }
#endif
    return 0;
}

static void DetectAndDraw(Mat& _src, HOGDescriptor& hog)
{
  vector<Rect> found,found_filtered;
  Mat tested;
  resize(_src,tested,Size(200,200));
  //tested=_src;
  hog.detectMultiScale(tested,found,0,Size(20,20),Size(32,32));

   //查看是否有嵌套的矩形框
   for (size_t i = 0; i < found.size(); i++)
        {
            Rect r = found[i];
            int j = 0;
            for (; j < found.size(); j++)
            {
                if ( i != j && (r & found[j]) == r)
                {
                    break;
                }
            }
            if(j == found.size())
                found_filtered.push_back(r);
	} 
	
  //缩小矩形框
  for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];

        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(tested, r.tl(), r.br(), cv::Scalar(0,255,0), 1);
    }
    
  imshow("found_filled",tested);
  waitKey(0);
    
}
