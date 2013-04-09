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

#include "zbar_barcode_reader/barcode_reader.h"

#include <Magick++.h>
#include <zbar.h>

#include <iostream>

namespace barcode
{

BarcodeReader::BarcodeReader()
{
}

BarcodeReader::~BarcodeReader()
{
}

int BarcodeReader::parse(const cv_bridge::CvImageConstPtr& cv_img_ptr)
{
  zbar::ImageScanner scanner;
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  int width = cv_img_ptr->image.cols;
  int height = cv_img_ptr->image.rows;

  Magick::Blob blob(cv_img_ptr->image.ptr(0), width * height);

  zbar::Image image(width, height, "Y800", blob.data(), width * height);

  int n = scanner.scan(image);
  barcodes.clear();
  if (n > 0)
  {
    zbar::Image::SymbolIterator symbol = image.symbol_begin();
    for (; symbol != image.symbol_end(); ++symbol)
    {
      if (symbol->get_location_size() == 4)
      {
        Barcode barcode;
        barcode.data = symbol->get_data();
        for (int i = 0; i < symbol->get_location_size(); i++)
        {
          barcode.x += symbol->get_location_x(i);
          barcode.y += symbol->get_location_y(i);
        }
        barcode.x /= symbol->get_location_size();
        barcode.y /= symbol->get_location_size();

        int dx0 = symbol->get_location_x(0) - symbol->get_location_x(2);
        int dx1 = symbol->get_location_x(1) - symbol->get_location_x(3);
        int dy0 = symbol->get_location_y(0) - symbol->get_location_y(2);
        int dy1 = symbol->get_location_y(1) - symbol->get_location_y(3);
        float d0 = dx0 * dx0 + dy0 * dy0;
        float d1 = dx1 * dx1 + dy1 * dy1;
        barcode.diagonal_size = (sqrt(d0) + sqrt(d1)) / 2.0;
        barcodes.push_back(barcode);
      }
    }
  }
  return n;
}

std::vector<Barcode>& barcode::BarcodeReader::getBarcodes()
{
  return barcodes;
}

} /* namespace barcode */

