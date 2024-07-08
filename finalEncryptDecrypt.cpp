#include <bits/stdc++.h>
#include <thread>
#include <chrono> //For system_clock
#include <random>
#include <unistd.h>
using namespace std;
#define ll long long

class EncryptDecrypt
{

        string encryptedMessage;
        // string key;
        map<char, int> mp;
        vector<pair<int, int>> v;
        vector<int> ncVector;

public:
    string getData()
    {
        string s;
        cout<<"\nEnter the message you want to encrypt\n";
        getline(cin,s);
        return s;
    }
    
    // function for converting the string into bits

    string to_bits(int n)
    {
        string s = "";
        while (n)
        {
            s += to_string(n % 2);
            n /= 2;
        }
        reverse(s.begin(), s.end());
        if (s.size() < 7)
        {
            ll a = 7 - s.size();
            string t = "";
            for (int i = 0; i < a; i++)
            {
                t += "0";
            }
            return t + s;
        }
        return s;
    }

    // function for generating random number

    int randoms(int lower, int upper)
    {
        return rand() % (upper - lower + 1) + lower;
    }

    // code for adding noise

    string noiseAdder(string s)
    {
        vector<int> vtemp;
        for (int i = 0; i < 30; i++)
        {
            vtemp.push_back(randoms(1, 5));
        }

        int j = 0, l = s.length(), i = -1;
        for (int k = 0; k < l; k++)
        {
            i = (i + vtemp[j] + 1) % s.length();
            char s2 = char(i % 128);
            string s1 = s.substr(0, i);
            string s3 = s.substr(i);
            s = s1 + s2 + s3;
            j = (j + 1) % vtemp.size();
        }
        ncVector.push_back(i);
        for (int k = 0; k < 30; k++)
        {
            ncVector.push_back(vtemp[k]);
        }
        ncVector.push_back(j);
        return s;
        
    }
    
    // function for removing noise

    string noiseRemover(string s)
    {
        vector<int> vTemp;
        for (int k = 0; k < 30; k++)
        {
            vTemp.push_back(ncVector[k + 1]);
        }
        int i = ncVector[0], l = s.length() / 2, j = ncVector[ncVector.size() - 1];
        j = (j + vTemp.size() - 1) % vTemp.size();

        for (int k = 0; k < l; k++)
        {
            string s1 = s.substr(0, i);
            string s2 = s.substr(i + 1);
            s = s1 + s2;
            i = (i + s.length() - vTemp[j] - 1) % s.length();
            j = (j + vTemp.size() - 1) % vTemp.size();
        }
        return s;

    }

    // function for encrypting the message

