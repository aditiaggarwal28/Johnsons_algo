#include<bits/stdc++.h>
#include<iostream>
#include<chrono>
using namespace std;
using namespace std::chrono;
/*implemented using array*/
vector<int> array_dijkstra(int N,vector<vector<int>> matrix,int s){
    vector<int> array_to_store_vertices_value(N,0);
    for(int i=0;i<N;i++){
        if(i!=s-1){
            array_to_store_vertices_value[i]=999999;
        }
    }
    vector<int> contain_shortest_distances(N,-1);
    int come=0;
    while(come!=N){
        int mini=0;
        int number=999999;
        for(int i=0;i<N;i++){
            if(array_to_store_vertices_value[i]<number){
                number=array_to_store_vertices_value[i];
                mini=i;
            }
        }
        if(array_to_store_vertices_value[mini]>=999999){
            break;
        }
        for(int j=0;j<N;j++){
            if(matrix[mini][j]<0 && array_to_store_vertices_value[j]!=10000000){
                contain_shortest_distances[j]=-2;
                array_to_store_vertices_value[j]=10000000;
            }
            if(matrix[mini][j]<999999 && array_to_store_vertices_value[j]!=10000000){
                if(array_to_store_vertices_value[mini]+matrix[mini][j]<array_to_store_vertices_value[j]){
                    array_to_store_vertices_value[j]=array_to_store_vertices_value[mini]+matrix[mini][j];
                    contain_shortest_distances[j]=array_to_store_vertices_value[j];
                }
            }
        }

        contain_shortest_distances[mini]=array_to_store_vertices_value[mini];
        array_to_store_vertices_value[mini]=10000000;
        come=0;
        for(int i=0;i<N;i++){
            if(array_to_store_vertices_value[i]==10000000){
                come++;
            }
        }
    }
        return contain_shortest_distances;
}

vector<int> implementing_bellman_ford(int N,vector<vector<int> > matrix,int s){
    queue<int> bellman;
    int times[N]={0};
    times[s-1]+=1;
    int in_queue[N]={0};
    vector<int> weights(N,0);
    for(int i=0;i<N;i++){
        if(i==s-1){
            weights[i]=0;
        }
        else{
            weights[i]=999999;
        }
    }
    in_queue[s-1]=1;
    bellman.push(s);
    int com=0;
    while(!bellman.empty() && com==0){
        int a=bellman.front();
        int b=weights[a-1];
        for(int i=0;i<N;i++){
            if(matrix[a-1][i]!=999999 && a-1!=i){
                if(b+matrix[a-1][i]<weights[i]){
                    weights[i]=b+matrix[a-1][i];
                    if(in_queue[i]==0){
                        in_queue[i]=1;
                        times[i]+=1;
                        if(times[i]==N){
                            com=1;
                        }
                        bellman.push(i+1);
                    }
                }
            }
        }
        bellman.pop();
        in_queue[a-1]=0;
    }
    if(com==1){
        weights[0]=-10000000;
        return weights;
    }
    return weights;
}

vector<pair<int,int>> perculate_up(vector<pair<int,int>> storing_nodes,int index){
    if(index==0){
        return storing_nodes;
    }
    else if(storing_nodes[(index-1)/2].first<=storing_nodes[index].first){
        return storing_nodes;
    }
    else{
        pair<int,int> a=storing_nodes[(index-1)/2];
        storing_nodes[(index-1)/2]=storing_nodes[index];
        storing_nodes[index]=a;
        storing_nodes=perculate_up(storing_nodes,(index-1)/2);
        return storing_nodes;
    }
}

vector<pair<int,int>> perculate_down(vector<pair<int,int>> storing_nodes,int index){
    if((2*index+1)>(storing_nodes.size()-1)){
        return storing_nodes;
    }
    else if((2*index+1)==storing_nodes.size()-1){
        if(storing_nodes[(2*index+1)].first>=storing_nodes[index].first){
            return storing_nodes;
        }
        else{
            pair<int,int> a=storing_nodes[2*(index)+1];
            storing_nodes[2*index+1]=storing_nodes[index];
            storing_nodes[index]=a;
            return storing_nodes;
        }
    }
    else{
        int new_index=0;
        if(storing_nodes[2*index+1]<storing_nodes[2*index+2]){
            new_index=2*index+1;
        }
        else{
            new_index=2*index+2;
        }
        if(storing_nodes[new_index].first>=storing_nodes[index].first){
            return storing_nodes;
        }
        else{
            pair<int,int> a=storing_nodes[new_index];
            storing_nodes[new_index]=storing_nodes[index];
            storing_nodes[index]=a;
            storing_nodes=perculate_down(storing_nodes,new_index);
            return storing_nodes;
        }
    }
}

