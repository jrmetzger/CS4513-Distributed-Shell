#
# test_mv.sh
# 
# Jonathan Metzger
# Spring 2018
#
# Project for CS4513 Distributed Computing Systems

# LOCAL

rm test_documents/test*.txt

total=0
# Create files with size 1 Byte to 10 MB
echo "> [ Create files with size 1 Byte to 10 MB on Local Server ]"
for i in {1..10}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 1234 -u user1 -w pass1 -c "head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Created test$i.txt with Filesize $i MB in $delta milliseconds"
	done
echo "10 Conections in $total milliseconds"
echo "Average Conections in $((total/10)) milliseconds"
echo "----------------------------------"

total=0
# Create files with size 1 Byte to 10 MB
echo "> [ Create files with size 1 Byte to 10 MB on Local Server ]"
for i in {1..10}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 1234 -u user1 -w pass2 -c "head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Created test$i.txt with Filesize $i MB in $delta milliseconds"
	done
echo "10 Conections in $total milliseconds"
echo "Average Conections in $((total/10)) milliseconds"
echo "----------------------------------"

# AWS

total=0
# Create files with size 1 Byte to 10 MB
echo "> [ Create files with size 1 Byte to 10 MB on AWS Server]"
for i in {1..10}
	do
		sync
		init=$(date +%s%3N)
		./client -s 18.222.16.160 -p 1234 -u user1 -w pass1 -c "head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Created test$i.txt with Filesize $i MB in $delta milliseconds"
	done
echo "10 Conections in $total milliseconds"
echo "Average Conections in $((total/10)) milliseconds"
echo "----------------------------------"

total=0
# Create files with size 1 Byte to 10 MB
echo "> [ Create files with size 1 Byte to 10 MB on AWS Server]"
for i in {1..10}
	do
		sync
		init=$(date +%s%3N)
		./client -s 18.222.16.160 -p 1234 -u user1 -w pass2 -c "head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Created test$i.txt with Filesize $i MB in $delta milliseconds"
	done
echo "10 Conections in $total milliseconds"
echo "Average Conections in $((total/10)) milliseconds"
echo "----------------------------------"