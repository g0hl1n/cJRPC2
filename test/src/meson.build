test_get_param_double = executable('test-get-param-double',
  [
    'test-get-param-double.c',
    test_common_src,
  ],
  include_directories: [
    test_common_inc,
  ],
  dependencies: [
    test_common_dep,
  ],
)
test('get-param-double', test_get_param_double, is_parallel: true)
