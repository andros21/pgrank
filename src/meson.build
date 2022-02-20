# src/meson.build
# ===============

# argparse: search dependency, otherwise use cmake subproject
argparse_dep  = dependency('argparse', required : false)
if not argparse_dep.found()
  argparse_sub = cmake.subproject('argparse')
  argparse_dep = argparse_sub.get_variable('argparse_dep')
endif
armadillo_dep = dependency('armadillo', version : '>=9.x')
openblas_dep  = dependency('openblas')
rapidjson_dep = dependency('RapidJSON', version : '>=1.1.0')

# json2mat: shared library
json2mat = library('json2mat',
                   sources : ['json2mat/json2mat.cpp'],
                   dependencies : [armadillo_dep, openblas_dep, rapidjson_dep],
                   include_directories : include_directories('../include/json2mat/'),
                   install : true)

# json2mat: pc file
pkg.generate(json2mat)

# json2mat: dependency object
json2mat_dep = declare_dependency(dependencies : [armadillo_dep, openblas_dep, rapidjson_dep],
                                  include_directories : include_directories(['../include/json2mat/',
                                                                             '../include/pagerank/']),
                                  link_with : json2mat)

# pagerank: shared library
pagerank = library('pagerank',
                   sources : ['pagerank/pagerank.cpp'],
                   dependencies : json2mat_dep,
                   install : true)

# pagerank: pc file
pkg.generate(pagerank)

# pagerank: dependency object
pagerank_dep = declare_dependency(dependencies: json2mat_dep,
                                  link_with : pagerank)

# pgrank: binary
pgrank = executable('pgrank',
                    sources : ['pgrank.cpp'],
                    dependencies : [argparse_dep, pagerank_dep],
                    install : true)
