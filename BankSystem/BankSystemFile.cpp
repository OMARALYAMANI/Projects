
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;
void ShowMainMenuScreen();
void ShowTransactionsMenuScreen();
void ShowManageUsersScreen();
void GoBackToMainMenu();

const string ClientFile = "CLients Bank System.txt";
const string UserFile = "Users Bank System.txt";

struct stClientInfo {
	string AccountNumber;
	string  PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};
struct stUserInfo {
	string username;
	string  password;
	int permission;
	bool MarkForDelete = false;
};

stUserInfo CurrentUser;

enum enPermissons {
	eAll = -1, eShowClientList = 1, eShowAddNewClient = 2, eShowDeleteClient = 4,
	eShowUpdateClient = 8, eShowFindClient = 16, eShowTransactions = 32, eShowManageUsers = 64
};
enum enMainMenuOptions {
	eClientList = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8
};
enum enTransactionsMenuOptions {
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenu = 4
};
enum enManageUsersMenuOptions {
	eListUser = 1, eAddNewUser = 2, eDeleteUser = 3,
	eUpdateUser = 4, eFindUser = 5, eMainMenuFromManageUser = 6
};

vector<string> SplitString(string str, string delim) {

	vector <string> Tokens;
	short pos = 0;
	string sWord;

	while ((pos = str.find(delim)) != string::npos) {

		sWord = str.substr(0, pos);
		if (sWord != "")
			Tokens.push_back(sWord);

		str.erase(0, pos + delim.length()); //earse(), deletes the word,
		//it takes the postion and the number of chars as substr().
	}
	if (str != "")
		Tokens.push_back(str);

	return Tokens;
}

//Check Access for every user:
bool CheckAccessPermission(enPermissons Permission) {

	if (CurrentUser.permission == enPermissons::eAll)
		return true;

	if ((CurrentUser.permission & Permission) == Permission)
		return true;
	else
		return false;
}


//Go back methods:
void GoBackToMainMenu() {
	cout << "\n\nPress any key to go back to main Menu...\n";
	system("pause>0");
	ShowMainMenuScreen();
}
void GoBackToTransactionsMenu() {
	cout << "\n\nPress any key to go back to Transactions Menu...\n";
	system("pause>0");
	ShowTransactionsMenuScreen();
}
void GoBackToManageUsersMenu() {
	cout << "\n\nPress any key to go back to Manage Users Menu...\n";
	system("pause>0");
	ShowManageUsersScreen();

}



//User String Methods:
stUserInfo ConvertUserLineToRecord(string Line, string seperator = "#//#") {
	stUserInfo UserInfo;

	vector <string> vUserData = SplitString(Line, seperator);

	UserInfo.username = vUserData.at(0);
	UserInfo.password = vUserData.at(1);
	UserInfo.permission = stoi(vUserData.at(2));




	return UserInfo;
}
vector <stUserInfo> LoadUserDataFromFile(string FileName) {
	fstream MyFile;

	vector < stUserInfo> vUsersRecords;

	MyFile.open(FileName, ios::in);

	string Line;
	if (MyFile.is_open()) {

		while (getline(MyFile, Line)) {
			vUsersRecords.push_back(ConvertUserLineToRecord(Line));
		}
	}
	MyFile.close();

	return vUsersRecords;
}
string ConvertUserRecordToLine(stUserInfo UserInfo, string seperator = "#//#") {

	string UserLine = "";
	UserLine += UserInfo.username + seperator;
	UserLine += UserInfo.password + seperator;
	UserLine += to_string(UserInfo.permission);

	return UserLine;
}


//show Denied Screen:
void ShowDeniedScreen() {
	cout << "--------------------------------------\n";
	cout << "Access Denied\n";
	cout << "You do not have Permission to do this,\nPlease Contact your Admin.\n";
	cout << "--------------------------------------\n";
	GoBackToMainMenu();
}

//manage users features:

string ReadUserName() {
	string username;

	cout << "Enter username: ";
	getline(cin >> ws, username);

	return username;
}
string ReadPassword() {
	string password;

	cout << "Enter password: ";
	cin >> password;

	return password;
}

// List users:
void PrintUsersRecord(stUserInfo User) {
	cout << "| " << setw(20) << left << User.username;
	cout << "| " << left << setw(15) << User.password;
	cout << "| " << left << setw(40) << User.permission;
	cout << endl;
}
void ShowUsersListScreen() {

	vector <stUserInfo> vUsers = LoadUserDataFromFile(UserFile);
	cout << "\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n________________________________________________________________________________________________\n\n";

	for (stUserInfo User : vUsers) {

		PrintUsersRecord(User);
	}
	cout << "\n\n_______________________________________________________________________________________________\n";
}


