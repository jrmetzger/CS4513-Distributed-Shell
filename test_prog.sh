#
# test.sh
# 
# Jonathan Metzger
# Spring 2018
#
# Project for CS4513 Distributed Computing Systems


total=0
# Connecting to Server 20 times with Correct Password
echo "> [ Connecting to Server 20 times with Correct Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 1234 -u user1 -w pass1 -c "ls"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Attempt: $i Accessing Server in $delta milliseconds"
	done
echo "20 Conections in $total milliseconds"
echo "Average Conections in $((total/20)) milliseconds"
echo "----------------------------------"

total=0
# Connecting to Server 20 times with Incorrect Password
echo "> [ Connecting to Server 20 times with Incorrect Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 1234 -u user1 -w pass2 -c "ls"

		sync
		final=$(date +%s%3N)
		delta=$((final-init))
		total=$((total + delta))
		echo
		echo "Attempt: $i Accessing Server in $delta milliseconds"
	done
echo "20 Conections in $total milliseconds"
echo "Average Conections in $((total/20)) milliseconds"
echo "----------------------------------"



# rm test*.txt throughputTest

# # Create files with size 1 Byteto 10 MB
# for i in {1..10}
# do
# 	head -c $((i*1000000)) /dev/urandom > test_documents/test$i.txt
# 	echo "Created test$i.txt with Filesize $i MB"
# done

# # Generate a 10MB file to test throughput.
# head -c $((10000000)) /dev/urandom > test_documents/throughputTest
# echo "Created throughputTest with Filesize 10 MB"


# total=0
# # Connecting to Server 20 times with Correct Password
# echo "> [ Connecting to Server 20 times with Correct Password ]"
# for i in {1..20}
# 	do
# 		sync
# 		init=$(date +%s%3N)
# 		./client -s localhost -p 1234 -u user1 -w pass1 -c "mv test_documents/test$i.txt > test_documents/test_copy$i.txt"

# 		sync
# 		final=$(date +%s%3N)
# 		delta=$((final-init))
# 		total=$((total + delta))
# 		echo
# 		echo "Moving test$i.txt in $delta milliseconds"
# 	done
# echo "20 Conections in $total milliseconds"
# echo "Average Conections in $((total/20)) milliseconds"
# echo "----------------------------------"

