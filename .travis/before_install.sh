ans=`uname`
if [ "$ans" != "Linux" ]
then
    echo "not Linux"
    travis_retry brew update
    travis_retry brew install boost
    travis_retry brew install libcryptopp
else
    echo "Linux"
    travis_retry sudo apt-get update -qq
    travis_retry sudo apt-get install -qq autotools-dev
    travis-retry sudo apt-get install -qq libboost-all-dev
    travis-retry sudo apt-get install -qq libcrypto++-dev
    travis-retry sudo apt-get install -qq libsqlite3-dev
fi
echo "ans is $ans"
