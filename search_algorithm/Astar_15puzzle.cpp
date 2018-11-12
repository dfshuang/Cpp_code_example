/*
file in.txt (初始状态个数n， 紧接着n个初始状态)
5
1 2 3 4 5 6 7 8 9 10 15 11 13 14 0 12
14 10 6 0 4 9 1 8 2 3 5 11 12 13 7 15
6 10 3 15 14 8 7 11 5 1 0 2 13 12 9 4
0 5 15 14 7 9 6 13 1 2 12 10 8 11 4 3
11 3 1 7 4 6 8 2 15 9 10 13 14 12 5 0
*/

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
#include <array>
#include <vector>
#include <functional>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
using namespace std;
#define size 4

int dxy[4][2]= {{1,0},{0,1},{0,-1},{-1,0}}; 
int board[size][size];
int path[100];  //存储路径 
int limit;   //评价函数受限 
int goal[16][2]= {{3,3},{0,0},{0,1}, {0,2},{0,3}, {1,0},
	{1,1}, {1,2}, {1,3},{2,0}, {2,1}, {2,2},{2,3},{3,0},{3,1},{3,2}
};//各个数字应在位置对照表
int dirt[4] = {0,1,2,3}; 

int hn(int a[][size]) { //估价函数，曼哈顿距离，小等于实际总步数
	int i,j,cost=0;
	for(i=0; i<size; i++) {
		for(j=0; j<size; j++) {
			int w=board[i][j];
			if(w == 0) continue;
			cost+=abs(i-goal[w][0])+abs(j-goal[w][1]);
		}
	}
	return cost;
}

int maxn = 0;
bool dfs(int sx,int sy,int deep,int pre_move,int hx) { //sx,sy是空格的位置
	// 若评价函数超过limit, 则返回false 
	if(deep + hx > limit)
		return false;
		
	//hx=0, 即到达终点 
	if(hx==0)
		return true;
	if(deep>maxn){
		maxn=deep;
		cout << "deep: " << maxn << endl;
	}
	
	int direc[3] = {-1,-1,-1};
	int left = 0;
	for(int i=0;i<4;i++){
		int nx=sx+dxy[i][0];  
		int ny=sy+dxy[i][1];
		
		//当和上一次移动方向相反时，即可不移动，继续下一个节点 
		int w = board[nx][ny];
		if(w == pre_move) continue;
		
		if(0<=nx&&nx<size && 0<=ny&&ny<size) { //判断移动合理
			int newh = 0;
			if(nx==sx){
				newh = -abs(ny-goal[w][1])+abs(sy-goal[w][1]);
			}
			else{
				newh = - abs(nx-goal[w][0])+abs(sx-goal[w][0]);
			}
			
			if(newh==-1){
				swap(board[sx][sy],board[nx][ny]);
				path[deep]=board[sx][sy];
				if(dfs(nx,ny,deep+1,board[sx][sy],hx+newh))
					return true;
				swap(board[sx][sy],board[nx][ny]);
			}
			else{
				direc[left++]=i;
			}
		}
	}
	
	for(int i=0;i<3;i++){
		if(direc[i]!=-1){
			int nx=sx+dxy[dirt[direc[i]]][0];  
			int ny=sy+dxy[dirt[direc[i]]][1];			
			swap(board[sx][sy],board[nx][ny]);
			path[deep]=board[sx][sy];
			if(dfs(nx,ny,deep+1,board[sx][sy],hx+1))
				return true;
			swap(board[sx][sy],board[nx][ny]);
		}
	}
	return false;
}
int main() {
	ifstream cin("in.txt");
	int n, sx, sy;
	maxn = 30;
	clock_t start, end;
	cin >> n;
	int problem[5] = {0,2,4,3,1}; 
	for(int cnt=0;cnt<n;cnt++) {
		start = clock();
		bool flag=0;
		memset(path,-1,sizeof(path));  //已定义path[100]数组，将path填满-1   void* memset(void*s,int ch,size_t n):将S中前n个字节用ch替换并返回S。
		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++){
				cin >> board[i][j];
				if(board[i][j]==0){
					sx=i;
					sy=j;
				}
			}
		}

	    printf("problem #%d\n", problem[cnt]);
		for(int x=0;x<size;x++){
			for(int y=0;y<size;y++){
				printf("%d ",board[x][y]);
			}
			printf("\n");
		} 
			         
		limit=hn(board);  //全部的曼哈顿距离之和
		printf("LowerBound: %d\n",limit);
		int original_limit = limit;
		
		while(!flag) {
			flag = dfs(sx,sy,0,0,original_limit);
			limit++; //得到的是最小步数
		}
		
		//输出path 
		printf("A optimal solution of problem: %d moves\n",limit-1);
		for(int i=0; i<limit-1; i++)
			printf("%d ",path[i]);  //根据path输出URLD路径
		printf("\n");	
		end = clock();
		printf("runtime: %f\n", double(end - start)/CLOCKS_PER_SEC);
		printf("--------------------------------------------\n\n\n");
	}
	return 0;
}
