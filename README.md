# An occlusion-resistant circle detector using inscribed triangles


## Introduction
用了git上的这个论文：https://github.com/zikai1/CircleDetection
没有完全研究有空再说。对比了opencv的圆检测和基于圆内接三角形的快速高精圆检测，好像是这个巨佬的厉害一点hhhh。
论文地址：https://zikai1.github.io/cirDet.pdf
基于Qt开发，slicer是切片类，他居然能跑起来？

![Image text]https://github.com/AKYakult/sliceTest/blob/main/origin.png


## Instructions
### 1. Requirements
The code was implemented with Qt, OpenCV 3.4.7, and Eigen3.

### 2. Detection of your data
To test images for your own data. Run the 'test.cpp' in the './src' directory.  
It allows you to specify the input file path:  
cv::String path = "E:/Code/patterns/Images1/";  
and the output path for the detected results:  
cv::String dst = "E:/Code/patterns/result/";  
Here, you need to create two directories, ie, 'Images1' and 'result'. If there are corresponding ground truths (GT), then you can further add the GT path:  
cv::String GT = "E:/Code/patterns/GT/";

### 3. Data sets
参见上述链接


## Suggestions

“T_inlier” 的比率阈值越大，越严格
锐角阈值“sharp_angle” 要检测小圆圈,可以稍微调整一下
其他参数通常是固定的。

## Citation
@article{zhao2021occlusion, title={An occlusion-resistant circle detector using inscribed triangles}, author={Zhao, Mingyang and Jia, Xiaohong and Yan, Dong-Ming}, journal={Pattern Recognition}, volume={109}, pages={107588}, year={2021}, publisher={Elsevier} }
# slicerTest
