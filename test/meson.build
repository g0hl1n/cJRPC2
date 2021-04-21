cmocka = dependency('cmocka', required : false)
if cmocka.found()
  test_common_src = [
    cjrpc2_src,
  ]
  test_common_inc = [
    cjrpc2_inc,
  ]
  test_common_dep = [
    cjrpc2_dep,
    cmocka,
  ]
  subdir('src')
else
  warning('test dependencies not found')
endif
