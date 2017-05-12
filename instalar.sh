rm -CMakeCache.txt
cmake -DJANSSON_BUILD_SHARED_LIBS=1 -DCMAKE_INSTALL_PREFIX:PATH=/usr . && make all install
