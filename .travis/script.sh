autoreconf -f -install
./configure --without-c++11 --with-system-boost --with-tests
make
sudo make install
ans=`uname`
if [ "$ans" != "Linux" ]
then
    echo "not Linux"
else
    echo "Linux"
    sudo ldconfig
fi
./tests_boost/unit-tests -l all
echo "ans is $ans"
