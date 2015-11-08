#!/bin/sh
echo FOLD $1
./gist.2.3/bin/gist-train-svm -train fold$1/trainFile$1.res -class fold$1/trainTargetFile$1.res -matrix -nonormalize > fold$1/weight.pes
./gist.2.3/bin/gist-classify -test fold$1/testFile$1.res -learned fold$1/weight.pes > fold$1/final.txt
./hope-k -a fold$1/testTargetFile$1.res -c fold$1/final.txt
