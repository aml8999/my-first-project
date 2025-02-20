#include <iostream>
#include <vector>
#include <string>

using namespace std;

// تعريف هيكل الحساب
struct Account {
    string FullName;
    long long ID;
    long long accountNumber;
    string TelephoneNumber;
    string Password;
    string Email;
    double Balance;
    bool isLoggedIn = false; // حالة تسجيل الدخول
    bool exists = false;    // هل الحساب موجود أم لا
};

// تعريف هيكل المعاملة
struct Transaction {
    string typeOf_process;
    double amount;
    double balanceAfter_process;
};

// متغير لتخزين جميع العمليات المالية
vector<Transaction> transactions;
Account client;

// توليد رقم الحساب باستخدام الهوية
long long GenerateAccountNumber(long long userID) {
    if (to_string(userID).length() != 14) {
        return 0;
    }
    long long lastFiveDigits = userID % 100000; // استخراج اخر 5 ارقام من رقم الهوية
    long long specialNumber = 5587;            // رقم مميز لاضافة التفرد
    long long accountNumber = lastFiveDigits * 10000 + specialNumber;

    return accountNumber;
}

// إنشاء حساب جديد
void CreateAccount() {
    if (client.exists) {
        cout << "An account already exists! You can't create another one .\n";
        return;
    }
    cout << "Enter Your Full Name : ";
    cin.ignore();//لنتجنب اى خطا ممكن ان يحدث بسبب الموجود فى ال input buffer
    getline(cin, client.FullName);

    do {
        cout << "Enter Your Telephone Number : ";
        cin >> client.TelephoneNumber;
        if (client.TelephoneNumber.length() != 11) {//التاكد ان رقم الهاتف يتكون من 11 رقم
            cout << "Please enter a correct telephone number\n";
        }
    } while (client.TelephoneNumber.length() != 11);

    cout << "Enter Your Email : ";
    cin >> client.Email;

    while (true) {//انشاء حلقه لا نهاءيه للتاكد ان الرقم القومى 
        cout << "Enter Your ID: ";
        cin >> client.ID;

        client.accountNumber = GenerateAccountNumber(client.ID);

        if (client.accountNumber != 0) {
            break;//انهاء الحلقه اذا كان الرقم القومى صحيح وتم انشار رقم الحساب بنجاح
        } else {
            cout << "Invalid ID! Please enter a valid 14-digit ID.\n";
        }
    }

    cout << "Enter Your Balance : ";
    cin >> client.Balance;
    while (client.Balance < 0) {//التاكد بان الحساب  رقم موجب
        cout << "Invalid Balance number, Please enter a positive number : ";
        cin >> client.Balance;
    }

    cout << "Enter Password : ";
    cin >> client.Password;
    client.exists = true;//اصبح الحساب موجودا بلفعل
    cout << "Account Created Successfully !\n";
}
//تسجيل الدخول
bool Login() {
    if (!client.exists) {//التاكد من وجود حساب للمستخدم
        cout << "No account found !\nPlease create an account first.\n";
        return false;
    }
    string pass;
    string mail;

    do {
        cout << "Enter Your Email : ";
        cin >> mail;
        cout << "Enter Password : ";
        cin >> pass;

        if (pass == client.Password && mail == client.Email) {
            cout << "Successful Login...\n";
            client.isLoggedIn = true;
            return true;
        } else {
            cout << "Your Password or Email may be Wrong!!!. Please Try again\n";
        }
    } while (true);//انشاء حلقه  لا نهاءيه تتوقف عند ادخال العميل بيانته بشكل صحيح
}
//تسجيل الخروج
void Logout() {
    cout << "Successful Logout.. Goodbye!\n";
    client.isLoggedIn = false;
}
//ايداع مبلغمن المال
double Deposit(double balance) {
    double deposit;
    cout << "Your balance is : " << balance << endl;
    cout << "Enter the amount you want to deposit : ";
    cin >> deposit;

    if (deposit >= 50 && deposit <= 30000) {//الايداع حده الادنى 50 والاقصى 30000
        balance += deposit;
        cout << "Deposit Successful!\nNew balance is : " << balance << " EGP " << endl;
        Transaction newTransaction = {"Deposit", deposit, balance};//حفظ بيانات عمليه الايداع التى قام بها المستخدم
        transactions.push_back(newTransaction);//حفظ هذه العمليه فى قاءمه العمليات الماليه لعرض سجل العمليات الماليه لاحقا
    } else {
        cout << "Invalid amount! Deposit must be greater than or equal to 50 EGP and less than or equal to 30000 EGP." << endl;
    }
    return balance;
}
//عمليه السحب
double Withdrawal(double balance) {
    double withdrawal;
    if (balance == 0) {//التاكد ان الحساب لي فارغ
        cout << "Warning!\nThere is no balance in your account";
        return balance;
    } else if (balance < 50) {// 50 التاكد ان البلغ السحوب ليس اقل من الحد الادنى للسحب 
        cout << "Warning!\nThere is less than 50 EGP in your account";
        return balance;
    }

    cout << "Your balance is : " << balance << endl;
    cout << "Enter the amount you want to withdrawal : ";
    cin >> withdrawal;

    if ((withdrawal >= 50) && (withdrawal <= balance)) {//التاكد ان المبلغ المراد سحبه موجود فى الحساب بلفعل 
        balance -= withdrawal;
        cout << "Withdrawal Successful!\nNew balance is : " << balance << " EGP " << endl;
        Transaction newTransaction = {"Withdrawal", withdrawal, balance};//حفظ بيانات عمليه السحب
        transactions.push_back(newTransaction);//حفظ العمليه فى سجل العمليات الماليه لعرض السجل لاحقا
    } else if (withdrawal < 50 || withdrawal > 30000) {
        cout << "Invalid amount! Withdrawal must be greater than or equal to 50 and less than or equal to 30000." << endl;
    } else if (withdrawal > balance) {
        cout << "You can't withdrawal more than " << balance << " EGP at a time ! " << endl;
    }
    return balance;
}
//عرض سجل العمليات الماليه
void showTransaction() {
    cout << "\n** YOUR TRANSACTION **\n";
    if (transactions.empty()) { //التحقق من وجود عمليات فى سجل المعاملات الماليه
        cout << "No transaction yet. \n";
    } else {
        for (size_t i = 0; i < transactions.size(); i++) {
            cout << transactions[i].typeOf_process << " : ";
            if (transactions[i].typeOf_process == "Deposit") {
                cout << "+" << transactions[i].amount << " | Balance after : " << transactions[i].balanceAfter_process << " EGP\n";
            } else {
                cout << "-" << transactions[i].amount << " | Balance after : " << transactions[i].balanceAfter_process << " EGP\n";
            }
        }
    }
    cout << "\n";
}
//عرض تفاصيل الحساب 
void ViewAccountDetails() {
    if (!client.exists) {
        cout << "No account found !\nPlease create an account first.\n";
        return;
    }
    cout << "Your FullName: " << client.FullName << endl;
    cout << "Your Email: " << client.Email << endl;
    cout << "Your Balance: " << client.Balance << " EGP" << endl;
    cout << "Your ID: " << client.ID << endl;
    cout << "Your Account Number: " << client.accountNumber << endl;
}
//اغلاق الحساب
void account_closure() {
    if (!client.exists) {//التاكد ان الحساب موجود
        cout << "No account found !\nPlease create an account first.\n";
        return;
    }
    string pass, mail;
    do {
        cout << "Please Enter your email to close your account : ";
        cin >> mail;
        cout << "Enter your password : ";
        cin >> pass;

        if (mail == client.Email && pass == client.Password) {
            break;
        } else {
            cout << "Your Password or Email may be Wrong!!!. Please Try again\n";
        }
    } while (true);

    int choice;
    cout << "Do you want to close your account ?\n";
    cout << "1-Yes\n";
    cout << "2-No\n";
    cin >> choice;
    if (choice == 1) {
        client = {"", 0, 0, "", "", "", 0.0, false, false};// ازاله كل بيانات العميل عند قفل الحساب 
        transactions.clear();//ازاله جميع المليات الماليه من سجل المعاملات الماليه
        cout << "Your account has been closed successfully!\n";
    } else {
        cout << "Account closure process has been cancelled.\n";
    }
}
//اختيار العمليه التى يرغب  العميل فى القيام بها
void TransactionMenu() {
    int choice;
    do {
        cout << "\n** TRANSACTION MENU **\n";
        cout << "1: Deposit\n";
        cout << "2: Withdrawal\n";
        cout << "3: Show Transactions\n";
        cout << "4: Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            client.Balance = Deposit(client.Balance);
            break;
        case 2:
            client.Balance = Withdrawal(client.Balance);
            break;
        case 3:
            showTransaction();
            break;
        case 4:
            Logout();
            return;
        default:
            cout << "Invalid choice! Please enter a number between 1 and 4.\n";
        }
    } while (client.isLoggedIn);//السماح للعميل بلقيام بلعمليات الماليه اذا كان مسجل الدخول 
}

int main() {
    int choice;
    do {
        cout << "\n** MAIN MENU **\n";
        cout << "1: Create Account\n";
        cout << "2: View account details\n";
        cout << "3: Login\n";
        cout << "4: Close Account\n";
        cout << "5: Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            CreateAccount();
            break;
        case 2:
            ViewAccountDetails();
            break;
        case 3:
            if (Login()) {
                TransactionMenu();
            }
            break;
        case 4:
            account_closure();
            break;
        case 5:
            cout << "Thank you for using our system bank.\n";
            break;
        default:
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}

