#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;

#define BINSIZE 2   // 邊界，例如 00,01,10,11 只需要2bit(從0開始算..到1，運用在char[])就可以記錄
                    //而2用來做flag(紀錄已經跟其他組的輸入比過了)
#define NUMLIST 3
#define MAX_MINTERMS 8
#define MAX_DONTCARES 8
#define SIZE 10
#define MAX_IMP 8
#define MAX 20
#define CHKCHAR 1
#define ROWSIZE BINSIZE+CHKCHAR+1
#define POSLEN 4

int nameCount = 0; //for .names node number
int g_num=0;
int g_rows=0;
int g_nodeNum=0; // count input,output node number

// for string delimiter
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

struct table{
    string inputNodename;
    string outputNodename;
    string namesNodename;
    char nodeValue[10][4];
};
struct table boolentable[20]; // save logic gate node

void test(char buffer[],char buffer3[]){
    string str = buffer3;
    string delimiter = " "; //token is space 
    vector<string> v = split (str, delimiter);

    int number = 0;
    for (auto i : v){
        
        // if keyword is .input
        if(buffer[0]=='.' && buffer[1]=='i'){
            boolentable[number].inputNodename = i;
            g_nodeNum++;
            //cout << "number ==> " << number << boolentable[number].inputNodename << endl;
        }
        // if keyword is .output
        else if(buffer[0]=='.' && buffer[1]=='o'){
            boolentable[number].outputNodename = i;
            g_nodeNum++;
            //cout << "number ==> " << number << boolentable[number].outputNodename << endl;
        }
        // if keyword is .name
        else if(buffer[0]=='.' && buffer[1]=='n'){
            boolentable[number].namesNodename = i;
            //cout << "number ==> " << number << boolentable[number].namesNodename << endl;
            nameCount++;
        }
        
        number++;
    }
}

int CompareTerms(const char *ar1, const char *ar2, int size)
{
	int i;
	int count = 0;
	int temp;
	for (i = 0; i < size; i++)
	{
		if (ar1[i] != ar2[i])
		{
			temp = i;
			count++;
		}
		else
			continue;
	}
    //只有一個bit不同才可以比較，所以count != 1 給值-1，如果=1就把那個位置[i]的最後一個空白字元給't'
	return (count == 1) ? temp : -1;
}