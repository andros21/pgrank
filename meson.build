# meson.build
# ===========

project('pgrank',
        'cpp',
        default_options : ['buildtype=debugoptimized',
                           'warning_level=3',
                           'wrap_mode=nofallback',
                           'cpp_std=c++2a'],
        version : '0.1.4')

# import cmake
cmake = import('cmake')
# import pkgconfig
pkg = import('pkgconfig')

# import subdirs
subdir('src/')
if get_option('test')
   subdir('tests/')
endif
