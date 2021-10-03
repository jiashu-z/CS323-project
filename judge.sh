#!/bin/bash


TEST_DIR="demo_test"
make_file() {
  able=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12")
  for i in "${able[@]}"; do
    echo "begin test test_1_r${i}:"
    bin/splc ./${TEST_DIR}/test_1_r${i}.spl > \
      ./${TEST_DIR}/test_1_r"${i}".test.out 2>&1
    diff ./${TEST_DIR}/test_1_r"${i}".test.out ./${TEST_DIR}/test_1_r"${i}".out \
      -y --suppress-common-lines
# when diff_rt is 0, there is no difference; when diff_rt is 1, there are some difference; 2 means diff run error.
    diff_rt=$?
    if [ "$diff_rt" == 0 ]; then
        echo "Test_1_r${i} passed!"
    else
        echo "test_1_r${i} failed. Difference are the above: [Actual|Expected]"
    fi
    echo "========================================================================="
    echo ""
  done
}
make_test_stdoutput() {
  able=("1" "2" "3" "4" "5")
  for i in "${able[@]}"; do
    echo "${i}"
    ./splc ./demo_test/test_SID_"${i}".spl > ./demo_test/test_SID_"${i}".demo_test.out 2>&1
  done
}
new_feature() {
    ./splc.out ./demo_test/test_SID_2.spl
    ./splc.out ./demo_test/test_1_r12.spl
}
make clean splc
make_file
#make_test_stdoutput
#new_feature
