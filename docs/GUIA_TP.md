# Guía general del TP

Esta guía explica qué hay que construir, cómo se relacionan los cuatro programas y en qué orden conviene trabajar. No reemplaza al [enunciado oficial](../enunciado.pdf).

## Objetivo

El TP implementa el procesamiento de ventas del buffet mediante archivos binarios. Parte de comandas históricas desordenadas y un inventario, incorpora ventas nuevas, realiza un cierre semanal y genera un resumen por mozo.

```text
comandas_historicas.dat + inventario.dat
                    │
                    ▼
           normalizacion.cpp
                    │
        ┌───────────┼─────────────┐
        ▼           ▼             ▼
    mozos.dat   comandas diarias  inventario actualizado
        │           │             │
        └───────────┼─────────────┘
                    ▼
               ventas.cpp
                    │
                    ▼
        comandas diarias actualizadas
                    │
                    ▼
               cierre.cpp
                    │
                    ▼
          comandas de la semana
                    │
                    ▼
               resumen.cpp
                    │
                    ▼
       informe por mozo + total general
```

## Archivos principales

| Archivo | Quién lo provee o genera | Contenido |
| --- | --- | --- |
| `datos/comandas_historicas.dat` | Cátedra | Ventas históricas desordenadas, con nombre completo del mozo |
| `datos/inventario.dat` | Cátedra | Productos ordenados por código, con precio y stock |
| `mozos.dat` | Normalización | Mozos únicos, ID, clave transformada y comisión acumulada |
| `comandas_dd-mm-aaaa.dat` | Normalización y ventas | Comandas del día ordenadas por `idMozo` |
| `comandas_semana_sX-mm.dat` | Cierre | Comandas de la semana ordenadas por `idMozo` |

Los archivos originales de `datos/` nunca se modifican. Para ejecutar los programas se copian a la raíz y se trabaja sobre esas copias. Los `.dat` generados no se versionan.

## 1. Normalización

`normalizacion.cpp` transforma los datos históricos al formato que utilizará el sistema.

Debe:

1. Leer `comandas_historicas.dat`.
2. Detectar los mozos distintos.
3. Asignar un `idMozo` a cada uno.
4. Acumular su comisión histórica.
5. Crear `mozos.dat` con una contraseña inicial transformada.
6. Convertir cada registro histórico a una `Comanda`, reemplazando el nombre por el ID.
7. Separar las comandas por fecha.
8. Ordenar cada archivo diario por `idMozo`.
9. Descontar del inventario las cantidades históricas.

Es el programa más complejo porque genera casi todos los archivos que usan los demás.

## 2. Ventas

`ventas.cpp` registra ventas nuevas.

Debe pedir la fecha una sola vez y, para cada operación:

1. Pedir ID y contraseña del mozo.
2. Validarlos contra `mozos.dat`.
3. Pedir el código del producto y buscarlo en `inventario.dat`.
4. Validar que la cantidad sea positiva y exista stock.
5. Calcular la comisión:

```cpp
comision = precio * cantidad * TASA_COMISION;
```

6. Descontar el stock.
7. Agregar la comanda al archivo del día.
8. Dejar el archivo diario ordenado por `idMozo`.

Si una validación falla, la operación debe cancelarse sin modificar archivos.

## 3. Cierre semanal

`cierre.cpp` combina los archivos diarios que integran una semana.

Como las entradas ya están ordenadas por `idMozo`, la técnica natural es el **apareo**:

1. Leer el siguiente registro de cada entrada.
2. Escribir primero el que tenga menor `idMozo`.
3. Si las claves son iguales, conservar ambos registros.
4. Al terminar una entrada, copiar los registros restantes de la otra.
5. Repetir el proceso hasta incorporar todos los días.

El resultado es `comandas_semana_sX-mm.dat`, también ordenado por `idMozo`. Un día sin archivo no debe hacer fallar todo el cierre.

## 4. Resumen

`resumen.cpp` recorre el archivo semanal y realiza un **corte de control** por `idMozo`.

Para cada mozo debe mostrar:

- cantidad total de unidades vendidas;
- comisión semanal acumulada.

También debe mostrar la cantidad total de unidades vendidas por el buffet.

El algoritmo general es:

1. Leer el primer registro.
2. Guardar su ID como clave actual.
3. Acumular mientras el ID no cambie.
4. Cuando cambia, mostrar el grupo terminado y reiniciar acumuladores.
5. Al llegar al fin del archivo, mostrar explícitamente el último grupo.

Olvidar el último grupo es el error clásico del corte de control.

## Contrato obligatorio

Los cuatro programas se comunican mediante binarios. Por eso tienen que coincidir exactamente en:

- structs, orden de campos y tipos;
- nombres de archivo;
- formato de fecha;
- criterio de orden;
- cálculo de comisión;
- transformación de contraseña.

En particular:

- `sizeof(ComandaHistorica) == 76`;
- `sizeof(Producto) == 64`;
- no usar `std::string` dentro de structs persistidos;
- usar `FILE*`, `fopen`, `fread`, `fwrite`, `fseek` y `fclose`;
- recorrer con `while (fread(...) == 1)`.

Las definiciones y decisiones pendientes están en [CONTRATO_COMUN.md](CONTRATO_COMUN.md).

## Orden de desarrollo recomendado

El orden de ejecución es fijo, pero el desarrollo puede hacerse en paralelo después de cerrar el contrato común:

1. Acordar structs, archivos y decisiones ambiguas.
2. Comprobar que todos pueden compilar con `g++`.
3. Crear un esqueleto compilable de cada programa.
4. Implementar normalización y ventas.
5. Desarrollar cierre y resumen usando archivos pequeños de prueba.
6. Integrar por primera vez la cadena completa.
7. Cubrir casos límite.
8. Probar desde un clon y copias limpias.

```text
copias limpias
→ normalizacion
→ ventas
→ cierre
→ resumen
```

## Criterio de terminado

El TP está listo cuando:

- los cuatro programas compilan por separado;
- normalización crea los mozos y archivos diarios correctos;
- el inventario queda actualizado;
- los archivos diarios y semanal están ordenados;
- ventas valida credenciales, producto, cantidad y stock;
- una operación rechazada no modifica archivos;
- cierre admite días inexistentes o vacíos;
- resumen incluye el último mozo y el total general;
- la contraseña no se guarda en texto plano;
- se manejan errores de apertura y lectura;
- la cadena completa funciona desde datos limpios;
- no se versionan ejecutables ni `.dat` generados;
- cada integrante tiene commits propios y puede explicar su aporte.

Los valores esperados y casos límite están en [PRUEBAS.md](PRUEBAS.md).

## Organización sugerida

Una posibilidad es que cada programa tenga una persona de referencia y que el resto colabore en revisiones, pruebas e integración. Si al grupo le resulta útil, pueden trabajar con ramas cortas y pull requests para conservar el historial. Hay más alternativas en [FLUJO_GIT.md](FLUJO_GIT.md) y un calendario orientativo en [PLAN_DE_TRABAJO.md](PLAN_DE_TRABAJO.md).
