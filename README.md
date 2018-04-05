# Project 2: Distributed Shell
Jon Metzger
April 4th 2018
CS4513 Distrubuted Systems
Hugh C. Lauer

## RUN

```
$ make
$ make clean
$ make
```

### TERMINAL 1: Server

#### Help Usage Message

```
$ ./server -h

******** Distributed Shell Server ********
Jonathan Metzger ~ April 5th 2018 ~ CS4513
usage: ./server [flags]
  -h usage message
  -p (optional) port: default is 4513
******************************************
```


#### Run the Server

```
$ ./server -p 1234
user is accessing information!
user entered password incorrectly.
```

### TERMINAL 2: Client

#### Help Usage Message

```
$ ./client -h

******** Distributed Shell Client ********
Jonathan Metzger ~ April 5th 2018 ~ CS4513
usage: ./client [flags] {-c command}
  -h usage message
  -s server
  -p (optional) port: default is 4513
  -u username
  -w password
  -c 'command'
******************************************
```

#### Run the Client 

CORRECT CREDENTIALS

```
$ ./client -s localhost -p 1234 -u user -w password -c "ls"
Username: user
Password: 

 ** Password is correct. Accessing information. **
```

INCORRECT CREDENTIALS

```
$ ./client -s localhost -p 1234 -u user -c "ls"
Username: user
Password: 

** ERROR: Credentials don't match. Exiting... **
```


## AWS

### SSH to Server

```
$ ssh -i CS4513.pem ubuntu@ec2-18-222-16-160.us-east-2.compute.amazonaws.com
```

OR 

```
$ ssh -i CS4513.pem ubuntu@18.222.16.160
```

IP ADDRESS is 18.222.16.160

### Copy File to Server

```
$ sudo scp -i "CS4513.pem" CS4513-Distributed-Shell.zip ubuntu@ec2-18-222-16-160.us-east-2.compute.amazonaws.com:/home/ubuntu
```

### Connect

#### TERMINAL 1: SERVER

```
ubuntu@ip-172-31-38-241:~$ ./server -p 1234
```

#### TERMINAL 2: CLIENT

```
$ ./client -s 18.222.16.160 -p 1234 -u user -c "ls"
```

