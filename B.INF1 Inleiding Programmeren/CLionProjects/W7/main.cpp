#include <iostream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

int computeSom(vector<int> values){
    int sum = 0;
    for (int i = 0; i < values.size(); i++) {
        sum+=values[i];
    }
    return sum;
}
void printNames(vector<string> names) {
    for (string name:names) {
        cout << name << endl;
    }
}
void removeFirstAndPrint(vector<string> names){
    names.erase(names.begin());
    for (vector<string>::iterator it = names.begin(); it != names.end(); it++) {
        cout << *it << endl;
    }
}
void printContacts(map<int,string> contacts){
    for (map<int,string>::iterator it = contacts.begin(); it != contacts.end(); it++) {
        cout << it->second << ": " << it->first << endl;
    }
}
void removeAndPrint(map<int,string> contacts, int telToBeRemoved){
    contacts.erase(telToBeRemoved);
    for (auto contact:contacts) {
        cout << contact.second << ": " << contact.first << endl;
    }
}
map<string,int> createFreqTab(vector<string> woorden){
    map<string,int> freqtab;
    for (string woord:woorden) {
        freqtab[woord]++;
    }
    return freqtab;
}
void printFreqTab(map<string,int> freqtab){
    for (auto freq:freqtab) {
        cout << freq.first << " " << freq.second << endl;
    }
}
string vecToString(vector<int> v1) {
    string result = "[";
    for (int i = 0; i < v1.size(); i++) {
        result += to_string(v1[i]);
        result += ", ";
    }
    result.pop_back();
    result.pop_back();
    result += "]";
    return result;
}
vector<int> addVector(vector<int> v1, int x) {
    for (int i=0; i < v1.size(); i++) {
        v1[i]+=x;
    }
    return v1;
}
vector<int> addVectors(vector<int> v1, vector<int> v2) {
    if (v1.size() != v2.size()) {
        return {-1};
    }
    vector<int> result;
    for (int i=0; i < v1.size();i++) {
        result.push_back(v1[i]+v2[i]);
    }
    return result;
}
float normVector(vector<int> v1) {
    float sum_of_squares = 0;
    for (int element : v1) {
        sum_of_squares += pow(element, 2);
    }
    return sqrt(sum_of_squares);
}
void printTextRectangleFilled(int width, int height) {
    string line;
    for(int i=0; i < width; i++) {
        line+="*";
    }
    for(int i=0; i < height; i++) {
        cout<<line<<endl;
    }
}
void printTextRectangleOutline(int width, int height) {
    string line;
    for(int i=0; i < width; i++) {
        line+="*";
    }
    string centerline;
    if(width != 0) {
        centerline+="*";
    }
    if(width>2) {
        for (int i = 0; i < width-2; i++) {
            centerline+=" ";
        }
        centerline+="*";
    }
    if(height>=1) {
        cout<<line<<endl;
    }
    if(height>=2) {
        for(int i=0; i < height-2;i++ ) {
            cout<<centerline<<endl;
        }
    }
    if(height>=2) {
        cout<<line<<endl;
    }
}
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
bool isPrime2(int n) {
        /* Pseudocode:
        function is_prime(n)
            if n <= 1
                return false
            else if n <= 3
                return true
            else if n mod 2=0 or n mod 3=0
                return false
            i = 5
            while i * i <= n
                if n mod i = 0 or n mod (i + 2) = 0
                    return false
                i = i + 6
            return true
        */
    if (n<=1) {
        return false;
    }
    else if (n<=3){
        return true;
    }
    else if (n % 2 == 0 | n % 3 == 0) {
        return 0;
    }
    int i = 5;
    while (i*i <= n) {
        if (n % i == 0 | n % (i+2) == 0) {
            return false;
        }
        i = i + 6;
        return true;
    }
}
vector<vector<int>> upper_triangle(vector<vector<int>>& matrix) {
    for (int row = 0; row < matrix.size(); row++) {
        for (int col = 0; col < matrix[row].size(); col++) {
            if (col < row) {
                matrix[row][col] = 0;
            }
        }
    }
    return matrix;
}

ostream operator<<(const ostream & lhs, const vector<vector<int>> & rhs);
int main(){
    cout << "==Oefening 1==" << endl;
    cout << computeSom({12, 10, 32, 3, 66, 17, 42, 99, 20, 1}) << endl;
    cout << "==Oefening 2==" << endl;
    printNames({"Toon", "Tom", "Daphne", "Wannes", "Stephen"});
    cout << "==Oefening 4==" << endl;
    removeFirstAndPrint({"Stephen","Toon", "Tom", "Daphne", "Wannes"});
    cout << "==Oefening 5==" << endl;
    printContacts({{123, "Toon"}, {789, "Daphne"}, {456, "Tom"} , {159, "Stephen"} });
    cout << "==Oefening 6==" << endl;
    removeAndPrint({{123, "Toon"}, {789, "Daphne"}, {456, "Tom"} },456);
    cout << "==Oefening 7==" << endl;
    map<string,int> freqtab = createFreqTab({"dit","is","een","dit","een","dit","hallo","dit"});
    cout << freqtab["dit"] << endl;
    cout << freqtab["is"] << endl;
    cout << "==Oefening 8==" << endl;
    printFreqTab(freqtab);
    cout << "==Oefening 9==" << endl;
    vector<int> v1 = {1, 2, 3, 4, 5, 1};
    vector<int> v2 = {6, 7, 8, 9, 10, 2};
    cout<<vecToString(v1)<< endl;
    cout<<vecToString(v2)<< endl;
    cout << "==Oefening 10==" << endl;
    vector<int> v33 = addVector(v1, 4);
    vector<int> v43 = addVector(v2, -2);
    cout<<vecToString(v33)<< endl;
    cout<<vecToString(v43)<< endl;
    cout<< "==Oefening 11=="<< endl;
    vector<int> v3 = {-1, 1, 0, 2, 100, 3};
    vector<int> v4 = addVectors(v1, v2);
    vector<int> v5 = addVectors(v2, v1);
    vector<int> v6 = addVectors(v1, v3);
    cout<<vecToString(v4)<< endl;
    cout<<vecToString(v5)<< endl;
    cout<<vecToString(v6)<< endl;
    cout<< "==Oefening 12=="<< endl;
    cout<<normVector(v1)<< endl;
    cout<<normVector(v2)<< endl;
    cout<<normVector(v3)<< endl;
    cout<<"==Oefening 13=="<<endl;
    printTextRectangleFilled(5, 6);
    cout<<"==Oefening 14=="<<endl;
    printTextRectangleOutline(5, 6);
    cout<<"==Oefening 15=="<<endl;
    cout<<isPrime(1)<<endl;
    cout<<isPrime(2)<<endl;
    cout<<isPrime(18)<<endl;
    cout<<isPrime(29)<<endl;
    cout<<"==Oefening 17=="<<endl;
    cout<<"Working on INGInious but not in CLion for some reason..."<<endl;
    // vector<vector <int>> matrix = {{10,11,12},{13,14,15},{-1,17,18}};
    // cout<<to_string(upper_triangle(matrix))<<endl;
    // vector<vector <int>> matrixB = {{1,2,3},{4,5,6},{7,8,1}};
    // cout<<to_string(upper_triangle(matrixB))<<endl;
    // vector<vector <int>> matrixC = {{1,2},{3,4}};
    // cout<<to_string(upper_triangle(matrixC))<<endl;
    return 0;
}
