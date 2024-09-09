# OS MiniProject
# Online Banking Management System

The banking system project aims to develop a user-friendly and multi-functional banking application with various features and functionalities. The project utilizes file storage for storing account transaction details and customer information. It incorporates a login system to ensure secure access to user accounts, including password-protected administrative access to manage the system.

## Functionalities

The banking system project includes the following functionalities:

1. User Management:
   - Login system for account holders to access their accounts.
   - Three types of logins: normal user, joint account user, and administrator.

2. Administrative Access:
   - Password-protected administrative access to manage the banking system.
   - Ability to add, delete, modify, and search for specific account details.

3. Customer Transactions:
   - Account holders can perform various transactions such as deposit, withdrawal, balance inquiry, password change, view details, and exit.
   - User-friendly menu for easy navigation.

4. Joint Account Handling:
   - Proper file locking mechanism implemented for joint account holders.
   - Read lock for viewing account details and write lock for deposit and withdrawal operations.
   - Ensures data integrity and prevents conflicts in concurrent access.

5. Socket Programming:
   - Server-client architecture using socket programming.
   - The server maintains the database and services multiple clients concurrently.
   - Clients can connect to the server to access their specific account details.

6. System Calls:
   - Utilization of system calls instead of library functions wherever possible.
   - System calls used for process management, file management, file locking, multithreading, and interprocess communication mechanisms.

## Usage

To use the banking system application, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/project-repo.git`
2. Compile and run the server program on the server machine.
3. Compile and run the client program on client machines to connect to the server and access their account details.
4. Follow the login prompts and navigate the menu options for performing various transactions.
5. For administrative access, use the specified login credentials to manage account details.

## Dependencies

The banking system project does not require any external dependencies. It is built using system calls and standard libraries available in the chosen programming language.
