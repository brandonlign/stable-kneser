#!/usr/bin/env python3
"""Section 5, second method: for each of the six graphs C_c + P_p in Table 1,
no n-5 maximal nonstar intersecting families of independent triples cover all
independent triples. For each orbit of possible largest family A, the stored
certificate (no candidates, an uncovered triple, or integer weights) is
checked in exact integer arithmetic. Usage: verify_orbits.py <maximal_families exe>.
Prints one OK line per graph; any failure raises an AssertionError."""
from pathlib import Path
from itertools import combinations, product
from subprocess import run
from tempfile import TemporaryDirectory
import sys

HERE = Path(__file__).resolve().parent
CASES = [(9,2),(9,3),(10,2),(9,4),(10,3),(11,2)]

def verify(c,p,exe,tmp):
    n, k = c+p, c+p-5
    base_edges = {frozenset((i,(i+1)%c)) for i in range(c)}
    base_edges |= {frozenset((i,i+1)) for i in range(c,n-1)}
    stable = [t for t in combinations(range(n),3)
              if all(frozenset(e) not in base_edges for e in combinations(t,2))]
    stable.sort(key=lambda t: sum(1<<v for v in t))
    assert len(stable) == {11:84,12:120,13:165}[n]
    nT = len(stable)
    tripid = {frozenset(t):i for i,t in enumerate(stable)}
    path = Path(tmp)/"families.txt"
    run([exe,str(c),str(p),str(path)],check=True)
    families = [frozenset(map(int,row.split())) for row in path.read_text().splitlines()]
    size = list(map(len,families))
    assert len(set(families)) == len(families)
    index = {f:i for i,f in enumerate(families)}
    alltrip = frozenset(range(nT))
    actions = []
    for s,d,r in product(range(c),(-1,1),(False,True)):
        perm = [(s+d*i)%c if i<c else n+c-1-i if r else i for i in range(n)]
        actions.append([tripid[frozenset(perm[v] for v in t)] for t in stable])
    todo = set()
    for i,f in enumerate(families):
        if k*size[i]>=nT:
            todo.add(min(index[frozenset(u[t] for t in f)] for u in actions))
    lines = (HERE/f"C{c}-P{p}.txt").read_text().splitlines()
    proved = set()
    for row in lines:
        kind,s,*rest = row.split()
        i = int(s)
        assert i in todo and i not in proved
        proved.add(i)
        rem = sorted(alltrip-families[i])
        cand = [g for j,g in enumerate(families)
                if i != j and size[j]<=size[i] and
                len(g-families[i]) >= max(1,len(rem)-(k-2)*size[i])]
        if kind=="count":
            assert not cand and rest == [str(len(rem))]
        elif kind=="uncovered":
            assert len(rest)==1 and int(rest[0]) in rem
            assert all(int(rest[0]) not in g for g in cand)
        elif kind=="weights":
            q,*w = map(int,rest)
            assert len(w)==len(rem) and min(w)>=0 and q>=0
            assert sum(w)>(k-1)*q
            weight = dict(zip(rem,w))
            assert all(sum(weight.get(t,0) for t in g)<=q for g in cand)
        else:
            raise ValueError(kind)
    assert proved == todo, (c,p,todo-proved)
    print(f"OK C{c}+P{p}: {len(proved)} largest-family orbit certificates",flush=True)

if __name__ == '__main__':
    if not __debug__:
        raise RuntimeError("verification requires Python assertions")
    exe = str(Path(sys.argv[1]).resolve())
    with TemporaryDirectory() as tmp:
        for c,p in CASES:
            verify(c,p,exe,tmp)
