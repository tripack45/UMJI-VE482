#! /bin/bash

program='../l4'

recho() { echo "\$ $@" ; $@ ; }

test() {
rm -f *_*.txt
for i in {1..30}; do
	echo "item$i=$RANDOM" >> rand_int.txt
	echo "item$i=$RANDOM.$RANDOM" >> rand_double.txt
	num=$(( 32+$RANDOM%(128-32) ))
	char=$(printf "\x$(printf %x $num)")
	echo "item$i=$char" >> rand_char.txt
done;

sort -t '=' -k 2 -n  rand_int.txt > ans_inc_int.txt
sort -t '=' -k 2 -n -r rand_int.txt > ans_dec_int.txt

sort -t '=' -k 2 -n rand_double.txt > ans_inc_double.txt
sort -t '=' -k 2 -n -r rand_double.txt > ans_dec_double.txt

LC_ALL=C sort -s -t '=' -k 2 rand_char.txt > ans_inc_char.txt
LC_ALL=C sort -s -t '=' -k 2 -r rand_char.txt > ans_dec_char.txt

$program rand_int.txt inc
$program rand_int.txt dec
$program rand_double.txt inc
$program rand_double.txt dec
$program rand_char.txt inc
$program rand_char.txt dec

recho 'diff ans_inc_int.txt inc_int.txt'
recho 'diff ans_dec_int.txt dec_int.txt'
recho 'diff ans_inc_double.txt inc_double.txt'
recho 'diff ans_dec_double.txt dec_double.txt'
recho 'diff ans_inc_char.txt inc_char.txt'
recho 'diff ans_dec_char.txt dec_char.txt'
}

for j in {1..100}; do
	echo "Test number $j"
	test
	sleep 0.1
	clear
done
