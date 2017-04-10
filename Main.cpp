#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <limits.h>
#include <chrono>
#define TIME_TESTE true
using namespace std;


typedef struct Node{
  int id;
  vector<pair<int,int> > adjacent;
  int position;
  bool visited;
  int dist;
} Node;
///Variaveis Globais
unordered_map<int,Node> NodesHash;





class Heap{
public:
    vector<pair<int,int> > nodes;
    Heap(){
        number_children = 2;
    }
    Heap(int n){
        number_children = n;
    }
    void insert( pair<int,int>);
    void deleteMin ( );
    void update ( int position, int value);
    void printHeap(int position){
        int i,childrenPosition = children(position);
        cout<<"ID: "<<nodes.at(position).second<< " childrens: ";
        for(i = childrenPosition;i<childrenPosition+number_children && i<=nodes.size()-1;i++){
            cout<< nodes.at(i).second << "  ";
        }
        cout<<endl;
        for(i = childrenPosition;i<childrenPosition+number_children && i<=nodes.size()-1;i++){
            printHeap(i);
        }
    }
private:
    void heapifyUp(int position);
    void heapifyDown(int position);
    int number_children ;
    int children(int p){
        return (number_children*p)+1;
    }
    int parent(int p){
        return (p-number_children +1)/number_children;
    }
    void vectorSwap(int p1, int p2){
        unordered_map<int,Node>::iterator got;
        got = NodesHash.find(nodes.at(p1).second);
        got ->second.position = p2;
        got = NodesHash.find(nodes.at(p2).second);
        got ->second.position = p1;
        pair <int,int> element1;
        element1 = nodes.at(p1);
        nodes.at(p1) = nodes.at(p2);
        nodes.at(p2) = element1;
        
    }
};
void Heap::insert( pair<int,int> newPair ){
    nodes.push_back( newPair );
    unordered_map<int,Node>::iterator got;
    got = NodesHash.find(newPair.second);
    got->second.position = nodes.size() -1;
    Heap::heapifyUp(nodes.size() -1 );
};
void Heap::deleteMin ( ){
    if(nodes.size()>1){
        
        vectorSwap(0,nodes.size()-1);
        nodes.pop_back();
        Heap::heapifyDown(0);
    }
    else
        nodes.pop_back();

};
void Heap::heapifyUp(int position){
    if(position == 0)
        return;
    else{
            int parentPosition = parent(position);
            if(nodes.at(position).first < nodes.at(parentPosition).first){
                    vectorSwap(position,parentPosition);
                    heapifyUp(parentPosition);
            }
    }
};
void Heap::heapifyDown(int position){
    int childrenPosition = children(position);

    if(childrenPosition > nodes.size()-1) // case no children
        return;
    if(childrenPosition == nodes.size()-1){ // case one children
        if(nodes.at(position).first > nodes.at(childrenPosition).first){
            swap(position,childrenPosition);
        }
    }
    else{
        int i,swapFlag = 0,smallerChildrenPosition= childrenPosition; //case two or more children
        for(i = childrenPosition;i<childrenPosition+number_children && i<nodes.size()-1;i++){
            if(nodes.at(position).first > nodes.at(i).first)
                swapFlag = 1;
            if(nodes.at(smallerChildrenPosition) >  nodes.at(i))
                smallerChildrenPosition = i;
        }
        if(swapFlag == 1){
            Heap::vectorSwap(smallerChildrenPosition,position);
            heapifyDown(smallerChildrenPosition);
        }
    }
};
void Heap::update ( int position, int value){
    nodes.at(position) = make_pair(value,nodes.at(position).second);
    if(nodes.at(position).first < nodes.at(parent(position)).first){
        heapifyUp(position);
    }
    else{
        heapifyDown(position);
    }
};
void teste_heap(){
        int chave,i = 0,flagInsert = 1;
      Heap heap;
      while(flagInsert){
        cout<<"Insira o valor chave :";
        cin>> chave;
        if(cin.good()){
            heap.insert(make_pair(chave,i));
            i++;
        }
        else{
            cin.clear();
            cin.ignore(256, '\n'); // NB: preferred method for flushing cin
            flagInsert = 0;
        }

      }
      heap.printHeap(0);
      flagInsert = 1;
      int aux;
      while(flagInsert){
      cout<<"Insira o valor chave que deseja trocar :";
      cin>> chave;
      cout<<"\nInsira a posição que ele assumira: ";
      cin>> aux;
      if(cin.good()){
          heap.update(aux,chave);
          heap.printHeap(0);
      }
      else{
          cin.clear();
          cin.ignore(256, '\n'); // NB: preferred method for flushing cin
          flagInsert = 0;
      }

    }
    cout<<"\n";
    heap.printHeap(0);
    cout<<"DELETOU"<<endl;
    heap.deleteMin();
    heap.printHeap(0);
	cout<<"DELETOU"<<endl;

    heap.deleteMin();
    heap.printHeap(0);
    }


