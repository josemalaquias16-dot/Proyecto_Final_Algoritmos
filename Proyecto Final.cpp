#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Estructura para la cuenta bancaria
struct CuentaBancaria {
    string numCuenta;
    string nombreTitular;
    string dpiTitular;
    double saldoMonetario = 0.0;
    double saldoAhorro = 0.0;
    double intereses = 0.0;
    bool tieneMonetaria = false;
    bool tieneAhorro = false;
    bool activo = false;
};

// Estructura para el registro de transacciones
struct TransaccionRegistro {
    string origen;
    string destino;
    string tipoOrg;
    string tipoDst;
    double monto;
    double comision;
};

const int MAX = 100;
const double APERTURA_AHORRO_MIN = 100.0;
const double APERTURA_MONETARIA_MIN = 1000.0;
const double TASA_INTERES = 0.01;

CuentaBancaria misCuentas[MAX];
int totalCuentas = 0;

TransaccionRegistro historial[500];
int totalTransacciones = 0;

// Prototipos de funciones
void cargarArchivo();
void guardarArchivo();
void limpiarConsolaBuffer();

// Algoritmos de búsqueda
int buscarCuenta(string cuentaBuscada);
int buscarPorDPI(string dpiBuscado);

// Módulos principales
void menuAperturas();
void abrirMonetaria();
void abrirAhorro();
void verCuentas();

void menuCaja();
void hacerRetiro();
void hacerTransferencia();
void consultarSaldos();

// Módulo de Reportes (3 reportes requeridos)
void menuReportes();
void ordenarBurbuja();
void generarCorte();
void reporteResumenGeneral();
void reporteSaldosGlobales();

int main() {
    cargarArchivo();

    int opcion = 0;
    do {
        cout << "\n===================================\n";
        cout << "      SISTEMA BANCARIO - CAJA      \n";
        cout << "===================================\n";
        cout << "1. Apertura de Cuentas\n";
        cout << "2. Operaciones de Caja\n";
        cout << "3. Reportes\n";
        cout << "4. Salir del Sistema\n";
        cout << "===================================\n";
        cout << "Elige una opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            limpiarConsolaBuffer();
            cout << "Opcion no valida, intenta de nuevo.\n";
            continue;
        }

        switch (opcion) {
            case 1:
                menuAperturas();
                break;
            case 2:
                menuCaja();
                break;
            case 3:
                menuReportes();
                break;
            case 4:
                cout << "\nSaliendo del programa y guardando...\n";
                break;
            default:
                cout << "Opcion fuera de rango.\n";
        }
    } while (opcion != 4);

    guardarArchivo();
    return 0;
}

void limpiarConsolaBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Criterio de Búsqueda 1: Búsqueda Secuencial por Número de Cuenta
int buscarCuenta(string cuentaBuscada) {
    for (int i = 0; i < totalCuentas; i++) {
        if (misCuentas[i].activo && misCuentas[i].numCuenta == cuentaBuscada) {
            return i;
        }
    }
    return -1;
}

// Criterio de Búsqueda 2: Búsqueda Secuencial por DPI del Cliente
int buscarPorDPI(string dpiBuscado) {
    for (int i = 0; i < totalCuentas; i++) {
        if (misCuentas[i].activo && misCuentas[i].dpiTitular == dpiBuscado) {
            return i;
        }
    }
    return -1;
}

void menuAperturas() {
    int op = 0;
    do {
        cout << "\n--- MENU APERTURA DE CUENTAS ---\n";
        cout << "1. Cuenta Monetaria (Minimo Q1,000)\n";
        cout << "2. Cuenta de Ahorro (Minimo Q100)\n";
        cout << "3. Ver listado de cuentas\n";
        cout << "4. Regresar al menu principal\n";
        cout << "Selecciona: ";
        cin >> op;

        if (cin.fail()) {
            limpiarConsolaBuffer();
            continue;
        }

        if (op == 1) {
            abrirMonetaria();
        } else if (op == 2) {
            abrirAhorro();
        } else if (op == 3) {
            verCuentas();
        }
    } while (op >= 1 && op <= 3);
}

