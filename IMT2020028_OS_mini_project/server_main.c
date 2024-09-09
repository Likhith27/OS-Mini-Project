#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <netinet/in.h> 
#include <string.h> 
#include <fcntl.h>
#include <pthread.h>
#include "constants.h"

int signup(int,char*,char*); 
int signin(int,char*,char*);
int deposit(char*,int);
int withdraw(char*,int);
int balance(char*);
int change_password(char*,char*);
char* get_details(char*);
int modify_user (char*,char*,char*);
int del_user(char*);
void *connection_handler(void *);

int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 

	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT); 
	
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	if(listen(server_fd, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
    printf("Server is ready to listen!!\n");
    while(true) {
        if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        pthread_t thread_id;
        if(pthread_create( &thread_id, NULL, connection_handler, (void*) &new_socket) < 0) {
            perror("could not create thread");
            return 1;
        }
        puts("Handler assigned");
    }
} 

void *connection_handler(void *socket_desc) {
	int sock = *(int*)socket_desc , option, deposit_amt, withdraw_amt, ret,balance_amt;
	char* username = malloc(BUF_SIZE*sizeof(char));
	char* password = malloc(BUF_SIZE*sizeof(char));
	char* type = malloc(BUF_SIZE*sizeof(char));
	char* new_username = malloc(BUF_SIZE*sizeof(char));
	char* return_message = malloc(BUF_SIZE*sizeof(char));
	char* option_string = malloc(BUF_SIZE*sizeof(char));
	char* amt_string = malloc(BUF_SIZE*sizeof(char));

	while(true) {
		printf("sock = %d\n",sock);
		read(sock, option_string, sizeof(option_string)); 
		option = atoi(option_string);
        printf("option = %d\n",option);
		switch(option){
			case SIGN_IN_AS_USER :
				read(sock, username, sizeof(username));
				read(sock, password, sizeof(password));
				ret = signin(option,username,password);
				printf("username: %s  password: %s\n",username, password);

				if(ret == -1)
					return_message = "sign in failed\n";
				else 
					return_message = "successfully signed in!\n";
				break;
			
			case SIGN_IN_AS_JOINT :
				read( sock , username, sizeof(username));
				read( sock , password, sizeof(password));
				ret = signin(option,username,password);
				if(ret == -1) 
					return_message = "sign in failed\n";
				else 
					return_message = "successfully signed in!\n";
				break;

			case SIGN_IN_AS_ADMIN :
				read( sock , username, sizeof(username));
				read( sock , password, sizeof(password));
				ret = signin(option,username,password);
				if(ret == -1) 
					return_message = "sign in failed\n";
				else 
					return_message = "successfully signed in!\n";
				break;

			case DEPOSIT :
				read(sock,amt_string,sizeof(amt_string));
				deposit_amt = atoi(amt_string);
				ret = deposit(username,deposit_amt);
				if (ret == 0) 
					return_message = "amount deposited\n";
		    	break;

			case WITHDRAW :
				read(sock,amt_string,sizeof(amt_string));
				withdraw_amt = atoi(amt_string);
				ret = withdraw(username,withdraw_amt);
				if(ret == -1) 
					return_message = "unable to withdraw\n";
				else 
					return_message = "withdrew successfully\n";
		    break ;

		    case  BALANCE :
				balance_amt = balance(username);
				sprintf(return_message,"%d",balance_amt);
		    	break ;

		    case PASSWORD :
				read(sock, password, sizeof(password));
				ret = change_password(username, password);
				if(ret == -1) 
					return_message = "unable to change password\n";
				else 
					return_message = "changed password successfully\n";
	        	break;

	        case DEL_USER : 
				read(sock, username, sizeof(username));
				ret = del_user(username);
				printf("unlink returned %d\n",ret);
				if(ret == -1) 
					return_message = "unable to delete user\n";
				else 
					return_message = "user deleted successfully\n";
		    	break;

		    case GET_USER_DETAILS : 
				read( sock , username, sizeof(username));
				printf("username = %s\n",username);
				return_message = get_details(username);
		    	break ;
		
		    case  MOD_USER :
				read(sock, username, sizeof(username));
				read(sock, new_username, sizeof(new_username));
				read(sock, password, sizeof(password));
				ret = modify_user(username, new_username,password);
				if(ret == -1) 
					return_message = "unable to change user\n";
				else 
					return_message = "changed user successfully\n";
				break;
		
		    case DETAILS : 
				return_message = get_details(username);
		    	break ;
		
		    case  ADD_USER : 
				read( sock , type, sizeof(type));
				printf("type = %s username = %s pwd = %s\n",type,username,password);
				if(!strcmp(type,"1")) {
					option = SIGN_UP_AS_USER;
					read(sock , username, sizeof(username));
					read(sock , password, sizeof(password));
				}
				else
					option = SIGN_UP_AS_JOINT;
				ret = signup(option,username,password);
				if(ret == -1) 
					return_message = "account could not be added\n";
				else 
					return_message = "successfully added account!\n";
		    	break ;
		}
		send(sock , return_message , BUF_SIZE * sizeof(char) , 0 ); 
	}
} 

int del_user(char* username){
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[5] = ".txt";
    strncat(filename,extension,sizeof(extension));
    int fd = open(filename,O_RDWR,0644);
    if(fd == -1){ perror("open");}
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl");}

    return unlink(filename);
}

int modify_user(char* username, char* new_username, char* password) {
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd,option;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1){
        perror("mod user"); return -1;
    }
    struct user u;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl");}
    // start of critical section
    lseek(fd,0,SEEK_SET);
    if(read(fd,&u,sizeof(struct user))==-1) { perror("read"); return -1; }
    del_user(username);
    if(strcmp(u.type,"normal")==0) option = SIGN_UP_AS_USER;
    else option = SIGN_UP_AS_JOINT;
    strcpy(u.username,new_username);
    signup(option,new_username,password);
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}

