import sys
import os
import subprocess
import time

def inFilter(s):
    return s.find("in") != -1

def runTest(ej, file):
    print(f"Ejecutando ejercicio: {ej} prueba {file}\n")
    print(f"Ejecutado " + "g++ " +f"ejercicio{ej}.cpp " + "-g " + "-std=c++11")
    process = subprocess.run(["g++", f"ejercicio{ej}.cpp", "-g", "-std=c++11"])
    if process.returncode != 0:
        print(f"ERROR: ejercicio{ej}.cpp could not compile")
        print(f"STDERROR:\n {process.stderr}")
        exit(1)
            
    start = time.perf_counter()

    print(f"Ejecutado " +"./a.out " + "< " + f"Prueba/ejercicio{ej}/{file}.in.txt " + "> " + f"Prueba/ejercicio{ej}/{file}.myOut.txt")
    with open(f"Prueba/ejercicio{ej}/{file}.in.txt") as input_file, open(f"Prueba/ejercicio{ej}/{file}.myOut.txt", "w") as output_file:
        process = subprocess.run(["./a.out"], stdin=input_file, stdout=output_file)

    end = time.perf_counter()
    print(f"Duro {(end - start):.6f} seconds")

    print(f"Ejecutado " +"diff " + f"Prueba/ejercicio{ej}/{file}.myOut.txt " + f"Prueba/ejercicio{ej}/{file}.out.txt ")
    process = subprocess.run(["diff", f"Prueba/ejercicio{ej}/{file}.myOut.txt", f"Prueba/ejercicio{ej}/{file}.out.txt"])
    if process.returncode != 0:
        print(f"ERROR: ejercicio{ej}.cpp diff")
        print(f"STDOUT:\n {process.stdout}")
        print(f"STDERROR:\n {process.stderr}")
        exit(1)

    print("\n")

def main():
    program_name, *argv = sys.argv

    if len(argv) == 0:
        print(f'Uso: {program_name} [1..10]')
        print('ERROR: Numero de ejercicio no ingresado')
        exit(1)
    
    ej = int(argv[0])

    if ej <= 0 or ej > 10:
        print(f'Uso: {program_name} [1..10]')
        print('ERROR: Numero de ejercicio no ingresado')
        exit(1)
    
    dir_list = sorted(os.listdir(f"Prueba/ejercicio{ej}"))
    c = filter(inFilter, dir_list)
    for s in c:
        runTest(ej, s.split(".")[0])


if __name__ == '__main__':
    main()