vector<pair<int,int>> inserting_node(vector<pair<int,int>> storing_nodes,int value,int index){
    storing_nodes.push_back(make_pair(value,index));
    storing_nodes=perculate_up(storing_nodes,storing_nodes.size()-1);
    return storing_nodes;
}

vector<pair<int,int>> change_the_value(vector<pair<int,int>> storing_nodes,int value,int index){
    int indi=0;
    for(int i=0;i<storing_nodes.size();i++){
        if(storing_nodes[i].second==index){
            storing_nodes[i].first=value;
            indi=i;
        }
    }
    storing_nodes=perculate_up(storing_nodes,indi);
    return storing_nodes;
}

vector<pair<int,int>> delete_element(vector<pair<int,int>> storing_nodes){
    pair<int,int> a;
    a=storing_nodes[0];
    storing_nodes[0]=storing_nodes[storing_nodes.size()-1];
    storing_nodes[storing_nodes.size()-1]=a;
    storing_nodes.pop_back();
    if(!storing_nodes.empty()){
    storing_nodes=perculate_down(storing_nodes,0);
    }
    return storing_nodes;
}

vector<int> dijkstra_binary(int N,vector<vector<int> > matrix,int s){
    //this is the vector of the pointers which points to the corresponding node and from this we can easily know it's parent
    //it's sibling,it's child and all the other things
    int size_heap=0;
    vector<int> value_storer(N,999999);
    vector<int> node_storer(N,0);
    vector<pair<int,int>> storing_nodes;
    value_storer[s-1]=0;
    storing_nodes=inserting_node(storing_nodes,0,s-1);
    node_storer[s-1]=1;
    int minimum_index=0;
    while(storing_nodes.size()!=0 && size_heap!=N){
        //finding minimum node in a heap
            minimum_index=storing_nodes[0].second;
            node_storer[minimum_index]=2;
            value_storer[minimum_index]=storing_nodes[0].first;
            //deleting the minimum value from the heap and then arranging the heap as it is
            storing_nodes=delete_element(storing_nodes);
            size_heap++;
        for(int j=0;j<N;j++){
            if(matrix[minimum_index][j]<999999 && node_storer[j]!=2){
                if(value_storer[minimum_index]+matrix[minimum_index][j]<value_storer[j]){
                    value_storer[j]=value_storer[minimum_index]+matrix[minimum_index][j];
                    if(node_storer[j]==0){
                        storing_nodes=inserting_node(storing_nodes,value_storer[j],j);
                        node_storer[j]=1;
                    }
                    else if(node_storer[j]==1){
                        storing_nodes=change_the_value(storing_nodes,value_storer[j],j);
                    }
                }
            }
        }
    }
   return value_storer;
}



struct Node{
    int data;
    int index_in_matrix;
    struct Node *sibling;
    struct Node *parent;
    struct Node *child;
    int degree;
};

Node *head;
//binomial Node
//here I am creating a new Node and emptying all the previous stored things
void create_a_new_heap(Node *headi){
    if(headi==head){
        head=NULL;
    }
    else{
        headi=NULL;
    }
    return;
}

//finding the minimum
Node * finding_the_minimum(){
    Node *a=head;
    Node *b=a;
    if(a!=NULL){
    int mini=a->data;
    while(a->sibling!=NULL){
        a=a->sibling;
        if(a->data<mini){
            mini=a->data;
            b=a;
        }
    }
    }
    return b;
}

//merging two binomial Nodes
vector<Node *> merging_two_binomial_Nodes(Node *a,Node *b,Node *k){
    vector<Node *> random;
    if(a->data<b->data){
        if(a==head){
            Node *childd=a->child;
            a->child=b;
            a->sibling=b->sibling;
            b->sibling=childd;
            a->degree++;
            b->parent=a;
            head=a;
            random.push_back(head);
            random.push_back(k);
            return random;
        }
        Node *childd=a->child;
        a->child=b;
        a->sibling=b->sibling;
        b->sibling=childd;
        a->degree++;
        b->parent=a;
        random.push_back(a);
        random.push_back(k);
        return random;
    }
    else{
        if(a==head){
            Node *childd=b->child;
            b->child=a;
            a->sibling=childd;
            b->degree++;
            a->parent=b;
            head=b;
        /*if(head->child->sibling){
            //cout<<"run"<<head->child->sibling->data<<endl;
            if(head->child->sibling->sibling){
              //  cout<<"run"<<head->child->sibling->sibling->data<<endl;
        }
        }*/
            random.push_back(head);
            random.push_back(k);
            return random;
        }
        Node *childd=b->child;
        b->child=a;
        if(k!=NULL){
        if(k==head){
            head->sibling=b;
        }
        else{
            k->sibling=b;
        }
        }
        a->sibling=childd;
        b->degree++;
        a->parent=b;
        random.push_back(b);
        random.push_back(k);
        return random;
    }
}