//add Users:
bool UserExistsUsername(string username, string FileName) {
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		stUserInfo User;

		while (getline(MyFile, Line)) {

			User = ConvertUserLineToRecord(Line);
			if (User.username == username) {

				MyFile.close();
				return true;
			}

		}
		MyFile.close();
	}
	return false;


}
void AddLineToFile(string ClientLine, string FileName) {
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);
	//MyFile << endl;
	if (MyFile.is_open()) {

		MyFile << ClientLine << endl;

	}
	MyFile.close();
}
short ReadPermissionsToSet() {
	short permissions = 0;

	char fullAccess = 'n';
	char answer = 'y';

	cout << "\n\nDo you want to give full access? (y/n)?";
	cin >> fullAccess;

	if (tolower(fullAccess) == 'y') {
		return -1;
	}


	
	cout << "\n\nDo you want to give access to:\n\n";

	cout << "\n\nShow Client List (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowClientList;

	cout << "\n\nAdd New Client (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowAddNewClient;

	cout << "\n\nDelete Client (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowDeleteClient;

	cout << "\n\nUpadate Client (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowUpdateClient;

	cout << "\n\nFind Client (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowFindClient;

	cout << "\n\nTransactions (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowTransactions;

	cout << "\n\nManage Users (y/n)?";
	cin >> answer;

	if (answer == 'y')
		permissions += enPermissons::eShowManageUsers;

	return permissions;

}
stUserInfo ReadNewUser() {
	stUserInfo User;
	
	User.username = ReadUserName();
	
	while (UserExistsUsername(User.username, UserFile)) {

		cout << "\n\nUser with [" << User.username << "] already exists, Enter another username? ";
		getline(cin >> ws, User.username);
	}

	User.password = ReadPassword();

	User.permission = ReadPermissionsToSet();

	return User;
}
void AddNewUser() {
	stUserInfo User;
	User = ReadNewUser();
	AddLineToFile(ConvertUserRecordToLine(User), UserFile);
}
void AddNewUsers() {
	char addAgain = 'Y';

	do {

		cout << "\nAdding new User: \n\n";
		AddNewUser();

		cout << "\n\nUser Added Successfully, do you want to more Users (Y/N)? ";
		cin >> addAgain;


	} while (toupper(addAgain) == 'Y');
}
void ShowAddUserScreen() {
	cout << "-------------------------------------------\n";
	cout << "\t\tAdd New Users Screen\n";
	cout << "-------------------------------------------\n";

	AddNewUsers();
}


//delete User:
bool SearchByUsername(stUserInfo& User, string username, vector <stUserInfo>& vUsers) {

	for (stUserInfo U : vUsers) {

		if (U.username == username) {
			User = U;
			return true;
		}

	}
	return false;


}
void PrintUserData(stUserInfo User) {
	cout << "\n\nThe following are the user details: \n";
	cout << "--------------------------------------\n";
	cout << "Username    : " << User.username << endl;
	cout << "Password    : " << User.password << endl;
	cout << "Permissions : " << User.permission << endl;
	cout << "--------------------------------------\n";

}
bool MarkUserForDeleteByUsername(string username, vector <stUserInfo>& vUsers) {

	for (stUserInfo& C : vUsers) {

		if (username == C.username) {
			C.MarkForDelete = true;
			return true;
		}

	}
	return false;
}
vector <stUserInfo> SaveUsersDataToFile(string UserFile, vector <stUserInfo> vUsers) {

	fstream MyFile;

	MyFile.open(UserFile, ios::out);

	if (MyFile.is_open()) {

		for (stUserInfo U : vUsers) {

			if (U.MarkForDelete == false)
				MyFile << ConvertUserRecordToLine(U) << endl;
		}

		MyFile.close();
	}


	return vUsers;
}
void DeleteUserByUsername(string username, vector <stUserInfo>& vUsers) {
	stUserInfo User;
	char DeleteOrNot = 'n';

	if (SearchByUsername(User, username, vUsers))
	{
		PrintUserData(User);

		cout << "\n\nAre you sure you want to delete this User? (y/n): ";
		cin >> DeleteOrNot;
		if (tolower(DeleteOrNot) == 'y') {

			MarkUserForDeleteByUsername(username, vUsers);
			SaveUsersDataToFile(UserFile, vUsers);
			vUsers = LoadUserDataFromFile(UserFile);

			cout << "\n\nUser deleted Successfully.\n";
		}

	}
	else {
		cout << "\nUser with Username (" << username << ") is NOT Found!\n";
	}

}
void ShowDeleteUsersScreen() {
	cout << "-------------------------------------------\n";
	cout << "\tDelete Users Screen\n";
	cout << "-------------------------------------------\n";

	vector <stUserInfo> vUsers = LoadUserDataFromFile(UserFile);
	string username = ReadUserName();
	if (username == "Admin") {
		cout << "\n\nYou cannot delete this user.\n\n";
		GoBackToManageUsersMenu();
	}
	DeleteUserByUsername(username, vUsers);
}



