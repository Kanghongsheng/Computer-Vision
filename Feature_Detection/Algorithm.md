# Feature Detection
-----------------------
## FAST
--------
### **Introduction**

* __FAST__ which means _"Features from accelerated segment test"_, is the first corner detect algorithm based on the **AST** to find a set of key points in the image. 

### **Algorithm**

* The main idea of the algorithm is to evaluating the pixels in a Bresenham circle of radius _r_ around the candidate point.
* If _n_ contiguous pixels are all brighter than the nucleus by at least _t_ or all darker than the nucleus by _t_, then the pixel under the nucleus is considered to be a feature.
* Without the improvement of *machine learning*, N is usually chosen as 12. In OpenCV, other type of FAST are available.

If N is 12, you can follow these steps to **accelerate** the algorithm:
1. Examining pixels 1 and 9,if both are within*[Ip-t,Ip+t]* ,then candidate is not a corner
2. Otherwise pixels 5 and 13 are examined. if there exists 3 of them that are either brighter or darker, then rest pixels are then examined for final conclusion

### **Reference**

> _Features from accelerated segment test_ in wiki

> "_Real-time Video Annotations for Augmented Reality_" Edward Rosten 2005

## **SURF**
------------
### **Introduction**

_SURF_ which means "_Speeded up Robust Features_", is a **scale- and rotation-invariant interest point detector and descriptor**.

### **'_Fast-Hessian_' Detector**
* **Integral images** is used to reduce the computation time.
* The detector is based on the **Hessian matrix**, the **Hessian matrix** in x at scale \sigma is defined as follows:
where _Lxx_ is the convolution of the Gaussian second order derivative with the image _I_ in point _x_.
![Hessian Matrix](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/Hessian.jpg)

* For Gaussian filters are not-ideal in any case,we use **box filters** to approximate Gaussian filter.
* The scale space is analysed by up-scaling the filter size rather than iteratively reducing the image size, due to the use of box filters and integral images.
* A **non-maximum suppression** in a 3x3x3 neighbourhood is applied to lacalise interest points in the image and over scales. 
* The _determinant_ of the Hessian matrix is used as a measure of local change around the point and points are chosen where the determinant is maximal.

### **Descriptor**
* The **Haar wavelet responses** in both x- and y-directions within a circular neighbourhood of radius *6s* around the point of interest are computed, where _s_ is the scale at which the point of interest was detected.
* The **responses are weighted** by a Gaussian function centered at the point of interest, then represented as **vectors** in a two-dimensional space, with the horizontal response in the abscissa and the vertical response in the ordinate.
* The **dominant orientation** is estimated by calculating the sum of all responses within a sliding orientation window of size π/3. The horizontal and vertical responses within the window are summed.
* The size of the sliding window is a parameter that has to be chosen carefully to achieve a desired balance between **robustness and angular resolution**.
* The longest vector overall defines **the orientation of the point of interest**. 

### **Reference**
> "Speeded up robust features" in wiki

> "SURF: Speed Up Robust Features" by Herbert Bay in ETH Zurich

## **ORB**
---------
### **Introduction**
Oriented FAST and rotated BRIEF(ORB) is an algorithm, which use **FAST** to detect points of interest and describe it via **BRIEF** algorithm. To get a target number of _N_ points by this algorithm, we first set the threshold low enough to get more than _N_ keypoints, then order them according to the **Harris corner measure** and pick the top _N_ points.
Besides, we employ a **scale pryamid of the image** to produce FAST features(filtered by Harris) at each level in the pyramid to produce multi-scale features.

### **Detector**(_oFAST:FAST Keypoint Orientation_)

* a _Harris corner measure_ to order the **FAST** keypoints
* Orientation by _Intensity Centroid_

### **Descriptor**(_rBRIEF:Rotation-Aware BRIEF_)
* **BRIEF** descriptor is robust to lighting, blur, and perspective distortion like SIFT. However,it is **sensitive to in-plane rotation**.
* Combine the BRIEF descriptor with the patch orientation, we get **steered BRIEF**.
* Using **PCA** or other dimensionality-reduction method to get new features that have **high variance** and are **uncorrelated** over a large training set, which is called **rBRIEF**

### **Reference**
>"_Oriented FAST and rotated BRIEF_" by wiki

> "_ORB:an efficient alternative to SIFT or SURF_" by Ethan Rublee

> Tutorial On Binary Descriptors -part 1&2&3 in Gil's CV blog(https://gilscvblog.com/page/2/)


**Scalable Matching of Binary Features**
* **Locality Sensitive Hashing** as our nearest neighbor search

### table of tech in ORB
|Problem|Technique|Advantage|
|:--------:|:--------------:|:---------:|
|key localization/scale/rotation|FAST/scale-space pyramid/Intensity Centroid|accuracy,stability,scale&rotational invariance|
|indexing and matching|nearest neighbor/Locality Sensitive Hashing|Efficiency/speed|

## **SIFT**
---------
### **Introduction**
SIFT which means the _Scale Invariant Feature Transform_ can transforms an image into a large collection of local feature vectors, each of which is invariant to image translation,scaling, and rotation, and partially invariant to illumination changes and affine or 3D projection.

### Detector
**Scale space**
* Under some rather general assumptions on **scale invariance**, the Gaussian kernel and its derivations are the only possible smoothing kernels for scale space analysis.
* Build an **image pyramid** 
1. Convolving the Gaussian function with the input image to get image _A_
2. Repeated a second time to get a new image _B_
3. The **difference of Gaussian** function is obtained by subtracting image B from A
4. Resample image _B_ using **bilinear interpolation** with pixel spacing of 1.5 in each direction to generate the next pyramid level

