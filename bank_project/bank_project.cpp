#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

const string clientsFileName = "Clients.txt";

enum enOptions
{
	ShowClientsList = 1,
	AddNewClient = 2,
	DeleteClient = 3,
	UpdateClient = 4,
	FindClient = 5,
	Exit = 6
};

struct stClient
{
	string accountNumber = "";
	string name = "";
	string id = "";
	string phoneNumber = "";
	string pincode = "";
	double balance = 0;
	bool markToDelete = false;
};

void validationNumber(int& num, int from, int to, string msg)
{
	while (cin.fail() || num < from || num > to)
	{
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << msg;
		cin >> num;
	}
}

int readMainMenuOption()
{
	int tmp = 0;

	cout << "Choose what do you want to do: [1 to 6]? ";
	cin >> tmp;

	validationNumber(tmp, 1, 6, "\nInvalid number, please enter a valid number: ");

	return tmp;
}

void showMainMenu()
{
	system("cls");
	cout << "============================================\n";
	cout << right << setw(32) << "Main Menu Screen\n";
	cout << "============================================\n";
	cout << "[1] Show Clients List.\n";
	cout << "[2] Add New Client.\n";
	cout << "[3] Delete Client.\n";
	cout << "[4] Update Client Info.\n";
	cout << "[5] Find Clients.\n";
	cout << "[6] Exit.\n";
	cout << "===========================================\n";
}

enOptions showMainMenuScreen()
{
	showMainMenu();
	return ((enOptions)readMainMenuOption());
}

void backToMenuScreen()
{
	cout << "\nPress any key to go back to main menu...";
	system("pause>0");
}

vector<string> vSplit(string str, string delim = "#//#")
{
	vector<string> split;
	short pos = 0;
	string word = "";

	while ((pos = str.find(delim)) != std::string::npos)
	{
		word = str.substr(0, pos);

		if (word != "")
		{
			split.push_back(word);
		}

		str = str.erase(0, pos + delim.length());
	}

	split.push_back(str);

	return split;
}

stClient convertLineToStruct(string line)
{
	vector<string> splitLine = vSplit(line);
	stClient data;

	data.accountNumber = splitLine[0];
	data.name = splitLine[1];
	data.id = splitLine[2];
	data.phoneNumber = splitLine[3];
	data.pincode = splitLine[4];
	data.balance = stod(splitLine[5]);

	return data;
}

string convertStructToLine(stClient clientData, string separator = "#//#")
{
	string str = "";

	str = clientData.accountNumber + separator
		+ clientData.name + separator
		+ clientData.id + separator
		+ clientData.phoneNumber + separator
		+ clientData.pincode + separator
		+ to_string(clientData.balance);

	return str;
}

vector<stClient> loadClientsDataFromFileToVector(string fileName)
{
	fstream myFile;
	vector<stClient>vClients;
	string line = "";

	myFile.open(fileName, ios::in);

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			vClients.push_back(convertLineToStruct(line));
		}

		myFile.close();
	}

	return vClients;
}

void reloadDataFromVectorToFile(string fileName, vector<stClient>vClients)
{
	fstream myFile;

	myFile.open(fileName, ios::out);

	if (myFile.is_open())
	{
		for (stClient& data : vClients)
		{
			if (!data.markToDelete)
			{
				myFile << convertStructToLine(data) << endl;
			}
		}
		myFile.close();
	}
}

void showClientsList(vector<stClient> clientsData)
{
	if (clientsData.size() == 0)
		cout << right << setw(75) << "No Clients Available In the System!\n";
	else
	{
		cout << right << setw(65) << "Number Of Clients (" << clientsData.size() << ").\n";
		cout << "-----------------------------------------------------------------------------------------------------------------------\n";
		cout << "|" << left << setw(20) << "Account Number" << "|" << setw(25) << "Name" << "|" << setw(20) << "ID"
			<< "|" << setw(17) << "Phone Number" << "|" << setw(15) << "PIN Code" << "|" << setw(15) << "Balance" << "|" << endl;
		cout << "-----------------------------------------------------------------------------------------------------------------------\n";

		for (stClient& data : clientsData)
		{
			cout << "|" << left << setw(20) << data.accountNumber
				<< "|" << setw(25) << data.name
				<< "|" << setw(20) << data.id
				<< "|" << setw(17) << data.phoneNumber
				<< "|" << setw(15) << data.pincode
				<< "|" << setw(15) << data.balance << "|" << endl;
		}
		cout << "-----------------------------------------------------------------------------------------------------------------------\n";
	}
}

