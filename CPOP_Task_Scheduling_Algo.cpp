#include "bits/stdc++.h"
using namespace std;

int dag[100][100],cost[100][100];
double ranksUp[100],ranksDown[100];
vector<pair<double,int>> rankOrder;
int n, m;

int avalProcessor[100][100];
int AST[100], assignedProcessor[100],actualFinishTime[100];

double calculateAvgRunningTime(int task) {
    double totalTime = 0;
    for(int proc = 0; proc < m; proc++) totalTime += cost[task][proc];
    totalTime /= m;
    return totalTime;
}

void calculateRankUp(int task) {
    double maxNext = 0;
    for(int next = 0; next < n; next++) {
        if(dag[task][next] == -1) continue;
        if(ranksUp[next] == -1) calculateRankUp(next);
        maxNext = max(maxNext, dag[task][next] + ranksUp[next]);
    }
    ranksUp[task] = calculateAvgRunningTime(task) + maxNext;
}
void calculateRankDown(int task){
    double maxPrev=0;
    for(int prev=0;prev<n;prev++){
        if(dag[prev][task]==-1)continue;
        if(ranksDown[prev]==-1)calculateRankDown(prev);
        maxPrev=max(maxPrev,dag[prev][task]+ranksDown[prev]+calculateAvgRunningTime(task));
    }
    ranksDown[task]=maxPrev;
}



void calculateAFT(int task) {
    //cout<<task<<endl;
    int EST=0;
    int AFT=1e9;
    
    
    for(int pred=0;pred<n;pred++){
        if(dag[pred][task]!=-1){
            if(assignedProcessor[pred]==-1){
                calculateAFT(pred);
            }
        }
    }
    int processor=-1;
    int p1[m];
    for(int proc=0;proc<m;proc++){
        int count=0;
        for(int prec=0;prec<n;prec++){
            if(dag[prec][task]!=-1){
                int count1=actualFinishTime[prec];
                if(assignedProcessor[prec]!=proc)count1+=dag[prec][task];
                count1=max(count1,avalProcessor[prec][proc]);
                count=max(count,count1);
            }
        }
        p1[proc]=count;
        if(count<AFT){
            AFT=count;
            processor=proc;
        }
    }
    
    
    //we do scheduling here
    if(assignedProcessor[task]==-1){
        assignedProcessor[task]=processor;
        avalProcessor[task][processor]=AFT;
    }else{
        avalProcessor[task][assignedProcessor[task]]=AFT;
    }
    
    actualFinishTime[task]=AFT;
}



int main() {
    cin>>n>>m; //No. of tasks, No. of processors
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) cin>>dag[i][j]; //dag[i][j] = amount of input task j needs from i
    }
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {cin>>cost[i][j];avalProcessor[i][j]=-1;assignedProcessor[j]=-1;} //cost[i][j] = time taken for task i to finish on processor j
    }
    
    for(int i=0;i<n;i++){
        ranksUp[i]=ranksDown[i]=-1;
        
    }
    for(int i=0;i<n;i++){
        calculateRankDown(i);
        calculateRankUp(i);
    }
    
    for(int task=0;task<n;task++){
        cout<<(ranksUp[task]+ranksDown[task])<<endl;
        rankOrder.push_back(make_pair(ranksUp[task]+ranksDown[task],task));
    }
    sort(rankOrder.rbegin(),rankOrder.rend());
    
    
    int processor=-1;
    int minSum=1e9;
    for(int i=0;i<m;i++){
        int sum=cost[rankOrder[0].second][i];
        int j=1;
        for(;j<n;j++){
            if(rankOrder[j].first!=rankOrder[j-1].first)break;
            sum+=cost[rankOrder[j].second][i];
        }
        if(minSum>sum){processor=i;minSum=sum;}
    }
    
    int j=1;
    assignedProcessor[rankOrder[0].second]=processor;
    actualFinishTime[rankOrder[0].second]=cost[rankOrder[0].second][processor];
    for(;j<n;j++){
        if(rankOrder[j].first!=rankOrder[j-1].first)break;
        assignedProcessor[rankOrder[j].second]=processor;
        actualFinishTime[rankOrder[j].second]=cost[rankOrder[j].second][processor]+actualFinishTime[rankOrder[j-1].second];
    }
    
    for(int i=0;i<n;i++)
    calculateAFT(rankOrder[i].second);
    
    cout << "Task\tProc \tAFT\n";
    int makespan = 0;
    for(int i = 0; i < n; i++) {
        cout << i << " \t\t " << assignedProcessor[i] << " \t " << actualFinishTime[i]  << "\n";
    } 
    
}
