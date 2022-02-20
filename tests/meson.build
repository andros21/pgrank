# tests/meson.build
# =================

# catch2: search dependency, otherwise use cmake subproject
catch2_dep  = dependency('catch2', version: ['>=2.x', '<3.x'], required : false)
if not catch2_dep.found()
  catch2_sub = cmake.subproject('catch2')
  catch2_dep = catch2_sub.get_variable('Catch2_dep')
endif

# pgrank_test: binary
pgrank_test = executable('pgrank_test',
                         sources : ['pgrank_test.cpp'],
                         dependencies : [catch2_dep, pagerank_dep])
# meson unit test
test('json_input',  pgrank_test, args : ['[json_input]'],  priority :  0, is_parallel : false)
test('json_struct', pgrank_test, args : ['[json_struct]'], priority : -1, is_parallel : false)
test('json2mat',    pgrank_test, args : ['[json2mat]'],    priority : -2, is_parallel : false)
test('pagerank',    pgrank_test, args : ['[pagerank]'],    priority : -3, is_parallel : false)
test('pgrank0',     pgrank,      args : ['../tests/input/caibg.fake.json', '/tmp/caibg.fake.csv'],
                                                           priority : -4, is_parallel : false)
test('pgrank1',     pgrank,      args : ['../tests/input/caibg.fake.json', '-q'],
                                                           should_fail : true,
                                                           priority : -5, is_parallel : false)
test('pgrank2',     pgrank,      args : ['../tests/input/an_invalid_file', '/tmp/caibg.fake.csv', '-q'],
                                                           should_fail : true,
                                                           priority : -6, is_parallel : false)
test('pgrank3',     pgrank,      args : ['../tests/input/caibg.fake.json',
                                         './an_invalid_path/caibg.fake.csv', '-q'],
                                                           should_fail : true,
                                                           priority : -7, is_parallel : false)
