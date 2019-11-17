#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <stack>
#include <queue>
#include <ctime>
#include <cmath>
#include "RBTree.h"
using namespace std;

void general_shrinker(RBTree &,int,int*,int*);
void general_shrinker_plus(RBTree &,int,int*,int*);
void EA_shrinker(RBTree &,int,int*);

int main(){

	stack <string> S;
	stack <Node*> N;
	char str[10];
	string s;
	const char* c;
	Node* temproot;
	RBTree CE;
	//build counterexample
	ifstream ifile("counterexample-24-949-1277.txt",ios::in);
	while(!ifile.eof()){
		ifile.getline(str,10,' ');
		if(strcmp(")",str)!=0){
			S.push(str);
		}else{
			Node* temp=new Node(0);
			s=S.top();
			if("E"==s) { temp->right=NULL;S.pop();s=S.top(); }
			else if("N"==s) { temp->right=N.top();S.pop();s=S.top();N.pop(); }
			c=s.c_str();
			temp->key=atoi(c);S.pop();s=S.top();
			if("E"==s) { temp->left=NULL;S.pop();s=S.top(); }
			else if("N"==s) { temp->left=N.top();S.pop();s=S.top();N.pop(); }
			if("Red"==s) { temp->color=RED;S.pop();	}
			else if("Black"==s) { temp->color=BLACK;S.pop(); }
			S.pop();S.pop();S.push("N");N.push(temp);temproot=temp;
		}
	}
	ifile.close();
	CE.root=temproot;
	CE.Setneel();
	CE.Setparent();

	/*while(!S.empty()){
		cout << S.top() << endl;
		S.pop();
	}
	if(CE.IsRBT()) cout << "A Red-Black Tree!" << endl;
	if(CE.IsCE(1234)) cout << "A counterexample!" << endl;*/
	ofstream stat("RBTstat.txt",ios::app);
	ofstream rs("RBT_rs.txt",ios::app);
	ofstream rw("RBT_rw.txt",ios::app);
	ofstream ga("RBT_ga.txt",ios::app);
	int round = 30;
	int mts[round];
	int cec[round];
	int ta[round];
	clock_t t;


	for(int i=1;i<=round;i++){
		cout << "round " << i << endl;
		t=clock();
		general_shrinker(CE,i,cec,mts);
		//EA_shrinker(CE,i,mts);
		ta[i-1]=int(clock()-t);
		rs << mts[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mts,round) << " " << sd(mts,round,avg(mts,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << " ";
	for(int i=1;i<=round;i++){
		cout << "round " << i << endl;
		t=clock();
		general_shrinker_plus(CE,i,cec,mts);
		//EA_shrinker(CE,i,mts);
		ta[i-1]=int(clock()-t);
		rw << mts[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mts,round) << " " << sd(mts,round,avg(mts,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << " ";
	for(int i=1;i<=round;i++){
		cout << "round " << i << endl;
		t=clock();
		//general_shrinker_plus(CE,i,cec,mts);
		EA_shrinker(CE,i,mts);
		ta[i-1]=int(clock()-t);
		ga << mts[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mts,round) << " " << sd(mts,round,avg(mts,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << endl;
	rs << "*" << endl;
	rw << "*" << endl;
	ga << "*" << endl;
	stat.close();
	rs.close();
	rw.close();
	ga.close();
	////ofstream statefile("EA_shrinker.txt",ios::app);
	////statefile << "(" << CE.keynum() << ",1234) ";
	////statefile << avg(mts,round) << " " << sd(mts,round,avg(mts,round)) << endl;


	/*
	//general_shrinker(CE,i);
	//EA_shrinker(CE,1);
	int round = 30;
	int cec[round],mts[round];
	ofstream gfile("general_shrinker_v2.txt",ios::app);
	for(int i=1;i<=round;i++) general_shrinker_plus(CE,i,cec,mts);
	float cecavg,mtsavg;
	cecavg = avg(cec,round);
	mtsavg = avg(mts,round);
	gfile << "(" << CE.keynum() << ",1234) ";
	gfile << cecavg << " " << sd(cec,round,cecavg) << " ";
	gfile << mtsavg << " " << sd(mts,round,mtsavg) << endl;
	gfile.close();*/
	return 0;

}

void EA_shrinker(RBTree &CE,int NO,int *mts){
	//parameter
	int inum = 1234;               //after insert 1234, no longer a red-black tree.
	int psize = 100;               //the upperbound of population
	int initdmax = CE.keynum()/10; //at most delete 10% of nodes
	int penalty = CE.keynum();     //penalty for fitness value
	int evalmax = 50000;         //maximum evaluation
	int survive = 40;              // first fifty RBT is guaranteed to survive
	int cemrate = 20;              // counterexample mutation rate
	int ncemrate = 5;              // non counterexample mutation rate
	int mdmax = 5;                 // the maximum key to delete during mutation
	int minttom = 8;              // the minimum tree that can be mutated **minttom must > mdmax**
	//set random seed
	int seed = time(0);
	srand(seed);
    //file setting
	//store all object value, generation by generation.
	////ofstream afile(filename("EA1234-",NO,".txt").c_str(),ios::app);
	////ofstream statefile("EA1234-statistic.txt",ios::app);
	////ofstream avgfile("EA1234-avg.txt",ios::app);
	////ofstream sdfile("EA1234-sd.txt",ios::app);
	////ofstream mincefile("EA1234-mince.txt",ios::app);
	//initial population
	RBTree *P[psize*2];
	int fitness[psize*2];
	RBTree *temp=&CE;
	P[0]=new RBTree();//P[0] = original counterexample
	P[0]->copy(temp);
	for(int i=1;i<psize;i++) P[i]=randomdelete(P[0],initdmax);
	for(int i=psize;i<psize*2;i++) P[i]=NULL;
	updatefitness(P,fitness,psize*2,penalty,inum);
	fitnessbubble(P,fitness,psize*2);

	////avgfile << avg(fitness,psize) << " ";
	////sdfile << sd(fitness,psize,avg(fitness,psize)) << " ";
	////mincefile << P[0]->keynum() << " ";

	//generation by generation
	int generation = 0;
	evaluation = 0;
	////printfitness(fitness,psize,generation,afile);
	while(evaluation < evalmax){
		// select and crossover utill pool size doubles.
		for(int i=psize;i<psize*2;i++){
			// random select index
			int *sindex = random(psize);
			// select two RBT as parent from pool
			int indexA=sindex[0],indexB=sindex[1];
			// crossover
			P[i]=crossover(P[indexA],P[indexB]);
			if(P[i]==NULL) i--;
			delete sindex;
		}
		// mutation
		mutation(P,psize,minttom,mdmax,inum,cemrate,ncemrate);

		whocansurvive(P,fitness,survive,psize,penalty,inum);
		generation++;
		////printfitness(fitness,psize,generation,afile);
		////avgfile << avg(fitness,psize) << " ";
	    ////sdfile << sd(fitness,psize,avg(fitness,psize)) << " ";
		////mincefile << P[0]->keynum() << " ";
	}
	mts[NO-1]=P[0]->keynum();
	////statefile << seed << " " << P[0]->keynum() << " ";
	////writetofile(P[0]->root,P[0]->neel,statefile);
	////statefile << endl;
	////avgfile << "0.0" << endl;
	////sdfile << "0.0" << endl;
	////mincefile << "0" << endl;
	////afile.close();
	////statefile.close();
	////avgfile.close();
	////sdfile.close();
	////mincefile.close();
}

void general_shrinker(RBTree &CE,int iteration,int *cec,int *mts){
	//general shrinker (v2.0 random delete from original CE and random insert)

	//cout << "iteration-" << iteration << endl;

	int i=0,tcount=0,cecount=0,maxeval=0;
	int deletenum=0,insertnum=0,tsize=0,ntsize=0,mintsize=0;
	int *element,*dindex;
	RBTree *minCE,*tempRBT,*BI,*temp;
	////ofstream statistic("1234-Statistic.txt",ios::app);
	////statistic << CE.keynum() << " ";
	//initial minimum counterexample = CE
	minCE=new RBTree();
	temp=&CE;
	minCE->copy(temp);
	tsize=minCE->keynum();
	mintsize=minCE->keynum();
	//Store all key value of RBT in order.
	element = FetchAllKey(minCE,tsize);
	//Set where to store CE.
	////ofstream afile(filename("1234-",iteration,".txt").c_str(),ios::app);
    //Set random seed.
    int seed=time(0);
	srand(seed);
	////statistic << seed << " ";
	//Set maximum iteration
	evaluation = 0;
	maxeval = 50000;
	while(evaluation<maxeval){
		tempRBT = new RBTree();
		tempRBT->root=CE.root->copy();
		tempRBT->Setneel();
		tempRBT->Setparent();
		rbt++;
	//random delete n node
		//select n from 1 to CE.size-1
		deletenum = rand()%(tsize-1)+1;
		//random select "deletenum" nodes.
		dindex = random(tsize);
		//deletion:
		for(i=0;i<deletenum;i++){
			tempRBT->DeleteRBT(element[dindex[i]]);
		}
		ntsize = tempRBT->keynum();
		BI = new RBTree();
		BI->root=tempRBT->root->copy();
		BI->Setneel();
		BI->Setparent();
		rbt++;
		tempRBT->insert(1234);
		if(!tempRBT->IsRBT()){// a counterexample since not a RBT afer insertion
			cecount++;
			////writetofile(BI->root,BI->neel,afile);
			////afile << endl;
			if(ntsize<mintsize){// BI is a smaller counterexample.
				mintsize=ntsize;
			}

		}
		delete dindex;
		delete BI;
		delete tempRBT;
		tcount++;
	}
	delete minCE;
	////statistic << cecount << " " << mintsize << endl;
	cec[iteration-1] = cecount;
	mts[iteration-1] = mintsize;

	////afile.close();
	////statistic.close();
}

void general_shrinker_plus(RBTree &CE,int iteration,int *cec,int *mts){
	//general shrinker (v3.0 random walk!)

	//cout << "iteration-" << iteration << endl;

	int i=0,tcount=0,cecount=0,maxeval=0;
	int deletenum=0,insertnum=0,tsize=0,ntsize=0,mintsize=0;
	int *element,*dindex;
	bool IsCE;
	RBTree *minCE,*tempRBT,*BI,*temp;
	////ofstream statistic("1234-Statistic.txt",ios::app);
	////statistic << CE.keynum() << " ";
	//initial minimum counterexample = CE
	minCE=new RBTree();
	temp=&CE;
	minCE->copy(temp);
	mintsize=minCE->keynum();
	//Set where to store CE.
	////ofstream afile(filename("1234-",iteration,".txt").c_str(),ios::app);
    //Set random seed.
    int seed=time(0);
	srand(seed);
	////statistic << seed << " ";
	//Set maximum iteration
	evaluation = 0;
	maxeval = 50000;
	while(evaluation<maxeval){
		tempRBT = new RBTree();
		tempRBT->root=CE.root->copy();
		tempRBT->Setneel();
		tempRBT->Setparent();
		rbt++;
		do{
			IsCE=false;
			tsize = tempRBT->keynum();
			element =  FetchAllKey(tempRBT,tsize);
		//random delete n node
			//select n from 1 to CE.size-1
			deletenum = rand()%(tsize-1)+1;
			//random select "deletenum" nodes.
			dindex = random(tsize);
			//deletion:
			for(i=0;i<deletenum;i++){
				tempRBT->DeleteRBT(element[dindex[i]]);
			}
			ntsize = tempRBT->keynum();
			BI = new RBTree();
			BI->root=tempRBT->root->copy();
			BI->Setneel();
			BI->Setparent();
			rbt++;
			tempRBT->insert(1234);
			if(!tempRBT->IsRBT()){// a counterexample since not a RBT afer insertion
				cecount++;
				IsCE=true;
				////writetofile(BI->root,BI->neel,afile);
				////afile << endl;
				if(ntsize<mintsize){// BI is a smaller counterexample.
					mintsize=ntsize;
				}
			}
			temp = tempRBT; // temp => tree after insert
			tempRBT = BI;   // tempRBT => tree before insert(randomly delete again)
			BI = temp;      // BI => tree after insert (delete)
			delete dindex;
			delete BI;
			delete element;
		}while(IsCE);
		delete tempRBT;
	}
	delete minCE;
	////statistic << cecount << " " << mintsize << endl;
	cec[iteration-1] = cecount;
	mts[iteration-1] = mintsize;

	////afile.close();
	////statistic.close();
}

/*
	//general shrinker (v1.0)
	int size=CE.size(CE.root);
	int cecount=0,count=0,rbtcount=0,i=0;
	RBTree *minCE,*tempRBT,*beforeinsert;
	// initial minimum counterexample = CE
	minCE=new RBTree();
	minCE->root=CE.root->copy();
	minCE->Setneel();
	// restore Node pointers for latter checking
	queue <Node*> NQ;
	// start from root of first counterexample
	temproot=CE.root;
	NQ.push(temproot);
	while(!NQ.empty()){
		temproot=NQ.front();
		if(temproot->right!=NULL){// T c l k r
			NQ.push(temproot->left);
			NQ.push(temproot->right);
			count++;
			cout << count << endl;
			tempRBT=new RBTree();
			tempRBT->root=temproot->copy();//clone a new RBT
			tempRBT->Setneel();
			beforeinsert=new RBTree();//before insertion version
			beforeinsert->root=temproot->copy();
			beforeinsert->Setneel();
			if(tempRBT->IsRBT()){//before insertion is a RBT
				rbtcount++;
				tempRBT->insert(1234);
				if(!tempRBT->IsRBT()){//after insertion is not a RBT => a counterexample
					cecount++;
					if(beforeinsert->size(beforeinsert->root) < minCE->size(minCE->root)) minCE=beforeinsert;// a smaller counterexample
				}
			}
		}else{// E
			count++;
		}
		NQ.pop();
	}
	cout << "After checking " << count << " trees." << endl;
	cout << "(There are " << rbtcount << " red-black tree.)" << endl;
	cout << "Found " << cecount << " counterexamples" << endl;
	cout << "And the minimum size is " << minCE->size(minCE->root) << " (nodes)." << endl;

	//After checking 1757 trees.
	//Found 4 counterexamples
	//And the minimum size is 51 (nodes).  */
