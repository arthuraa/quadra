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

#ifndef _HEADER_VIDEO
#define _HEADER_VIDEO

#include "SDL.h"
#include "clipable.h"
#include "palette.h"
#include "bitmap.h"

class Bitmap;

class Video_bitmap: public Clipable {
public:
  Video_bitmap(int px, int py, int w, int h);
  ~Video_bitmap();

  /* fills a rectangle at position 'x','y' of width 'w', height 'h'
     and color 'color'. */
  void rect(const int x,const int y,const int w,const int h,
		    const int color) const;

  /* empty rectangle at position 'x','y' of width 'w', height 'h' and
     color 'color'. */
  void box(const int x,const int y,const int w,const int h,
		   const int color) const;

  /* puts a pixel at position 'x','y' with color 'c' */
  void put_pel(const int x, const int y, const Byte c) const;

  /* horizontal line starting from 'x','y', width 'w' and color 'c' */
  void hline(const int y, const int x,
		     const int w, const Byte c) const;

  /* vertical line starting from 'x','y', height 'h' and color 'c' */
  void vline(const int x, const int y,
		     const int h, const Byte c) const;

  void put_surface(SDL_Surface* surface, int dx, int dy) const;
  void put_surface(SDL_Surface* surface, const SDL_Rect& _srcrect, int dx, int dy) const;

  /* blits a Bitmap to position 'dx','dy' */
  void put_bitmap(const Bitmap& d, int dx, int dy) const;

private:
  friend class Video;

  void clip_dirty(int x, int y, int w, int h) const;

  const int pos_x;
  const int pos_y;
};

class Video {
	friend class Video_bitmap;

public:
	Video();
  ~Video();

  void end_frame();
  void setpal(const Palette& p);
  void snap_shot(int x, int y, int w, int h);
  void toggle_fullscreen();
  void resize_event(int w, int h);
  SDL_Surface* surface() const {
    return offscreen;
  }
  void clone_palette(SDL_Surface* surface);
  void transform_to_local(Uint16& x, Uint16& y);

  Video_bitmap vb;
  int need_paint;
  Dword framecount;

private:
  void SetVideoMode();
  void set_dirty(int x1, int y1, int x2, int y2);
  void create_display(int w, int h);
  void update_dirty_display();
  void update_sdl_display();

  bool newpal;
  Palette pal;
  int needsleep;
  int lastticks;
  bool fullscreen;
	bool mDirtyEmpty;
	int mDirtyX1;
	int mDirtyY1;
	int mDirtyX2;
	int mDirtyY2;
  SDL_Surface* display; // real SDL video surface
  SDL_Surface* offscreen; // internal surface always in 640x480
  Uint32* mHScaleDst2Src; // horizontal scaling conversion table (destination to source)
  Uint32* mVScaleDst2Src; // vertical scaling conversion table (destination to source)
  Uint32 mHScaleSrc2Dst[641]; // horizontal scaling conversion table (source to destination)
  Uint32 mVScaleSrc2Dst[481]; // vertical scaling conversion table (source to destination)
};

extern Video* video;

#endif
