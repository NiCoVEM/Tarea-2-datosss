#include <iostream>
#include <fstream>
using namespace std;

class Programa {
private:
    int largo_operaciones;
    char* puntero_operaciones;
    int* puntero_salida;
    char* operaciones;
    int* salida;

public:
    Programa(int largo_operaciones, int* salida);
    void ejecutar_operador();
    void mover(char dir);
    void asignar(int valor, int* salida);
    char obtener(int index);
    void terminar_programa();
    void ejecutar();
    void cargarOperaciones(const string& ops);
};

Programa::Programa(int largo_operaciones, int* salida) : largo_operaciones(largo_operaciones), salida(salida) {
    operaciones = new char[largo_operaciones];  // Inicializa el arreglo de operaciones
    puntero_operaciones = operaciones;          // Inicializa el puntero a la primera operación
    puntero_salida = salida;                    // Inicializa el puntero de salida
}

void Programa::cargarOperaciones(const string& ops) {
    for (int i = 0; i < largo_operaciones; ++i) {
        operaciones[i] = ops[i];  // Carga las operaciones desde la cadena proporcionada
    }
}

void Programa::mover(char dir) {
    if (dir == '>') {
        puntero_salida++;
    } else if (dir == '<') {
        puntero_salida--;
    }
}

void Programa::asignar(int valor, int* salida) {
    *salida = valor;
}

char Programa::obtener(int index) {
    return operaciones[index];
}

void Programa::ejecutar_operador() {
    switch (*puntero_operaciones) {
        case '>':
            mover('>');
            break;
        case '<':
            mover('<');
            break;
        case '+':
            ++(*puntero_salida);
            break;
        case '-':
            --(*puntero_salida);
            break;
        case '.':
            cout << *puntero_salida;
            break;
        case ':':
            if (*puntero_salida == 0) {
                cout << ' ';
            } else if (*puntero_salida >= 1 && *puntero_salida <= 26) {
                cout << static_cast<char>('a' + *puntero_salida - 1);
            } else if (*puntero_salida >= 27 && *puntero_salida <= 52) {
                cout << static_cast<char>('A' + *puntero_salida - 27);
            } else if (*puntero_salida >= 53 && *puntero_salida <= 62) {
                cout << static_cast<char>('0' + *puntero_salida - 53);
            } else {
                cout << static_cast<char>(" .:+-<>[]!"[*puntero_salida % 72 - 63]);
            }
            break;
        case '[':
            if (*puntero_salida == 0) {
                int depth = 1;
                while (depth > 0) {
                    puntero_operaciones++;
                    if (*puntero_operaciones == '[') depth++;
                    if (*puntero_operaciones == ']') depth--;
                }
            }
            break;
        case ']':
            if (*puntero_salida != 0) {
                int depth = 1;
                while (depth > 0) {
                    puntero_operaciones--;
                    if (*puntero_operaciones == ']') depth++;
                    if (*puntero_operaciones == '[') depth--;
                }
            }
            break;
        case '!':
            return;
    }
    puntero_operaciones++;
}

void Programa::terminar_programa() {
    delete[] operaciones;
}

void Programa::ejecutar() {
    puntero_operaciones = operaciones;  // Reiniciar el puntero a la primera operación
    while (*puntero_operaciones != '!') {
        ejecutar_operador();
    }
}

class Interprete {
private:
    int cant_programas;
    int largo_salida;
    int cargado;
    Programa** programas;
    int* salida;

public:
    Interprete(int cant_programas, int largo_salida);
    void cargar_programa(int n);
    void ejecutar_programa();
    void mostrar_programa_cargado();
    void terminar_ejecucion();
};

Interprete::Interprete(int cant_programas, int largo_salida) : cant_programas(cant_programas), largo_salida(largo_salida), cargado(-1) {
    programas = new Programa*[cant_programas];  // Reservar memoria para el arreglo de programas
    salida = new int[largo_salida]();           // Inicializa el arreglo de salida con ceros
}

void Interprete::cargar_programa(int n) {
    if (n >= 0 && n < cant_programas) {
        cargado = n;
        fill(salida, salida + largo_salida, 0);  // Reiniciar salida a ceros
        cout << "Programa " << n << " cargado.\n";
    } else {
        cout << "Índice de programa no válido.\n";
    }
}

void Interprete::ejecutar_programa() {
    if (cargado != -1) {
        programas[cargado]->ejecutar();
    } else {
        cout << "No hay programa cargado.\n";
    }
}

void Interprete::mostrar_programa_cargado() {
    if (cargado != -1) {
        cout << "Programa " << cargado << ": ";
        for (int i = 0; i < largo_salida; i++) {
            cout << programas[cargado]->obtener(i);
        }
        cout << endl;
    } else {
        cout << "No hay programa cargado.\n";
    }
}

void Interprete::terminar_ejecucion() {
    for (int i = 0; i < cant_programas; i++) {
        delete programas[i];
    }
    delete[] programas;
    delete[] salida;
}

int main() {
    ifstream archivo("Programas.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo\n";
        return 1;
    }

    int largo_salida, cant_programas;
    archivo >> largo_salida >> cant_programas;

    Interprete interprete(cant_programas, largo_salida);

    for (int i = 0; i < cant_programas; i++) {
        int largo_operaciones;
        archivo >> largo_operaciones;

        int* salida = new int[largo_salida];
        Programa* programa = new Programa(largo_operaciones, salida);

        string ops;
        archivo >> ops;  // Leer las operaciones como una cadena
        programa->cargarOperaciones(ops);  // Cargar las operaciones en el programa

        interprete.cargar_programa(i);
    }

    archivo.close();

    char comando;
    int n;
    while (true) {
        cout << "> ";
        cin >> comando;
        switch (comando) {
            case 'c':
                cin >> n;
                interprete.cargar_programa(n);
                break;
            case 'e':
                interprete.ejecutar_programa();
                break;
            case 'm':
                interprete.mostrar_programa_cargado();
                break;
            case 's':
                interprete.terminar_ejecucion();
                return 0;
        }
    }
}