//rearranging Node
vector<Node *>  exchanging_Node(Node *a,Node *b,Node *drum){
    vector<Node *> random;
    Node * pp=b->sibling;
    b->sibling=a;
    a->sibling=pp;
    if(drum){
    if(drum==head){
        head->sibling=b;
    }
    else{
        drum->sibling=b;
    }
    }
    random.push_back(b);
    random.push_back(drum);
    return random;
}

//checking order is maintained or not
void order_maintained(){
    Node *dum=head;
    Node *drum=NULL;
    while(dum->sibling!=NULL){
        if(dum->degree==dum->sibling->degree){
            vector<Node *> random;
            if(dum==head){
            random=merging_two_binomial_Nodes(dum,dum->sibling,drum);
            head=random[0];
            dum=head;
            drum=NULL;
            }
            else{
                random=merging_two_binomial_Nodes(dum,dum->sibling,drum);
                dum=random[0];
                drum=random[1];
            }
        }
        else if(dum->degree>dum->sibling->degree){
            vector<Node *> random;
            if(dum==head){
            random=exchanging_Node(dum,dum->sibling,drum);
            head=random[0];
            dum=head;
            drum=NULL;
            }
            else{
                random=exchanging_Node(dum,dum->sibling,drum);
                dum=random[0];
                drum=random[1];
            }
        }
        else{
            drum=dum;
            dum=dum->sibling;
        }
    }
    return;
}

//creating a node
Node * create_node(int data,int index){
    Node *a= (Node *)malloc(sizeof(Node));
    a->data=data;
    a->index_in_matrix=index;
    a->parent=NULL;
    a->sibling=NULL;
    a->degree=0;
    a->child=NULL;
    return a;
}

//insert a node
void inserting_a_node(Node *a){
    //if(head){
      //  cout<<head->data<<"akad\n";
    //}
    a->sibling=head;
    head=a;
    //cout<<head->data<<endl;
    //if(head->sibling){
      //  cout<<head->sibling->data<<endl;
    //}
    order_maintained();
    return;
}


//delete min or extract min function
Node * delete_min(){
    Node *a=head;
    Node *b=a;
    Node *c=a;
    int mini=a->data;
    //cout<<"ldk"<<endl;
    if(a->sibling==NULL){
        //cout<<"ldkkk"<<endl;
        head=a->child;
        //cout<<"hero"<<endl;
        if(head){
        head->parent=NULL;
        }
        else{
            return head;
        }
        //cout<<"her"<<endl;
        if(a->child==NULL){
            //cout<<"hello"<<endl;
            head=NULL;
            a->child=NULL;
            a->parent=NULL;
            a->sibling=NULL;
            return head;
        }
        a->child=NULL;
        a=head;
        while(a->sibling!=NULL){
            a=a->sibling;
            a->parent=NULL;
        }
        return head;
    }
    while(a->sibling->sibling!=NULL){
        if(a->sibling->data<mini){
            mini=a->sibling->data;
            b=a->sibling;
            c=a;
        }
        a=a->sibling;
    }
    if(a->sibling->data<mini){
        mini=a->sibling->data;
        b=a->sibling;
        c=a;
    }
    if(b==head){
        //cout<<"hell"<<endl;
        a=b->child;
        if(!a){
            head=b->sibling;
            return head;
        }
        //cout<<"jello"<<endl;
        while(a->sibling!=NULL){
            a->parent=NULL;
            a=a->sibling;
        }
        a->parent=NULL;
        a->sibling=head->sibling;
        //cout<<"HEl"<<endl;
        head=head->child;
        b->sibling=NULL;
        b->child=NULL;
        order_maintained();
        return head;
    }
    if(c==head){
        //cout<<"tech"<<endl;
        //childpoin is the pointer to the sibling of minvalue child
        Node *childpoin=b->sibling;
        Node *storer=b;
        head->sibling=b->child;
        if(b->child){
            b->child->parent=NULL;
        }
        else{
          //  cout<<"tecjO"<<endl;
            head->sibling=childpoin;
            b->child=NULL;
            b->parent=NULL;
            b->sibling=NULL;
            return head;
        }
        while(b->child->sibling!=NULL){
            b->child->sibling->parent=NULL;
            b->child=b->child->sibling;
        }
        b->child->sibling=childpoin;
        //cout<<"techi"<<endl;
        storer->child=NULL;
        storer->parent=NULL;
        storer->sibling=NULL;
        order_maintained();
        return head;
    }
    else{
            //cout<<"sada"<<endl;
        Node *childpoin=b->sibling;
        Node *storer=b;
        c->sibling=b->child;
        if(b->child){
        b->child->parent=NULL;
        }
        else{
          //  cout<<"sand"<<endl;
            c->sibling=childpoin;
            b->child=NULL;
            b->parent=NULL;
            b->sibling=NULL;
            return head;
        }
        //cout<<"saaa"<<endl;
        while(b->child->sibling!=NULL){
            b->child->sibling->parent=NULL;
            b->child=b->child->sibling;
        }
        //cout<<"sans"<<endl;
        b->child->sibling=childpoin;
        storer->child=NULL;
        storer->parent=NULL;
        storer->sibling=NULL;
        order_maintained();
        return head;
    }
    return head;
}


