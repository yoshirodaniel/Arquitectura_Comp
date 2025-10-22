#include <iostream>
#include <bitset>
#include <climits>
#include <cmath>

using namespace std;

// Función para multiplicar dos números en formato de punto flotante y mostrar el resultado en binario
void multiplicacion(float x, float y) {
    // Definir el bias (para exponentes)
    int bias = (1 << 7) - 1;

    // Unión para acceder a los bits de los números en punto flotante
    union {
        float input;
        int bits;
    } data1, data2, result;

    data1.input = x;
    data2.input = y;

    // Extraer los componentes: signo, exponente y significando
    int sign1 = (data1.bits >> 31) & 1;
    int sign2 = (data2.bits >> 31) & 1;

    int exponent1 = ((data1.bits >> 23) & ((1 << 8) - 1)) - bias;
    int exponent2 = ((data2.bits >> 23) & ((1 << 8) - 1)) - bias;

    int significand1 = (data1.bits & ((1 << 23) - 1)) | (1 << 23); // Añadir el bit implícito del significando
    int significand2 = (data2.bits & ((1 << 23) - 1)) | (1 << 23); // Añadir el bit implícito del significando

    // Realizar la multiplicación de los significandos
    long long product = static_cast<long long>(significand1) * significand2;

    // Calcular el exponente del resultado
    int exponent_result = exponent1 + exponent2 + bias;

    // Verificar y corregir la normalización y el redondeo
    if (product & (1LL << 47)) {
        product >>= 1;
        exponent_result++;
    }

    // Verificar el overflow y el underflow en exponentes
    if (exponent_result > (1 << 8) - 1) {
        // Overflow en el exponente
        result.bits = (sign1 ^ sign2) << 31 | ((1 << 8) - 1) << 23;
    }

    else if (exponent_result < 0) {
        // Underflow en el exponente
        result.bits = (sign1 ^ sign2) << 31;
    }

    else {
        // Resultado final
        result.bits = (sign1 ^ sign2) << 31 | (exponent_result + bias) << 23 | ((product >> 24) & ((1 << 23) - 1));
    }

    // Extraer la parte binaria del resultado
    int resultado_binario = result.bits;

    // Mostrar solo el resultado en bits
    cout << "Resultado en binario:" << bitset<23>(resultado_binario) << endl;
}

int main() {
    float x, y;
    cout << "Ingresa el primer número float: ";
    cin >> x;
    cout << "Ingresa el segundo número float: ";
    cin >> y;

    multiplicacion(x, y);

    return 0;
}