void abrirMonetaria() {
    string cuenta;
    cout << "\nNumero de cuenta: ";
    cin >> cuenta;

    int pos = buscarCuenta(cuenta);
    if (pos != -1) {
        // Control de duplicados si la modalidad ya está activa
        if (misCuentas[pos].tieneMonetaria) {
            cout << "[DUPLICADO DENEGADO] Esta cuenta ya posee una Cuenta Monetaria activa.\n";
            return;
        }
        double monto;
        cout << "Deposito inicial (Minimo Q1000): Q";
        cin >> monto;
        while (monto < APERTURA_MONETARIA_MIN) {
            cout << "Monto menor al minimo. Ingresa de nuevo: Q";
            cin >> monto;
        }
        misCuentas[pos].saldoMonetario = monto;
        misCuentas[pos].tieneMonetaria = true;
        cout << "Cuenta monetaria agregada con exito al cliente " << misCuentas[pos].nombreTitular << ".\n";
    } else {
        if (totalCuentas >= MAX) {
            cout << "Ya no caben mas cuentas.\n";
            return;
        }
        CuentaBancaria nuev;
        nuev.numCuenta = cuenta;
        limpiarConsolaBuffer();
        cout << "Nombre del titular: ";
        getline(cin, nuev.nombreTitular);
        cout << "DPI: ";
        cin >> nuev.dpiTitular;

        cout << "Monto inicial (Minimo Q1000): Q";
        cin >> nuev.saldoMonetario;
        while (nuev.saldoMonetario < APERTURA_MONETARIA_MIN) {
            cout << "Invalido. Cuanto vas a depositar?: Q";
            cin >> nuev.saldoMonetario;
        }
        nuev.tieneMonetaria = true;
        nuev.activo = true;

        misCuentas[totalCuentas] = nuev;
        totalCuentas++;
        cout << "Cuenta monetaria creada correctamente.\n";
    }
    guardarArchivo();
}

void abrirAhorro() {
    string cuenta;
    cout << "\nNumero de cuenta: ";
    cin >> cuenta;

    int pos = buscarCuenta(cuenta);
    if (pos != -1) {
        // Control de duplicados
        if (misCuentas[pos].tieneAhorro) {
            cout << "[DUPLICADO DENEGADO] El cliente ya posee una Cuenta de Ahorro activa.\n";
            return;
        }
        double monto;
        cout << "Deposito inicial (Minimo Q100): Q";
        cin >> monto;
        while (monto < APERTURA_AHORRO_MIN) {
            cout << "Debe ser al menos Q100: Q";
            cin >> monto;
        }
        misCuentas[pos].saldoAhorro = monto;
        misCuentas[pos].intereses = monto * TASA_INTERES;
        misCuentas[pos].tieneAhorro = true;
        cout << "Cuenta de ahorro agregada con exito.\n";
    } else {
        if (totalCuentas >= MAX) {
            cout << "Memoria llena.\n";
            return;
        }
        CuentaBancaria nuev;
        nuev.numCuenta = cuenta;
        limpiarConsolaBuffer();
        cout << "Nombre del titular: ";
        getline(cin, nuev.nombreTitular);
        cout << "DPI: ";
        cin >> nuev.dpiTitular;

        cout << "Monto inicial (Minimo Q100): Q";
        cin >> nuev.saldoAhorro;
        while (nuev.saldoAhorro < APERTURA_AHORRO_MIN) {
            cout << "Monto bajo. Intenta de nuevo: Q";
            cin >> nuev.saldoAhorro;
        }
        nuev.intereses = nuev.saldoAhorro * TASA_INTERES;
        nuev.tieneAhorro = true;
        nuev.activo = true;

        misCuentas[totalCuentas] = nuev;
        totalCuentas++;
        cout << "Cuenta de ahorro creada.\n";
    }
    guardarArchivo();
}

void verCuentas() {
    cout << "\n--- LISTADO DE CLIENTES ---\n";
    if (totalCuentas == 0) {
        cout << "No hay nada registrado aun.\n";
        return;
    }
    for (int i = 0; i < totalCuentas; i++) {
        cout << "Cuenta: " << misCuentas[i].numCuenta << " | Titular: " << misCuentas[i].nombreTitular << " | DPI: " << misCuentas[i].dpiTitular << "\n";
        cout << " - Monetaria: " << (misCuentas[i].tieneMonetaria ? "Q" + to_string(misCuentas[i].saldoMonetario) : "No tiene") << "\n";
        cout << " - Ahorro:    " << (misCuentas[i].tieneAhorro ? "Q" + to_string(misCuentas[i].saldoAhorro) : "No tiene") << "\n";
        cout << "-----------------------------------\n";
    }
}