//decreasing the value and then sorting //let all the nodes sorted
vector<Node *> perculate_up(Node *a,vector<Node *> initial_storer){

//I am actually making a vector where the pointers to all the nodes are stored and I am changing the data value of that node from vector
    Node *b=a;
    while(b->parent!=NULL && b->parent->data>b->data){
        int det=b->data;
        Node* hola=b->parent;
        if(b->parent==head){
            b->data=b->parent->data;
            head->data=det;
            det=b->index_in_matrix;
            b->index_in_matrix=b->parent->index_in_matrix;
            head->index_in_matrix=det;
            initial_storer[head->index_in_matrix]=head;
            initial_storer[b->index_in_matrix]=b;
            return initial_storer;
        }
        b->data=b->parent->data;
        b->parent->data=det;
        det=b->index_in_matrix;
        b->index_in_matrix=b->parent->index_in_matrix;
        b->parent->index_in_matrix=det;
        initial_storer[b->parent->index_in_matrix]=b->parent;
        initial_storer[b->index_in_matrix]=b;
        b=hola;
    }
    return initial_storer;
}

//implementing dijkstra using binomial Node
vector<int> dijkstra_binomial(int N,vector<vector<int> > matrix,int s){
    //this is the vector of the pointers which points to the corresponding node and from this we can easily know it's parent
    //it's sibling,it's child and all the other things
    vector<Node *> storing_nodes;
    vector<int> node_in_heap(N,0);
    vector<int> dij(N,-1);
    for(int i=0;i<N;i++){
        if(i!=s-1){
            Node *a;
            a=create_node(999999,i);
            storing_nodes.push_back(a);
        }
        else{
            Node *a;
            a=create_node(0,i);
            storing_nodes.push_back(a);
        }
    }
    //cout<<"hellodon "<<endl;
    //inserting source node in binomial heap
    inserting_a_node(storing_nodes[s-1]);
    dij[s-1]=0;
    int dummy=0;
    Node * minimum_node;
    while(head!=NULL){
       // cout<<"man"<<endl;
        //finding minimum node in a heap
        //cout<<head->data<<endl;
        //if(head->sibling){
          //  cout<<head->sibling->data<<endl;

        //}
        //for(int i=0;i<N;i++){
          //  cout<<storing_nodes[i]->degree<<" ";
        //}
/*
        for(int i=0;i<N;i++){
            if(storing_nodes[i]){
            cout<<storing_nodes[i]->data<<" ";
            if(storing_nodes[i]->sibling){
                cout<<"sibling "<<storing_nodes[i]->sibling->data<<" ";
            }
            if(storing_nodes[i]->parent){
                cout<<"parent"<<storing_nodes[i]->parent->data<<" ";
            }
            if(storing_nodes[i]->child){
                cout<<"child"<<storing_nodes[i]->child->data<<" ";
            }
            }
            cout<<endl;
        }

*/
        minimum_node=finding_the_minimum();
        //deleting the minimum value from the heap and then arranging the heap as it is
        //cout<<"hekko"<<endl;
        head=delete_min();
        //if(head){
          //  cout<<"HI"<<endl;
        //}
        //for(int i=0;i<N;i++){
          //  cout<<storing_nodes[i]->degree<<" ";
        //}
        //cout<<endl;
        //cout<<"hero"<<endl;
        dij[minimum_node->index_in_matrix]=minimum_node->data;
        node_in_heap[minimum_node->index_in_matrix]=0;
        //variable storing the index of minimum value
        //for(int i=0;i<N;i++){
          //  cout<<node_in_heap[i]<<" ";
        //}
        //cout<<endl;
        dummy=minimum_node->index_in_matrix;
        //cout<<"herrr"<<endl;
        for(int j=0;j<N;j++){
            if(matrix[dummy][j]<0){
                dij[j]=-2;
            }
            if(matrix[dummy][j]<999999 && dij[j]==-1){
                if(dij[dummy]+matrix[dummy][j]<storing_nodes[j]->data){
                    storing_nodes[j]->data=dij[dummy]+matrix[dummy][j];
                    if(node_in_heap[j]==0){
                        //cout<<"hellll"<<endl;
                        inserting_a_node(storing_nodes[j]);
        //cout<<"chllle chlo"<<endl;
         /*               for(int i=0;i<N;i++){
            if(storing_nodes[i]){
            cout<<storing_nodes[i]->data<<" ";
            if(storing_nodes[i]->sibling){
                cout<<"sibling "<<storing_nodes[i]->sibling->data<<" ";
            }
            if(storing_nodes[i]->parent){
                cout<<"parent"<<storing_nodes[i]->parent->data<<" ";
            }
            if(storing_nodes[i]->child){
                cout<<"child"<<storing_nodes[i]->child->data<<" ";
            }
            }
            cout<<endl;
        }*/
                        node_in_heap[j]=1;
                    }
                    else{
                           // cout<<j<<" "<<j<<" "<<endl;
                        //value of a node is decreased.Now just percolate up to rearrange all the nodes in its place
                        //update this function as per the convenience
                        storing_nodes=perculate_up(storing_nodes[j],storing_nodes);
                    }
                }
            }
        }
    }
    return dij;
}

