project(
    'etap-toy',
    version : '1.0',
    license: ['GplV3']
)

subdir('po')

prefix = get_option('prefix')
appdir = prefix / 'share/etap'

glibdir = get_option('prefix')/'share/glib-2.0/schemas/'
icondir = get_option('prefix')/'share/icons/hicolor/scalable/apps'
desktopdir = get_option('prefix')/'share/applications'

install_data('data/tr.org.etap.toy.svg', install_dir : icondir)
install_data('data/tr.org.etap.toy.desktop', install_dir : desktopdir)
install_data('data/tr.org.etap.toy.gschema.xml', install_dir : glibdir)

install_subdir('toy/', install_dir: appdir)
install_data('toy.py', install_dir : bindir, rename: 'etap-toy')
