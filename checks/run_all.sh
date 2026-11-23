#!/usr/bin/env bash
# Runs every computer check in the paper and writes checks/output.txt.
# Needs a C++17 compiler and Python 3 (standard library only).
set -euo pipefail
cd -- "$(dirname -- "$0")"
build=build; mkdir -p "$build"
CXX=${CXX:-c++}
$CXX -std=c++17 -O3 isolate/verifier.cpp -o "$build/isolate"
$CXX -std=c++17 -O3 covers/maximal_families.cpp -o "$build/families"
$CXX -std=c++17 -O3 covers/exact_cover.cpp -o "$build/cover"

{
echo "== Lemma 3.2: a cycle of length 9-12 plus one isolated vertex"
"$build/isolate" | tee "$build/isolate.out"
diff -q isolate/expected-output.txt "$build/isolate.out" >/dev/null
awk 'NR>1 && $5!=0 {bad=1} END {exit bad}' "$build/isolate.out"
echo "no collection survives in any of the four cases"

echo
echo "== Section 5: no cover by n-5 nonstar families (Table 1), branch-and-bound search"
for case in '9 2' '9 3' '10 2' '9 4' '10 3' '11 2'; do
  read -r c p <<< "$case"
  line=$("$build/families" "$c" "$p" "$build/triples")
  read -r _c _p triples families _nodes <<< "$line"
  result=$("$build/cover" "$build/triples" "covers/weights/C$c-P$p.txt" "$((c+p-5))")
  [ "${result%% *}" = UNSAT ]
  echo "C$c+P$p: $triples independent triples, $families maximal nonstar families, no cover by $((c+p-5))"
done

echo
echo "== Section 5: no cover, integer weight certificates (second method)"
python3 orbits/verify_orbits.py "$build/families"

echo
echo "all checks passed"
} 2>&1 | tee output.txt