//fibonacci heap implementation

struct fibonacci_nodes{
int data;
int color;
int index;
int degree;
struct fibonacci_nodes *right;
struct fibonacci_nodes *left;
struct fibonacci_nodes *parent;
struct fibonacci_nodes *child;
};

fibonacci_nodes * create_a_new_node(int data,int index){
fibonacci_nodes *a;
a=(fibonacci_nodes *)malloc(sizeof(fibonacci_nodes));
a->child=NULL;
a->parent=NULL;
a->degree=0;
a->data=data;
a->index=index;
a->left=NULL;
a->right=NULL;
a->color=0;
return a;
}

fibonacci_nodes * min_pointer;
fibonacci_nodes * fibo_head;

void min_value(fibonacci_nodes *a){
    if(a->data<min_pointer->data){
        min_pointer=a;
    }

    fibonacci_nodes *ff=min_pointer;
    while(a->right!=NULL){
        if(a->right->data<ff->data){
            ff=a->right;
        }
        a=a->right;
    }

    a=min_pointer;
    while(a->left!=NULL){
        if(a->left->data<ff->data){
            ff=a->left;
        }
        a=a->left;
    }
    min_pointer=ff;
}

//insertion of a node in fibonacci heaps
void insertion(fibonacci_nodes * nod){
    nod->right=fibo_head;
    if(fibo_head){
        fibo_head->left=nod;
        fibo_head=nod;
        min_value(fibo_head);
        return;
    }
    else{
        min_pointer=nod;
        fibo_head=nod;
        return;
    }
}

//merging two fibonacci Nodes
fibonacci_nodes * merging_two_fibonacci_Nodes(fibonacci_nodes *a,fibonacci_nodes *b,fibonacci_nodes *k){
    if(a->data<b->data){
        if(a==fibo_head){
            fibonacci_nodes *childd=a->child;
            a->child=b;
            a->right=b->right;
            if(b->right){
                b->right->left=a;
            }
            b->left=NULL;
            b->right=childd;
            if(childd){
            childd->left=b;
            }
            a->degree++;
            b->parent=a;
            fibo_head=a;
            if(a->data==min_pointer->data){
                min_pointer=a;
            }
            return fibo_head;
        }
        fibonacci_nodes *childd=a->child;
        a->child=b;
        a->right=b->right;
        if(b->right){
        b->right->left=a;
        }
        b->left=NULL;
        b->right=childd;
        if(childd){
        childd->left=b;
        }
        a->degree++;
        b->parent=a;
        if(a->data==min_pointer->data){
                min_pointer=a;
        }
        return a;
    }
    else{
        if(a==fibo_head){
        fibonacci_nodes *childd=b->child;
        b->child=a;
        b->left=NULL;
        a->right=childd;
        if(childd){
        childd->left=a;
        }
        b->degree++;
        a->parent=b;
        fibo_head=b;
        if(b->data==min_pointer->data){
                min_pointer=b;
        }
        return fibo_head;
        }
        fibonacci_nodes *childd=b->child;
        b->child=a;
        if(k!=NULL){
        if(k==fibo_head){
            fibo_head->right=b;
            a->left=NULL;
            b->left=fibo_head;
        }
        else{
            a->left=NULL;
            k->right=b;
            b->left=k;
        }
        }
        a->right=childd;
        if(childd){
        childd->left=a;
        }
        b->degree++;
        a->parent=b;
        if(b->data==min_pointer->data){
                min_pointer=b;
        }
        return b;
    }
}

//rearranging fibonacci node
fibonacci_nodes * exchanging_Node(fibonacci_nodes *a,fibonacci_nodes *b,fibonacci_nodes *drum){
    fibonacci_nodes * pp=b->right;
    b->right=a;
    b->left=a->left;
    a->right=pp;
    a->left=b;
    if(drum){
    if(drum==fibo_head){
        fibo_head->right=b;
    }
    else{
        drum->right=b;
    }
    }
    if(pp){
        pp->left=a;
    }
    return b;
}