void menuCaja() {
    int op = 0;
    do {
        cout << "\n--- MENU OPERACIONES DE CAJA ---\n";
        cout << "1. Retiro de efectivo\n";
        cout << "2. Transferencia entre cuentas\n";
        cout << "3. Consultar saldos\n";
        cout << "4. Regresar\n";
        cout << "Elige: ";
        cin >> op;

        if (op == 1) hacerRetiro();
        else if (op == 2) hacerTransferencia();
        else if (op == 3) consultarSaldos();
    } while (op >= 1 && op <= 3);
}

void hacerRetiro() {
    string cuenta;
    cout << "Numero de cuenta: ";
    cin >> cuenta;

    int idx = buscarCuenta(cuenta);
    if (idx == -1) {
        cout << "Cuenta no encontrada.\n";
        return;
    }

    int tipo;
    cout << "De donde retira?\n1. Monetaria\n2. Ahorro\nOpcion: ";
    cin >> tipo;

    double monto;
    cout << "Monto a retirar: Q";
    cin >> monto;

    if (tipo == 1) {
        if (!misCuentas[idx].tieneMonetaria || misCuentas[idx].saldoMonetario < monto) {
            cout << "Fondos insuficientes o cuenta inactiva.\n";
            return;
        }
        misCuentas[idx].saldoMonetario -= monto;
    } else if (tipo == 2) {
        if (!misCuentas[idx].tieneAhorro || misCuentas[idx].saldoAhorro < monto) {
            cout << "Fondos insuficientes o cuenta inactiva.\n";
            return;
        }
        misCuentas[idx].saldoAhorro -= monto;
    } else {
        cout << "Opcion incorrecta.\n";
        return;
    }

    cout << "Retiro exitoso. Entregue el efectivo.\n";
    guardarArchivo();
}

void hacerTransferencia() {
    string c1, c2;
    cout << "Cuenta origen: ";
    cin >> c1;
    cout << "Cuenta destino: ";
    cin >> c2;

    int o = buscarCuenta(c1);
    int d = buscarCuenta(c2);

    if (o == -1 || d == -1) {
        cout << "Alguna de las cuentas no existe.\n";
        return;
    }

    int tOrg, tDst;
    cout << "Tipo origen (1. Ahorro, 2. Monetaria): ";
    cin >> tOrg;
    cout << "Tipo destino (1. Ahorro, 2. Monetaria): ";
    cin >> tDst;

    if (tOrg == 1 && !misCuentas[o].tieneAhorro) { cout << "Origen sin ahorro.\n"; return; }
    if (tOrg == 2 && !misCuentas[o].tieneMonetaria) { cout << "Origen sin monetaria.\n"; return; }
    if (tDst == 1 && !misCuentas[d].tieneAhorro) { cout << "Destino sin ahorro.\n"; return; }
    if (tDst == 2 && !misCuentas[d].tieneMonetaria) { cout << "Destino sin monetaria.\n"; return; }

    double monto;
    cout << "Monto a transferir: Q";
    cin >> monto;

    double comisionPorc = 0.0;
    if (tOrg == 1 && tDst == 2) comisionPorc = 0.02; // Ahorro a monetaria 2%
    else if (tOrg == 2 && tDst == 1) comisionPorc = 0.05; // Monetaria a ahorro 5%

    double comision = monto * comisionPorc;
    double totalPagar = monto + comision;

    if (tOrg == 1) {
        if (misCuentas[o].saldoAhorro < totalPagar) {
            cout << "No le alcanza en ahorro (incluyendo comision).\n";
            return;
        }
        misCuentas[o].saldoAhorro -= totalPagar;
    } else {
        if (misCuentas[o].saldoMonetario < totalPagar) {
            cout << "No le alcanza en monetaria (incluyendo comision).\n";
            return;
        }
        misCuentas[o].saldoMonetario -= totalPagar;
    }

    if (tDst == 1) {
        misCuentas[d].saldoAhorro += monto;
        misCuentas[d].intereses += (monto * TASA_INTERES);
    } else {
        misCuentas[d].saldoMonetario += monto;
    }

    if (totalTransacciones < 500) {
        TransaccionRegistro tr;
        tr.origen = c1;
        tr.destino = c2;
        tr.tipoOrg = (tOrg == 1) ? "Ahorro" : "Monetaria";
        tr.tipoDst = (tDst == 1) ? "Ahorro" : "Monetaria";
        tr.monto = monto;
        tr.comision = comision;
        historial[totalTransacciones] = tr;
        totalTransacciones++;
    }

    cout << "Transferencia realizada con exito. Comision cobrada: Q" << comision << "\n";
    guardarArchivo();
}

