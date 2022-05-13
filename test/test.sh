if [ $# != 1 ]; then
    echo "usage: test.sh <test_name>"
    exit 1
fi

../bin/crpp $1.cr $1.cri
../bin/crg $1.cri $1.crs
../bin/crasm $1.crs $1.cro
diff <(../bin/crvm $1.cro < $1.in) $1.out
