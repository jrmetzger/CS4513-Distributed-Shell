# Project 2: Distributed Shell
Jon Metzger
April 5th 2018
CS4513 Distrubuted Systems
Hugh C. Lauer

## RUN

### TERMINAL 1

```
$ ./server -h

```
******** Distributed Shell Server ********
Jonathan Metzger ~ April 5th 2018 ~ CS4513
usage: ./server [flags]
  -h usage message
  -p (optional) port: default is 4513
******************************************

```
$ ./server -p 1234
user is accessing information!
user entered password incorrectly.
```

### TERMINAL 2

```
$ ./client -h
```
******** Distributed Shell Client ********
Jonathan Metzger ~ April 5th 2018 ~ CS4513
usage: ./client [flags] {-c command}
  -h usage message
  -s server
  -p (optional) port: default is 4513
  -u username
  -c 'command'
******************************************

CORRECT CREDENTIALS
```
$ ./client -s localhost -p 1234 -u user -c "ls"
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





