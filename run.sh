RUNS=10
PROBLEMS=("radio" "nqueens8x8" "nqueens16x16" "nqueens32x32" "nqueens64x64" "nqueens128x128" "sat")

for p in ${PROBLEMS[*]}
do
    mkdir -p out/$p
    for i in $(seq 1 $RUNS)
    do
        if [[ "$p" == "sat" ]]
        then
            { time ./build/ocev-ga ./examples/confs/sat.conf ./out/sat/run$i.csv < ./examples/sat/uf100-01.cnf; } > ./out/sat/run$i.out 2>&1
        else
            { time ./build/ocev-ga ./examples/confs/$p.conf ./out/$p/run$i.csv; } > ./out/$p/run$i.out 2>&1
        fi
    done
done