//Update User:
stUserInfo ChangeUserData(string username) {
	stUserInfo User;

	User.username = username;

	cout << "\n\n\nEnter password: ";
	getline(cin >> ws, User.password);

	User.permission = ReadPermissionsToSet();

	return User;
}
void UpdateUserByUsername(string username, vector <stUserInfo>& vUsers) {

	stUserInfo User;
	char answer = 'n';

	if (SearchByUsername(User, username, vUsers))
	{
		PrintUserData(User);

		cout << "\n\nAre you sure you want to update this User? (y/n): ";
		cin >> answer;
		if (tolower(answer) == 'y') {

			for (stUserInfo& C : vUsers) {

				if (C.username == username) {
					C = ChangeUserData(username);
					break;
				}
			}

			SaveUsersDataToFile(UserFile, vUsers);

			cout << "\n\nClient Updated Successfully.\n";
		}

	}
	else {
		cout << "\nUser with username (" << username << ") is NOT Found!\n";
	}

}
void ShowUpdateUserInfoScreen() {
	cout << "-------------------------------------------\n";
	cout << "\tUpdate User Info Screen\n";
	cout << "-------------------------------------------\n";

	vector <stUserInfo> vUsers = LoadUserDataFromFile(UserFile);
	string username = ReadUserName();
	UpdateUserByUsername(username, vUsers);

}


//find user:
void ShowFindUserScreen() {
	cout << "-------------------------------------------\n";
	cout << "\t\tFind User Screen\n";
	cout << "-------------------------------------------\n";

	stUserInfo User;
	vector <stUserInfo> vUsers = LoadUserDataFromFile(UserFile);
	string username = ReadUserName();

	if (SearchByUsername(User, username, vUsers)) {
		PrintUserData(User);
	}
	else {
		cout << "\nUser with username (" << username << ") is NOT Found!\n";
	}
}


short ReadManageUsersOption() {
	short Option;

	cout << "Choose what do you want to do? [1 to 6]: ";
	cin >> Option;

	return Option;
}
void PerformManageUsersOption(enManageUsersMenuOptions option) {

	switch (option) {

	case enManageUsersMenuOptions::eListUser:
		system("cls");
		ShowUsersListScreen();
		GoBackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eAddNewUser:
		system("cls");
		ShowAddUserScreen();
		GoBackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eDeleteUser:
		system("cls");
		ShowDeleteUsersScreen();
		GoBackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eUpdateUser:
		system("cls");
		ShowUpdateUserInfoScreen();
		GoBackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenu();
		break;

	case enManageUsersMenuOptions::eMainMenuFromManageUser:
		system("cls");
		ShowMainMenuScreen();
		break;
	}

}
void ShowManageUsersScreen() {

	if (!CheckAccessPermission(enPermissons::eShowManageUsers)) {
		ShowDeniedScreen();
		return;
	}
	system("cls");
	cout << "=========================================================\n";
	cout << "\t\tManage Users Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] List Users.\n";
	cout << "\t\t[2] Add New User.\n";
	cout << "\t\t[3] Delete User.\n";
	cout << "\t\t[4] Update User.\n";
	cout << "\t\t[5] Find User.\n";
	cout << "\t\t[6] Main Menu.\n";
	cout << "=========================================================\n";

	PerformManageUsersOption((enManageUsersMenuOptions)ReadManageUsersOption());

}





//client side:

//Client String methods:

