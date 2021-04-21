project('cjrpc2', 'c',
  license: 'MIT',
  version: '0.0.1',
  default_options: [
    'c_std=c89',
    'warning_level=3',
  ],
)

cjrpc2_inc = include_directories(
  [
    '.',
    'include',
  ]
)

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

cjrpc2 = static_library('cjrpc2',
  [
    'src/cJRPC2.c',
    'cJSON/cJSON.c',
  ],
  include_directories : cjrpc2_inc,
)
cjrpc2_dep = declare_dependency(
  link_with: cjrpc2,
  dependencies: [
    m_dep,
  ],
  include_directories : cjrpc2_inc,
)

# examples
executable('example-minimal',
  'examples/minimal.c',
  dependencies: [
    cjrpc2_dep,
  ],
)

executable('example-multiple',
  'examples/multiple.c',
  dependencies: [
    cjrpc2_dep,
  ],
)

executable('example-calc',
  'examples/calc.c',
  dependencies: [
    cjrpc2_dep,
  ],
)
