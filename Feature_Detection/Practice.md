# Practice
----------

|Features|type|OpenCV_file|time_cost|match_type|
|--------|------|-------|----------|-----------|
|FAST|detector|features2D.hpp|9ms in test||
|ORB|detector and descriptor| features2D.hpp|67ms in test|BruteForce|
|SIFT|detector and descriptor|xfeatures2D/nonfree.hpp|1.2s in test|FlannBased|
|SURF|detector and descriptor|xfeatures2D/nonfree.hpp|900ms in test|FlannBased|