int signup(int option, char* username, char* password){
    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[4] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_WRONLY);
    if(fd != -1) return -1;
    else close(fd);
    fd = open(filename,O_WRONLY | O_CREAT,0644);
    if(fd == -1){
        perror("signup"); return -1;
    }
    int count = 0;
    struct user u;
    strcpy(u.username,username);
    strcpy(u.password,password);
    switch(option){
        case SIGN_UP_AS_USER :
            strcpy(u.type,"normal");
            break;
        case ADD_USER :
            strcpy(u.type,"normal");
            break;
        case SIGN_UP_AS_JOINT :
            strcpy(u.type,"joint");
            break;
    }
    write(fd,&u,sizeof(struct user));
    struct account acc;
    acc.balance = 0;
    write(fd,&acc,sizeof(struct account));
    close(fd);
    return 0;
}

int signin(int option, char* username, char* password){
    char filename[80];
    strcpy(filename,username);
    char extension[4] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDONLY,0644);
    if(fd == -1){
        perror("signin"); return -1;
    }
    struct user u;
    lseek(fd,0,SEEK_SET);
    read(fd,&u,sizeof(struct user));
    if((strcmp(u.password,password)!=0) 
        || (option==SIGN_IN_AS_USER && (strcmp(u.type,"normal")!=0))
        || (option==SIGN_IN_AS_ADMIN && (strcmp(u.type,"admin")!=0))
        || (option==SIGN_IN_AS_JOINT && (strcmp(u.type,"joint")!=0))) 
        return -1;
    close(fd);
    return 0;
}

int deposit(char* username, int amt){
    char filename[80];
    char extension[4] = ".txt";
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = sizeof(struct user);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct account);
    lock.l_pid = getpid();
    strcpy(filename,username);
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1){
        perror("Can't open"); return -1;
    }
    struct account acc;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl") ; return -1;}
    // start of critical section
    lseek(fd,sizeof(struct user),SEEK_SET);
    if(read(fd,&acc,sizeof(struct account))==-1) {perror("read"); return -1;}
    acc.balance += amt;
    lseek(fd,sizeof(struct user),SEEK_SET);
    if(write(fd,&acc,sizeof(struct account))==-1) {perror("write"); return -1;}
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}

int withdraw(char* username, int amt){
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = sizeof(struct user);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct account);
    lock.l_pid = getpid();

    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[4] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1){
        perror("signin"); return -1;
    }
    struct account acc;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl") ; return -1;}
    // start of critical section
    lseek(fd,sizeof(struct user),SEEK_SET);
    if(read(fd,&acc,sizeof(struct account)) == -1) {perror("read"); return -1;}
    printf("balance = %d\n",acc.balance);
    acc.balance -= amt;
    if(acc.balance < 0) return -1;
    lseek(fd,sizeof(struct user),SEEK_SET);
    if(write(fd,&acc,sizeof(struct account))==-1) {perror("write"); return -1;}
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}

int balance(char* username){
    static struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_start = sizeof(struct user);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct account);
    lock.l_pid = getpid();

    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDONLY,0644);
    if(fd == -1){
        perror("signin"); return -1;
    }
    struct account acc;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl") ; return -1;}
    // start of critical section
    lseek(fd,sizeof(struct user),SEEK_SET);
    if(read(fd,&acc,sizeof(struct account))==-1) perror("read");
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return acc.balance;
}

int change_password(char* username, char* pwd){
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct user);
    lock.l_pid = getpid();

    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1){
        perror("change pwd"); return -1;
    }
    struct user u;
    lseek(fd,0,SEEK_SET);
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl") ; return -1;}
    // start of critical section
    if(read(fd,&u,sizeof(struct user))==-1) { perror("read"); return -1; }
    strcpy(u.password,pwd);
    lseek(fd,0,SEEK_SET);
    if(write(fd,&u,sizeof(struct user))==-1) { perror("write"); return -1; }
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}

char* get_details(char* username){
    static struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    char filename[BUF_SIZE];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1){
        perror("open"); 
        return "user does not exist\n";
    }
    struct account acc;
    struct user u;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {perror("fcntl"); return "sorry, section is locked\n";}
    // start of critical section
    lseek(fd,0,SEEK_SET);
    if(read(fd,&u,sizeof(struct user))==-1)
        {perror("read"); return "unable to read file\n";}
    if(read(fd,&acc,sizeof(struct account))==-1)
        {perror("read"); return "unable to read file\n";}
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    char* return_string = (char*)malloc(BUF_SIZE * sizeof(char));
    sprintf(return_string,"username : %s \npassword : %s \ntype : %s\nbalance : %d\n",
        u.username,u.password,u.type,acc.balance);
    return return_string;
}