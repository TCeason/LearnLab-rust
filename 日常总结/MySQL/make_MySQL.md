编译安装 MySQL
---

* 编译 release 版本
```perl
git clone https://github.com/TCeason/tokudb.git mysql
cd mysql
git checkout releases-tags
git submodule init
git submodule update
mkdir build; cd build
cmake ..\
  -DCMAKE_BUILD_TYPE=RelWithDebInfo\
  -DBUILD_CONFIG=mysql_release\
  -DFEATURE_SET=community\
  -DWITH_EMBEDDED_SERVER=OFF\
  -DTOKUDB_VERSION=7.5.6\
  -DBUILD_TESTING=OFF\
  -DWITHOUT_ROCKSDB=ON\
  -DWITH_BOOST=../extra/boost/boost_1_59_0.tar.gz\
  -DCOMPILATION_COMMENT="Study MySQL build $(date +%Y%m%d.%H%M%S.$(git rev-parse --short HEAD))"\
  -DCMAKE_INSTALL_PREFIX=  # MySQL basedir 路径  
  # cmake 的时候，如果没有依赖项需要自行安装
make -jn # n的数量取决于 CPU Core 数，如果 make -j报错，尝试 make
make install 

```
* 编译 Debug 版本

```perl
git clone https://github.com/TCeason/tokudb.git mysql
cd mysql
git checkout releases-tags
git submodule init
git submodule update
mkdir build
cd build
cmake ..\
  -DCMAKE_BUILD_TYPE=Debug\
  -DBUILD_CONFIG=mysql_release\
  -DFEATURE_SET=community\
  -DWITH_EMBEDDED_SERVER=OFF\
  -DTOKUDB_VERSION=7.5.6\
  -DBUILD_TESTING=OFF\
  -DWITHOUT_ROCKSDB=ON\
  -DWITH_BOOST=../extra/boost/boost_1_59_0.tar.gz\
  -DCMAKE_INSTALL_PREFIX=  # MySQL basedir 路径
make -jn # n的数量取决于 CPU Core 数，如果 make -j报错，尝试 make
make install

```
