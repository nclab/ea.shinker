#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <utility>
#include "auxfunction.h"
using namespace std;

int evaluation;

void Merge_Sort(int A[],int p,int q);
void Merge(int A[], int p, int q, int m);
int CEchecker(int A[],int size);
bool CEcheck(int A[],int size);
void general_shrinker(int *A,int size,int iteration,int *mls,int*cec);
void general_shrinker_plus(int *A,int size,int iteration,int *mls,int* cec);
void EA_shrinker(CE &ce,int iteration,int *mls);

int main() {
	clock_t t;
	ifstream ifile("counterexample-13-508.txt",ios::in);
	vector<int> temp;
	int input;
	while(!ifile.eof()){
		ifile >> input;
		temp.push_back(input);
	}
	ifile.close();
	int size = temp.size();
	int A[size];
	for(int i=0;i<size;i++){
		A[i]=temp.front();
		temp.erase(temp.begin());
	}
	sort(A,A+size);
	int d = CEchecker(A,size);
	if(d==-1)return 1;
	int round = 30;
	int mls[round];
	int cec[round];
	int ta[round];
	CE ce;
	ce.copy(A,size,d);

	ofstream stat("MSstat.txt",ios::app);
	ofstream rs("MS_rs.txt",ios::app);
	ofstream rw("MS_rw.txt",ios::app);
	ofstream ga("MS_ga.txt",ios::app);
	rs << "*" << endl;
	rw << "*" << endl;
	ga << "*" << endl;
	for(int i=1;i<=round;i++){
		t=clock();
		general_shrinker(A,size,i,mls,cec);
		ta[i-1]=int(clock()-t);
		rs << mls[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mls,round) << " " << sd(mls,round,avg(mls,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << " ";
	for(int i=1;i<=round;i++){
		t=clock();
		general_shrinker_plus(A,size,i,mls,cec);
		ta[i-1]=int(clock()-t);
		rw << mls[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mls,round) << " " << sd(mls,round,avg(mls,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << " ";
	for(int i=1;i<=round;i++){
		t=clock();
		EA_shrinker(ce,i,mls);
		ta[i-1]=int(clock()-t);
		ga << mls[i-1] << " " << ta[i-1] << endl;
	}
	stat << avg(mls,round) << " " << sd(mls,round,avg(mls,round)) << " ";
	stat << avg(ta,round) << " " << sd(ta,round,avg(ta,round)) << endl;
	stat.close();
	rs.close();
	rw.close();
	ga.close();

	/*ofstream gs("general_shrinker_v2.txt",ios::app);
	gs << size << " " << d << " ";
	gs << avg(cec,round) << " ";
	gs << sd(cec,round,avg(cec,round)) << " ";
	gs << avg(mls,round) << " ";
	gs << sd(mls,round,avg(mls,round)) << endl;
	gs.close();*/

	/*for (int i=1;i<=round;i++)general_shrinker(A,size,i,mls,cec);
	ofstream gs("general_shrinker_v1.txt",ios::app);
	gs << size << " " << d << " ";
	gs << avg(cec,round) << " ";
	gs << sd(cec,round,avg(cec,round)) << " ";
	gs << avg(mls,round) << " ";
	gs << sd(mls,round,avg(mls,round)) << endl;
	gs.close();*/
	/*int round = 30;
	int mls[round];
	int cec[round];
	CE ce;
	ce.copy(A,size,d);
	//EA_shrinker(ce,1,mls);
	for(int i=1;i<=round;i++)EA_shrinker(ce,i,mls);
	cout << avg(mls,round) << endl;
	ofstream statefile("EA_shrinker.txt",ios::app);
	statefile << ce.size << " ";
	statefile << avg(mls,round) << " " << sd(mls,round,avg(mls,round)) << endl;
	statefile.close();*/
	return 0;

}
void EA_shrinker(CE &ce,int iteration,int *mls){
	int psize = 10;
	int gmax = 50;
	int initdmax = ce.size/2; //at most delete 10% of nodes
	int penalty = ce.size;     //penalty for fitness value
	int survive = 8;              // first fifty RBT is guaranteed to survive
	int cemrate = 50;              // counterexample mutation rate
	int ncemrate = 0;              // non counterexample mutation rate
	int mdmax = 20;                 // the maximum key to delete during mutation
	CE *P[psize*2];
	int obj[psize*2];
	ccount=0;
	int seed = time(0)+iteration;
	srand(seed);

	//file
	////ofstream objit(filename("ce50-",iteration,".txt").c_str(),ios::app);
	////ofstream state("ce50-setting.txt",ios::app);
	////ofstream objavg("ce50-avg.txt",ios::app);
	////ofstream objsd("ce50-sd.txt",ios::app);
	////ofstream objmin("ce50-min.txt",ios::app);


	//initialization
	CE *temp=&ce;
	initialization(temp,initdmax,P,obj,psize);
	updateobj(P,obj,psize*2,penalty);
	objbubble(P,obj,psize*2);

	////objavg << avg(obj,psize) << " ";
	////objsd << sd(obj,psize,avg(obj,psize)) << " ";
	////objmin << P[0]->truesize() << " ";
	////for(int i=0;i<psize;i++) objit << obj[i] << " ";
	////objit << endl;

	int generation =0;
	while(generation<gmax){
		//cout << "generation" << generation << endl;
		for(int i=psize;i<psize*2;i++){
			// random select index
			int *sindex = random(psize);
			// select two RBT as parent from pool
			int indexA=sindex[0],indexB=sindex[1];
			// crossover
			P[i]=crossover(P[indexA],P[indexB]);
			if(P[i]->truesize()==0) i--;
			delete sindex;
		}

		for(int i=1;i<psize*2;i++){
			temp=P[i];
			P[i]=mutation(P[i],mdmax,cemrate,ncemrate);
			delete temp;
		}
		whocansurvive(P,obj,survive,psize,penalty);
		//printobj(psize,obj);
		//for(int i=0;i<psize;i++)cout << P[i]->size << " "; cout << endl;
		//cout << ccount << endl;
		////objavg << avg(obj,psize) << " ";
		////objsd << sd(obj,psize,avg(obj,psize)) << " ";
		////objmin << P[0]->truesize() << " ";
		////for(int i=0;i<psize;i++) objit << obj[i] << " ";
		////objit << endl;
		generation++;
	}
	//cout << "minimum counterexample size = " << P[0]->size << endl;
	mls[iteration-1]=P[0]->truesize();
	////state << seed << " " << P[0]->size << " " << P[0]->truesize() << " || ";
	////for(int i=0;i<P[0]->size;i++){
	////	if(P[0]->list[i]!=-1) state << P[0]->list[i] << " ";
	////}
	////state << endl;
	////objavg << "0.0" << endl;
	////objsd << "0.0" << endl;
	////objmin << "0" << endl;
	////state.close();
	////objit.close();
	////objavg.close();
	////objsd.close();
	////objmin.close();
}
void general_shrinker(int *A,int size,int iteration,int *mls,int *cec){
	int seed = time(0)+iteration;
	srand(seed);
	int *temp,*dindex;
	int tempsize,minsize,cecount;
	minsize = size;
	evaluation = 0;
	cecount = 0;

	////ofstream ceit(filename("ce50-",iteration,".txt").c_str(),ios::app);
	////ofstream state("ce50-Statistic.txt",ios::app);

	////state << seed << " ";

	while(evaluation < 500){
		dindex = random(size);
		tempsize = rand()%(size-1)+1;
		temp = new int[tempsize];
		for(int i=0;i<tempsize;i++) temp[i]=A[dindex[i]];
		sort(temp,temp+tempsize);
		if(CEcheck(temp,tempsize)){
			cecount++;
			////for(int i=0;i<tempsize;i++) ceit << temp[i] << " "; ceit << endl;
			if(tempsize<minsize)minsize=tempsize;
		}
		delete dindex;
		delete temp;
		evaluation++;
	}
	mls[iteration-1]=minsize;
	cec[iteration-1]=cecount;
	////state << cecount << " " << minsize << endl;
}
void general_shrinker_plus(int *A,int size,int iteration,int *mls,int *cec){
	int seed = time(0)+iteration;
	srand(seed);
	int *temp,*pre,*dindex;
	int tempsize,minsize,cecount,nextsize;
	bool IsCE;
	minsize = size;

	////ofstream ceit(filename("ce50-",iteration,".txt").c_str(),ios::app);
	////ofstream state("ce50-Statistic.txt",ios::app);

	////state << seed << " ";

	evaluation = 0;
	cecount = 0;
	while(evaluation < 500){
		IsCE = false;
		nextsize = size;
		pre=new int[size];
		for(int i=0;i<size;i++)pre[i]=A[i];
		do{
			evaluation++;
			IsCE=false;
			//cout << evaluation << endl;
			dindex = random(nextsize);
			tempsize = rand()%(nextsize-1)+1;
			temp = new int[tempsize];
			for(int i=0;i<tempsize;i++) temp[i]=pre[dindex[i]];
			sort(temp,temp+tempsize);
			if(CEcheck(temp,tempsize)){
				cecount++;
				////for(int i=0;i<tempsize;i++) ceit << temp[i] << " "; ceit << endl;
				nextsize = tempsize;
				pre = new int[tempsize];
				for(int i=0;i<tempsize;i++)pre[i]=temp[i];
				IsCE = true;
				if(tempsize<minsize)minsize=tempsize;
			}
			delete temp;
			delete dindex;
		}while(IsCE&&evaluation<500);
		delete pre;
	}
    mls[iteration-1]=minsize;
	cec[iteration-1]=cecount;
	////state << cecount << " " << minsize << endl;

}


int CEchecker(int A[],int size){
	int i,j;
	for(i=0;i<size-1;i++){
		if(A[i]==A[i+1]){
			//cout << A[i] << endl;
			return A[i];
		}
	}
	return -1;
}
bool CEcheck(int A[],int size){
	int i,j;
	for(i=0;i<size-1;i++){
		if(A[i]==A[i+1]){
			return true;
		}
	}
	return false;
}

void Merge_Sort(int A[], int p, int q) {
	if (p < q) {
		int m = (p + q) / 2;
		Merge_Sort(A, p, m);  //遞迴排序子陣列 A[p...m]
		Merge_Sort(A, m + 1, q); //遞迴排序子陣列 A[m+1...q]
		Merge(A, p, q, m);  //將左右兩個以排序好的子陣列合併
	}
}
void Merge(int A[], int p, int q, int m) {
	int n1 = m - p + 1;
	int n2 = q - m;
	/*Let L[0...n1] and R[0...n2] be new arrays
	and copy A[p...m] to L[0...n1]
	and copy A[m+1...q] to R[0...n2] */
	int* L = new int[n1];
	int* R = new int[n2];

	for (int i = 0; i < n1; i++) {
		L[i] = A[p+i];
	}
	for (int i = 0; i < n2; i++) {
		R[i] = A[i+m+1];
	}
	L[n1] = 2147483647;
	R[n2] = 2147483647;
	int i = 0, j = 0;
	for (int k = p; k <= q; k++) {
		if (L[i] <= R[j]) {
			A[k] = L[i];
			i++;
		}
		else if (L[i] > R[j]){
			A[k] = R[j];
			j++;
		}
		/*else {
			A[k]=L[i];
			i++;
			j++;
		}*/
	}
}
