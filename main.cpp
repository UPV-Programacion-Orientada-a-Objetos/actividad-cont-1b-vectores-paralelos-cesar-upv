#include <iostream>
#include <iomanip> // para hacer bonitos los mensajes
#include <fstream>

int PRODUCTO_ACTUAL = 0;
const int MAX_SIZE = 100;
int codigos[MAX_SIZE] = {};
std::string nombres[MAX_SIZE] = {};
int stocks[MAX_SIZE] = {};
float precios[MAX_SIZE] = {};
std::string ubicaciones[MAX_SIZE] = {};

void consultar_producto(void); // funciones principales
void actualizar_inventario(void);
void crear_reporte(void);
void buscar_producto_mas_caro(void);
void agregar_nuevo_producto(void);
void buscar_producto_mas_barato(void);
void crear_reporte_bajo_stock(void);

int index_por_codigo(int busqueda); // funciones auxiliares
void cambiar_stock(int index, int stock_cambio, bool necesita_confirmacion);
int preguntar_int(std::string pregunta);
int preguntar_float(std::string pregunta);
int obtener_index_producto(std::string mensaje);
bool confirmar_accion(void);
void guardar_producto(std::string linea);

int obtener_ubicacion_producto(std::string mensaje);
int index_por_ubicacion(std::string busqueda);
void guardar_archivo(void);

int main(int argc, char *argv[]) {

    // LEEER CONTENIDO DEL ARCHIVO
    std::string nombre_archivo = "data/inventario.txt";
    std::ifstream lector_archivos (nombre_archivo);
    
    if (lector_archivos.is_open()) {
        std::cout << "Cargando inventario desde \"inventario.txt\". \n\n";
        std::string linea;
        if (lector_archivos) {
            std::getline(lector_archivos, linea); // leer la primera linea individual

            while (std::getline(lector_archivos, linea)) {
                guardar_producto(linea);
            }
            std::cout << "Inventario cargado exitosamente. " << PRODUCTO_ACTUAL << " productos encontrados. \n\n";
            lector_archivos.close();
        }
    } else {
        std::cout << "\nError: archivo \"inventario.txt\" no encontrado. \n\n";
        std::cout << "Iniciando en ceros. \n\n";
    }

    // especificar que todos los floats se impriman SIN notación cientifica y con dos decimales
    std::cout << std::fixed << std::setprecision(2);

    // menú principal del programa
    int menu_int = -1;
    while (menu_int != 0)
    {
        std::cout << "---- Ferretería \"El Martillo\" ---- \n";
        std::cout << "1 | Consulta de producto \n";
        std::cout << "2 | Actualización de inventario \n";
        std::cout << "3 | Reporte de inventario \n";
        std::cout << "4 | Buscar producto más caro \n";
        std::cout << "5 | Agregar nuevo producto \n";        
        std::cout << "6 | Buscar producto más barato \n";
        std::cout << "7 | Reporte de bajo stock \n";                  
        std::cout << "0 | Salir \n";
        menu_int = preguntar_int("OPCIÓN A REALIZAR: ");

        switch (menu_int)
        {
        case 1:
            consultar_producto();
            break;
        case 2:
            actualizar_inventario();
            break;
        case 3:
            crear_reporte();
            break;
        case 4:
            buscar_producto_mas_caro();
            break;
        case 5:
            agregar_nuevo_producto();
            break;            
        case 6:
            buscar_producto_mas_barato();
            break;
        case 7:
            crear_reporte_bajo_stock();
            break;                        
        case 0:
            guardar_archivo();
            std::cout << "\nSaliendo del programa... \n\n";
            break;
        default:
            std::cout << "\nError: opción inválida. \n\n";
            break;
        }
    }

    return 0;
}

/**
 * func. principal que consulta e imprime de forma extensa la información de un producto.
 */
void consultar_producto()
{
    std::cout << "\n- - CONSULTAR PRODUCTO\n";
    int index = obtener_index_producto("Código del producto a consultar: ");

    if (index != -1)
    {
        std::cout << "\nInformación del Producto: \n";
        std::cout << std::left << std::setw(20) << "Código: " << codigos[index] << std::endl;
        std::cout << std::left << std::setw(19) << "Nombre: " << nombres[index] << std::endl;
        std::cout << std::left << std::setw(19) << "Cantidad en stock: " << stocks[index] << std::endl;
        std::cout << std::left << std::setw(19) << "Precio unitario: " << "$" << precios[index] << "\n\n";
    }
}

