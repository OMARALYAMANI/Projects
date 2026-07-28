
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>

using namespace std;
void ShowMainMenuScreen();
void ShowTransactionsMenuScreen();

const string ClientFile = "CLients Data project.txt";

enum enMainMenuOptions {
	eClientList = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eExit = 7
};
enum enTransactionsMenuOptions {
	eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenu = 4
};

struct stClientInfo {
	string AccountNumber;
	string  PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
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
void AddLineToFile(string ClientLine, string FileName) {
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);
	//MyFile << endl;
	if (MyFile.is_open()) {

		MyFile << ClientLine << endl;

	}
	MyFile.close();
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
vector <stClientInfo> SaveClientsDataToFile(string FileName, vector <stClientInfo> vClients) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

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
	cout << "-------------------------------------------\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "-------------------------------------------\n";

	vector <stClientInfo> vClient = LoadDataFromFile(ClientFile);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

//Find Client method:
void ShowFindClientScreen() {
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
void ShowExitScreen() {
	cout << "-------------------------------------------\n";
	cout << "\t\tProgram Ends :-)\n";
	cout << "-------------------------------------------\n";
}

//deposit Transactions methods:
void DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount, vector<stClientInfo> vClients) {

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
	cin >>Amount;

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

	DepositBalanceToClientByAccountNumber(AccountNumber, -1 * Amount , vClient);
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

	case enMainMenuOptions::eExit:
		system("cls");
		ShowExitScreen();
		break;
	}
}
short ReadMainMenuOption() {
	short Option;
	cout << "Choose what do you want to do? [1 to 7]: ";
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
	cout << "\t\t[7] Exit.\n";
	cout << "=========================================================\n";
	PreformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
}


int main() {

	ShowMainMenuScreen();

}