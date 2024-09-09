# OS-mini-project---Banking-system

# Overview

This mini project aims to model a banking system which supports three types of users - normal, joint and admin. Users can use basic functionalities of the banking system, which include authentication, depost and withdrawal. A concurrent server is used to facilitate multiple clients at once. In order to demonstrate file locking, we show that multiple users holding a joint account can query the server simultaneously. 

# Functionality Provided

The commands which a user can execute are divided based on the user as follows.

## User Commands (Normal and Joint)

### Sign Up 
Different options are provided to sign up as a normal and joint user. Username and password must be provided. One can't sign up if the username already exists. On sign up, your balance is initialized to 0.
### Sign In 
Username and passowrd should be provided while signing in. One must specify what kind of user they are signing in as.
### Deposit
Deposit money into your account.
### Withdraw
Withdraw money from your account.
### Check Balance
Used to check how much money your account holds.
### Change Password
Used to change password to login.
### View Details
Will return username, password, type of user and balance.
### Exit
Used to terminate the program.

## Admin Commands

### Add User
Used to create a user of type normal or joint.
### Delete User
Used to delete user given username
### Modify User
Used to modify username and password given the old username.
### Get User Details
Used to get password and balance given the username.

# File Structure
### constants.h
Holds constant values for every user and admin commands. These are used throughout for the sake of readability. Also contains constants to indicate which option form the menu is chosen. The structs used to define user and account are also sepcified here.

### client.c
Program for client. This can be thought of as the "front end", and contains code for the user and admin interface. Messages are sent to the server via a socket.

### server.c
Program for a concurrent server. This can be thought of as the "back end". For every client, the server creates a new thread in order to service it further. This helps ensure smooth querying of server from various clients independetn of each other. 

### admin_commands.c
Contains functions for commands exclusively used by admin. These include commands to delete and modify a user.

### user_commands.c
Contains functions for user commands. These implement file locking for the case of joint user.

# Overview of Working
The server creates a new file for every user who signs up. The name of this file is username.txt. If a file with a given username already exists, then that user cannot sign up. Every file contains first a struct which has user details - username, password and type - followed by a struct which has account details - the balance amount. All queries are answered by opening the corresponding file, and reading from it. In order to delete a user, the corresponding file is deleted.  

# Commands to compile and run

## Compilation
To compile for server : make server <br>
To compile for client : make client

## Execution
To execute server : ./server <br>
To execute client : ./client

# Commands to clean
make clean