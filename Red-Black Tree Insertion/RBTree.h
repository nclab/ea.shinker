#include <cstdlib>
using namespace std;

enum Color {RED, BLACK, DOUBLE_BLACK};

int evaluation=0;
int rbt=0,ncount=0;

struct Node
{
    int key;
    int color;
    Node *left, *right, *parent;
    Node(int k){
    	this->key=k;
    	this->color=RED;
    	this->parent=NULL;
		this->left=NULL;
		this->right=NULL;
		ncount++;
	}
	//copy constructor
	Node* copy(){
		Node *root=new Node(this->key);
		root->color=this->color;
		if(this->left->left!=NULL) root->left=this->left->copy();
		if(this->right->right!=NULL) root->right=this->right->copy();
		return root;
	}
	
};

class RBTree
{
	public:
		Node* root;
		Node* neel;
		
		// constructor
		RBTree(){
        		neel = new Node(0);   
		        neel->color = BLACK;     
        		root = neel;            
        		root->parent = neel;
    		};
    	// copy constructor
    	void copy(RBTree *T){
    		root=T->root->copy();
    		Setneel();
    		Setparent();
    		rbt++;
		}
    	// destructor
    	~RBTree(){
    		DestroyRecursive(this->root);
    		delete neel;
    		rbt--;
		}
		// delete node by node
		void DestroyRecursive(Node* node){
			if(node!=neel){
				if(node->left!=neel) DestroyRecursive(node->left);
				if(node->right!=neel) DestroyRecursive(node->right);
				delete node;
				ncount--;
			}
		}
    	
