/*
 * Copyright (c) 2013, University of Massachusetts Lowell.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of University of Massachusetts Lowell. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Author: Mikhail Medvedev */

#include <zbar_barcode_reader/barcode_reader.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

class Node
{
private:
  barcode::BarcodeReader reader_;
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber sub_image_;
  float barcode_size; ///< Real size of the barcode, width or height (square barcode)

  void imageCallback(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_img_ptr;

    try
    {
      cv_img_ptr = cv_bridge::toCvCopy(msg, "mono8");
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR_STREAM("Could not convert ROS image to CV: "<< e.what());
      return;
    }

    ROS_INFO_STREAM("Got image, has "<<reader_.parse(cv_img_ptr)<<" barcodes");
    std::vector<barcode::Barcode> barcodes = reader_.getBarcodes();
    for (int i = 0; i < reader_.getBarcodes().size(); i++)
    {
      ROS_INFO_STREAM("Barcode: " << barcodes[i].data //
          <<" diag:"<< barcodes[i].diagonal_size//
          << " x:"<<barcodes[i].x//
          << " y:"<<barcodes[i].y);
    }

  }

public:
  Node() :
          it_(nh_),
          sub_image_(it_.subscribe("image", 1, &Node::imageCallback, this)),
          barcode_size(0.2)
  {
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "barcode_reader");
  Node node;
  ros::spin();
  return 0;
}

