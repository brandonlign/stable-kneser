// Section 5, first method: decide exactly whether the families listed by
// maximal_families can cover every independent triple using at most m of them.
// Branch and bound; the integer weights in weights/ only strengthen the
// pruning bounds, which hold for any nonnegative weights.
// Usage: exact_cover families.txt weights.txt m. Prints UNSAT if no cover exists.
#include "../bitset_iter.h"
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using S=std::bitset<256>;
struct Family {S vertices;int id=0,w=0,num=0;};
std::vector<Family> F;
std::vector<int> W;
std::vector<int> stack,answer;
uint64_t nodes=0;
int sum_weight(const S&A){
 int w=0;
 for(size_t a=find_first(A);a<256;a=find_next(A,a))w+=W[a];
 return w;
}
bool search(const S& missing,const std::vector<Family>& families,int slots,int reqw){
 ++nodes;
 if(missing.none()){answer=stack;return true;}
 if(slots==0||families.empty())return false;
 int reqn=missing.count();
 int topw[20]={},topn[20]={};
 auto put=[&](int a,int* b){ for(int i=0;i<slots;i++) if(a>b[i]) std::swap(a,b[i]); };
 for(auto& f:families){put(f.w,topw);put(f.num,topn);}
 int sumw=0,sumn=0;
 for(int i=0;i<slots;i++){sumw+=topw[i];sumn+=topn[i];}
 if(sumw<reqw||sumn<reqn)return false;
 if(slots==1){
  for(auto&f:families)if(f.vertices==missing){answer=stack;answer.push_back(f.id);return true;}
  return false;
 }
 std::vector<Family> active;
 int ha[256]={};
 for(auto& f:families) {
  if(sumw-topw[slots-1]+f.w<reqw||sumn-topn[slots-1]+f.num<reqn)continue;
  active.push_back(f);
  for(size_t x=find_first(f.vertices);x<256;x=find_next(f.vertices,x))++ha[x];
 }
 size_t pick=256;
 for(size_t x=find_first(missing);x<256;x=find_next(missing,x)){
  if(!ha[x])return false;
  if(pick==256||ha[x]<ha[pick]||(ha[x]==ha[pick]&&W[x]>W[pick]))pick=x;
 }
 std::vector<Family> rest;
 std::vector<Family> branch;
 for(auto&f:active)(f.vertices[pick]?branch:rest).push_back(f);
 std::sort(branch.begin(),branch.end(),[](const auto&a,const auto&b){if(a.w!=b.w)return a.w<b.w;return a.num<b.num;});
 for(size_t j=branch.size();j-->0;){
  auto& f=branch[j];
  auto m=missing&~f.vertices;
  int w=reqw-f.w,n=reqn-f.num;
  std::vector<Family> next;
  // Fixing f and another family leaves slots-2 families. Their untrimmed
  // weights (or sizes) are bounded by the corresponding top-(slots-2) sum.
  auto append = [&](Family g) {
   if(g.w+(sumw-topw[slots-1]-topw[slots-2])<w ||
      g.num+(sumn-topn[slots-1]-topn[slots-2])<n) return;
   g.vertices &= m;
   g.num=g.vertices.count();
   if(!g.num)return;
   g.w=sum_weight(g.vertices);
   next.push_back(g);
  };
  for(auto g:rest) append(g);
  for(size_t q=0;q<j;++q) append(branch[q]);
  stack.push_back(f.id);
  if(search(m,next,slots-1,w))return true;
  stack.pop_back();
 }
 return false;
}
int main(int argc,char**argv){
 if(argc!=4)return 2;
 std::ifstream iw(argv[2]),in(argv[1]);
 if(!iw||!in)return 2;
 int a;while(iw>>a){if(a<0)return 2;W.push_back(a);}
 if(W.empty()||W.size()>256||!iw.eof())return 2;
 std::string s;
 while(std::getline(in,s)){
  Family f;f.id=F.size();std::istringstream is(s);
  while(is>>a){if(a<0||size_t(a)>=W.size())return 2;f.vertices.set(a);}
  if(!is.eof()||f.vertices.none())return 2;
  f.num=f.vertices.count();f.w=sum_weight(f.vertices);
  F.push_back(f);
 }
 S all;for(size_t i=0;i<W.size();i++)all.set(i);
 const int k=std::stoi(argv[3]);
 if(k<1||k>19||F.empty())return 2;
 bool sat=search(all,F,k,sum_weight(all));
 std::cout<<(sat?"SAT":"UNSAT")<<" nodes "<<nodes<<"\n";
 if(sat){
  for(int a:answer)std::cout<<a<<" ";
  std::cout<<"\n";
 }
 return sat ? 1 : 0;
}