int parser(string filePath);

//just for the porpuse of testing the heap

int main(int argc, char *argv[]){
    if(argc != 4){
        cout<<"USAGE: SRC(INT) DST(INT)GRAPH_PATH(STRING)"<<endl;
        exit(-1);
    }
    ofstream myfile("tempos.txt",ofstream::app);
    chrono::system_clock::time_point total; //medição tempo total
    int num_updates =0,num_deletemin = 0,num_edges;
    if(TIME_TESTE){
        total = chrono::system_clock::now();
    }
    
    int origem = atoi(argv[1]), destino = atof(argv[2]);
    string filePath(argv[3]);
    num_edges = parser(filePath);
    if(NodesHash.size()>0){
        Heap heap;
        unordered_map<int,Node>::iterator got;
        got = NodesHash.find(origem);
        vector<pair<int,int> > vizinhos = got->second.adjacent;

        for ( auto it = NodesHash.begin(); it != NodesHash.end(); ++it ){
            heap.insert(make_pair(INT_MAX,it->second.id)); //INT_MAX para simular o infinito
        }
        got = NodesHash.find(origem);
        if(got == NodesHash.end()){
            cout<<"Erro, origem invalida"<<endl;
        }
        else{
            vizinhos = got->second.adjacent;
            got->second.visited = true;
            got->second.dist = 0;
            for(int i=0;i<vizinhos.size();i++){
                got = NodesHash.find(vizinhos[i].first);
                heap.update(got->second.position,vizinhos[i].second);
            }

            while(heap.nodes.empty() == false){
                got = NodesHash.find(heap.nodes[0].second);
                if(got->second.visited == false){
                    got->second.visited == true;
                    int actualWeight = heap.nodes[0].first;
                    got->second.dist = actualWeight;
                    vizinhos = got->second.adjacent;
                    for(int i=0;i<vizinhos.size();i++){ // aqui faz a parte de testar pra ver se da uptade ou naao
                        int newWeight = vizinhos[i].second+actualWeight;
                        int vizinho_position = NodesHash.find(vizinhos[i].first)->second.position;

                        if(heap.nodes[vizinho_position].first > newWeight){
                            heap.update(vizinho_position,newWeight);
                            num_updates++;
                        }
                    }
                }
                if(heap.nodes.empty() == false){
                    heap.deleteMin();
                    num_deletemin++;
                }
            }
            if(NodesHash.find(destino)->second.dist != INT_MAX)
                cout<<NodesHash.find(destino)->second.dist<<endl;
            else
                cout<<"inf"<<endl;


        }
    //    heap.printHeap(0);
    //    for (auto& x: NodesHash){
    //        cout<<x.second.id<<endl;
    //        heap.insert(INT_MAX,x.second.id);
    //    }
    //    
    //    for (auto& x: NodesHash){
    //         myfile <<" "<< x.first << ": " << x.second.id <<" "<< x.second.adjacent.size()<<"  "<<x.second.position << std::endl;
    //    } // teste da NodeHash;;;

        if(TIME_TESTE){

            myfile<<"Tempo total: "<<chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()-total).count()<<" Numero de nodes: "<<NodesHash.size()<<" Numero de arestas: "<<num_edges<< endl;
            myfile<<"Numero de inserts: "<<NodesHash.size()<<" Numero de deleteMin: "<<num_deletemin<<" Numero de update: "<<num_updates<<endl;
        }
    }
    else
        cout<<"inf"<<endl;
 
    return 0;
}

int parser(string filePath){
    ifstream inputFile( filePath, ifstream::in );
    int num_edges= 0;
    if ( !inputFile.is_open() ) {
                    cout << "Could not open input file." << endl;
                    exit(-1);
            }
    else
    {
	string token;
        while(token != "a" && inputFile.eof()){
            (inputFile>> token);
        }
        int id,dest, weight;
        std::unordered_map<int,Node>::iterator got;
        while(token == "a"){
            num_edges++;
            inputFile>> token;
            id = atoi( token.c_str() );
            inputFile>> token;
            dest = atoi( token.c_str() );
            inputFile>> token;
            weight = atoi( token.c_str() );
            inputFile>> token;
            got = NodesHash.find(id);
            if( got == NodesHash.end() ){
               Node newNode;
               newNode.id = id;
               newNode.adjacent.push_back(make_pair(dest,weight));
               newNode.visited = false;
               NodesHash.insert(make_pair(id,newNode));
               
            }
            else{
                got->second.adjacent.push_back(make_pair(dest,weight));
            }
        }
    }
    return num_edges;
}
