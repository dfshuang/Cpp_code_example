#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <ctime>
#include <fstream>
using namespace std;

// N 表示棋盘大小
#define N 10  
int pos[N];
ofstream co("bt10.txt");

bool is_valid(int r){
    for(int i=0;i<r;i++){
        if(pos[i]==pos[r] || abs(pos[i]-pos[r])==abs(i-r)) return false;
    }
    return true;
}

void print_board(){
    for(int i=0;i<N;i++){
        co << pos[i] << " ";
    }
    co << endl;
}

//只搜索一个解
bool back_tracking1(int r){
    //如果 r 比 N 大，则搜索完毕
    if(r >= N){
        //print_board();
        return true;;
    }
    //r < N，继续搜索
    for(int i=0;i<N;i++){
        pos[r] = i;
        if(is_valid(r) && back_tracking1(r+1)) return true; 
    }
    return false;
}

//搜索全部解
void back_tracking(int r){
    //如果 r 比 N 大，则搜索完毕
    if(r >= N){
        //print_board();
        return  ;
    }
    //r < N，继续搜索
    for(int i=0;i<N;i++){
        pos[r] = i;
        if(is_valid(r)) back_tracking(r+1);
    }
}


int main(){
    clock_t start, end;

    //搜索一个解的时间
    start = clock();
    back_tracking1(0);
    end = clock();
    printf("back tracking %d queens for one solution, runtime: %f\n", N ,double(end - start)/CLOCKS_PER_SEC);

    //搜索全部解的时间
    start = clock();
    back_tracking(0);
    end = clock();
    printf("back tracking %d queens for all solution, runtime: %f\n", N ,double(end - start)/CLOCKS_PER_SEC);
}