		//balance function for erroneous insertion
		Node* balance (Node *&t) {
			Node *t1,*t2,*t3,*t4,*t5;
			if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel &&
				 t->left->left!=NULL && t->left->left!=neel &&
				 t->color==BLACK && t->left->color==RED && t->left->left->color==RED &&
				 t->left->left->left==neel && t->left->left->right==neel &&
				 t->left->right==neel && t->right==neel){
				 	t->left->right=t;
				 	t->left->parent=NULL;
				 	t=t->left;
				 	t->right->left=neel;
				 	t->color=BLACK;
				 	t->right->color=RED;
				 	//cout << "case LL" << endl;
				 	return t;
				 }
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && 
				 t->left->right!=NULL && t->left->right!=neel &&
				 t->color==BLACK && t->left->color==RED && t->left->right->color==RED &&
				 t->left->left==neel && t->left->right->left==neel &&
				 t->left->right->right==neel && t->right==neel){
				 	t->left->right->left=t->left;
				 	t->left->right->right=t;
				 	t->left->right->parent=NULL;
				 	t=t->left->right;
				 	t->left->parent=t;
				 	t->left->right=neel;
				 	t->right->parent=t;
				 	t->right->left=neel;
				 	t->color=BLACK;
				 	t->right->color=RED;
					//cout << "case LR" << endl;
				 	return t;
				 }
			else if ( t!=NULL && t!=neel && t->right!=NULL && t->right!=neel &&
				 t->right->left!=NULL && t->right->left!=neel &&
				 t->color==BLACK && t->right->color==RED && t->right->left->color==RED &&
				 t->left==neel && t->right->left->left==neel &&
				 t->right->left->right==neel && t->right->right==neel){
				 	t->right->left->left=t;
				 	t->right->left->right=t->right;
				 	t->right->left->parent=NULL;
				 	t=t->right->left;
				 	t->left->parent=t;
				 	t->left->right=neel;
				 	t->right->parent=t;
				 	t->right->left=neel;
				 	t->color=BLACK;
				 	t->left->color=RED;
				 	//cout << "case RL" << endl;
				 	return t;
				 }
			else if ( t!=NULL && t!=neel && t->right!=NULL && t->right!=neel &&
				 t->right->right!=NULL && t->right->right!=neel && 
				 t->color==BLACK && t->right->color==RED && t->right->right->color==RED &&
				 t->left==neel && t->right->left==neel &&
				 t->right->right->left==neel && t->right->right->right==neel){
				 	t->right->left=t;
				 	t->right->parent=NULL;
				 	t=t->right;
				 	t->left->right=neel;
				 	t->left->parent=t;
				 	t->color=BLACK;
				 	t->left->color=RED;
				 	//cout << "case RR" << endl;
				 	return t;
				 }
			//case 1 (1/4)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && 
				 t->left->left!=NULL && t->left->left!=neel && t->right!=NULL && t->right!=neel && 
				 t->color==BLACK && t->left->color==RED &&
				 t->left->left->color==RED && t->right->color==RED){
					t->color=RED;
					t->left->color=BLACK;
					t->right->color=BLACK;
					//cout << "case 1-1" << endl;
					return t;	
				 }
			//case 1 (2/4)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->left->right!=NULL &&
				 t->left->right!=neel && t->right!=NULL && t->right!=neel && 
				 t->color==BLACK && t->left->color==RED &&
				 t->left->right->color==RED && t->right->color==RED){
				 	t->color=RED;
					t->left->color=BLACK;
					t->right->color=BLACK;
					//cout << "case 1-2" << endl;
					return t;
				 }
			//case 1 (3/4)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->right!=NULL && t->right!=neel && 
				 t->right->left!=NULL && t->right->left!=neel &&
				 t->color==BLACK && t->left->color==RED &&
				 t->right->color==RED && t->right->left->color==RED){
				 	t->color=RED;
					t->left->color=BLACK;
					t->right->color=BLACK;
					//cout << "case 1-3" << endl;
					return t;
				 }
			//case 1 (4/4)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->right!=NULL && t->right!=neel && 
				 t->right->right!=NULL && t->right->right!=neel &&
				 t->color==BLACK && t->left->color==RED &&
				 t->right->color==RED && t->right->right->color==RED){
				 	t->color=RED;
					t->left->color=BLACK;
					t->right->color=BLACK;
					//cout << "case 1-4" << endl;
					return t;
				 }
			//case 2 (1/2)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->left->right!=NULL &&
				 t->left->right!=neel &&  t->right!=NULL &&t->right!=neel &&
				 t->color==BLACK && t->left->color==RED &&
				 t->left->right->color==RED && t->right->color==BLACK){
				 	t2=t->left->right->left;
				 	t3=t->left->right->right;
					t->left->right->left=t->left;
				 	t->left->right->right=t;
				 	t->left->right->parent=NULL;
				 	t=t->left->right;
				 	t->left->parent=t;
				 	t->left->right=t2;
				 	t->right->parent=t;
				 	t->right->left=t3;
				 	t->color=BLACK;
				 	t->right->color=RED;
				 	//cout << "case 2-1" << endl; 
				 	return t;
				 }
			//case 2 (2/2)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel &&
				 t->right!=NULL && t->right!=neel && t->right->left!=NULL && t->right->left!=neel &&
				 t->color==BLACK && t->left->color==BLACK &&
				 t->right->color==RED && t->right->left->color==RED){
				 	t3=t->right->left->left;
				 	t4=t->right->left->right;
				 	t->right->left->left=t;
				 	t->right->left->right=t->right;
				 	t->right->left->parent=NULL;
				 	t=t->right->left;
				 	t->left->parent=t;
				 	t->left->right=t3;
				 	t->right->parent=t;
				 	t->right->left=t4;
				 	t->color=BLACK;
				 	t->left->color=RED;
				 	t->left->left->color=RED;//incorrect, should be removed
				 	//cout << "case 2-2" << endl;
				 	return t;
				 }
			//case 3 (1/2)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->left->left!=NULL &&
				 t->left->left!=neel &&  t->right!=NULL &&t->right!=neel &&
				 t->color==BLACK && t->left->color==RED &&
				 t->left->left->color==RED && t->right->color==BLACK){
				 	t3=t->left->right;
				 	t->left->right=t;
				 	t->left->parent=NULL;
				 	t=t->left;
				 	t->right->parent=t;
				 	t->right->left=t3;
				 	t->color=BLACK;
				 	t->right->color=RED;
				 	//cout << "case 3-1" << endl;
				 	return t;
				 }
			//case 3 (2/2)
			else if ( t!=NULL && t!=neel && t->left!=NULL && t->left!=neel && t->right!=NULL &&
				 t->right!=neel && t->right->right!=NULL && t->right->right!=neel && 
				 t->color==BLACK && t->left->color==BLACK &&
				 t->right->color==RED && t->right->right->color==RED){
				 	t3=t->right->left;
				 	t->right->left=t;
				 	t->right->parent=NULL;
				 	t=t->right;
				 	t->left->right=t3;
				 	t->left->parent=t;
				 	t->color=BLACK;
				 	t->left->color=RED;
				 	//cout << "case 3-2" << endl;
				 	return t;
				 }
			else{
				//cout << "ELSE" << endl;
				return t;
			} 
		}
		//BST part of insertion
		Node* ins (int k,Node *t){
			if (t==neel){
				Node* insnode=new Node(k);
				SetneelAux(insnode);
				return insnode;
			}
			if (k>t->key){
				t->right=ins(k,t->right);
				t->right->parent=t;
				t=balance(t);
				SetneelAux(t);
			}else if (k<t->key){
				t->left=ins(k,t->left);
				t->left->parent=t;
				t=balance(t);
				SetneelAux(t);
			}
			return t;
		}
		// make the color of a node BLACK.
		void makeblack(Node *t){
			if (t!=NULL){
				t->color=BLACK;
			}
		}
		// insert function under erroneous balance function.
		void insert(int k){
			root=ins(k,root);
			Setneel();
			makeblack(root);
		}

		//implement verified delete and insert operation.
		//Both base on material in "Introduction to Algorithms"(third edition).
		////////////
		//DELETION//
		////////////
    	void DeleteRBT(int key){
    		Node *delete_node=Search(key);
    		if(delete_node!=neel) RBdelete(delete_node);
    		else cout << key << " is not found." << endl;
		}
    	void RBtransplant(Node* u,Node* v){
    		if(u->parent==neel) root=v;
    		else if (u==u->parent->left) u->parent->left=v;
    		else u->parent->right=v;
    		v->parent=u->parent;
		}
		void RBdelete(Node* z){
			Node *x,*y;
			Node *d;
			d = z;
			y = z;
			int yoc=y->color;
			if (z->left==neel){
				x = z->right;
				RBtransplant(z,z->right);
			}else if(z->right==neel){
				x = z->left;
				RBtransplant(z,z->left);
			}else{
				y = Leftmost(z->right);
				yoc = y->color;
				x = y->right;
				if (y->parent==z) x->parent = y;
				else{
					RBtransplant(y,y->right);
					y->right=z->right;
					y->right->parent=y;
				}
				RBtransplant(z,y);
				y->left=z->left;
				y->left->parent=y;
				y->color=z->color;				
			}
			if (yoc==BLACK) RBdelete_fixup(x);
			delete d;
			ncount--;
		}
		void RBdelete_fixup(Node* x){
			Node *w;
			while (x!=root&&x->color==BLACK){
				if(x==x->parent->left){
					w=x->parent->right;
					if(w->color==RED){
						w->color=BLACK;
						x->parent->color=RED;
						LeftRotation(x->parent);
						w = x->parent->right;
					}
					if(w->left->color==BLACK&&w->right->color==BLACK){
						w->color=RED;
						x=x->parent;
					}else{
						if(w->right->color==BLACK){
							w->left->color=BLACK;
							w->color=RED;
							RightRotation(w);
							w=x->parent->right;
						}
						w->color=x->parent->color;
						x->parent->color=BLACK;
						w->right->color=BLACK;
						LeftRotation(x->parent);
						x=root;
					}		
					
				}else{
					w=x->parent->left;
					if(w->color==RED){
						w->color=BLACK;
						x->parent->color=RED;
						RightRotation(x->parent);
						w = x->parent->left;
					}
					if(w->left->color==BLACK&&w->right->color==BLACK){
						w->color=RED;
						x=x->parent;
					}else{
						if(w->left->color==BLACK){
							w->right->color=BLACK;
							w->color=RED;
							LeftRotation(w);
							w=x->parent->left;
						}
						w->color=x->parent->color;
						x->parent->color=BLACK;
						w->left->color=BLACK;
						RightRotation(x->parent);
						x=root;
					}					
				}
			}
			x->color=BLACK; 
		}
		/////////////
		//INSERTION//
		/////////////
		void InsertRBT(int key){
    		
    		Node *y = neel;
    		Node *x = root;
    		Node *insert_node = new Node(key);

    		while (x != neel) {     
        		y = x;
        		if (insert_node->key < x->key){
            		x = x->left;
        		}
        		else if (insert_node->key > x->key){
            		x = x->right;
        		}else{
        			cout << key << " is already inserted." << endl;
        			delete insert_node;
        			return;
				}
    		}
		
    		insert_node->parent = y;
		
    		if (y == neel){
        		this->root = insert_node;
    		}
    		else if (insert_node->key < y->key){
        		y->left = insert_node;
    		}
    		else{
        		y->right = insert_node;
    		}
		    
    		
    		insert_node->left = neel;
    		insert_node->right = neel;
    		insert_node->color = RED;             
        
		    InsertFixedUpRBT(insert_node);     
		}
		void InsertFixedUpRBT(Node *current){

    		
    		while (current->parent->color == RED) {   
		
        		
        		if (current->parent == current->parent->parent->left) { 
            		Node *uncle = current->parent->parent->right;
            		
            		if (uncle->color == RED) {
                		current->parent->color = BLACK;
                		uncle->color = BLACK;
                		current->parent->parent->color = RED;             
                		current = current->parent->parent;
            		}
            		
            		else {  
                		if (current == current->parent->right){     
                    		current = current->parent;
                    		LeftRotation(current);
                		}
                		
                		current->parent->color = BLACK;                      
                		current->parent->parent->color = RED;             
                		RightRotation(current->parent->parent);
            			}
        		}
        	
        		else {  
            		Node *uncle = current->parent->parent->left;
            	
            		if (uncle->color == RED) {
                		current->parent->color = BLACK;
                		uncle->color = BLACK;
                		current->parent->parent->color = RED;              
                		current = current->parent->parent;
            		}
            		
            		else {
                		if (current == current->parent->left) {    
                    		current = current->parent;
                    		RightRotation(current);
                		}
                		
                		current->parent->color = BLACK;
                		current->parent->parent->color = RED;
                		LeftRotation(current->parent->parent);
            		}
        		}
    		}
    		root->color = BLACK;    
		}
		void LeftRotation(Node *x){

    		Node *y = x->right;         

    		x->right = y->left;        

    		if (y->left != neel){           
        		y->left->parent = x;       
    		}

		    y->parent = x->parent;           

		    if (x->parent == neel){             
		        root = y;          
    		}         
    		else if (x == x->parent->left){ 
    		    x->parent->left = y;        
    		}
    		else{                                
        		x->parent->right = y;      
    		}
    		y->left = x;                   
    		x->parent = y;                      
		}
		void RightRotation(Node *y){

		    Node *x = y->left;           

		    y->left = x->right;           
    		if (x->right != neel){           
        		x->right->parent = y;
    		}
    		x->parent = y->parent;                
		                                         
    		if (y->parent == neel){              
        		root = x;          
    		}              
    		else if (y == y->parent->left){ 
        		y->parent->left = x;         
    		}
    		else{                                
        		y->parent->right = x;        
    		}
    		x->right = y;                    
    		y->parent = x;                        
		}
		//return the node that contains the minimum key value within current subtree.
		Node* Leftmost(Node *current){
    
    		while (current->left != neel){
        		current = current->left;
    		}
    		return current;
		}
		Node* Successor(Node *current){
    
    		if (current->right != neel){
        		return Leftmost(current->right);
   			}
    
	    	Node *new_node = current->parent;
    
    		while (new_node != neel && current == new_node->right) {
        		current = new_node;
        		new_node = new_node->parent;
    		}
    
    		return new_node;
		}
		//Look up a node with specific key value in the tree.
		Node* Search(int k){
			return SearchAux(root,k);
		}
		//Aux function for search.
		Node* SearchAux(Node* current,int k){
			if(current==neel||k==current->key) return current;
			if(k<current->key) return SearchAux(current->left,k);
			else return SearchAux(current->right,k);			
		}

		
		//property 1: The color of root node must be BLACK.
		bool RootIsBlack(Node *t){
			if (t==neel) return true;//NULL
			return (t->color==BLACK);
		}
		//property 2: RBT is also a binary search tree.
		bool IsBST(Node *t){
			if (t==neel) return true;//NULL
			if ((t->left!=neel)&&(t->right!=neel)){
				return (t->left->key < t->key)&&(t->right->key > t->key)&&IsBST(t->left)&&IsBST(t->right);
			}else if((t->left!=neel)&&(t->right==neel)){
				return (t->left->key < t->key)&&IsBST(t->left)&&IsBST(t->right);
			}else if((t->left==neel)&&(t->right!=neel)){
				return (t->right->key > t->key)&&IsBST(t->left)&&IsBST(t->right);
			}else return true;
		}
		//property 3: There isn't any consecutive RED nodes in the tree.
		bool NoConsecutiveRed(Node *t){
			if (t==neel) return true;
			if (t->left!=neel && t->right!=neel){
				return (t->color+t->left->color)!=0 && (t->color+t->right->color)!=0 && NoConsecutiveRed(t->left) && NoConsecutiveRed(t->right);
			}else if (t->left!=neel && t->right==neel){
				return (t->color+t->left->color)!=0 && NoConsecutiveRed(t->left);
			}else if (t->left==neel && t->right!=neel){
				return (t->color+t->right->color)!=0 && NoConsecutiveRed(t->right);
			}else return true;
		}
		//aux function for APSB, return the numbur of BLACK nodes in a path from root to leaf.
		int BlackNodeNum(Node *t){
			if(t==neel) return 1;
			int hl,hr;
			hl=BlackNodeNum(t->left);
			hr=BlackNodeNum(t->right);
			if(hl==hr){
				if(t->color==BLACK) return hl+1;
				else return hl;
			}else return -1;			
		}
		//property 4: Every path from root to leaf contains the same amount of BLACK nodes.
		bool AllPathSameBlack(Node *t){
			if(BlackNodeNum(t)>=0) return true;
			else return false;
		}
		//property 5: Leaf nodes are BLACK, since we can take NULL as a BlACK node.
		//Aux function used to check parent pointer of every node is set.
		bool ParentCheck(Node *t){
			if (t==neel) return true;
			if (t->left!=neel&&t->right!=neel){
				return t->left->parent==t&&t->right->parent==t&&ParentCheck(t->left)&&ParentCheck(t->right);
			}else if (t->left!=neel&&t->right==neel){
				return t->left->parent==t&&ParentCheck(t->left);
			}else if (t->left==neel&&t->right!=neel){
				return t->right->parent==t&&ParentCheck(t->right);
			}else return true;
		}
		//Parent pointer checker.
		bool ParentVerification(){
			return root->parent==neel&&ParentCheck(root);
		}
		//Red-Black tree checker.
		bool IsRBT(){
			evaluation++;
			if(!RootIsBlack(root)){
				//cout << "The root of this tree is not black!" << endl;
				return false;
			}
			if(!IsBST(root)){
				//cout << "This tree is not a standard BST!" << endl;
				return false;
			}
			if(!NoConsecutiveRed(root)){
				//cout << "There are some consecutive RED nodes!" << endl;
				return false;
			}
			if(!AllPathSameBlack(root)){
				//cout << "Some paths from roof to leaf contain different amount of BLACK nodes!" << endl;
				return false;
			}
			return true;
		}
		//counterexample checker.
		bool IsCE(int inum){
			RBTree *test=new RBTree();
			test->root=this->root->copy();
			test->Setneel();
			test->Setparent();
			if(!test->IsRBT()){
				cout << "This candidate is not a red-black tree!!!" << endl;
				delete test;
				return false;
			}else{
				test->insert(inum);
				if(!test->IsRBT()){//a counterexample
					delete test;
					return true;
				}else{//not a counterexample
					delete test;
					return false;
				}
			}	
		}
		
		//aux for preorder
		void pre(Node *t){
			if (t==neel) return;
			cout << "(" << t->color << "," << t->key << ")" << endl;
			pre(t->left);
			pre(t->right);
		}
		//preorder traverse through RBT.
		void preorder(){
			cout << "-------------------" << endl;
			pre(root);
			cout << "-------------------" << endl;
		}
		//aux for inorder
		void in(Node *t){
			if (t==neel) return;
			in(t->left);
			cout << "(" << t->color << "," << t->key << ")" << endl;
			in(t->right); 
		}
		//inorder traverse through RBT.
		void inorder(){
			cout << "-------------------" << endl;
			in(root);
			cout << "-------------------" << endl;
		}
		//return the size of a RBTree, leaf nodes(NULL) are also counted.
		int size(Node *t){
			if (t==neel) return 1;
			else return size(t->left)+size(t->right)+1;
		}
		//return total amount of key in a RBTree, since leaf nodes don't have any key value.
		int keynum(){
			return (size(root)-1)/2;
		}
		//Make all undefined leaf nodes (point to NULL at first) point to neel.
		void Setneel(){
			this->root->parent=this->neel;
			SetneelAux(this->root);
		}
		//Aux for set neel function
		void SetneelAux(Node* current){
			if(current==this->neel) return;
			if(current!=NULL){
				if(current->left==NULL) current->left=this->neel;
				if(current->right==NULL) current->right=this->neel;
				SetneelAux(current->left);
				SetneelAux(current->right);
			}
		}
		//Fix parent pointer for non function call built RBT
		void Setparent(){
			this->root->parent=this->neel;
			SetparentAux(this->root);
		}
		//Aux for set parent function
		void SetparentAux(Node* current){
			if(current==this->neel) return;
			if(current->left!=this->neel) current->left->parent=current;
			if(current->right!=this->neel) current->right->parent=current;
			SetparentAux(current->left);
			SetparentAux(current->right);
		}
		
};

