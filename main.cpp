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

//https://reurl.cc/Mdyayp 說明如何從command輸入./main.exe 後面加入xor.blif可以被抓到
int main(int argc, char *argv[]){

    struct table truthtable[20]={""}; // save logic value of gate node  for karnaugh map simplify
    fstream file;
    char buffer[80];
    char buffer2[80];
    char buffer3[20];
    char prime_imp[MAX_IMP][ROWSIZE] = {""};
    int bit_pos , k = 0;
    bool numflag;

    file.open(argv[1], ios::in);
    if(!file){
        cout << "file not open!!" << endl;
    }
    else{
        while(!file.eof()){
            file.getline(buffer,sizeof(buffer)); // get one line
            // clear buffer3[i] array
            for(int i=0;i<80;i++){
                buffer3[i] = ' ';
            }

            //第一輪的.name node value 讀取完給count++，就可進入此判斷是做化簡運算
            if((buffer[1]=='n' || buffer[1]=='e') && numflag){
                numflag = false;
                
                bit_pos = CompareTerms(truthtable[g_num-1].nodeValue[0], truthtable[g_num].nodeValue[0], BINSIZE);
                if (bit_pos != -1){
                    //wait for writting
                }
                else
                {
                    /*===================Combining same group=========================*/
                    if (truthtable[0].nodeValue[0][BINSIZE] != 't'){
                        truthtable[0].nodeValue[0][BINSIZE] = '*';
                    }
                    truthtable[0].nodeValue[0][BINSIZE+CHKCHAR]= '\0';
                    if (truthtable[1].nodeValue[0][BINSIZE] != 't'){
                        truthtable[1].nodeValue[0][BINSIZE] = '*';
                    }
                    truthtable[1].nodeValue[0][BINSIZE+CHKCHAR]= '\0';
                    /*===================Combining same group=========================*/
                }
                /*===================== Finding the prime implicants in list 1 ===================*/
                for(int i=0;i<g_num;i++){
                    if (strchr(truthtable[i].nodeValue[0], '*') != NULL){
	                    for (int j = 0; j < BINSIZE; j++)
	                		prime_imp[k][j] = truthtable[i].nodeValue[0][j];
	                	prime_imp[k++][BINSIZE] = '\0';
	                }
                }
                /*===================== Finding the prime implicants in list 1 ===================*/
                /*===================== Finding the eseential prime implicants ===================*/
                /*prime_imp[index][BINSIZE] = '*';
			    prime_imp[index][BINSIZE+CHKCHAR] = '\0';
			    strncpy(ess_pri_imp[p++], prime_imp[index], BINSIZE);
			    p2++;*/
                /*===================== Finding the eseential prime implicants ===================*/
                /*===================== Diplaying the function =====================*/
                for(int i=0;i<g_rows;i++){
                    if(i==0){
                        cout << "Node function:" << endl;
                        cout << boolentable[0].outputNodename << " = ";
                    }
                    for(int j=0;j<BINSIZE;j++){
                        switch(j){
                            case 0:
                                if(truthtable[i].nodeValue[0][j] == '0')
                                    cout << boolentable[0].inputNodename <<"'" ;
                                else if(truthtable[i].nodeValue[0][j] == '1')
                                    cout << boolentable[0].inputNodename << " ";
                                break;
                            case 1:
                                if(truthtable[i].nodeValue[0][j] == '0')
                                    cout << boolentable[1].inputNodename <<"'" ;
                                else if(truthtable[i].nodeValue[0][j] == '1')
                                    cout << boolentable[1].inputNodename << " ";
                                break;
                        }
                    }
                    if (i % g_rows == i && (g_rows - i) > 1)
				    {
					    putchar(' ');putchar('+');putchar(' ');
				    }
                    if(i == g_rows-1){
                        cout << endl;
                        cout << "END" <<endl;
                    }
                }
            }
            else if(buffer[1]=='m'){
                continue;
            }

            // save input value
            if(buffer[0]=='.' && buffer[1]=='i'){
                //cout << "this is input " << endl;
                sscanf(buffer, "%*[^ ] %[^\n]",buffer3);
                test(buffer,buffer3);
            }
            // save output value
            else if(buffer[0]=='.' && buffer[1]=='o'){
                //cout << "this is output " << endl;
                sscanf(buffer, "%*[^ ] %[^\n]",buffer3);
                test(buffer,buffer3);
            }
            // save .names node name
            else if(buffer[0]=='.' && buffer[1]=='n'){
                //cout << "this is name " << endl;
                sscanf(buffer, "%*[^ ] %[^\n]",buffer3);
                test(buffer,buffer3);
            }
            // save .name node value
            else{
                //cout << "this is number " << endl;
                if(buffer[0]=='.' && buffer[1]=='e') break;
                //========================================20200921============================
                sscanf(buffer, "%[^ ]",buffer3);
                //========================================20200921============================
                //原本 ==> truthtable[g_num].nodeValue[0],buffer3; 會產生 error: invalid array assignment
                //解決辦法 ==> https://reurl.cc/EzoQmk
                strcpy(truthtable[g_num].nodeValue[0],buffer3); 
                //cout << truthtable[g_num].nodeValue[0] << endl;
                g_num++;
                g_rows++;
                numflag = true;   
            } 
        }
        file.close();
    }
    system("pause");
}
