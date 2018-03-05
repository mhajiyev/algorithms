#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Entry {   // This structure will be an individual entry in the matrix
    int valueG;  // maximized cost for the subproblem
    int valueP;  // 0 or 1 indicating how we computed the particular entry
    struct Entry * fromWhom;  // pointer to the entry which was used to compute the given entry
    int column;    // column number of the given entry in the matrix
};


int main(int argc,char ** argv) {
    std::string input = argv[1];
    std::string output = argv[2];
    std::ifstream inputFile{input};


    std::vector <std::vector <struct Entry>> table; //matrix containing the results for subproblems
    std::vector <int> costs;   //the costs for the items
    std::vector <int> weights;   // the weights for the items
    int counter = 0;
    int n,reader,b;

while (inputFile>>reader){     // Reading from stdin: n becomes # of items, b becomes knapsack size
    if (!counter) n = reader;
    if (counter && counter <= n) costs.push_back(reader);
    else if (counter && counter>n && counter<=2*n) weights.push_back(reader);
    else b = reader;
    counter++;              //------------
}


    for (int j = 0; j < n ; j++){         // Initially filling the matrix with entries and filling the base case
        std::vector <struct Entry> column;
        for (int t = 0; t<=b; t++){

        struct Entry entry;
        entry.valueG = 0;
        entry.valueP = 0;
        entry.fromWhom = nullptr;
        entry.column = j;
        column.push_back(entry);}
        table.push_back(column);            //------------------
    }


    // Computing the rest of the values in the matrix
    for (int k = 0; k < n; k++){
        for (int l = 1; l <= b; l++){
            if ((k && l-weights.at(k)>=0 && table.at(k-1).at(l).valueG >= costs.at(k)+table.at(k).at(l-weights.at(k)).valueG)
                             || (k && l-weights.at(k)<0)){

                table.at(k).at(l).valueG = table.at(k-1).at(l).valueG;
                table.at(k).at(l).valueP = 0;
                table.at(k).at(l).fromWhom = & table.at(k-1).at(l);

            }
            else if ((k && l-weights.at(k)>=0 && table.at(k-1).at(l).valueG < costs.at(k)+table.at(k).at(l-weights.at(k)).valueG)
                             || (!k && l-weights.at(k)>=0)){

                table.at(k).at(l).valueG = costs.at(k)+table.at(k).at(l-weights.at(k)).valueG;
                table.at(k).at(l).valueP = 1;
                table.at(k).at(l).fromWhom = & table.at(k).at(l-weights.at(k));
            }
        }
    }
    //--------------------------



    struct Entry iterator = table.at(n-1).at(b);   // We start backtracking from the final value for the actual problem

    std::vector <int> optimalSolution;    // Optimal solution
    int sofar = iterator.valueP;


    while (iterator.fromWhom != nullptr){       // We are backtracking until we reach the [0][0] entry
        if (iterator.column != iterator.fromWhom->column){
            optimalSolution.push_back(sofar);   // Reconstructing the optimal solution
            sofar = iterator.fromWhom->valueP;
        }
        else {
           sofar+=iterator.fromWhom->valueP;
        }
        iterator = *(iterator.fromWhom);
    }
    optimalSolution.push_back(sofar);   // Reconstructing the optimal solution


    ofstream myfile;
    myfile.open(output);
    for (int i = optimalSolution.size(); i>0; i--) myfile<<optimalSolution.at(i-1)<<" ";   //Output to the file
    myfile<<endl;
    myfile.close();


    return 0;}