**Key localization**
* Key locations at maxima and minima of a **difference of Gaussian function** applied in scale space.
1. A pixel is compared to its 8 neighbours at the same level of the pyramid.
2. If it is a maxima or minima at this level, then the closest pixel location is calculated at the **next lowest level** of the pyramid
3. If the pixel remains higher(or lower), then the test is repeated for the **level above**

### Descriptor
1. Compute the image **gradient magnitude and orientation** at each pixel at each level
2. Orientation is determined by the **peak in a histogram of local image gradient orientations**.
* The orientation histogram is created using a **Gaussian-weighted window**
* The histogram has 36 **bins** covering the 360 degree range of rotations.
3. Local image description
* To make description robust against small **shifts** in local geometry
* For each keypoint, we use the pixels that fall in a circle of radius 8 pixels around the key location are inserted into the orientation planes. 

### Reference
> "Scale-invariant feature transform" in wiki

> "Object Recognition from Local Scale-Invariant Features" by David G. Lowe

### table of tech in SIFT
|Problem|Technique|Advantage|
|:--------:|:--------------:|:---------:|
|key localization/scale/rotation|DoG/scale-space pyramid/orientation assignment|accuracy,stability,scale&rotational invariance|
|geometric distortion|blurring/resampling of local image orientation planes|affine invariance|
|indexing and matching|nearest neighbor/ Bin First search|Efficiency/speed|
|Cluster identification|Hough Transform voting|reliable pose models|
|Model verification/outlier detection|Linear least squares|better error tolerance with matches|
|Hypothesis acceptance|Bayesian Probability analysis|reliability|

## Evaluation of SIFT, SURF and ORB

### test_1(matching test vs. the angle of rotation)
**Conditions**
* datasets: images with **synthetic in-plane rotation and added Gaussian noise**
* The added Gaussian noise in the dataset is 10
* Targeting 500 keypoints per image
* Brute-force matching to find the best correspondence.
* The results are given in terms of **percentage of correct matches against the angle of rotation**

**Results**

The Matching performance of SIFT,SURF,BRIEF with FAST, and ORB(oFAST+rBRIEF) under synthetic rotations with Gaussian noise of 10 is as follows:

![Matching Performance](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/Matching_performance.jpg)

From the picture above, we can conclude that:
1. the standard BRIEF operator falls off dramatically after about 10 degrees
2. SIFT **outperforms** SURF,which shows quantization effects at 45-degree angles due to its Harr-wavelet composition.
3. ORB has the best performance with over 70% inliers.

### test_2(matching test vs. noise)
**Conditions**
* plot the inlier performance vs. noise
* The noise levels are 0, 5, 10, 15, 20, 25

**Results**

The results is as follows:

![Matching behavior](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/Matching_behavior.jpg)

* SIFT exhibits a steady drop of 10% with each additional noise increment of 5
* ORB also drops, but at a much lower rate

### test_3(inlier/outlier test)
**Conditions**
* datasets: 
1. indoor set of high-textured magazines on a table
2. outdoor scene
* The datasets have **scale, viewpoint, and lighting changes**
* The images in the first column are matched to those in the second.
* The last column is the resulting warp of the first onto the second.
* The test is performed in the following manner:
1. Pick a reference view V0
2. For all Vi, find a homographic warp Hi0 that maps Vi to V0
3. use the Hi0 as ground truth for descriptor matches from SIFT,SURF,and ORB

**Results**

The results is as follows:

![Real_World_data](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/Real_World_data.jpg)

![test3](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/test3.jpg)

From the pictures above, we can conclude that:
1. ORB **outperforms** SIFT and SURF on the outdoor dataset and the indoor set.
2. Blob-detection keypoints like SIFT tend to be better on graffititype images.

### test_4(Evaluation rBRIEF LSH with kdtrees of SIFT features using FLANN)
**Dataset**
* Pascal 2009 dataset

**Target**
* A correlation between **the speed and the accuracy** of **kd-trees with SIFT**(SURF is equivalent) and **LSH with rBRIEF**

**Results**

![test4.jpg](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/test4.jpg)

we can see that:
1. A successful match of the test image occurs when **more than 50** descriptors are found in the correct database image.
2. The LSH is **faster** than kd-trees.
3. The steered BRIEF is much slower due to its **uneven buckets**.

### test_5(time-cost)
**Datasets**
* A set of images at 5 scales
* The images size 640x480 are from the Pascal dataset 

**Conditions**
* ORB somputed separately on 5 scales of the image
* **area-based imterpolation** is used for efficient decimation
* The code was executed in a **single thread** running on an Intel i7 2.8Ghz processor

**Results**
* Computing ORB on a set of 2686 images at 5 scales, we can get the time cost is as follows: 

|ORB:|Pyramid|oFAST|rBRIEF|
|:-----:|:-----:|:-------:|:-------:|
|Time(ms)|4.43|8.68|2.12|

* Compared to SIFT and SURF on the same data for the same number of features and the same number of scales,we get:

|Detector|ORB|SURF|SIFT|
|:------:|:------:|:------:|:-----:|
|Time per frame(ms)|15.3|217.3|5228.7|

* ORB is **an order faster** than SURF, and **over two orders faster** than SIFT.
