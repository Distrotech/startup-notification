/*
 * Copyright (C) 2002 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <config.h>
#include <liblf/lf.h>
#include <liblf/lf-xmessages.h>

#include "test-boilerplate.h"

int
main (int argc, char **argv)
{
  Display *xdisplay;
  LfDisplay *display;

  if (argc != 3)
    {
      fprintf (stderr, "Must specify message type and message content as first and second args\n");
      return 1;
    }
  
  xdisplay = XOpenDisplay (NULL);
  if (xdisplay == NULL)
    {
      fprintf (stderr, "Could not open display\n");
      return 1;
    }

  if (getenv ("LIBLF_SYNC") != NULL)
    XSynchronize (xdisplay, True);
  
  XSetErrorHandler (x_error_handler);
  
  display = lf_display_new (xdisplay,
                            error_trap_push,
                            error_trap_pop);
  
  lf_internal_broadcast_xmessage (display,
                                  argv[1],
                                  argv[2]);
  
  return 0;
}