//maintaining order
//after applying this function on a heap we can say that there are no trees in the heap with same degree
//so this function ensure that there are no two trees with same order
void maintain_order(){
    fibonacci_nodes *dum=fibo_head;
    fibonacci_nodes *drum=NULL;
    while(dum->right!=NULL){
        if(dum->degree==dum->right->degree){
            if(dum==fibo_head){
            fibo_head=merging_two_fibonacci_Nodes(dum,dum->right,dum->left);
            dum=fibo_head;
            }
            else{
                dum=merging_two_fibonacci_Nodes(dum,dum->right,dum->left);
            }
        }
        else if(dum->degree>dum->right->degree){
            if(dum==fibo_head){
            fibo_head=exchanging_Node(dum,dum->right,dum->left);
            dum=fibo_head;
            }
            else{
                dum=exchanging_Node(dum,dum->right,dum->left);
            }
        }
        else{
            dum=dum->right;
        }
    }
    return;
}


//remove the node from where it is and insert it again before the head
void remove_colored_node(fibonacci_nodes *a){
    fibonacci_nodes *dummy=a;
    if(a->left==NULL){
        a->parent->child=a->right;
        if(a->right){
            a->right->left=NULL;
        }
        a->right=NULL;
    }
    a->color=0;
    if(a->left){
        a->left->right=a->right;
        if(a->right){
            a->right->left=a->left;
        }
        a->right=NULL;
        a->left=NULL;
    }
    a->parent=NULL;
    a->right=fibo_head;
    a->left=NULL;
    fibo_head->left=a;
    fibo_head=a;
    if(fibo_head->data<min_pointer->data){
        min_pointer=fibo_head;
    }
    return;
}

//this is the function which removes all the 2 color nodes and maintain the min pointer as it is
void remove_the_node_with_that_color(fibonacci_nodes * a){
    if(a->color==2 && a->parent==NULL){
        a->color=0;
        return;
    }
    else if(a->color==1 || a->color==0){
        return;
    }
    else{
        if(a->right && a->child){
        }
        a->parent->color+=1;
        fibonacci_nodes *cc=a->parent;
        remove_colored_node(a);
        remove_the_node_with_that_color(cc);
        return;
    }
}

//changing the value
//if value is greater than the parent maintain it as it is otherwise just do the same operations as we do in delete minimum
//this is the function which check whether we have to remove that node or do we have to keep that node in its place
//like what we have to do
//it just instructs what we have to do if we change the value of anything
void value_change(fibonacci_nodes * node,int value){
    if(node->parent==NULL){
        if(fibo_head==node){
            fibo_head->data=value;
        }
        else{
        node->data=value;
        }
        min_value(fibo_head);
    }
    else if(node->parent->data<value){
        node->data=value;
    }
    else{
        node->data=value;
        node->color=2;
        remove_the_node_with_that_color(node);
        min_value(fibo_head);
    }
    return;
}

//deleting the minimum value node from the heap
void deleting_min(){
    if(!min_pointer){
        return;
    }
    else if(min_pointer->left==NULL && min_pointer->right==NULL && min_pointer->child==NULL){
        min_pointer=NULL;
        fibo_head=NULL;
        return;
    }

    else if(min_pointer->child==NULL){
        fibonacci_nodes *a=min_pointer->left;
        if(min_pointer->left){
            if(min_pointer->left==fibo_head){
                fibo_head->right=min_pointer->right;
            }
            else{
                min_pointer->left->right=min_pointer->right;
            }
        }
        if(min_pointer->right){
            min_pointer->right->left=a;
        }
        if(fibo_head==min_pointer){
            fibo_head=min_pointer->right;
        }
        fibonacci_nodes *b=min_pointer;
        b->right=NULL;
        b->left=NULL;
        b=NULL;
        min_pointer=fibo_head;
        min_value(fibo_head);
    }
    else if(min_pointer->left==NULL && min_pointer->right==NULL){
        fibonacci_nodes *a=min_pointer->child;
        min_pointer->child=NULL;
        fibonacci_nodes *ff=a;
        a->parent=NULL;
        min_pointer=a;
        fibo_head=a;
        while(a->right!=NULL){
            a->right->parent=NULL;
            if(a->right->data<ff->data){
                ff=a;
            }
            a=a->right;
        }
        min_pointer=ff;
    }
    else{
        fibonacci_nodes *a=min_pointer->left;
        if(min_pointer->left){
            if(min_pointer->left==fibo_head){
                fibo_head->right=min_pointer->right;
            }
            else{
                min_pointer->left->right=min_pointer->right;
            }
        }
        if(min_pointer->right){
            min_pointer->right->left=a;
        }
        if(fibo_head==min_pointer){
            fibo_head=min_pointer->right;
        }
        fibonacci_nodes *childi=min_pointer->child;
        a=min_pointer->child;
        min_pointer->child=NULL;
        min_pointer->right=NULL;
        min_pointer->left=NULL;
        a->parent=NULL;
        min_pointer=a;
        if(a->right){
            while(a->right->right!=NULL){
                a->right->parent=NULL;
                a=a->right;
            }
            a=a->right;
            a->parent=NULL;
        }
        a->right=fibo_head;
        fibo_head->left=a;
        fibo_head=childi;
        min_pointer=fibo_head;
        min_value(fibo_head);
    }
    maintain_order();
    min_value(fibo_head);
    return;
}


