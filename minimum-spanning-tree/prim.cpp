#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>


using namespace std;

struct Node {
    int name;   //Node number
    vector <int> neighbours; //all neighbouring nodes
    vector <int> costs;      //all costs of the edges that connect the given node to its neighbours
    int minCostEdge;  // index of the edge with the smallest cost that is incident with the given Node
    int withWhom;      //name of the neighbor node whom the given node connect via min cost edge

    Node(int name){
        this->name = name;
        minCostEdge = std::numeric_limits<int>::max();;  //Initially minCostEdge is not valid because V(H) is empty set
        withWhom = -1;  // Indicates initial non-validity
    }
};

std::vector <struct Node> Graph; // Representation of the graph

std::vector <int> Span;  // Keeps all the nodes in the spanning so far

std::vector <int> deltaVH; //keeps numbers of the nodes that are in delta(V(H)) at each step

void minCostEdge(struct Node &,std::ofstream &);  // Actual function that implements improved Prim's algorithm

int main(int argc,char ** argv) {
    std::string input = argv[1];
    std::string output = argv[2];
    std::ifstream inputFile{input};
    int counter = 0;
    int numNodes,numEdges,costmax;

    int a,b,c;
    while (inputFile>>a>>b>>c){
        if (!counter){
            numNodes = a;
            numEdges = b;
            costmax = c;
            for (int i=0;i<numNodes;i++){
                struct Node myNode = Node(i);
                Graph.push_back(myNode);
            }
        }
        else {
            Graph.at(a).neighbours.push_back(b);    // Filling neighbor list and their costs for each Node
            Graph.at(a).costs.push_back(c);
            Graph.at(b).neighbours.push_back(a);    // Reciprocally
            Graph.at(b).costs.push_back(c);
        }

        counter++;
    }

    Span.push_back(0);  // Initially spanning starts from vertex 0

    for (int a =0; a < Graph.at(0).neighbours.size();a++ ){  //adding the neighbours of node 0 to delta(V(H))
    deltaVH.push_back(Graph.at(0).neighbours.at(a));}

    ofstream myfile;
    myfile.open(output);
    while (Span.size()!=Graph.size()) minCostEdge(Graph.at(Span.back()), myfile);  // Until the spanning covers all the nodes in the graph
    myfile.close();
    return 0;
}




void minCostEdge(struct Node & added,std::ofstream & myfile){  // added is the last node that was added to the spanning
    for (int j=0;j<added.neighbours.size();j++){    // -> O(n)

                if (Graph.at(added.neighbours.at(j)).minCostEdge > added.costs.at(j)){   // We may update min cost edges for some nodes in the
                                                                                        // delta(V(H))
                    Graph.at(added.neighbours.at(j)).minCostEdge = added.costs.at(j);
                    Graph.at(added.neighbours.at(j)).withWhom = added.name;
                }
        }

    int minEdge = std::numeric_limits<int>::max();  //temporary variable will keep minimum minCostEdge among all neighbouring nodes
    int neighNode1,neighNode2;

    std::vector <int>::iterator it;
          for (int l = 0; l < deltaVH.size(); l++){  // Iterate through all nodes in delta(V(H)) -->O(n)
            it = find(Span.begin(),Span.end(),deltaVH.at(l)); // -->O(n)

            if (it == Span.end()){  // We check that if the other end of the edge is not already in the spanning

                if (Graph.at(deltaVH.at(l)).minCostEdge < minEdge) { //---> O(1)
                    minEdge = Graph.at(deltaVH.at(l)).minCostEdge;   // update minEdge

                    neighNode1 = Graph.at(deltaVH.at(l)).withWhom; // one end of the potential edge
                    neighNode2 = deltaVH.at(l);     // another end of the potential edge

                }
            }

    }


    Span.push_back(neighNode2);   // Spanning gets increased in size
    myfile<<neighNode1<<" "<<neighNode2<<endl;

    std::vector <int>::iterator it1;
    for (int s =0 ; s < Graph.at(neighNode2).neighbours.size();s++){  //-->O(n)
        it1 = find(deltaVH.begin(),deltaVH.end(),Graph.at(neighNode2).neighbours.at(s));   // -->O(n)
        if (it1 == deltaVH.end()){                   // If a neighbour edge is not already in delta(V(H))
            deltaVH.push_back(Graph.at(neighNode2).neighbours.at(s));    // We increase the set of delta(V(H))
        }
    }

        it1 = find(deltaVH.begin(),deltaVH.end(),neighNode2);
        deltaVH.erase(it1);   // Removing the added node from delta(V(H))

}




