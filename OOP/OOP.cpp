#include <iostream>
#include <string>
#include "clsBankClient.h";
#include "clsUtil.h";
#include "clsInputValidate.h";
#include <iomanip>

using namespace std;

//UI not with object
void ReadClientInfo(clsBankClient& Client)
{
    cout << "\nEnter FirstName: ";
    Client.FirstName = clsInputValidate::ReadString();

    cout << "\nEnter LastName: ";
    Client.LastName = clsInputValidate::ReadString();

    cout << "\nEnter Email: ";
    Client.Email = clsInputValidate::ReadString();

    cout << "\nEnter Phone: ";
    Client.Phone = clsInputValidate::ReadString();

    cout << "\nEnter PinCode: ";
    Client.PinCode = clsInputValidate::ReadString();

    cout << "\nEnter Account Balance: ";
    Client.AccountBalance = clsInputValidate::ReadFloatNumber();
}

void UpdateClient()
{
    string AccountNumber = clsInputValidate::ReadString();

    while (!clsBankClient::IsClientExist(AccountNumber))
    {
        cout << "Not Found, Account Number: ";
        string AccountNumber = clsInputValidate::ReadString();
    }

    clsBankClient Client = clsBankClient::Find(AccountNumber);
    Client.Print();
    ReadClientInfo(Client);

    clsBankClient::enSaveResult SaveResult;

    SaveResult = Client.Save();

    switch (SaveResult)
    {
    case clsBankClient::enSaveResult::svSucceseded:
    {
        cout << "\nAccount Updated Successfully :-)\n";
        Client.Print();
        break;
    }
    case clsBankClient::enSaveResult::svFalid:
    {
        cout << "\nError account was not saved because it's Empty";
        break;
    }
    }
}

void AddNewClient()
{
    string AccountNumber = clsInputValidate::ReadString();

    while (clsBankClient::IsClientExist(AccountNumber))
    {
        cout << "This Client Already Exists\n";
        AccountNumber = clsInputValidate::ReadString();
    }

    clsBankClient NewClient = clsBankClient::GetNewClient(AccountNumber);// here we set mode to add new

    ReadClientInfo(NewClient);

    clsBankClient::enSaveResult SaveResult;

    SaveResult = NewClient.Save();

    switch (SaveResult)
    {
    case  clsBankClient::enSaveResult::svSucceseded:
    {
        cout << "\nAccount Addeded Successfully :-)\n";
        NewClient.Print();
        break;
    }
    case clsBankClient::enSaveResult::svFalid:
    {
        cout << "\nError account was not saved because it's Empty";
        break;

    }
    }
}

void DeleteClient()
{
    string AccountNumber = clsInputValidate::ReadString();

    while (!clsBankClient::IsClientExist(AccountNumber))
    {
        cout << "This Client not Exists\n";
        AccountNumber = clsInputValidate::ReadString();
    }

    clsBankClient Client = clsBankClient::Find(AccountNumber);

    if (Client.Delete())
    {
        cout << "Deleted Successfully";
        Client.Print();
    }
    else
    {
        cout << "Deletion Falid!";
    }
}

void PrintClientRecordLine(clsBankClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber();
    cout << "| " << setw(20) << left << Client.FullName();
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(20) << left << Client.Email;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(clsBankClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber();
    cout << "| " << setw(40) << left << Client.FullName();
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowClientsList()
{

    vector <clsBankClient> vClients = clsBankClient::GetClientsList();

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(20) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(20) << "Email";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (clsBankClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <clsBankClient> vClients = clsBankClient::GetClientsList();

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = clsBankClient::GetTotalBalances();

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (clsBankClient Client : vClients)
        {
            PrintClientRecordBalanceLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances << endl;
    cout << "\t\t\t\t\t   ( " << clsUtil::NumberToText(TotalBalances) << ")";
}

int main()
{
    ShowTotalBalances();
}