void consultarSaldos() {
    cout << "\nSeleccione el criterio de busqueda:\n";
    cout << "1. Buscar por Numero de Cuenta\n";
    cout << "2. Buscar por DPI\n";
    cout << "Opcion: ";
    int criterio;
    cin >> criterio;

    int idx = -1;
    if (criterio == 1) {
        string cuenta;
        cout << "Numero de cuenta: ";
        cin >> cuenta;
        idx = buscarCuenta(cuenta);
    } else if (criterio == 2) {
        string dpi;
        cout << "DPI del titular: ";
        cin >> dpi;
        idx = buscarPorDPI(dpi);
    } else {
        cout << "Opcion de busqueda invalida.\n";
        return;
    }

    if (idx == -1) {
        cout << "Cliente o cuenta no encontrada.\n";
        return;
    }

    cout << "\n--- ESTADO DE CUENTA ---\n";
    cout << "Titular: " << misCuentas[idx].nombreTitular << "\n";
    cout << "DPI: " << misCuentas[idx].dpiTitular << "\n";
    cout << "Numero de Cuenta: " << misCuentas[idx].numCuenta << "\n";
    cout << "Monetaria: Q" << (misCuentas[idx].tieneMonetaria ? to_string(misCuentas[idx].saldoMonetario) : "N/A") << "\n";
    cout << "Ahorro:    Q" << (misCuentas[idx].tieneAhorro ? to_string(misCuentas[idx].saldoAhorro) : "N/A") << "\n";
    cout << "Intereses acumulados (Ahorro): Q" << misCuentas[idx].intereses << "\n";
    cout << "------------------------\n";
}

// Módulo de Reportes con 3 reportes diferenciados
void menuReportes() {
    int op = 0;
    do {
        cout << "\n--- MENU DE REPORTES SISTEMA BANCARIO ---\n";
        cout << "1. Reporte 1: Corte de Caja Diario (Ordenado por Monto)\n";
        cout << "2. Reporte 2: Resumen General de Cuentas y Clientes\n";
        cout << "3. Reporte 3: Balance Global de Saldos e Intereses\n";
        cout << "4. Regresar al Menu Principal\n";
        cout << "Opcion: ";
        cin >> op;

        if (cin.fail()) {
            limpiarConsolaBuffer();
            continue;
        }

        switch (op) {
            case 1: generarCorte(); break;
            case 2: reporteResumenGeneral(); break;
            case 3: reporteSaldosGlobales(); break;
            case 4: break;
            default: cout << "Opcion invalida.\n";
        }
    } while (op >= 1 && op <= 3);
}

void ordenarBurbuja() {
    for (int i = 0; i < totalTransacciones - 1; i++) {
        for (int j = 0; j < totalTransacciones - i - 1; j++) {
            if (historial[j].monto < historial[j + 1].monto) {
                TransaccionRegistro temporal = historial[j];
                historial[j] = historial[j + 1];
                historial[j + 1] = temporal;
            }
        }
    }
}

