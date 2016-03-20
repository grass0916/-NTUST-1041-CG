# Move to the project root.
BASEDIR=$(dirname "$0")
cd $BASEDIR
# Start qmake and make.
rm -rf debug
mkdir -p debug
qmake NTUST-CG-Pro1.pro -o ./debug/
cd ./debug/
make
open -W ./NTUST-CG-Pro1.app