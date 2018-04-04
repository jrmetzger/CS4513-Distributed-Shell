#
# test_connect.sh
# 
# Jonathan Metzger
# Spring 2018
#
# Project for CS4513 Distributed Computing Systems


total=0
# Connecting to Server 20 times with Correct Password
echo "> [ Connecting to Local Server 20 times with Correct Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 4824 -u user1 -w pass1 -c "ls"

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
echo "> [ Connecting to Local Server 20 times with Incorrect Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s localhost -p 4824 -u user1 -w pass2 -c "ls"

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
# Connecting to Server 20 times with Correct Password
echo "> [ Connecting to AWS Server 20 times with Correct Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s 18.222.16.160 -p 4824 -u user1 -w pass1 -c "ls"

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
echo "> [ Connecting to AWS Server 20 times with Incorrect Password ]"
for i in {1..20}
	do
		sync
		init=$(date +%s%3N)
		./client -s 18.222.16.160 -p 4824 -u user1 -w pass2 -c "ls"

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