// Reporte 1: Corte Diario de Caja
void generarCorte() {
    cout << "\n=========================================\n";
    cout << "    REPORTE 1: CORTE DE CAJA DIARIO      \n";
    cout << "=========================================\n";
    cout << "Total transferencias hechas: " << totalTransacciones << "\n";

    double totalComisiones = 0;
    for (int i = 0; i < totalTransacciones; i++) {
        totalComisiones += historial[i].comision;
    }
    cout << "Ganancia total por comisiones: Q" << fixed << setprecision(2) << totalComisiones << "\n";

    if (totalTransacciones > 0) {
        ordenarBurbuja();
        cout << "\nDetalle (ordenado por monto de mayor a menor):\n";
        cout << left << setw(10) << "Origen" << setw(10) << "Destino" << setw(12) << "Monto" << setw(10) << "Comision" << "\n";
        cout << "-----------------------------------------\n";
        for (int i = 0; i < totalTransacciones; i++) {
            cout << left << setw(10) << historial[i].origen
                 << setw(10) << historial[i].destino
                 << setw(12) << historial[i].monto
                 << setw(10) << historial[i].comision << "\n";
        }
    } else {
        cout << "Aun no hay transferencias registradas el dia de hoy.\n";
    }
    cout << "=========================================\n";
}

// Reporte 2: Resumen General de Cuentas y Estado de Clientes
void reporteResumenGeneral() {
    cout << "\n=========================================\n";
    cout << "   REPORTE 2: RESUMEN GENERAL DE CLIENTES\n";
    cout << "=========================================\n";
    cout << "Total de clientes registrados: " << totalCuentas << "\n";

    int totalMonetarias = 0, totalAhorros = 0;
    for (int i = 0; i < totalCuentas; i++) {
        if (misCuentas[i].tieneMonetaria) totalMonetarias++;
        if (misCuentas[i].tieneAhorro) totalAhorros++;
    }
    cout << "Cuentas Monetarias activas: " << totalMonetarias << "\n";
    cout << "Cuentas de Ahorro activas: " << totalAhorros << "\n";
    cout << "-----------------------------------------\n";
}

// Reporte 3: Balance Global de Saldos e Intereses Acumulados
void reporteSaldosGlobales() {
    cout << "\n=========================================\n";
    cout << "   REPORTE 3: BALANCE GLOBAL BANCARIO    \n";
    cout << "=========================================\n";
    double sumaMonetaria = 0.0, sumaAhorro = 0.0, sumaIntereses = 0.0;

    for (int i = 0; i < totalCuentas; i++) {
        sumaMonetaria += misCuentas[i].saldoMonetario;
        sumaAhorro += misCuentas[i].saldoAhorro;
        sumaIntereses += misCuentas[i].intereses;
    }

    cout << "Total depositado en Cuentas Monetarias: Q" << fixed << setprecision(2) << sumaMonetaria << "\n";
    cout << "Total depositado en Cuentas de Ahorro:    Q" << sumaAhorro << "\n";
    cout << "Total de Intereses Generados a Clientes:  Q" << sumaIntereses << "\n";
    cout << "Grand Total Custodiado por el Banco:      Q" << (sumaMonetaria + sumaAhorro) << "\n";
    cout << "=========================================\n";
}

void guardarArchivo() {
    ofstream archivo("datos_banco.txt");
    if (!archivo.is_open()) return;

    archivo << totalCuentas << "\n";
    for (int i = 0; i < totalCuentas; i++) {
        archivo << misCuentas[i].numCuenta << "\n"
                << misCuentas[i].nombreTitular << "\n"
                << misCuentas[i].dpiTitular << "\n"
                << misCuentas[i].saldoMonetario << "\n"
                << misCuentas[i].saldoAhorro << "\n"
                << misCuentas[i].intereses << "\n"
                << misCuentas[i].tieneMonetaria << "\n"
                << misCuentas[i].tieneAhorro << "\n";
    }
    archivo.close();
}

void cargarArchivo() {
    ifstream archivo("datos_banco.txt");
    if (!archivo.is_open()) return;

    archivo >> totalCuentas;
    for (int i = 0; i < totalCuentas; i++) {
        archivo >> misCuentas[i].numCuenta;
        archivo.ignore();
        getline(archivo, misCuentas[i].nombreTitular);
        archivo >> misCuentas[i].dpiTitular;
        archivo >> misCuentas[i].saldoMonetario;
        archivo >> misCuentas[i].saldoAhorro;
        archivo >> misCuentas[i].intereses;
        archivo >> misCuentas[i].tieneMonetaria;
        archivo >> misCuentas[i].tieneAhorro;
        misCuentas[i].activo = true;
    }
    archivo.close();
}