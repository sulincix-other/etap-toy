#!/usr/bin/env python3

import gi
import os

import time

gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk, GdkPixbuf

from DashWindow import DashWindow

try:
    import locale
    from locale import gettext as _

    # Translation Constants:
    APPNAME = "example"
    TRANSLATIONS_PATH = "/usr/share/locale"
    locale.bindtextdomain(APPNAME, TRANSLATIONS_PATH)
    locale.textdomain(APPNAME)
except:
    # locale load fallback
    def _(msg):
        return msg


class MainWindow(Gtk.Window):
    def __init__(self, application):
        super().__init__(title="Etap Toy", name="toy")

        # Set window visual
        self.set_decorated(False)
        self.set_keep_above(True)
        self.set_skip_taskbar_hint(True)
        self.set_visual(self.get_screen().get_rgba_visual())
        self._time = 0

        self.set_application(application)
        but = Gtk.Button(name="button")
        image = Gtk.Image.new_from_icon_name("gtk-yes", Gtk.IconSize.BUTTON)
        image.set_pixel_size(64)
        but.add(image)

        # Signals
        self.connect("destroy", Gtk.main_quit)
        but.connect("motion-notify-event", self.on_motion_notify_event)
        but.connect("button-release-event", self.on_release_event)
        but.connect("button-press-event", self.on_press_event)
        but.set_size_request(64,64)

        # Add button
        self.add(but)
        # Dash
        self.dash = DashWindow(self)

        # Add some dash button
        minify = Gtk.Button(label="minify")
        minify.connect("clicked", self.dash.on_hide)
        self.dash.add_button(minify)

        # Load CSS for transparency
        self.dash.load_css()

        # show main window
        self.show_all()

    def do_popup_event(self, widget=None, event=None):
        print("Hello World")
        self.dash.on_show()


    def on_release_event(self, widget, event):
        # Do event if clicked
        if (time.time() * 1000) - self._time  < 150:
            self.do_popup_event()

    def on_press_event(self, widget, event):
        self._time = time.time() * 1000

    def on_motion_notify_event(self, widget, event):
        if (event.state & Gdk.ModifierType.BUTTON1_MASK) == Gdk.ModifierType.BUTTON1_MASK:
            # Get the allocation (size) of the window
            allocation = widget.get_allocation()
            w = allocation.width
            h = allocation.height

            # Move the window to the mouse position, centered
            self.move(int(event.x_root - (w / 2)), int(event.y_root - (h / 2)))
            self._time = 0
            return True
        return False
