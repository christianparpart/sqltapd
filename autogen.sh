#! /bin/bash

if [[ -z "$CXX" ]]; then
  if which clang++ &>/dev/null; then
    export CXX=$(which clang++)
  else
    export CXX=$(which g++)
  fi
  if which clang &>/dev/null; then
    export CC=$(which clang)
  else
    export CC=$(which cc)
  fi
fi

if [[ "$1" == "clean" ]]; then
  rm -vf {configure,build,install}-stamp
  rm -vf debian/*.log
  rm -vf debian/*.substvars
  rm -vf debian/*.debhelper
  rm -rvf debian/{files,tmp,sqltapd}
  rm -rf doc/html
  rm -vf install_manifest.txt

  rm -vf "include/xzero/sysconfig.h" \
           "tests/libxzero-test"

  find . \( -name 'CMakeCache.txt' -o -name 'CMakeFiles' \
      -o -name 'Makefile' -o -name cmake_install.cmake \
      -o -name '*.so' \
      -o -name '*.a' \
      -o -name '*.so.*' \
      -o -name 'vgcore.*' -o -name core \
      -o -name '.directory' \
      -o -name 'Doxyfile' \
      -o -name 'sqltapd.pc' \
      \) \
    -exec rm -rf {} \; 2>/dev/null
else
  cmake "$(dirname $0)" \
    -DCMAKE_BUILD_TYPE="debug" \
    -DCMAKE_CXX_FLAGS_DEBUG="-O0 -g3" \
    -DCMAKE_INSTALL_PREFIX="${HOME}/local" \
    -DCMAKE_VERBOSE_MAKEFILE=OFF
fi
