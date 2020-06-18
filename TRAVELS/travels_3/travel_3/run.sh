cd ../algorithms
make
cd ../simulator
make 
simulator -travel_path ../travels/travels_$1 -algorithm_path ../algorithms/ -output ../output