vector<int> dijkstra_fibonacci(int N,vector<vector<int> > matrix,int s){
    //this is the vector of the pointers which points to the corresponding node and from this we can easily know it's parent
    //it's sibling,it's child and all the other things
    vector<fibonacci_nodes *> storing_nodes;
    vector<int> node_in_heap(N,0);
    vector<int> dij(N,-1);
    for(int i=0;i<N;i++){
        if(i!=s-1){
            fibonacci_nodes *a;
            a=create_a_new_node(999999,i);
            storing_nodes.push_back(a);
        }
        else{
            fibonacci_nodes *a;
            a=create_a_new_node(0,i);
            storing_nodes.push_back(a);
        }
    }
    //inserting source node in binomial heap
    insertion(storing_nodes[s-1]);
    dij[s-1]=0;
    int dummy=0;
    fibonacci_nodes * minimum_node;
    while(fibo_head!=NULL){
        //finding minimum node in a heap
        minimum_node=min_pointer;
        //deleting the minimum value from the heap and then arranging the heap as it is
        dij[minimum_node->index]=minimum_node->data;
        deleting_min();
        node_in_heap[minimum_node->index]=0;
        //variable storing the index of minimum value
        dummy=minimum_node->index;
        for(int j=0;j<N;j++){
            if(matrix[dummy][j]<0){
                dij[j]=-2;
            }
            if(matrix[dummy][j]<999999 && dij[j]==-1){

                if(dij[dummy]+matrix[dummy][j]<storing_nodes[j]->data){
                    storing_nodes[j]->data=dij[dummy]+matrix[dummy][j];
                    if(node_in_heap[j]==0){
                        insertion(storing_nodes[j]);
                        node_in_heap[j]=1;
                    }
                    else{
                        //value of a node is decreased.Now just percolate up to rearrange all the nodes in its place
                        //update this function as per the convenience
                        value_change(storing_nodes[j],dij[dummy]+matrix[dummy][j]);
                    }
                }
            }
        }
    }
    return dij;
}


