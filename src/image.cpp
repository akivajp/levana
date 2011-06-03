/////////////////////////////////////////////////////////////////////////////
// Name:        src/image.cpp
// Purpose:     source for image handling classes
// Author:      Akiva Miura <akiva.miura@gmail.com>
// Modified by:
// Created:     01/25/2011
// Copyright:   (C) 2010-2011 Akiva Miura
// Licence:     MIT License
/////////////////////////////////////////////////////////////////////////////

#include "prec.h"
#include "lev/image.hpp"
#include <wx/rawbmp.h>

namespace lev
{
  bitmap::bitmap(int width, int height) : base()
  {
    try {
      wxBitmap *bmp = new wxBitmap(width, height, 32);
      _obj.reset(bmp);
      this->clear();
    }
    catch (...) {
      fprintf(stderr, "bitmap: allocation error");
    }
  }


  bitmap::bitmap(const char *filename)
  {
    try {
      wxString f = wxString(filename, wxConvUTF8);
      wxImage img(f);
      wxBitmap *bmp = new wxBitmap(img);
      _obj.reset(bmp);
      if ( !(bmp->IsOk()) ) { wxMessageBox(_("error!!!")); }
    }
    catch (...) {
      fprintf(stderr, "bitmap: file load error");
    }
  }


  void bitmap::clear()
  {
    if (!_obj) { return; }
    int num_pix = geth() * getw();
    wxBitmap *bmp = (wxBitmap *)_obj.get();
    wxAlphaPixelData data(*bmp);
// deprecated in 2.9
//    data.UseAlpha();
    wxAlphaPixelData::Iterator p(data);
//    for (int i = 0 ; i < num_pix ; i++, p++) { p.Alpha() = 128; }
  }


  void bitmap::drawcircle(int x, int y, int radius)
  {
    if (!_obj) { return; }
    wxMemoryDC dc;
    wxBitmap *bitmap = (wxBitmap *)_obj.get();
    dc.SelectObject(*bitmap);
//    wxGCDC gdc(dc);
//    gdc.DrawCircle(x, y, radius);
    dc.DrawCircle(x, y, radius);
  }


  void bitmap::drawtext(const char *text, int x, int y, double angle)
  {
    if (!_obj) { return; }
    wxMemoryDC dc;
    wxBitmap &bitmap = *((wxBitmap *)_obj.get());
    dc.SelectObject(bitmap);
    dc.DrawRotatedText(wxString(text, wxConvUTF8), x, y, angle);
  }


  int bitmap::geth() const
  {
    if (!_obj) { return -1; }
    return ((wxBitmap *)_obj.get())->GetHeight();
  }


  int bitmap::getw() const
  {
    if (!_obj) { return -1; }
    return ((wxBitmap *)_obj.get())->GetWidth();
  }


  bool bitmap::isvalid() const
  {
    if (!_obj) { return false; }
    return ((wxBitmap *)_obj.get())->IsOk();
  }


  bool bitmap::save(const char *filename) const
  {
    if (!_obj) { return false; }
    wxString f = wxString(filename, wxConvUTF8);
    return ((wxBitmap *)_obj.get())->ConvertToImage().SaveFile(f);
  }
}

