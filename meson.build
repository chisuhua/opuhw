project(
  'opuhw',
  ['c', 'cpp'],
  version : run_command(
    [find_program('python', 'python3'), 'bin/meson_get_version.py']
  ).stdout(),
  license : 'MIT',
  meson_version : '>= 0.52',
  default_options : ['buildtype=debug'] #, 'b_ndebug=if-release', 'c_std=c99', 'cpp_std=c++17']
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
buildtype = get_option('buildtype')
builddir = meson.current_build_dir()

null_dep = dependency('', required : false)

# Arguments for the preprocessor, put these in a separate array from the C and
# C++ (cpp in meson terminology) arguments since they need to be added to the
# default arguments for both C and C++.
pre_args = [
  '-D__STDC_CONSTANT_MACROS',
  '-D__STDC_FORMAT_MACROS',
  '-D__STDC_LIMIT_MACROS',
  '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
  '-DPACKAGE_BUGREPORT="https://gitlab.freedesktop.org/mesa/mesa/-/issues"',
]

extra_cpp_args = ['-Wall', '-std=c++17']

if buildtype == 'release'
  extra_cpp_args += ['-O3', '-g', '-Wno-strict-aliasing', '-msse4.1', '-flto']
else
  extra_cpp_args += ['-O0', '-g', '-Wno-strict-aliasing', '-msse4.1']
endif

extra_c_args = extra_cpp_args

#if cpp.get_id() == 'gcc'
#  extra_cpp_args += ['-std=c++17']
#else
#  extra_cpp_args += []
#endif

link_args = ['-ldl', '-lpthread', '-L' + meson.current_build_dir(), '-lstdc++' ]

symbol_list = meson.current_source_dir() + '/symbol_export.def'

DESIGN_ROOT = run_command([find_program('python', 'python3'), '-c', 'import os; print(os.environ["DESIGN_ROOT"])']).stdout().strip()
GEM5_ROOT = run_command([find_program('python', 'python3'), '-c', 'import os; print(os.environ["GEM5_ROOT"])']).stdout().strip()
CUDA_INSTALL_PATH = run_command([find_program('python', 'python3'), '-c', 'import os; print(os.environ["CUDA_INSTALL_PATH"])']).stdout().strip()

message('DESIGN_ROOT is ' + DESIGN_ROOT)
message('GEM5_ROOT is ' + GEM5_ROOT)
message('CUDA_INSTALL_PATH is ' + CUDA_INSTALL_PATH)

test_link_args = link_args + ['-L' + DESIGN_ROOT + '/cosim', '-lcash' ] + ['-Wl,--version-script='+symbol_list]
#isa_link_args += ['-L' + DESIGN_ROOT + '/opu/3rdparty/cash/build/lib', '-lcash']
test_inc = [
  DESIGN_ROOT + '/opu/3rdparty/cash/include',
  DESIGN_ROOT + '/opu/3rdparty/cash/soft-ieee754/includes',
  ]

builddir = meson.current_source_dir() + '/../3rdparty/cash/'

test_src = [
  'tests/common.cpp',
  'tests/arbiter.cpp',
  'tests/counter.cpp',
  'tests/regbank.cpp',
  'tests/main.cpp'
  ]


#test_simple_src = [
#  'test/main.cpp',
#  'test/module/Simple.cpp'
#  ]

testsuite = executable(
  'testsuite',
  test_src,
  include_directories : include_directories(test_inc),
  install : false,
  cpp_args : extra_cpp_args,
  link_args : test_link_args,
  #dependencies: declare_dependency(link_with : )
)

counter = executable(
  'counter',
  'examples/counter.cpp',
  include_directories : include_directories(test_inc),
  install : false,
  cpp_args : extra_cpp_args,
  link_args : test_link_args,
  #dependencies: declare_dependency(link_with : )
)

