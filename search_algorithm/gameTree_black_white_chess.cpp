#include <iostream>
#include <string>
#include <climits>
#include <cstdlib>
using namespace std;

//八个方向 
const int dir[8][2] = {{-1,-1},{-1,0},{-1,1},
						{0,-1},{0,1},
						{1,-1},{1,0},{1,1}
						};  
						
//棋盘大小 						
const int boardSize = 8;   

//不同位置对应的分数权值 
int scoreGrid[boardSize][boardSize] = {{1000,-60,10,10,10,10,-60,1000},
											{-60,-80,5,5,5,5,-80,-60},
											{10,5,1,1,1,1,5,10},
											{10,5,1,1,1,1,5,10},
											{10,5,1,1,1,1,5,10},
											{10,5,1,1,1,1,5,10},
											{-60,-80,5,5,5,5,-80,-60},
											{1000,-60,10,10,10,10,-60,1000}};

//判断棋盘board是否可以下棋 
template<int N>
bool canplay(string (&board)[N]){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(board[i][j]=='*') return true;
		}
	}
	return false;
}

/* 为board添加 '*' 号 */
template<int N>
void helpPlay(string (&board)[N], int step){
	//将能下的位置改为'*' ，轮到黑下，则找白色，提供帮助 
	char findPiece = (step == 0 ? 'w' : 'b');     //需要找什么棋子 
	char currentPiece = findPiece=='b'?'w':'b';  //现在轮到哪种棋子下 
	for(int x=0;x<boardSize;x++){
		for(int y=0;y<boardSize;y++){
			if(board[x][y]=='b' || board[x][y]=='w') continue; //有棋子不设置'*' 
			bool flag2 = false; //用于判断是否要设置'-' 
			for(int k=0;k<8;k++){ //8个方向 
				//(nx, ny), (x, y)旁边的棋子 
				int nx=x+dir[k][0];
				int ny=y+dir[k][1];
				bool flag = false;  //用于判断是否要设置'*' 
				//cout << "play: " << nx << " " << ny << endl; 
				while(nx>=0 && nx<=7 && ny>=0 && ny<=7 && board[nx][ny]==findPiece){
					flag=true;
					nx+=dir[k][0];
					ny+=dir[k][1];
				}
				if(flag && nx>=0 && nx<=7 && ny>=0 && ny<=7 && board[nx][ny]==currentPiece){
					board[x][y] = '*';
					flag2=true;
					break;
				}
			}
			if(flag2==false) board[x][y]='_';
		}
	}
} 

/* 在board[x][y]处下棋 */
template<int N>
bool play(int x,int y, string (&board)[N], int& step){
	//判断下的位置是否合法
	if(board[x][y]!='*') return false; 
	//修改下的位置的状态 
	board[x][y] = step==0 ? 'b' : 'w';
	//翻转棋子
	char eatchess = board[x][y]=='b'?'w':'b';
	for(int p=0;p<8;p++){
		//判断是否是反色棋子 
		int nx = x+dir[p][0];
		int ny = y+dir[p][1];
		//cout << "play: " << nx << " " << ny << endl; 
		while(nx>=0 && nx<=7 && ny>=0 && ny<=7 && board[nx][ny]==eatchess){
			nx += dir[p][0];
			ny += dir[p][1];
		}
		//判断是否出现了同色棋子，是的话则翻转 
		if(nx>=0 && nx<=7 && ny>=0 && ny<=7 && board[nx][ny]==board[x][y]){
			nx = x+dir[p][0];
			ny = y+dir[p][1];
			while(nx>=0 && nx<=7 && ny>=0 && ny<=7 && board[nx][ny]==eatchess){
				board[nx][ny]=board[x][y]; 
				nx += dir[p][0];
				ny += dir[p][1];
			}
		}
	}
	
	step ^= 1;
	helpPlay(board,step);
	return true;
} 

template<int N>
class chess{
	public:
		string board[N];       //棋盘元素 
		int step;              //判断轮到谁下，0黑 1白 
		
		chess();
		void print_chessboard();  //打印棋盘
		bool playerPlay(int x, int y);  //玩家下棋 
		bool AIPlay();   //电脑下棋 
		
		/*博弈树搜索，depth=0时返回搜索位置，depth>0时返回当前节点的score */
		int gameDfs(string (&b)[N],int step,int score, int scorePar, int depth,int maxdepth);  
		//估算节点的分数
		int fscore(string (&b)[N], int step);
		
		
		void result();  //输出结果，包括谁赢等信息 
};

template<int N>
chess<N>::chess(){
	step = 0;
	for(int i=0;i<boardSize;i++){
		board[i] = "________";
	}
	board[3] = "___wb___";
	board[4] = "___bw___";
	helpPlay(board,step);
}

template<int N>
void chess<N>::print_chessboard(){
	int bcnt = 0, wcnt=0;
	cout << "  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n"; 
	for(int i=0;i<boardSize;i++){
		cout << i+1 << " | ";
		for(int j=0;j<boardSize;j++){
			if(board[i][j]=='b')  {
				cout << "●| ";
				bcnt += 1;
			}
			else if(board[i][j]=='w') {
				cout << "○| ";
				wcnt+=1;
			}
			else cout << board[i][j] << " | ";
		}
		cout << "\n\n";
	}
	cout << "black(player)   score: " << bcnt << endl;
	cout << "white(computer) score: " << wcnt << endl;
}

template<int N>
bool chess<N>::playerPlay(int x, int y){
	return play(x-1,y-1,board,step);
}

