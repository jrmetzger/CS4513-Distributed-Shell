#
# test_mv.sh
# 
# Jonathan Metzger
# Spring 2018
#
# Project for CS4513 Distributed Computing Systems

rm test_documents/test*.txt

# Create files with size 1 Byte to 10 MB
for i in {1..20}
do
	head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt
	echo "Created test$i.txt with Filesize $i MB"
done


total=0
# Moving 20 files over the local server
echo "> [ Moving 20 files over the local server ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 4824 -u user1 -w pass1 -c "mv test_documents/test$i.txt test_documents/test_copy$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Moving test$i.txt in $delta milliseconds"
	done
echo "20 Conections in $total milliseconds"
echo "Average Conections in $((total/20)) milliseconds"
echo "----------------------------------"

rm test_documents/test*.txt

# Create files with size 1 Byte to 10 MB
for i in {1..20}
do
	head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt
	echo "Created test$i.txt with Filesize $i MB"
done


total=0
# Moving 20 files over the local server
echo "> [ Moving 20 files over the AWS server ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s 18.222.16.160 -p 4824 -u user1 -w pass1 -c "mv test_documents/test$i.txt test_documents/test_copy$i.txt"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Moving test$i.txt in $delta milliseconds"
	done
echo "20 Conections in $total milliseconds"
echo "Average Conections in $((total/20)) milliseconds"
echo "----------------------------------"

