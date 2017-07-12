# Feature Detection
-----------------------
## FAST
--------
**Introduction**

* __FAST__ which means _"Features from accelerated segment test"_, is the first corner detect algorithm based on the **AST** to find a set of key points in the image. 

**Algorithm**

* The main idea of the algorithm is to evaluating the pixels in a Bresenham circle of radius _r_ around the candidate point.
* If _n_ contiguous pixels are all brighter than the nucleus by at least _t_ or all darker than the nucleus by _t_, then the pixel under the nucleus is considered to be a feature.
* Without the improvement of *machine learning*, N is usually chosen as 12. In OpenCV, other type of FAST are available.

If N is 12, you can follow these steps to **accelerate** the algorithm:
1. Examining pixels 1 and 9,if both are within*[Ip-t,Ip+t]* ,then candidate is not a corner
2. Otherwise pixels 5 and 13 are examined. if there exists 3 of them that are either brighter or darker, then rest pixels are then examined for final conclusion

**Reference**

> _Features from accelerated segment test_ in wiki

> "_Real-time Video Annotations for Augmented Reality_" Edward Rosten 2005

## **SURF**
------------
**Introduction**

_SURF_ which means "_Speeded up Robust Features_", is a **scale- and rotation-invariant interest point detector and descriptor**.

**'_Fast-Hessian_' Detector**
* **Integral images** is used to reduce the computation time.
* The detector is based on the **Hessian matrix**, the **Hessian matrix** in x at scale \sigma is defined as follows:
![Hessian Matrix](https://github.com/Kanghongsheng/Computer-Vision/raw/modified_version/Feature_Detection/Pictures/Hessian.jpg)

where _Lxx_ is the convolution of the Gaussian second order derivative with the image _I_ in point _x_. 
* For Gaussian filters are not-ideal in any case,we use **box filters** to approximate Gaussian filter.
* The scale space is analysed by up-scaling the filter size rather than iteratively reducing the image size, due to the use of box filters and integral images.
* A **non-maximum suppression** in a 3x3x3 neighbourhood is applied to lacalise interest points in the image and over scales. 
* The _determinant_ of the Hessian matrix is used as a measure of local change around the point and points are chosen where the determinant is maximal.

**Distribution-based descriptor**
* The descriptor describes a **distribution of Harr-wavelet responses** within the interest point neighborhood.
* **Integral images** reducing the time for feature computation and matching, and increasing simultaneously the robustness.


* The  first step consists of fixing a reproducible orientation based on information from a **circular region around the interest point**. 
 * Then we construct a **square region aligned to the selected orientation**, and extract the SURF descriptor from it.

**Descriptor**
* The **Haar wavelet responses** in both x- and y-directions within a circular neighbourhood of radius *6s* around the point of interest are computed, where _s_ is the scale at which the point of interest was detected.
* The **responses are weighted** by a Gaussian function centered at the point of interest, then represented as **vectors** in a two-dimensional space, with the horizontal response in the abscissa and the vertical response in the ordinate.
* The **dominant orientation** is estimated by calculating the sum of all responses within a sliding orientation window of size π/3. The horizontal and vertical responses within the window are summed.
* The size of the sliding window is a parameter that has to be chosen carefully to achieve a desired balance between **robustness and angular resolution**.
* The longest such vector overall defines **the orientation of the point of interest**. 

**Reference**
> "Speeded up robust features" in wiki

> "SURF: Speed Up Robust Features" by Herbert Bay in ETH Zurich

## **ORB**
---------
**Introduction**
Oriented FAST and rotated BRIEF(ORB) is an algorithm, which use **FAST** to detect points of interest and describe it via **BRIEF** algorithm. To get a target number of _N_ points by this algorithm, we first set the threshold low enough to get more than _N_ keypoints, then order them according to the **Harris corner measure** and pick the top _N_ points.
Besides, we employ a **scale pryamid of the image** to produce FAST features(filtered by Harris) at each level in the pyramid to produce multi-scale features.

**Detector**(_oFAST:FAST Keypoint Orientation_)

* a _Harris corner measure_ to order the **FAST** keypoints
* Orientation by _Intensity Centroid_

**Descriptor**(_rBRIEF:Rotation-Aware BRIEF_)

> Steered BRIEF: 
>>in-plane rotation invariant
>>steer BRIEF according to the orientation of keypoints

> Variance and Correlation
>>High variance makes a feature more discriminative
>>Uncorrelated means more new information

> learning Good Binary Featuress to get **rBRIEF**
>> **Aim**: To recover from the loss of variance in steered BRIEF,and to reduce correlation among the binary tests.
>>Run each test against all training patches
>>Order the tests by their distance from a mean of 0.5, forming the vector T.
>>Greedy search

**Scalable Matching of Binary Features**
> We choose Locality Sensitive Hashing as our nearest neighbor search

## **SIFT**
---------
* detector and descriptor
* This approach transforms an image into a large collection of local feature vectors,each of which is invariant to image translation, scaling, and rotation, and partially invariant to illumination changes and affine or 3D projection.

### Tips
* The scale-invariant features are efficiently identified by using a staged filtering approach.
* Partial invariance to local variations, such as affine or 3D projections, by blurring image gradient locations.

### Detector
1. Use the Gaussian kernel and its derivations to build an image pyramid.
2. Select key locations at maxima and minima of a difference of Gaussian function applied in scale space.

### Descriptor
1. Compute the image gradient magnitude and orientation at each pxel at each level
2. Each key location is assigned a canonical orientation which is determined by the peak in a histogram of local image gradient orientations.

|Problem|Technique|Advantage|
|:--------:|:--------------:|:---------:|
|key localization/scale/rotation|DoG/scale-space pyramid/orientation assignment|accuracy,stability,scale&rotational invariance|
|geometric distortion|blurring/resampling of local image orientation planes|affine invariance|
|indexing and matching|nearest neighbor/ Bin First search|Efficiency/speed|
|Cluster identification|Hough Transform voting|reliable pose models|
|Model verification/outlier detection|Linear least squares|better error tolerance with matches|
|Hypothesis acceptance|Bayesian Probability analysis|reliability|