/**
 * func. principal que actualiza el stock de un producto, primero obteniendo el producto, y luego
 * llama a otra función que evalua el nuevo stock, pidiendo una confirmación.
 */
void actualizar_inventario()
{
    std::cout << "\n- - ACTUALIZAR INVENTARIO\n";
    int index = obtener_ubicacion_producto("Ubicación del producto a actualizar: ");

    if (index != -1)
    {
        std::string stock_cambio_string;
        std::cout << "\nNombre: " << nombres[index] << " | Stock actual: " << stocks[index] << "\n";
        int stock_cambio = preguntar_int("Ingresar nuevo stock, o stock a eliminar: ");
        cambiar_stock(index, stock_cambio, true);
    }
}

/**
 * func. principal que imprime un reporte de inventario con los detalles de
 * los productos disponibles.
 */
void crear_reporte()
{
    std::cout << "\n------------------ Reporte de Inventario ------------------\n";
    std::cout << std::left << std::setw(8) << "Código" << std::setw(32) << "| Nombre" << std::setw(9) << "| Stock" << std::setw(8) << "| Precio\n";
    std::cout << "-----------------------------------------------------------\n";
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        std::cout << std::left << std::setw(7) << codigos[i];
        std::cout << std::left << "| " << std::setw(30) << nombres[i];
        std::cout << std::left << "| " << std::setw(7) << stocks[i];
        std::cout << std::left << "| " << "$" << precios[i] << std::endl;
    }
    std::cout << "-----------------------------------------------------------\n\n";
}

/**
 * func. principal que recorre todos los productos en el inventario e imprime el más caro,
 * o varios si comparten el mismo precio.
 */
void buscar_producto_mas_caro()
{
    // empezar con el primer elemento de la lista...
    float precio_mayor = precios[0];

    // recorrer los precios hasta encontrar el mayor de todos
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (precios[i] > precio_mayor)
        {
            precio_mayor = precios[i];
        }
    }

    // imprimir todos los productos que tengan este precio
    std::cout << "\nProducto(s) más caros: \n";
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (precio_mayor == precios[i])
        {
            std::cout << nombres[i] << " con un precio de $" << precios[i] << std::endl;
        }
    }
    std::cout << std::endl;
}

void buscar_producto_mas_barato()
{
    // empezar con el primer elemento de la lista...
    float precio_menor = precios[0];

    // recorrer los precios hasta encontrar el mayor de todos
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (precios[i] < precio_menor)
        {
            precio_menor = precios[i];
        }
    }

    // imprimir todos los productos que tengan este precio
    std::cout << "\nProducto(s) más baratos: \n";
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (precio_menor == precios[i])
        {
            std::cout << nombres[i] << " con un precio de $" << precios[i] << std::endl;
        }
    }
    std::cout << std::endl;
}

void crear_reporte_bajo_stock() {
    std::cout << std::endl;
    int umbral = preguntar_int("Ingrese el umbral: ");
    if (umbral < 1) {
        std::cout << "\nError: umbral menor a 1, cancelando operación. \n\n";
        return;
    }

    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (stocks[i] < umbral) {
            break;
        } else if (i == PRODUCTO_ACTUAL - 1)
        {
            std::cout << "\nNo hay ningún producto debajo del umbral. \n\n";
            return;
        }
        
    }

    std::cout << "\n------------------ Reporte de Bajo Stock ------------------\n";
    std::cout << std::left << std::setw(8) << "Código" << std::setw(32) << "| Nombre" << std::setw(9) << "| Stock" << std::setw(8) << "| Precio\n";
    std::cout << "-----------------------------------------------------------\n";
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (stocks[i] < umbral) {
            std::cout << std::left << std::setw(7) << codigos[i];
            std::cout << std::left << "| " << std::setw(30) << nombres[i];
            std::cout << std::left << "| " << std::setw(7) << stocks[i];
            std::cout << std::left << "| " << "$" << precios[i] << std::endl;
        }
    }
    std::cout << "-----------------------------------------------------------\n\n";
}

