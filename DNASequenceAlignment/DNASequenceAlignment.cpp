#include <iostream>
#include <sstream>
#include <vector>



using namespace std;

struct Penalty {
    int value;
    int whichCase;
    struct Penalty * fromWhom;
};




int main() {

    std::string sequenceX,sequenceY;

    std::vector <std::vector <struct Penalty>> table;

    int gap,mismatch;

    getline(std::cin,sequenceX);
    getline(std::cin,sequenceY);
    cin>>gap>>mismatch;



    for (int j = 0; j <= sequenceX.size() ; j++){
        std::vector <struct Penalty> column;
        for (int t = 0; t<=sequenceY.size(); t++){

            struct Penalty penalty;
            column.push_back(penalty);}
        table.push_back(column);
    }

    for (int j = 0; j <= sequenceX.size() ; j++){
        for (int t = 0; t<=sequenceY.size(); t++) {
            if (j != 0 && t == 0) {
                table.at(j).at(t).value = table.at(j - 1).at(0).value + gap;
                table.at(j).at(t).whichCase = 2;
                table.at(j).at(t).fromWhom = &table.at(j - 1).at(0);
            } else if (j == 0 && t != 0) {
                table.at(j).at(t).value = table.at(0).at(t - 1).value + gap;
                table.at(j).at(t).whichCase = 3;
                table.at(j).at(t).fromWhom = &table.at(0).at(t - 1);
            } else {
                table.at(j).at(t).value = 0;
                table.at(j).at(t).whichCase = 0;
                table.at(j).at(t).fromWhom = nullptr;
            }
        }
    }


    for (int k = 1; k <= sequenceX.size(); k++){
        for (int l = 1; l <= sequenceY.size(); l++){

            int a = mismatch;
            if(sequenceX.at(k-1) == sequenceY.at(l-1)) a = 0;

            if ((table.at(k-1).at(l-1).value + a <= table.at(k).at(l-1).value + gap)
                && (table.at(k-1).at(l-1).value + a <= table.at(k-1).at(l).value + gap)){

                table.at(k).at(l).value = table.at(k-1).at(l-1).value + a;
                table.at(k).at(l).whichCase = 1;
                table.at(k).at(l).fromWhom = & table.at(k-1).at(l-1);

            }
            else if ((table.at(k-1).at(l-1).value + a >= table.at(k).at(l-1).value + gap)
                && (table.at(k).at(l-1).value + gap <= table.at(k-1).at(l).value + gap)){

                table.at(k).at(l).value = table.at(k).at(l-1).value + gap;
                table.at(k).at(l).whichCase = 3;
                table.at(k).at(l).fromWhom = & table.at(k).at(l-1);
            }

            else if ((table.at(k-1).at(l-1).value + a >= table.at(k-1).at(l).value + gap)
                     && (table.at(k).at(l-1).value + gap >= table.at(k-1).at(l).value + gap)){

                table.at(k).at(l).value = table.at(k-1).at(l).value + gap;
                table.at(k).at(l).whichCase = 2;
                table.at(k).at(l).fromWhom = & table.at(k-1).at(l);
            }

        }
    }

    int m = sequenceX.size();
    int n = sequenceY.size();
    struct Penalty iterator = table.at(m).at(n);

    std::vector <char> alignmentX;
    std::vector <char> alignmentY;


    while (iterator.fromWhom != nullptr){
        if (iterator.whichCase == 1){
            alignmentX.push_back(sequenceX.at(m-1));
            alignmentY.push_back(sequenceY.at(n-1));
            m--;
            n--;
        }
        else if (iterator.whichCase == 2) {
            alignmentX.push_back(sequenceX.at(m - 1));
            alignmentY.push_back('-');
            m--;
        }
        else if (iterator.whichCase == 3) {
            alignmentX.push_back('-');
            alignmentY.push_back(sequenceY.at(n-1));
            n--;
        }

        iterator = *(iterator.fromWhom);
    }



    for (int i = alignmentX.size(); i>0; i--) cout<<alignmentX.at(i-1);
    cout<<endl;
    for (int i = alignmentX.size(); i>0; i--) {
        if (alignmentX.at(i-1) == alignmentY.at(i-1))
             cout<<"|";
    else cout<<" ";}
    cout<<endl;
    for (int i = alignmentY.size(); i>0; i--) cout<<alignmentY.at(i-1);

    cout<<endl;



    return 0;}