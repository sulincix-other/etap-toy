project('example', 'cpp')
# Source files
src = [
    'src/main.cpp',
    'src/MainWindow.cpp'
]

# Qt5 dependencies
qt6_dep = [
    dependency('Qt6Core'),
    dependency('Qt6Gui'),
    dependency('Qt6Widgets'),
]

# -fPIC required
add_project_arguments('-fPIC', language: 'cpp')

# executable file
executable('main', src, dependencies: qt6_dep)
