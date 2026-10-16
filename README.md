# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

> **Grupo:** 14
>
> **Integrantes:**
> - Lorenzo Vitale — [@lvitale12](https://github.com/lvitale12)
> - Mateo Agustin Gongora — [@magongora](https://github.com/magongora)
> - Tomas Genovesi — [@TomasGenovesi](https://github.com/TomasGenovesi)
> - Guadalupe Erli — [@lupe-erli](https://github.com/lupe-erli)

La consigna completa está en **`enunciado.pdf`**. El proyecto implementa el
circuito de normalización, carga de ventas, cierre semanal y resumen solicitado.

## Estructura del repo

```
.
├── enunciado.pdf              # la consigna
├── datos/                     # archivos PROVISTOS por la cátedra (no se tocan)
│   ├── comandas_historicas.dat
│   ├── inventario.dat
│   └── dump_datos_de_prueba.txt   # los dos .dat en texto, para verificar la lectura
├── .gitignore
│
├── normalizacion.cpp          # cada uno con su propio main; NO hay main.cpp
├── ventas.cpp
├── cierre.cpp
└── resumen.cpp
```

El repositorio contiene los cuatro programas en la raíz, cada uno con su propio
`main`, como se indica en el diagrama del enunciado.

## Compilación y ejecución

Los programas leen y escriben archivos `.dat` en la carpeta donde se ejecutan.
Los archivos de `datos/` son los **originales** y no se modifican: copiálos a la
carpeta donde vas a compilar y correr, y trabajá sobre esas copias.

```bash
cp datos/comandas_historicas.dat datos/inventario.dat .
```

Compilar y correr cada programa (cada uno tiene su propio `main`, no hay `main.cpp`):

```bash
g++ -std=c++17 -O2 -o normalizacion normalizacion.cpp
g++ -std=c++17 -O2 -o ventas        ventas.cpp
g++ -std=c++17 -O2 -o cierre        cierre.cpp
g++ -std=c++17 -O2 -o resumen       resumen.cpp
```

Orden de ejecución (cada programa deja los archivos que usa el siguiente):

```
./normalizacion     # historicas + inventario  ->  mozos.dat, comandas_dd-mm-aaaa.dat (varios), inventario actualizado
./ventas            # carga ventas de una fecha       ->  agrega a la planilla del día
./cierre            # junta los días de la semana      ->  comandas_semana_sX-mm.dat
./resumen           # imprime el resumen por mozo + total del buffet
```

Entradas interactivas:

- `ventas` pide la fecha en formato `DD-MM-AAAA` y luego permite cargar ventas.
- `cierre` pide el identificador semanal `sN-MM` (por ejemplo, `s1-06`) y el año.
- `resumen` pide el nombre completo del archivo semanal generado por `cierre`.

Para repetir el circuito desde cero hay que volver a copiar
`datos/inventario.dat` a la raíz antes de ejecutar `normalizacion`, porque ese
programa descuenta del inventario las comandas históricas.

## Distribución del trabajo

- **Normalización:** Lorenzo Vitale.
- **Ventas:** Tomas Genovesi.
- **Cierre semanal:** Guadalupe Erli.
- **Resumen:** Mateo Agustin Gongora.
- **Integración y pruebas:** trabajo grupal.

## Decisiones de implementación

- Los registros persistentes usan estructuras de tamaño fijo y campos `char[]`.
- Los IDs de los mozos se asignan desde 1 según su primera aparición en el
  histórico y quedan guardados en ese orden.
- En `mozos.dat`, la clave inicial se guarda aplicando un corrimiento `K = 5` a
  los caracteres del ID. Para ingresar en `ventas`, el mozo escribe su ID sin
  transformar y el programa aplica el mismo corrimiento antes de comparar.
- La fecha queda representada por el nombre del archivo diario; cada registro
  diario contiene solamente la `Comanda` con el `idMozo`.
- El inventario se busca por código mediante búsqueda binaria y se actualiza en
  el mismo archivo.
- Las comandas diarias se ordenan por `idMozo` para que cierre pueda aparearlas
  y generar el archivo semanal.
- El resumen semanal calcula las cantidades y comisiones directamente desde las
  comandas agrupadas por `idMozo`.

## Verificar que leen bien el binario

`datos/dump_datos_de_prueba.txt` tiene el contenido de los dos archivos provistos
en texto legible. Si tu programa imprime otra cosa al leerlos, revisá los `struct`
(orden de campos / tamaños): tienen que dar `sizeof(ComandaHistorica)=76` y
`sizeof(Producto)=64`.

## Contenido de la entrega

- Los cuatro programas `.cpp` están en la raíz y tienen su propio `main`.
- Los archivos binarios de `datos/` son los originales provistos por la cátedra.
- Los `.dat` generados durante la ejecución y los ejecutables quedan excluidos
  mediante `.gitignore`.
