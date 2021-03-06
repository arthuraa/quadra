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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "error.h"
#include "res.h"

#ifndef NDEBUG
	bool _debug = true;
#else
	bool _debug = false;
#endif

bool skelton_debug = true;

static void output_msg(char *m) {
	OutputDebugString(m);
	int siz=strlen(m);
	if(m[siz-1] ==10) {
		m[siz-1] = 13;
		m[siz] = 10;
		siz++;
	}
	static Res_dos out("output.txt", RES_CREATE);
	if(out.exist) // so we don't crash if creating 'output.txt' didn't work!
		out.write(m, siz);
}

void fatal_msgbox(const char* m, ...) {
	delete_obj();
	ShowCursor(TRUE);
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	char st[1024];
	va_list marker;
	va_start(marker, m);
	vsprintf(st, m, marker);
	va_end(marker);
	msgbox("Error::Error: %s\n", st);
	MessageBox(NULL, st, "Error", MB_ICONEXCLAMATION);
	exit(1);
}

void msgbox(const char* m, ...) {
	if(_debug) {
		char st[1024];
		va_list marker;
		va_start(marker, m);
		vsprintf(st, m, marker);
		va_end(marker);
		output_msg(st);
	}
}

void skelton_msgbox(const char* m, ...) {
	if(_debug && skelton_debug) {
		char st[1024];
		va_list marker;
		va_start(marker, m);
		vsprintf(st, m, marker);
		va_end(marker);
		output_msg(st);
	}
}

void user_output(const char* title, const char *msg) {
	ShowCursor(TRUE);
	MessageBox(NULL, msg, title, MB_ICONINFORMATION);
	ShowCursor(FALSE);
}