    void encrypt()
    {

        string s= getData();
        const int arraySize = 96;

        char charArray[arraySize];
        for (int i = 0; i < arraySize; i++)
        {
            charArray[i] = i + 32;
        }
        // shuffle the array randomly
        for (int i = 0; i < arraySize; i++)
        {
            int randomIndex = rand() % arraySize; // generate a random index
            char temp = charArray[i];             // swap the current element with the randomly selected element
            charArray[i] = charArray[randomIndex];
            charArray[randomIndex] = temp;
        }

        for (int i = 0; i < 96; i++)
            mp[charArray[i]] = i;

        for (int i = 0; i < s.size(); i++)
        {
            int p = (int)s[i] - 32;
            s[i] = charArray[p];
        }
        string a = "", res = "";

        // String to Binary
        for (int i = 0; i < s.length(); i++)
        {
            a += to_bits(int(s[i]));
        }
        // Creating block size and shift value
        int value1, value2;
        for (int i = 0; i < 31; i++)
        {
            if (i < 5)
            {
                value1 = randoms(2, 15);    // block
                value2 = randoms(1, 9) % value1;    // shift
                if (value2 == 7)
                    value2 = 6;
                else if (value2 == 0)
                    value2 = value1 / 2;
                v.push_back({value1, value2});
                continue;
            }
            value1 = randoms(2, 99);    // block
            value2 = randoms(1, 9) % value1;    // shift
            if (value2 == 7)
                value2 = 6;
            else if (value2 == 0)
                value2 = value1 / 2;
            v.push_back({value1, value2});
        }
        int n = a.size(), temp = n;

        // Implementing shifting
        int j = 0;
        for (int i = 0; i < a.size();)
        {
            string t;
            if (temp < v[j].first)
            {
                value1 = temp;
                value2 = randoms(1, 9) % value1;
                if (value2 == 7)
                    value2 = 6;
                else if (value1 == 1)
                    value2 = 1;
                else if (value2 == 0)
                    value2 = value1 / 2;
                v.push_back({value1,value2});
                t = a.substr(i, v[31].first);
                reverse(t.begin(), t.begin() + v[31].second);
                reverse(t.begin() + v[31].second, t.end());
                reverse(t.begin(), t.end());
                a.replace(i, v[31].first, t);
                break;
            }
            t = a.substr(i, v[j].first);
            reverse(t.begin(), t.begin() + v[j].second);
            reverse(t.begin() + v[j].second, t.end());
            reverse(t.begin(), t.end());
            temp -= v[j].first;
            a.replace(i, v[j].first, t);
            i += v[j].first;
            j = (j + 1) % 31;
        }

        // Binary to String
        for (int i = 0; i < a.size(); i += 7)
        {
            string t = a.substr(i, 7);
            int temp = 0;
            for (int j = 6; j >= 0; j--)
            {
                temp += ((t[j] - '0') * pow(2, 6 - j));
            }
            res += char(temp);
        }
        string codedMessage=noiseAdder(res);
        encryptedMessage=codedMessage;
    }

    // function for decrypting the encrypted the code

    string decrypt()
    {
        string se=encryptedMessage;
        string s =noiseRemover(se);
        string a = "";
        // encrypted string to binary

        for (int i = 0; i < s.length(); i++)
        {
            a += to_bits(int(s[i]));
        }
        // Implementing shifting
        int n = a.size(), temp = n;
        int j = 0;
        for (int i = 0; i < a.size();)
        {
            string t;
            if (temp < v[j].first)
            {
                t = a.substr(i, v[31].first);
                reverse(t.begin(), t.begin() + (v[31].first - v[31].second));
                reverse(t.begin() + (v[31].first - v[31].second), t.end());
                reverse(t.begin(), t.end());
                a.replace(i, v[31].first, t);
                break;
            }
            t = a.substr(i, v[j].first);
            reverse(t.begin(), t.begin() + (v[j].first - v[j].second));
            reverse(t.begin() + (v[j].first - v[j].second), t.end());
            reverse(t.begin(), t.end());
            temp -= v[j].first;
            a.replace(i, v[j].first, t);
            i += v[j].first;
            j = (j + 1) % 31;
        }
        string res;
        for (int i = 0; i < a.size(); i += 7)
        {
            string t = a.substr(i, 7);
            int temp = 0;
            for (int j = 6; j >= 0; j--)
            {
                temp += ((t[j] - '0') * pow(2, 6 - j));
            }
            res += char(temp);
        }

        for (int i = 0; i < res.size(); i++)
        {
            res[i] = int(mp[res[i]] + 32);
        }
        return res;
    }

    // function for printing the encrypted message

    void putData()
    {
        cout<<"\nEncrypted text is below : \n"<<encryptedMessage<<"\n---------------------------------------------\n";
    }

};
void print15(char ch)
{
    for(int i=0;i<15;i++)
        cout<<ch;
}

int main()
{
    srand(time(0));
    cout<<"\n\n";
    EncryptDecrypt e;
    cout<<'\n';
    print15('-');print15('-');print15('-');print15('-');print15('-');print15('-');
    e.encrypt();
    print15('-');print15('-');print15('-');
    e.putData();
    cout<<"\nDecrypted text is below :\n";
    cout<<e.decrypt()<<endl;
    print15('-');print15('-');print15('-');print15('-');print15('-');print15('-');
    cout<<"\n";
    
    return 0;
}