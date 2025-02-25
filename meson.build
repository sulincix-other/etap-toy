project('example', 'cpp', 'c')
# Source files
src = [
    'src/main.cpp',
    'src/MainWindow.cpp',
    'src/toy/RightClick.cpp',
    'src/uinput_client.c'
]

# Qt5 dependencies
qt6_dep = [
    dependency('Qt6Core'),
    dependency('Qt6Gui'),
    dependency('Qt6Widgets'),
]

uinput_dep = [
    dependency('libevdev')
]

rcc_args = ['rcc', './data/resources.qrc', '-o', meson.current_build_dir()/'resources.cpp']
run_command(rcc_args, check:true)
src += [meson.current_build_dir()/'resources.cpp']


# -fPIC required
add_project_arguments('-fPIC', language: 'cpp')

# executable file
executable('main', src, dependencies: qt6_dep)
executable('uinput', ['src/uinput_service.c'], dependencies: uinput_dep)