//below are some function for general and EA shrinkers.
//store counterexample in a txt file.
void writetofile(Node* cur,Node* neel,ostream &file){
	if(cur==neel) { file << "E ";return;}
	file << "( T ";
	if(cur->color==0) file << "Red ";
	else file << "Black ";
	writetofile(cur->left,neel,file);
	file << cur->key << " ";
	writetofile(cur->right,neel,file);
	file << ") ";
}
//dynamic file naming.
string filename(const string& basename, int index, const string& ext){
	ostringstream result;
  	result << basename << index << ext;
  	return result.str();
}
//random permutation list (0,n-1)
int* random(int n) 
{
    int* r = new int[n];
    // initial range of numbers
    for(int i=0;i<n;++i){
        r[i]=i;
    }
    // shuffle
    for (int i = n-1; i >= 0; --i){
    	//generate a random number [0, n-1]
    	int j = rand() % (i+1);

    	//swap the last element with element at random index
    	int temp = r[i];
    	r[i] = r[j];
    	r[j] = temp;
	}
  return r;
}
//bubble sort depends on tree's fitness value
void fitnessbubble(RBTree** P,int *fitness,int length){
	int i,j;
	for(i=0;i<length-1;i++){
		for(j=0;j<length-i-1;j++){
			if(fitness[j] > fitness[j+1]){
				RBTree* temp=P[j];
				P[j]=P[j+1];
				P[j+1]=temp;
				int t=fitness[j];
				fitness[j]=fitness[j+1];
				fitness[j+1]=t;
			}
		}
	}
}
//update fitness value of the population
void updatefitness(RBTree** P,int *fitness,int length,int penalty,int inum){
	int i;
	for(i=0;i<length;i++){
		//cout << "uf" << i << "//";
		if(!P[i]){
			//cout << "!P[i]";
			fitness[i]=penalty*2;
		}else{
			//if(P[i]->root->left==NULL)cout << "P[i]";
			if(P[i]->IsCE(inum)) fitness[i]=P[i]->keynum();
			else fitness[i]=P[i]->keynum()+penalty;
		}
	}
}
//build an array consist of all key within a red-black tree.
int* FetchAllKey(RBTree *t,int keynum){
	int *element = new int[keynum];
	int i=0;
	stack<Node *> IO;
  	Node *cur = t->root;
  	IO.push(cur);
  	while ( !IO.empty()) {
    	if (cur!=t->neel) {
        	IO.push(cur);
        	cur = cur->left;
    	} else {
        	Node *node = IO.top();
        	IO.pop();
        	if(i<keynum) element[i++]=node->key;
        	cur = node->right;
    	}
    }
    return element;
}
//aux function for crossover
void intersec(RBTree *child,int *PAK,int Akeynum,RBTree *PB){
	int i;
	int *index=random(Akeynum);
	for(i=0;i<Akeynum;i++){
		//If key in parent A is not found in parent B, delete that key from child.
		//Random index allow searching and deleting in any order.
		if(PB->Search(PAK[index[i]])==PB->neel) child->DeleteRBT(PAK[index[i]]);
	}
	delete index;
	return;
}
//crossover between selected parents. 
RBTree* crossover(RBTree *PA,RBTree *PB){
	RBTree *child=new RBTree();
	child->copy(PA);
	intersec(child,FetchAllKey(PA,PA->keynum()),PA->keynum(),PB);
	if(child->root->right==NULL) return NULL;
	return child;
}
//fork tree t, randomly delete at most n+1 node, return the address for new tree.
RBTree* randomdelete(RBTree *t,int n){
	//cout <<"*IN rd*";
	int *element,*dindex;
	int dnum,i;

	//store all key in t.
	element=FetchAllKey(t,t->keynum());
	//cout << "afak*";
	//number of nodes to delete.
	dnum=(rand()%n)+1;
	//permutation (0,keynum-1) list.
	dindex=random(t->keynum());
	//fork a new tree.
	RBTree *fork=new RBTree();
	fork->copy(t);
	//cout << "afcp*";
	for(i=0;i<dnum;i++) fork->DeleteRBT(element[dindex[i]]);
	//cout << "afdl*";
	delete element;
	delete dindex;
	return fork;
}
// mutation function
void mutation(RBTree **P,int psize,int minttom,int mdmax,int inum,int cemrate,int ncemrate){
	for(int i=1;i<psize*2;i++){
		int m = rand()%100;
		if(P[i]->keynum()>minttom){
			if(P[i]->IsCE(inum) && m<cemrate){
				RBTree *d=P[i];
				P[i]=randomdelete(P[i],mdmax);
				delete d;
			}else if(!P[i]->IsCE(inum) && m<ncemrate){
				RBTree *d=P[i];
				P[i]=randomdelete(P[i],mdmax);
				delete d;
			}
		}		
	}
}
//decide next generation pool
void whocansurvive(RBTree **P,int *fitness,int survive,int psize,int penalty,int inum){
	updatefitness(P,fitness,psize*2,penalty,inum);
	fitnessbubble(P,fitness,psize*2);
	// first "survive"th survive, and choose randomly from the rest.
	for (int i = psize*2-survive-1; i >= 0; --i){
    	int j = rand() % (i+1);
    	//swap the last element with element at random index
    	RBTree *temp = P[survive+i];
    	P[survive+i] = P[survive+j];
    	P[survive+j] = temp;
    	int t=fitness[survive+i];
    	fitness[survive+i]=fitness[survive+j];
    	fitness[survive+j]=t;
	}
	for(int i=psize;i<psize*2;i++){
		delete P[i];
		P[i]=NULL;
		fitness[i]=penalty*2;
	}
	//updatefitness(P,fitness,psize*2,penalty,inum);
	fitnessbubble(P,fitness,psize*2);
}
//average
float avg(int *array,int length){
	float sum = 0;
	for(int i=0;i<length;i++) sum=sum+array[i];
	return (sum/length);
}
//standard deviation
float sd(int *array,int length,float avg){
	float sd = 0;
	for(int i=0;i<length;i++) sd=sd+(array[i]-avg)*(array[i]-avg);
	return sqrt(sd/length);
}
// output fitness
void printfitness(int *fitness,int psize,int generation,ostream &file){
	file << fitness[0] << " " << avg(fitness,psize) << " || ";
	for(int i=0;i<psize;i++) file << fitness[i] << " ";
	file << endl;
}


