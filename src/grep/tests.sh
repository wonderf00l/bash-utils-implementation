#!/bin/bash
#set -x
test_file1=../../data-samples/test.txt
test_file2=../../data-samples/text.txt
test_file3=../../data-samples/unicode-file
test_file4=../../data-samples/code.txt
test_file5=../../data-samples/mix.txt
test_file6=../../data-samples/random.txt
template_file=templates.txt
templates=("include" "main" "hello" "a" "her")

opt1="-e ^Lorem"
opt2="-e 1.10.33 -e tny -e STANDART"
opt3="-f templates.txt"
opt4="-f templates.txt -e 1.10.33 -e tny -e STANDART"

opts_list=("-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o" "-iv" "-in" "-ic" "-il" "-ih" "-io" "-vc" "-vl" "-vn" "-vh" "-vo" "-ch" "-lh" "-chs" "-lhs" "-ls" "-nih" "-ovi" "-ovih" "-ovihn" "-oh" "-oc" "-ol")
files_list=($test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6)

echo "GREP TEST"

for template in ${templates[@]}; do
    for test_file in ${files_list[@]}; do
        diff <(./s21_grep $template $test_file) <(grep $template $test_file) && echo "OK: '$opt' -- '$template' -- '$test_file' " || echo "FAIL: '$opt' -- '$template' -- '$test_file' "
    done
done

diff <(./s21_grep $opt1 $test_file4) <(grep $opt1 $test_file4) && echo "OK: '$opt1' -- '$test_file4' " || echo "FAIL: '$opt1' -- '$test_file4' "
diff <(./s21_grep $opt2 $test_file4) <(grep $opt2 $test_file4) && echo "OK: '$opt2' -- '$test_file4' " || echo "FAIL: '$opt2' -- '$test_file4' "
diff <(./s21_grep $opt3 $test_file4) <(grep $opt3 $test_file4) && echo "OK: '$opt3' -- '$test_file4' " || echo "FAIL: '$opt3' -- '$test_file4' "
diff <(./s21_grep $opt4 $test_file4) <(grep $opt4 $test_file4) && echo "OK: '$opt4 -- '$test_file4' " || echo "FAIL: '$opt4' -- '$test_file4' "

for template in ${templates[@]}; do
    for opt in ${opts_list[@]}; do 
        for test_file in ${files_list[@]}; do
            diff <(./s21_grep $opt $template $test_file) <(grep $opt $template $test_file) && echo "OK: '$opt' -- '$template' -- '$test_file' " || echo "FAIL: '$opt' -- '$template' -- '$test_file' "
        done
    done
done

for template in ${templates[@]}; do
    for opt in ${opts_list[@]}; do 
        diff <(./s21_grep $opt $template $test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6) <(grep $opt $template $test_file1 $test_file2 $test_file3 $test_file4 $test_file5 $test_file6) && echo "OK: '$opt' -- '$template'-- all files " || echo "FAIL: '$opt' -- '$template' -- all files "
    done
done

var="$(./s21_grep)"

if [[ $var == "Usage: [OPTION]... PATTERNS [FILE]..." ]]; then
    echo "OK WITH NO ARGS"
else
    echo "FAIL WITH NO ARGS"
fi

printf "\n\n\n"