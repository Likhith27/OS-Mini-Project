#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include "constants.h"

void interface(int a);
int authorisation_details(int sock);
void user_interface(int sock);
void admin_interface(int sock);

int main(int argc,char *argv[])
{
    int sock = 0; 
	struct sockaddr_in serv_addr; 
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  { 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	interface(sock);
}

void interface(int a) {
	int option, opt_ret;
	char* option_string[100];
	char* return_message[100];
	printf("Select any one of the options...\n");
	printf("1 : Sign In\n");
	printf("0 : To exit\n");
	scanf("%d", &option);

	if(option == 0)
		exit(1);
	
	else if(option != 1){
	    printf("Invalid input\n");
	    exit(1);
	}

	else {
		opt_ret = authorisation_details(a);
		switch (opt_ret) {
			case SIGN_IN_AS_USER :
				while(1) {
					user_interface(a);
				}
				break;
			case SIGN_IN_AS_ADMIN :
				while(1) {
					admin_interface(a);
				}
				break;
			case SIGN_IN_AS_JOINT :
				while(1) {
					user_interface(a);
				}
				break;
		}  
    }
}

int authorisation_details(int sock) {
    int option;
	char* username=malloc(BUF_SIZE*sizeof(char));
	char* password=malloc(BUF_SIZE*sizeof(char));
	char returnmsg[100];
	char* int_to_str=malloc(BUF_SIZE*sizeof(char));
	printf("How would you like to sign in?\n");
	printf("10 : User\n");
	printf("11 : Joint Account User\n");
	printf("12 : Administrator\n");
	scanf("%d", &option);

	printf("Enter the username : ");
	scanf("%s", username);
	printf("Enter the password : ");
	scanf("%s", password);

	sprintf(int_to_str,"%d",option);
	send(sock, int_to_str, sizeof(int_to_str), 0); 
	send(sock, username, sizeof(username), 0); 
	send(sock, password, sizeof(password), 0); 
	read(sock, returnmsg, BUF_SIZE*sizeof(char)); 
	printf("%s\n", returnmsg); 
	if(!strcmp(returnmsg, "sign in failed\n")) 
		exit(1);
	return option;
 }

 void user_interface(int sock){
	int option,deposit_amt, withdraw_amt ;
	char* option_string = malloc(10*sizeof(char));
	char* return_message = malloc(BUF_SIZE*sizeof(char));
	char* amt_string = malloc(BUF_SIZE*sizeof(char));
	char* password = malloc(BUF_SIZE*sizeof(char));

	printf("What would you like us to do for you?\n");
	printf("13 : Deposit\n");
	printf("14 : Withdraw\n");
	printf("15 : Check Balance\n");
	printf("16 : Change Password\n");
	printf("17 : View Details\n");
	printf("18 : Exit\n");
	scanf("%d",&option);

	sprintf(option_string,"%d",option);
	send(sock, option_string, sizeof(option_string), 0); 
	switch(option) {
		case DEPOSIT : 
			printf("Enter amount to be deposited : ");
			scanf("%d", &deposit_amt);
			sprintf(amt_string, "%d", deposit_amt);	
			printf("%s\n", amt_string);		
			send(sock, amt_string, sizeof(amt_string), 0);
			break;
		case WITHDRAW :
			printf("Enter amount to be withdrawn : ");
			scanf("%d",&withdraw_amt);
			sprintf(amt_string,"%d",withdraw_amt);
			send(sock, amt_string, sizeof(amt_string), 0);
			break;
		case BALANCE : 
			break;
		case PASSWORD : 
			printf("Enter new password\n");
			scanf("%s",password);
			send(sock, password, sizeof(password), 0);
			break;
		case DETAILS : 
			break;
		case EXIT : 
			exit(0);
	}
	read(sock, return_message, BUF_SIZE*sizeof(char)); 
	printf("%s\n",return_message); 
}

void admin_interface(int sock){
	int option , type;
	char* option_string = malloc(10*sizeof(char));
	char* username = malloc(BUF_SIZE*sizeof(char));
	char* new_username = malloc(BUF_SIZE*sizeof(char));
	char* password = malloc(BUF_SIZE*sizeof(char));
	char* return_message = malloc(BUF_SIZE*sizeof(char));

	printf("19 : Add User\n");
	printf("20 : Delete User\n");
	printf("21 : Modify User\n");
	printf("22 : Search for Account Details\n");
	printf("18 : Exit\n") ;
	scanf("%d",&option);

	sprintf(option_string,"%d",option);
	send(sock, option_string, sizeof(option_string), 0); 
	switch(option) {
		case ADD_USER :
			printf("Enter User Type\n");
			printf("1 : Normal\n");
			printf("2 : Joint\n");
			scanf("%d",&type);
			printf("Enter username : ");
			scanf("%s",username);
			printf("Enter password : ");
			scanf("%s",password);
			switch (type) {
				case 1 : 
					send(sock, "1", sizeof("1"), 0); 
					break;
				case 2 : 
					send(sock, "2", sizeof("2"), 0); 
					break;
				default :
					printf("Invalid Type\n");
					exit(1);
			}
			printf("username = %s\n",username);
			printf("password = %s\n",password);
			send(sock, username , sizeof(username) , 0 ); 
			send(sock , password , sizeof(password) , 0 ); 
			break;
		case DEL_USER : 
			printf("Enter username : ");
			scanf("%s",username);
			send(sock , username , sizeof(username) , 0 ); 
			break;
		case MOD_USER : 
			printf("Enter old username : " );
			scanf("%s",username);
			send(sock , username , sizeof(username) , 0 ); 
			printf("Enter new username : ");
			scanf("%s",new_username);
			send(sock , new_username , sizeof(new_username) , 0 ); 
			printf("Enter new password : ");
			scanf("%s",password);
			send(sock , password , sizeof(password) , 0 ); 
			break;
		case GET_USER_DETAILS : 
			printf("Enter username : ");
			scanf("%s",username);
			send(sock , username , sizeof(username) , 0 ); 
			break;
		case EXIT : 
			exit(0);
		default :
			printf("Invalid input\n");
			exit(0);
	}
	read( sock , return_message, BUF_SIZE * sizeof(char)); 
	printf("%s\n",return_message); 
}
 
