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
	vector<int> state;          //当前节点的状态列表 
	int g;                      //节点的实际代价值 
	int sx,sy;                  //记录数0（即空格）的位置，方便后面操作 
	vector<int> path;           //记录到此节点的路径
	
	/*构造函数 
		vs: 传入的状态列表
		gs: 到当前节点的实际代价
		pathx: 到当前的节点的路径 
	*/
	node(vector<int>& vs, int gs=0, vector<int> pathx=vector<int>()){
		state = vs;
		for(int i=0;i<16;i++){
			if(vs[i]==0){
				sx=i/4;
				sy=i%4;
			}
		}
		g=gs;
		path=pathx;
	}
	
	//返回节点的估价函数值 
	int f()const{
		return g+h();
	}
	
	//返回节点到目标节点的估计代价 
	int h()const{
		int dis = 0;
		for(int i=0;i<16;i++){
			if(state[i]==0) continue;
			dis+=abs(i/4-goal[state[i]][0])+abs(i%4-goal[state[i]][1]);
		}
		return dis;
	}

	//用于构建优先队列产生优先级 
	bool operator < (node& p){
        return f() < p.f();
    }
    bool operator > (node& p){
    	return f() > p.f();
	}
	friend bool operator > (const node& px, const node& py){
    	return px.f() > py.f();
	}
}; 

void print_path(){
	for(int i=0; i<len; i++)
		printf("%d ",path[i]); 
	printf("\n");	
}

bool solve(){
	//初始化节点 
	node initn(vinit);

	//初始曼哈顿距离 
	int limit=initn.f(); 
	printf("LowerBound: %d\n",limit);
	
	//定义边缘节点优先队列 
	priority_queue<node, vector<node>, greater<node> > pri;
	pri.push(initn);
	
	while(!pri.empty()){
		node topx = pri.top();
		pri.pop();
		vector<int> pathx = topx.path;
		
		//h=0时，到达终点 
		if(topx.h()==0){
			len = topx.g;
			for(int i=0;i<len;i++){
				path[i] = topx.path[i];
			}
			return true;
		}

		//计算邻接的四个可能的边缘节点 
		vector<int> vtemp = topx.state;
		for(int i=0;i<4;i++){
			int nx = topx.sx+dxy[i][0];
			int ny = topx.sy+dxy[i][1];
			if(0<=nx&&nx<4 && 0<=ny&&ny<4){
				//pos: 要移动的节点数字的位置 
				int pos=4*nx+ny;		
				//等于前一个移动反向时，不运行 
				if(pathx.size()!=0 && vtemp[pos]==pathx.back()) continue;  
				//修改路径 
				pathx.push_back(vtemp[pos]);
				//pos_ori: 0的一维位置 
				int pos_ori = 4*topx.sx+topx.sy;
				//先交换vtemp，用于构造边缘节点即可，节省空间		
				swap(vtemp[pos_ori],vtemp[pos]);
				pri.push(node(vtemp,topx.g+1,pathx));
				swap(vtemp[pos_ori],vtemp[pos]);
				pathx.pop_back();
			}
		} 
	}
	return false;
}

int main() {
	//从文件读取数据
	ifstream cin("in.txt");
	int n, sx, sy;
	clock_t start, end;
	cin >> n;
	int problem[5] = {0,2,4,3,1}; 
	for(int cnt=0;cnt<n;cnt++) {
		start = clock();
		vinit.clear();
		memset(path,-1,sizeof(path));  
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
		
		//解决问题
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
