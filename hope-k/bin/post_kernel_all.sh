#!/bin/sh
echo FOLD 0 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold0/trainFile0.res -class fold0/trainTargetFile0.res -matrix -nonormalize > fold0/weight.pes
./gist.2.3/bin/gist-classify -test fold0/testFile0.res -learned fold0/weight.pes > fold0/final.txt
./hope-k -a fold0/testTargetFile0.res -c fold0/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 1 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold1/trainFile1.res -class fold1/trainTargetFile1.res -matrix -nonormalize > fold1/weight.pes
./gist.2.3/bin/gist-classify -test fold1/testFile1.res -learned fold1/weight.pes > fold1/final.txt
./hope-k -a fold1/testTargetFile1.res -c fold1/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 2 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold2/trainFile2.res -class fold2/trainTargetFile2.res -matrix -nonormalize > fold2/weight.pes
./gist.2.3/bin/gist-classify -test fold2/testFile2.res -learned fold2/weight.pes > fold2/final.txt
./hope-k -a fold2/testTargetFile2.res -c fold2/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 3 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold3/trainFile3.res -class fold3/trainTargetFile3.res -matrix -nonormalize > fold3/weight.pes
./gist.2.3/bin/gist-classify -test fold3/testFile3.res -learned fold3/weight.pes > fold3/final.txt
./hope-k -a fold3/testTargetFile3.res -c fold3/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 4 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold4/trainFile4.res -class fold4/trainTargetFile4.res -matrix -nonormalize > fold4/weight.pes
./gist.2.3/bin/gist-classify -test fold4/testFile4.res -learned fold4/weight.pes > fold4/final.txt
./hope-k -a fold4/testTargetFile4.res -c fold4/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 5 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold5/trainFile5.res -class fold5/trainTargetFile5.res -matrix -nonormalize > fold5/weight.pes
./gist.2.3/bin/gist-classify -test fold5/testFile5.res -learned fold5/weight.pes > fold5/final.txt
./hope-k -a fold5/testTargetFile5.res -c fold5/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 6 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold6/trainFile6.res -class fold6/trainTargetFile6.res -matrix -nonormalize > fold6/weight.pes
./gist.2.3/bin/gist-classify -test fold6/testFile6.res -learned fold6/weight.pes > fold6/final.txt
./hope-k -a fold6/testTargetFile6.res -c fold6/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 7 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold7/trainFile7.res -class fold7/trainTargetFile7.res -matrix -nonormalize > fold7/weight.pes
./gist.2.3/bin/gist-classify -test fold7/testFile7.res -learned fold7/weight.pes > fold7/final.txt
./hope-k -a fold7/testTargetFile7.res -c fold7/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 8 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold8/trainFile8.res -class fold8/trainTargetFile8.res -matrix -nonormalize > fold8/weight.pes
./gist.2.3/bin/gist-classify -test fold8/testFile8.res -learned fold8/weight.pes > fold8/final.txt
./hope-k -a fold8/testTargetFile8.res -c fold8/final.txt >> ./auto.res
echo "" >> ./auto.res
echo FOLD 9 >> ./auto.res
./gist.2.3/bin/gist-train-svm -train fold9/trainFile9.res -class fold9/trainTargetFile9.res -matrix -nonormalize > fold9/weight.pes
./gist.2.3/bin/gist-classify -test fold9/testFile9.res -learned fold9/weight.pes > fold9/final.txt
./hope-k -a fold9/testTargetFile9.res -c fold9/final.txt >> ./auto.res
echo ""  >> ./auto.res
