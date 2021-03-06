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

#include "input.h"

#include <assert.h>
#include <stdlib.h>
#include "SDL.h"
#include "video.h"
#include "input_dumb.h"
#include "cursor.h"
#include "global.h"

Input *input = NULL;

Input::Input()
{
  pause = false;
  allow_repeat = false;

  mouse.quel = -1;
  mouse.wheel = 0;
  for(int i = 0; i < 4; ++i)
    mouse.button[i] = RELEASED;

  clear_key();
}

void Input::clear_key() {
  last_key.sym = SDLK_UNKNOWN;
  key_pending = 0;
  
  for(int i = SDLK_FIRST; i < SDLK_LAST; ++i)
    keys[i] = 0;
}

void Input::allow_key_repeat(bool _allow)
{
	if(allow_repeat == _allow) return;
	allow_repeat = _allow;
	if(allow_repeat)
	{
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
		SDL_EnableUNICODE(1);
	}
	else
	{
		SDL_EnableKeyRepeat(0, 0);
		SDL_EnableUNICODE(0);
	}
}

class Input_SDL: public Input {
public:
  Input_SDL();
  virtual ~Input_SDL();
  virtual void check();
};

Input* Input::New(bool dumb) {
  if(dumb)
    return new Input_Dumb();
  else
    return new Input_SDL;
}

Input_SDL::Input_SDL() {
}

Input_SDL::~Input_SDL() {
}

void Input_SDL::check() {
  SDL_Event event;
  mouse.wheel = 0;

  while(SDL_PollEvent(&event)) {
    switch(event.type) {
    case SDL_QUIT:
      delete_obj();
      exit(0);
      break;

    case SDL_MOUSEMOTION:
      if(cursor)
      {
        video->transform_to_local(event.motion.x, event.motion.y);
        cursor->set_pos(event.motion.x, event.motion.y);
      }
      break;

    case SDL_MOUSEBUTTONDOWN:
      switch(event.button.button) {
      case SDL_BUTTON_LEFT:
        if(mouse.button[0] == RELEASED)
          mouse.quel = 0;
        mouse.button[0] = PRESSED;
        break;
      case SDL_BUTTON_MIDDLE:
        if(mouse.button[1] == RELEASED)
          mouse.quel = 1;
        mouse.button[1] = PRESSED;
        break;
      case SDL_BUTTON_RIGHT:
        if(mouse.button[2] == RELEASED)
          mouse.quel = 2;
        mouse.button[2] = PRESSED;
        break;
      case SDL_BUTTON_WHEELUP:
		mouse.wheel = 1;
		break;
      case SDL_BUTTON_WHEELDOWN:
		mouse.wheel = -1;
		break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch(event.button.button) {
      case SDL_BUTTON_LEFT:
        mouse.button[0] = RELEASED;
        break;
      case SDL_BUTTON_MIDDLE:
        mouse.button[1] = RELEASED;
        break;
      case SDL_BUTTON_RIGHT:
        mouse.button[2] = RELEASED;
        break;
      }
      break;

    case SDL_KEYDOWN:
      if((event.key.keysym.sym == SDLK_p && event.key.keysym.mod & (KMOD_META|KMOD_ALT))
		 || event.key.keysym.sym == SDLK_PAUSE)
        pause = true;
      else if(event.key.keysym.sym == SDLK_RETURN
		  && event.key.keysym.mod & (KMOD_META|KMOD_ALT))
        video->toggle_fullscreen();
      else if((event.key.keysym.sym == SDLK_q
          && event.key.keysym.mod & (KMOD_META|KMOD_ALT))
          || (event.key.keysym.sym == SDLK_F4
          && event.key.keysym.mod & (KMOD_ALT)))
	  {
	    delete_obj();
        exit(0);
	  }
      else {
        keys[event.key.keysym.sym] |= PRESSED;
        last_key = event.key.keysym;
        if(key_pending < MAXKEY) {
		  if(event.key.keysym.unicode <= 255)
		  {
			key_buf[key_pending] = event.key.keysym.unicode & 0xff;
          }
		  else
		  {
			key_buf[key_pending] = 0;
		  }
          key_sym_buf[key_pending] = event.key.keysym.sym;
          ++key_pending;
        }
      }
      break;

    case SDL_KEYUP:
      keys[event.key.keysym.sym] = RELEASED;
      break;
      
    case SDL_VIDEORESIZE:
    {
      SDL_ResizeEvent* resize = (SDL_ResizeEvent *) &event;
      video->resize_event(resize->w, resize->h);
      break;
    }

    default:
      break;
    }
  }
}

