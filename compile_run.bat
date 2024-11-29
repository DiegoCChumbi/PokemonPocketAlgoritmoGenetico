g++ main.cpp -o main
if %errorlevel% neq 0 (
    echo ERROR DE COMPILACION
    exit /b
)    
main