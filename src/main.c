/* ncdu - NCurses Disk Usage 
    
  Copyright (c) 2007 Yoran Heling

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:
  
  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "ncdu.h"

/* check ncdu.h what these are for */
struct dir dat;
int winrows, wincols;
char sdir[PATH_MAX];
int sflags, bflags, sdelay, bgraph;

/* main program */
int main(int argc, char **argv) {
  int r, gd;
  gd = settingsCli(argc, argv);
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  ncresize();
  
  if(gd && settingsWin()) goto mainend;
  while((r = calcUsage()) != 0) {
    if(r == 1 && settingsWin()) goto mainend;
    else if(r == 2) goto mainend;
  }
  showBrowser();

  mainend:
  erase();
  refresh();
  endwin();

  return(0);
}

