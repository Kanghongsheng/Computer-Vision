# Monocular pose and shape estimation of moving targets, for autonomous rendezvous and docking
----------
Name: Sean Augenstein
Time: June 2011
School: Stanford University
---------
Objective: The design and implementation of an algorithm for tracking a moving target.
Assumption: No a priori information about the target is assumed
Equipment: A single camera 
Means: Simultaneously estimation of both the target's 6DOF pose and 3D shape.
Key Technology: 
> 1. real-time frame-to-frame pose estimation
> 2. shape reconstruction
> 3. Bayesian estimation methods
> 4. Nonlinear optimization techniques

Features:
> real-time capable, feasible, smooth estimate of the relative pose.
------------
##Chapter 2
Mainly discuss the Image Processing technology
The vision approach taken in this research is feature-based and the map constructed is sparse point cloud of the locations of these features. 
###Chapter 2.1 introducted some Robust image features. 
Chapter 2.1.1 introduced SIFT feature in detail. 
For the SIFT feature doesn't perform well in low intensity range of light, chapter 2.1.2 suggest to implement the Constrast Limited Adaptive Histogram Equalization(CLAHE) before the SIFT detection step.
Chapter 2.1.3 mentioned the feature detection via GPUs
###Chapter 2.2 
introduced a few data structures used for storing and processing the raw features
Chapter 2.2.1 introduce the classical SIFT match algorithm (Euclidean distance and threshold)
Chapter 2.2.2 use an additional logical check to improve match performance and eliminate false positives.
###Chapter 2.3
construct a model representing how the camera physically maps 3D locations into 2D pixel measurements.

----
##Chapter 3
This chapter presents the SLAM/SFM problem and describes the existing approaches to online SLAM/SFM in the context of static targets.
### Chapter3.1
Introduction of background of SLAM and SFM
### Chapter3.2
Mainly discuss two models that relate the estimators' states and measurements, the measurement model and motion model.
### Chapter 3.3
make contrast between **Bayesian Filtering** and **Nonlinear Optimization**.
Chapter 3.3.1 introduce the **Bayesian Filtering** in detail, including **Extended Kalman Filters**, **Extended Information Filters**, **Particle Filters**, **Rao-Blackwellized Particle Filters**
Chapter 3.3.2 introduce the **Nonlinear Optimization** in the following aspect: **Iterative Minimization, Effects of Measurement Noise, Initializing the Minimization, Connection to Bundle Adjustment**.

----------
##Chapter 4
This chapter explores the application of existing SLAM/SFM algorithms to the problem of real-time tracking reconstruction of an unpredictable _moving_ target.
### Chapter 4.1
Illustration about No Motion Model Approaches. There are online SLAM/SFM algorithms that use only the information from the camera measurements.
Chapter 4.1.1 discuss the Uncertainty in Pose Estimate.
Chapter 4.1.2 discuss the Ambiguity in Orientation Estimate.
### Chapter 4.2
Discussion about the MotionModel Approaches.

##Chapter5
This chapter presents a novel solution to tracking the relative pose in real-time during moving target SLAM/SFM, which represents a hybirdization of Bayesian filtering and nonlinear optimization. The concept is to separate orientation and position states, estimate the former based on Bayesian principles involving a motion model, and estimate the latter based on measurement model-only nonlinear optimization.
###Chapter 5.1 
Chapter 5.1.1 presents the Rotation Estimation 
Chapter 5.1.2 presents the Translation Estimation
### Chapter 5.2
Chapter 5.1 described the general approach to hybrid pose tracking, this chapter presents the implementation with Rao-Blackwellized particle filters.
Chapter5.2.2 describes the three steps of hybrid estimation with a Rao-Blackwellized particle filter: _Time Update, Measurement Update, Particle Weighting and Resampling_.
Chapter 5.2.3 discuss the feature initialization in EKF.
##Chapter 6
Three sets of simulations and experiments were conceived to test the performance of the new hybrid algorithm for moving target SLAM/SFM: simulations with a synthetic target and camera, laboratory experiments, and field experiments.
###Chapter 6.1
Two SLAM/SFM algorithms are compared in **simulation**: a standard Bayes filter and the hybrid algorithm, they differ in the pose estimation process but have the same computationally complexity.
Chapter 6.1.1 shows the simulation results
### Chapter 6.2
presents some laboratory experiments 
Chapter6.2.1 presents the essential hardware.
Chapter 6.2.2 shows the results
### Chapter6.3
Show the experiments in the **filed** with real vision and complex lighting conditions, to demonstrate its real-time capability for tracking and reconstructing a moving target with **a prior unknown shape and motion properties**.
Chapter 6.3.1 introduce the field hardware
Chapter 6.3.2 shows the results
### 6.4 Summary
## Chapter 7 Conclusion
Including review of contributions and future research