void agregar_nuevo_producto() {
    std::cout << "\n- - AGREGAR NUEVO PRODUCTO\n";

    int codigo = preguntar_int("Nuevo código: ");
    if (index_por_codigo(codigo) != -1) {
        std::cout << "\nError: código duplicado, cancelando operación. \n\n";
        return;
    } 

    std::string nombre;
    std::cout << "Nuevo nombre: ";
    std::cin >> nombre;

    int stock = preguntar_int("Nuevo stock: ");
    if (stock < 0) {
        std::cout << "\nError: nuevo stock menor a 0, cancelando operación. \n\n";
    }

    float precio = preguntar_float("Nuevo precio: ");
    if (precio < 0) {
        std::cout << "\nError: nuevo precio menor a 0, cancelando operación. \n\n";
    }

    std::string ubicacion;
    std::cout << "Nueva ubicación: ";
    std::cin >> ubicacion;

    if (index_por_ubicacion(ubicacion) != -1) {
        std::cout << "\nError: ubicación duplicada, cancelando operación. \n\n";
        return;
    } 

    codigos[PRODUCTO_ACTUAL] = codigo;
    nombres[PRODUCTO_ACTUAL] = nombre;
    stocks[PRODUCTO_ACTUAL] = stock;
    precios[PRODUCTO_ACTUAL] = precio;
    ubicaciones[PRODUCTO_ACTUAL] = ubicacion;
    PRODUCTO_ACTUAL++;

    std::cout << "\nProducto agregado exitosamente.\n\n";
}

/**
 * func. auxiliar que recorre el array de códigos de acuerdo a un término de búsqueda y retorna un índice válido o de error (-1).
 * @param busqueda código a buscar.
 */
int index_por_codigo(int busqueda)
{
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (codigos[i] == busqueda)
        {
            return i;
        }
    }

    return -1; // error, si no encuentra nada
}

/**
 * func. auxiliar que determina y confirma un cambio de stock.
 * @param index el indice del producto
 * @param stock_cambio el stock a eliminar/añadir
 * @param necesita_confirmacion booleano para determinar si se necesita confirmar la operación (para cambios individuales o masivos)
 */
void cambiar_stock(int index, int stock_cambio, bool necesita_confirmacion)
{
    // si el nuevo stock es inferior a 0...
    if (stocks[index] + stock_cambio < 0)
    {
        std::cout << "\nError: el nuevo stock es menor a 0. \n\n";
        return;
    }

    if (!necesita_confirmacion) {
        stocks[index] += stock_cambio;
    } else {
        // avisar al usuario..
        std::cout << "\nEl nuevo stock de [" << nombres[index] << "] será de: " << (stocks[index] + stock_cambio) << ".\n";

        if (confirmar_accion()) {
            stocks[index] += stock_cambio;
        }
    }
}

/**
 * func. auxiliar que pregunta al usuario repetidamente por un número entero negativo o positivo hasta que ingrese uno válido y lo retorna.
 * @param pregunta cadena de texto de la pregunta hacia el usuario.
 */
int preguntar_int(std::string pregunta)
{
    std::string input_usuario;
    bool es_valido = false;

    do
    {
        std::cout << pregunta;
        std::cin >> input_usuario;

        try
        { // convertir el string a int...
            return std::stoi(input_usuario); 
        }
        catch (const std::invalid_argument &e)
        { // si NO ingresa un número
            std::cout << "\nError: por favor, ingrese un número. \n\n";
        }
        catch (const std::out_of_range &e)
        { // si el número está fuera de los limites de un int
            std::cout << "\nError: valor ingresado fuera del rango. \n\n";
        }
    } while (!es_valido);

    return 0;
}

/**
 * func. auxiliar que le pide al usuario un código de producto y retorna el indice del producto, si es que lo encuentra.
 * @param mensaje la pregunta con la que se pedirá el código.
 */
int obtener_index_producto(std::string mensaje) {
    int busqueda = preguntar_int(mensaje);
    int index = index_por_codigo(busqueda);

    if (index == -1)
    {
        std::cout << "\nError: código no encontrado. \n\n";
    }

    return index;
}

/**
 * func. auxiliar confirmar una operacion al usuario hasta que ingrese una opcion correcta.
 */
