/* -*- Mode: C++; c-basic-offset: 2; tab-width: 2; indent-tabs-mode: nil -*-
 * 
 * Quadra, an action puzzle game
 * Copyright (C) 1998-2000  Ludus Design
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "palette.h"

#include "video.h"

Palette noir;

void Palette::load(const Image& raw) {
  size=raw.palettesize();
  int j=0;
  for(int i(0); i<size; i++) {
    pal[i].r = raw.pal()[j++];
    pal[i].g = raw.pal()[j++];
    pal[i].b = raw.pal()[j++];
  }
}

Remap::Remap(const Palette& d, Palette* src): dst(d) {
  if(src) {
    for(int i=0; i<src->size; i++)
      findrgb(i, src->r(i), src->g(i), src->b(i));
  }
}

void Remap::findrgb(const Byte m, const Byte r, const Byte g, const Byte b) {
  int best_diff=9999999, best_i=0, diff;
  for(int i=1; i<dst.size; i++) {
    diff=(int) ((dst.pal[i].r-r)*(dst.pal[i].r-r)*2 + (dst.pal[i].g-g)*(dst.pal[i].g-g)*3 + (dst.pal[i].b-b)*(dst.pal[i].b-b));
    if(diff == 0) {
      map[m] = i;
      return;
    }
    if(diff < best_diff) {
      best_i = i;
      best_diff = diff;
    }
  }
  map[m] = best_i;
}

Fade::Fade(const Palette& dst, const Palette& src, int frame) {
  int j=0;
  for(int i(0); i<256; i++) {
    current[j++]=src.pal[i].r<<7;
    current[j++]=src.pal[i].g<<7;
    current[j++]=src.pal[i].b<<7;
  }
  newdest(dst, frame);
}

void Fade::setdest(const Palette& dst) {
  dest=dst;
  int j=0;
  for(int i(0); i<256; i++) {
    current[j++]=dest.pal[i].r<<7;
    current[j++]=dest.pal[i].g<<7;
    current[j++]=dest.pal[i].b<<7;
  }
  video->setpal(dest);
  currentframe=destframe;
}

void Fade::newdest(const Palette& dst, int frame) {
  dest=dst;
  int j=0;

  for(int i(0); i<256; i++) {
    delta[j]=((dest.pal[i].r<<7)-current[j])/frame;
	j++;
    delta[j]=((dest.pal[i].g<<7)-current[j])/frame;
	j++;
    delta[j]=((dest.pal[i].b<<7)-current[j])/frame;
	j++;
  }
  currentframe=0;
  destframe=frame;
}

int Fade::step() {
  if(currentframe==destframe)
    return 1;
  else {
    for(int i(0); i<768; i++)
      current[i]+=delta[i];
    currentframe++;
    return 0;
  }
}

void Fade::set() {
  if(currentframe==destframe)
    return;
  if(currentframe==destframe-1) {
    video->setpal(dest);
  } else {
    Palette mypal;
    for(int i(0); i<256; i++)
      mypal.setcolor(i, current[i*3]>>7, current[i*3+1]>>7, current[i*3+2]>>7);
    video->setpal(mypal);
  }
}

