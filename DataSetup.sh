if [ ! -d "$2" ]; then
    mkdir $2
fi
./convert $1 $2