int main(int argc,char* argv[]){
    int t;
    cin>>t;
    vector<double>tim;
    int myInt=0;
    int dummy=0;
    if(argc==1){
    	dummy=1;
    }
    else{
	    myInt=atoi(argv[1]);
	}
    while(t--){
        head=NULL;
        fibo_head=NULL;
        min_pointer=NULL;
        int N,D;
        cin>>N>>D;
        vector<vector<int> > bellman_matrix(N,vector<int> (N,0));
        vector<vector<int> > matrix(N,vector<int> (N,0));
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                cin>>matrix[i][j];
            }
        }
        bellman_matrix=matrix;
        //if implementing by array
        for(int i=0;i<N;i++){
            bellman_matrix[i].push_back(999999);
        }
        vector<int> bellma(N+1,0);
        bellman_matrix.push_back(bellma);
        //implementing bellman ford algorithm to make all the edges positive
        vector<int> final_edges(N+1,0);
        final_edges=implementing_bellman_ford(N+1,bellman_matrix,N+1);
        int come=0;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(i!=j && matrix[i][j]!=999999){
                    matrix[i][j]+=final_edges[i]-final_edges[j];
                }
                if(matrix[i][j]<0){
                    come=1;
                }
            }
        }
        


        if(myInt==1 && dummy==0){
        	clock_t starting=clock();
        	if(come==1 || final_edges[0]==-10000000){
		        cout<<-1<<endl;
		    }
		    else{
        	vector<int> heap_maker;
            
            for(int j=0;j<N;j++){
                heap_maker=array_dijkstra(N,matrix,j+1);
                for(int i=0;i<N;i++){
                if(heap_maker[i]>=0){
                if(j!=i){
                    heap_maker[i]+=final_edges[i]-final_edges[j];
                }
                cout<<heap_maker[i]<<" ";
                }
                else if(heap_maker[i]==-1){
                    cout<<"999999 ";
                }
                else{
                 cout<<"-1 ";
                }

            }
            cout<<endl;
            }
            }
            clock_t ending=clock();
            tim.push_back((double)(ending-starting)/CLOCKS_PER_SEC);
        }

        else if(myInt==2 && dummy==0){
            clock_t starting=clock();
            if(come==1 || final_edges[0]==-10000000){
		        cout<<-1<<endl;
		    }
		    else{
        	vector<int> heap_maker;
            for(int j=0;j<N;j++){
                heap_maker=dijkstra_binary(N,matrix,j+1);
                for(int i=0;i<N;i++){
                if(heap_maker[i]!=999999){
                if(j!=i){
                    heap_maker[i]+=final_edges[i]-final_edges[j];
                }
                cout<<heap_maker[i]<<" ";
                }
                else if(heap_maker[i]==999999){
                    cout<<"999999 ";
                }
                else{
                    cout<<"-1 ";
                }

            }
            cout<<endl;
            }
            }
             clock_t ending=clock();
             tim.push_back((double)(ending-starting)/CLOCKS_PER_SEC);
        }

        else if(myInt==3 && dummy==0){
            clock_t starting=clock();
            if(come==1 || final_edges[0]==-10000000){
		        cout<<-1<<endl;
		    }
		    else{
        	vector<int> heap_maker;
            for(int j=0;j<N;j++){
                head=NULL;
                heap_maker=dijkstra_binomial(N,matrix,j+1);
                for(int i=0;i<N;i++){
                if(heap_maker[i]>=0){
                if(j!=i){
                    heap_maker[i]+=final_edges[i]-final_edges[j];
                }
                cout<<heap_maker[i]<<" ";
                }
                else if(heap_maker[i]==-1){
                    cout<<"999999 ";
                }
                else{
                 cout<<"-1 ";
                }

            }
            cout<<endl;
            }
            }
             clock_t ending=clock();
             tim.push_back((double)(ending-starting)/CLOCKS_PER_SEC);
        }

        else{
            clock_t starting=clock();
            if(come==1 || final_edges[0]==-10000000){
		        cout<<-1<<endl;
		    }
		    else{
        	vector<int> heap_maker;
            for(int j=0;j<N;j++){
                //cout<<"hello"<<endl;
                min_pointer=NULL;
                fibo_head=NULL;
                heap_maker=dijkstra_fibonacci(N,matrix,j+1);
                //cout<<"his"<<endl;
                for(int i=0;i<N;i++){
                if(heap_maker[i]>=0){
                if(j!=i){
                    heap_maker[i]+=final_edges[i]-final_edges[j];
                }
                cout<<heap_maker[i]<<" ";
                }
                else if(heap_maker[i]==-1){
                    cout<<"999999 ";
                }
                else{
                 cout<<"-1 ";
                }

            }
            cout<<endl;
            }
            }
             clock_t ending=clock();
             tim.push_back((double)(ending-starting)/CLOCKS_PER_SEC);
        }

        /*

        auto start=high_resolution_clock::now();

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        cout<<endl;
        cout <<"Time taken by function:"<< duration.count() << " microseconds" << endl;
        //if implementing using binary heaps
        for(int i=0;i<N;i++){
            if(i==s-1){
                heap_maker.push_back(0);
            }
            else{
                heap_maker.push_back(999999);
            }
        }
        clock_t starti, endi;
        starti=clock();
        heap_maker=implementing_dijkstra(heap_maker,N,matrix,s);
        endi=clock();
        double time_taken = double(endi - starti) / double(CLOCKS_PER_SEC);
        for(int i=0;i<N;i++){
            cout<<heap_maker[i]<<" ";
        }
        cout<<endl;
        cout << "Time taken by function: "<< time_taken << " microseconds" << endl;
        //if implementing using binomial heaps
        start=high_resolution_clock::now();
        heap_maker=dijkstra_binomial(N,matrix,s);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        for(int i=0;i<N;i++){
            if(heap_maker[i]>=0){
            cout<<heap_maker[i]<<" ";
            }
            else if(heap_maker[i]==-1){
                cout<<"999999 ";
            }
            else{
             cout<<"-1 ";
            }
        }
        cout<<endl;
        cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
        start=high_resolution_clock::now();
        heap_maker=dijkstra_fibonacci(N,matrix,s);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        for(int i=0;i<N;i++){
            if(heap_maker[i]>=0){
            cout<<heap_maker[i]<<" ";
            }
            else if(heap_maker[i]==-1){
                cout<<"999999 ";
            }
            else{
             cout<<"-1 ";
            }
        }
        cout<<endl;
        cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
        */
    
    }
	for(int i=0;i<tim.size();i++){
            cout<<fixed<<tim[i]<<setprecision(6)<<" ";
        }
        cout<<endl;
}