stClientInfo ConvertLineToRecord(string Line, string seperator = "#//#") {
	stClientInfo ClientInfo;

	vector <string> vClientData = SplitString(Line, seperator);

	ClientInfo.AccountNumber = vClientData.at(0);
	ClientInfo.PinCode = vClientData.at(1);
	ClientInfo.Name = vClientData.at(2);
	ClientInfo.Phone = vClientData.at(3);
	ClientInfo.AccountBalance = stod(vClientData.at(4));



	return ClientInfo;
}
vector <stClientInfo> LoadDataFromFile(string FileName) {
	fstream MyFile;

	vector < stClientInfo> vClientsRecords;

	MyFile.open(FileName, ios::in);

	string Line;
	if (MyFile.is_open()) {

		while (getline(MyFile, Line)) {
			vClientsRecords.push_back(ConvertLineToRecord(Line));
		}
	}
	MyFile.close();

	return vClientsRecords;
}
string ConvertRecordToLine(stClientInfo ClientInfo, string seperator = "#//#") {

	string ClientLine = "";
	ClientLine += ClientInfo.AccountNumber + seperator;
	ClientLine += ClientInfo.PinCode + seperator;
	ClientLine += ClientInfo.Name + seperator;
	ClientLine += ClientInfo.Phone + seperator;
	ClientLine += to_string(ClientInfo.AccountBalance);

	return ClientLine;
}


//Client list methods:
void PrintClientRecord(stClientInfo Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;
	cout << endl;
}
void ShowClientListScreen() {

	if (!CheckAccessPermission(enPermissons::eShowClientList)) {
		ShowDeniedScreen();
		return;
	}
	vector < stClientInfo> vClients = LoadDataFromFile(ClientFile);
	cout << "\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n________________________________________________________________________________________________\n\n";

	for (stClientInfo Client : vClients) {

		PrintClientRecord(Client);
	}
	cout << "\n\n_______________________________________________________________________________________________\n";
}

//Add new client methods:
bool ClientExistsAccoutNumber(string AccountNumber, string FileName) {
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {
		string Line;
		stClientInfo Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber) {

				MyFile.close();
				return true;
			}

		}
		MyFile.close();
	}
	return false;


}
stClientInfo ReadNewClient() {
	stClientInfo Client;

	cout << "\nEnter Account Number: ";
	getline(cin >> ws, Client.AccountNumber);//cin >> ignore all white characters

	while (ClientExistsAccoutNumber(Client.AccountNumber, ClientFile)) {

		cout << "\n\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode: ";
	getline(cin, Client.PinCode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}
void AddNewCLient() {
	stClientInfo Client;
	Client = ReadNewClient();
	AddLineToFile(ConvertRecordToLine(Client), ClientFile);
}
void AddNewClients() {

	char addAgain = 'Y';

	do {
		//system("cls");
		cout << "\nAdding new client: \n\n";
		AddNewCLient();

		cout << "\n\nClient Added Successfully, do you want to more clients (Y/N)? ";
		cin >> addAgain;


	} while (toupper(addAgain) == 'Y');
}
void ShowAddNewClientsScreen() {

	if (!CheckAccessPermission(enPermissons::eShowAddNewClient)) {
		ShowDeniedScreen();
		return;
	}
	cout << "-------------------------------------------\n";
	cout << "\t\tAdd New Clients Screen\n";
	cout << "-------------------------------------------\n";

	AddNewClients();
}


//delete client methods:
string ReadAccountNumber() {
	string AccountNumber;

	cout << "Please enter Account Number: ";
	cin >> AccountNumber;

	return AccountNumber;
}
void printClientData(stClientInfo ClientInfo) {

	cout << "\n\nThe following are the client details: \n";
	cout << "--------------------------------------\n";
	cout << "Account Number : " << ClientInfo.AccountNumber << endl;
	cout << "Pin Code       : " << ClientInfo.PinCode << endl;
	cout << "Name           : " << ClientInfo.Name << endl;
	cout << "Phone          : " << ClientInfo.Phone << endl;
	cout << "Account Balance: " << ClientInfo.AccountBalance << endl;
	cout << "--------------------------------------\n";
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClientInfo>& vClients) {

	for (stClientInfo& C : vClients) {

		if (AccountNumber == C.AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}

	}
	return false;
}
vector <stClientInfo> SaveClientsDataToFile(string ClientFile, vector <stClientInfo> vClients) {

	fstream MyFile;

	MyFile.open(ClientFile, ios::out);

	if (MyFile.is_open()) {

		for (stClientInfo C : vClients) {

			if (C.MarkForDelete == false)
				MyFile << ConvertRecordToLine(C) << endl;
		}

		MyFile.close();
	}


	return vClients;
}
bool SearchByAccountNumber(stClientInfo& Client, string AccountNumber, vector <stClientInfo>& vClients) {

	for (stClientInfo c : vClients) {

		if (c.AccountNumber == AccountNumber) {

			Client = c;
			return true;
		}

	}
	return false;


}
void DeleteClientByAccountNumber(string AccountNumber, vector <stClientInfo>& vClients) {
	stClientInfo Client;
	char DeleteOrNot = 'n';

	if (SearchByAccountNumber(Client, AccountNumber, vClients))
	{
		printClientData(Client);

		cout << "\n\nAre you sure you want to delete this client? (y/n): ";
		cin >> DeleteOrNot;
		if (tolower(DeleteOrNot) == 'y') {

			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientFile, vClients);
			vClients = LoadDataFromFile(ClientFile);

			cout << "\n\nClient deleted Successfully.\n";
		}

	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found!\n";
	}

}
void ShowDeleteClientsScreen() {

	if (!CheckAccessPermission(enPermissons::eShowDeleteClient)) {
		ShowDeniedScreen();
		return;
	}
	cout << "-------------------------------------------\n";
	cout << "\tDelete Clients Screen\n";
	cout << "-------------------------------------------\n";

	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);
}

