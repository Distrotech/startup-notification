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

#include "test-boilerplate.h"

int
main (int argc, char **argv)
{
  Display *xdisplay;
  LfDisplay *display;
  LfLauncheeContext *context;
  int i;
  
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

  context = lf_launchee_context_new_from_environment (display);

  if (context == NULL)
    {
      fprintf (stderr, "Failed to get launch feedback info from DESKTOP_LAUNCH_ID/DESKTOP_LAUNCH_WINDOW\n");
      exit (1);
    }

  printf ("Launchee started with window 0x%lx ID \"%s\"\n",
          lf_launchee_context_get_launch_window (context),
          lf_launchee_context_get_launch_id (context));

  /* simulate startup time */
  i = 0;
  while (i < 4)
    {
      sleep (1);
      lf_launchee_context_pulse (context);
      ++i;
    }

  printf ("Launchee startup complete\n");
  lf_launchee_context_complete (context);
  
  while (TRUE)
    {
      XEvent xevent;

      XNextEvent (xdisplay, &xevent);

      lf_display_process_event (display, &xevent);
    }
  
  return 0;
}
