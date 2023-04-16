#!/usr/bin/env bash

remove_rubbish() {
  for fld in $(find -name "CMakeLists.txt" -printf '%h ')
    do
      for cmakefile in CMakeCache.txt cmake_install.cmake CMakeFiles Makefile
        do
          rm -rfv $fld/$cmakefile &>/dev/null
      done
  done
}

main() {
  if ! cmake -version &>/dev/null; then
    sudo apt-get install cmake
  fi
  if ! find "/usr/include/boost/version.hpp" &>/dev/null; then
    echo "Для сборки проекта необходимо установить библиотеку boost"
    sudo apt-get install libboost-all-dev
  fi
    cmake -S . -B ./build
    cd build
    make
    remove_rubbish
}



main "$@"
