project('ic-server', 'cpp', version : '0.1')

compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'msvc'
  add_project_arguments(['/std:c++latest', '-D_WIN32_WINNT=0x0A00'],
                        language : 'cpp')
else
  add_project_arguments('-std=c++2a', language : 'cpp')
endif

boost = dependency('boost')
gtest = dependency('gtest')
# gtest_main = dependency('gtest_main')
log = dependency('spdlog')

ic_server = library('ic-server',
  [
      'config.hpp',
      'log.hpp',            'log.cpp',
      'server.hpp',         'server.cpp',
      'connection.hpp',     'connection.cpp',
      'level.hpp',          'level.cpp',
      'chunk.hpp',          'chunk.cpp',
      'blocks.hpp',         'blocks.cpp',
      'utils.hpp',          'utils.cpp',
  ],
  dependencies: [boost, log],
  include_directories: ['third/'],
  cpp_args: ['-DIC_SERVER_BUILDING_THE_LIB', '-DBOOST_ALL_DYN_LINK'],
)

#-------------------------------------------------------------------------------

executable('ic-server',
    [
        'main_server.cpp'
    ],
    link_with: [ic_server],
    dependencies: [boost, log],
    include_directories: ['third']
)

#-------------------------------------------------------------------------------

tests_level = executable('tests_level',
  ['tests_level.cpp'],
  link_with : [ic_server],
  dependencies : [boost, gtest, log],
  include_directories: ['.', 'third'],
  cpp_args: ['-DBOOST_ALL_DYN_LINK']
)

test('tests_level', tests_level)
