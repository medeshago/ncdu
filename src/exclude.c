/* ncdu - NCurses Disk Usage 
    
  Copyright (c) 2007-2012 Yoran Heling

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

#include "exclude.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>


struct exclude {
  char *pattern;
  struct exclude *next;
} *excludes = NULL;



void exclude_add(char *pat) {
  struct exclude **n;

  n = &excludes;
  while(*n != NULL)
    n = &((*n)->next);

  *n = (struct exclude *) calloc(1, sizeof(struct exclude));
  (*n)->pattern = (char *) malloc(strlen(pat)+1);
  strcpy((*n)->pattern, pat);
}


int exclude_addfile(char *file) {
  FILE *f;
  char buf[256];
  int len;

  if((f = fopen(file, "r")) == NULL)
    return 1;

  while(fgets(buf, 256, f) != NULL) {
    len = strlen(buf)-1;
    while(len >=0 && (buf[len] == '\r' || buf[len] == '\n'))
      buf[len--] = '\0';
    if(len < 0)
      continue;
    exclude_add(buf);
  }

  fclose(f);
  return 0;
}


int exclude_match(char *path) {
  struct exclude *n;
  char *c;

  for(n=excludes; n!=NULL; n=n->next) {
    if(!fnmatch(n->pattern, path, 0))
      return 1;
    for(c = path; *c; c++)
      if(*c == '/' && c[1] != '/' && !fnmatch(n->pattern, c+1, 0))
        return 1;
  }
  return 0;
}


void exclude_clear() {
  struct exclude *n, *l;

  for(n=excludes; n!=NULL; n=l) {
    l = n->next;
    free(n->pattern);
    free(n);
  }
  excludes = NULL;
}

