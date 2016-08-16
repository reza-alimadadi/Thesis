reset
make clean
make
cd python/
python3.4 fir.py $1
python3.4 est_fir.py > ../estLog
cd ../
./quality input/spec input/inputs input/outputs > calcLog
python3.4 relative.py

rm -rf estLog calcLog
