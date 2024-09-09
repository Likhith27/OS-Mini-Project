#define PORT 8080 

// possible menus
#define START_MENU 0
#define SIGN_IN_OPTIONS 2
#define USER_OPTIONS 3
#define ADMIN_OPTIONS 4

// possible options
#define SIGN_IN 6
#define SIGN_UP_AS_USER 7
#define SIGN_UP_AS_JOINT 8
#define SIGN_IN_AS_USER 10
#define SIGN_IN_AS_JOINT 11
#define SIGN_IN_AS_ADMIN 12
#define DEPOSIT 13
#define WITHDRAW 14
#define BALANCE 15
#define PASSWORD 16
#define DETAILS 17
#define EXIT 18
#define ADD_USER 19
#define DEL_USER 20
#define MOD_USER 21
#define GET_USER_DETAILS 22
#define INVALID -1
#define BUF_SIZE 100

//struct user
struct user {
    char type[BUF_SIZE];
    char password[BUF_SIZE];
    char username[BUF_SIZE];
};

//struct details
struct account {
    int balance;
};