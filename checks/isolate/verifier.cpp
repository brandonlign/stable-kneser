// Lemma 3.2: C_c plus one isolated vertex is 3-tight for 9 <= c <= 12.
// For each c, enumerate every collection of c-4 independent triples of C_c
// ("anchors") such that no two anchors share a pair and every independent
// triple contains a pair of some anchor, then test the pair condition of
// Lemma 3.1 on each collection. The last column (survivors) must be 0.
// Exhaustive, no symmetry pruning, integer arithmetic only.
#include "../bitset_iter.h"
#include <bitset>
#include <vector>
#include <iostream>
#include <cstdlib>
using M=std::bitset<256>;
int n, target, cyl;
std::vector<int> triples, pairs;
std::vector<M> cover;
long long nodes=0,leaves=0,feasible=0;
bool meets(int pair, M xs) {
 for (size_t x=find_first(xs); x<256; x=find_next(xs,x))
  if (!(triples[x]&pair)) return false;
 return true;
}
void visit(M outstanding,M cand,std::vector<int>& selected) {
 ++nodes;
 if(selected.size()==(size_t)target) {
  if(outstanding.any())return;
  ++leaves;
  std::vector<M> forced;
  for (int x:selected) {
   M a=cover[x];
   for(int y:selected)if(x!=y)a&=~cover[y];
   forced.push_back(a);
  }
  for (int f:pairs) {
   bool good=false;
   for (size_t i=0;i<selected.size();++i)
    if ((f&triples[selected[i]])&&meets(f,forced[i])){good=true;break;}
   if(!good)return;
  }
  ++feasible; return;
 }
 if(selected.size()+cand.count()<(size_t)target)return;
 // If every triple is covered, still enumerate all extensions up to target.
 M best=cand;
 if(outstanding.any()){
  size_t number=best.count()+1;
  for(size_t t=find_first(outstanding);t<256;t=find_next(outstanding,t)){
   M b=cand&cover[t];size_t score=b.count(); if(!score)return;
   if(score<number){best=b;number=score;if(number==1)break;}
  }
 }
 // Exclude earlier choices of the selected uncovered triple in each branch.
 for(size_t x=find_first(best);x<256;x=find_next(best,x)){
  cand.reset(x); selected.push_back(x);
  visit(outstanding&~cover[x],cand&~cover[x],selected);
  selected.pop_back();
 }
}
int check(int cycle_length, int old_isolates) {
 cyl = cycle_length;
 n = cyl + old_isolates;
 target = n - 4;
 triples.clear(); pairs.clear(); cover.clear();
 nodes = leaves = feasible = 0;
 const int mask=(1<<cyl)-1;
 for(int S=0;S<(1<<n);++S){
  int T=S&mask;
  if(T&((T<<1)|(T>>(cyl-1))))continue;
  int k=__builtin_popcount((unsigned)S);
  if(k==2)pairs.push_back(S);
  if(k==3)triples.push_back(S);
 }
 cover.resize(triples.size());
 if(triples.size()>256)return 2;
 M all;
 for(size_t i=0;i<triples.size();++i){
  all.set(i);
  for(size_t j=0;j<triples.size();++j)
   if(__builtin_popcount((unsigned)(triples[i]&triples[j]))>=2)cover[i].set(j);
 }
 std::vector<int> selected;
 visit(all,all,selected);
 std::cout<<cyl<<" "<<triples.size()<<" "<<nodes<<" "<<leaves<<" "<<feasible<<"\n";
 return feasible!=0;
}

int main() {
 std::cout << "cycle independent_triples search_nodes covering_collections survivors\n";
 for (int length = 9; length <= 12; ++length)
   if (int status = check(length, 0)) return status;
}
