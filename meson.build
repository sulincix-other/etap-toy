project('etap-toy', 'cpp', 'c')

icondir = get_option('prefix')/'share/icons/hicolor/scalable/apps'
desktopdir = get_option('prefix')/'share/applications'
libexecdir = get_option('prefix')/'libexec'

subdir('po')

# Generate ctx.cpp
run_command('bash', './tools/mkctx.sh', meson.current_build_dir(), check:true)

# Source files
src = [
    meson.current_build_dir() / 'ctx.c',
    'src/main.cpp',
    'src/MainWindow.cpp',
]

src += run_command('find', 'src/toy','-type','f' , '-iname', '*.c*' ,check:true).stdout().strip().split('\n')

# Extra warning as error
add_project_arguments('-Wall', '-Wextra', '-Werror', language: 'cpp')


# Qt6 dependencies
main_dep = [
    dependency('Qt6Core'),
    dependency('Qt6Gui'),
    dependency('Qt6Widgets'),
    dependency('pam'),
    dependency('pam_misc'),
]

uinput_dep = [
    dependency('libevdev')
]

cotp_dep = []
cotp = dependency('cotp', required:false)
if cotp.found()
    cotp_dep += cotp
    add_project_arguments('-DCOTP', language: 'cpp')
endif


rcc_args = ['rcc', './data/resources.qrc', '-o', meson.current_build_dir()/'resources.cpp']
run_command(rcc_args, check:true)
src += [meson.current_build_dir()/'resources.cpp']




# -fPIC required
add_project_arguments('-fpermissive', language: 'cpp')
add_project_arguments('-fPIC', language: 'cpp')

# executable file
executable('etap-toy', src, dependencies: main_dep, install: true)
executable('uinput-etap', ['src/uinput_service.c'], dependencies: uinput_dep,  install: true, install_dir: libexecdir)
if cotp.found()
    executable('etap-totp', ['src/otp-auth.c'], dependencies: cotp_dep,  install: true)
endif

# data file
install_data('data/tr.org.etap.toy.svg', install_dir : icondir)
install_data('data/uinput-etap.initd', install_dir : '/etc/init.d/', rename: 'uinput-etap', install_mode: 'rwxr-xr-x')
install_data('data/tr.org.etap.toy.desktop', install_dir : desktopdir)