void saveClientDataToFile(string fileName, stClient clientData)
{
	fstream myFile;

	myFile.open(fileName, ios::out | ios::app);

	if (myFile.is_open())
	{
		myFile << convertStructToLine(clientData) << endl;
		myFile.close();
	}
}

bool ClientExistsByAccountNumber(string accountNumber, vector<stClient> vClients)
{
	for (stClient& data : vClients)
	{
		if (data.accountNumber == accountNumber)
		{
			return true;
		}
	}

	return false;
}

void readNewClient(string fileName, vector<stClient>& vClients)
{
	stClient clientData;

	cout << "\n\nPlease enter account number: ";
	getline(cin >> ws, clientData.accountNumber);

	while (ClientExistsByAccountNumber(clientData.accountNumber, vClients))
	{
		cout << "\nClient with account number [" << clientData.accountNumber << "] is already exists, please enter another account number: ";
		getline(cin >> ws, clientData.accountNumber);
	}

	cout << "\nPlease enter name: ";
	getline(cin, clientData.name);

	cout << "\nPlease enter ID: ";
	getline(cin, clientData.id);

	cout << "\nPlease enter phone number: ";
	getline(cin, clientData.phoneNumber);

	cout << "\nPlease enter PIN code: ";
	getline(cin, clientData.pincode);

	cout << "\nPlease enter balance: ";
	cin >> clientData.balance;

	saveClientDataToFile(fileName, clientData);
}

void ShowAddNewClientsScreen()
{
	cout << "--------------------------------------\n";
	cout << setw(30) << "Add New Client Screen\n";
	cout << "--------------------------------------\n";
	cout << "Adding a new client: ";
}

void addClients(string fileName, vector<stClient>& vClients)
{
	char moreClient = 'y';

	while (tolower(moreClient) == 'y')
	{
		system("cls");
		ShowAddNewClientsScreen();
		readNewClient(fileName, vClients);
		cout << "\nClient added successfully, do you want to add more clients? [y/n]? ";
		cin >> moreClient;
		if (tolower(moreClient) == 'y')
			vClients = loadClientsDataFromFileToVector(fileName);
	}
}

void showClientData(stClient clientData)
{
	cout << "\nThe follwing are the client data: \n";
	cout << "--------------------------------------\n";
	cout << "Account Number: " << clientData.accountNumber << endl;
	cout << "Name          : " << clientData.name << endl;
	cout << "ID            : " << clientData.id << endl;
	cout << "Phone Number  : " << clientData.phoneNumber << endl;
	cout << "PIN Code      : " << clientData.pincode << endl;
	cout << "Balance       : " << clientData.balance << endl;
	cout << "--------------------------------------\n";
}

void markForDelete(vector<stClient>& vClients, string accountNumber)
{
	for (stClient& clientData : vClients)
	{
		if (clientData.accountNumber == accountNumber)
		{
			clientData.markToDelete = true;
		}
	}
}

bool findClientByAccountNumber(vector<stClient>vClients, string accountNumber, stClient& clientData)
{
	for (stClient& data : vClients)
	{
		if (data.accountNumber == accountNumber)
		{
			clientData = data;
			return true;
		}
	}
	return false;
}

string readAccountNumber()
{
	string accountNumber = "";

	cout << "\nPlease enter account number: ";
	getline(cin >> ws, accountNumber);

	return accountNumber;
}

void deleteClientScreen()
{
	cout << "--------------------------------------\n";
	cout << setw(30) << "Delete Client Screen\n";
	cout << "--------------------------------------\n";
}

