#include <list>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <Windows.h>
#include <ctime>
#include <fstream>
using namespace std;

//N 皇后问题
#define N 15

int dom[N][N];
int pos[N];   //每一行的皇后对应的位置
ofstream co("fc10.txt");

//输出棋盘皇后的分布
void print_board(){
    for(int i=0;i<N;i++){
        co << pos[i] << " ";
    }
    co << endl;
}

//输出dom
void print_dom(){
    cout << "-----------------------------------\n";
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            cout << dom[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------------------------------\n";
}

//根据新加的行修改后面行的可行域
bool modify_dom(int r){
    for(int i=r+1;i<N;i++){
        //修改dom
        int left = pos[r] - (i-r);
        int right = pos[r]+(i-r);
        if(dom[i][pos[r]] == -1) dom[i][pos[r]] = r;  //竖
        if(left>=0 && dom[i][left]==-1) dom[i][left] = r; //左下角
        if(right<N && dom[i][right]==-1) dom[i][right] = r;  //右下角

        //判断dom是否为空
        bool flag = false;
        for(int j=0;j<N;j++)
            if(dom[i][j]==-1){
                flag = true;
                break;
            }
        if(flag == false) return false;
    }
    return true;
}

//修改dom，恢复之前的状态
void recovery(int r){
    for(int i=r+1;i<N;i++){
        for(int j=0;j<N;j++)
            if(dom[i][j]==r){
                dom[i][j]=-1;
            }
    }
}


//只搜索一个答案
bool forward_checking1(int r){
    if(r >= N) {
        //print_board();
        return true;
    }
    for(int i=0;i<N;i++){
        if(dom[r][i] == -1) {
            pos[r] = i;
            if(modify_dom(r) && forward_checking1(r+1)) return true; 
            recovery(r);
        }
    }
    return false;
}

//搜索全部答案
void forward_checking(int r){
    if(r >= N) {
        //print_board();
        return ;
    }
    for(int i=0;i<N;i++){
        //cout << r << " " << i << endl;
        //print_dom();
        if(dom[r][i] == -1) {
            pos[r] = i;
            if(modify_dom(r)) forward_checking(r+1);
            recovery(r);
        }
    }
}

int main(){
    //init
    memset(dom,-1,sizeof(dom));
    clock_t start, end;

    //搜索一个解的时间
    start = clock();
    forward_checking1(0);
    end = clock();
    printf("forward checking %d queens for one solution, runtime: %f\n", N ,double(end - start)/CLOCKS_PER_SEC);

    //搜索全部解的时间
    start = clock();
    forward_checking(0);
    end = clock();
    printf("forward checking %d queens for all solution, runtime: %f\n", N ,double(end - start)/CLOCKS_PER_SEC);
}
