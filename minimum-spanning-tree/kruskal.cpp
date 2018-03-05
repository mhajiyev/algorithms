#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    int name;
    int blockNum;
   // vector <int> neighbours;
    //vector <int> costs;

    Node(int name){
        this->name = name;
        this->blockNum = name;
    }
};


struct Edge {
    int cost;
    struct Node * endA;
    struct Node * endB;
};


std::vector <struct Node> Graph;             // Representation of the graph itself
std::vector <std::list <int>> Blocks;        // Contains parts of the forest
std::vector <struct Edge> Edges;            //  Contains all the edges in the graph with corresponding costs

void minCostEdge(std::string);      //This function actually implements Kruskal's algorithm

bool myFunction(struct Edge & i, struct Edge & j){return i.cost<j.cost;}      // Based on this function we sort all the
                                                                                // edges in the graph by their costs(ascending)


int main(int argc,char ** argv) {
    std::string input = argv[1];
    std::string output = argv[2];
    std::ifstream inputFile{input};
    int counter = 0;
    int numNodes,numEdges,costmax;

    int a,b,c;
    while (inputFile>>a>>b>>c){  //Reading from input file
        if (!counter){
            numNodes = a;
            numEdges = b;
            costmax = c;
            for (int i=0;i<numNodes;i++){
                struct Node myNode = Node(i);
                std::list <int> temp;
                temp.push_back(i);         // We first assign each block only one vertex
                Graph.push_back(myNode);
                Blocks.push_back(temp);    // Adding all n blocks to the container
            }

        }
        else {

            struct Edge myEdge;
            myEdge.cost = c;
            myEdge.endA = & (Graph.at(a));      //Assigning costs and both ends to the edges in the Graph
            myEdge.endB = & (Graph.at(b));
            Edges.push_back(myEdge);

        }

        counter++;
    }

    std::sort(Edges.begin(),Edges.end(),myFunction);  // Sorting the Edges by their cost --> O(mlogm)



    minCostEdge(output);

    return 0;
}



void minCostEdge(std::string output) {
    ofstream myfile;
    myfile.open(output);

    for (int i = 0; i < Edges.size(); i++) {
        if (Edges.at(i).endA->blockNum != Edges.at(i).endB->blockNum) {   // Checking if the ends of the edge not located in the same
                                                                          // block. This prevents creation of cycles.

            myfile << Edges.at(i).endA->name << " " << Edges.at(i).endB->name << endl;  //Output to the file

            int block1 = Blocks.at(Edges.at(i).endA->blockNum).size();
            int block2 = Blocks.at(Edges.at(i).endB->blockNum).size();
            if (block1 < block2) {     //If the number of nodes in block1 is less than in block2
                std::list<int> temp = Blocks.at(Edges.at(i).endA->blockNum);

                for (std::list<int>::const_iterator iterator = temp.begin(), end = temp.end();
                     iterator != end; ++iterator) {

                    Graph.at(*iterator).blockNum = Edges.at(i).endB->blockNum;   //Updating block number of nodes in the smaller block
                                                                                 //by assigning block number of the bigger block
                }
                std::list <int>::iterator it;
                it = Blocks.at(Edges.at(i).endB->blockNum).end();
                Blocks.at(Edges.at(i).endB->blockNum).splice(it,temp);  // Merge 2 blocks into 1
            }
            else {   // # of nodes in block2 is less than in block1
                std::list<int> temp = Blocks.at(Edges.at(i).endB->blockNum);
                for (std::list<int>::const_iterator iterator = temp.begin(), end = temp.end();
                     iterator != end; ++iterator) {

                    Graph.at(*iterator).blockNum = Edges.at(i).endA->blockNum;

                }
                std::list <int>::iterator it;
                it = Blocks.at(Edges.at(i).endA->blockNum).end();
                Blocks.at(Edges.at(i).endA->blockNum).splice(it,temp);
            }
        }
    }
    myfile.close();
}