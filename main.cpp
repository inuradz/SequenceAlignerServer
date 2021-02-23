#include <thread>
#include <iostream>
#include <thread>
#include <vector>
#include <functional>

#include<ctime>

#include "SequenceAligner/Matrixes/Scoring_Matrix.h"
#include "SequenceAligner/Matrixes/BLOSUM62.h"

#include "SequenceAligner/src/NeedlemanWunsch.h"

std::vector<std::string> a;
std::vector<std::vector<int> > scores;

void thread_align_seq(NeedlemanWunsch &n, int seq_a){
    int seq_b;
    for (seq_b = 0; seq_b < a.size(); seq_b++)
    //n.alignSequence(a[seq_a],a[seq_b]);
    scores[seq_a][seq_b] = n.alignSequence(a[seq_a],a[seq_b]);
}



int main(){
    BLOSUM62 b;
    std::string s;
    while (std::cin >> s){
        a.push_back(s);
    }
    int i;
    for (i = 0; i < a.size();i++){
        std::vector<int> t(a.size(),0);
        scores.push_back(t);
    }
    // Declaring argument for time() 
    time_t tt; 
  

    std::vector<std::thread> v;
    std::vector<NeedlemanWunsch> nV;
    for ( i = 0 ; i < std::thread::hardware_concurrency()-3; i++){
        NeedlemanWunsch n(10000, -10, b);
        nV.push_back(n);
    }
    time(&tt);
    std::cout << "made the needleman" << asctime(localtime(&tt)) << std::endl;
    int j;
    for(j=0;j<a.size();j+=32){
        time(&tt);
        std::cout << "processing :" << j << asctime(localtime(&tt)) << std::endl;
        for ( i = 0 ; i < std::thread::hardware_concurrency()-3; i++){
            v.push_back(std::thread (thread_align_seq,std::ref(nV[i]),j+i));
        }
        time(&tt);
        std::cout << "threads made" << asctime(localtime(&tt)) << std::endl;
        for ( i = 0 ; i < std::thread::hardware_concurrency()-3; i++){
            v[i].join();
        }
        time(&tt);
        std::cout << "all threads joined" << asctime(localtime(&tt)) << std::endl;
        v.clear();
        
    }
    return 0;
}