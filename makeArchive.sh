set -e # stop if one command fails

# create directory for the resulting archives
mkdir -p archives

DIR="prototype"
if [ ! -z "$1" ]
   then DIR=$1
fi

# create directory for the archive
mkdir $DIR
cd $DIR

# copy relevant files
cp ../CMakeLists.txt .
cp ../compile.sh .
cp ../plot.py .
cp -r ../src/ .
cp ../README.md .

# create needed directories
mkdir outputs

# return to the root
cd ..

# create archive
tar czf archives/$DIR.tar.gz $DIR

# remove temporary directory
rm -rf $DIR

echo "archives/$DIR.tar.gz created"
