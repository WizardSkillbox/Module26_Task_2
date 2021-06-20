#include <iostream>
#include <string>
#include <map>

using namespace std;

class Record {
    string userName;
public:
    explicit Record(const string &userName) {
        this->userName = userName;
    }

    const string *getUserName() const {
        return &userName;
    }
};

class AddressBook {
    map<string, const Record *> book;

public:
    ~AddressBook() {
        for (auto &it : book) {
            delete it.second;
        }
        book.clear();
    }

    void add(const string &phone, const Record *record) {
        book[phone] = record;
    }

    const Record *findRecord(const string &phone) const {
        auto it = book.find(phone);
        if (it != book.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool findPhone(const string &contactName, string &phone) const {
        bool res = false;
        for (const auto &it : book) {
            if (contactName == *(it.second->getUserName())) {
                phone = it.first;
                res = true;
                break;
            }
        }
        return res;
    }
};

class MobilePhone {
    AddressBook *addressBook;

    void getStringFromStream(const string &message, string &str) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << message << endl;
        cin >> str;
    }

    bool isPhone(const string &str) {
        if (str.length() != 12 || str[0] != '+' || str[1] != '7') {
            return false;
        }

        for (size_t i = 2; i < str.length(); ++i) {
            if (str[i] < '0' || str[i] > '9') {
                return false;
            }
        }
        return true;
    }

    void getPhoneFromUser(string &phone) {
        bool isCorrect = false;
        while (!isCorrect) {
            getStringFromStream("Enter phone number (+7XXXXXXXXXX):", phone);
            isCorrect = isPhone(phone);
            if (!isCorrect) {
                cout << "Error! IncorrectPhone!" << endl;
            }
        }
    }

    bool makeAction(const string &action) {
        bool res = false;
        string callRequest;
        getStringFromStream("Enter contact name or phone number", callRequest);

        cout << action << ": " << endl;

        if (isPhone(callRequest)) {
            cout << callRequest << endl;
            const Record *record = addressBook->findRecord(callRequest);
            if (nullptr == record) {
                cout << "Unknown user" << endl;
            } else {
                cout << *(record->getUserName()) << endl;
            }
            res = true;
        } else {
            string phone;
            if (addressBook->findPhone(callRequest, phone)) {
                cout << phone << endl;
                cout << callRequest << endl;
                res = true;
            } else {
                cout << "Unable to make action. Contact is not found" << endl;
            }
        }
        return res;
    }

public:

    MobilePhone() {
        addressBook = new AddressBook();
    }

    ~MobilePhone() {
        delete addressBook;
    }

    void addRecord() {
        string userName, phone;
        getStringFromStream("Enter contact name:", userName);
        getPhoneFromUser(phone);
        auto newRecord = new Record(userName);
        addressBook->add(phone, newRecord);
    }

    void call() {
        makeAction("CALL");
    }

    void sms() {
        if (makeAction("SMS")) {
            string text;
            getStringFromStream("Enter sms message: ", text);
            cout << "Sent" << endl;
        }
    }
};

int main() {
    string cmd;

    auto mobilePhone = new MobilePhone();

    while (true) {
        cout << "Enter command (add|call|sms|exit): " << endl;
        cin >> cmd;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (cmd == "add") {
            mobilePhone->addRecord();
        } else if (cmd == "call") {
            mobilePhone->call();
        } else if (cmd == "sms") {
            mobilePhone->sms();
        } else if (cmd == "exit") {
            break;
        } else {
            cout << "Error. Incorrect command" << endl;
        }
    }
    delete mobilePhone;
}
