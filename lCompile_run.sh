g++ main.o -o main
if [ $? -Ne 0 ]; then
    echo "ERROR DE COMPILACION"
    exit 1
fi
main