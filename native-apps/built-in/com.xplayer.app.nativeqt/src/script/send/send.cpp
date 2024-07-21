#include <iostream>
#include <string>
using namespace std;

int main() {

    while(true) {
        int age;
        string name;

        // Đọc tuổi từ người dùng
        cout << "input your age";
        cin >> age;
        cout << "input your name";
        cin >> name;
        cout  << "your name =  " << name << ", your age =  " << age + 1 << endl;
    }

    return 0;
}