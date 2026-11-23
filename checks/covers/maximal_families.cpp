// Section 5: list the maximal intersecting families of independent triples of
// C_c + P_p that have no common vertex. They are the maximal cliques of the
// "share a vertex" graph on independent triples (pivoted Bron-Kerbosch),
// kept when their common intersection is empty.
// Usage: maximal_families c p out.txt. Prints c, p, #triples, #families, #nodes.
#include "../bitset_iter.h"
#include <algorithm>
#include <cstdint>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
using B=std::bitset<512>;
using namespace std;
vector<int> triples;
array<B,512> adj;
B clique;
uint64_t calls=0, found=0;
ostream* listing=nullptr;
void bk(B p,B x,int common){
 ++calls;
 if(p.none()){ 
  if(!x.any()&&!common){
   ++found;
   if(listing){
    for(size_t i=find_first(clique);i<512;i=find_next(clique,i)) *listing<<i<<" ";
    *listing<<"\n";
   }
  }
  return;
 }
 B px=p|x;
 size_t u=find_first(px),max_deg=0;
 for(size_t a=u;a<512;a=find_next(px,a)){
  auto k=(p&adj[a]).count();
  if(k>max_deg){max_deg=k;u=a;}
 }
 B branch=p&~adj[u];
 for(size_t v=find_first(branch);v<512;v=find_next(branch,v)){
  clique.set(v);
  bk(p&adj[v],x&adj[v],common&triples[v]);
  clique.reset(v); p.reset(v); x.set(v);
 }
}
int main(int argc,char**argv){
 int c=atoi(argv[1]), p=atoi(argv[2]), n=c+p;
 assert(c>=9&&p>=2&&n<=16);
 int cm=(1<<c)-1;
 for(int s=0;s<(1<<n);++s)if(__builtin_popcount(s)==3){
  int a=s&cm,b=s>>c;
  if(!(a&((a<<1)|(a>>(c-1)))) && !(b&(b<<1)))triples.push_back(s);
 }
 size_t k=triples.size(); B all;
 assert(k<=512);
 for(size_t i=0;i<k;++i){
  all.set(i);
  for(size_t j=0;j<k;++j) if(i!=j&&(triples[i]&triples[j]))adj[i].set(j);
 }
 ofstream out;
 if(argc==4){out.open(argv[3]);if(!out)return 2;listing=&out;}
 bk(all,B(),(1<<n)-1);
 cout<<c<<" "<<p<<" "<<k<<" "<<found<<" "<<calls<<"\n";
}
