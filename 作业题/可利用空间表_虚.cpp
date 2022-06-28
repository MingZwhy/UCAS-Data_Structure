#include<iostream>

using namespace std;

int main(){
    int low,high,n;
    cin >> low;
    cin.get();
    cin >> high;
    cin.get();
    cin >> n;
    cin.get();

    int index=0;
    char c;
    int size=0;
    int count=0;
    int tag;
    bool first=false;
    bool none=true;
    bool init=true;

    while(c!='\n'){
        cin >> tag;
        if(tag==0){
            count++;
            first=true;
            none=false;
        }
        else{
            if(!first){
                low=low+n;
            }
            else{
                size=size + n*count;
                if(init){
                    cout << "0 " << low  << " " << size;
                    init=false;
                }
                else{
                    cout << endl << "0 " << low  << " " << size;
                }
                low=low+n*(count+1);
                index++;
                size=0;
                count=0;
                first=false;
            }
        }
        c=cin.get();
    }
    if(first){
        size=size + n*count;
        if(init){
            cout << "0 " << low  << " " << size;
            init=false;
        }
        else{
            cout << endl << "0 " << low  << " " << size;
        }
    }
    if(none){
        cout << "0 0 0";
    }
    return 0;
}