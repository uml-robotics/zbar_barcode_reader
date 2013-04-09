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

#ifndef BARCODE_READER_H_
#define BARCODE_READER_H_

#include <cv_bridge/cv_bridge.h>

#include <vector>

namespace barcode
{

struct Barcode
{
  std::string data;
  double x;
  double y;
  double z;
  Barcode() :
          x(0),
          y(0),
          z(0)
  {
  }
};

/*
 *
 */
class BarcodeReader
{
private:
  std::vector<Barcode> barcodes;
  // Assuming camera with f=1
  double camera_horizontal_fov_deg_; // Degrees
  double barcode_size_; // Meters
  double sensor_size_;

  void updateSensorSize();

public:
  BarcodeReader() :
          camera_horizontal_fov_deg_(60),
          barcode_size_(0.16)
  {
    updateSensorSize();
  }
  virtual ~BarcodeReader();

  /**
   *
   * @param fov Camera horizontal field of view in degrees.
   * @todo Use camera calibration params
   * @return
   */
  BarcodeReader & setFOV(double fov);

  /**
   *
   * @param size Rectangular barcode size, in meters.
   * We assuming that all the barcodes we encounter would
   * be the same size. Could get rid of this requirement by encoding barcode's
   * size in the barcode itself.
   * @return
   */

  BarcodeReader & setBarcodeSize(double size);

  int parse(const cv_bridge::CvImageConstPtr & cv_img_ptr);

  std::vector<Barcode> & getBarcodes();
};

} /* namespace barcode */
#endif /* BARCODE_READER_H_ */
