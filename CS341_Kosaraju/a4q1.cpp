#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;



struct Node {
    int name;   //Node number
    vector <Node *> neighbours; //all neighbouring nodes
    vector <Node *> neighboursReverse; //all neighbouring nodes
    bool visited;
    Node(int name){
        this->name = name;
        this->visited = false;
    }
};


bool comparison(int i,int j){return (i>j);}
void DFS();
void DFS_reverse(struct Node *);
int DFS_original(struct Node *);
std::vector <struct Node *> Graph; // Representation of the graph
std::vector <Node *> fTimes;
int size =0;


int main() {
    int numNodes,numEdges;
    std::cin>>numNodes;
    std::cin>>numEdges;

    for (int i=1;i<=numNodes;i++){
        struct Node * myNode = new struct Node(i);
        Graph.push_back(myNode);
}

    int a,b;
    int counter = 0;
    while (counter!=numEdges){
            std::cin>>a>>b;
            Graph.at(a-1)->neighbours.push_back(Graph.at(b-1));    //
            Graph.at(b-1)->neighboursReverse.push_back(Graph.at(a-1));    //
        counter++;
    }

   DFS();


    for (int i =0 ; i<Graph.size();i++){
        Graph.at(i)->visited = false;
    }

    std::vector <int> sizes;

    for (int i = fTimes.size()-1; i >= 0 ;i--){
       if (fTimes.at(i)->visited == false){
           size = 0;
           size = DFS_original(fTimes.at(i));
           sizes.push_back(size);
       }
    }

    std::sort(sizes.begin(),sizes.end(),comparison);
    if (sizes.size()>=5){
    for (int i = 0; i<5;i++){cout<<sizes.at(i)<<"\t";} }
    else {
        for (int i = 0; i<sizes.size();i++){cout<<sizes.at(i)<<"\t";}
        for (int i = 0; i<5-sizes.size();i++){cout<<0<<"\t";}
    }
    cout<<endl;

    for (int i =0 ; i<Graph.size();i++){
        delete Graph.at(i);
    }


    return 0;
}


int DFS_original(Node * start){
    start->visited = true;
    for (int i =0 ; i<start->neighbours.size();i++){
        if (start->neighbours.at(i)->visited == false){
            int temp = DFS_original(start->neighbours.at(i));
            (void)temp;
        }

    }
    size++;
    return size;
}

void DFS_reverse(Node * start){
    start->visited = true;
    for (int i =0 ; i<start->neighboursReverse.size();i++){
        if (start->neighboursReverse.at(i)->visited == false){
            DFS_reverse(start->neighboursReverse.at(i));

        }
    }
    fTimes.push_back(start);
}

void DFS(){
    for (int i=0;i<Graph.size();i++){
        if (Graph.at(i)->visited == false) DFS_reverse(Graph.at(i));
    }
}