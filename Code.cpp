#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;


int checkRank(char opr){
    if (opr == '+'){
        return 1;
    } else if(opr == '-'){
        return 1;
    } else if(opr == '*'){
        return 2;
    } else if(opr == '/'){
        return 2;
    } else {
        return 0;
    }
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

string convert(string operation){
    stack<char> temp;
    int len = operation.length();
    string result = "";

    for (int i = 0; i < len; i++) {
        char c = operation[i];

        if (isdigit(c)) { //cek angka
            int j = i;
            while(j < len && isdigit(operation[j])){
                result += operation[j];
                j++;
            }
            result += ";";
            i = j-1;
        } else if (isOperator(c)) { //cek operator
            if (temp.empty() || temp.top() == '(') {
                temp.push(c);
            } else {
                while (!temp.empty() && checkRank(temp.top()) >= checkRank(c) ) {
                    result += temp.top();
                    temp.pop();
                }
                temp.push(c);
            }
        } else if (c == '(') { //cek kurung buka
            temp.push(c);
        } else if (c == ')') { // cek kurung tutup
            while (!temp.empty() && temp.top() != '(') {
                result += temp.top();
                temp.pop();
            }
            if (!temp.empty() && temp.top() == '(') {
                temp.pop();
            } else { //return -1 untuk error dalam tanda kurung
                throw -1;
            }
        } else if(c !='\n'){ // return -1 untuk error karakter tidak valid selain garis baru
            throw -1;
        }
    }

    while (!temp.empty()) {
        if (temp.top() == '(') { // return -1 untuk error dalam tanda kurung
            throw -1;
        }
        result += temp.top();
        temp.pop();
    }

    return result;
}

double evaluate(string operation){
    stack<double> temp;
    int len = operation.length();
    for(int i = 0; i < len; i++){
        char c = operation[i];

        if(isdigit(c)){
            string temp_angka;
            int j = i;
            while(j<len && isdigit(operation[j])){
                temp_angka += operation[j];
                j++;
            }
            temp.push(stoi(temp_angka));
            i = j;

        } else if(isOperator(c) && temp.size() > 1){
            double angka1 = temp.top();
            temp.pop();
            double angka2 = temp.top();
            temp.pop();

            switch(c){
                case '+':
                    temp.push(angka2 + angka1);
                    break;

                case '-':
                    temp.push(angka2 - angka1);
                    break;
                
                case '*':
                    temp.push(angka2 * angka1);
                    break;
                
                case '/':
                    temp.push(angka2 / angka1);
                    break;
            }
        } else if(isOperator(c) && temp.size() < 2){
            double angka = temp.top();
            temp.pop();
            
            switch (c)
            {
            case '+':
                temp.push(angka);
                break;
            case '-':
                temp.push(0-angka);
                break;
            case '/':
                throw -1;
                break;
            case '*':
                throw -1;
                break;
            }
        }
    }

    return temp.top();
}

int main(){
    ifstream file;

    system("cls");
    file.open("teks.txt");

    string temp;
    if (file.is_open()){
        bool kosong = true;
        while(getline(file,temp,';')){
            if (!temp.empty()){
                kosong = false;
                try {
                    cout << temp << " << postfix >>  " << convert(temp) << "   = " << evaluate(convert(temp)) << endl;
                } catch (int error){
                    cout << "Invalid error: " << error << endl;
                }
                
            } 
        }
        if (kosong){
            cout << "file kosong!";
            return -1;
        }
    } else {
        cout << "error ketika membuka file" << endl;
    }

    file.close();
    cin.get();
    return 0;
}
