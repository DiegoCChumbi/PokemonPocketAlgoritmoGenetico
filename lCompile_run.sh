g++ main.cpp -o main
if [ $? -ne 0 ]; then
    echo "ERROR DE COMPILACION"
    exit 1
fi
./main