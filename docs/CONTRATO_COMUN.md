# Contrato común entre programas

Estas definiciones deben conservar el mismo orden, tipo y tamaño en todos los programas que las utilicen:

```cpp
struct ComandaHistorica {
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const float TASA_COMISION = 0.10f;
```

Controles obligatorios:

- `sizeof(ComandaHistorica) == 76`.
- `sizeof(Producto) == 64`.
- Registros persistidos con `char[N]`, nunca `std::string`.
- Recorrido seguro con `while (fread(...) == 1)`.
- Archivos mediante `FILE*`, `fopen`, `fread`, `fwrite`, `fseek` y `fclose`.

## Nombres de archivo

- `mozos.dat`
- `comandas_dd-mm-aaaa.dat`
- `comandas_semana_sX-mm.dat`
- `inventario.dat`

## Decisiones que deben cerrarse antes de programar

- Cómo se asignan los `idMozo`.
- Valor `K` y operación reversible para las contraseñas.
- Cómo recibe `cierre.cpp` las fechas de la semana.
- Qué hace `normalizacion.cpp` si sus archivos de salida ya existen.
- Si `ventas.cpp` también actualiza `Mozo.totalComision`.
- Qué se hace ante cantidades cero, negativas o superiores al stock.

Para evitar incompatibilidades, conviene comunicar al resto cualquier cambio en estas decisiones antes de integrarlo.
