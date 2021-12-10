#!/bin/bash



TEST_DIR_OWN="test"
TEST_DIR_EX="test-ex"
demo_test() {
  TEST_DIR="project2_test/demo_test"
  PREFIX="test_2_r"
  able=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12" "13" "14" "15")
  for i in "${able[@]}"; do
    echo "begin test ${PREFIX}${i}:"
    bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".test.out 2>&1
    diff ./${TEST_DIR}/${PREFIX}"${i}".test.out ./${TEST_DIR}/${PREFIX}"${i}".out -y --suppress-common-lines --ignore-blank-lines
# when diff_rt is 0, there is no difference; when diff_rt is 1, there are some difference; 2 means diff run error.
    diff_rt=$?
    if [ $diff_rt == 0 ]; then
        echo "${PREFIX}${i} passed!"
    else
        echo "${PREFIX}${i} have difference. Difference are the above: [Actual|Expected]"
    fi
    echo "========================================================================="
  done
}
own_test() {
  TEST_DIR="project2_test/test"
  PREFIX="test_11811620_"
  able=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10")
  for i in "${able[@]}"; do
    echo "begin test ${PREFIX}${i}:"
   # echo "command: bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".test.out 2>&1"
    bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".test.out 2>&1
    diff ./${TEST_DIR}/${PREFIX}"${i}".test.out ./${TEST_DIR}/${PREFIX}"${i}".out -y --suppress-common-lines --ignore-blank-lines
# when diff_rt is 0, there is no difference; when diff_rt is 1, there are some difference; 2 means diff run error.
    diff_rt=$?
    if [ $diff_rt == 0 ]; then
        echo "${PREFIX}${i} passed!"
    else
        echo "${PREFIX}${i} have difference. Difference are the above: [Actual|Expected]"
    fi
    echo "========================================================================="
  done
}
ex_test() {
  TEST_DIR="project2_test/test-ex"
  PREFIX="test_"
  able=("1" "2" "3" "4" "5" "6" "7" "8" "9" "10")
  for i in "${able[@]}"; do
    echo "begin test ${PREFIX}${i}:"
    bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".test.out 2>&1
    diff ./${TEST_DIR}/${PREFIX}"${i}".test.out ./${TEST_DIR}/${PREFIX}"${i}".out -y --suppress-common-lines --ignore-blank-lines
# when diff_rt is 0, there is no difference; when diff_rt is 1, there are some difference; 2 means diff run error.
    diff_rt=$?
    if [ $diff_rt == 0 ]; then
        echo "${PREFIX}${i} passed!"
    else
        echo "${PREFIX}${i} have difference. Difference are the above: [Actual|Expected]"
    fi
    echo "========================================================================="
  done

}
other_test(){
      TEST_DIR="project1_test/test"
      PREFIX="test_11811620_"
      able=("1" "2" "3" "4" "5")
      for i in "${able[@]}"; do
        echo "begin test ${PREFIX}${i}:"
        bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".test.out 2>&1
        diff ./${TEST_DIR}/${PREFIX}"${i}".test.out ./${TEST_DIR}/${PREFIX}"${i}".out -y --suppress-common-lines --ignore-blank-lines
    # when diff_rt is 0, there is no difference; when diff_rt is 1, there are some difference; 2 means diff run error.
        diff_rt=$?
        if [ $diff_rt == 0 ]; then
            echo "${PREFIX}${i} passed!"
        else
            echo "${PREFIX}${i} have difference. Difference are the above: [Actual|Expected]"
        fi
        echo "========================================================================="
      done
}

output_ir() {
  TEST_DIR="project3_test/test"
  PREFIX="test_3_r"
  able=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11")
  for i in "${able[@]}"; do
    echo "begin output ${PREFIX}${i}:"
    bin/splc ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".ir 2>&1
    echo "========================================================================="
  done
}

output_compare() {
  TEST_DIR="project3_test/test"
  PREFIX="test_3_r"
  able=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11")
  for i in "${able[@]}"; do
    echo "begin compare output ${PREFIX}${i}:"
    bin/compare ./${TEST_DIR}/${PREFIX}${i}.spl > ./${TEST_DIR}/${PREFIX}"${i}".ir.compare 2>&1
    echo "========================================================================="
  done
}
make clean splc
output_compare
output_ir
#echo "==================demo_test begin"
#demo_test
#echo "==================own_test begin"
#own_test
#echo "==================ex_test begin"
#ex_test
#other_test
#make_test_stdoutput
#new_feature
