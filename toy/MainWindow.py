#!/usr/bin/env python3

import gi
import os

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


class MainWindow(Gtk.Window):
    def __init__(self, application):
        super().__init__(title="Etap Toy", name="toy")

        # Set window visual
        self.set_decorated(False)
        self.set_keep_above(True)
        self.set_skip_taskbar_hint(True)
        self.set_visual(self.get_screen().get_rgba_visual())

        # Load CSS for transparency
        self.load_css()

        self.set_application(application)

        # Destroy signal
        self.connect("destroy", Gtk.main_quit)

        # Add button
        self.add(Gtk.Button())

        # show main window
        self.show_all()

    def load_css(self):
        # Create a CSS provider and load the CSS data
        css_provider = Gtk.CssProvider()
        css_data = b"""
        #toy {
            background-color: rgba(0, 0, 0, 0);
        }
        """
        css_provider.load_from_data(css_data)
        Gtk.StyleContext.add_provider_for_screen(
            Gdk.Screen.get_default(),
            css_provider,
            Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
        )
