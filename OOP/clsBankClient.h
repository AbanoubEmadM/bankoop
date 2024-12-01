#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "clsPerson.h";
#include "clsString.h";

using namespace std;
class clsBankClient : public clsPerson
{
private:
	enum enMode {EmptyMode = 0, UpdateMode = 1, AddNewMode = 2};
	enMode _Mode; // declare variable of type enMode

	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	bool _MarkedForDeletion = false;

	static clsBankClient _ConvertLineToObject(string Line, string Separator = "#//#")
	{
		vector<string> vClientData = clsString::Split(Line, Separator);

		return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
			vClientData[3], vClientData[4], vClientData[5], stod(vClientData[6]));
	}

	static string _ConvertObjectToLine(clsBankClient Client, string Separator = "#//#")
	{
		string Line;

		Line += Client.FirstName + Separator;
		Line += Client.LastName + Separator;
		Line += Client.Email + Separator;
		Line += Client.Phone + Separator;
		Line += Client.AccountNumber() + Separator;
		Line += Client.PinCode + Separator;
		Line += to_string(Client.AccountBalance);

		return Line;
	}

	static  vector <clsBankClient> _LoadClientsDataFromFile()
	{

		vector <clsBankClient> vClients;

		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{

			string Line;


			while (getline(MyFile, Line))
			{

				clsBankClient Client = _ConvertLineToObject(Line);

				vClients.push_back(Client);
			}

			MyFile.close();

		}

		return vClients;

	}

	void _AddLineToFile(string Line)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out | ios::app);

		if (MyFile.is_open())
		{
			MyFile << Line << endl;
		}
		MyFile.close();
	}

	static void _SaveClientsDataToFile(vector <clsBankClient> vClients)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);//overwrite

		string DataLine;

		if (MyFile.is_open())
		{
			for (clsBankClient C : vClients)
			{
				if (C.MarkedForDeletion() == false)
				{
					DataLine = _ConvertObjectToLine(C);
					MyFile << DataLine << endl;
				}
			}

			MyFile.close();

		}

	}

	static clsBankClient _GetEmptyClientObject()
	{
		return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
	}

	void _Update()
	{
		vector<clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClient& C : _vClients)
		{
			if (C.AccountNumber() == AccountNumber());
			{
				// *this is the updated client
				C = *this;
			}
		}
		_SaveClientsDataToFile(_vClients);
	}

	void _AddNew()
	{
		_AddLineToFile(_ConvertObjectToLine(*this));
	}

public:
	clsBankClient(enMode Mode, string FirstName, string LastName,
		string Email, string Phone, string AccountNumber, string PinCode,
		float AccountBalance) :
		clsPerson(FirstName, LastName, Email, Phone)

	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
		_MarkedForDeletion = false;
	}

	string AccountNumber()
	{
		return _AccountNumber;
	}

	void SetPinCode(string PinCode)
	{
		_PinCode = PinCode;
	}

	bool MarkedForDeletion()
	{
		return _MarkedForDeletion;
	}

	bool Delete()
	{
		vector <clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClient& C : _vClients)
		{
			// _AccountNumber means the current number of current object 
			if (C.AccountNumber() == _AccountNumber)
			{
				C._MarkedForDeletion = true;
				break;
			}
		}
		_SaveClientsDataToFile(_vClients);

		*this = _GetEmptyClientObject();

		return true;
	}

	string GetPinCode()
	{
		return _PinCode;
	}
	__declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

	void SetAccountBalance(float AccountBalance)
	{
		_AccountBalance = AccountBalance;
	}

	bool IsEmpty()
	{
		return (_Mode == enMode::EmptyMode);
	}

	static bool IsClientExist(string AccountNumber)
	{
		 clsBankClient Client = clsBankClient::Find(AccountNumber);
		 return (!Client.IsEmpty());
	}

	float GetAccountBalance()
	{
		return _AccountBalance;
	}
	__declspec(property(get = GetAccountBalance, put = SetAccountBalance)) float AccountBalance;

	void Print()
	{
		cout << "\nClient Card:";
		cout << "\n___________________";
		cout << "\nFirstName   : " << FirstName;
		cout << "\nLastName    : " << LastName;
		cout << "\nFull Name   : " << FullName();
		cout << "\nEmail       : " << Email;
		cout << "\nPhone       : " << Phone;
		cout << "\nAcc. Number : " << _AccountNumber;
		cout << "\nPassword    : " << _PinCode;
		cout << "\nBalance     : " << _AccountBalance;
		cout << "\n___________________\n";

	}

	static clsBankClient Find(string AccountNumber)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClient Client = _ConvertLineToObject(Line);

				if (Client.AccountNumber() == AccountNumber)
				{
					MyFile.close();
					return Client;
				}
			}
			MyFile.close();
		}
		return _GetEmptyClientObject();
	}

	enum enSaveResult {svFalid = 0, svSucceseded = 1};

	// we use mode to adjust the ui or to choose what to show to user according to modes
	enSaveResult Save()
	{
		switch (_Mode)
		{
		case enMode::EmptyMode:
			return enSaveResult::svFalid;

		case enMode::UpdateMode:
		{
			_Update();
			return enSaveResult::svSucceseded;
		}

		case enMode::AddNewMode:
		{
			if (clsBankClient::IsClientExist(_AccountNumber))
			{
				return enSaveResult::svFalid;
			}
			else
			{
				//here we set mode add new
				_AddNew();
				//set mode update after add new
				_Mode = enMode::UpdateMode;
				return enSaveResult::svSucceseded;
			}
		}
		}
	}

	static clsBankClient GetNewClient(string AccountNumber)
	{
		return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
	}

	static vector <clsBankClient> GetClientsList()
	{
		return _LoadClientsDataFromFile();
	}
	static float GetTotalBalances()
	{
		vector <clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();
		float Total = 0;

		for (clsBankClient& C : _vClients)
		{
			Total += C.AccountBalance;
		}
		return Total;
	}
};

