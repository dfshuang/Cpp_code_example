#include<stdio.h>
#include<string.h>
#include<math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int dxy[4][2]= {{-1,0},{0,-1},{0,1},{1,0}}; 
vector<int> vinit;
int path[100]={0};  //存储路径 
int len=70;   
int goal[16][2]= {{3,3},{0,0},{0,1}, {0,2},{0,3}, {1,0},
	{1,1}, {1,2}, {1,3},{2,0}, {2,1}, {2,2},{2,3},{3,0},{3,1},{3,2}
};//各个数字应在位置对照表

struct node{
	unsigned long long state;
	int g;
	int pre;
	int sx,sy;
	
	node(vector<int>& vs, int gs=0, int pres=0){
		state = 0;
		for(int i=0;i<16;i++){
			state = state*(unsigned long long)16 + (unsigned long long)vs[i];
			if(vs[i]==0){
				sx=i/4;
				sy=i%4;
			}
		}
		g=gs;
		pre=pres;
	}
	bool operator == (node& nx)const{
		return state==nx.state;
	}
	int f()const{
		return g+h();
	}
	int h()const{
		vector<int> st = analy();
		int dis = 0;
		for(int i=0;i<16;i++){
			if(st[i]==0) continue;
			dis+=abs(i/4-goal[st[i]][0])+abs(i%4-goal[st[i]][1]);
		}
		return dis;
	}
	vector<int> analy()const{
		//获取状态值
		unsigned long long st = state;
		vector<int> v;
		for(int i=0;i<16;i++){
	 		v.push_back((int)(st%(unsigned long long)16));
	 		st = st/(unsigned long long)16;
		}
		if(v.size()<=15){
			v.push_back(0);
		}
		reverse(v.begin(),v.end());
		return v;
	}
	bool operator < (node& p){
        return f() < p.f();
    }
    bool operator > (node& p){
    	return f() > p.f();
	}
	friend bool operator > (const node& px, const node& py){
    	return px.f() > py.f();
	}
	void print(){
		vector<int> v = analy();
		for(int i=0;i<16;i++){
			cout << v[i] << " ";
		} 
		cout << "\n";
	}
}; 

void print_path(){
	for(int i=0; i<70; i++)
		printf("%d ",path[i]); 
	printf("\n");	
}

bool solve(){
	int maxg = 0;
	
	node initn(vinit);
	//initn.print();
	
	int limit=initn.f(); //初始曼哈顿距离 
	printf("LowerBound: %d\n",limit);
	
	priority_queue<node, vector<node>, greater<node> > pri;
	pri.push(vinit);
	
	while(!pri.empty()){
		node topx = pri.top();
		pri.pop();
		
		if(topx.g>0) path[topx.g-1] = topx.pre;
		topx.print();
		print_path();
		cout << "---------------------\n\n";
		if(topx.h()==0){
			len = topx.g;
			return true;
		}
		if(topx.g>maxg){
			maxg=topx.g;
			cout << "maxg: " << maxg << endl;
		}
		vector<int> vtemp = topx.analy();
		for(int i=0;i<4;i++){
			int nx = topx.sx+dxy[i][0];
			int ny = topx.sy+dxy[i][1];
			if(0<=nx&&nx<4 && 0<=ny&&ny<4){
				int pos=4*nx+ny;
				if(vtemp[pos]==topx.pre) continue;
				int pos_ori = 4*topx.sx+topx.sy;
				swap(vtemp[pos_ori],vtemp[pos]);
				pri.push(node(vtemp,topx.g+1,vtemp[pos_ori]));
				swap(vtemp[pos_ori],vtemp[pos]);
			}
		} 
	}
	return false;
}

int main() {
	ifstream cin("in.txt");
	int n, sx, sy;
	clock_t start, end;
	cin >> n;
	int problem[5] = {0,2,4,3,1}; 
	for(int cnt=0;cnt<n;cnt++) {
		start = clock();
		vinit.clear();
		memset(path,-1,sizeof(path));  //已定义path[100]数组，将path填满-1   void* memset(void*s,int ch,size_t n):将S中前n个字节用ch替换并返回S。
		for(int i=0;i<16;i++){
			int x;
			cin >> x;
			vinit.push_back(x); 
		}

	    printf("problem #%d\n", problem[cnt]);
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				printf("%d ",vinit[4*x+y]);
			}
			printf("\n");
		} 
		
		solve();
		
		//输出path 
		printf("A optimal solution of problem: %d moves\n",len);
		print_path();
		end = clock();
		printf("runtime: %f\n", double(end - start)/CLOCKS_PER_SEC);
		printf("--------------------------------------------\n\n\n");
	}
	return 0;
}
