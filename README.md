# A characterization of 3-tight graphs

Brandon Li

Paper and verification code proving the case $k=3$ of Conjecture 1.3 of
Arana, Haxell, Kaiser and Stehlík
([arXiv:2610.17884](https://arxiv.org/abs/2610.17884)). The $G$-stable Kneser
graph $\mathrm{KG}(G,k)$ has the independent $k$-sets of $G$ as vertices, two
of them adjacent when disjoint, and $G$ is $k$-tight when
$\chi(\mathrm{KG}(G,k))=n-2k+2$, the largest possible value.

**Theorem.** A graph on at least seven vertices is 3-tight if and only if it
is a cycle, or every component is a path or a cycle of length at least 9.

In particular, a cycle $C_c$ together with any number of isolated vertices is
3-tight exactly when $c\ge9$. The one-isolate case was previously known for
$c=9$, $10$ and $c\ge15$
([AFTD OP-193](https://rainwangphy.github.io/aftd-platform/blog/2026-11-12-op-193/)).
For every $k\ge3$, the paper also shows that if the conjecture fails, it fails
on a graph with at most $k^3-k-1$ vertices.

- [Paper (PDF)](paper/main.pdf)
- [LaTeX source](paper/)
- [Verification code](checks/)

## Reproduce

```sh
bash checks/run_all.sh
```

This needs a C++17 compiler and Python 3 (standard library only) and takes
about ten seconds. It compiles and runs:

- `isolate/verifier.cpp`: the exhaustive search of Lemma 3.2, showing that
  $C_c$ plus one isolated vertex is 3-tight for $9\le c\le12$.
- `covers/maximal_families.cpp`: lists the maximal intersecting nonstar
  families of independent triples for the six graphs in Table 1.
- `covers/exact_cover.cpp`: an exact branch-and-bound search showing that
  no $n-5$ of these families cover all independent triples.
- `orbits/verify_orbits.py`: the same conclusion by a second method, checking
  integer weight certificates for each orbit of largest family.

All arithmetic is exact. The output is in
[`checks/output.txt`](checks/output.txt). The PDF builds with Tectonic
(`cd paper && tectonic main.tex`) or `pdflatex`.

## License

Code: MIT. Paper: CC BY 4.0.
