#!/bin/bash
# set -x
test_file1=../../data-samples/test.txt
test_file2=../../data-samples/text.txt
test_file3=../../data-samples/unicode-file
test_file4=../../data-samples/code.txt
test_file5=../../data-samples/mix.txt
test_file6=../../data-samples/random.txt

opts_list=("-b" "--number-nonblank" "-e" "-E" "-n" "--number" "-s" "--squeeze-blank" "-t" "-T" "-v")
files_list=($test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6)

echo "CAT TEST"

for test_file in ${files_list[@]}; do
    diff <(./s21_cat $test_file) <(cat $test_file) && echo "OK: '$opt' -- '$test_file' " || echo "FAIL: '$opt' -- '$test_file' "
done

for opt in ${opts_list[@]}; do 
    for test_file in ${files_list[@]}; do
        diff <(./s21_cat $opt $test_file) <(cat $opt $test_file) && echo "OK: '$opt' -- '$test_file' " || echo "FAIL: '$opt' -- '$test_file' "
    done
done

for opt in ${opts_list[@]}; do 
    diff <(./s21_cat $opt $test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6) <(cat $opt $test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6) && echo "OK: '$opt' -- all files " || echo "FAIL: '$opt' -- all files "
done

printf "\n\n\n"
