echo "--- params ---"
echo "--- branch: $1 ---"
echo "--- bin src path: $2 ---"
echo "--- cp dst: $3 ---"

echo "--- clone ---"
git clone --recurse-submodules -b $1 --single-branch https://github.com/Alexander-Degtyar/OpenGreenery.git
rv=$?
if [ $rv -ne 0 ]; then
	exit $rv
fi
echo "--- create build dir ---"
mkdir -p OpenGreenery/build
cd OpenGreenery/build

echo "--- generate cmake ---"
cmake ..
rv=$?
if [ $rv -ne 0 ]; then
	exit $rv
fi
echo "--- build ---"
make -s
rv=$?
if [ $rv -ne 0 ]; then
	exit $rv
fi
echo "--- --- build result: $? --- ---"

echo "--- copy ---"
echo "--- --- path: $2 --- ---"
echo "--- --- ls src --- ---"
ls -l $2
if [ $rv -ne 0 ]; then
	exit $rv
fi
echo "--- --- --- ls result: $? --- --- ---"

echo "--- --- cp --- ---"
cp $2 $3
rv=$?
if [ $rv -ne 0 ]; then
	exit $rv
fi
echo "--- --- --- cp result: $? --- --- ---"

echo "--- --- ls dst --- ---"
ls $3
echo "--- --- --- ls result: $? --- --- ---"