//update Client methods:
stClientInfo ChangeClientData(string AccountNumber) {
	stClientInfo Client;

	Client.AccountNumber = AccountNumber;

	cout << "\n\n\nEnter PinCode: ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name: ";
	getline(cin, Client.Name);

	cout << "Enter Phone: ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}
void UpdateClientByAccountNumber(string AccountNumber, vector <stClientInfo>& vClients) {

	stClientInfo Client;
	char answer = 'n';

	if (SearchByAccountNumber(Client, AccountNumber, vClients))
	{
		printClientData(Client);

		cout << "\n\nAre you sure you want to update this client? (y/n): ";
		cin >> answer;
		if (tolower(answer) == 'y') {

			for (stClientInfo& C : vClients) {

				if (C.AccountNumber == AccountNumber) {
					C = ChangeClientData(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(ClientFile, vClients);

			cout << "\n\nClient Updated Successfully.\n";
		}

	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found!\n";
	}

}
void ShowUpdateClientInfoScreen() {

	if (!CheckAccessPermission(enPermissons::eShowUpdateClient)) {
		ShowDeniedScreen();
		return;
	}
	cout << "-------------------------------------------\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "-------------------------------------------\n";

	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

//Find Client method:
void ShowFindClientScreen() {

	if (!CheckAccessPermission(enPermissons::eShowFindClient)) {
		ShowDeniedScreen();
		return;
	}

	cout << "-------------------------------------------\n";
	cout << "\t\tFind Client Screen\n";
	cout << "-------------------------------------------\n";

	stClientInfo Client;
	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();

	if (SearchByAccountNumber(Client, AccountNumber, vClient)) {
		printClientData(Client);
	}
	else {
		cout << "\nClient with Account Number (" << AccountNumber << ") is NOT Found!\n";
	}
}


//deposit Transactions methods:
void DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClientInfo> vClients) {

	char answer = 'n';

	cout << "\n\nAre you sure you want to perform this Transaction? (y/n): ";
	cin >> answer;

	if (tolower(answer) == 'y') {

		for (stClientInfo& C : vClients) {

			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;

				SaveClientsDataToFile(ClientFile, vClients);
				cout << "\n\nDone Successfully, your new balance is: " << C.AccountBalance << endl;
				break;
			}
		}
	}




}
void ShowDepositScreen() {
	cout << "-------------------------------------------\n";
	cout << "\t\tDeposit Screen\n";
	cout << "-------------------------------------------\n";

	stClientInfo Client;
	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();

	while (!SearchByAccountNumber(Client, AccountNumber, vClient)) {

		cout << "\n\nClient with [" << AccountNumber << "] does not exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}
	printClientData(Client);

	double Amount;
	cout << "\nplease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClient);
}

//withdraw transactions methods:
double CheckWithdrawAmount(stClientInfo Client, double withdrawAmount) {

	while (Client.AccountBalance < withdrawAmount) {
		cout << "Amount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance << endl;

		cout << "\nplease enter another withdraw amount? ";
		cin >> withdrawAmount;
	}
	return withdrawAmount;

}
void ShowWithdrawScreen() {
	cout << "-------------------------------------------\n";
	cout << "\t\tWithdraw Screen\n";
	cout << "-------------------------------------------\n";

	stClientInfo Client;
	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();

	while (!SearchByAccountNumber(Client, AccountNumber, vClient)) {

		cout << "\n\nClient with [" << AccountNumber << "] does not exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}
	printClientData(Client);

	double Amount;
	cout << "\nplease enter withdraw amount? ";
	cin >> Amount;

	Amount = CheckWithdrawAmount(Client, Amount);

	DepositBalanceToClientByAccountNumber(AccountNumber, -1 * Amount, vClient);
}

//Total Balances methods:
void PrintClientBalance(stClientInfo Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.AccountBalance;
	cout << endl;
}
void ShowTotalBalancesScreen() {
	vector <stClientInfo> vClients = LoadDataFromFile(ClientFile);

	cout << "\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";

	cout << "\n________________________________________________________________________________________________\n\n";

	double TotalBalances = 0;

	for (stClientInfo Client : vClients) {
		PrintClientBalance(Client);
		TotalBalances += Client.AccountBalance;
	}
	cout << "\n\n_______________________________________________________________________________________________\n";

	cout << "\t\t\t\t\tTotal Balances = " << TotalBalances << endl;
}


void PreformTransactionsMenuOption(enTransactionsMenuOptions option) {
	switch (option) {

	case enTransactionsMenuOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eTotalBalances:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eMainMenu:
		system("cls");
		ShowMainMenuScreen();
		break;
	}

}
short ReadTransactionsOption() {
	short Option;
	cout << "Choose what do you want to do? [1 to 4]: ";
	cin >> Option;
	return Option;
}
void ShowTransactionsMenuScreen() {

	if (!CheckAccessPermission(enPermissons::eShowTransactions)) {
		ShowDeniedScreen();
		return;
	}
	system("cls");
	cout << "=========================================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] Deposit.\n";
	cout << "\t\t[2] Withdraw.\n";
	cout << "\t\t[3] Total Balances.\n";
	cout << "\t\t[4] Main Menu.\n";
	cout << "=========================================================\n";
	PreformTransactionsMenuOption((enTransactionsMenuOptions)ReadTransactionsOption());
}


//login:
bool SearchByUsernameAndPassword(stUserInfo& User, string username, string password, vector <stUserInfo>& vUsers) {

	for (stUserInfo U : vUsers) {

		if (U.password == password && U.username == username) {
			User = U;
			return true;
		}

	}
	return false;


}
void Login() {

	bool LoginFaild = false;
	string username, password;


	do {
		system("cls");
		cout << "-------------------------------------------\n";
		cout << "\t\tLogin screen\n";
		cout << "-------------------------------------------\n";

		vector <stUserInfo> vUsers = LoadUserDataFromFile(UserFile);

		if (LoginFaild) {
			cout << "Invalid Username/Password!\n";
		}

		cout << "Enter username: ";
		getline(cin >> ws, username);

		cout << "Enter password: ";
		cin >> password;

		if (!SearchByUsernameAndPassword(CurrentUser, username, password, vUsers))
			LoginFaild = true;
		else
			LoginFaild = false;



	} while (LoginFaild);

	ShowMainMenuScreen();



}

void PreformMainMenuOption(enMainMenuOptions option) {

	switch (option) {

	case enMainMenuOptions::eClientList:
		system("cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientInfoScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;

	case enMainMenuOptions::eTransactions:
		system("cls");
		ShowTransactionsMenuScreen();
		break;

	case enMainMenuOptions::eManageUsers:
		system("cls");
		ShowManageUsersScreen();
		break;

	case enMainMenuOptions::eLogout:
		system("cls");
		Login();
		break;
	}
}
short ReadMainMenuOption() {
	short Option;
	cout << "Choose what do you want to do? [1 to 8]: ";
	cin >> Option;
	return Option;
}

void ShowMainMenuScreen() {
	system("cls");
	cout << "=========================================================\n";
	cout << "\t\t\t Main Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t\t[1] Show CLient List.\n";
	cout << "\t\t[2] Add New Client.\n";
	cout << "\t\t[3] Delete Client.\n";
	cout << "\t\t[4] Update Client Info.\n";
	cout << "\t\t[5] Find CLient.\n";
	cout << "\t\t[6] Transactions.\n";
	cout << "\t\t[7] Manage Users.\n";
	cout << "\t\t[8] Logout.\n";
	cout << "=========================================================\n";
	short Option = ReadMainMenuOption();
	PreformMainMenuOption((enMainMenuOptions)Option);


}



int main() {

	Login();


}