bool confirmar_accion() {
    std::string confirmar_cambio;
    std::cout << "Está seguro que desea continuar? [Y/N]: ";
    std::cin >> confirmar_cambio;
    
    while (confirmar_cambio != "y" && confirmar_cambio != "Y" && confirmar_cambio != "n" && confirmar_cambio != "N")
    {
        std::cout << "\nError: opción inválida. Desea continuar? [Y/N]: ";
        std::cin >> confirmar_cambio;
    }

    // terminar operación
    if (confirmar_cambio == "y" || confirmar_cambio == "Y")
    {
        std::cout << "\nAcción realizada con éxito. \n\n";
        return true;
    }
    else
    {
        std::cout << "\nAcción cancelada exitosamente. \n\n";
        return false;
    }
}

void guardar_producto(std::string linea) {
    if (linea.empty()) {
        return;
    }

    std::stringstream s(linea);
    std::string dato;
    int campo = 0;

    while (std::getline(s, dato, ',') && campo < 5) {
        switch (campo) {
            case 0: // codigo
                try {
                    codigos[PRODUCTO_ACTUAL] = std::stoi(dato);
                } catch (const std::exception& e) {
                    std::cout << "Error al convertir código: " << dato << std::endl;
                    return;
                }
                break;
            case 1: // nombre
                nombres[PRODUCTO_ACTUAL] = dato;
                break;
            case 2: // stock
                try {
                    stocks[PRODUCTO_ACTUAL] = std::stoi(dato);
                } catch (const std::exception& e) {
                    std::cout << "Error al convertir stock: " << dato << std::endl;
                    return;
                }
                break;
            case 3: // precio
                try {
                    precios[PRODUCTO_ACTUAL] = std::stof(dato);
                } catch (const std::exception& e) {
                    std::cout << "Error al convertir precio: " << dato << std::endl;
                    return;
                }
                break;
            case 4: // ubi
                ubicaciones[PRODUCTO_ACTUAL] = dato;
                break;
        }
        campo++;
    }

    if (campo == 5) {
        PRODUCTO_ACTUAL++;
    } else {
        std::cout << "Error: línea incompleta en archivo: " << linea << std::endl;
    }
}

int obtener_ubicacion_producto(std::string mensaje) {
    std::string busqueda;

    std::cout << mensaje;
    std::cin >> busqueda;

    int index = index_por_ubicacion(busqueda);

    if (index == -1)
    {
        std::cout << "\nError: ubicación no encontrada. \n\n";
    }

    return index;
}

int index_por_ubicacion(std::string busqueda)
{
    for (int i = 0; i < PRODUCTO_ACTUAL; i++)
    {
        if (ubicaciones[i] == busqueda)
        {
            return i;
        }
    }

    return -1; // error, si no encuentra nada
}

int preguntar_float(std::string pregunta)
{
    std::string input_usuario;
    bool es_valido = false;

    do
    {
        std::cout << pregunta;
        std::cin >> input_usuario;

        try
        { // convertir el string a int...
            return std::stof(input_usuario); 
        }
        catch (const std::invalid_argument &e)
        { // si NO ingresa un número
            std::cout << "\nError: por favor, ingrese un número. \n\n";
        }
        catch (const std::out_of_range &e)
        { // si el número está fuera de los limites de un int
            std::cout << "\nError: valor ingresado fuera del rango. \n\n";
        }
    } while (!es_valido);

    return 0;
}

void guardar_archivo(void) {
    std::string nombre_archivo = "data/inventario.txt";
    std::ofstream escritor_archivos(nombre_archivo);

    if (escritor_archivos.is_open()) {
        std::cout << "Guardando cambios en \"inventario.txt\". \n\n";
        escritor_archivos << "Código,Nombre,Cantidad,Precio,Ubicación\n";
        for (int i = 0; i < PRODUCTO_ACTUAL; i++) {
            escritor_archivos << codigos[i] << "," << nombres[i] << "," << stocks[i] << "," << precios[i] << "," << ubicaciones[i] << "\n";
        }
        escritor_archivos.close();
        std::cout << "Inventario guardado exitosamente. " << PRODUCTO_ACTUAL << " productos escritos. \n\n";
    } else {
        std::cout << "\nError: archivo \"inventario.txt\" no encontrado. Descartando cambios. \n\n";
    }
}
