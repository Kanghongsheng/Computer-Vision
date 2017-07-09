# Practice
----------

|Features|type|OpenCV_file|time-consuming|
|--------|------|-------|----------|
|FAST|detector|features2D.hpp|9ms in test|
|ORB|detector and descriptor| features2D.hpp|67ms in test|
|SIFT|detector and descriptor|xfeatures2D/nonfree.hpp|1.2s in test|
|SURF|detector and descriptor|xfeatures2D/nonfree.hpp|900ms in test|

* From ORB_matches.png, SIFT_matches.png and SURF_matches.png, we can see that the SIFT and SURF have a better performance than ORB in Matching accuracy. However,the speed of ORB is faster than that of SIFT and SURF, for which ORB has been widely used in real-time systems.
* There exist many mismatch in this practice, because of which we have to use other method like **RANSAC** to filter out the mismatch to improve the accuracy.
* In the code of Feature_Test.cpp, the minimum distance of BruteForce-Hamming is used to contribute to the accuracy of matching.
