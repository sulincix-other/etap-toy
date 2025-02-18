#!/usr/bin/env python3

import gi
import os

import time

gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk

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


class DashWindow(Gtk.Window):
    def __init__(self, main):
        super().__init__(title="Etap Toy Dash", name="dash")
        self.main = main
        # Set window visual
        self.set_decorated(False)
        self.set_keep_above(True)
        self.set_skip_taskbar_hint(True)
        self.set_visual(self.get_screen().get_rgba_visual())

        self.flowbox = Gtk.FlowBox()
        self.flowbox.set_valign(Gtk.Align.CENTER)
        self.flowbox.set_halign(Gtk.Align.CENTER)

        # Set the size of the FlowBox to half of the screen size
        screen = Gdk.Screen.get_default()
        self.screen_width = screen.get_width()
        self.screen_height = screen.get_height()

        # Set the size of the FlowBox
        self.flowbox.set_size_request(self.screen_width / 2, self.screen_height / 2)

        self.add(self.flowbox)
        self.show_all()
        self.fullscreen()
        self.hide()

        self._x = 0
        self._y = 0

    def add_button(self,widget):
        self.flowbox.add(widget)
        widget.show()


    def on_show(self, widget=None):
        self._x, self._y = self.main.get_position()
        self.main.hide()
        self.show()

    def on_hide(self, widget=None):
        self.main.move(self._x, self._y)
        self.main.show()
        self.hide()

    def load_css(self):
        # Create a CSS provider and load the CSS data
        css_provider = Gtk.CssProvider()
        css_data = b"""
        #toy, #button {
            background-color: rgba(0, 0, 0, 0);
        }
        #dash {
            background-color: rgba(0.31, 0.31, 0.31, 0.69);
            color: white;
        }
        """
        css_provider.load_from_data(css_data)
        Gtk.StyleContext.add_provider_for_screen(
            Gdk.Screen.get_default(),
            css_provider,
            Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
        )