void deleteClient(string fileName, vector<stClient>& vClients)
{
	string accountNumber = "";
	char check = 'n';
	stClient clientData;

	deleteClientScreen();

	accountNumber = readAccountNumber();

	if (!findClientByAccountNumber(vClients, accountNumber, clientData))
		cout << "\nClient with account number [" << accountNumber << "] is not found!\n";
	else
	{
		showClientData(clientData);

		cout << "\nAre you sure you want to delete this client? [y/n]? ";
		cin >> check;

		if (tolower(check) == 'y')
		{
			markForDelete(vClients, accountNumber);

			reloadDataFromVectorToFile(fileName, vClients);

			vClients = loadClientsDataFromFileToVector(fileName);

			cout << "\nClient deleted successfully.\n";
		}
	}
}

void ChangeClientInfo(vector<stClient>& vClients, string accountNumber)
{
	for (stClient& data : vClients)
	{
		if (data.accountNumber == accountNumber)
		{
			cout << "\nPlease enter name: ";
			getline(cin >> ws, data.name);

			cout << "\nPlease enter ID: ";
			getline(cin, data.id);

			cout << "\nPlease enter phone number: ";
			getline(cin, data.phoneNumber);

			cout << "\nPlease enter PIN code: ";
			getline(cin, data.pincode);

			cout << "\nPlease enter balance: ";
			cin >> data.balance;
			break;
		}
	}
}

void showUpdateClientInfoScreen()
{
	cout << "--------------------------------------\n";
	cout << setw(33) << "Update Client Info Screen\n";
	cout << "--------------------------------------\n";
}

void updateClientInfo(string fileName, vector<stClient>& vClients)
{
	string accountNumber = "";
	stClient clientData;
	char check = 'n';

	showUpdateClientInfoScreen();

	accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(vClients, accountNumber, clientData))
	{
		cout << "\nClient with account number [" << accountNumber << "] is not found, please enter another account number: ";
		getline(cin >> ws, accountNumber);
	}

	showClientData(clientData);

	cout << "\nAre you sure you want to update this client info? [y/n]? ";
	cin >> check;

	if (tolower(check) == 'y')
	{
		ChangeClientInfo(vClients, accountNumber);
		reloadDataFromVectorToFile(fileName, vClients);
		cout << "\nClient info updated successfully." << endl;
	}
}

void showFindClientScreen()
{
	cout << "--------------------------------------\n";
	cout << setw(30) << "Find Client Screen\n";
	cout << "--------------------------------------\n";
}

void findClient(string fileName, vector<stClient> vClients)
{
	string accountNumber = "";
	stClient clientData;

	showFindClientScreen();

	accountNumber = readAccountNumber();

	if (findClientByAccountNumber(vClients, accountNumber, clientData))
		showClientData(clientData);
	else
		cout << "\nClient with account number [" << accountNumber << "] is not found!\n";
}

void exit()
{
	cout << "--------------------------------------\n";
	cout << setw(27) << "Program Ends.\n";
	cout << "--------------------------------------\n";
}

void startProgram(string fileName)
{
	while (true)
	{
		vector<stClient>vClients = loadClientsDataFromFileToVector(fileName);
		enOptions option = showMainMenuScreen();

		switch (option)
		{
		case enOptions::ShowClientsList:
			system("cls");
			showClientsList(vClients);
			backToMenuScreen();
			break;
		case enOptions::AddNewClient:
			addClients(fileName, vClients);
			backToMenuScreen();
			break;
		case enOptions::DeleteClient:
			system("cls");
			deleteClient(fileName, vClients);
			backToMenuScreen();
			break;
		case enOptions::UpdateClient:
			system("cls");
			updateClientInfo(fileName, vClients);
			backToMenuScreen();
			break;
		case enOptions::FindClient:
			system("cls");
			findClient(fileName, vClients);
			backToMenuScreen();
			break;
		case enOptions::Exit:
			system("cls");
			exit();
			return;
		}
	}
}
// 
int main()
{
	startProgram(clientsFileName);
	system("pause");
	return 0;
}