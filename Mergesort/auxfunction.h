#include <cstdlib>
using namespace std;
int ccount=0;
class CE{
	public:
		int *list;
		int size;
		int dup;
		CE(){
			list=NULL;
			size=-1;
			dup=-1;
		}
		void copy(int *A,int s,int d){
			list = new int[s];
			for(int i=0;i<s;i++) list[i]=A[i];
			size=s;
			dup=d;
		}
		void copy(CE *ce){
			copy(ce->list,ce->size,ce->dup);
		}
		~CE(){
			if(list)delete list;
			ccount--;
		}
		bool IsCE(){
			int dupcount=0;
			for(int i=0;i<size;i++){
				if(list[i]==dup)dupcount++;
			}
			if(dupcount>=2)return true;
			else return false;
		}
		int truesize(){
			int sizecount=0;
			for(int i=0;i<size;i++){
				if(list[i]!=-1)sizecount++;
			}
			return sizecount;
		}
};

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

int* fetchindex(CE *ce){
	int newsize=ce->truesize();
	int *index=new int[newsize];
	int i=0,j=0;
	for(i=0;i<ce->size;i++){
		if(ce->list[i]!=-1)index[j++]=i;
	}
	//cout << j << "," << newsize << endl;
	if(j!=newsize)cout << "fetch error!!!" << endl;
	return index;
}
CE* randomdelete(CE *aim,int dnum){
	CE *c=new CE();
	c->copy(aim);
	if(aim->truesize()<=2)return c;
	if(dnum>aim->truesize())dnum=aim->truesize();
	int *index = fetchindex(aim); //length = truesize()
	int *dindex = random(aim->truesize());
	int drnum=(rand()%(dnum-1))+1;
	for(int i=0;i<drnum;i++)c->list[index[dindex[i]]]=-1;
	return c;
}

//CE* randomdelete(CE *cur,int dnum){
//	if(dnum>cur->size)dnum=cur->size-1;
//	CE *c=new CE();
//	if(cur->size==2){
//		c->copy(cur);
//		return c;
//	}
//	ccount++;
//	int d = rand()%(dnum-1)+1;
//	int *dindex=random(cur->size);
//	c->list=new int[cur->size-d];
//	c->size=cur->size-d;
//	for(int i=0;i<c->size;i++) c->list[i]=cur->list[dindex[i]];
//	//c->CEsort();
//	delete dindex;
//	return c;
//}
void initialization(CE *ori,int initdmax,CE **P,int *obj,int psize){
	for(int i=0;i<psize;i++) P[i]=randomdelete(ori,initdmax);
	for(int i=psize;i<psize*2;i++) P[i]=NULL;
}
CE* crossover(CE *PA,CE *PB){
	CE *child=new CE();
	child->copy(PA);
	if(child->size!=PB->size)cout <<"size error!!!" << endl;
	for(int i=0;i<PB->size;i++){
		if(PB->list[i]==-1)child->list[i]=-1;
	}
	return child;
}
//CE* crossover(CE *PA,CE *PB){
//	CE *temp=new CE();
//	temp->copy(PA);
//	int i=0,j=0;
//	while(i<temp->size&&j<PB->size){
//		if(temp->list[i]<PB->list[j]){
//			temp->list[i]=-1;
//			i++;
//		}else if(temp->list[i]>PB->list[j]) j++;
//		else { i++;j++; }
//	}
//	int newsize=0;
//	for(int i=0;i<temp->size;i++){
//		if(temp->list[i]!=-1)newsize++;
//	}
//	if(newsize==0){
//		delete temp;
//		return NULL;
//	}
//	CE *child =new CE();
//	child->list=new int[newsize];
//	child->size=newsize;
//	ccount++;
//	i=0,j=0;
//	while(i<newsize){
//		if(PB->list[j]!=-1){
//			child->list[i]=PB->list[j];
//			i++;j++;
//		}else j++;
//	}
//	delete temp;
//	return child;
//}
CE* mutation(CE *ce,int mdmax,int cemrate,int ncemrate){
	int m;
	m=rand()%100;
	if(ce->IsCE() && m<cemrate){
		return randomdelete(ce,mdmax);
	}else if(!ce->IsCE() && m<ncemrate){
		return randomdelete(ce,mdmax);
	}else{
		CE *cp=new CE();
		cp->copy(ce);
		return cp;
	}
}
//bubble sort depends on tree's fitness value
void objbubble(CE** P,int *obj,int length){
	int i,j;
	for(i=0;i<length-1;i++){
		for(j=0;j<length-i-1;j++){
			if(obj[j] > obj[j+1]){
				CE* temp=P[j];
				P[j]=P[j+1];
				P[j+1]=temp;
				int t=obj[j];
				obj[j]=obj[j+1];
				obj[j+1]=t;
			}
		}
	}
}
//update fitness value of the population
void updateobj(CE **P,int *obj,int length,int penalty){
	int i;
	for(i=0;i<length;i++){
		//cout << "uf" << i << "//";
		if(!P[i]){
			//cout << "!P[i]";
			obj[i]=penalty*2;
		}else{
			//if(P[i]->root->left==NULL)cout << "P[i]";
			if(P[i]->IsCE()) obj[i]=P[i]->truesize();
			else obj[i]=P[i]->truesize()+penalty;
		}
	}
}
void whocansurvive(CE **P,int *obj,int survive,int psize,int penalty){
	updateobj(P,obj,psize*2,penalty);
	objbubble(P,obj,psize*2);
	for (int i = psize*2-survive-1; i >= 0; --i){
    	int j = rand() % (i+1);
    	//swap the last element with element at random index
    	CE *temp = P[survive+i];
    	P[survive+i] = P[survive+j];
    	P[survive+j] = temp;
    	int t=obj[survive+i];
    	obj[survive+i]=obj[survive+j];
    	obj[survive+j]=t;
	}
	for(int i=psize;i<psize*2;i++){
		delete P[i];
		P[i]=NULL;
		obj[i]=penalty*2;
	}
	//updatefitness(P,fitness,psize*2,penalty,inum);
	objbubble(P,obj,psize*2);
}
void printobj(int psize,int *obj){
	for(int i=0;i<10;i++) cout << obj[i] << " ";
	cout << endl;
}