template<int N>
bool chess<N>::AIPlay(){
	//若不能下，则直接返回，不用预测 
	if(!canplay(board)) return false;
	
	//改变scoreGrid的权值
	if(scoreGrid[0][0] == 'w') {
		scoreGrid[0][1] = scoreGrid[1][0] = scoreGrid[1][1] = 100;
	} 
	if(scoreGrid[0][7] == 'w'){
		scoreGrid[0][6] = scoreGrid[1][6] = scoreGrid[1][7] = 100;
	}
	if(scoreGrid[7][0] == 'w'){
		scoreGrid[7][1] = scoreGrid[6][0] = scoreGrid[6][1] = 100;
	}
	if(scoreGrid[7][7] == 'w'){
		scoreGrid[7][6] = scoreGrid[6][6] = scoreGrid[6][7] = 100;
	}
	
	//预测AI下一步的走法 
	int pos = gameDfs(board,step,-INT_MAX,INT_MAX,0,6); // 下子的位置 
	cout << "AI play pos: " << pos/boardSize+1 << " " << pos%boardSize+1 << endl;
	play(pos/boardSize,pos%boardSize,board,step);
	return true;
}

/*
	b: 棋盘的状态, boardSize: 棋盘大小
	step: 0表示黑棋回合(极小值结点), 1表示白棋回合(极大值结点)
	score: 当前结点的分数，极小值结点初始化为INT_MAX，极大值结点初始化为-INT_MAX
	depth: 当前搜索深度
	maxdepth: 设定搜索的最大深度，当depth > maxdepth时，退出搜索，返回搜索分数
*/
template<int N>
int chess<N>::gameDfs(string (&b)[N],int step,int score, int scorePar, int depth, int maxdepth){
	
	//深度大于最大深度时，返回当前节点的分数 
	if(depth > maxdepth)  return fscore(b,step); 
	
	//不能下直接下一个
	if(!canplay(b)){
		int steptemp = step^1;
		string btemp[N];
		for(int i=0;i<N;i++){
			btemp[i] = b[i];
		}
		helpPlay(btemp,steptemp);
		int scorex = gameDfs(btemp,steptemp, (steptemp ? -INT_MAX : INT_MAX), score,depth+1,maxdepth);
		//极小节点 
		if(step==0){
			if(scorex < score){
				//beta剪枝
				score = scorex;
				if(score <= scorePar) return score;
			}
		}
		//极大节点 
		else{
			if(scorex > score){
				//alpha剪枝
				score = scorex;
				if(score >= scorePar) return score;
			} 				
		}
		return score;
	} 
	else{
		int bestxy = 0;
		for(int x=0;x<boardSize;x++){
			for(int y=0;y<boardSize;y++){
				//不是'*'不可以下 
				if(b[x][y]!='*') continue;
				//新建临时board 
				string btemp[N];
				for(int i=0;i<N;i++){
					btemp[i] = b[i];
				}
				int steptemp = step;
	
				play(x,y,btemp,steptemp);
	
				int scorex = gameDfs(btemp,steptemp, (steptemp ? -INT_MAX : INT_MAX), score,depth+1,maxdepth);
				
				//极小节点 
				if(step==0){
					if(scorex < score){
						//beta剪枝
						score = scorex;
						if(score <= scorePar) return score;
						//行动的位置 
						if(depth==0) bestxy = boardSize*x+y; 
					}
				}
				//极大节点 
				else{
					if(scorex > score){
						//alpha剪枝
						score = scorex;
						if(score >= scorePar) return score;
						//行动的位置 
						if(depth==0) bestxy = boardSize*x+y; 
						
					} 				
				}
				
			}
		}
		if(depth>0) return score;
		return bestxy;
	}
	
	
}

//针对一个棋盘计算双方得分，最后节点得分为：黑(0b)-白(1w) 
template<int N>
int chess<N>::fscore(string (&b)[N], int step){
	int bscore=0,wscore=0;
	for(int x=0;x<boardSize;x++){
		for(int y=0;y<boardSize;y++){
			//算棋子位置得分 
			if(b[x][y]=='b') bscore += scoreGrid[x][y] + 12;
			else if(b[x][y]=='w') wscore += scoreGrid[x][y] + 12;

			//算空位得分 
			if(b[x][y]=='*') {
				if(step==0) bscore+=5;
				else wscore+=5;
			} 
		}
	}
	return wscore-bscore;
}

template<int N>
void chess<N>::result(){
	int bcnt=0,wcnt=0;
	for(int x=0;x<boardSize;x++){
		for(int y=0;y<boardSize;y++){
			if(board[x][y]=='b') bcnt+=1;
			else if(board[x][y] == 'w') wcnt+=1;
		}
	}
	if(bcnt==wcnt) cout << "draw" << endl;
	else if(bcnt>wcnt) cout << "black(player) win" << endl;
	else cout << "white(AI) win" << endl; 
} 

int main(){
	chess<8> cb;
	bool flagPlayer = true;
	bool flagAI = true;
	while(flagPlayer || flagAI){
		//输出棋盘 
		cb.print_chessboard();
		cout << "\n-------------------------------------\n"; 
		
		//人，判断是否可以下
		if(canplay(cb.board)){
			//输入下子位置 
			int x,y;
			cout << "please input pos of your chess: \n";
			cin >> x >> y;
			while(!cb.playerPlay(x,y)) {
				cout << "invalid, please input pos of your chess\n";
				cin >> x >> y;
			}	
			flagPlayer = true;
		}else{
			flagPlayer = false;
			cb.step^=1;
			helpPlay(cb.board,cb.step);
		}

		//输出棋盘 
		cb.print_chessboard();
		cout << "\n-------------------------------------\n"; 

		//电脑下
		flagAI = cb.AIPlay();
		if(flagAI==false){
			cb.step^=1;
			helpPlay(cb.board,cb.step);
		}
	}
	//判断谁赢
	cb.result(); 
}
