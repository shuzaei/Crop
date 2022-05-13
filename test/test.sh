if [ $# != 1 ]; then
    echo "usage: test.sh <test_name>"
    exit 1
fi

./bin/crpp test/$1.cr test/$1.crp
./bin/crc test/$1.crp test/$1.crs
./bin/crasm test/$1.crs test/$1.cro
diff <(./bin/crvm test/$1.cro < test/$1.in) test/$1.out
